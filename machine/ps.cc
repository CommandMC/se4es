// *************************************************************************************************
//
//      Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/
//
//
//        Redistribution and use in source and binary forms, with or without
//        modification, are permitted provided that the following conditions
//        are met:
//
//          Redistributions of source code must retain the above copyright
//          notice, this list of conditions and the following disclaimer.
//
//          Redistributions in binary form must reproduce the above copyright
//          notice, this list of conditions and the following disclaimer in the
//          documentation and/or other materials provided with the
//          distribution.
//
//          Neither the name of Texas Instruments Incorporated nor the names of
//          its contributors may be used to endorse or promote products derived
//          from this software without specific prior written permission.
//
//        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//        "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//        LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//        OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//        LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//        DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//        THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// *************************************************************************************************
// pressure sensor driver functions
// *************************************************************************************************

// *************************************************************************************************
// Include section
#include <msp430.h>
#include "machine/timer.h"

// driver
#include "machine/ps.h"

// *************************************************************************************************
// Prototypes section

// *************************************************************************************************
// Defines section

// *************************************************************************************************
// Global Variable section

// pressure (hPa) to altitude (m) conversion tables
const signed short h0[17] =
{ -153, 0, 111, 540, 989, 1457, 1949, 2466, 3012, 3591, 4206, 4865, 5574, 6344, 7185, 8117, 9164 };
const unsigned short p0[17] =
{ 1031, 1013, 1000, 950, 900, 850, 800, 750, 700, 650, 600, 550, 500, 450, 400, 350, 300 };

float p[17];

// *************************************************************************************************
// Extern section

// *************************************************************************************************
// @fn          bmp_ps_init
// @brief       Init pressure sensor I/O
// @param       none
// @return      none
// *************************************************************************************************
void ps_init(void)
{
    PS_INT_DIR &= ~PS_INT_PIN;           // EOC is input
    PS_INT_IES &= ~PS_INT_PIN;           // Interrupt on EOC rising edge
    PS_I2C_OUT |= PS_SCL_PIN + PS_SDA_PIN; // SCL and SDA are high by default
    PS_I2C_DIR |= PS_SCL_PIN + PS_SDA_PIN; // SCL and SDA are outputs by default

    // 100msec delay to guarantee stable operation
    for (int i=0; i<10; i++) {
        Timer::delay_us(10000);
    }
}

// *************************************************************************************************
// @fn          ps_i2c_sda
// @brief       Control SDA line
// @param       unsigned char condition            PS_I2C_SEND_START, PS_I2C_SEND_RESTART, PS_I2C_SEND_STOP
//                                      PS_I2C_CHECK_ACK
// @return      unsigned char                      1=ACK, 0=NACK
// *************************************************************************************************
unsigned char ps_i2c_sda(unsigned char condition)
{
    unsigned char sda = 0;

    if (condition == PS_I2C_SEND_START)
    {
    	PS_I2C_SDA_OUT;      // SDA is output
    	PS_I2C_SCL_HI;
        ps_i2c_delay();
        PS_I2C_SDA_LO;
        ps_i2c_delay();
        PS_I2C_SCL_LO;       // SDA 1-0 transition while SCL=1 (will be 0)
        ps_i2c_delay();
    }
    else if (condition == PS_I2C_SEND_RESTART)
    {
    	PS_I2C_SDA_OUT;      // SDA is output
    	PS_I2C_SCL_LO;
    	PS_I2C_SDA_HI;
        ps_i2c_delay();
        PS_I2C_SCL_HI;
        ps_i2c_delay();
        PS_I2C_SDA_LO;
        ps_i2c_delay();
        PS_I2C_SCL_LO;       // SDA 1-0 while SCL = 1 (was 0)
        ps_i2c_delay();
    }
    else if (condition == PS_I2C_SEND_STOP)
    {
    	PS_I2C_SDA_OUT;      // SDA is output
    	PS_I2C_SDA_LO;
        ps_i2c_delay();
        PS_I2C_SCL_LO;
        ps_i2c_delay();
        PS_I2C_SCL_HI;
        ps_i2c_delay();
        PS_I2C_SDA_HI;       // SDA 0-1 transition while SCL=1
        ps_i2c_delay();
    }
    else if (condition == PS_I2C_CHECK_ACK)
    {
    	PS_I2C_SDA_IN;       // SDA is input
    	PS_I2C_SCL_LO;
        ps_i2c_delay();
        PS_I2C_SCL_HI;
        ps_i2c_delay();
        sda = PS_I2C_IN & PS_SDA_PIN; // ACK = SDA during ack clock pulse
        PS_I2C_SCL_LO;
    }

    // Return value will only be evaluated when checking device ACK
    return (sda == 0);
}

// *************************************************************************************************
// @fn          ps_i2c_write
// @brief       Clock out bits through SDA.
// @param       unsigned char data                 Byte to send
// @return      none
// *************************************************************************************************
void ps_i2c_write(unsigned char data)
{
    unsigned char i, mask;

    // Set mask byte to 10000000b
    mask = BIT0 << 7;

    PS_I2C_SDA_OUT;                    // SDA is output

    for (i = 8; i > 0; i--)
    {
    	PS_I2C_SCL_LO;                 // SCL=0
        if ((data & mask) == mask)
        {
        	PS_I2C_SDA_HI;             // SDA=1
        }
        else
        {
        	PS_I2C_SDA_LO;             // SDA=0
        }
        mask = mask >> 1;
        ps_i2c_delay();
        PS_I2C_SCL_HI;                 // SCL=1
        ps_i2c_delay();
    }

    PS_I2C_SCL_LO;                     // SCL=0
    PS_I2C_SDA_IN;                     // SDA is input
}

