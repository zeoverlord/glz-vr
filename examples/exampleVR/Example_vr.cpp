// Copyright 2013 Peter Wallstr�m
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and limitations under the License.

// while it is not required i like to request a few things
// 1. please do share any meaningfull/usefull changes/additions/fixes you make with me so that i could include it in any future version
// 2. likewise do share any ideas for improvements
// 3. If you make something comersiol or at least something you release publicly that relies on this code then i would like to know and maybe use in my CV
// 4. Please do include me in your credits

// glz 2D example app - i wouldn't say it produces any meningfull results, but it's a start.
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git



#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <gl\glext.h>
#include "Example_vr.h"
#include <fstream>
#include <math.h>
#include "..\..\glz-core\app\appbase.h"
#include "..\..\glz-core\3d\geo.h"
#include "..\..\glz-core\shader\shader.h"
#include "..\..\glz-core\utilities\glz.h"
#include "..\..\glz-core\utilities\vectormath.h"
#include "..\..\glz-core\utilities\node3.h"
#include "..\..\glz-core\image\tex.h"
#include "..\..\glz-core\2d\geo-2d.h"
#include "..\..\glz-core\2d\2d-utilities.h"
#include "..\..\glz-core\3d\geo-generate.h"
#include "..\..\glz-core\effects\particle.h"
#include "..\..\glz-core\utilities\resourcemanager.h"
#include "..\..\glz-core\input\input.h"

using namespace std;
using namespace GLZ;

#pragma comment( lib, "opengl32.lib" )							// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )								// Search For GLu32.lib While Linking

#ifndef CDS_FULLSCREEN											// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4										// Compilers. By Defining It This Way,
#endif															// We Can Avoid Errors


// User Defined Variables
float		angle = 0, width, height;												// Used To Rotate The Triangles
unsigned int vao[16], vao_num[16];
glzMatrix m;
glzMatrix mnew;


char tbuffer[160];
char tbuffer2[160];
float texttimer = 0;
float spriteframetimer = 0;
int spriteframe = 0;


glzQuaternion qr;
glzQuaternion q;
glzQuaternion q2;
glzQuaternion q3;
glzQuaternion qn;

node3 tankpos(vert3(0, -2, -17));

int gamestate = 2;

img_head img;
unsigned char* data;

float aspect = 1.0f;


static PFNGLUSEPROGRAMPROC						glUseProgram;
static PFNGLUNIFORM1IPROC                       glUniform1i;
static PFNGLUNIFORMMATRIX4FVPROC                glUniformMatrix4fv;
static PFNGLUNIFORMMATRIX4DVPROC                glUniformMatrix4dv;
static PFNGLUNIFORM4FARBPROC                    glUniform4f;
static PFNGLGETUNIFORMLOCATIONPROC              glGetUniformLocation;

int WINDOW_HEIGHT;
int WINDOW_WIDTH;

ExampleVRState::ExampleVRState() :
	angle(0),
	texttimer(0),
	spriteframetimer(0),
	spriteframe(0),
	gamestate(2)
{

	tankpos.setPosition(vert3(0.0f, -2.0f, -17.0f));
}

