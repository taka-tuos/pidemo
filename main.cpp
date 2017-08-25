/*
 * SDL OpenGL Tutorial.
 * (c) Michael Vance, 2000
 * briareos@lokigames.com
 *
 * Distributed under terms of the LGPL. 
 */

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "Gtexture.h"
#include "Gtruetype.h"

void sysQuitProgram(int code)
{
	SDL_Quit();

	exit(code);
}

void sysHandleKeyDown(SDL_keysym* keysym)
{
	switch(keysym->sym) {
	case SDLK_ESCAPE:
		sysQuitProgram(0);
		break;
	default:
		break;
	}

}

void sysProcessEvents(void)
{
	SDL_Event event;

	while(SDL_PollEvent(&event)) {

		switch(event.type) {
		case SDL_KEYDOWN:
		
			sysHandleKeyDown(&event.key.keysym);
			break;
		case SDL_QUIT:
		
			sysQuitProgram(0);
			break;
		}

	}

}

//----------------------------------------------------------------------

Gtruetype *text;
Gtruetype *textmini;
Gtexture *symbol;
Gtexture *winnin;
Gtexture *bg;
Gtexture *logo;
Gtexture *throwgun;

uint32_t frame = 0;

const float win_scale = 1.0f;
const float win_speed = 4.0f;

void VSRotate2D(float *xp, float *yp, float x, float y, float xc, float yc, float theta)
{
	*xp = (x - xc) * cos(theta) - (y - yc) * sin(theta) + xc;
	*yp = (x - xc) * sin(theta) + (y - yc) * cos(theta) + yc;
}

void VS30_Rotate(float u, float v, float x, float y, float w, float h, VertexShader_Data *ctx, void *ext)
{
	float rx[4],ry[4];
	
	float angle = (30.0f / 180.0f * 3.14f) * sin((float)frame / 30.0f * 3.14f);
	
	VSRotate2D(&rx[0],&ry[0],x    ,y    ,x + w/2.0f,y + h/2.0f,angle);
	VSRotate2D(&rx[1],&ry[1],x    ,y + h,x + w/2.0f,y + h/2.0f,angle);
	VSRotate2D(&rx[2],&ry[2],x + w,y + h,x + w/2.0f,y + h/2.0f,angle);
	VSRotate2D(&rx[3],&ry[3],x + w,y    ,x + w/2.0f,y + h/2.0f,angle);
	
	glTexCoord2f(0, 0); glVertex2f(rx[0],ry[0]);
	glTexCoord2f(0, v);	glVertex2f(rx[1],ry[1]);
	glTexCoord2f(u, v);	glVertex2f(rx[2],ry[2]);
	glTexCoord2f(u, 0);	glVertex2f(rx[3],ry[3]);
}

void VS30_Scale(float u, float v, float x, float y, float w, float h, VertexShader_Data *ctx, void *ext)
{
	w *= win_scale;
	h *= win_scale;
	glTexCoord2f(0, 0); glVertex2f(x	, y		);
	glTexCoord2f(0, v);	glVertex2f(x	, y + h);
	glTexCoord2f(u, v);	glVertex2f(x + w, y + h);
	glTexCoord2f(u, 0);	glVertex2f(x + w, y		);
}

void VS30_Width(float u, float v, float x, float y, float w, float h, VertexShader_Data *ctx, void *ext)
{
	float scale = w / 1024.0f;
	w /= scale;
	h /= scale;
	glTexCoord2f(0, 0); glVertex2f(x	, y		);
	glTexCoord2f(0, v);	glVertex2f(x	, y + h);
	glTexCoord2f(u, v);	glVertex2f(x + w, y + h);
	glTexCoord2f(u, 0);	glVertex2f(x + w, y		);
}

float wx,wy,wh,ww,wvx,wvy;

void gfxDrawScreen(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glColor4f(1, 1, 1, 1.0);
	
	bg->Draw(0,0);
	
	logo->Draw(1024-180-32,16);
	
	symbol->Draw(1024/2-symbol->texture_sz[2]/2,0);
	
	throwgun->Draw(0,600-170);
	
	winnin->Draw(wx,wy);
	
	glColor4f(0, 0, 0, 1.0);
	
	textmini->Draw(4,600-16-4,"(c) interhigh",1);
	
	text->Draw(64,16,"2018 彩る感動 東海総体",1);
	
	frame++;
	
	wy += wvy;
	wx += wvx;
	
	if(wx > 1024 - ww || wx < 0) wvx = -wvx;
	if(wy > 600  - wh || wy < 0) wvy = -wvy;
	
	SDL_GL_SwapBuffers();
}

void gfxSetView2D(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void gfxInitialize(int width, int height)
{
	gfxSetView2D(width, height);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char* argv[])
{
	const SDL_VideoInfo* info = NULL;
	int width = 0;
	int height = 0;
	int bpp = 0;
	int flags = 0;

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		/* Failed, exit. */
		fprintf(stderr, "Video initialization failed: %s\n",
			 SDL_GetError());
		sysQuitProgram(1);
	}

	info = SDL_GetVideoInfo();

	if(!info) {
		fprintf(stderr, "Video query failed: %s\n",
			 SDL_GetError());
		sysQuitProgram(1);
	}

	width = 1024;
	height = 600;
	bpp = info->vfmt->BitsPerPixel;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	flags = SDL_OPENGL /*| SDL_FULLSCREEN*/;

	if(SDL_SetVideoMode(width, height, bpp, flags) == 0) {
		fprintf(stderr, "Video mode set failed: %s\n",
			 SDL_GetError());
		sysQuitProgram(1);
	}

	gfxInitialize(width, height);
	
	text = new Gtruetype("衡山毛筆フォント.ttf");
	textmini = new Gtruetype("circle-mplus-1p-regular.ttf");
	symbol = new Gtexture;
	winnin = new Gtexture;
	throwgun = new Gtexture;
	bg = new Gtexture;
	logo = new Gtexture;
	
	symbol->Load("symbol.png");
	symbol->AttachVertexShader(VS30_Rotate);
	
	winnin->Load("winnin-test.png");
	
	throwgun->Load("throwgun.png");
	
	bg->Load("backdesign.png");
	
	logo->Load("interlogo.png");
	
	text->Size(48);
	textmini->Size(16);
	
	ww = winnin->texture_sz[2] * win_scale;
	wh = winnin->texture_sz[3] * win_scale;
	wx = fmod((float)rand(),1024 - ww);
	wy = fmod((float)rand(),600 - wh);
	
	wvx = (rand() & 1) ? win_speed : -win_speed;
	wvy = (rand() & 1) ? win_speed : -win_speed;

	while(1) {
		sysProcessEvents();
		gfxDrawScreen();
	}
	
	return 0;
}
