#include "fixedpoint.h"

FixedPoint::FixedPoint(const long value) {
    this->value = value;
}

FixedPoint::FixedPoint(const float value) {
    this->value = value * (0b1 << FP_SCALING_FACTOR);
}

FixedPoint FixedPoint::operator+(const FixedPoint& other) const {
    return FixedPoint(this->value + other.value);
}

FixedPoint FixedPoint::operator-(const FixedPoint& other) const {
    return FixedPoint(this->value - other.value);
}

FixedPoint FixedPoint::operator*(const FixedPoint &other) const {
    return FixedPoint((this->value * other.value) >> FP_SCALING_FACTOR);
}

FixedPoint FixedPoint::operator/(const FixedPoint &other) const {
    return FixedPoint((this->value / other.value) << FP_SCALING_FACTOR);
}

void FixedPoint::operator+=(const FixedPoint& other) {
    this->value += other.value;
}

void FixedPoint::operator-=(const FixedPoint &other) {
    this->value -= other.value;
}


FixedPoint::operator long() const {
    return this->value >> FP_SCALING_FACTOR;
}
