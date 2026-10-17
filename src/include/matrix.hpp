#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "camera.hpp"
#include <array>
#include <cmath>
#include <stdexcept>

enum class rotation_axis
{
    x,
    y,
    z
};

class matrix4x4
{
    public:
        matrix4x4()
            : values{{
                {{1.0, 0.0, 0.0, 0.0}},
                {{0.0, 1.0, 0.0, 0.0}},
                {{0.0, 0.0, 1.0, 0.0}},
                {{0.0, 0.0, 0.0, 1.0}}
            }}
        {
        }

        static matrix4x4 scaling(double scale)
        {
            matrix4x4 result;
            result.values[0][0] = scale;
            result.values[1][1] = scale;
            result.values[2][2] = scale;
            return result;
        }

        static matrix4x4 translation(const Coordinate& offset)
        {
            matrix4x4 result;
            result.values[0][3] = offset.x;
            result.values[1][3] = offset.y;
            result.values[2][3] = offset.z;
            return result;
        }

        static matrix4x4 rotation(double angle_degrees, rotation_axis axis)
        {
            constexpr double pi = 3.14159265358979323846;
            const double radians = angle_degrees * pi / 180.0;
            const double cosine = std::cos(radians);
            const double sine = std::sin(radians);
            matrix4x4 result;

            switch (axis)
            {
                case rotation_axis::x:
                    result.values[1][1] = cosine;
                    result.values[1][2] = -sine;
                    result.values[2][1] = sine;
                    result.values[2][2] = cosine;
                    break;
                case rotation_axis::y:
                    result.values[0][0] = cosine;
                    result.values[0][2] = sine;
                    result.values[2][0] = -sine;
                    result.values[2][2] = cosine;
                    break;
                case rotation_axis::z:
                    result.values[0][0] = cosine;
                    result.values[0][1] = -sine;
                    result.values[1][0] = sine;
                    result.values[1][1] = cosine;
                    break;
                default:
                    throw std::invalid_argument("Invalid rotation axis");
            }

            return result;
        }

        matrix4x4 operator*(const matrix4x4& other) const
        {
            matrix4x4 result;
            for (std::size_t row = 0; row < 4; ++row)
            {
                for (std::size_t column = 0; column < 4; ++column)
                {
                    result.values[row][column] = 0.0;
                    for (std::size_t i = 0; i < 4; ++i)
                        result.values[row][column] +=
                            values[row][i] * other.values[i][column];
                }
            }
            return result;
        }

        Coordinate transform_point(const Coordinate& point) const
        {
            const std::array<double, 4> input = {{point.x, point.y, point.z, 1.0}};
            std::array<double, 4> output = {{0.0, 0.0, 0.0, 0.0}};

            for (std::size_t row = 0; row < 4; ++row)
            {
                for (std::size_t column = 0; column < 4; ++column)
                    output[row] += values[row][column] * input[column];
            }

            if (output[3] == 0.0)
                throw std::domain_error("Homogeneous transform produced zero w");

            return Coordinate(output[0] / output[3],
                              output[1] / output[3],
                              output[2] / output[3]);
        }

    private:
        std::array<std::array<double, 4>, 4> values;
};

#endif
