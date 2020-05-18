#include <mcu/spi.h>
#include <lib/bits.h>

/*
 * Control register 1 bits
 */

/* Data frame format (0: 8-bit data, 1: 16-bit data) */
#define SPI_CR1_DFF_BIT		BIT(11)

/*
 * Software slave management
 *
 * When the SSM bit is set, the NSS pin input is replaced with the value
 * from the SSI bit.
 * 0: Software slave management disabled
 * 1: Software slave management enabled
*/
#define SPI_CR1_SSM_BIT		BIT(9)
#define SPI_CR1_SSI_BIT		BIT(8)

/* Frame format (0: MSB transmitted first, 1: LSB transmitted first */
#define SPI_CR1_LSBFIRST_BIT	BIT(7)

/* SPI enable (0: disabled, 1: enabled */
#define SPI_CR1_SPE_BIT		BIT(6)

/* Master selection (0: Slave mode, 1: Master mode) */
#define SPI_CR1_MSTR_BIT	BIT(2)

/*
 * Status register bits
 */

/* Busy (0: not busy, 1: busy (in communication or TX buffer not empty)) */
#define SPI_SR_BSY_BIT		BIT(7)

/* Transmit buffer empty (0: tx buffer not empty, 1: tx buffer empty) */
#define SPI_SR_TXE_BIT		BIT(1)

uint8_t spi_recv_u8(struct spi *handle)
{
	return 0;
}

void spi_send_u8(struct spi *handle, uint8_t data)
{
	while (!(handle->SR & SPI_SR_TXE_BIT))
		;

	handle->DR = data;
}

void spi_send(struct spi *handle, uint8_t *tx, int txlen)
{
	while (txlen--) {
		spi_send_u8(handle, *tx);
		tx++;
	}
}

void spi_recv(struct spi *handle, uint8_t *rx, int rxlen)
{
	while (rxlen--) {
		*rx = spi_recv_u8(handle);
		rx++;
	}
}

void spi_init(struct spi *handle)
{
	/* APB2 is 36 Mhz, prescaler is 2, so SPI clock should be 16 Mhz */
	handle->CR1 =
		SPI_CR1_SSM_BIT | SPI_CR1_SSI_BIT |	/* Soft CS control */
		SPI_CR1_MSTR_BIT;			/* Master mode */

	/* TODO: Set mode */

	/* TODO: Set speed */

	/* SPI enable */
	handle->CR1 |= SPI_CR1_SPE_BIT;
}

