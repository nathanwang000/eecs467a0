#include <eecs467/vx_gtk_window_base.hpp>
#include <vx/vxo_text.h>
#include <vx/vxo_mat.h>
#include <vx/vx_object.h>
#include <cmath>
#include <vx/vx_resc.h>
#include <vx/vxo_lines.h>
#include <vx/vxo_mesh.h>
#include <vx/vxo_triangle.h>
#include <vx/vxo_pix_coords.h>
#include <vx/vx.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

struct MousePos {
  float x, y;
};

class a0VxWindow : public eecs467::VxGtkWindowBase
{
public:
    
    a0VxWindow(int argc, char** argv)
    : VxGtkWindowBase(argc, argv, 640, 480, 15),
      mouse_pos{0,0},
      linecolor(0),
      trianglecolor(8),
      colors {
	{1.0f, 0.0f, 0.0f, 1.0f},
        {1.0f, 0.65f, 0.0f, 1.0f},
        {1.0f, 1.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 1.0f, 1.0f},
        {0.39f, 0.15f, 0.80f, 1.0f},
        {0.0f, 1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
        {0.5f, 0.5f, 0.5f, 1.0f},
        {0.0f, 0.0f, 0.0f, 0.0f},
	{0.5f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.5f, 0.0f, 1.0f},
        {0.0f, 0.0f, 0.5f, 1.0f},
        {0.5f, 0.5f, 0.0f, 1.0f},
	{0.5f, 0.0f, 0.5f, 1.0f},
        {0.0f, 0.5f, 0.5f, 1.0f}
      },
      npoints(0)
    {
      ifstream fin("../data/robot_poses.txt");
      float x, y, t;
      while (fin >> x) {
	fin >> y >> t;
	npoints++;
	points.push_back(x);
	points.push_back(y);
	theta.push_back(t);
      }
      cout << npoints << endl;
      for (auto i: points) cout << i << " ";
      cout << endl;
    }
    
    virtual int onMouseEvent(vx_layer_t* layer, vx_camera_pos_t* cameraPosition, vx_mouse_event_t* event)
    {
        mouse_pos = MousePos{event->x, event->y};
        return 0;
    }
    
    virtual int onKeyEvent(vx_layer_t* layer, vx_key_event_t* event)
    {
        std::cout << "Key press: Released:" << event->released << " Key:" << event->key_code << '\n';
	switch (event->key_code) {
	case 268: // left
	  std::cout << "left fired" << '\n';
	  trianglecolor = (trianglecolor - 1) % 17;
	  break;
	case 271: // up
	  std::cout << "up fired" << '\n';
	  linecolor = (linecolor + 1) % 18;
	  break;
	case 269: // right
	  std::cout << "right fired" << '\n';
	  trianglecolor = (trianglecolor + 1) % 17;
	  break;
	case 270: // down
	  std::cout << "down fired" << '\n';
	  linecolor = (linecolor - 1) % 18;
	}

        return 0;
    }
    
private:
    MousePos mouse_pos;
    int linecolor;
    int trianglecolor;
    float colors [17][4];
    // robot poses
    int npoints;
    vector<float> points;
    vector<float> theta;
    
    virtual void render(void)
    {
      vx_buffer_t* buf = vx_world_get_buffer(world_, "buf");
      // draw robot poses (triangle)
      for (int i=0; i<npoints; i++) {
	vx_object_t *obj = vxo_chain(
				     vxo_mat_translate2(points[i*2], points[i*2+1]),
				     vxo_mat_rotate_z(theta[i]),
				     vxo_mat_scale2(1,0.1),
				     vxo_triangle(vxo_mesh_style(colors[trianglecolor]))
				     );
	vx_buffer_add_back(buf, obj);
      }

      
      // connect path
      vx_resc_t *verts = vx_resc_copyf(&points[0], npoints*2);
      vx_buffer_add_back(buf,
			 vxo_lines(verts,npoints,GL_LINE_LOOP,vxo_lines_style(colors[linecolor],3.0f)));
      // draw the mouse position
      string mptext = "mouse at (" + to_string(mouse_pos.x) 
	+ ", " + to_string(mouse_pos.y) + ")";
      vx_object_t* mouseText = vxo_text_create(VXO_TEXT_ANCHOR_BOTTOM_RIGHT, mptext.c_str());
      vx_buffer_add_back(buf, vxo_pix_coords(VX_ORIGIN_BOTTOM_RIGHT, mouseText));
      vx_buffer_swap(buf);

    }
};


int main(int argc, char** argv)
{
    a0VxWindow window(argc, argv);
    window.run();
    
    std::cout << "Window closed. Exiting program...\n";
    
    return 0;
}
