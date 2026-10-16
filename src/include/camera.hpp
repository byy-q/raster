#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <vector>

class Coordinate
{   
    public:
        Coordinate(const double& _x,const double& _y,const double& _z)
        :x(_x),y(_y),z(_z){}
        Coordinate():x(0.0),y(0.0),z(0.0){}
    public:
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;

};




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