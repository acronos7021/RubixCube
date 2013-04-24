/*
* @ Author: Alex Veit
* 
* This code defines a visual interface class of a ribik's cube
*	
*/
#ifndef _CUBE_INTERFACE_
#define _CUBE_INTERFACE_
#include <windows.h>
#include <gl\glu.h>
#include <vector>

class CubeInterface
{

#define CUBE_FRAME_WIN_CLASS	"CUBE_FRAME_WIN_CLASS"
#define CUBE_CONTROLS_WIN_CLASS "CUBE_CONTROLS_WIN_CLASS"
#define CUBE_GL_WIN_CLASS		"CUBE_GL_WIN_CLASS"

#define KEY_B		0x42
#define KEY_G		0x47
#define KEY_O		0x4F
#define KEY_R		0x52
#define KEY_W		0x57
#define KEY_Y		0x59
#define KEY_Z		0x5A
#define KEY_CTRL	0x11

	//class to hold RGB values
	class cell_color
	{
	#define C_BLUE		0
	#define C_GREEN		1
	#define C_ORANGE	2
	#define C_RED		3
	#define C_WHITE		4
	#define C_YELLOW	5
	#define C_GREY		6

		float r,g,b;
		int _color;

	public:
		cell_color() : r(0.5f), g(0.5f), b(0.5f), _color(C_GREY) {}

		float get_r() { return r; }

		float get_g() { return g; }

		float get_b() { return b; }

		int get_color() { return _color; }

		void set_color(const cell_color& c);

		void set_color(int c);
	};

	//represents a cell in the cube
	class cell
	{
	#define LEFT_SIDE	0
	#define FRONT_SIDE	1
	#define RIGHT_SIDE	2
	#define BACK_SIDE	3
	#define TOP_SIDE	4
	#define BOTTOM_SIDE	5

		float _fx, _fy, _fz;
		int _name;
		int _side;
		int _pos;

		cell_color _color;
	public:
		cell() : _fx(0.0f), _fy(0.0f), _fz(0.0f), _name(0) {}

		cell(const cell &c);

		int get_color() { return _color.get_color(); }

		cell& operator=(const cell &c);

		//each cell will be addigned a name (int value) when it is drawn
		//do we can identify it when the user clicks on it
		void set_name(int name) { _name = name; }

		void set_color(cell_color c) { _color.set_color(c); }

		void set_color(int c) { _color.set_color(c); }

		//set drawing coordinates for flat x y z
		void set_flat_coord(float x, float y, float z) { _fx = x; _fy = y; _fz = z; }

		void set_side(int side) { _side = side; }

		void set_position(int pos) { _pos = pos; }

		//call openGL code to render flat cell on screen
		void draw_flat();

		//call openGL code to render 3d cell on screen
		void draw_3d();

	};

	struct ctrl_z_struct
	{
		int _pos;
		cell _cell;

		ctrl_z_struct(int p, cell c): _pos(p), _cell(c) {}
	};

	//keeps rotation values of mouse
	GLfloat _xrot, _yrot;

	//keeps x,y coordinates of mouse
	int _mouse_x, _mouse_y;

	//handle to module instance
	HINSTANCE _hInstance;

	//handle to my custom windows
	HWND _frame_wnd;
	HWND _control_wnd;
	HWND _gl_wnd;

	//handle to win32 API radio buttons
	HWND _blue_radio_wnd;
	HWND _green_radio_wnd;
	HWND _red_radio_wnd;
	HWND _white_radio_wnd;
	HWND _yellow_radio_wnd;
	HWND _orange_radio_wnd;

	//handle to win32 API push button
	HWND _solve_btn_wnd;
	HWND _reset_btn_wnd;
	HWND _center_btn_wnd;

	//handle to a device context
	//this will hold the gl_wnd DC
	HDC _hDC;

	//handle do openGL rendering context
	HGLRC _hRC;

	//this will store the color selected by the user
	//when he clicks on one of the 6 radio buttons representing the colors
	cell_color _selected_color;

	//cube cells
	cell _cells[54];

	std::vector<ctrl_z_struct> _ctrl_z;

	bool _keys[256];

	//static callback function to process frame window messages
	static LRESULT CALLBACK WndProcFrame (HWND frame_wnd, UINT message, WPARAM wParam, LPARAM lParam);

	//static callback function to process control window messages
	static LRESULT CALLBACK WndProcControls (HWND control_wnd, UINT message, WPARAM wParam, LPARAM lParam);

	//static callback function to OpenGL frame window messages
	static LRESULT CALLBACK WndProcGL (HWND gl_wnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	void center_cube();
	
	//create control and GL window
	void create_child_windows(HWND frame_wnd);

	//create radio and push buttons
	void create_controls(HWND control_wnd);
	
	void do_ctrl_z();

	//render the cube on the screen
	void draw_gl_scene();
	
	//get rectangle for client area of my 3 windows
	void get_rects(RECT *frame_rect, RECT *gl_rect, RECT *control_rect);
	
	//identify what cell has been clicked
	void gl_select(int x, int y);

	//initialize cells with their respective coordinates
	void init_cells();

	//initialyze a few openGL parameters
	//I just copyed this from a tutorial
	void init_gl();

	void mouse_move_proc(WPARAM wParam, LPARAM lParam);
	
	//identify witch radio button was pushed
	void process_button_click(LPARAM lParam);
	
	//process hits after GL selection mode
	//basicaly identify witch cell the user clicked
	void process_hits(GLint hits, GLuint buff[]);

	//process keyboard events
	void process_key(WPARAM wParam);
	
	//Register window classes so they can be instantiated in the future
	void reg_win_class();

	//release handles
	void release_resources();

	//resets all cells to color gray
	void reset();

	//resize control window
	void resize_control_window();
	
	//resize GL viewport
	void resize_gl_scene(RECT frame_rect, RECT control_rect);

	//resize OpenGL window
	void resize_gl_window();
	
	//this is where we tell openGL in which device context
	//the rendering will take place
	void set_up_gl_context();

	//send resizing message to control and OpenGL windows
	void size_frame_window();
	
	bool validade_color_count();

public:

	//construct CubeInterface object
	CubeInterface(HINSTANCE hInstance);

	//the main message (GUI) loop
	int msg_loop();
};

#endif