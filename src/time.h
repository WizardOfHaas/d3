#ifndef TIME_H
#define TIME_H

#include <stddef.h>

int get_secs();
int get_mins();
int get_hours();
int get_year();
int get_month();
int get_day_of_month();
int32_t get_time();

void sleep(int t);

#endif
