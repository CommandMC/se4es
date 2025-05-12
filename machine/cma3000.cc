#include "config.h"
#include <msp430.h>
#include "device/plugbox.h"
#include "machine/timer.h"
#include "machine/cma3000.h"
#include "machine/accel_spi.h"

/* CMA_CTRL bits */
#define G_RANGE   (BIT7)
#define INT_LEVEL (BIT6)
#define MDET_EXIT (BIT5)
#define I2C_DIS   (BIT4)
#define MODE2     (BIT3)
#define MODE1     (BIT2)
#define MODE0     (BIT1)
#define INT_DIS   (BIT0)

CMA3000::CMA3000() = default;

void CMA3000::set_mode_range(Mode mode, CMA3000::Range range) {
	write_register(Register::CTRL, I2C_DIS | static_cast<unsigned char>(mode) | static_cast<unsigned char>(range));
}

void CMA3000::enable(AccelSensor::Range range, AccelSensor::SampleRate rate) {
	CMA3000::Range cma_range = [=] {
		switch (range) {
			case AccelSensor::Range::RANGE_2G:
				return CMA3000::Range::RANGE_2G;
			case AccelSensor::Range::RANGE_8G:
				return CMA3000::Range::RANGE_8G;
			default:
				return CMA3000::Range::RANGE_2G;
		}
	}();
	CMA3000::Mode cma_mode = [=] {
		switch (rate) {
			case AccelSensor::SampleRate::LOW:
				return (range == AccelSensor::Range::RANGE_2G)
				? CMA3000::Mode::MEASURE_100HZ
				: CMA3000::Mode::MEASURE_40HZ;
			case AccelSensor::SampleRate::MED:
				return CMA3000::Mode::MEASURE_100HZ;
			case AccelSensor::SampleRate::HIGH:
				return CMA3000::Mode::MEASURE_400HZ;
			default:
				return CMA3000::Mode::POWERDOWN;
		}
	}();

	set_mode_range(cma_mode, cma_range);
}

// Returns true if there are new sensor data available
// Is reset by the sensor when data is read
bool CMA3000::data_available() {
	return AS_INT_IN & AS_INT_PIN;
}

void CMA3000::suspend() {
	set_mode_range(CMA3000::Mode::POWERDOWN, CMA3000::Range::RANGE_2G);
}

signed char CMA3000::read_x() {
	return read_register(Register::DOUTX);
}

signed char CMA3000::read_y() {
	return read_register(Register::DOUTY);
}

signed char CMA3000::read_z() {
	return read_register(Register::DOUTZ);
}

unsigned char CMA3000::read_register(Register reg) {
	unsigned char address = static_cast<unsigned char>(reg);
	address <<= 2; // Address to be shifted left by 2. RW bit (BIT1) is reset implicitly.

	return accel_read_write_register(address);
}

unsigned char CMA3000::write_register(Register reg, unsigned char data) {
	unsigned char address = static_cast<unsigned char>(reg);
	address <<= 2;                              // Address has to be shifted left
	address |= BIT1;                            // RW bit to be set

	return accel_read_write_register(address, data);
}

void CMA3000::plugin() {
	accel_setup_spi(400'000);

	// Reset sensor
	write_register(Register::RSTR, 0x02);
	write_register(Register::RSTR, 0x0A);
	write_register(Register::RSTR, 0x04);

	/* I2C abschalten, Stromsparmodus einschalten */
	set_mode_range(Mode::POWERDOWN, Range::RANGE_2G);
}

bool CMA3000::prologue() {
	return false;
}
