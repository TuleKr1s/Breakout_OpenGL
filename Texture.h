#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

#include <iostream>

class Texture {
public:

	unsigned int ID;

	unsigned int m_width, m_height;

	unsigned int m_internalFormat; // internal texture format
	unsigned int m_imageFormat; // image format

	unsigned int m_wrap_S; // S-axis blend mode
	unsigned int m_wrap_T; // T-axis blend mode
	// filtering mode
	unsigned int m_filterMin; 
	unsigned int m_filterMax; 

	Texture();

	void generate(unsigned int width, unsigned int height, unsigned char* data);

	void bind() const;
};




#endif