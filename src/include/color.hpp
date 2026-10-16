#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdint>

class color
{
    
public:
    std::uint8_t red;
    std::uint8_t green;
    std::uint8_t blue;

    color() : red(0), green(0), blue(0) {}
    color(std::uint8_t __red, std::uint8_t __green, std::uint8_t __blue)
        : red(__red), green(__green), blue(__blue){}

    friend color operator*(const color& _col,const double& intensity)
    {
        return {static_cast<uint8_t>(static_cast<double>(_col.red)*intensity),
            static_cast<uint8_t>(static_cast<double>(_col.green)*intensity),
            static_cast<uint8_t>(static_cast<double>(_col.blue)*intensity)};
    }
};

#endif
