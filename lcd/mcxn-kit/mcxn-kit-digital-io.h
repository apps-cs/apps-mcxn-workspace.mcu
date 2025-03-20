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
//!
//! @file mcxn-kit-digital-io.h
//! @brief Digital IO interface.

#ifndef _MCXN_KIT_DIGITAL_IO_H_
#define _MCXN_KIT_DIGITAL_IO_H_

//!
//! @defgroup DigitalIO_grp Digital IO module
//! @brief Classes for digital input and output.


//!
//! @addtogroup DigitalIO_grp
//! @anchor PinConf
//! @brief Macros for pin initialization.
//! @{ @noop macros

#define PIN_OUT_LOW		( 0 )			//!< Output level low
#define PIN_OUT_HIGH	( 1 )			//!< Output level high
#define PIN_OUT_MASK	( 0x1 )			//!< Output level mask

//!
#define PIN_DIR_OUTPUT	( 0 )			//!< Output pin
#define PIN_DIR_INPUT	( 2 )			//!< Input pin
#define PIN_DIR_MASK	( 0x2 )			//!< Pin i/o direction mask

#define PIN_PULL_DOWN	( 0 )			//!< Pull down
#define PIN_PULL_UP		( 4 )			//!< Pull up
#define PIN_PULL_DU_MASK ( 0x4 )		//!< Pull down/up mask

#define PIN_PULL_OFF	( 0 )			//!< Disable pull down/up
#define PIN_PULL_ON		( 8 )			//!< Enable pull down/up
#define PIN_PULL_OO_MASK ( 0x8 )		//!< Pull on/off mask

#define PIN_ODE_OFF		( 0 )			//!< Open drain off
#define PIN_ODE_ON		( 16 )			//!< Open drain on
#define PIN_ODE_MASK	( 0x10 )		//!< Open drain mask

//! @} @noop macros

//!
//! @addtogroup DigitalIO_grp
//! @brief Classes for Digital input/output interface.
//!
//! @{ @noop classes

//! @brief Base of DigitalIO classes.
//! The whole class is protected and can't be used directly.
class DigitalBase
{
  protected:
	//! @brief Protected constructor for any MCU pin.
	//! @param t_pin_name Pin name, see @ref PINs_grp
	//! @param t_pin_config Pin configuration, see @ref PinConf
	DigitalBase( uint32_t t_pin_name, uint32_t t_pin_config );

	uint32_t _m_port_inx;				//!< MCU Port/GPIO number.
	uint32_t _m_pin_num;				//!< Pin number.
};

//! @brief Class for digital input pin.
class DigitalIn : public DigitalBase
{
  public:
	//! @brief Constructor for digital input pin.
	//! @param t_pin_name Any MCU pin name, see @ref PINs_grp
	DigitalIn( uint32_t t_pin_name );

	//! @brief Constructor for digital input pin. It is for experienced users.
	//! @param t_pin_name Any MCU pin name, see @ref PINs_grp
	//! @param t_pin_config Pin configuration, see @ref PinConf
	DigitalIn( uint32_t t_pin_name, uint32_t t_pin_config );

	//! @brief Read current state of input pin.
	//! @return Return value is low or high (0 or 1) level of input pin.
	uint32_t read();
};

//! @brief Class for digital output pin.
class DigitalOut : public DigitalBase
{
  public:
	//! @brief Constructor for digital output pin.
	//! @param t_pin_name Any MCU pin name, see @ref PINs_grp.
	//! @param t_pin_output Initial logic level on output.
	DigitalOut( uint32_t t_pin_name, uint32_t t_pin_output = 0 );

	//! @brief Constructor for digital output pin. It is for experienced users.
	//! @param t_pin_name Any MCU pin name, see @ref PINs_grp.
	//! @param t_pin_output Initial logic level on output.
	//! @param t_pin_config Pin configuration, see @ref PinConf
	DigitalOut( uint32_t t_pin_name, uint32_t t_pin_output, uint32_t t_pin_config );

	//! @brief Read current state of output pin.
	//! @return Return value is low or high (0 or 1) level of input pin.
	uint32_t read();

	//! @brief Set low or high (0 or 1) level on output pin.
	//! @param t_pin_output Requred level of output pin.
	void write( uint32_t t_pin_output );
};

//! @} @noop classes

#endif // _MCXN_KIT_DIGITAL_IO_H_

