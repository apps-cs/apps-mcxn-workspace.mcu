// **************************************************************************
//
//               MCXN-KIT Library
//
// Subject:      Computer Architectures and Parallel systems
// Author:       Petr Olivka, petr.olivka@vsb.cz, 02/2025
// Organization: Department of Computer Science, FEECS,
//               VSB-Technical University of Ostrava, CZ
//
// File:         Module delay
//
// **************************************************************************
//!
//! @file mcxn-kit-delay.h
//! @brief Functions for MCU delay.
//!

#ifndef _MCXN_KIT_DELAY_H_
#define _MCXN_KIT_DELAY_H_

//!
//! @defgroup Delay_grp MCU delay
//! @brief Functions for MCU delay.

//!
//! @addtogroup Delay_grp
//! @{

//! Function stops a program for required amount of milliseconds.
//! @param t_tout_ms Timeout in milliseconds.
void delay_ms( uint32_t t_tout_ms );

// @}

#endif // _MCXN_KIT_DELAY_H_
