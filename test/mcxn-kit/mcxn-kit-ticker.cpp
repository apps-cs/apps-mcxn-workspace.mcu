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

#include "fsl_common.h"
#include "fsl_mrt.h"

#include "mcxn-kit.h"

//! Length of table for tickers
#define TICKER_CALLBACK_TABLE_LEN	64

//!
//! @cond 0
//! Item for ticker table
struct callback_table_item_t
{
	std::function<void()> m_callback;
	uint32_t m_tout_ms;
};

//! Internal ticker table.
static callback_table_item_t _ticker_callback_table[ TICKER_CALLBACK_TABLE_LEN ];
//! Current top position of attached ticker in table.
static uint32_t _ticker_callback_table_top = 0;
//! Internal ticker counter.
static uint32_t _ticker_counter = 0;
//! MTR timer initialized.
static uint32_t _ticker_initialized = 0;

//! Implementation of MRT HW interrupt handler.
extern "C" {
	void MRT0_IRQHandler(void);
}

void MRT0_IRQHandler( void )
{
    // Clear interrupt flag.
    MRT_ClearStatusFlags( MRT0, kMRT_Channel_0, kMRT_TimerInterruptFlag );

    // check timeout for all attached Tickers
    for ( uint32_t inx = 0; inx < _ticker_callback_table_top; inx++ )
    {
		if ( ( _ticker_callback_table[ inx ].m_tout_ms != 0 ) &&
				( ( _ticker_counter % _ticker_callback_table[ inx ].m_tout_ms ) == 0 ) )
		{
			_ticker_callback_table[ inx ].m_callback();
		}
    }

    // Increment current time
    _ticker_counter++;
}

//! Initialization of MRT timer.
static void _init_mrt()
{
	mrt_config_t mrtConfig;
	//! Get default configuration
    MRT_GetDefaultConfig( &mrtConfig );
    //! Init MRT0
    MRT_Init( MRT0, &mrtConfig );
    //! Setup channel 0 to repeat mode
    MRT_SetupChannelMode( MRT0, kMRT_Channel_0, kMRT_RepeatMode );
    //! Enable MRT channel 0 interrupt
    MRT_EnableInterrupts( MRT0, kMRT_Channel_0, kMRT_TimerInterruptEnable );
    EnableIRQ( MRT0_IRQn );
    //! Enable MRT interrupt (NVIC)
    MRT_StartTimer( MRT0, kMRT_Channel_0, MSEC_TO_COUNT( 1, CLOCK_GetFreq( kCLOCK_BusClk ) ) );
}

//! @endcond

//! class Ticker

//! Ticker constructor
Ticker::Ticker()
{
	if ( _ticker_initialized == 0 )
	{
		_init_mrt();
		_ticker_initialized = 1;
	}
	_m_table_index = -1;
}

//! Ticker destructor
Ticker::~Ticker()
{
	detach();
}

//! Attach any callback.
uint32_t Ticker::_attach( std::function<void()> t_callback, uint32_t t_time_ms )
{
	if ( t_time_ms == 0 || _m_table_index != -1 )
	{
		// Ticker already attached or time interval is not allowed 0.
		return 0;
	}

	DisableIRQ( MRT0_IRQn );

	int32_t l_inx = -1;
	// search empty position under top of table
	for ( uint32_t inx = 0; inx < _ticker_callback_table_top; inx++ )
	{
		if ( _ticker_callback_table[ inx ].m_tout_ms == 0 )
		{
			l_inx = inx;
			break;
		}
	}
	// move up top of table?
	if ( ( l_inx == -1 ) && _ticker_callback_table_top < TICKER_CALLBACK_TABLE_LEN )
	{
		l_inx = _ticker_callback_table_top;
		_ticker_callback_table_top++;
	}
	// attach new ticker
	if ( l_inx != -1 )
	{
		_ticker_callback_table[ l_inx ].m_callback = t_callback;
		_ticker_callback_table[ l_inx ].m_tout_ms = t_time_ms;

		_m_table_index = l_inx;
	}

	EnableIRQ(MRT0_IRQn);

	return ( l_inx != -1 );
}

//! Detach current ticker
void Ticker::detach()
{
	if ( _m_table_index != -1 )
	{
		DisableIRQ(MRT0_IRQn);

		// clean table position
		_ticker_callback_table[ _m_table_index ].m_callback = nullptr;
		_ticker_callback_table[ _m_table_index ].m_tout_ms = 0;
		// set invalid index
		_m_table_index = -1;
		// move down top of ticker table
		while ( _ticker_callback_table_top )
		{
			if ( _ticker_callback_table[ _ticker_callback_table_top - 1 ].m_tout_ms == 0 )
			{
				_ticker_callback_table_top--;
			}
			else
			{
				break;
			}
		}

		EnableIRQ(MRT0_IRQn);
	}
}

