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
#include <iomanip>

manager::manager(int cut_time_input, int queue_input, int interval_input, int time_input, int barber_input)
{
    queue_size = queue_input, max_customer_interval = interval_input, runtime = time_input, customers_so_far = 0, barber_number = barber_input;
    customers_end = false;

    for(int i = 0; i < barber_number; i++){
        barber_vector.push_back(std::make_shared<barber>(cut_time_input));
        barber_thread_vector.push_back(std::make_shared<std::thread>(&manager::barbers, this, i));
        usleep(1000);
    }
    std::thread customerThread(&manager::customers, this);

    customerThread.join();
    std::vector<std::shared_ptr<std::thread>>::iterator i1 = barber_thread_vector.begin();
    for(int i = 0; i < barber_number; i++){
        (*(*i1)).join();
        ++i1;
    }
}

void manager::gui()
{
    system("cls");
    for(int i = 0; i < queue_size; i++){
        std::cout << " " << char(218) << char(196) << char(196) << char(196) << char(191) << " ";
    }
    std::cout << std::endl;
    for(int i = 0; i < queue_size; i++){
        std::cout << " " << char(179);
        if(i < customer_queue.size()){
            if((customer_queue.front()->get_number()+i) < 10){
                std::cout << " " << (customer_queue.front()->get_number() + i) << " ";
            }
            else if((customer_queue.front()->get_number()+i) < 100){
                std::cout << " " << (customer_queue.front()->get_number() + i);
            }
            else{
                std::cout << (customer_queue.front()->get_number() + i);
            }
        }
        else{
            std::cout << "   ";
        }
        std::cout << char(179) << " ";
    }
    std::cout << std::endl;
    for(int i = 0; i < queue_size; i++){
        std::cout << " " << char(192) << char(196) << char(196) << char(196) << char(217) << " ";
    }
    std::cout << std::endl << std::endl;
    for(int i = 0; i < barber_number; i++){
        std::cout << " " << char(218) << char(196) << char(196) << char(196) << char(191) << " ";
    }
    std::cout << std::endl;
    std::vector<std::shared_ptr<barber>>::iterator ib = barber_vector.begin();
    for(int i = 0; i < barber_number; i++){
        std::cout << " " << char(179);
        if((*ib)->current_customer != nullptr){
            if((*ib)->current_customer->get_number() < 10){
                std::cout << " " << (*ib)->current_customer->get_number() << " ";
            }
            else if((*ib)->current_customer->get_number() < 100){
                std::cout << " " << (*ib)->current_customer->get_number();
            }
            else{
                std::cout << (*ib)->current_customer->get_number();
            }
        }
        else{
            std::cout << "   ";
        }
        std::cout << char(179) << " ";
        ++ib;
    }
    std::cout << std::endl;
    for(int i = 0; i < barber_number; i++){
        std::cout << " " << char(192) << char(196) << char(196) << char(196) << char(217) << " ";
    }
    usleep(50000);
}

void manager::barbers(int index)
{
    clock_t start = clock();
    while((((clock() - start) / CLOCKS_PER_SEC) <= runtime) || (customer_queue.size() > 0) || (customers_end == false)){
        std::vector<std::shared_ptr<barber>>::iterator ib = barber_vector.begin();
        advance(ib, index);
        std::unique_lock<std::mutex> lock(gLock);
        if(customer_queue.size() == 0){
            (*ib)->set_is_asleep(true);
            //std::cout << "There is no one in the queue. Barber " << (index + 1) << " is going to sleep." << std::endl;
            gConditionVariable.wait(lock);
            ib = barber_vector.begin();
            advance(ib, index);
            (*ib)->set_is_asleep(false);
            if(customers_end == false){
                //std::cout << "The customer has woken up barber " << (index + 1) << "." << std::endl;
            }
        }
        else{
            (*ib)->current_customer = std::move(customer_queue.front());
            //std::cout << "Barber " << (index + 1) << " is cutting the hair of customer " << (*ib)->current_customer->get_number() << "." << "                ";
            customer_queue.pop();
            if(customer_queue.size() == 1){
                //std::cout << " There is now " << customer_queue.size() << " person in the queue." << std::endl;
            }
            else{
                //std::cout << " There are now " << customer_queue.size() << " people in the queue." << std::endl;
            }
            gui();
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(4));
            lock.lock();
            //std::cout << "Customer " << (*ib)->current_customer->get_number() << "'s hair has been cut." << std::endl;
            (*ib)->current_customer = nullptr;
            gui();
        }
    }
    //std::cout << "There are no more customers, so barber " << (index + 1) << " is finished working for the day." << std::endl;
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
            if(customer_queue.size() < queue_size){
                customers_so_far++;
                customer_queue.push(std::make_unique<customer>(customers_so_far));
                //std::cout << "A new customer has joined the queue. They are customer " << customer_queue.back()->get_number() << ".  ";
                if(customer_queue.size() == 1){
                    //std::cout << " There is now " << customer_queue.size() << " person in the queue." << std::endl;
                }
                else{
                    //std::cout << " There are now " << customer_queue.size() << " people in the queue." << std::endl;
                }
            }
            else{
                //std::cout << "A new customer has entered, but the queue is full, so they left." << std::endl;
            }
            gui();
        }
        std::this_thread::sleep_for(std::chrono::seconds(rand()%max_customer_interval));
    }
    std::unique_lock<std::mutex> lock(gLock);
    //std::cout << "The day has ended, no more customers will arrive." << std::endl;
    customers_end = true;
    std::vector<std::shared_ptr<barber>>::iterator iw = barber_vector.begin();
    for(int i = 0; i < barber_number; i++){
        if((*iw)->get_is_asleep() == true){
            gConditionVariable.notify_one();
        }
        ++iw;
    }
}