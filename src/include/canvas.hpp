#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include "color.hpp"

class canvas
{

    public:
        using fstream = std::fstream;
        using ofstream = std::ofstream;
        using string = std::string;


        canvas(const size_t& __X,const size_t& __y,const string& __filename,
               const color& back_color = color())
            : pixel_x(__X), pixel_y(__y)
        {
            output_file.open(__filename, std::ios::binary | std::ios::out | std::ios::trunc);
            if (!output_file)
                throw std::runtime_error("Failed to open canvas output file");
            file_init(back_color);
        }
        canvas():pixel_x(1),pixel_y(1)
        {
            output_file.open("./images/image_1.bmp", std::ios::binary | std::ios::out | std::ios::trunc);
            if (!output_file)
                throw std::runtime_error("Failed to open canvas output file");
            file_init(color());
        }
        ~canvas(){output_file.close();}
        canvas(const canvas& ) =delete;
        canvas(canvas&&)=delete;
        canvas& operator=(const canvas&)=delete;
        canvas& operator=(canvas&&)=delete;

        void draw_color(const size_t& __x,const size_t& __y,const color& __col);

    
//the pixel number of the canvas
        size_t pixel_x;
        size_t pixel_y;
    private:
        std::ofstream output_file;
//the file format initialize
        void file_init(const color& back_color);
};


inline void canvas::file_init(const color& back_color)
{
    const std::uint64_t width = pixel_x;
    const std::uint64_t height = pixel_y;
    if (width == 0 || height == 0 ||
        width > static_cast<std::uint64_t>(std::numeric_limits<std::int32_t>::max()) ||
        height > static_cast<std::uint64_t>(std::numeric_limits<std::int32_t>::max()))
        throw std::invalid_argument("BMP dimensions must be positive and fit in 32 bits");

    /** 
     * @details:the calculation of the row_size,the bmp formats require that the size per row must can be divide by 4,and single pixel
     * use 3 bytes,so width * 3 in total,we need to find the nearest number can be divied by 4 of width*3,so we add 3 to it then divide 4
     * and multiply 4 to get the bytes size
    */    
    const std::uint64_t row_size = ((width * 3 + 3) / 4) * 4;//the bytes number per row of the pixel data

    const std::uint64_t pixel_data_size = row_size * height;//the bytes number of all pixel data
    const std::uint64_t file_size = 54 + pixel_data_size;//entire file size ,the 54 are the file header size
    if (file_size > std::numeric_limits<std::uint32_t>::max())
        throw std::invalid_argument("BMP file size exceeds the format limit");


    //put the data directly into file and the lower end first    
    const auto write_u16 = [this](std::uint16_t value)
    {
        output_file.put(static_cast<char>(value & 0xff));
        output_file.put(static_cast<char>((value >> 8) & 0xff));
    };
    const auto write_u32 = [this](std::uint32_t value)
    {
        output_file.put(static_cast<char>(value & 0xff));
        output_file.put(static_cast<char>((value >> 8) & 0xff));
        output_file.put(static_cast<char>((value >> 16) & 0xff));
        output_file.put(static_cast<char>((value >> 24) & 0xff));
    };
    //start builing file attrabutes here
    output_file.put('B');
    output_file.put('M');
    write_u32(static_cast<std::uint32_t>(file_size));
    write_u32(0);
    write_u32(54);
    write_u32(40);
    write_u32(static_cast<std::uint32_t>(width));
    write_u32(static_cast<std::uint32_t>(height));
    write_u16(1);
    write_u16(24);
    write_u32(0);
    write_u32(static_cast<std::uint32_t>(pixel_data_size));
    write_u32(2835);
    write_u32(2835);
    write_u32(0);
    write_u32(0);

    const std::size_t padding_size = static_cast<std::size_t>(row_size - width * 3);//the stuffed number size
    for (std::uint64_t y = 0; y < height; ++y)
    {
        for (std::uint64_t x = 0; x < width; ++x)
        {
            output_file.put(static_cast<char>(back_color.blue));
            output_file.put(static_cast<char>(back_color.green));
            output_file.put(static_cast<char>(back_color.red));
        }
        for (std::size_t i = 0; i < padding_size; ++i)
            output_file.put(0);
    }

    if (!output_file)
        throw std::runtime_error("Failed to initialize BMP output file");
}



inline void canvas::draw_color(const size_t& __x,const size_t& __y,const color& __col)
{
    if (__x >= pixel_x || __y >= pixel_y)
        throw std::out_of_range("Canvas pixel coordinates are out of range");
    if (!output_file)
        throw std::runtime_error("Canvas output file is not writable");

    const std::uint64_t row_size =
        ((static_cast<std::uint64_t>(pixel_x) * 3 + 3) / 4) * 4;
    const std::uint64_t offset = 54 +
        (static_cast<std::uint64_t>(pixel_y - 1 - __y) * row_size) +
        static_cast<std::uint64_t>(__x) * 3;
    output_file.seekp(static_cast<std::streamoff>(offset), std::ios::beg);
    output_file.put(static_cast<char>(__col.blue));
    output_file.put(static_cast<char>(__col.green));
    output_file.put(static_cast<char>(__col.red));
    if (!output_file)
        throw std::runtime_error("Failed to write canvas pixel");
}

class Point
{
    public:
        uint32_t _x;
        uint32_t _y;
    
        friend void swap(Point& A,Point& B)
        {
            Point temp = B;
            B = A;
            A = temp;
        }
};

#endif