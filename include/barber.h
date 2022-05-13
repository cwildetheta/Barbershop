#ifndef BARBER_H_INCLUDED
#define BARBER_H_INCLUDED
#include <iostream>

class barber{
    private:
        int cut_time;
        bool is_asleep;
    public:
        explicit barber(int);

        //VARIABLE INTERACTIONS//
        int get_cut_time() const;
        void set_cut_time(int);
        bool get_is_asleep() const;
        void set_is_asleep(bool);
};

#endif //BARBER_H_INCLUDED