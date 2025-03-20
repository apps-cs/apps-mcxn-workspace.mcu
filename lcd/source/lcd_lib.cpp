// **************************************************************************
//
//               Demo program for labs
//
// Subject:      Computer Architectures and Parallel systems
// Author:       Petr Olivka, petr.olivka@vsb.cz, 02/2025
// Organization: Department of Computer Science, FEECS,
//               VSB-Technical University of Ostrava, CZ
//
// File:         Programming interface for LCD module
//
// **************************************************************************

#include "pin_mux.h"

#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_lpspi.h"

#include "mcxn-kit.h"
#include "lcd_lib.h"

static void _init_spi_pins(void)
{
    /* Enables the clock for PORT2: Enables clock */
    CLOCK_EnableClock(kCLOCK_Port2);

    /* PORT2_2 (pin H3) is configured as FC9_P3 */
    PORT_SetPinMux(PORT2, 2U, kPORT_MuxAlt2);

    PORT2->PCR[2] = ((PORT2->PCR[2] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_IBE_MASK)))

                     /* Input Buffer Enable: Enables. */
                     | PORT_PCR_IBE(PCR_IBE_ibe1));

    /* PORT2_3 (pin J3) is configured as FC9_P1 */
    PORT_SetPinMux(PORT2, 3U, kPORT_MuxAlt2);

    PORT2->PCR[3] = ((PORT2->PCR[3] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_IBE_MASK)))

                     /* Input Buffer Enable: Enables. */
                     | PORT_PCR_IBE(PCR_IBE_ibe1));

    /* PORT2_4 (pin K3) is configured as FC9_P0 */
    PORT_SetPinMux(PORT2, 4U, kPORT_MuxAlt2);

    PORT2->PCR[4] = ((PORT2->PCR[4] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_IBE_MASK)))

                     /* Input Buffer Enable: Enables. */
                     | PORT_PCR_IBE(PCR_IBE_ibe1));

    /* PORT2_5 (pin K1) is configured as FC9_P2 */
    PORT_SetPinMux(PORT2, 5U, kPORT_MuxAlt2);

    PORT2->PCR[5] = ((PORT2->PCR[5] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_IBE_MASK)))

                     /* Input Buffer Enable: Enables. */
                     | PORT_PCR_IBE(PCR_IBE_ibe1));

#if 0
    /* PORT2_6 (pin K2) is configured as PIO2_6 */
    PORT_SetPinMux(PORT2, 6U, kPORT_MuxAlt0);

    PORT2->PCR[6] = ((PORT2->PCR[6] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_IBE_MASK)))

                     /* Input Buffer Enable: Enables. */
                     | PORT_PCR_IBE(PCR_IBE_ibe1));
#endif
}

static void _spi_init()
{
	// set clock
    CLOCK_AttachClk( kPLL0_to_PLLCLKDIV );
	CLOCK_SetClkDiv( kCLOCK_DivPllClk, 1U );
    CLOCK_SetClkDiv( kCLOCK_DivFlexcom9Clk, 1u );
    CLOCK_AttachClk( kPLL_DIV_to_FLEXCOMM9 );

    // init pins
    _init_spi_pins();

    lpspi_master_config_t masterConfig;

    /*SPI Master config*/
    LPSPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate = 25000000;
    masterConfig.whichPcs = kLPSPI_Pcs0;
    masterConfig.pcsToSckDelayInNanoSec        = 0U;
    masterConfig.lastSckToPcsDelayInNanoSec    = 0U;
    masterConfig.betweenTransferDelayInNanoSec = 0U;
    masterConfig.bitsPerFrame = 9;

    LPSPI_MasterInit( LPSPI9, &masterConfig, CLOCK_GetLPFlexCommClkFreq(LPSPI_GetInstance( LPSPI9 ) ) );
}

