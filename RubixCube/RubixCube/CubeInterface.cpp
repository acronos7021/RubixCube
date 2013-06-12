/*
* @ Author: Alex Veit
* 
* This code implements a visual interface of a ribik's cube
*
* Its purpose is to allow a user to input colors into a virtual cube
*	according to a real scrambled Rubik’s cube. Once the user has correctly 
*	input the colors on all faces of the virtual cube, this interface will instantiate a
*	Cube class (Cube.h/Cube.cpp @ Author: Brian Davis) and its subsequent classes
*	(Block.h/Block.cpp @ Author: Brian Davis, Rotator.h/ Rotator.cpp @ Author: Brian Davis,
*	TransferMoves.h/TransferMoves.cpp @ Author: Brian Davis, Enums.h/Enums.cpp @ Author: Brian Davis)
*	to perform solution algorithms.
*	
*/
#include "CubeInterface.h"
#include <cstdio>

//tell compiler that we will be using some openGL libraries
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" ) 

//tell compiler we want to use pretty controls such as buttons and stuff
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

void CubeInterface::cell_color::set_color(const cell_color& c)
{
	r = c.r;
	g = c.g;
	b = c.b;
	_color = c._color;
}

void CubeInterface::cell_color::set_color(int c)
{
	switch(c)
	{
	case C_BLUE:
		r = 0; g = 0; b = 1.0f;
		break;
	case C_YELLOW:
		r = 1.0f; g = 1.0f; b = 0;
		break;
	case C_GREEN:
		r = 0; g = 1.0f; b = 0;
		break;
	case C_WHITE:
		r = 1.0f; g = 1.0f; b = 1.0f;
		break;
	case C_RED:
		r = 1.0f; g = 0; b = 0;
		break;
	case C_ORANGE:
		r = 1.0f; g = 0.5f; b = 0;
		break;
	case C_GREY:
		r = 0.5f; g = 0.5f; b = 0.5f;
		break;
	}
	_color = c;
}

CubeInterface::cell::cell(const cell &c)
{
	_fx = c._fx;
	_fy = c._fy;
	_fz = c._fz;
	_name = c._name;
	_pos = c._pos;
	_side = c._side;
	_color = c._color;
}

CubeInterface::cell& CubeInterface::cell::operator=(const cell &c)
{
	_fx = c._fx;
	_fy = c._fy;
	_fz = c._fz;
	_name = c._name;
	_pos = c._pos;
	_side = c._side;
	_color = c._color;
	return *this;
}

void CubeInterface::cell::draw_flat()
{
	glLoadName(_name);
	glPushMatrix();
		glTranslatef(_fx,_fy,_fz);
		glColor3f(_color.get_r(),_color.get_g(),_color.get_b());
		glBegin(GL_QUADS);
			glVertex3f(-1.0f, 1.0f, 0.0f);
			glVertex3f( 1.0f, 1.0f, 0.0f);
			glVertex3f( 1.0f,-1.0f, 0.0f);
			glVertex3f(-1.0f,-1.0f, 0.0f);
		glEnd();

		glColor3f(0,0,0);
		glBegin(GL_LINE_LOOP);
			glVertex3f(-1.0f, 1.0f, 0.0f);
			glVertex3f( 1.0f, 1.0f, 0.0f);
			glVertex3f( 1.0f,-1.0f, 0.0f);
			glVertex3f(-1.0f,-1.0f, 0.0f);
		glEnd();
	glPopMatrix();
}