bool ExampleVRState::Initialize(int width, int height)					// Any GL Init Code & User Initialiazation Goes Here
{
	glzAppinitialization app;
	glzResourcemanager rm;

	GetFocus();
	GetAsyncKeyState(WM_KEYUP);

	// Start Of User Initialization
	angle		= 0.0f;											// Set Starting Angle To Zero

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);						// Black Background
	glClearDepth(1.0f);										// Depth Buffer Setup
	glDepthFunc(GL_LEQUAL);									// The Type Of Depth Testing (Less Or Equal)
	glEnable(GL_DEPTH_TEST);									// Enable Depth Testing
	glShadeModel(GL_SMOOTH);									// Select Smooth Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Set Perspective Calculations To Most Accurate
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glFrontFace(GL_CCW);



	glUseProgram				= (PFNGLUSEPROGRAMPROC) wglGetProcAddress("glUseProgram");
	glGetUniformLocation		= (PFNGLGETUNIFORMLOCATIONPROC) wglGetProcAddress("glGetUniformLocation");
	glUniform1i = (PFNGLUNIFORM1IPROC) wglGetProcAddress("glUniform1i");
	glUniform4f = (PFNGLUNIFORM4FARBPROC) wglGetProcAddress("glUniform4fARB");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) wglGetProcAddress("glUniformMatrix4fv");
	glUniformMatrix4dv = (PFNGLUNIFORMMATRIX4DVPROC)wglGetProcAddress("glUniformMatrix4dv");

	mView.setDisplay(0, 0, width, height);


	glzMatrix mt;
	glzMatrix mt2;
	glzMatrix mt3;
	glzMatrix mo;
	glzMatrix mg;
	glzMatrix mh;

	unsigned int ad[64] = {2, 2, 2, 1, 2, 2, 2, 2,
	                       2, 2, 4, 1, 4, 2, 2, 2,
	                       3, 3, 0, 1, 0, 3, 3, 3,
	                       3, 3, 0, 1, 0, 3, 3, 3,
	                       2, 2, 4, 1, 4, 2, 2, 2,
	                       2, 2, 2, 1, 2, 2, 2, 2,
	                       2, 2, 2, 1, 1, 1, 1, 1,
	                       2, 2, 2, 2, 2, 2, 2, 2
	                      };





	mt.LoadIdentity();
	mt2.LoadIdentity();
	mo.LoadIdentity();
	mg.LoadIdentity();
	mh.LoadIdentity();



	//glzTranslatef(mt,-3.9f,1.9f,0);
	mg.translate(0, 0, 0);

	mt.scale(0.17f, 0.17f, 0.17f);
	mt2.scale(0.3f, 0.3f, 0.3f);
	mt3.scale(0.17f, 0.17f, 0.17f);

	mg.scale(32.0f, 32.0f, 32.0f);
	mh.scale(0.4f, 0.4f, 0.4f);

	mh.rotate(90, 1.0f, 0.0f, 0.0f);

	mh.translate(-8.0, 8.0, 0.0);


	texture_transform obj_tt = glzMakeTTDefault();
	vao_num[8] = glzVAOMakeFromFile("data\\objects\\cv9040c.obj", mo, obj_tt, &vao[8]);

	glzReadImageHead(&img, "data\\heightmap.tga");
	data = new unsigned char[img.imageSize];
	glzLoadImage(&img, "data\\heightmap.tga", data);


	texture_transform height_tt = glzMakeTTVertexCoordAdopt(1.0f, 1.0f, glzAxis::Z, glzOrigin::BOTTOM_LEFT);
	image_geo_transform igt = glzMakeIGT(glzIGTType::DISPLACE_ADD, img.m_width, img.m_height, img.m_bpp, 0, 0, 0, 2.0f, 32.0, glzAxis::Z, data);
	vao_num[9] = glzVAOMakeHeightAtlasGrid(16, 16, mh, height_tt, igt, &vao[9]);

	delete[] data;
	data = NULL;



	if(app.data.legacyMode)
	{
		ProgramObject = glzShaderLoad("data\\glsl-legacy.vert", "data\\glsl-legacy.frag", glzVAOType::AUTO);
	}
	else
	{
		ProgramObject = glzShaderLoad("data\\glsl.vert", "data\\glsl.frag", glzVAOType::AUTO);
	}
	glzShaderLink(ProgramObject);

	// load the textures
	rm.createTexture("font.ms_gothic", "data\\fonts\\ms_gothic.tga", glzTexFilter::LINEAR);
	rm.createTexture("background.back", "data\\back.tga", glzTexFilter::LINEAR);
	rm.createTexture("texture.cv90base", "data\\cv90base.tga", glzTexFilter::ANSIO_MAX, 2);
	rm.createTexture("texture.gridlines", "data\\gridlines.tga", glzTexFilter::ANSIO_MAX, 2);


	//	tankpos.pos = vert3(0, -2, -17);
	tankpos.setScale(vec3(0.5, 0.5, 0.5));



	return TRUE;												// Return TRUE (Initialization Successful)
}


void ExampleVRState::Deinitialize(void)										// Any User DeInitialization Goes Here
{

	// this shouldn't normally be nessecary, but it's better to make it a habit to delete data for when you start to load and unload resources mid game.

	glzKillAllVAO();		// deletes all vaos in existance


}

