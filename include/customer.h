#ifndef CUSTOMER_H_INCLUDED
#define CUSTOMER_H_INCLUDED

class customer{
    private:
        int number;

    public:
        customer(int);

        //VARIABLE INTERACTIONS
        int get_number();
        void set_number(int);
};

#endif //CUSTOMER_H_INCLUDED