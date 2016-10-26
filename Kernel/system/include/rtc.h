#ifndef RTC_H_
#define RTC_H_

#define SECONDS 0x00
#define MINUTES 0x02
#define HOURS 	0x04
#define DAY_OF_MONTH 0x07
#define	MONTH 	0x08
#define YEAR 	0x09

/*
 00  RTC seconds
 01  RTC seconds alarm
 02  RTC minutes
 03  RTC minutes alarm
 04  RTC hours
 05  RTC hours alarm
 06  RTC day of week
 07  RTC day of month
 08  RTC month
 09  RTC year
 0A  RTC Status register A:
*/

int get_time(char mode);
 
char * time(void);
char * date(void);

int set_time(unsigned int hour, unsigned int minutes, unsigned int seconds);
int set_date(unsigned int day, unsigned int month, unsigned int year);

void show_time(void);
void hide_time(void);

#endif