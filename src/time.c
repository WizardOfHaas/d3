#include "term.h"
#include "mem.h"
#include "io.h"
#include "str.h"
#include "kernel.h"
#include "time.h"

#define from_bcd(val)  ((val / 16) * 10 + (val & 0xf))

uint16_t days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int t_hz = 100;

void timer_phase(int hz){
  t_hz = hz;
  int divisor = 1193180 / hz;       /* Calculate our divisor */
  outb(0x43, 0x36);             /* Set our command byte 0x36 */
  outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
  outb(0x40, divisor >> 8);     /* Set high byte of divisor */
}

void timer_handler(struct regs *r){
  uptime++;

  if(uptime % t_hz == 0){
  }
}

void init_timer(){
  uptime = 0;
  timer_phase(100);
  irq_install_handler(0, timer_handler);
}

void cmos_dump(uint16_t * values){
  uint16_t index;
  for (index = 0; index < 128; ++index) {
    outb(0x70, index);
    values[index] = inb(0x71);
  }
}

int32_t get_time(){
  uint16_t hours;
  uint16_t minutes;
  uint16_t seconds;
  uint16_t day;
  uint16_t month;
  uint16_t year;

  hours   = get_hours();
  minutes = get_mins();
  seconds = get_secs();
  month = get_month();
  day   = get_day_of_month();
  year = get_year();

  return 
  	(int32_t)(
  		seconds +
  		minutes * 60 +
  		hours * 60 * 60 +
  		day * 24 * 60 * 60 +
  		days_in_month[month] * 24 * 60 * 60 +
  		(year - 2000) * 365.25 * 24 * 60 * 60
  	);
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

  year   = from_bcd(values[9]) + from_bcd(values[50]) * 100;

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