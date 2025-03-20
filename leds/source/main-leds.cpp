// **************************************************************************
//
//               Demo program for APPS labs
//
// Subject:      Computer Architectures and Parallel systems
// Author:       Petr Olivka, petr.olivka@vsb.cz, 02/2025
// Organization: Department of Computer Science, FEECS,
//               VSB-Technical University of Ostrava, CZ
//
// File:         Main program for LEDs
//
// **************************************************************************

/**
 * @file    main-leds.cpp
 * @brief   Application entry point.
 */
#include <stdio.h>
#include <functional>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_mrt.h"

#include "mcxn-kit.h"

// **************************************************************************
//! System initialization. Do not modify it!!!
void _mcu_initialization() __attribute__(( constructor( 0x100 ) ));

void _mcu_initialization()
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();
    CLOCK_EnableClock( kCLOCK_Gpio0 );
    CLOCK_EnableClock( kCLOCK_Gpio1 );
    CLOCK_EnableClock( kCLOCK_Gpio2 );
    CLOCK_EnableClock( kCLOCK_Gpio3 );
    CLOCK_EnableClock( kCLOCK_Gpio4 );
}
// **************************************************************************

//! Global data

//! LEDs on MCXN-KIT - instances of class DigitalOut
DigitalOut g_led_P3_16( P3_16 );
DigitalOut g_led_P3_17( P3_17 );

//! Button on MCXN-KIT - instance of class DigitalIn
DigitalIn g_but_P3_18( P3_18 );
DigitalIn g_but_P3_19( P3_19 );
DigitalIn g_but_P3_20( P3_20 );
DigitalIn g_but_P3_21( P3_21 );

//! Callback function for LED control
void control_from_ticker()
{
	static int l_ticks = 0;

	int32_t l_periode = 500;					// T = 0.5 second

	if ( g_but_P3_18.read() == 0 )				// check button
	{
		l_periode /= 10;						// increase speed of blinking
	}

	if ( l_ticks < l_periode / 2 )				// time to switch on or off?
	{
		g_led_P3_16.write( 0 );					// LED Off
	}
	else
	{
		g_led_P3_16.write( 1 );					// LED On
	}

	l_ticks++;									// milliseconds counter
	if ( l_ticks >= l_periode )					// end of time period?
	{
		l_ticks = 0;							// start time period again
	}
}

int main()
{
	PRINTF( "LED demo program started...\n" );

#if 0
	// modern approach with timer (and interrupt)
	Ticker l_ticker;

	l_ticker		.attach( control_from_ticker, 1 );		// attach callback function

	while ( 1 )
	{ // infinite loop
	}

#else

	// lazy (conservative) approach
	while ( 1 )
	{
		int32_t l_periode = 500;				// T = 500 ms = 0.5 sec

		if ( g_but_P3_18.read() == 0 )			// button pressed?
		{
			l_periode /= 10;					// speed up blinking
		}

		g_led_P3_16.write( 0 );					// LED off

		delay_ms( l_periode / 2 );

		g_led_P3_16.write( 1 );					// LED on

		delay_ms( l_periode / 2 );
	}

#endif
}

