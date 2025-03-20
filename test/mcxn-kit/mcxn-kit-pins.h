// **************************************************************************
//
//               MCXN-KIT Library
//
// Subject:      Computer Architectures and Parallel systems
// Author:       Petr Olivka, petr.olivka@vsb.cz, 02/2025
// Organization: Department of Computer Science, FEECS,
//               VSB-Technical University of Ostrava, CZ
//
// File:         Pins
//
// **************************************************************************
//!
//! @file mcxn-kit-pins.h
//! @brief MCXN-KIT Pins definition
//!

#ifndef MCXN_KIT_PINS_H_
#define MCXN_KIT_PINS_H_

//!
//! @defgroup PINs_grp PIN names
//! @brief Pins used on MCXN-KIT.
//!
//! Pin's names correspond to MCXN MCU technical datasheet.

//!
//! @name Internal Pin Macros
//! @brief Internal macros for pin definition.
//! @{
#define _PIN_DEF( GP_NUM, PIN_NUM )		( ( ( GP_NUM ) << 8 ) | ( PIN_NUM ) )
#define _PIN_GP_INX( PIN ) 				( ( ( PIN ) & ( 0x0000FF00 ) ) >> 8 )
#define _PIN_NUM( PIN )					( ( PIN ) & ( 0x000000FF ) )
//! @}

//!
//! @addtogroup PINs_grp
//! @brief Symbol definitions of all MCXN-KIT pins.
//!
//! @{ @noop pin_names

//! \{
#define P0_14 			( _PIN_DEF( 0, 14 ) )
#define P0_15 			( _PIN_DEF( 0, 15 ) )
#define P0_22 			( _PIN_DEF( 0, 22 ) )
#define P0_24 			( _PIN_DEF( 0, 24 ) )
#define P0_25 			( _PIN_DEF( 0, 25 ) )
#define P0_26 			( _PIN_DEF( 0, 26 ) )
#define P0_28 			( _PIN_DEF( 0, 28 ) )
#define P0_29 			( _PIN_DEF( 0, 29 ) )
#define P0_30 			( _PIN_DEF( 0, 30 ) )
#define P0_31 			( _PIN_DEF( 0, 31 ) )

#define P1_00			( _PIN_DEF( 1,  0 ) )
#define P1_01			( _PIN_DEF( 1,  1 ) )
#define P1_12			( _PIN_DEF( 1, 12 ) )
#define P1_21			( _PIN_DEF( 1, 21 ) )
#define P1_22			( _PIN_DEF( 1, 22 ) )
#define P1_23			( _PIN_DEF( 1, 23 ) )

#define P2_00			( _PIN_DEF( 2,  0 ) )
#define P2_02			( _PIN_DEF( 2,  2 ) )
#define P2_04			( _PIN_DEF( 2,  4 ) )
#define P2_05			( _PIN_DEF( 2,  5 ) )
#define P2_06			( _PIN_DEF( 2,  6 ) )
#define P2_07			( _PIN_DEF( 2,  7 ) )

#define P3_16			( _PIN_DEF( 3, 16 ) )
#define P3_17			( _PIN_DEF( 3, 17 ) )
#define P3_18			( _PIN_DEF( 3, 18 ) )
#define P3_19			( _PIN_DEF( 3, 19 ) )
#define P3_20			( _PIN_DEF( 3, 20 ) )
#define P3_21			( _PIN_DEF( 3, 21 ) )

#define P4_00			( _PIN_DEF( 4,  0 ) )
#define P4_01			( _PIN_DEF( 4,  1 ) )
#define P4_02			( _PIN_DEF( 4,  2 ) )
#define P4_03			( _PIN_DEF( 4,  3 ) )
#define P4_12			( _PIN_DEF( 4, 12 ) )
#define P4_13			( _PIN_DEF( 4, 13 ) )
#define P4_16			( _PIN_DEF( 4, 16 ) )
#define P4_20			( _PIN_DEF( 4, 20 ) )

#define P5_02			( _PIN_DEF( 5,  2 ) )
#define P5_03			( _PIN_DEF( 5,  3 ) )
//! \}

//! @} @noop pin_names


#endif // MCXN_KIT_PINS_H_

