#ifndef BARBER_H_INCLUDED
#define BARBER_H_INCLUDED
#include "customer.h"
#include <iostream>
#include <memory>

class barber{
    private:
        int cut_time;
        bool is_asleep;

    public:
        std::unique_ptr<customer> current_customer;
        
        barber(int);

        //VARIABLE INTERACTIONS//
        int get_cut_time();
        void set_cut_time(int);
        bool get_is_asleep();
        void set_is_asleep(bool);
};

#endif //BARBER_H_INCLUDED