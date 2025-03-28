// **************************************************************************
//
//               Demo program for APPS labs
//
// Subject:      Computer Architectures and Parallel systems
// Author:       Petr Olivka, petr.olivka@vsb.cz, 02/2025
// Organization: Department of Computer Science, FEECS,
//               VSB-Technical University of Ostrava, CZ
//
// File:         Main program for testing.
//
// **************************************************************************

/**
 * @file    MCXN947_APPS.cpp
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

void test_leds();
void test_lcd();
void test_i2c();

int main()
{
    PRINTF( "Testing program started!\r\n" );

    // uncomment selected test
    //	test_leds();
    //	test_lcd();
    //	test_i2c();

    // ******************************************************************

    // default test for 2 LEDs and 4 buttons
    while ( 1 )
    {
        g_led_P3_16.write( !g_led_P3_16.read() );	// invert current state
        g_led_P3_17.write( !g_led_P3_16.read() );	// blink asynchronously

        delay_ms( 250 );                      		// delay

        while ( g_but_P3_18.read() == 0 ||
        		g_but_P3_19.read() == 0 ||
				g_but_P3_20.read() == 0 ||
				g_but_P3_21.read() == 0 ); 			// stop blinking
    }
}

