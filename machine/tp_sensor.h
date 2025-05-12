#ifndef __TP_SENSOR__H__
#define __TP_SENSOR__H__

struct TPSensor {
    /* Operating modes */
    enum class Mode : unsigned char {
        LOW_POWER = 0u,
        HIGH_RESOLUTION = 1u
    };

    /* Get an initialized instance of the sensor */
    static TPSensor& instance();

    virtual ~TPSensor() {}

    virtual bool init() = 0;

    /* Set operating mode */
    virtual bool set_mode(Mode mode) = 0;

    /* Get temperature from sensor in 0.1 degree Celsius.
     * For a temperature of 20.5 degree Celsius this method returns 205.
     */
    virtual int get_temperature() = 0;

    /* Get pressure in Pascal */
    virtual long get_pressure() = 0;
};

#endif  //!__TP_SENSOR__H__
