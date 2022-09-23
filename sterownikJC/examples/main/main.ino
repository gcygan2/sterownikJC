//Sterownik czasowy

#include "sterownikJC.h"

Sterownik st;

//19200 bps

void setup ()
{
	st.init();
}

void loop ()
{
	st.run();	
}