// **************************************************************************
//
//               Demo program for APPS labs
//
// Subject:      Computer Architectures and Parallel systems
// Author:       Petr Olivka, petr.olivka@vsb.cz, 03/2026
// Organization: Department of Computer Science, FEECS,
//               VSB-Technical University of Ostrava, CZ
//
// File:         I2C bus programming interface
//
// **************************************************************************

#include <fsl_common.h>

#include "i2c-lib.h"

//************************************************************************
// I2C bus (low level) - signals SDA & SCL control

#define I2C_DELAY		10

DigitalOut g_sda( SDA_PIN, 1, PIN_DIR_OUTPUT | PIN_ODE_ON );
DigitalOut g_scl( SCL_PIN, 1, PIN_DIR_OUTPUT | PIN_ODE_ON );

void _i2c_delay( uint32_t t_us_delay = I2C_DELAY )
{
	SDK_DelayAtLeastUs( t_us_delay, CLOCK_GetCoreSysClkFreq() );
}

// set signal SDA
static void _i2c_sda( uint8_t t_value, uint8_t t_delay = 1 )
{
	g_sda.write( t_value != 0 );
	if ( t_delay != 0 )
	{
		_i2c_delay();
	}
}

// read signal SDA
uint8_t _i2c_get_sda( uint8_t t_delay = 1 )
{
	if ( t_delay != 0 )
	{
		_i2c_delay();
	}
	return g_sda.read() != 0;
}

// set signal SCL
void _i2c_scl( uint8_t t_value )
{
	g_scl.write( t_value != 0 );
	_i2c_delay();
}

//************************************************************************
// Principle of I2C communication is described in datasheets of all I2C
// integrated circuits

// I2C initialization
void i2c_init( void )
{
	_i2c_sda( 1 );
	_i2c_scl( 1 );

	i2c_start();
	i2c_stop();
}

// Start communication on I2C
void i2c_start( void )
{
	_i2c_sda( 1 );
	_i2c_scl( 1 );
	_i2c_sda( 0 );
	_i2c_scl( 0 );
}

// Stop communication on I2C
void i2c_stop( void )
{
	_i2c_scl( 0 );
	_i2c_sda( 0 );
	_i2c_scl( 1 );
	_i2c_sda( 1 );
}

// Send ACK bit
void i2c_ack( void )
{
	_i2c_scl( 0 );
	_i2c_sda( 0 );
	_i2c_scl( 1 );
	_i2c_scl( 0 );
}

// Send NACK bit
void i2c_nack( void )
{
	_i2c_scl( 0 );
	_i2c_sda( 1 );
	_i2c_scl( 1 );
	_i2c_scl( 0 );
}

// get ACK
uint8_t i2c_get_ack( void )
{
	_i2c_sda( 1 );
	_i2c_scl( 1 );
	uint8_t ret = _i2c_get_sda( 0 );
	_i2c_scl( 0 );
	return ret;
}

// Send one byte to I2C bus
uint8_t i2c_output( uint8_t t_value )
{
	uint8_t l_mask = 0x80;
	// send 8 bits, MSB first
	for ( uint32_t i = 0; i < 8; i++ )
	{
		_i2c_sda( t_value & l_mask );
		l_mask >>= 1;
		_i2c_scl( 1 );
		_i2c_scl( 0 );
	}
	return i2c_get_ack();
}

// Receive one byte from I2C bus
uint8_t i2c_input( void )
{
	uint8_t l_value = 0;
	// quiet state on SDA line
	g_sda.write( 1 );

	// read 8 bits, MSB first
	for ( uint32_t i = 0; i < 8; i++ )
	{
		l_value <<= 1;
		_i2c_scl( 1 );
		l_value |= _i2c_get_sda( 0 );
		_i2c_scl( 0 );
	}

	return l_value;
}

