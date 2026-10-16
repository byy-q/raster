#include "../include/camera.hpp"
#include "../include/canvas.hpp"
#include "../include/color.hpp"
#include "../include/tool.hpp"
#include "../include/world_objects.hpp"


canvas Canvas(2560,1280,"images/image1.bmp",{255,255,255});
camera _camera(1.0,2.0,4.0);

int main()
{
    triangle triangle_1({-2.1,-0.3,3.0},{-1.0,-0.3,3.0},{-1.6,0.5,3.0});
    cube cube_1(
        {-1.3927,-0.5601,4.9850},{0.2456,-0.5601,3.8378},
        {0.2456,1.2525,4.5302},{-1.3927,1.2525,5.6774},
        {-0.2456,-1.2525,6.4698},{1.3927,-1.2525,5.3226},
        {1.3927,0.5601,6.0150},{-0.2456,0.5601,7.1622});
    triangle_1.draw(_camera,Canvas,{255,0,0});
    cube_1.draw(Canvas,_camera,{0,0,255});

}