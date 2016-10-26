#include <video.h>
#include <rtc.h>
#include <io.h>

#define RTC_OUT	0x70
#define RTC_IN	0x71

/* Taken from https://www.kernel.org/pub/linux/kernel/people/marcelo/linux-2.4/arch/m68k/bvme6000/rtc.c */
#define BCD2BIN(val) (((val)&15) + ((val)>>4)*10)
#define BIN2BCD(val) ((((val)/10)<<4) + (val)%10)

char * _time_ = "hh:mm:ss";
char * _date_ = "dd/mm/yyyy";

int get_time(char mode) {
 	write_port(RTC_OUT, mode);
 	return read_port(RTC_IN);
}

char * time(void) {
	int seconds = get_time(SECONDS);
	int minutes = get_time(MINUTES);
	int hours = get_time(HOURS);

	seconds = BCD2BIN(seconds);
	minutes = BCD2BIN(minutes);
	hours = BCD2BIN(hours);

	_time_[0] = ((hours / 10) % 10) + '0';
	_time_[1] = (hours % 10) + '0';
	_time_[3] = ((minutes / 10) % 10) + '0';
	_time_[4] = (minutes % 10) + '0';
	_time_[6] = ((seconds / 10) % 10) + '0';
	_time_[7] = (seconds % 10) + '0';

	return _time_;
}

char * date() {
	int day = get_time(DAY_OF_MONTH);
	int month = get_time(MONTH);
	int year = get_time(YEAR);

	day = BCD2BIN(day);
	month = BCD2BIN(month);
	year = BCD2BIN(year);
	
	_date_[0] = ((day / 10) % 10) + '0';
	_date_[1] = (day % 10) + '0';
	_date_[3] = ((month / 10) % 10) + '0';
	_date_[4] = (month % 10) + '0';
	_date_[6] = '2';
	_date_[7] = '0';
	_date_[8] = ((year / 10) % 10) + '0';
	_date_[9] = (year % 10) + '0';
	
	return _date_;
}

int set_time(unsigned int hours, unsigned int minutes, unsigned int seconds) {
	write_port_word(RTC_OUT, HOURS);
	write_port_word(RTC_IN, BIN2BCD(hours));

	write_port_word(RTC_OUT, MINUTES);
	write_port_word(RTC_IN, BIN2BCD(minutes));

	write_port_word(RTC_OUT, SECONDS);
	write_port_word(RTC_IN, BIN2BCD(seconds));

	return 0;
}

int set_date(unsigned int day, unsigned int month, unsigned int year) {
	write_port(RTC_OUT, DAY_OF_MONTH);
	write_port(RTC_IN, BIN2BCD(day));

	write_port(RTC_OUT, MONTH);
	write_port(RTC_IN, BIN2BCD(month));

	write_port(RTC_OUT, YEAR);
	write_port(RTC_IN, BIN2BCD(year));

	return 0;
}

void show_time(void) {
	puts_at(time(), GREEN, 0, 72);
}

void hide_time(void) {
	puts_at("        ", DEFAULT, 0, 72); 
}
