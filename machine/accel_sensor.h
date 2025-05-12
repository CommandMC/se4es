#ifndef __ACCEL_SENSOR__H__
#define __ACCEL_SENSOR__H__

struct AccelSensor {
    /** Constants for the measurement range */
    enum class Range : unsigned char {
        /** Measurement range +- 2g */
        RANGE_2G,
        /** Measurement range +- 8g */
        RANGE_8G
    };

    /** Constants for the sample rate for acceleration data */
    enum class SampleRate : unsigned char {
        /** Sample with 30-40 Hz */
        LOW,
        /** Sample with 100-125 Hz */
        MED,
        /** Sample with 400-500 Hz */
        HIGH
    };

    /** Get an initialized instance of the sensor */
    static AccelSensor &instance();
    virtual ~AccelSensor() {}

    /** Start a measurement with the specified range and sample rate */
    virtual void enable(Range range, SampleRate rate) = 0;

    /** Stop the measurement */
    virtual void suspend() = 0;

    /** Check if acceleration data can be fetched using read_x/y/z */
    virtual bool data_available() = 0;

    /** Fetch acceleration value for the x-axis */
    virtual signed char read_x() = 0;
    /** Fetch acceleration value for the y-axis */
    virtual signed char read_y() = 0;
    /** Fetch acceleration value for the z-axis */
    virtual signed char read_z() = 0;
};

#endif
