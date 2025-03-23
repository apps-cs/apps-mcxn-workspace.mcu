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

#ifndef _MCXN_KIT_H_
#define _MCXN_KIT_H_

#include "fsl_common.h"

#include "mcxn-kit-delay.h"

void delay_ms( uint32_t t_tout_ms )
{
	SDK_DelayAtLeastUs( t_tout_ms * 1000, CLOCK_GetCoreSysClkFreq() );
}

#endif // _MCXN_KIT_H_
