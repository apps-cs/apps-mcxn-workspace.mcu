// **************************************************************************
//
//               Demo program for labs
//
// Subject:      Computer Architectures and Parallel systems
// Author:       Petr Olivka, petr.olivka@vsb.cz, 02/2025
// Organization: Department of Computer Science, FEECS,
//               VSB-Technical University of Ostrava, CZ
//
// File:         Programming interface for LCD module
//
// **************************************************************************
#ifndef __LCD_LIB_H
#define __LCD_LIB_H

#include <stdint.h>

#define LCD_WIDTH       480
#define LCD_HEIGHT      320

// LCD controller initialization
void lcd_init();

// draw one pixel to LCD screen
void lcd_put_pixel( uint16_t t_x, uint16_t t_y, uint16_t t_color );

void lcd_put_square( uint16_t t_x, uint16_t t_y, uint16_t t_color, uint8_t t_size_1_8 = 1 );

#endif // __LCD_LIB_H
