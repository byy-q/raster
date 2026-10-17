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
#include "matrix.hpp"
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



class model
{

};

class instance;

inline std::vector<instance*>& all_instances()
{
    static std::vector<instance*> instances;
    return instances;
}

class instance
{
    public:
        instance()
        {
            all_instances().push_back(this);
        }

        virtual ~instance()
        {
            std::vector<instance*>& instances = all_instances();
            instances.erase(std::remove(instances.begin(), instances.end(), this), instances.end());
        }

        instance(const instance&) = delete;
        instance& operator=(const instance&) = delete;
        instance(instance&&) = delete;
        instance& operator=(instance&&) = delete;

        virtual void extend(const double&) = 0;
        virtual void move(const _Vector&) = 0;
        virtual void rotate(const double&,rotation_axis)=0;
        virtual void draw(canvas& ,camera&,const color&)=0;

    protected:
        friend class camera;

        void apply_transform(const matrix4x4& operation)
        {
            transform = operation * transform;
        }

        matrix4x4 transform;
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
            apply_transform(matrix4x4::scaling(r));
        }    

        void move(const _Vector& _v)override
        {
            apply_transform(matrix4x4::translation(_v));
        }

        void draw(canvas& _canvas, camera& _camera, const color& _col) override
        {
            std::array<Coordinate, 8> projected_vertices;
            std::array<Coordinate, 8> transformed_vertices;
            for (std::size_t i = 0; i < init_cube.vertex.size(); ++i)
            {
                transformed_vertices[i] = transform.transform_point(init_cube.vertex[i]);
                if (!Project(_camera, transformed_vertices[i], projected_vertices[i]))
                    return;
            }

            std::array<std::size_t, 12> draw_order;
            for (std::size_t i = 0; i < draw_order.size(); ++i)
                draw_order[i] = i;

            std::sort(draw_order.begin(), draw_order.end(),
                [this, &transformed_vertices](std::size_t left, std::size_t right)
                {
                    double left_depth = 0.0;
                    double right_depth = 0.0;
                    for (std::size_t i = 0; i < 3; ++i)
                    {
                        left_depth += transformed_vertices[init_cube.triangles[left][i]].z;
                        right_depth += transformed_vertices[init_cube.triangles[right][i]].z;
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
            apply_transform(matrix4x4::rotation(angle_degrees, axis));
        }
    private:
        cube_model init_cube;


};


#endif
