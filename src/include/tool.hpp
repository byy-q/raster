#ifndef TOOL_HPP
#define TOOL_HPP

#include <vector>
#include <inttypes.h>
#include "./camera.hpp"
#include "./color.hpp"
#include "./canvas.hpp"

std::vector<uint32_t> interpolate(uint32_t _x1,uint32_t _y1,uint32_t _x2,uint32_t _y2);
std::vector<double> interpolate(uint32_t x1,double h1,uint32_t x2,double h2);
void draw_line(canvas& Canvas,const Point& P1,const Point& p2,const color& _col);
void draw_clinal_line(canvas& Canvas,const Point& A,const double& h1,const Point& B,const double& h2,const color& _col);
void draw_triangle(canvas& Canvas,const Point& A,const Point& B,const Point& C,const color& _col );
void fill_triangle(canvas& Canvas,Point A,Point B,Point C,const color& _col);
void fill_clinal_triangle(canvas& Canvas,Point A,const double& h1,Point B,const double& h2,Point C,const double& h3,const color& _col);
bool Project(camera& _camera,const Coordinate& _unproject,Coordinate&);
Point zoom(canvas&,camera&,const Coordinate&);

#endif