void CubeInterface::cell::draw_3d()
{
	glLoadName(_name);
	glPushMatrix();

		switch(_side)
		{
		case LEFT_SIDE:
			glRotatef(-90,0.0f,1.0f,0.0f);
			break;
		case RIGHT_SIDE:
			glRotatef(90,0.0f,1.0f,0.0f);
			break;
		case BACK_SIDE:
			glRotatef(180,0.0f,1.0f,0.0f);
			break;
		case TOP_SIDE:
			glRotatef(-90,1.0f,0.0f,0.0f);
			break;
		case BOTTOM_SIDE:
			glRotatef(90,1.0f,0.0f,0.0f);
			break;
		}

		switch(_pos)
		{
		case 0:
			glTranslatef(-2.0f,2.0f,3.0f);
			break;
		case 1:
			glTranslatef(0,2.0f,3.0f);
			break;
		case 2:
			glTranslatef(2.0f,2.0f,3.0f);
			break;
		case 3:
			glTranslatef(-2.0f,0,3.0f);
			break;
		case 4:
			glTranslatef(0,0,3.0f);
			break;
		case 5:
			glTranslatef(2.0f,0,3.0f);
			break;
		case 6:
			glTranslatef(-2.0f,-2.0f,3.0f);
			break;
		case 7:
			glTranslatef(0,-2.0f,3.0f);
			break;
		case 8:
			glTranslatef(2.0f,-2.0f,3.0f);
			break;
		}

		
		glColor3f(_color.get_r(),_color.get_g(),_color.get_b());
		glBegin(GL_QUADS);
			glVertex3f(-1.0f, 1.0f, 0.0f);
			glVertex3f( 1.0f, 1.0f, 0.0f);
			glVertex3f( 1.0f,-1.0f, 0.0f);
			glVertex3f(-1.0f,-1.0f, 0.0f);
		glEnd();

		glColor3f(0,0,0);
		glBegin(GL_LINE_LOOP);
			glVertex3f(-1.0f, 1.0f, 0.0f);
			glVertex3f( 1.0f, 1.0f, 0.0f);
			glVertex3f( 1.0f,-1.0f, 0.0f);
			glVertex3f(-1.0f,-1.0f, 0.0f);
		glEnd();
	glPopMatrix();
}

LRESULT CALLBACK CubeInterface::WndProcFrame (HWND frame_wnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CubeInterface *ci = NULL;

	//set user data, in this case, a pointer to my CubeInterface class
	if(message == WM_CREATE)
	{
		//lParam contains a pointer to a CREATESTRUCT
		//(CREATESTRUCT*)lParam)->lpCreateParams contains a pointer to CubeInterface
		ci = (CubeInterface*)((CREATESTRUCT*)lParam)->lpCreateParams;

		//Actual line that stores my class in the window's user data
		SetWindowLongPtr(frame_wnd,GWLP_USERDATA,(LONG_PTR)ci);

		ci->create_child_windows(frame_wnd);

		return 0;
	}

	//Retrieve my class
	ci = (CubeInterface*)GetWindowLongPtr(frame_wnd,GWLP_USERDATA);

	switch (message)
	{
	case WM_CLOSE:
		PostQuitMessage (0);
		return 0;

	case WM_ERASEBKGND:
		return TRUE;

	case WM_SIZE:
		ci->size_frame_window();
		break;
	}

	return DefWindowProc(frame_wnd, message, wParam, lParam);
}

LRESULT CALLBACK CubeInterface::WndProcControls (HWND control_wnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CubeInterface *ci = NULL;

	//set user data, in this case, a pointer to my CubeInterface class
	if(message == WM_CREATE)
	{
		//lParam contains a pointer to a CREATESTRUCT
		//(CREATESTRUCT*)lParam)->lpCreateParams contains a pointer to CubeInterface
		ci = (CubeInterface*)((CREATESTRUCT*)lParam)->lpCreateParams;

		ci->create_controls(control_wnd);

		//Actual line that stores my class in the window's user data
		SetWindowLongPtr(control_wnd,GWLP_USERDATA,(LONG_PTR)ci);
		return 0;
	}

	//Retrieve my class
	ci = (CubeInterface*)GetWindowLongPtr(control_wnd,GWLP_USERDATA);

	switch (message)
	{
	case WM_SIZE:
		ci->resize_control_window();
		break;

	case WM_COMMAND:
		switch(HIWORD(wParam))
		{
		case BN_CLICKED:
			ci->process_button_click(lParam);
			break;
		}
		break;
	}

	return DefWindowProc(control_wnd, message, wParam, lParam);
}

LRESULT CALLBACK CubeInterface::WndProcGL (HWND gl_wnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CubeInterface *ci = NULL;

	//set user data, in this case, a pointer to my CubeInterface class
	if(message == WM_CREATE)
	{
		//lParam contains a pointer to a CREATESTRUCT
		//(CREATESTRUCT*)lParam)->lpCreateParams contains a pointer to CubeInterface
		ci = (CubeInterface*)((CREATESTRUCT*)lParam)->lpCreateParams;

		//Actual line that stores my class in the window's user data
		SetWindowLongPtr(gl_wnd,GWLP_USERDATA,(LONG_PTR)ci);
		return 0;
	}

	//Retrieve my class
	ci = (CubeInterface*)GetWindowLongPtr(gl_wnd,GWLP_USERDATA);

	switch (message)
	{
	case WM_SIZE:
		ci->resize_gl_window();
		return 0;

	case WM_MOUSEMOVE:
		ci->mouse_move_proc(wParam,lParam);
		break;

	case WM_LBUTTONDOWN:
		SetFocus(ci->_gl_wnd);
		ci->gl_select(LOWORD(lParam),HIWORD(lParam));
		break;
	case WM_KEYDOWN:
		ci->_keys[wParam] = true;
		ci->process_key(wParam);
		break;
	case WM_KEYUP:
		ci->_keys[wParam] = false;
		break;

	case WM_PAINT:
		ci->draw_gl_scene();
		break;

	case WM_ERASEBKGND:
		return TRUE;
	}

	return DefWindowProc(gl_wnd, message, wParam, lParam);
}

