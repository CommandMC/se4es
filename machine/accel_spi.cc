#include <msp430.h>

#include "machine/timer.h"
#include "machine/accel_spi.h"

bool as_ok = false;

void accel_init() {
    // Deactivate connection to acceleration sensor
    AS_PWR_OUT &= ~AS_PWR_PIN;         // Power off
    AS_INT_OUT &= ~AS_INT_PIN;         // Pin to low to avoid floating pins
    AS_SPI_OUT &= ~(AS_SDO_PIN + AS_SDI_PIN + AS_SCK_PIN); // Pin to low to avoid floating pins
    AS_CSN_OUT &= ~AS_CSN_PIN;         // Pin to low to avoid floating pins
    AS_INT_DIR |= AS_INT_PIN;          // Pin to output to avoid floating pins
    AS_SPI_DIR |= AS_SDO_PIN + AS_SDI_PIN + AS_SCK_PIN; // Pin to output to avoid floating pins
    AS_CSN_DIR |= AS_CSN_PIN;          // Pin to output to avoid floating pins
    AS_PWR_DIR |= AS_PWR_PIN;          // Power pin to output direction

    // Reset global sensor flag
    as_ok = true;
}

void accel_start() {
    AS_SPI_DIR &= ~AS_SDI_PIN;                   // Switch SDI pin to input
    AS_SPI_REN |= AS_SDI_PIN;                    // Pulldown on SDI pin
    AS_SPI_SEL |= AS_SDO_PIN + AS_SDI_PIN + AS_SCK_PIN; // Port pins to SDO, SDI and SCK function
    AS_CSN_OUT |= AS_CSN_PIN;                    // Deselect acceleration sensor
    AS_PWR_OUT |= AS_PWR_PIN;                    // Power on active high

    // Delay of >5ms required between switching on power and configuring sensor
    Timer::delay_us(10000);

    // Initialize interrupt pin for polling the data read out from acceleration sensor
    AS_INT_DIR &= ~AS_INT_PIN; // Switch INT pin to input
    // Pullup on INT pin
    AS_INT_OUT |= AS_INT_PIN;
    AS_INT_REN |= AS_INT_PIN;
}

void accel_setup_spi(unsigned long bitrate)
{
    // Initialize SPI interface to acceleration sensor (SLAU259E 23.3.1)
    AS_SPI_CTL1 |= UCSWRST; // Hold in reset during configuration

    // 3-pin SPI, synchronous, SPI master, 8 data bits, MSB first, clock idle low, data output on falling edge
    // This configuration is called 4-wire SPI in the BMA250 datasheet, section 6.1
    AS_SPI_CTL0 = UCMODE_0 | UCSYNC | UCMST | UCMSB | UCCKPH;
    AS_SPI_CTL1 = UCSSEL__SMCLK | UCSWRST; // SMCLK as clock source, stay in reset

    AS_SPI_BR0   = CONFIG_CPU_FREQUENCY / bitrate; // Low byte of division factor for baud rate (about 6MHz)
    AS_SPI_BR1   = 0x00;                         	 // High byte of division factor for baud rate

    // Map SPI pin function
    PMAPPWD = PMAPPW; // Get write-access to port mapping registers with the Port Mapping Password
    PMAPCTL = PMAPRECFG; // Allow reconfiguration during runtime
    P1MAP5  = PM_UCB0SOMI; // P1.5 = SPI MISO input
    P1MAP6  = PM_UCB0SIMO; // P1.6 = SPI MOSI output
    P1MAP7  = PM_UCB0CLK; // P1.7 = SPI CLK output
    PMAPPWD = 0; // Disable write-access to port mapping registers

    // Configure interface pins
    accel_init();
    accel_start();

    AS_SPI_CTL1 &= ~UCSWRST; // Start SPI hardware
    Timer::delay_us(20000);
}

//for read omit optional data parameter
unsigned char accel_read_write_register(unsigned char address, unsigned char data)
{
    unsigned char result;
    unsigned short timeout;

    // Exit function if an error was detected previously
    if (!as_ok)
        return (0);

    AS_SPI_REN &= ~AS_SDI_PIN;                   // Pulldown on SDI pin not required
    AS_CSN_OUT &= ~AS_CSN_PIN;                   // Select acceleration sensor

    result = AS_RX_BUFFER;                      // Read RX buffer just to clear
                                                 // interrupt flag

    AS_TX_BUFFER = address;                     // Write address to TX buffer

    timeout = AS_SPI_TIMEOUT;
    while (!(AS_IRQ_REG & AS_RX_IFG) && (--timeout > 0)); // Wait until new data was written into
                                                 // RX buffer
    if (timeout == 0) {
        as_ok = false;
        return (0);
    }
    result = AS_RX_BUFFER;                      // Read RX buffer just to clear
                                                 // interrupt flag

    AS_TX_BUFFER = data;                        // Write data to TX buffer

    timeout = AS_SPI_TIMEOUT;
    while (!(AS_IRQ_REG & AS_RX_IFG) && (--timeout > 0)); // Wait until new data was written into
                                                 // RX buffer
    if (timeout == 0) {
        as_ok = false;
        return (0);
    }
    result = AS_RX_BUFFER;                      // Read RX buffer

    AS_CSN_OUT |= AS_CSN_PIN;                    // Deselect acceleration sensor
    AS_SPI_REN |= AS_SDI_PIN;                    // Pulldown on SDI pin required again

    return result;
}