// *************************************************************************************************
// @fn          ps_i2c_read
// @brief       Read bits from SDA
// @param       unsigned char ack                  1=Send ACK after read, 0=Send NACK after read
// @return      unsigned char                      Bits read
// *************************************************************************************************
unsigned char ps_i2c_read(unsigned char ack)
{
    unsigned char i;
    unsigned char data = 0;

    PS_I2C_SDA_IN;                     // SDA is input

    for (i = 0; i < 8; i++)
    {
    	PS_I2C_SCL_LO;                 // SCL=0
        ps_i2c_delay();
        PS_I2C_SCL_HI;                 // SCL=0
        ps_i2c_delay();

        // Shift captured bits to left
        data = data << 1;

        // Capture new bit
        if ((PS_I2C_IN & PS_SDA_PIN) == PS_SDA_PIN)
            data |= BIT0;
    }

    PS_I2C_SDA_OUT;                    // SDA is output

    // 1 aditional clock phase to generate master ACK
    PS_I2C_SCL_LO;                     // SCL=0
    if (ack == 1)
    	PS_I2C_SDA_LO                  // Send ack -> continue read
    else
    	PS_I2C_SDA_HI                  // Send nack -> stop read
    ps_i2c_delay();
    PS_I2C_SCL_HI;                     // SCL=0
    ps_i2c_delay();
    PS_I2C_SCL_LO;

    return (data);
}

// *************************************************************************************************
// @fn          as_write_register
// @brief       Write a byte to the pressure sensor
// @param       unsigned char device               Device address
//              unsigned char address              Register address
//              unsigned char data                 Data to write
// @return      unsigned char
// *************************************************************************************************
unsigned char ps_write_register(unsigned char device, unsigned char address, unsigned char data)
{
    volatile unsigned char success;

    ps_i2c_sda(PS_I2C_SEND_START);               // Generate start condition

    ps_i2c_write(device | PS_I2C_WRITE);         // Send 7bit device address + r/w bit '0' -> write
    success = ps_i2c_sda(PS_I2C_CHECK_ACK);      // Check ACK from device
    if (!success)
        return (0);

    ps_i2c_write(address);                       // Send 8bit register address
    success = ps_i2c_sda(PS_I2C_CHECK_ACK);      // Check ACK from device
    if (!success)
        return (0);

    ps_i2c_write(data);                          // Send 8bit data to register
    success = ps_i2c_sda(PS_I2C_CHECK_ACK);      // Check ACK from device
    // Slave does not send this ACK
    // if (!success) return (0);

    ps_i2c_sda(PS_I2C_SEND_STOP);                // Generate stop condition

    return (1);
}

// *************************************************************************************************
// @fn          ps_read_register
// @brief       Read a byte from the pressure sensor
// @param       unsigned char device               Device address
//              unsigned char address              Register address
//              unsigned char mode                 PS_I2C_8BIT_ACCESS, PS_I2C_16BIT_ACCESS
// @return      unsigned long                     Register content
// *************************************************************************************************
unsigned long ps_read_register(unsigned char device, unsigned char address, unsigned char mode)
{
    unsigned char success;
    unsigned long data = 0;

    ps_i2c_sda(PS_I2C_SEND_START);               // Generate start condition

    ps_i2c_write(device | PS_I2C_WRITE);         // Send 7bit device address + r/w bit '0' -> write
    success = ps_i2c_sda(PS_I2C_CHECK_ACK);      // Check ACK from device
    if (!success)
        return (0);

    ps_i2c_write(address);                       // Send 8bit register address
    success = ps_i2c_sda(PS_I2C_CHECK_ACK);      // Check ACK from device
    if (!success)
        return (0);

    ps_i2c_sda(PS_I2C_SEND_RESTART);             // Generate restart condition

    ps_i2c_write(device | PS_I2C_READ);          // Send 7bit device address + r/w bit '1' -> read
    success = ps_i2c_sda(PS_I2C_CHECK_ACK);      // Check ACK from device
    if (!success)
        return (0);

    if (mode == PS_I2C_24BIT_ACCESS)
    {
        data = (long)ps_i2c_read(1) << 16;        // Read MSB 8bit data from register
        data |= ps_i2c_read(1) << 8;              // Read LSB 8bit data from register
        data |= ps_i2c_read(0);                   // Read XLSB 8bit data from register
    }
    else if (mode == PS_I2C_16BIT_ACCESS)
    {
        data = ps_i2c_read(1) << 8;              // Read MSB 8bit data from register
        data |= ps_i2c_read(0);                  // Read LSB 8bit data from register
    }
    else
    {
        data = ps_i2c_read(0);                   // Read 8bit data from register
    }

    ps_i2c_sda(PS_I2C_SEND_STOP);                // Generate stop condition

    return (data);
}

// *************************************************************************************************
// @fn          ps_i2c_delay
// @brief       Delay between I2C signal edges.
// @param       none
// @return      none
// *************************************************************************************************
void ps_i2c_delay(void)
{
    for (unsigned int i=0; i<100; i++) {
        asm volatile("nop");
    }
}
