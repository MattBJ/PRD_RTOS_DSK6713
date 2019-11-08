/*
 * Project description:
 *
 * 	Create a DSP BIOS app. Create a PRD to initially go off every 1 sec and toggle LED0.

	Unchecking the (Enable Real-TIme Analysis) and (Enable All TRC Event Classes) options will improve stability.

	Every time the PRD goes off, read the switches (0-15).  If SW = 0,  stop PRD. Otherwise, make LED0 cycle at that Hz rate (1-15)
	by adjusting PRD refresh rate.

	Remember 2 toggles is 1 period.
 */

/*
 * AUTHORS:
 * 			PRINCIPAL		AUTHOR		- Matthew Bailey
 *
 * 			Lower 			beings 		- Samuel Okei
 * 										- Patrick Gipson
 * 										- Brad Ward (executive shit talker)
 * 										- Manvitha Karre
 * 										- Lawrence Lewis
 *
 *
 */

// HEADERS
#include <std.h>
//#include <log.h>
#include <prd.h>

#include "dsk6713_dip.h"
#include "dsk6713_led.h"
#include "dsk6713.h"

#include "MattB_HW7cfg.h"
//

typedef struct _Global {
	volatile char prev_switch; // keeps track of DIP switches
	PRD_Obj *PRD0;
	char startup; // flag for beginning of runtime
} Global;

Global Glo; // initialize global structure

Uint32 CONST_PERIOD[] = {500,250,166,125,100,83,71,62,56,50,45,42,38,36,33}; // 1-15 hz
void init(void);

int main(void) {
	init();
	Glo.prev_switch = 0;
	Glo.startup = 1;
	Glo.PRD0 = &PRD0;
	// --> Becomes DSP/BIOS idle loop
	return 0;
}

void init(void){
	DSK6713_init();                   	//call BSL to init DSK-EMIF,PLL)
	CSL_init();
	DSK6713_LED_init();  				//init LEDs
	DSK6713_DIP_init();                 //init DIP SWs
}


// ticks = 32bit unsigned
void PRD0_ftn(int arg0, int arg1) {
	char temp = ((!DSK6713_DIP_get(3))<<3) + ((!DSK6713_DIP_get(2))<<2) + ((!DSK6713_DIP_get(1))<<1) + (!DSK6713_DIP_get(0));

	if(temp != Glo.prev_switch) { // changes frequency
		if(!Glo.prev_switch)
			Glo.startup = 0;
		PRD0.period =		(temp == 1)? CONST_PERIOD[0] :
							(temp == 2)? CONST_PERIOD[1]:
							(temp == 3)? CONST_PERIOD[2]:
							(temp == 4)? CONST_PERIOD[3]:
							(temp == 5)? CONST_PERIOD[4]:
							(temp == 6)? CONST_PERIOD[5]:
							(temp == 7)? CONST_PERIOD[6]:
							(temp == 8)? CONST_PERIOD[7]:
							(temp == 9)? CONST_PERIOD[8]:
							(temp == 10)? CONST_PERIOD[9]:
							(temp == 11)? CONST_PERIOD[10]:
							(temp == 12)? CONST_PERIOD[11]:
							(temp == 13)? CONST_PERIOD[12]:
							(temp == 14)? CONST_PERIOD[13]: CONST_PERIOD[14]; // last is 15 Hz
	}
	if(!(Glo.startup) && (temp == 0))
		PRD_stop(Glo.PRD0);
	// toggle LED
	DSK6713_LED_toggle(0);
}
