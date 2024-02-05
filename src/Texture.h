#pragma once
#ifndef Texture_H
#define Texture_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <string>

class Texture
{
public:
	Texture();
	virtual ~Texture();
	void setFilename(const std::string &f) { filename = f; }
	void init();
	void setUnit(GLint u) { unit = u; }
	GLint getUnit() const { return unit; }
	void bind(GLint handle);
	void unbind();
	void setWrapModes(GLint wrapS, GLint wrapT); // Must be called after init()
	
	GLuint getID() { return tid; }int png_to_gl_texture();
private:
	std::string filename;
	int width;
	int height;
	GLuint tid;
	GLint unit;
	//nandini
	/*GLenum format;
	GLenum min_filter;
	GLenum mag_filter;*/
	
};

#endif
