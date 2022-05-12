#include "customer.h"
#include <iostream>

customer::customer(int int_input)
{
    number = int_input;
}

//VARIABLE INTERACTIONS
int customer::get_number()
{
    return number;
}
void customer::set_number(int int_input)
{
    number = int_input;
}