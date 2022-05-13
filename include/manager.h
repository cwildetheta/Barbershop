#ifndef MANAGER_H_INCLUDED
#define MANAGER_H_INCLUDED
#include <condition_variable>
#include <mutex>
#include <thread>
#include <queue>
#include <vector>
#include <memory>
#include "barber.h"
#include "customer.h"

class manager{
    private:
        std::mutex gLock;
        std::condition_variable gConditionVariable;
        std::vector<std::shared_ptr<barber>> barber_vector;
        std::vector<std::shared_ptr<std::thread>> barber_thread_vector;
        std::queue<std::unique_ptr<customer>> customer_queue;
        int queue_size, max_customer_interval, runtime, customers_so_far, barber_number;
        bool customers_end;

    public:
        manager(int, int, int, int, int);
        void gui();
        void barbers(int);
        void customers();

        //VARIABLE INTERACTIONS//
};

#endif //MANAGER_H_INCLUDED