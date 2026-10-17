#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <vector>

class Coordinate
{   
    public:
        Coordinate(const double& _x,const double& _y,const double& _z)
        :x(_x),y(_y),z(_z){}
        Coordinate():x(0.0),y(0.0),z(0.0){}
        void operator*=(const double&r){
            x*=r;y*=r;z*=r;
        }
        friend Coordinate& operator*(const Coordinate&_co,const double&r)
        {
            Coordinate result = _co;
            result*=r;
            return result;
        }

        void operator+=(const Coordinate& _co)
        {
            x+=_co.x;
            y+=_co.y;
            z+=_co.z;
        }
        friend Coordinate operator+(const Coordinate& _co1,const Coordinate& _co2)
        {
            Coordinate result = _co1;
            result+=_co2;
            return result;
        }
    public:
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;

};
typedef Coordinate _Vector;



class camera
{
    public:
        camera(const double& _distance,const double& _width,const double& _height ):
        distance(_distance),width(_width),height(_height){}
        camera() = default;
    private:
        Coordinate origin = {0.0,0.0,0.0};
    public:
        double distance = 1.0;
        double width = 2.0;
        double height = 2.0;
};



#endif