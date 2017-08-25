
#include <GL/gl.h>

#ifndef __GTRUETYPE__
#define __GTRUETYPE__

#include <FTGL/ftgl.h>

class Gtruetype {
public:
	FTPixmapFont *m_pFont;
	GLuint m_ulFontSize = 16;

	Gtruetype(const char *ttf);
	~Gtruetype();
	void Draw(float x, float y, char *sz, int m);
	void Size(int n);
};

#endif
