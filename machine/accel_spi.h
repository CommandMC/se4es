#ifndef ACCEL_SPI_H
#define ACCEL_SPI_H

//AS = acceleration sensor

// Port and pin resource for power-up of acceleration sensor, VDD=PJ.0
#define AS_PWR_OUT           (PJOUT)
#define AS_PWR_DIR           (PJDIR)
#define AS_PWR_PIN           (BIT0)

// CSN=PJ.1
#define AS_CSN_OUT           (PJOUT)
#define AS_CSN_DIR           (PJDIR)
#define AS_CSN_PIN           (BIT1)

// Port and pin resource for SPI interface to acceleration sensor
// SDO=MOSI=P1.6, SDI=MISO=P1.5, SCK=P1.7
#define AS_SPI_IN            (P1IN)
#define AS_SPI_OUT           (P1OUT)
#define AS_SPI_DIR           (P1DIR)
#define AS_SPI_SEL           (P1SEL)
#define AS_SPI_REN           (P1REN)
#define AS_SDO_PIN           (BIT6)
#define AS_SDI_PIN           (BIT5)
#define AS_SCK_PIN           (BIT7)

// Port, pin and interrupt resource for interrupt from acceleration sensor, CMA_INT=P2.5
#define AS_INT_IN            (P2IN)
#define AS_INT_OUT           (P2OUT)
#define AS_INT_DIR           (P2DIR)
#define AS_INT_REN           (P2REN)
#define AS_INT_IFG           (P2IFG)
#define AS_INT_IE            (P2IE)
#define AS_INT_PIN           (BIT5)

#define AS_TX_BUFFER         (UCB0TXBUF)
#define AS_RX_BUFFER         (UCB0RXBUF)
#define AS_TX_IFG            (UCTXIFG)
#define AS_RX_IFG            (UCRXIFG)
#define AS_IRQ_REG           (UCB0IFG)
#define AS_SPI_CTL0          (UCB0CTL0)
#define AS_SPI_CTL1          (UCB0CTL1)
#define AS_SPI_BR0           (UCB0BR0)
#define AS_SPI_BR1           (UCB0BR1)
#define AS_SPI_IE            (UCB0IE)
#define AS_SPI_IV            (UCB0IV)

// SPI timeout to detect sensor failure
#define AS_SPI_TIMEOUT       (1000u)

void accel_init();
void accel_start();
void accel_setup_spi(unsigned long bitrate);
unsigned char accel_read_write_register(unsigned char address, unsigned char data = 0);

#endif
