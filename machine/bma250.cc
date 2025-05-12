#include "config.h"
#include <msp430.h>
#include "machine/bma250.h"
#include "machine/accel_spi.h"

BMA250::BMA250() = default;

void BMA250::set_range(Range range) {
	write_register(Register::GRANGE, static_cast<unsigned char>(range));
}

void BMA250::set_bandwidth(Bandwidth bw) {
	write_register(Register::BWD, static_cast<unsigned char>(bw));
}

void BMA250::enable_normal_mode() {
	write_register(Register::PM, 0);
}

void BMA250::enable_lowpower_mode(SleepPhase sleep) {
	write_register(Register::PM, static_cast<unsigned char>(sleep));
}

void BMA250::enable(AccelSensor::Range range, AccelSensor::SampleRate rate) {
	BMA250::Range bma_range;
	switch (range) {
		case AccelSensor::Range::RANGE_2G:
			bma_range = BMA250::Range::RANGE_2;
			break;
		case AccelSensor::Range::RANGE_8G:
			bma_range = BMA250::Range::RANGE_8;
			break;
		default:
			bma_range = BMA250::Range::RANGE_2;
	}

	BMA250::Bandwidth bma_bw;
	BMA250::SleepPhase bma_sleep;
	switch (rate) {
		case AccelSensor::SampleRate::LOW:
			bma_bw = BMA250::Bandwidth::BANDWIDTH_16;
			bma_sleep = BMA250::SleepPhase::SLEEPPHASE_25;
			break;
		case AccelSensor::SampleRate::MED:
			bma_bw = BMA250::Bandwidth::BANDWIDTH_63;
			bma_sleep = BMA250::SleepPhase::SLEEPPHASE_6;
			break;
		case AccelSensor::SampleRate::HIGH:
			bma_bw = BMA250::Bandwidth::BANDWIDTH_250;
			bma_sleep = BMA250::SleepPhase::SLEEPPHASE_2;
			break;
		default:
			bma_bw = BMA250::Bandwidth::BANDWIDTH_8;
			bma_sleep = BMA250::SleepPhase::SLEEPPHASE_50;
	}

	set_range(bma_range);
	set_bandwidth(bma_bw);
	enable_lowpower_mode(bma_sleep);
}

bool BMA250::data_available() {
	return true; // FIXME
}

void BMA250::suspend() {
	write_register(Register::PM, BIT7);
}


// Axes are changed to match the orientation of the CMA3000
//    _________          ^   ^
//   /  Top   /          |  /
//  /        /           z x
// /________/    <---y---|/

signed char BMA250::read_x() {
	unsigned char data;

	// Dummy read LSB from LSB acceleration data to update MSB (BMA250 datasheet 4.4.1)
	data = read_register(Register::ACC_Y_LSB);
	// Store X/Y/Z MSB acceleration data in buffer
	data = read_register(Register::ACC_Y_MSB);

	return static_cast<signed char>(data);
}

signed char BMA250::read_y() {
	unsigned char data;

	// Dummy read LSB from LSB acceleration data to update MSB (BMA250 datasheet 4.4.1)
	data = read_register(Register::ACC_X_LSB);
	// Store X/Y/Z MSB acceleration data in buffer
	data = read_register(Register::ACC_X_MSB);

	return - static_cast<signed char>(data);
}

signed char BMA250::read_z() {
	unsigned char data;

	// Dummy read LSB from LSB acceleration data to update MSB (BMA250 datasheet 4.4.1)
	data = read_register(Register::ACC_Z_LSB);
	// Store X/Y/Z MSB acceleration data in buffer
	data = read_register(Register::ACC_Z_MSB);

	return static_cast<signed char>(data);
}

unsigned char BMA250::read_register(Register reg) {
	// Exit if sensor is not powered up
	if ((AS_PWR_OUT & AS_PWR_PIN) != AS_PWR_PIN) return 0;

	unsigned char address = static_cast<unsigned char>(reg);
	address |= BIT7;                   // set R/W bit for reading

    return accel_read_write_register(address);
}

unsigned char BMA250::write_register(Register reg, unsigned char value) {
	unsigned char address = static_cast<unsigned char>(reg);
	address &= ~BIT7;                   // R/W bit to be not set

	return accel_read_write_register(address, value);
}

void BMA250::plugin() {
	accel_setup_spi(6'000'000);

	// Set default sensor configuration
	BMA250::set_range(Range::RANGE_2);
	BMA250::set_bandwidth(Bandwidth::BANDWIDTH_63);
	BMA250::suspend();
}

bool BMA250::prologue() {
	return false;
}
