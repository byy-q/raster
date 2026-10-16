/** 
 * @brief:this file contains the objetcs exists in the three dimensionnal coordinate system
 * we create objects in the real three dimensional world
 * and project them to the screen ,then draw them on the canvas
*/
#ifndef WORLD_HPP
#define WORLD_HPP

#include "camera.hpp"
#include "canvas.hpp"
#include "color.hpp"
#include "tool.hpp"
#include <vector>
#include <algorithm>

class triangle
{
    public:
        triangle():A(0.0,0.0,0.0),B(0.0,0.0,0.0),C(0.0,0.0,0.0){}
        triangle(const Coordinate& _A,const Coordinate& _B,const Coordinate& _C)
        :A(_A),B(_B),C(_C){}
        void draw(camera& _camera,canvas& _canvas,const color& _col){

            Coordinate A_on_camera;
            if(!Project(_camera,A,A_on_camera))
                return;
            //if A is out the screen,we simply undo draw entire triangle
            //we will transform this into cut later
            Coordinate B_on_camera; 
            if(!Project(_camera,B,B_on_camera))
                return ;  
            Coordinate C_on_camera;
            if(!Project(_camera,C,C_on_camera))
                return;

            Point A_on_canvas = zoom(_canvas,_camera,A_on_camera);
            Point B_on_canvas = zoom(_canvas,_camera,B_on_camera);
            Point C_on_canvas = zoom(_canvas,_camera,C_on_camera);

            fill_triangle(_canvas,A_on_canvas,B_on_canvas,C_on_canvas,_col);

        }
        void draw(camera& _camera,canvas& _canvas,const color& _col,
            const double& h1,const double& h2,const double& h3)
        {
 
            Coordinate A_on_camera;
            if(!Project(_camera,A,A_on_camera))
                return;
            Coordinate B_on_camera; 
            if(!Project(_camera,B,B_on_camera))
                return ;  
            Coordinate C_on_camera;
            if(!Project(_camera,C,C_on_camera))
                return;
            
                Point A_on_canvas = zoom(_canvas,_camera,A_on_camera);
            Point B_on_canvas = zoom(_canvas,_camera,B_on_camera);
            Point C_on_canvas = zoom(_canvas,_camera,C_on_camera);

            fill_clinal_triangle(_canvas,A_on_canvas,h1,B_on_canvas,h2,C_on_canvas,h3,_col);           

        }

    private:
        Coordinate A;
        Coordinate B;
        Coordinate C;
};


class cube
{
    public:
        cube(){
            vertex = new Coordinate[8];
            for(int i = 0; i<8;i++)
                vertex[i] = {0.0,0.0,0.0}; 
        }
        cube(const Coordinate& A,const Coordinate& B,const Coordinate& C,const Coordinate& D
        ,const Coordinate& E,const Coordinate&F,const Coordinate& G,const Coordinate&H){
            vertex = new Coordinate[8];
            vertex[0] = A;vertex[1] = B;vertex[2] = C;vertex[3] = D;
            vertex[4] = E;vertex[5] = F;vertex[6] = G;vertex[7] = H;
        }

        void draw(canvas& _canvas,camera& _camera,const color& _col)
        {
            Coordinate* projected_vertex = new Coordinate[8];
            for(int i=0;i<8;i++)
            {
                if(!Project(_camera,vertex[i],projected_vertex[i]))
                {
                    delete[] projected_vertex;
                    return;
                }
            }

            fill_triangle(_canvas,zoom(_canvas,_camera,projected_vertex[0]),zoom(_canvas,_camera,projected_vertex[1]),zoom(_canvas,_camera,projected_vertex[2]),_col);
            fill_triangle(_canvas,zoom(_canvas,_camera,projected_vertex[0]),zoom(_canvas,_camera,projected_vertex[2]),zoom(_canvas,_camera,projected_vertex[3]),_col);
            fill_triangle(_canvas,zoom(_canvas,_camera,projected_vertex[4]),zoom(_canvas,_camera,projected_vertex[0]),zoom(_canvas,_camera,projected_vertex[3]),_col);
            fill_triangle(_canvas,zoom(_canvas,_camera,projected_vertex[4]),zoom(_canvas,_camera,projected_vertex[3]),zoom(_canvas,_camera,projected_vertex[7]),_col);
            fill_triangle(_canvas,zoom(_canvas,_camera,projected_vertex[5]),zoom(_canvas,_camera,projected_vertex[4]),zoom(_canvas,_camera,projected_vertex[7]),_col);
            fill_triangle(_canvas,zoom(_canvas,_camera,projected_vertex[5]),zoom(_canvas,_camera,projected_vertex[7]),zoom(_canvas,_camera,projected_vertex[6]),_col);
            fill_triangle(_canvas,zoom(_canvas,_camera,projected_vertex[1]),zoom(_canvas,_camera,projected_vertex[5]),zoom(_canvas,_camera,projected_vertex[6]),_col);
            fill_triangle(_canvas,zoom(_canvas,_camera,projected_vertex[1]),zoom(_canvas,_camera,projected_vertex[6]),zoom(_canvas,_camera,projected_vertex[2]),_col);
            fill_triangle(_canvas,zoom(_canvas,_camera,projected_vertex[4]),zoom(_canvas,_camera,projected_vertex[5]),zoom(_canvas,_camera,projected_vertex[1]),_col);
            fill_triangle(_canvas,zoom(_canvas,_camera,projected_vertex[4]),zoom(_canvas,_camera,projected_vertex[1]),zoom(_canvas,_camera,projected_vertex[0]),_col);
            fill_triangle(_canvas,zoom(_canvas,_camera,projected_vertex[2]),zoom(_canvas,_camera,projected_vertex[6]),zoom(_canvas,_camera,projected_vertex[7]),_col);
            fill_triangle(_canvas,zoom(_canvas,_camera,projected_vertex[2]),zoom(_canvas,_camera,projected_vertex[7]),zoom(_canvas,_camera,projected_vertex[3]),_col);
            delete[] projected_vertex;
        }
        ~cube(){
            delete[] vertex;
        }
    private:
        Coordinate* vertex;
};

#endif