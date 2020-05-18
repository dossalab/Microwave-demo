#ifndef MCU_SPI_H
#define MCU_SPI_H

#include <stdint.h>

struct spi {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t CRCPR;
	volatile uint32_t RXCRCR;
	volatile uint32_t TXCRCR;

	/* I2S registers */
};

#define SPI1_BASE	0x40013000U
#define SPI1		((struct spi*)SPI1_BASE)

uint8_t spi_recv_u8(struct spi *handle);
void spi_send_u8(struct spi *handle, uint8_t data);

void spi_send(struct spi *handle, uint8_t *tx, int txlen);
void spi_recv(struct spi *handle, uint8_t *rx, int rxlen);

void spi_init(struct spi *handle);

#endif

