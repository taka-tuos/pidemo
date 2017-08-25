#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#ifndef __GTEXTURE__
#define __GTEXTURE__

typedef struct __VSS {
	GLuint texture_id;
	GLfloat texture_sz[4];
	GLubyte *pix;
	GLuint width, height;
	void(*VertexShader)(float, float, float, float, float, float, struct __VSS *, void *);
} VertexShader_Data;

void Gtexture_DefaultVS(float u, float v, float x, float y, float w, float h, VertexShader_Data *ctx, void *ext);

class Gtexture {
public:
	GLuint texture_id;
	GLfloat texture_sz[4];
	GLubyte *pix;
	GLuint width, height;
	void *ext;

	void (*VertexShader)(float , float , float , float , float , float , VertexShader_Data *, void *);
	
	Gtexture()
	{
		glGenTextures(1, &texture_id);
		VertexShader = Gtexture_DefaultVS;
	}

	~Gtexture()
	{
		glDeleteTextures(1, &texture_id);
	}

	void Draw(GLfloat x, GLfloat y);
	void Load(const char *sz);
	void Load(GLuint w, GLuint h, GLuint *bit);
	void AttachExtObject(void *extdata)
	{
		ext = extdata;
	}
	void RestoreVertexShader(void)
	{
		VertexShader = Gtexture_DefaultVS;
	}
	void AttachVertexShader(void(*NewVertexShader)(float, float, float, float, float ,float , VertexShader_Data *, void *))
	{
		VertexShader = NewVertexShader;
	}
};

#endif