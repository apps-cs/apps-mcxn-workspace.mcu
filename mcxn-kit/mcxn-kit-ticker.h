// **************************************************************************
//
//               MCXN-KIT Library
//
// Subject:      Computer Architectures and Parallel systems
// Author:       Petr Olivka, petr.olivka@vsb.cz, 02/2025
// Organization: Department of Computer Science, FEECS,
//               VSB-Technical University of Ostrava, CZ
//
// File:         Module Ticker
//
// **************************************************************************
#ifndef _MCXN_KIT_TICKER_H_
#define _MCXN_KIT_TICKER_H_

//!
//! @file mcxn-kit-ticker.h
//! @brief Recurring callback

#include <functional>

//!
//! @defgroup Ticker_grp Ticker module
//! @brief Class for recurring callback.
//!
//! Ticker is used for call a function or an object method at recurring interval.

//!
//! @addtogroup Ticker_grp
//! @{ @noop class

//! @brief Class for recurring callback.
//! One Ticker can attach only one callback!
class Ticker
{
public:
	//! Ticker constructor.
	Ticker();

	//! Ticker destructor.
	~Ticker();

	//! @brief Method to attach some object and its method for callback.
	//! @param t_callback A class method for the callback.
	//! @param t_instance An object with the callback method.
	//! @param t_time_ms The interval of the callback period. Must be greater then 0.
	//! @return When attach failed then return 0. Otherwise 1.
	template <class T> uint32_t attach( void ( T::*t_callback )(), T *t_instance, uint32_t t_time_ms )
	{
		return _attach( std::bind( t_callback, t_instance ), t_time_ms );
	}

	//! @brief Method to attach some function for callback.
	//! @param t_callback A function for callback.
	//! @param t_time_ms The interval of the callback period. Must be greater then 0.
	//! @return When attach failed then return 0. Otherwise 1.
	uint32_t attach( void ( *t_callback )(), uint32_t t_time_ms )
	{
		return _attach( std::bind( t_callback ), t_time_ms );
	}

	//! @brief Method to attach some function with one `void*` argument for callback.
	//! @param t_callback A function for callback.
	//! @param tp_arg Argument for attached callback.
	//! @param t_time_ms The interval of the callback period. Must be greater then 0.
	//! @return When attach failed then return 0. Otherwise 1.
	uint32_t attach( void ( *t_callback )( void * ), void *tp_arg, uint32_t t_time_ms )
	{
		return _attach( std::bind( t_callback, tp_arg ), t_time_ms );
	}

	//! @brief Detach the currently attached Ticker.
	void detach();

  protected:
    //! @brief Method to attach any callback to HW timer.
	//! @param t_callback Any callback.
	//! @param t_time_ms The interval of the callback period. Must be greater then 0.
	//! @return When attach failed then return 0. Otherwise 1.
	uint32_t _attach( std::function<void()> t_callback, uint32_t t_time_ms );

	int32_t _m_table_index = -1;		//!< Index of attached callback in timer table.
};

//! @} @noop class


#endif // _MCXN_KIT_TICKER_H_

