#include "Gtexture.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_SIMD
#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h>

void Gtexture_DefaultVS(float u, float v, float x, float y, float w, float h, VertexShader_Data *ctx, void *ext)
{
	glTexCoord2f(0, 0); glVertex2f(x	, y		);
	glTexCoord2f(0, v);	glVertex2f(x	, y + h);
	glTexCoord2f(u, v);	glVertex2f(x + w, y + h);
	glTexCoord2f(u, 0);	glVertex2f(x + w, y		);
}

GLuint __pow2_list[] = {
	1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536
};

GLuint __return_pow2(int n)
{
	int i;
	for (i = 1; i < 16; i++) {
		if (__pow2_list[i] >= n) return __pow2_list[i];
	}
	return 0;
}

#define GL_CLAMP_TO_EDGE 0x812F

void Gtexture::Load(const char *sz)
{
	int x, y, w, h, bpp;
	GLubyte *bit = stbi_load(sz, &w, &h, &bpp, 4);

	int iw = __return_pow2(w);
	int ih = __return_pow2(h);

	texture_sz[0] = (float)w / (float)iw;
	texture_sz[1] = (float)h / (float)ih;
	texture_sz[2] = (float)w;
	texture_sz[3] = (float)h;

	pix = (GLubyte *)malloc(iw * ih * 4);

	memset(pix, 0, iw * ih * 4);

	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			pix[(y * iw + x) * 4 + 0] = bit[(y * w + x) * 4 + 0];
			pix[(y * iw + x) * 4 + 1] = bit[(y * w + x) * 4 + 1];
			pix[(y * iw + x) * 4 + 2] = bit[(y * w + x) * 4 + 2];
			pix[(y * iw + x) * 4 + 3] = bit[(y * w + x) * 4 + 3];
		}
	}

	width = iw;
	height = ih;

	stbi_image_free(bit);

	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, iw, ih, 0, GL_RGBA, GL_UNSIGNED_BYTE, pix);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Gtexture::Load(GLuint w, GLuint h, GLuint *bit)
{
	int x, y;

	int iw = __return_pow2(w);
	int ih = __return_pow2(h);

	texture_sz[0] = (float)w / (float)iw;
	texture_sz[1] = (float)h / (float)ih;
	texture_sz[2] = (float)w;
	texture_sz[3] = (float)h;

	pix = (GLubyte *)malloc(iw * ih * 4);

	memset(pix, 0, iw * ih * 4);

	width = iw;
	height = ih;

	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			pix[(y * iw + x) * 4 + 0] = ((GLubyte *)bit)[(y * w + x) * 4 + 0];
			pix[(y * iw + x) * 4 + 1] = ((GLubyte *)bit)[(y * w + x) * 4 + 1];
			pix[(y * iw + x) * 4 + 2] = ((GLubyte *)bit)[(y * w + x) * 4 + 2];
			pix[(y * iw + x) * 4 + 3] = ((GLubyte *)bit)[(y * w + x) * 4 + 3];
		}
	}

	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, iw, ih, 0, GL_RGBA, GL_UNSIGNED_BYTE, pix);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Gtexture::Draw(GLfloat x, GLfloat y)
{
	VertexShader_Data ctx;
	ctx.height = height;
	ctx.width = width;
	ctx.pix = pix;
	ctx.texture_id = texture_id;
	ctx.VertexShader = VertexShader;
	memcpy(ctx.texture_sz, texture_sz, sizeof texture_sz);

	/*glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0, 1024, 600, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);*/

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glBegin(GL_QUADS);
	VertexShader(texture_sz[0], texture_sz[1], x, y, texture_sz[2], texture_sz[3], &ctx, ext);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	/*glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);*/

	VertexShader = ctx.VertexShader;
}
