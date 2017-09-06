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
#include <time.h>


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
Gtruetype *textbig;
Gtruetype *textmini;
Gtexture *symbol;
Gtexture *winnin,*winnin2;
Gtexture *bg;
Gtexture *logo,*logo2,*mie;
Gtexture *throwgun;

uint32_t frame = 0;

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

float wx[3],wy[3],wh[3],ww[3],wvx[3],wvy[3];

void gfxDrawScreen(int mode)
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glColor4f(1, 1, 1, 1.0);
	
	bg->Draw(0,0);
	
	if(mode == 0) {
		logo->Draw(1024-180-32,16);
		
		symbol->Draw(1024/2-symbol->texture_sz[2]/2,0);
	
		throwgun->Draw(0,600-170);
		
		winnin->Draw(wx[0],wy[0]);
		
		wy[0] += wvy[0];
		wx[0] += wvx[0];
		
		if(wx[0] > 1024 - winnin->texture_sz[2] || wx[0] < 0) wvx[0] = -wvx[0];
		if(wy[0] > 600  - winnin->texture_sz[3] || wy[0] < 0) wvy[0] = -wvy[0];
	}
	
	if(mode == 1) {
		winnin->Draw(1024-180-32,16);
		
		logo2->Draw(1024/2-logo2->texture_sz[2]/2,50);
	
		throwgun->Draw(0,600-170);
		
		logo->Draw(wx[1],wy[1]);
		
		wy[1] += wvy[1];
		wx[1] += wvx[1];
		
		if(wx[1] > 1024 - logo->texture_sz[2] || wx[1] < 0) wvx[1] = -wvx[1];
		if(wy[1] > 600  - logo->texture_sz[3] || wy[1] < 0) wvy[1] = -wvy[1];
	}
	
	if(mode == 2) {
		struct tm t;
	
		t.tm_year = 118;     /* 2001年 */
		t.tm_mon  = 6;       /* 5月    */
		t.tm_mday = 26;      /* 20日   */
		t.tm_wday = 4;       /* 日曜日 */
		t.tm_hour = 0;      /* 14時   */
		t.tm_min  = 0;      /* 20分   */
		t.tm_sec  = 0;       /* 00秒   */
		t.tm_isdst= -1;      /* 夏時間無効 */
		
		long days = ((mktime(&t) - time(NULL))+86399)/86400;
		
		logo->Draw(1024-180-32,16);
		
		mie->Draw(1024/2-mie->texture_sz[2]/2,100);
	
		throwgun->Draw(0,600-170);
		
		winnin2->Draw(wx[2],wy[2]);
		
		char s[30];
	
		sprintf(s,"あと%d日",days);
		
		glColor4f(0, 0, 0, 1.0);
		
		for(int i = -1; i < 2; i++)
			for(int j = -1; j < 2; j++)
				if(i && j) textbig->Draw(64+i,64+j,s,1);
		
		glColor4f(1, 0.5, 0, 1.0);
		
		textbig->Draw(64,64,s,1);
		
		wy[2] += wvy[2];
		wx[2] += wvx[2];
		
		wvy[2]++;
		
		if(wx[2] > 1024 - winnin2->texture_sz[2] || wx[2] < 0) wvx[2] = -wvx[2];
		if(wy[2] > 600  - winnin2->texture_sz[3]) wvy[2] = -30;
	}
	
	glColor4f(0, 0, 0, 1.0);
	
	textmini->Draw(4,600-16-4,"(c) interhigh",1);
	
	text->Draw(64,16,"2018 彩る感動 東海総体",1);
	
	frame++;
	
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
	textbig = new Gtruetype("衡山毛筆フォント.ttf");
	textmini = new Gtruetype("circle-mplus-1p-regular.ttf");
	symbol = new Gtexture;
	winnin = new Gtexture;
	winnin2 = new Gtexture;
	throwgun = new Gtexture;
	bg = new Gtexture;
	logo = new Gtexture;
	logo2 = new Gtexture;
	mie = new Gtexture;
	
	symbol->Load("symbol.png");
	symbol->AttachVertexShader(VS30_Rotate);
	
	winnin->Load("winnin-test.png");
	
	winnin2->Load("winnin2.png");
	//w2->AttachVertexShader(VS30_Rotate);
	
	throwgun->Load("throwgun.png");
	
	bg->Load("backdesign.png");
	
	logo->Load("interlogo.png");
	
	logo2->Load("interlogo2.png");
	logo2->AttachVertexShader(VS30_Rotate);
	
	mie->Load("mie.png");
	mie->AttachVertexShader(VS30_Rotate);
	
	text->Size(48);
	textbig->Size(128);
	textmini->Size(16);
	
	Gtexture *list[3];
	
	list[0] = winnin;
	list[1] = logo;
	list[2] = winnin2;
	
	for(int i = 0; i < 3; i++) {
		wx[i] = fmod((float)rand(),1024 - list[i]->texture_sz[2]);
		wy[i] = fmod((float)rand(),600 - list[i]->texture_sz[3]);
		
		wvx[i] = (rand() & 1) ? win_speed : -win_speed;
		wvy[i] = (rand() & 1) ? win_speed : -win_speed;
	}

	while(1) {
		time_t time_now=time(NULL);
		sysProcessEvents();
		gfxDrawScreen((time_now / 20UL) % 3UL);
	}
	
	return 0;
}
