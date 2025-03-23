// **************************************************************************
//
//               MCXN-KIT Library
//
// Subject:      Computer Architectures and Parallel systems
// Author:       Petr Olivka, petr.olivka@vsb.cz, 02/2025
// Organization: Department of Computer Science, FEECS,
//               VSB-Technical University of Ostrava, CZ
//
// File:         Module Digital IO
//
// **************************************************************************

#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_gpio.h"

#include "mcxn-kit.h"

static GPIO_Type *_g_gpios[] = GPIO_BASE_PTRS;
static PORT_Type *_g_ports[] = PORT_BASE_PTRS;


DigitalBase::DigitalBase( pin_name_t t_pin, uint32_t t_pin_config )
{
	assert( ( t_pin & _PIN_KEY ) == _PIN_KEY );

	// split port number and pin number
	_m_port_inx = _PIN_GP_INX( t_pin );
	_m_pin_num = _PIN_NUM( t_pin );

	gpio_pin_config_t l_gpio_pin_cfg;

	l_gpio_pin_cfg.pinDirection = ( ( ( t_pin_config & PIN_DIR_MASK ) == PIN_DIR_INPUT ) ?
			kGPIO_DigitalInput : kGPIO_DigitalOutput );
	l_gpio_pin_cfg.outputLogic = ( t_pin_config & PIN_OUT_MASK ) == PIN_OUT_HIGH;

	// set pin
	GPIO_PinInit( _g_gpios[ _m_port_inx ], _m_pin_num , &l_gpio_pin_cfg );

	port_pin_config_t l_port_pin_cfg =
	{
		.pullSelect = kPORT_PullDisable,
		.pullValueSelect = kPORT_LowPullResistor,
		.slewRate = kPORT_FastSlewRate,
		.passiveFilterEnable = kPORT_PassiveFilterDisable,
		.openDrainEnable = kPORT_OpenDrainDisable,
		.driveStrength = kPORT_LowDriveStrength,
		.mux = kPORT_MuxAlt0,
		.inputBuffer = kPORT_InputBufferEnable,
		.invertInput = kPORT_InputNormal,
		.lockRegister = kPORT_UnlockRegister
	};

	// Open Drain?
	l_port_pin_cfg.openDrainEnable = ( ( ( t_pin_config & PIN_ODE_MASK ) == PIN_ODE_ON )
			? kPORT_OpenDrainEnable : kPORT_OpenDrainDisable );

	// Pull Up/Down Active?
	if ( ( t_pin_config & PIN_PULL_OO_MASK ) == PIN_PULL_OFF )
	{
		l_port_pin_cfg.pullSelect = kPORT_PullDisable;
	}
	else
	{
		l_port_pin_cfg.pullSelect = ( ( ( t_pin_config & PIN_PULL_DU_MASK ) == PIN_PULL_UP ) ?
				kPORT_PullUp : kPORT_PullDown );
	}

	// set port
	PORT_SetPinConfig( _g_ports[ _m_port_inx ], _m_pin_num, &l_port_pin_cfg );
}

DigitalIn::DigitalIn( pin_name_t t_pin ) :
	DigitalBase( t_pin, PIN_OUT_LOW | PIN_DIR_INPUT | PIN_PULL_DOWN | PIN_PULL_OFF | PIN_ODE_OFF  )
{}

DigitalIn::DigitalIn( pin_name_t t_pin, uint32_t t_pin_config ) :
	DigitalBase( t_pin, ( t_pin_config & ( ~PIN_DIR_MASK ) ) | PIN_DIR_INPUT  )
{}

uint32_t DigitalIn::read()
{
	return GPIO_PinRead( _g_gpios[ _m_port_inx ], _m_pin_num );
}


DigitalOut::DigitalOut( pin_name_t t_pin, uint32_t t_pin_output ) :
		DigitalBase( t_pin, ( ( t_pin_output == 0 ) ? PIN_OUT_LOW : PIN_OUT_HIGH ) | PIN_DIR_OUTPUT | PIN_PULL_OFF | PIN_ODE_OFF )
{}

DigitalOut::DigitalOut( pin_name_t t_pin, uint32_t t_pin_output, uint32_t t_pin_config ) :
	DigitalBase( t_pin,
			( t_pin_config & ( ~( PIN_OUT_MASK | PIN_DIR_MASK ) ) ) |
			PIN_DIR_OUTPUT |
			( t_pin_output == 0 ) ? PIN_OUT_LOW : PIN_OUT_HIGH )
{}

uint32_t DigitalOut::read()
{
	return GPIO_PinRead( _g_gpios[ _m_port_inx ], _m_pin_num );
}

void DigitalOut::write( uint32_t t_pin_output )
{
	GPIO_PinWrite( _g_gpios[ _m_port_inx ], _m_pin_num, t_pin_output );
}
//};
//