void ExampleVRState::Update(float seconds)								// Perform Motion Updates Here
{
	glzInput input;

	glzResourcemanager rm;
	rm.load_one();

	if(input.getKeyState(VK_ESCAPE) == TRUE)					// Is ESC Being Pressed?
	{
		mMessageQuit = true;						// Terminate The Program
	}

	if(input.getKeyState(VK_F1) == TRUE)						// Is F1 Being Pressed?
	{
		mMessageFullscreen = true;							// Toggle Fullscreen Mode
	}

	if(gamestate == 1)
	{

		qr.identity();

		qr.rotate(50, 0.0, 1.0, 0.0);
		qr.rotate(40, 1.0, 0.0, 0.0);

		qr *= seconds;

		q *= qr;



	}

	if(gamestate == 2)
	{
		glzQuaternion temQS;
		temQS.identity();

		if(input.getKeyState(VK_UP))
		{
			temQS.rotate(40, 1.0, 0.0, 0.0);
		}
		if(input.getKeyState(VK_DOWN))
		{
			temQS.rotate(-40, 1.0, 0.0, 0.0);
		}

		if(input.getKeyState(VK_LEFT))
		{
			temQS.rotate(40, 0.0, 1.0, 0.0);
		}
		if(input.getKeyState(VK_RIGHT))
		{
			temQS.rotate(-40, 0.0, 1.0, 0.0);
		}

		tankpos.setRotationVelocity(temQS);

	//	tankpos.setPosition(vert3(0, -2 + (sin(angle * PI_OVER_180) * 2), -17));
		tankpos.tick(seconds);
	}


	if(gamestate == 3)
	{

		q3.rotate(seconds * 50, 0.0, 1.0, 0.0);
	}

	if(gamestate == 4)
	{

		q3.rotate(seconds * 50, 0.0, 1.0, 0.0);
		qn.rotate(seconds * 50, 0.0, 1.0, 0.0);
	}
	if(gamestate == 5)
	{

		q.rotate(seconds * 50, 0.0, 1.0, 0.0);
		q.rotate(seconds * 40, 1.0, 0.0, 0.0);
	}



	angle += seconds * 50;
	if(angle > 360)
	{
		angle -= 360;
	}

	if(input.getKeyState('1'))
	{
		gamestate = 1;
	}
	if(input.getKeyState('2'))
	{
		gamestate = 2;
	}
	if(input.getKeyState('3'))
	{
		gamestate = 3;
	}
	if(input.getKeyState('4'))
	{
		gamestate = 4;
	}
	if(input.getKeyState('5'))
	{
		gamestate = 5;
	}



}


void ExampleVRState::DisplayUpdate(int width, int height)
{
	mView.setDisplay(0, 0, width, height);
}


void ExampleVRState::draw_object(GLZ::texturecontainer* tx, int prim, float x, float y)
{
	unsigned int loc1 = glGetUniformLocation(ProgramObject, "projMat");
	// draw objects
	m.LoadIdentity();
	m.perspective(45.0f, 1.618f, 1.0f, 1000.0f);

	m.translate(x, y, -7);
	m.loadQuanternion(q);

	float mtemp[16];
	m.transferMatrix(&mtemp[0]);
	glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);

	glBindTexture(GL_TEXTURE_2D, tx->handle);
	glzDrawVAO(vao_num[prim], vao[prim], GL_TRIANGLES);
}

void ExampleVRState::draw_object2(GLZ::texturecontainer* tx, int prim, float x, float y)
{
	unsigned int loc1 = glGetUniformLocation(ProgramObject, "projMat");
	// draw objects
	m.LoadIdentity();
	m.perspective(45.0f, 1.618f, 1.0f, 1000.0f);

	m.translate(x, y, -7);
	m.loadQuanternion(q);
	float mtemp[16];
	m.transferMatrix(&mtemp[0]);
	glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);
	glPointSize(1);

	glBindTexture(GL_TEXTURE_2D, tx->handle);
	glzDrawVAO(vao_num[prim], vao[prim], GL_POINTS);
}


void ExampleVRState::Draw(void)
{
	glzResourcemanager rm;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	float mtemp[16];
	glEnable(GL_TEXTURE_2D);
	unsigned int loc1 = glGetUniformLocation(ProgramObject, "projMat");
	unsigned int loc2 = glGetUniformLocation(ProgramObject, "texunit0");
	unsigned int loc3 = glGetUniformLocation(ProgramObject, "tint");


	glUseProgram(ProgramObject);
	glUniform1i(loc2, 0);
	glUniform4f(loc3, 1.0f, 1.0f, 1.0f, 1.0f);

	// i have used these gamestates in a few games for LD48 now and they are really quick and dirty, but effective.
	// they allow you to quickly make a title screen and end screen at the end of the project without changing that much code, instead you just encapsulate it in a gamestate



	if(gamestate == 2)
	{

		// draw tank
		m.LoadIdentity();
		m.perspective(45.0f, 1.618f, 1.0f, 100.0f);

		m *= tankpos.getMatrix();
		//m.translate(0,-2,-17);
		//m.scale(0.5,0.5,0.5);

		//m.loadQuanternion(tankpos.r);

		m.transferMatrix(&mtemp[0]);



		m.transferMatrix(&mtemp[0]);
		glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);

		glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("texture.cv90base"));
		glzDrawVAO(vao_num[8], vao[8], GL_TRIANGLES);

		glzDrawText(".obj loading, try using the arrow keys", -0.5f, 0.49f, 0.05f, 1.0f, mView.getAspect(), rm.gettexture("font.ms_gothic"), COL_WHITE);
		glzDrawText("Switch screens with 1, 2, 3, 4, 5", 0.5f, -0.5f, 0.04f, 1.0f, mView.getAspect(), rm.gettexture("font.ms_gothic"), COL_WHITE, glzOrigin::BOTTOM_RIGHT);

	}

	glUseProgram(0);
	glFlush();													// Flush The GL Rendering Pipeline
}
