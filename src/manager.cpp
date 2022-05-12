#include "manager.h"
#include "barber.h"
#include "customer.h"
#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <queue>
#include <vector>
#include <memory>
#include <unistd.h>

manager::manager(int cut_time_input, int queue_input, int interval_input, int time_input, int barber_input)
{
    queue_size = queue_input, max_customer_interval = interval_input, runtime = time_input, customers_so_far = 0, barber_number = barber_input, currently_cutting = 0;
    customers_end = false;

    for(int i = 0; i < barber_number; i++){
        barber_vector.push_back(std::make_shared<barber>(cut_time_input));
        barber_thread_vector.push_back(std::make_shared<std::thread>(&manager::barbers, this, i));
        usleep(100000);
    }
    std::thread customerThread(&manager::customers, this);

    customerThread.join();
    std::vector<std::shared_ptr<std::thread>>::iterator i1 = barber_thread_vector.begin();
    for(int i = 0; i < barber_number; i++){
        (*(*i1)).join();
        ++i1;
    }
}

void manager::barbers(int index)
{
    clock_t start = clock();
    while((((clock() - start) / CLOCKS_PER_SEC) <= runtime) || ((customer_queue.size() - currently_cutting) > 0) || (customers_end == false)){
        std::vector<std::shared_ptr<barber>>::iterator ib = barber_vector.begin();
        advance(ib, index);
        std::unique_lock<std::mutex> lock(gLock);
        if((customer_queue.size() - currently_cutting) == 0){
            (*ib)->set_is_asleep(true);
            std::cout << "There is no one in the queue. The barber " << index << " is going to sleep." << std::endl;
            gConditionVariable.wait(lock);
            std::vector<std::shared_ptr<barber>>::iterator ib = barber_vector.begin();
            advance(ib, index);
            (*ib)->set_is_asleep(false);
            if(customers_end == false){
                std::cout << "The customer has woken up the barber " << index << "." << std::endl;
            }
        }
        //std::cout << "The customer queue size is: " << customer_queue.size() << std::endl;
        //if((customer_queue.size() - currently_cutting) != 0){
        else{
            currently_cutting++;
            std::cout << "The barber " << index << " is cutting a person's hair." << std::endl;
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(4));
            std::unique_lock<std::mutex> lock(gLock);
            std::cout << "The customer " << customer_queue.front().get_number() << "'s hair has been cut.";
            customer_queue.pop();
            currently_cutting--;
            if((customer_queue.size() - currently_cutting) == 1){
                std::cout << " There is now " << (customer_queue.size() - currently_cutting) << " person in the queue." << std::endl;
            }
            else{
                std::cout << " There are now " << (customer_queue.size() - currently_cutting) << " people in the queue." << std::endl;
            }
        }
    }
    std::cout << "There are no more customers, so the barber " << index << " is finished working for the day." << std::endl;
}
void manager::customers()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    clock_t start = clock();
    while(((clock() - start) / CLOCKS_PER_SEC) <= runtime){
        if(true){
            std::unique_lock<std::mutex> lock(gLock);
            int barber_increment = 0;
            bool has_woken = false;
            std::vector<std::shared_ptr<barber>>::iterator iw = barber_vector.begin();
            while((barber_increment < barber_number) && (has_woken == false)){
                if((*iw)->get_is_asleep() == true){
                    has_woken = true;
                    gConditionVariable.notify_one();
                }
                else{
                    barber_increment++;
                    ++iw;
                }
            }
            if((customer_queue.size() - currently_cutting) < queue_size){
                customers_so_far++;
                customer_queue.push(customer(customers_so_far));
                std::cout << "A new customer has joined the queue. They are customer " << customer_queue.back().get_number() << ". ";
                if(customer_queue.size() == 1){
                    std::cout << " There is now " << (customer_queue.size() - currently_cutting) << " person in the queue." << std::endl;
                }
                else{
                    std::cout << " There are now " << (customer_queue.size() - currently_cutting) << " people in the queue." << std::endl;
                }
            }
            else{
                std::cout << "A new customer has entered, but the queue is full, so they left." << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(rand()%max_customer_interval));
    }
    std::unique_lock<std::mutex> lock(gLock);
    std::cout << "The day has ended, no more customers will arrive." << std::endl;
    customers_end = true;
    std::vector<std::shared_ptr<barber>>::iterator iw = barber_vector.begin();
    for(int i = 0; i < barber_number; i++){
        if((*iw)->get_is_asleep() == true){
            gConditionVariable.notify_one();
        }
        advance(iw, 1);
    }
}