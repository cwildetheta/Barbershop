#include "barber.h"

barber::barber(int int_input)
{
    cut_time = int_input;
    is_asleep = true;
    current_customer = nullptr;
}

//VARIABLE INTERACTIONS//
int barber::get_cut_time() const
{
    return cut_time;
}
void barber::set_cut_time(int int_input)
{
    cut_time = int_input;
}
bool barber::get_is_asleep() const
{
    return is_asleep;
}
void barber::set_is_asleep(bool bool_input)
{
    is_asleep = bool_input;
}