#include "Texture.h"

Texture::Texture() : m_width(0), m_height(0), m_internalFormat(GL_RGB), m_imageFormat(GL_RGB),\
m_wrap_S(GL_REPEAT), m_wrap_T(GL_REPEAT), m_filterMin(GL_LINEAR), m_filterMax(GL_LINEAR) 
{
	glGenTextures(1, &this->ID);

}

void Texture::generate(unsigned int width, unsigned int height, unsigned char* data) {
	m_width = width;
	m_height = height;

	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_imageFormat, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filterMax);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind() const {
	glBindTexture(GL_TEXTURE_2D, this->ID);
}