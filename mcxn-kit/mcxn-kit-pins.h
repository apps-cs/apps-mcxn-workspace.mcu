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
//! @brief Internal macros for pin names.
//! @{
#define _PIN_KEY						( 0x47 << 16 )
#define _PIN_DEF( GP_NUM, PIN_NUM )		( ( ( ( GP_NUM ) & 0xFF ) << 8 ) | ( ( PIN_NUM ) & 0xFF ) | _PIN_KEY )
#define _PIN_GP_INX( PIN ) 				( ( ( PIN ) >> 8 ) & ( 0xFF ) )
#define _PIN_NUM( PIN )					( ( PIN ) & ( 0xFF ) )
//! @}

//!
//! @addtogroup PINs_grp
//! @brief Names of all MCXN-KIT pins.
//!
//! @{ @noop pin_names

//! \{
enum pin_name_t {
	P0_14 =			( _PIN_DEF( 0, 14 ) ),		//!< Port 0 - Pin 14
	P0_15 =			( _PIN_DEF( 0, 15 ) ),      //!< Port 0 - Pin 15
	P0_22 =			( _PIN_DEF( 0, 22 ) ),      //!< Port 0 - Pin 22
	P0_24 =			( _PIN_DEF( 0, 24 ) ),      //!< Port 0 - Pin 24
	P0_25 =			( _PIN_DEF( 0, 25 ) ),      //!< Port 0 - Pin 25
	P0_26 =			( _PIN_DEF( 0, 26 ) ),      //!< Port 0 - Pin 26
	P0_28 =			( _PIN_DEF( 0, 28 ) ),      //!< Port 0 - Pin 28
	P0_29 =			( _PIN_DEF( 0, 29 ) ),      //!< Port 0 - Pin 29
	P0_30 =			( _PIN_DEF( 0, 30 ) ),      //!< Port 0 - Pin 30
	P0_31 =			( _PIN_DEF( 0, 31 ) ),      //!< Port 0 - Pin 31

	P1_00 =			( _PIN_DEF( 1,  0 ) ),      //!< Port 1 - Pin  0
	P1_01 =			( _PIN_DEF( 1,  1 ) ),      //!< Port 1 - Pin  1
	P1_12 =			( _PIN_DEF( 1, 12 ) ),      //!< Port 1 - Pin 12
	P1_21 =			( _PIN_DEF( 1, 21 ) ),      //!< Port 1 - Pin 21
	P1_22 =			( _PIN_DEF( 1, 22 ) ),      //!< Port 1 - Pin 22
	P1_23 =			( _PIN_DEF( 1, 23 ) ),      //!< Port 1 - Pin 23

	P2_00 =			( _PIN_DEF( 2,  0 ) ),      //!< Port 2 - Pin  0
	P2_02 =			( _PIN_DEF( 2,  2 ) ),      //!< Port 2 - Pin  2
	P2_04 =			( _PIN_DEF( 2,  4 ) ),      //!< Port 2 - Pin  4
	P2_05 =			( _PIN_DEF( 2,  5 ) ),      //!< Port 2 - Pin  5
	P2_06 =			( _PIN_DEF( 2,  6 ) ),      //!< Port 2 - Pin  6
	P2_07 =			( _PIN_DEF( 2,  7 ) ),      //!< Port 2 - Pin  7

	P3_16 =			( _PIN_DEF( 3, 16 ) ),      //!< Port 3 - Pin 16
	P3_17 =			( _PIN_DEF( 3, 17 ) ),      //!< Port 3 - Pin 17
	P3_18 =			( _PIN_DEF( 3, 18 ) ),      //!< Port 3 - Pin 18
	P3_19 =			( _PIN_DEF( 3, 19 ) ),      //!< Port 3 - Pin 19
	P3_20 =			( _PIN_DEF( 3, 20 ) ),      //!< Port 3 - Pin 20
	P3_21 =			( _PIN_DEF( 3, 21 ) ),      //!< Port 3 - Pin 21

	P4_00 =			( _PIN_DEF( 4,  0 ) ),      //!< Port 4 - Pin  0
	P4_01 =			( _PIN_DEF( 4,  1 ) ),      //!< Port 4 - Pin  1
	P4_02 =			( _PIN_DEF( 4,  2 ) ),      //!< Port 4 - Pin  2
	P4_03 =			( _PIN_DEF( 4,  3 ) ),      //!< Port 4 - Pin  3
	P4_12 =			( _PIN_DEF( 4, 12 ) ),      //!< Port 4 - Pin 12
	P4_13 =			( _PIN_DEF( 4, 13 ) ),      //!< Port 4 - Pin 13
	P4_16 =			( _PIN_DEF( 4, 16 ) ),      //!< Port 4 - Pin 16
	P4_20 =			( _PIN_DEF( 4, 20 ) ),      //!< Port 4 - Pin 20

	P5_02 =			( _PIN_DEF( 5,  2 ) ),      //!< Port 5 - Pin  2
	P5_03 =			( _PIN_DEF( 5,  3 ) ),      //!< Port 5 - Pin  3
};
//! \}

//! @} @noop pin_names


#endif // MCXN_KIT_PINS_H_

