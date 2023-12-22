#ifndef _BITBANG_SPI_H
#define _BITBANG_SPI_H

#include "ch32v003fun.h"

#define SET_CLK SPI_GPIO->BSHR = ( 1 << SPI_CLK_PIN )
#define RESET_CLK SPI_GPIO->BSHR = ( 1 << ( 16 + SPI_CLK_PIN ) )

#define SET_TX SPI_GPIO->BSHR = ( 1 << SPI_TX_PIN )
#define RESET_TX SPI_GPIO->BSHR = ( 1 << ( 16 + SPI_TX_PIN ) )
// #define READ_RX ( ( SPI_GPIO->INDR >> SPI_TX_PIN ) & 0b1 )

void spi_gpio_init()
{
	SPI_GPIO->CFGLR &= ( ~( 0xf << ( 4 * SPI_CLK_PIN ) ) ) & ( ~( 0xf << ( 4 * SPI_TX_PIN ) ) );

	SPI_GPIO->CFGLR |= ( ( GPIO_Speed_50MHz | GPIO_CNF_OUT_PP ) << ( 4 * SPI_CLK_PIN ) ) |
	                   ( ( GPIO_Speed_50MHz | GPIO_CNF_OUT_PP ) << ( 4 * SPI_TX_PIN ) );
}

void bitbang_spi_write_8b( uint8_t *ptr, uint32_t cnt )
{
	for ( uint32_t i = 0; i < cnt; i++ )
	{
		uint8_t byte = ptr[i];
		for ( int j = 7; j >= 0; j-- )
		{
			RESET_CLK;

			if ( ( byte >> j ) & 0x01 )
				SET_TX;
			else
				RESET_TX;

			SET_CLK;
		}
	}
}

void bitbang_spi_write_16b( uint16_t *ptr, uint32_t cnt )
{
	for ( uint32_t i = 0; i < cnt; i++ )
	{
		uint16_t halfword = ptr[i];
		for ( int j = 15; j >= 0; j-- )
		{
			RESET_CLK;

			if ( ( halfword >> j ) & 0x01 )
				SET_TX;
			else
				RESET_TX;

			SET_CLK;
		}
	}
}
#endif