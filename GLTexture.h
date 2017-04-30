//-------------------------------------------------------------------
//	CGLTexture - OpenGL Texture Class
//-------------------------------------------------------------------

#ifndef __GLTexture_h_
#define __GLTexture_h_

#include "glut/glut.h"

class CGLTexture
{
public:
	CGLTexture();
	~CGLTexture();

	// Create and load the files.
	bool LoadTGA(char* fileName);
	void Use();

protected:
	int checkSize (int x);
	unsigned char* getRGBA (FILE *s, int size);
	unsigned char* getRGB (FILE *s, int size);
	unsigned char* getGray (FILE *s, int size);
	unsigned char* getData (FILE *s, int sz, int iBits);
	bool loadTGATexture(char *name, int id);

	unsigned int m_textureID;
	int m_nWidth, m_nHeight;
	GLenum texFormat;	
};

#endif // __GLTexture_h_