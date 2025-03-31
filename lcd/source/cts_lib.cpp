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

#include "pin_mux.h"

#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_lpi2c.h"

#include "fsl_debug_console.h"

#include "mcxn-kit.h"
#include "cts_lib.h"

// **************************************************************************
// I2C Interface Init

#define CTS_I2C		LPI2C1

#define PCR_IBE_ibe1 0x01u //!<@brief Input Buffer Enable: Enables
#define PCR_ODE_ode1 0x01u //!<@brief Open Drain Enable: Enables
#define PCR_PE_pe1 0x01u   //!<@brief Pull Enable: Enables
#define PCR_PS_ps0 0x00u   //!<@brief Pull Select: Enables internal pulldown resistor
#define PCR_PS_ps1 0x01u   //!<@brief Pull Select: Enables internal pullup resistor

static void _init_i2c_pins(void)
{
    /* PORT0_24 (pin B6) is configured as FC1_P0 */
    PORT_SetPinMux(PORT0, 24U, kPORT_MuxAlt2);

    PORT0->PCR[24] = ((PORT0->PCR[24] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_IBE_MASK)))

                      /* Pull Select: Enables internal pullup resistor. */
                      | PORT_PCR_PS(PCR_PS_ps1)

                      /* Pull Enable: Enables. */
                      | PORT_PCR_PE(PCR_PE_pe1)

                      /* Open Drain Enable: Enables. */
                      | PORT_PCR_ODE(PCR_ODE_ode1)

                      /* Input Buffer Enable: Enables. */
                      | PORT_PCR_IBE(PCR_IBE_ibe1));

    /* PORT0_25 (pin A6) is configured as FC1_P1 */
    PORT_SetPinMux(PORT0, 25U, kPORT_MuxAlt2);

    PORT0->PCR[25] = ((PORT0->PCR[25] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_IBE_MASK)))

                      /* Pull Select: Enables internal pullup resistor. */
                      | PORT_PCR_PS(PCR_PS_ps1)

                      /* Pull Enable: Enables. */
                      | PORT_PCR_PE(PCR_PE_pe1)

                      /* Open Drain Enable: Enables. */
                      | PORT_PCR_ODE(PCR_ODE_ode1)

                      /* Input Buffer Enable: Enables. */
                      | PORT_PCR_IBE(PCR_IBE_ibe1));
}

static void _i2c_init()
{
    lpi2c_master_config_t masterConfig;

	// attach FRO 12M to FLEXCOMM1
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom1Clk, 2u);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM1);

    _init_i2c_pins();

    LPI2C_MasterGetDefaultConfig( &masterConfig );

    // Change the default baudrate configuration
    // masterConfig.baudRate_Hz = LPI2C_BAUDRATE / 4;

    // Initialize the LPI2C master peripheral
    LPI2C_MasterInit( CTS_I2C, &masterConfig, CLOCK_GetLPFlexCommClkFreq(1u) );
}

// **************************************************************************

#define TS_GT911_I2C_ADR1					( 0xBA >> 1 )
#define TS_GT911_I2C_ADR2					( 0x28 >> 1 )

#define TS_GT911_ADR_XRES_L					( 0x8048 )
#define TS_GT911_ADR_XRES_H					( 0x8049 )
#define TS_GT911_ADR_YRES_L					( 0x804A )
#define TS_GT911_ADR_YRES_H					( 0x804B )

#define TS_GT911_ADR_MOD_SW1				( 0x804D )

#define TS_GT911_ADR_CFG_CHKSUM				( 0x80FF )
#define TS_GT911_ADR_CFG_FRESH				( 0x8100 )

#define TS_GT911_ADR_XY_DATA				( 0x814E )

#define TS_GT911_ADR_CFG_START				( 0x8047 )
#define TS_GT911_ADR_CFG_STOP				( 0x80FE )

#define TS_GT911_MOD_SW1_TRIG_RISE			( 0x00 )
#define TS_GT911_MOD_SW1_TRIG_FALL			( 0x01 )
#define TS_GT911_MOD_SW1_TRIG_LOW			( 0x02 )
#define TS_GT911_MOD_SW1_TRIG_HIGH			( 0x03 )
#define TS_GT911_MOD_SW1_SITO				( 0x04 )
#define TS_GT911_MOD_SW1_XY_SWAP			( 0x08 )
#define TS_GT911_MOD_SW1_STRETCH_0			( 0x00 )
#define TS_GT911_MOD_SW1_STRETCH_1			( 0x10 )
#define TS_GT911_MOD_SW1_STRETCH_2			( 0x20 )
#define TS_GT911_MOD_SW1_STRETCH_3			( 0x30 )
#define TS_GT911_MOD_SW1_MIRROR_X			( 0x40 )
#define TS_GT911_MOD_SW1_MIRROR_Y			( 0x80 )

