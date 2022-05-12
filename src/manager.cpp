#include "manager.h"
#include "barber.h"
#include "customer.h"
#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <queue>

manager::manager(int cut_time_input, int queue_input, int interval_input, int time_input) : barber1(cut_time_input)
{
    queue_size = queue_input, max_customer_interval = interval_input, runtime = time_input;
    customers_end = false;

    std::thread barberThread(&manager::barber, this);
    std::thread customerThread(&manager::customers, this);

    barberThread.join();
    customerThread.join();

    clock_t start = clock();
    while(((clock() - start) / CLOCKS_PER_SEC) <= 2){}
}

void manager::barber()
{
    clock_t start = clock();
    while((((clock() - start) / CLOCKS_PER_SEC) <= runtime) || (customer_queue.size() > 0) || (customers_end == false)){
        std::unique_lock<std::mutex> lock(gLock);
        if(customer_queue.size() == 0){
            barber1.set_is_asleep(true);
            std::cout << "There is no one in the queue. The barber is going to sleep." << std::endl;
            gConditionVariable.wait(lock);
            barber1.set_is_asleep(false);
            if(customers_end == false){
                std::cout << "The customer has woken up the barber." << std::endl;
            }
        }
        else{
            std::cout << "The barber is cutting a person's hair." << std::endl;
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(4));
            std::unique_lock<std::mutex> lock(gLock);
            std::cout << "The customer's hair has been cut.";
            customer_queue.pop();
            if(customer_queue.size() == 1){
                std::cout << " There is now " << customer_queue.size() << " person in the queue." << std::endl;
            }
            else{
                std::cout << " There are now " << customer_queue.size() << " people in the queue." << std::endl;
            }
        }
    }
    std::cout << "There are no more customers, so the barber is finished working for the day." << std::endl;
}
void manager::customers()
{
    clock_t start = clock();
    while(((clock() - start) / CLOCKS_PER_SEC) <= runtime){
        if(true){
            std::unique_lock<std::mutex> lock(gLock);
            if(barber1.get_is_asleep() == true){
                gConditionVariable.notify_one();
            }
            if(customer_queue.size() < queue_size){
                customer_queue.push(customer());
                std::cout << "A new customer has joined the queue.";
                if(customer_queue.size() == 1){
                    std::cout << " There is now " << customer_queue.size() << " person in the queue." << std::endl;
                }
                else{
                    std::cout << " There are now " << customer_queue.size() << " people in the queue." << std::endl;
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
    if(barber1.get_is_asleep() == true){
        gConditionVariable.notify_one();
    }
}
