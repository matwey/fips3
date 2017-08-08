#ifndef _OPENGLSHADERUNIFROMS_H
#define _OPENGLSHADERUNIFROMS_H

#include <QtGlobal>
#include <QDebug>

#include <cmath>
#include <OpenGL/OpenGL.h>

class OpenGLShaderUniforms {
private:
	GLfloat a_[4];
	GLfloat c_[4], z_[4];
public:
	const quint8 channels, channel_size;
	const double bzero, bscale;
	const double base;

	OpenGLShaderUniforms(quint8 channels, quint8 channel_size, double bzero, double bscale);

	void setMinMax(double minimum, double maximum);
	inline void setMinMax(std::pair<double, double> minmax) { setMinMax(minmax.first, minmax.second); }
	inline const GLfloat* get_a() const { return a_; }
	inline const GLfloat* get_c() const { return c_; }
	inline const GLfloat* get_z() const { return z_; }
};


#endif //_OPENGLSHADERUNIFROMS_H
