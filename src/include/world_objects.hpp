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
#include <array>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdexcept>

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

            const std::array<std::array<std::size_t, 4>, 6> faces = {{
                {{0, 1, 2, 3}}, {{4, 0, 3, 7}}, {{5, 4, 7, 6}},
                {{1, 5, 6, 2}}, {{4, 5, 1, 0}}, {{2, 6, 7, 3}}
            }};
            const std::array<double, 6> intensities = {{1.0, 0.72, 0.5, 0.82, 0.62, 0.9}};
            std::array<std::size_t, 6> draw_order = {{0, 1, 2, 3, 4, 5}};
            std::sort(draw_order.begin(), draw_order.end(),
                [this, &faces](std::size_t left, std::size_t right)
                {
                    double left_depth = 0.0;
                    double right_depth = 0.0;
                    for (std::size_t i = 0; i < 4; ++i)
                    {
                        left_depth += vertex[faces[left][i]].z;
                        right_depth += vertex[faces[right][i]].z;
                    }
                    return left_depth > right_depth;
                });

            for (const std::size_t face_index : draw_order)
            {
                const auto& face = faces[face_index];
                const color face_color = _col * intensities[face_index];
                const Point a = zoom(_canvas, _camera, projected_vertex[face[0]]);
                const Point b = zoom(_canvas, _camera, projected_vertex[face[1]]);
                const Point c = zoom(_canvas, _camera, projected_vertex[face[2]]);
                const Point d = zoom(_canvas, _camera, projected_vertex[face[3]]);
                fill_triangle(_canvas, a, b, c, face_color);
                fill_triangle(_canvas, a, c, d, face_color);
            }
            delete[] projected_vertex;
        }
        ~cube(){
            delete[] vertex;
        }
    private:
        Coordinate* vertex;
};

enum class rotation_axis
{
    x,
    y,
    z
};

class model
{

};

class instance
{
    public:
        virtual void extend(const double&) = 0;
        virtual void move(const _Vector&) = 0;
        virtual void rotate(const double&,rotation_axis)=0;
        virtual void draw(canvas& ,camera&,const color&)=0;
};



class cube_model : public model
{
    public:
        std::array<Coordinate,8> vertex = {Coordinate(1,1,1),Coordinate(1,-1,1),Coordinate(1,1,-1),Coordinate(1,-1,-1),
            Coordinate(-1,1,1),Coordinate(-1,-1,1),Coordinate(-1,1,-1),Coordinate(-1,-1,-1)};
        std::array<std::array<size_t,3>,12> triangles = {{
            {{0,1,3}}, {{0,3,2}},
            {{4,6,7}}, {{4,7,5}},
            {{0,2,6}}, {{0,6,4}},
            {{1,5,7}}, {{1,7,3}},
            {{0,4,5}}, {{0,5,1}},
            {{2,3,7}}, {{2,7,6}}
        }};
        std::array<double, 12> intensities = {{
            1.0, 1.0, 0.72, 0.72,
            0.5, 0.5, 0.82, 0.82,
            0.62, 0.62, 0.9, 0.9
        }};
};


class cube_instance : public instance
{
    public:
        void extend(const double& r)override{
            for(int i=0;i<8;i++)    
                init_cube.vertex[i]*=r;
        }    

        void move(const _Vector& _v)override
        {
            for(int i=0;i<8;i++)
                init_cube.vertex[i]+=_v;
        }

        void draw(canvas& _canvas, camera& _camera, const color& _col) override
        {
            std::array<Coordinate, 8> projected_vertices;
            for (std::size_t i = 0; i < init_cube.vertex.size(); ++i)
            {
                if (!Project(_camera, init_cube.vertex[i], projected_vertices[i]))
                    return;
            }

            std::array<std::size_t, 12> draw_order;
            for (std::size_t i = 0; i < draw_order.size(); ++i)
                draw_order[i] = i;

            std::sort(draw_order.begin(), draw_order.end(),
                [this](std::size_t left, std::size_t right)
                {
                    double left_depth = 0.0;
                    double right_depth = 0.0;
                    for (std::size_t i = 0; i < 3; ++i)
                    {
                        left_depth += init_cube.vertex[init_cube.triangles[left][i]].z;
                        right_depth += init_cube.vertex[init_cube.triangles[right][i]].z;
                    }
                    return left_depth > right_depth;
                });

            for (const std::size_t triangle_index : draw_order)
            {
                const auto& triangle_vertices = init_cube.triangles[triangle_index];
                const Point a = zoom(_canvas, _camera, projected_vertices[triangle_vertices[0]]);
                const Point b = zoom(_canvas, _camera, projected_vertices[triangle_vertices[1]]);
                const Point c = zoom(_canvas, _camera, projected_vertices[triangle_vertices[2]]);
                fill_triangle(_canvas, a, b, c, _col * init_cube.intensities[triangle_index]);
            }
        }

        void rotate(const double& angle_degrees, rotation_axis axis)override
        {
            constexpr double pi = 3.14159265358979323846;
            const double angle_radians = angle_degrees * pi / 180.0;
            const double cosine = std::cos(angle_radians);
            const double sine = std::sin(angle_radians);
            std::array<std::array<double, 3>, 3> rotation_matrix = {{
                {{1.0, 0.0, 0.0}},
                {{0.0, 1.0, 0.0}},
                {{0.0, 0.0, 1.0}}
            }};

            switch (axis)
            {
                case rotation_axis::x:
                    rotation_matrix = {{
                        {{1.0, 0.0, 0.0}},
                        {{0.0, cosine, -sine}},
                        {{0.0, sine, cosine}}
                    }};
                    break;
                case rotation_axis::y:
                    rotation_matrix = {{
                        {{cosine, 0.0, sine}},
                        {{0.0, 1.0, 0.0}},
                        {{-sine, 0.0, cosine}}
                    }};
                    break;
                case rotation_axis::z:
                    rotation_matrix = {{
                        {{cosine, -sine, 0.0}},
                        {{sine, cosine, 0.0}},
                        {{0.0, 0.0, 1.0}}
                    }};
                    break;
                default:
                    throw std::invalid_argument("Invalid rotation axis");
            }

            for (std::size_t i = 0; i < init_cube.vertex.size(); ++i)
            {
                const Coordinate& vertex = init_cube.vertex[i];
                init_cube.vertex[i] = Coordinate(
                    rotation_matrix[0][0] * vertex.x +
                        rotation_matrix[0][1] * vertex.y +
                        rotation_matrix[0][2] * vertex.z,
                    rotation_matrix[1][0] * vertex.x +
                        rotation_matrix[1][1] * vertex.y +
                        rotation_matrix[1][2] * vertex.z,
                    rotation_matrix[2][0] * vertex.x +
                        rotation_matrix[2][1] * vertex.y +
                        rotation_matrix[2][2] * vertex.z);
            }
        }
    private:
        cube_model init_cube;


};


#endif
