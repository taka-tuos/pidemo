#include <math.h>
#include <GL/glew.h>
#include <stdlib.h>

#ifdef __MACOSX__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "Gtruetype.h"

Gtruetype::Gtruetype(const char *ttf)
{
	m_pFont = new FTPixmapFont(ttf);
}

Gtruetype::~Gtruetype()
{
	delete m_pFont;
}

void Gtruetype::Draw(float x, float y, char *sz, int m)
{
	/*if (m) {
		glDisable(GL_LIGHTING);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, 1024, 600, 0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
	}*/
	
	glRasterPos2f(x, y + m_ulFontSize);
	m_pFont->Render(sz);

	/*if (m) {
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}*/
}

void Gtruetype::Size(int n)
{
	m_ulFontSize = n;
	m_pFont->FaceSize(m_ulFontSize, m_ulFontSize);
}