#define ST7796S_CMD_SWRESET (0x01U)
#define ST7796S_CMD_SLPIN   (0x10U)
#define ST7796S_CMD_SLPOUT  (0x11U)
#define ST7796S_CMD_INVOFF  (0x20U)
#define ST7796S_CMD_INVON   (0x21U)
#define ST7796S_CMD_DISPOFF (0x28U)
#define ST7796S_CMD_DISPON  (0x29U)
#define ST7796S_CMD_CASET   (0x2AU)
#define ST7796S_CMD_RASET   (0x2BU)
#define ST7796S_CMD_RAMWR   (0x2CU)
#define ST7796S_CMD_TEOFF   (0x34U)
#define ST7796S_CMD_TEON    (0x35U)
#define ST7796S_CMD_MADCTL  (0x36U)
#define ST7796S_CMD_COLMOD  (0x3AU)
#define ST7796S_CMD_CSCON   (0xF0U)
#define ST7796S_CMD_INVTR   (0xB4U)
#define ST7796S_CMD_FRMCTR1 (0xB1U)
#define ST7796S_CMD_BPC     (0xB5U)
#define ST7796S_CMD_DFC     (0xB6U)
#define ST7796S_CMD_PWR1    (0xC0U)
#define ST7796S_CMD_PWR2    (0xC1U)
#define ST7796S_CMD_PWR3    (0xC2U)
#define ST7796S_CMD_VCMPCTL (0xC5U)
#define ST7796S_CMD_DOCA    (0xE8U)
#define ST7796S_CMD_PGC     (0xE0U)
#define ST7796S_CMD_NGC     (0xE1U)

#define ST7796S_PixelFormatRGB565	(5U)
#define ST7796S_PixelFormatRGB666	(6U)
#define ST7796S_PixelFormatRGB888	(7U)
#define ST7796S_PixelFormatRGB444	(3U)

#define ST7796S_Orientation0   		(0x08U)
#define ST7796S_Orientation90		(0x68U)
#define ST7796S_Orientation180		(0xC8U)
#define ST7796S_Orientation270		(0xA8U)

/*
 * This is the panel-specific initialization parameters for driver section,
 * including analog power/voltage control, gamma correction, scan timing, etc.
 * Data format: | 1 Byte parameter length | 1 Byte command byte | N Bytes command parameter |
 */
static const uint8_t s_st7796s_driver_preset_pars035[] = {
    /* cclang-format off */
    0x01, ST7796S_CMD_CSCON,   0xC3,                            // Enable command part 1
    0x01, ST7796S_CMD_CSCON,   0x96,                            // Enable command part 2
    0x01, ST7796S_CMD_INVTR,   0x01,                            // Display inversion
    0x02, ST7796S_CMD_FRMCTR1, 0x80, 0x10,                      // Frame rate control 1
    0x04, ST7796S_CMD_BPC,     0x1F, 0x50, 0x00, 0x20,          // Blanking porch control
    0x03, ST7796S_CMD_DFC,     0x8A, 0x07, 0x3B,                // Display function control
    0x02, ST7796S_CMD_PWR1,    0x80, 0x64,                      // Power control 1
    0x01, ST7796S_CMD_PWR2,    0x13,                            // Power control 2
    0x01, ST7796S_CMD_PWR3,    0xA7,                            // Power control 3
    0x01, ST7796S_CMD_VCMPCTL, 0x09,                            // VCOM control
    0x08, ST7796S_CMD_DOCA,    0x40, 0x8A, 0x00, 0x00, 0x29, 0x19, 0xA5, 0x33, // DOCA
    0x0E, ST7796S_CMD_PGC,     0xF0, 0x06, 0x0B, 0x07, 0x06, 0x05, 0x2E, 0x33, 0x47, 0x3A, 0x17, 0x16, 0x2E, 0x31, // PGC
    0x0E, ST7796S_CMD_NGC,     0xF0, 0x09, 0x0D, 0x09, 0x08, 0x23, 0x2E, 0x33, 0x46, 0x38, 0x13, 0x13, 0x2C, 0x32, // NGC
    0x01, ST7796S_CMD_CSCON,   0x3C, // Disable command part 1
    0x01, ST7796S_CMD_CSCON,   0x69, // Disable command part 2
    /* cclang-format on */
};

