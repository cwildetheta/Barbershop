#ifndef MANAGER_H_INCLUDED
#define MANAGER_H_INCLUDED
#include <condition_variable>
#include <mutex>
#include <queue>
#include "barber.h"
#include "customer.h"

class manager{
    private:
        std::mutex gLock;
        std::condition_variable gConditionVariable;
        barber barber1;
        std::queue<customer> customer_queue;
        int queue_size, max_customer_interval, runtime;
        bool customers_end;

    public:
        manager(int, int, int, int);
        void barber();
        void customers();

        //VARIABLE INTERACTIONS//
};

#endif //MANAGER_H_INCLUDED