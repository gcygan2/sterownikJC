#include <avr/io.h>
#include "i2c_master.h"




class Sterownik
{
private:
	uint8_t rtc_h, rtc_m, rtc_s;
	uint8_t rtc_set_h, rtc_set_m;
	uint8_t on_h, on_m;
	uint8_t off_h, off_m;
	uint8_t alarm;
	uint8_t cmd;
	static char buf[24];
	void set ();
	void store();
	void check ();

public:
	void init ();
	void run ();
};
