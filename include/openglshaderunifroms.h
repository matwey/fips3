#ifndef _OPENGLSHADERUNIFROMS_H
#define _OPENGLSHADERUNIFROMS_H

#include <QtGlobal>
#include <QtGui/qopengl.h>
#include <QDebug>

#include <array>
#include <cmath>

class OpenGLShaderUniforms {
public:
	typedef std::array<GLfloat, 4> vec4_type;
private:
	vec4_type a_;
	vec4_type c_, z_;
public:
	const quint8 channels, channel_size;
	const double bzero, bscale;
	const double base;

	OpenGLShaderUniforms(quint8 channels, quint8 channel_size, double bzero, double bscale);

	void setMinMax(double minimum, double maximum);
	inline void setMinMax(const std::pair<double, double>& minmax) { setMinMax(minmax.first, minmax.second); }
	inline const vec4_type& get_a() const { return a_; }
	inline const vec4_type& get_c() const { return c_; }
	inline const vec4_type& get_z() const { return z_; }
};


#endif //_OPENGLSHADERUNIFROMS_H