void CubeInterface::center_cube() { _xrot = 0; _yrot = 0; }

void CubeInterface::create_child_windows(HWND frame_wnd)
{
	_control_wnd = CreateWindow (CUBE_CONTROLS_WIN_CLASS, NULL,
		WS_CHILD | WS_VISIBLE,0, 0, 130, 200,
		frame_wnd, NULL, _hInstance, this);

	_gl_wnd = CreateWindow (CUBE_GL_WIN_CLASS, NULL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | 
		WS_CLIPCHILDREN,200, 0, 200, 200,
		frame_wnd, NULL, _hInstance, this);

	//setup the rendering context
	set_up_gl_context();

	//set initial selected color to blue
	SendMessage(_blue_radio_wnd,BM_CLICK,(WPARAM)NULL,(WPARAM)NULL);
	_selected_color.set_color(C_BLUE);
}

void CubeInterface::create_controls(HWND control_wnd)
{
	CreateWindow ("BUTTON", "Colors",
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,10, 10, 100, 175,
		control_wnd, NULL, _hInstance, NULL);

	_blue_radio_wnd = CreateWindow ("BUTTON", "Blue",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP ,20, 30, 70, 20,
		control_wnd, NULL, _hInstance, NULL);

	_green_radio_wnd = CreateWindow ("BUTTON", "Green",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,20, 55, 70, 20,
		control_wnd, NULL, _hInstance, NULL);

	_orange_radio_wnd = CreateWindow ("BUTTON", "Orange",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,20, 80, 70, 20,
		control_wnd, NULL, _hInstance, NULL);

	_red_radio_wnd = CreateWindow ("BUTTON", "Red",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,20, 105, 70, 20,
		control_wnd, NULL, _hInstance, NULL);

	_white_radio_wnd = CreateWindow ("BUTTON", "White",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,20, 130, 70, 20,
		control_wnd, NULL, _hInstance, NULL);

	_yellow_radio_wnd = CreateWindow ("BUTTON", "Yellow",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,20, 155, 70, 20,
		control_wnd, NULL, _hInstance, NULL);

	_solve_btn_wnd = CreateWindow ("BUTTON", "Solve",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,10, 200, 100, 30,
		control_wnd, NULL, _hInstance, NULL);

	_center_btn_wnd = CreateWindow ("BUTTON", "Center",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,10, 240, 100, 30,
		control_wnd, NULL, _hInstance, NULL);

	 _reset_btn_wnd = CreateWindow ("BUTTON", "Reset",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,10, 280, 100, 30,
		control_wnd, NULL, _hInstance, NULL);
}

void CubeInterface::do_ctrl_z()
{
	if(_keys[KEY_Z] && _ctrl_z.size() > 0)
	{
		_cells[_ctrl_z.back()._pos] = _ctrl_z.back()._cell;
		_ctrl_z.pop_back();
	}
}

void CubeInterface::draw_gl_scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
		

	glPushMatrix();
	glTranslatef(0,10.0f,-50.0f);

	for(int i = 0; i < 54; i++)
		_cells[i].draw_flat();

	glPopMatrix();

	
	glPushMatrix();
	glTranslatef(0,-5.0f,-25.0f);
	//the next two will rotate the 3d cube
	glRotatef(_xrot,1.0f,0.0f,0.0f);
	glRotatef(_yrot,0.0f,1.0f,0.0f);

	//3d cube render will go here
	for(int i = 0; i < 54; i++)
		_cells[i].draw_3d();


	glPopMatrix();
	

	SwapBuffers(_hDC);
}