void _spi_send_data( const uint8_t *tp_data, uint32_t t_data_len )
{
	lpspi_transfer_t l_transfer;
	l_transfer.rxData = NULL;
	l_transfer.txData = tp_data;
	l_transfer.dataSize = t_data_len;
	l_transfer.configFlags = kLPSPI_MasterPcs0 | kLPSPI_MasterPcsContinuous;
	LPSPI_MasterTransferBlocking( LPSPI9, &l_transfer);
}

void _lcd_send_command( uint8_t t_command, const uint8_t *tp_data, uint32_t t_data_len )
{
	uint32_t l_tx_data_len = 2 + t_data_len * 2;
	uint8_t l_tx_data_buf[ l_tx_data_len ];
	memset( l_tx_data_buf, 0xFFU, l_tx_data_len );
	l_tx_data_buf[ 0 ] = t_command;
	l_tx_data_buf[ 1 ] = 0;
	for ( uint32_t inx = 0; inx < t_data_len; inx++ )
	{
		l_tx_data_buf[ 2 + 2 * inx ] = tp_data[ inx ];
	}

	_spi_send_data( l_tx_data_buf, l_tx_data_len );
}

static DigitalOut _g_lcd_reset_out_pit( P2_06 );

static void _lcd_hw_reset()
{
	_g_lcd_reset_out_pit.write( 0 );
    SDK_DelayAtLeastUs(1, SystemCoreClock);    /* Delay 1us. */
	_g_lcd_reset_out_pit.write( 1 );
    SDK_DelayAtLeastUs(5000, SystemCoreClock); /* Delay 5ms. */
}

static void _lcd_sw_reset()
{
    _lcd_send_command( ST7796S_CMD_SWRESET, NULL, 0 );
    SDK_DelayAtLeastUs( 5 * 1000, CLOCK_GetCoreSysClkFreq() );
}

static void _lcd_sleep( uint32_t t_on_off )
{
	if ( t_on_off != 0 )
	{
		_lcd_send_command( ST7796S_CMD_SLPIN, NULL, 0 );
	}
	else
	{
		_lcd_send_command( ST7796S_CMD_SLPOUT, NULL, 0 );
	}
}

static void _lcd_enable( uint32_t t_en_dis )
{
	if ( t_en_dis != 0 )
	{
		_lcd_send_command( ST7796S_CMD_DISPON, NULL, 0 );
	}
	else
	{
		_lcd_send_command( ST7796S_CMD_DISPOFF, NULL, 0 );
	}
}

static void _lcd_preset()
{
	const uint8_t *lp_cmds = s_st7796s_driver_preset_pars035;
	const uint8_t *lp_cmds_end = s_st7796s_driver_preset_pars035 + sizeof( s_st7796s_driver_preset_pars035 );

	while ( lp_cmds < lp_cmds_end )
	{
		_lcd_send_command( lp_cmds[ 1 ] , lp_cmds + 2, lp_cmds[ 0 ] );
		lp_cmds += 2 + lp_cmds[ 0 ];
	}

	uint8_t colmod = ST7796S_PixelFormatRGB565;
	_lcd_send_command( ST7796S_CMD_COLMOD, &colmod, 1 );
	uint8_t rot = ST7796S_Orientation270 ^ 0x80;
	_lcd_send_command( ST7796S_CMD_MADCTL, &rot, 1 );
	_lcd_send_command( ST7796S_CMD_INVON, NULL, 0 );
}

