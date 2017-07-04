#include "term.h"
#include "mem.h"
#include "io.h"
#include "str.h"
#include "kernel.h"

#define from_bcd(val)  ((val / 16) * 10 + (val & 0xf))

void cmos_dump(uint16_t * values){
  uint16_t index;
  for (index = 0; index < 128; ++index) {
    outb(0x70, index);
    values[index] = inb(0x71);
  }
}

int get_time(){
  uint16_t hours;
  uint16_t minutes;
  uint16_t seconds;
  uint16_t day;
  uint16_t month;

  hours   = get_hours();
  minutes = get_mins();
  seconds = get_secs();
  month = get_month();
  day   = get_day_of_month();

  return seconds + minutes * 60 + hours * 3600;
}

void sleep(t){ //Actually implement later
  int time = get_time();
  while(get_time() - time < t){}
}

int get_hours(){
	uint16_t values[128]; /* CMOS dump */
  cmos_dump(values);

  uint16_t hours;

  hours   = from_bcd(values[4]);

  return hours;
}

int get_mins(){
	uint16_t values[128]; /* CMOS dump */
  cmos_dump(values);

  uint16_t mins;

  mins   = from_bcd(values[2]);

  return mins;
}

int get_secs(){
	uint16_t values[128]; /* CMOS dump */
  cmos_dump(values);

  uint16_t secs;

  secs   = from_bcd(values[0]);

  return secs;
}

int get_year(){
	uint16_t values[128]; /* CMOS dump */
  cmos_dump(values);

  uint16_t year;

  year   = from_bcd(values[9]);

  return year;
}

int get_month(){
	uint16_t values[128]; /* CMOS dump */
  cmos_dump(values);

  uint16_t month;

  month   = from_bcd(values[8]);

  return month;
}

int get_day_of_month(){
	uint16_t values[128]; /* CMOS dump */
  cmos_dump(values);

  uint16_t date;

  date   = from_bcd(values[7]);

  return date;
}