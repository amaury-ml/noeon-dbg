#pragma once

#ifndef __GNUC__

union _Float16
{
    uint16_t r_;
    struct {
        uint16_t sign : 1;
        uint16_t exp : 5;
        uint16_t frac : 10;
    };

    _Float16() {}
    _Float16(float f);
    _Float16(_Float16 const& o): r_(o.r_) {}

    bool operator==(_Float16 const& rhs) const
    {
        return float(r_) == float(rhs.r_);
    }

    _Float16& operator=(_Float16 const& o) { r_ = o.r_; return *this; }
    operator float() const;
};
#endif