void CubeInterface::get_rects(RECT *frame_rect, RECT *gl_rect, RECT *control_rect)
{
	if(gl_rect != NULL)			GetClientRect(_gl_wnd,gl_rect);
	if(control_rect != NULL)	GetClientRect(_control_wnd,control_rect);
	if(frame_rect != NULL)		GetClientRect(_frame_wnd,frame_rect);
}

void CubeInterface::gl_select(int x, int y)
{
	GLuint buff[64] = {0};
	GLint hits, view[4];

	RECT gl_rect;
	get_rects(NULL,&gl_rect,NULL);

	//This choose the buffer where store the values for the selection data
	glSelectBuffer(64, buff);
 
	//This retrieve info about the viewport
	glGetIntegerv(GL_VIEWPORT, view);
 
	//Switching in selecton mode
	glRenderMode(GL_SELECT);
 
	//Clearing the name's stack
	//This stack contains all the info about the objects
	glInitNames();
 
	//Now fill the stack with one element (or glLoadName will generate an error)
	glPushName(0);
 
	//Now modify the vieving volume, restricting selection area around the cursor
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
 
	//restrict the draw to an area around the cursor
	gluPickMatrix(x, gl_rect.bottom-y, 1.0f, 1.0f, view);
	gluPerspective(45.0f,(GLfloat)gl_rect.right/(GLfloat)gl_rect.bottom,0.1f,100.0f);
 
	//Draw the objects onto the screen
	glMatrixMode(GL_MODELVIEW);
 
	//draw only the names in the stack, and fill the array
	SwapBuffers(_hDC);
	draw_gl_scene();
 
	//Do you remeber? We do pushMatrix in PROJECTION mode
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
 
	//get number of objects drawed in that area
	//and return to render mode
	hits = glRenderMode(GL_RENDER);
 
	//process hit
	if(hits != 0)
		process_hits(hits,buff);

	glMatrixMode(GL_MODELVIEW);
}

void CubeInterface::init_cells()
{
	float initX = -11;
	float x = initX, y = 2;
	for(int i = 0; i < 36; i++)
	{
		_cells[i].set_name(i);
		_cells[i].set_flat_coord(x,y,0);
		x += 2;
		if(i == 11 || i == 23)
		{
			x = initX;
			y -= 2;
		}
	}

	initX = -5;
	x = initX; y = 8;
	int count = 0, count2 = 0;
	for(int i = 36; i < 54; i++)
	{
		_cells[i].set_name(i);
		_cells[i].set_flat_coord(x,y,0);
		x += 2;
		if(count == 2)
		{
			count = 0;
			x = initX;
			y -= 2;
			if(count2 == 6)
				y = -4;
		}
		else
		{
			count++;
			count2++;
		}
	}

	count = 0;
	count2 = 0;
	int pos = 0;
	for(int i = 0; i < 36; i++)
	{
		if(count < 3)
			_cells[i].set_side(LEFT_SIDE);
		else if(count < 6)
			_cells[i].set_side(FRONT_SIDE);
		else if(count < 9)
			_cells[i].set_side(RIGHT_SIDE);
		else if(count < 12)
			_cells[i].set_side(BACK_SIDE);

		_cells[i].set_position(pos);

		pos++;
		count++;
		count2++;

		if(pos == 3 && count2 < 12)
			pos = 0;
		else if(pos == 6 && count2 < 24)
			pos = 3;
		else if(pos == 9 && count2 < 36)
			pos = 6;

		if(count == 12)
			count = 0;
	}

	count = 0;
	pos = 0;
	for(int i = 36; i < 54; i++)
	{
		if(count < 9)
			_cells[i].set_side(TOP_SIDE);
		else
			_cells[i].set_side(BOTTOM_SIDE);

		_cells[i].set_position(pos);

		pos++;
		if(pos == 9)
			pos = 0;

		count++;
	}
	_cells[13].set_color(C_RED);
	_cells[16].set_color(C_YELLOW);
	_cells[19].set_color(C_ORANGE);
	_cells[22].set_color(C_WHITE);
	_cells[40].set_color(C_BLUE);
	_cells[49].set_color(C_GREEN);
}

void CubeInterface::init_gl()							// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
}

void CubeInterface::mouse_move_proc(WPARAM wParam, LPARAM lParam)
{
	if(wParam != MK_RBUTTON)
		return;

	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	if(x > _mouse_x)
		_yrot += 1.0f;
	else if(x < _mouse_x)
		_yrot -= 1.0f;

	if(y > _mouse_y)
		_xrot += 1.0f;
	else if(y < _mouse_y)
		_xrot -= 1.0f;
	
	_mouse_x = x;
	_mouse_y = y;
}

