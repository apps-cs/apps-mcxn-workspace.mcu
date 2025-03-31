// **************************************************************************
//
//               Demo program for labs
//
// Subject:      Computer Architectures and Parallel systems
// Author:       Petr Olivka, petr.olivka@vsb.cz, 02/2025
// Organization: Department of Computer Science, FEECS,
//               VSB-Technical University of Ostrava, CZ
//
// File:         Programming interface for Capacitive Touch Screen Module
//
// **************************************************************************
#ifndef __CTS_LIB_H
#define __CTS_LIB_H

#include <stdint.h>

#define CTS_WIDTH       480
#define CTS_HEIGHT      320

//! Structure for Touch Screen Points
struct cts_points_t
{
	uint16_t m_num_points;				//!< Number of detected points
	struct
	{
		uint16_t x;						//!< x-point coordinate
		uint16_t y;						//!< y-point coordinate
		uint16_t size;					//!< size of touch
	} m_points[ 5 ];					//!< array of up to five points
};

//! CTS controller initialization
int32_t cts_init();

//! Get points from Touch Screen
//! @param t_poins Pointer to cts_points_t structure
//! @return Positive number - number of detected touches
//! @return 0 - touch was released
//! @return -1 - touch screen is no active
int32_t cts_get_ts_points( cts_points_t *t_points );

#endif // __CTS_LIB_H

