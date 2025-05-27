#ifndef FP_FLOAT_H
#define FP_FLOAT_H

#ifndef FP_SCALING_FACTOR
#define FP_SCALING_FACTOR 6
#endif

class FixedPoint {
public:
    FixedPoint(long value);
    FixedPoint(float value);

    FixedPoint operator+(const FixedPoint& other) const;
    FixedPoint operator-(const FixedPoint& other) const;
    FixedPoint operator*(const FixedPoint& other) const;
    FixedPoint operator/(const FixedPoint& other) const;
    void operator+=(const FixedPoint& other);
    void operator-=(const FixedPoint& other);

    operator long() const;


private:
    long value;
};

#endif