void CubeInterface::process_button_click(LPARAM lParam)
{
	if(_blue_radio_wnd == (HWND)lParam)
		_selected_color.set_color(C_BLUE);
	else if(_red_radio_wnd == (HWND)lParam)
		_selected_color.set_color(C_RED);
	else if(_yellow_radio_wnd == (HWND)lParam)
		_selected_color.set_color(C_YELLOW);
	else if(_green_radio_wnd == (HWND)lParam)
		_selected_color.set_color(C_GREEN);
	else if(_white_radio_wnd == (HWND)lParam)
		_selected_color.set_color(C_WHITE);
	else if(_orange_radio_wnd == (HWND)lParam)
		_selected_color.set_color(C_ORANGE);
	else if(_solve_btn_wnd == (HWND)lParam)
	{
		if(validade_color_count())
		{
			//call Brian's solution
		}
	}
	else if(_center_btn_wnd == (HWND)lParam)
		center_cube();
	else if(_reset_btn_wnd == (HWND)lParam)
		reset();

	SetFocus(_gl_wnd);
}

void CubeInterface::process_hits(GLint hits, GLuint buff[])
{
	GLuint *ptr = (GLuint *) buff;
	GLuint minZ = 0xffffffff;
	GLuint names,numberOfNames, *ptrNames;
	for (int i = 0; i < hits; i++)
	{	
		names = *ptr;
		ptr++;
		if (*ptr < minZ)
		{
			numberOfNames = names;
			minZ = *ptr;
			ptrNames = ptr+2;
		}
		ptr += names+2;
	}
	switch(*ptrNames)
	{
	case 13:
	case 16:
	case 19:
	case 22:
	case 40:
	case 49:
		return;
	}
	int color_count = 0;
	for(int i = 0; i < 54; i++)
	{
		if(_cells[i].get_color() == _selected_color.get_color())
			color_count++;
		if(color_count == 9)
			return;
	}
	_ctrl_z.push_back(ctrl_z_struct(*ptrNames,_cells[*ptrNames]));
	_cells[*ptrNames].set_color(_selected_color);	
}

void CubeInterface::process_key(WPARAM wParam)
{
	HWND radio;
	int color;
	switch(wParam)
	{
	case KEY_B:
		radio = _blue_radio_wnd; color = C_BLUE;
		break;
	case KEY_G:
		radio = _green_radio_wnd; color = C_GREEN;
		break;
	case KEY_O:
		radio = _orange_radio_wnd; color = C_ORANGE;
		break;
	case KEY_R:
		radio = _red_radio_wnd; color = C_RED;
		break;
	case KEY_W:
		radio = _white_radio_wnd; color = C_WHITE;
		break;
	case KEY_Y:
		radio = _yellow_radio_wnd; color = C_YELLOW;
		break;
	case KEY_Z:
		do_ctrl_z();
		return;
	default:
		return;
	}
	SendMessage(radio,BM_CLICK,(WPARAM)NULL,(WPARAM)NULL);
	_selected_color.set_color(color);
}

void CubeInterface::reg_win_class()
{
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof (wc));

	wc.style = CS_PARENTDC;
	wc.lpfnWndProc = WndProcFrame;
	wc.hInstance = _hInstance;
	wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor (NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
	wc.lpszClassName = CUBE_FRAME_WIN_CLASS;
	RegisterClass (&wc);

	wc.style = CS_CLASSDC;
	wc.lpfnWndProc = WndProcControls;
	wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
	wc.lpszClassName = CUBE_CONTROLS_WIN_CLASS;
	RegisterClass (&wc);

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProcGL;
	wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
	wc.lpszClassName = CUBE_GL_WIN_CLASS;
	RegisterClass (&wc);
}

void CubeInterface::release_resources()
{
	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(_hRC);
	ReleaseDC(_gl_wnd,_hDC);
	DestroyWindow(_gl_wnd);
	DestroyWindow(_control_wnd);
	DestroyWindow(_frame_wnd);
}

void CubeInterface::reset()
{
	for(int i = 0; i < 54; i++)
	{
		switch(i)
		{
		case 13:
		case 16:
		case 19:
		case 22:
		case 40:
		case 49:
			//dont do anything to center colors
			//they are fixed
			break;
		default:
			_cells[i].set_color(C_GREY);
		}
	}
	_ctrl_z.clear();
	center_cube();
}