void _lcd_select_area( uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY )
{
    uint8_t tx_buf[ 20 ];
    memset( tx_buf, 0xFF, sizeof( tx_buf ) );

    tx_buf[ 0 ] = ST7796S_CMD_CASET;
    tx_buf[ 1 ] = 0;
    tx_buf[ 2 ] = (uint8_t)(startX >> 0x08U) & 0xFFU;
    tx_buf[ 4 ] = (uint8_t)startX & 0xFFU;
    tx_buf[ 6 ] = (uint8_t)(endX >> 0x08U) & 0xFFU;
    tx_buf[ 8 ] = (uint8_t)endX & 0xFFU;

    //ST7796S_ERROR_CHECK(ST7796S_WriteCommand(handle, ST7796S_CMD_CASET, tx_buf, 4U));

    tx_buf[ 10 ] = ST7796S_CMD_RASET;
    tx_buf[ 11 ] = 0;
    tx_buf[ 12 ] = (uint8_t)(startY >> 0x08U) & 0xFFU;
    tx_buf[ 14 ] = (uint8_t)startY & 0xFFU;
    tx_buf[ 16 ] = (uint8_t)(endY >> 0x08U) & 0xFFU;
    tx_buf[ 18 ] = (uint8_t)endY & 0xFFU;

    //ST7796S_ERROR_CHECK(ST7796S_WriteCommand(handle, ST7796S_CMD_RASET, tx_buf, 4U));
    _spi_send_data( tx_buf,  sizeof( tx_buf ) );
}

// **************************************************************************
// public function for LCD

void lcd_init()
{
	_spi_init();

   _lcd_hw_reset();
   _lcd_sw_reset();
   _lcd_preset();
   _lcd_sleep( 0 );

   // clean LCD memory

   uint8_t black_line[ 2 + LCD_WIDTH * 4 ] = { ST7796S_CMD_RAMWR, 0 };
   memset( black_line + 2, 0xFF, sizeof( black_line ) - 2 );
   for ( int i = 0; i < LCD_WIDTH * 4; i += 2 )
   {
	   black_line[ 2 + i ] = 0; // black
   }
   for ( int y = 0; y < LCD_HEIGHT; y++ )
   {
	   _lcd_select_area( 0, y, LCD_WIDTH, y + 1 );
	   _spi_send_data( black_line, sizeof( black_line ) );
   }

   _lcd_enable( 1 );

}
void lcd_put_pixel( uint16_t t_x, uint16_t t_y, uint16_t t_color )
{
    uint8_t tx_buf[ 26 ];
    memset( tx_buf, 0xFF, sizeof( tx_buf ) );

    tx_buf[ 0 ] = ST7796S_CMD_CASET;
    tx_buf[ 1 ] = 0;
    tx_buf[ 2 ] = (uint8_t)(t_x >> 0x08U) & 0xFFU;
    tx_buf[ 4 ] = (uint8_t)t_x & 0xFFU;
    //t_x++;
    tx_buf[ 6 ] = (uint8_t)(t_x >> 0x08U) & 0xFFU;
    tx_buf[ 8 ] = (uint8_t)t_x & 0xFFU;

    //ST7796S_ERROR_CHECK(ST7796S_WriteCommand(handle, ST7796S_CMD_CASET, tx_buf, 4U));

    tx_buf[ 10 ] = ST7796S_CMD_RASET;
    tx_buf[ 11 ] = 0;
    tx_buf[ 12 ] = (uint8_t)(t_y >> 0x08U) & 0xFFU;
    tx_buf[ 14 ] = (uint8_t)t_y & 0xFFU;
    //t_y++;
    tx_buf[ 16 ] = (uint8_t)(t_y >> 0x08U) & 0xFFU;
    tx_buf[ 18 ] = (uint8_t)t_y & 0xFFU;

    tx_buf[ 20 ] = ST7796S_CMD_RAMWR;
    tx_buf[ 21 ] = 0;
    tx_buf[ 22 ] = t_color >> 8;
    tx_buf[ 24 ] = t_color & 0xff;

    //ST7796S_ERROR_CHECK(ST7796S_WriteCommand(handle, ST7796S_CMD_RASET, tx_buf, 4U));
    _spi_send_data( tx_buf,  sizeof( tx_buf ) );
}


