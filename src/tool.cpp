#include <vector>
#include <inttypes.h>
#include "./include/color.hpp"
#include "./include/canvas.hpp"
#include "./include/camera.hpp"
#include "./include/world_objects.hpp"

void camera::move(const _Vector& offset)
{
    const matrix4x4 inverse_translation = matrix4x4::translation(
        Coordinate(-offset.x, -offset.y, -offset.z));
    for (instance* object : all_instances())
        object->apply_transform(inverse_translation);
}

void camera::rotate(double angle_degrees, rotation_axis axis)
{
    const matrix4x4 inverse_rotation = matrix4x4::rotation(-angle_degrees, axis);
    for (instance* object : all_instances())
        object->apply_transform(inverse_rotation);
}

/** 
 * @return : return the value of y between y_1 and y_2,with linear interpolation
*/

std::vector<uint32_t> interpolate(uint32_t x_1,uint32_t y_1,uint32_t x_2,uint32_t y_2)
{
    if(x_1 == x_2)
        return {y_1};

    double k = static_cast<double>(static_cast<int64_t>(y_1)-static_cast<int64_t>(y_2))/
    static_cast<double>(static_cast<int64_t>(x_1)-static_cast<int64_t>(x_2));
    std::vector<uint32_t>result ;
    result.push_back(y_1);
    
    if(x_1 > x_2)
        for(uint32_t x = x_1+1;x>=x_2;x--)
        {
            result.push_back(static_cast<uint32_t>(static_cast<int64_t>(y_1) + k * (static_cast<int64_t>(x) - static_cast<int64_t>(x_1))));
        }
      else
        for(uint32_t x = x_1+1;x<=x_2;x++)
            result.push_back(static_cast<uint32_t>(static_cast<double>(y_1) +
                k * (static_cast<int64_t>(x) - static_cast<int64_t>(x_1))));
            
    return result;
}

std::vector<double> interpolate(uint32_t x1,double h1,uint32_t x2,double h2)
{
    if(x1 == x2)
        return {h1};
    
        double k = (h2 - h1) / static_cast<double>(static_cast<int64_t>(x2)-static_cast<int64_t>(x1));
        std::vector<double> result;
        result.push_back(h1);
    if(x1 > x2)
        for(uint32_t x = x1+1;x>=x2;x--)
        {
            result.push_back((h1 + k * static_cast<double>(static_cast<int64_t>(x) - static_cast<int64_t>(x1))));
        }
      else
        for(uint32_t x = x1+1;x<=x2;x++)
            result.push_back((h1 + k*static_cast<double>(static_cast<int64_t>(x) - static_cast<int64_t>(x1))));
            
    return result;
}    




void draw_line(canvas& Canvas,const Point& P1,const Point& p2,const color& _col)
{
    std::vector<uint32_t> y = interpolate(P1._x,P1._y,p2._x,p2._y);
    if(P1._x > p2._x)
    {
        for(uint32_t x = P1._x;x>=p2._x;x--)
        {
            Canvas.draw_color(x,y[P1._x-x],_col);
        }
    }
    else
    {
        for(uint32_t x = P1._x;x<=p2._x;x++)
            Canvas.draw_color(x,y[x-P1._x],_col);
    }
}
void draw_clinal_line(canvas& Canvas,const Point& A,const double& h1,const Point& B,const double& h2,const color& _col)
{
    std::vector<uint32_t> y = interpolate(A._x,A._y,B._x,B._y);
    std::vector<double> intensity = interpolate(A._x,h1,B._x,h2);
    if(A._x > B._x)
    {
        for(uint32_t x = A._x;x>=B._x;x--)
        {
            Canvas.draw_color(x,y[A._x-x],_col*intensity[A._x-x]);
        }
    }
    else
    {
        for(uint32_t x = A._x;x <= B._x;x++)
        {
            Canvas.draw_color(x,y[x-A._x],_col*intensity[x-A._x]);
        }
    }
}