#define TS_GT911_XRES						( CTS_HEIGHT )
#define TS_GT911_YRES						( CTS_WIDTH )

#define HI( X )								( ( ( X ) >> 8 ) & 0xFF )
#define LO( X )								( ( X ) & 0xFF )

uint8_t _g_cts_i2c_addr = 0;
DigitalIn _g_cts_int_pin( P0_26, PIN_DIR_INPUT | PIN_PULL_DOWN );

int32_t _i2c_send_recv( uint8_t t_i2c_slave_adr, uint8_t *t_tx_buf, uint32_t t_tx_buf_len, uint8_t *t_rx_buf, uint32_t t_rx_buf_len )
{
	if ( LPI2C_MasterStart( CTS_I2C, t_i2c_slave_adr, kLPI2C_Write ) != kStatus_Success )
    {
    	return -1; // communication not started
    }

	// Check master tx FIFO empty or not
    size_t l_tx_count = 0xFFU;
	LPI2C_MasterGetFifoCounts( CTS_I2C, NULL, &l_tx_count );
	while ( l_tx_count )
	{
		LPI2C_MasterGetFifoCounts( CTS_I2C, NULL, &l_tx_count );
	}

	// Check communicate with slave successful or not
	if ( LPI2C_MasterGetStatusFlags( CTS_I2C ) & kLPI2C_MasterNackDetectFlag )
	{
		LPI2C_MasterStop( CTS_I2C );
		return 0; // slave it not responding
	}

	if ( t_tx_buf != NULL && t_tx_buf_len > 0 )
	{
        uint32_t reVal = LPI2C_MasterSend( CTS_I2C, t_tx_buf, t_tx_buf_len );
        if ( reVal != kStatus_Success )
        {
            if ( reVal == kStatus_LPI2C_Nak )
            {
                LPI2C_MasterStop( CTS_I2C );
            }
            return -1;
        }
	}

	if ( t_rx_buf != NULL && t_rx_buf_len > 0 )
	{
        uint32_t reVal = LPI2C_MasterRepeatedStart( CTS_I2C, t_i2c_slave_adr, kLPI2C_Read );
        if ( reVal != kStatus_Success )
        {
            return -1;
        }

        reVal = LPI2C_MasterReceive( CTS_I2C, t_rx_buf, t_rx_buf_len );
        if ( reVal != kStatus_Success )
        {
            if ( reVal == kStatus_LPI2C_Nak )
            {
                LPI2C_MasterStop( CTS_I2C );
            }
            return -1;
        }

    }

    if ( LPI2C_MasterStop( CTS_I2C ) != kStatus_Success )
    {
        return -1;
    }

    return 1;
}


// **************************************************************************
// public function for CTS

