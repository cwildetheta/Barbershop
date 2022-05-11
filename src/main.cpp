#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <queue>

std::mutex gLock;
std::condition_variable gConditionVariable;
std::queue<bool> customer_queue;
bool is_asleep;

void barber()
{
    while(true){
        std::unique_lock<std::mutex> lock(gLock);
        if(customer_queue.size() == 0){
            is_asleep = true;
            std::cout << "There is no one in the queue. The barber is going to sleep." << std::endl;
            gConditionVariable.wait(lock);
            std::cout << "The customer has woken up the barber." << std::endl;
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
}
void customers()
{
    while(true){
        if(true){
            std::unique_lock<std::mutex> lock(gLock);
            if(is_asleep == true){
                gConditionVariable.notify_one();
            }
            if(customer_queue.size() < 6){
                customer_queue.push(true);
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
        std::this_thread::sleep_for(std::chrono::seconds(rand()%9));
    }
}

int main()
{
    system("cls");

    std::thread barberThread(barber);
    std::thread customerThread(customers);

    barberThread.join();
    customerThread.join();

    return 0;
}