void draw_triangle(canvas& Canvas,const Point& A,const Point& B,const Point& C,const color& _col )
{
    draw_line(Canvas,A,B,_col);
    draw_line(Canvas,B,C,_col);
    draw_line(Canvas,A,C,_col);
}

void fill_triangle(canvas& Canvas,Point A,Point B,Point C,const color& _col)
{
    //make sure that the A is the lowest point and the c is the highest point
    if(A._y > B._y)
        swap(A,B);
    if(A._y > C._y)
        swap(A,C);
    if(B._y > C._y)
        swap(B,C);

    std::vector<uint32_t> AC = interpolate(A._y,A._x,C._y,C._x);
    std::vector<uint32_t> AB = interpolate(A._y,A._x,B._y,B._x);
    std::vector<uint32_t> BC = interpolate(B._y,B._x,C._y,C._x);
    AB.pop_back();
    std::vector<uint32_t> x_left = AB;
    x_left.insert(x_left.end(),BC.begin(),BC.end());
    std::vector<uint32_t>& x_right = AC;    

    if(x_left[1] > x_right[1])
        swap(x_left,x_right);

    for(uint32_t y = A._y;y<=C._y;y++)
        draw_line(Canvas,{x_left[y-A._y],y},{x_right[y-A._y],y},_col);
}
void fill_clinal_triangle(canvas& Canvas,Point A,const double& h1,Point B,const double& h2,Point C,const double& h3,const color& _col)
{
     //make sure that the A is the lowest point and the c is the highest point
    if(A._y > B._y)
        swap(A,B);
    if(A._y > C._y)
        swap(A,C);
    if(B._y > C._y)
        swap(B,C);

    std::vector<uint32_t> AC = interpolate(A._y,A._x,C._y,C._x);
    std::vector<double> hAC = interpolate(A._x,h1,C._x,h3);
    std::vector<uint32_t> AB = interpolate(A._y,A._x,B._y,B._x);
    std::vector<double> hAB = interpolate(A._x,h1,B._x,h2);
    std::vector<uint32_t> BC = interpolate(B._y,B._x,C._y,C._x);
    std::vector<double> hBC = interpolate(B._x,h2,C._x,h3);
    AB.pop_back();
    hAB.pop_back();
    std::vector<uint32_t> x_left = AB;
    x_left.insert(x_left.end(),BC.begin(),BC.end());
    std::vector<double> h_left = hAB;
    h_left.insert(h_left.end(),hBC.begin(),hBC.end());
    std::vector<uint32_t>& x_right = AC;
    std::vector<double> h_right = hAC;

    if(x_left[1] > x_right[1])
    {
        swap(x_left,x_right);
        swap(h_left,h_right);
    }
    
    for(uint32_t y = A._y ; y<= C._y;y++)
    {
        draw_clinal_line(Canvas,{x_left[y-A._y],y},h_left[y-A._y],{x_right[y-A._y],y},h_right[y-A._y],_col);
    }
}

bool Project(camera& _camera,const Coordinate& _unproject,Coordinate& result)
{
    double _temp_x = (_unproject.x*_camera.distance/_unproject.z);
    if(_temp_x >= -_camera.width/2 && _temp_x <= _camera.width/2)
        result.x = _temp_x;
    else 
        return false;
    double _temp_y = (_unproject.y*_camera.distance/_unproject.z);
    if(_temp_y >= -_camera.height/2 && _temp_y <= _camera.height/2)
        result.y = _temp_y;
    else
        return false;
    result.z = (_camera.distance);
    return true;
}

Point zoom(canvas& _canvas,camera& _camera,const Coordinate& p)
{
    Point result;
    result._x = static_cast<uint32_t>(((p.x + _camera.width/2)/_camera.width) * _canvas.pixel_x);
    result._y = static_cast<uint32_t>(((_camera.height/2 - p.y)/_camera.height) * _canvas.pixel_y);
    return result;
}