int32_t cts_init()
{
	_i2c_init();

	_g_cts_i2c_addr = 0;

    if ( _i2c_send_recv( TS_GT911_I2C_ADR1, NULL, 0, 0, 0 ) > 0 )
    {
    	_g_cts_i2c_addr = TS_GT911_I2C_ADR1;
    }
    if ( _i2c_send_recv( TS_GT911_I2C_ADR2, NULL, 0, 0, 0 ) > 0 )
    {
    	_g_cts_i2c_addr = TS_GT911_I2C_ADR2;
    }
    //PRINTF( "CTS Adr 0x%2x\r\n", _g_cts_i2c_addr );

    if ( _g_cts_i2c_addr == 0 )
    {
    	return -1;
    }

    uint8_t l_set_res[] = {
    		HI( TS_GT911_ADR_XRES_L ), LO( TS_GT911_ADR_XRES_L ),
    		LO( TS_GT911_YRES ), HI( TS_GT911_YRES ),
    		LO( TS_GT911_XRES ), HI( TS_GT911_XRES )
    	};

    if ( _i2c_send_recv( _g_cts_i2c_addr, l_set_res, 6, NULL, 0 ) <= 0 )
    {
    	return -1;
    }

    uint8_t l_set_mod[] = {
    		HI( TS_GT911_ADR_MOD_SW1 ), LO( TS_GT911_ADR_MOD_SW1 ),
			TS_GT911_MOD_SW1_MIRROR_X | TS_GT911_MOD_SW1_STRETCH_0 | TS_GT911_MOD_SW1_XY_SWAP | TS_GT911_MOD_SW1_TRIG_HIGH
    	};

    if ( _i2c_send_recv( _g_cts_i2c_addr, l_set_mod, 3, NULL, 0 ) <= 0 )
	{
    	return -1;
    }

    uint8_t l_chksum = 0;
    uint8_t l_read_mem[] = { HI( TS_GT911_ADR_CFG_START ), LO( TS_GT911_ADR_CFG_START ) };
    uint8_t l_data_mem[ TS_GT911_ADR_CFG_STOP - TS_GT911_ADR_CFG_START + 1 ];

    if ( _i2c_send_recv( _g_cts_i2c_addr, l_read_mem, 2, l_data_mem, sizeof( l_data_mem ) ) <= 0 )
    {
    	return -1;
    }

    for ( uint32_t i = 0; i < sizeof( l_data_mem ); i++ )
    {
    	l_chksum += l_data_mem[ i ];
    }
    l_chksum = (~l_chksum) + 1;

    //PRINTF( "chksum %x\n", l_chksum );

	uint8_t l_cfg_fresh[] = { HI( TS_GT911_ADR_CFG_CHKSUM ), LO( TS_GT911_ADR_CFG_CHKSUM ), l_chksum, 1 };
	_i2c_send_recv( _g_cts_i2c_addr, l_cfg_fresh, 4, NULL, 0 );

	return 0;
}

int32_t cts_get_ts_points( cts_points_t *t_points )
{
	if ( t_points == NULL )
	{
		return -1;
	}
	// clear content of *t_points
	memset( t_points, 0, sizeof( *t_points ) );

	// no activity of touch screen detected
	if ( _g_cts_int_pin.read() == 0 )
	{
		return -1;
	}

	uint8_t l_tstatus;			// touch screen status
	int rep = 0;
	for ( ; rep < 50; rep++ )	// wait for status 0x80
	{
		uint8_t l_get_xy[] = { HI( TS_GT911_ADR_XY_DATA ), LO( TS_GT911_ADR_XY_DATA ) };
		_i2c_send_recv( _g_cts_i2c_addr, l_get_xy, 2, &l_tstatus, 1 );
		if ( l_tstatus != 0 ) break;
		SDK_DelayAtLeastUs( 500, SystemCoreClock );
	}

	if ( l_tstatus == 0 )		// data not ready
	{
		return 0;
	}

	// some point(s) detected?
	if ( ( l_tstatus & 0xF ) != 0 )
	{
		uint8_t l_get_xy[] = { HI( TS_GT911_ADR_XY_DATA ), LO( TS_GT911_ADR_XY_DATA ), 0 };
		uint8_t l_data[ 40 ];
		_i2c_send_recv( _g_cts_i2c_addr, l_get_xy, 2, l_data, sizeof( l_data ) );

		t_points->m_num_points = l_data[ 0 ] & 0xF;
		for ( uint32_t p = 0; p < t_points->m_num_points; p++ )
		{
			t_points->m_points[ p ].x = l_data[ p * 8 + 2 ] | l_data[ p * 8 + 3 ] << 8;
			t_points->m_points[ p ].y = l_data[ p * 8 + 4 ] | l_data[ p * 8 + 5 ] << 8;
			t_points->m_points[ p ].size = l_data[ p * 8 + 6 ] | l_data[ p * 8 + 7 ] << 8;
		}
	}

	// clear status
	if ( ( l_tstatus & 0x80 ) != 0 )
	{
		uint8_t l_clear[] = { HI( TS_GT911_ADR_XY_DATA ), LO( TS_GT911_ADR_XY_DATA ), 0 };
		_i2c_send_recv( _g_cts_i2c_addr, l_clear, 3, NULL, 0 );
		//SDK_DelayAtLeastUs( 1000, SystemCoreClock );
	}

	// no points
	if ( ( l_tstatus & 0xF ) == 0 )
	{
		return 0;
	}

	return t_points->m_num_points;
}