void CubeInterface::resize_control_window()
{
	RECT frame_rect, control_rect;
	get_rects(&frame_rect,NULL,&control_rect);
	MoveWindow(_control_wnd,0,0,control_rect.right,frame_rect.bottom,TRUE);
}

void CubeInterface::resize_gl_scene(RECT frame_rect, RECT control_rect)		// Resize And Initialize The GL Window
{
	GLsizei width = frame_rect.right-control_rect.right;
	GLsizei height = frame_rect.bottom;

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void CubeInterface::resize_gl_window()
{
	RECT frame_rect, gl_rect, control_rect;
	get_rects(&frame_rect,&gl_rect,&control_rect);
	MoveWindow(_gl_wnd,control_rect.right,gl_rect.top,frame_rect.right-control_rect.right,frame_rect.bottom,TRUE);
	resize_gl_scene(frame_rect,control_rect);
}

void CubeInterface::set_up_gl_context()
{
	int		PixelFormat;				// Holds The Results After Searching For A Match
	static	PIXELFORMATDESCRIPTOR pfd=	// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),	// Size Of This Pixel Format Descriptor
		1,								// Version Number
		PFD_DRAW_TO_WINDOW |			// Format Must Support Window
		PFD_SUPPORT_OPENGL |			// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,				// Must Support Double Buffering
		PFD_TYPE_RGBA,					// Request An RGBA Format
		16,								// Select Our Color Depth
		0, 0, 0, 0, 0, 0,				// Color Bits Ignored
		0,								// No Alpha Buffer
		0,								// Shift Bit Ignored
		0,								// No Accumulation Buffer
		0, 0, 0, 0,						// Accumulation Bits Ignored
		16,								// 16Bit Z-Buffer (Depth Buffer)  
		0,								// No Stencil Buffer
		0,								// No Auxiliary Buffer
		PFD_MAIN_PLANE,					// Main Drawing Layer
		0,								// Reserved
		0, 0, 0							// Layer Masks Ignored
	};

	_hDC = GetDC(_gl_wnd);
	PixelFormat = ChoosePixelFormat(_hDC,&pfd);
	SetPixelFormat(_hDC,PixelFormat,&pfd);
	_hRC=wglCreateContext(_hDC);
	wglMakeCurrent(_hDC,_hRC);

	init_gl();
}

void CubeInterface::size_frame_window()
{
	SendMessage(_control_wnd,WM_SIZE,(WPARAM)NULL,(WPARAM)NULL);
	SendMessage(_gl_wnd,WM_SIZE,(WPARAM)NULL,(WPARAM)NULL);
}

bool CubeInterface::validade_color_count()
{
	int colors[6];
	ZeroMemory(colors,sizeof(int)*6);
	for(int i = 0; i < 54; i++)
	{
		switch(_cells[i].get_color())
		{
		case C_BLUE:
			colors[C_BLUE]++;
			break;
		case C_GREEN:
			colors[C_GREEN]++;
			break;
		case C_RED:
			colors[C_RED]++;
			break;
		case C_ORANGE:
			colors[C_ORANGE]++;
			break;
		case C_WHITE:
			colors[C_WHITE]++;
			break;
		case C_YELLOW:
			colors[C_YELLOW]++;
			break;
		}
	}

	for(int i = 0; i < 6; i++)
	{
		if(colors[i] != 9)
		{
			char msg[256];
			sprintf_s(msg,"Invalid Cube:\n\nBlue: %d\nGreen: %d\nOrange: %d\nRed: %d\nWhite: %d\nYellow: %d",
				colors[0],colors[1],colors[2],colors[3],colors[4],colors[5]);
			MessageBox(_frame_wnd,msg,"Colors",MB_OK|MB_ICONEXCLAMATION);
			return false;
		}
	}
	return true;
}

CubeInterface::CubeInterface(HINSTANCE hInstance) : _hInstance(hInstance)
{
	_xrot = _yrot = 0;
	_mouse_x = _mouse_y = 0;
	ZeroMemory(&_keys,sizeof(bool)*256);
	reg_win_class();
	init_cells();
}

int CubeInterface::msg_loop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof (msg));

	_frame_wnd = CreateWindow (CUBE_FRAME_WIN_CLASS, "Cube Input", 
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,0, 0, 800, 600,
		NULL, NULL, GetModuleHandle(NULL), this);

	size_frame_window();

	SetFocus(_gl_wnd);

	while(GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		draw_gl_scene();
	}

	return (int)msg.message;
}
