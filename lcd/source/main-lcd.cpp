// **************************************************************************
//
//               Demo program for APPS labs
//
// Subject:      Computer Architectures and Parallel systems
// Author:       Petr Olivka, petr.olivka@vsb.cz, 02/2025
// Organization: Department of Computer Science, FEECS,
//               VSB-Technical University of Ostrava, CZ
//
// File:         Main program for LCD module
//
// **************************************************************************

/**
 * @file    main-lcd.cpp
 * @brief   Application entry point.
 */

#include <stdio.h>
#include <functional>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

/* TODO: insert other include files here. */

#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_mrt.h"
#include "fsl_lpspi.h"

#include "mcxn-kit.h"
#include "lcd_lib.h"

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



int main()
{
	PRINTF( "LCD demo program started...\n" );

	lcd_init();

    uint16_t l_color_red = 0xF800;
    uint16_t l_color_green = 0x07E0;
    uint16_t l_color_blue = 0x001F;
    uint16_t l_color_white = 0xFFFF;

    // simple animation display four color square using LCD_put_pixel function
    int32_t l_limit = LCD_HEIGHT - 40;

    for ( int32_t ofs = 0; ofs < 20; ofs++ ) // square offset in x and y axis
            for ( int32_t i = 0; i < l_limit; i++ )
            {
                    lcd_put_pixel( ofs + i,       ofs + 0,       l_color_red );
                    lcd_put_pixel( ofs + 0,       ofs + i,       l_color_green );
                    lcd_put_pixel( ofs + i,       ofs + l_limit, l_color_blue );
                    lcd_put_pixel( ofs + l_limit, ofs + i,       l_color_white );
            }

    while ( 1 );
}

