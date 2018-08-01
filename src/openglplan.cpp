/*
 *  Copyright (C) 2018  Matwey V. Kornilov <matwey.kornilov@gmail.com>
 *                      Konstantin Malanchev <hombit@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or (at
 *  your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <openglplan.h>

Uint8OpenGLPlan::Uint8OpenGLPlan(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu):
	AbstractOpenGL2Plan<Uint8OpenGLTexture>("uint8", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 1, 1) {
}

QString Uint8OpenGLPlan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#ifdef GL_ES
		#ifdef GL_FRAGMENT_PRECISION_HIGH
			precision highp float;
			precision highp sampler2D;
		#else
			precision mediump float;
			precision mediump sampler2D;
		#endif
	#endif
	varying vec2 UV;
	uniform sampler2D image_texture;
	uniform sampler1D colormap;
	uniform float c;
	uniform float z;

	void main() {
		float value = c * (texture2D(image_texture, UV).a - z);
		gl_FragColor = texture1D(colormap, clamp(value, 0.0, 1.0));
	}
	)";

	return source;
}

Uint8OpenGL30Plan::Uint8OpenGL30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu):
	AbstractOpenGL2Plan<Uint8OpenGL3Texture>("uint8-opengl3.0", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 1, 1) {
}

QString Uint8OpenGL30Plan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#ifdef GL_ES
		#ifdef GL_FRAGMENT_PRECISION_HIGH
			precision highp float;
			precision highp sampler2D;
		#else
			precision mediump float;
			precision mediump sampler2D;
		#endif
	#endif
	varying vec2 UV;
	uniform sampler2D image_texture;
	uniform sampler1D colormap;
	uniform float c;
	uniform float z;

	void main() {
		float value = c * (texture2D(image_texture, UV).r - z);
		gl_FragColor = texture1D(colormap, clamp(value, 0.0, 1.0));
	}
	)";

	return source;
}

Uint8OpenGL33Plan::Uint8OpenGL33Plan(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu):
	AbstractOpenGL33Plan<Uint8OpenGL3Texture>("uint8-opengl3.3", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 1, 1) {
}

QString Uint8OpenGL33Plan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#version 330
	in vec2 UV;
	out vec4 color;
	uniform sampler2D image_texture;
	uniform sampler1D colormap;
	uniform float c;
	uniform float z;

	void main() {
		float value = c * (texture(image_texture, UV).r - z);
		color = texture(colormap, clamp(value, 0.0, 1.0));
	}
	)";

	return source;
}

Int16OpenGLPlan::Int16OpenGLPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu):
	AbstractOpenGL2Plan<Int16OpenGLTexture>("int16", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 2, 1) {
}

QString Int16OpenGLPlan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#ifdef GL_ES
		#ifdef GL_FRAGMENT_PRECISION_HIGH
			precision highp float;
			precision highp sampler2D;
		#else
			precision mediump float;
			precision mediump sampler2D;
		#endif
	#endif
	varying vec2 UV;
	uniform sampler2D image_texture;
	uniform sampler1D colormap;
	uniform vec2 c;
	uniform vec2 z;

	void main() {
		vec2 raw_value = texture2D(image_texture, UV).ga;
		raw_value.x -= float(raw_value.x > 0.5) * 1.003921568627451; // 256.0 / 255.0
		float value = dot(c, raw_value - z);
		gl_FragColor = texture1D(colormap, clamp(value, 0.0, 1.0));
	}
	)";

	return source;
}

Int16OpenGL30Plan::Int16OpenGL30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu):
	AbstractOpenGL2Plan<Int16OpenGL3Texture>("int16-opengl3.0", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 1, 2) {
}

QString Int16OpenGL30Plan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#ifdef GL_ES
		#ifdef GL_FRAGMENT_PRECISION_HIGH
			precision highp float;
			precision highp sampler2D;
		#else
			precision mediump float;
			precision mediump sampler2D;
		#endif
	#endif
	varying vec2 UV;
	uniform sampler2D image_texture;
	uniform sampler1D colormap;
	uniform float c;
	uniform float z;

	void main() {
		float raw_value = texture2D(image_texture, UV).r;
		raw_value -= float(raw_value > 0.5) * 1.0000152590218967; // 65536.0 / 65535.0
		float value = c * (raw_value - z);
		gl_FragColor = texture1D(colormap, clamp(value, 0.0, 1.0));
	}
	)";

	return source;
}

Int16OpenGL33Plan::Int16OpenGL33Plan(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu):
	AbstractOpenGL33Plan<Int16OpenGL3Texture>("int16-opengl3.3", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 1, 2) {
}

QString Int16OpenGL33Plan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#version 330
	in vec2 UV;
	out vec4 color;
	uniform sampler2D image_texture;
	uniform sampler1D colormap;
	uniform float c;
	uniform float z;

	void main() {
		float raw_value = texture(image_texture, UV).r;
		raw_value -= float(raw_value > 0.5) * 1.0000152590218967; // 65536.0 / 65535.0
		float value = c * (raw_value - z);
		color = texture(colormap, clamp(value, 0.0, 1.0));
	}
	)";

	return source;
}

Int32OpenGLPlan::Int32OpenGLPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu):
	AbstractOpenGL2Plan<Int32OpenGLTexture>("int32", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 4, 1) {
}

QString Int32OpenGLPlan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#ifdef GL_ES
		#ifdef GL_FRAGMENT_PRECISION_HIGH
			precision highp float;
			precision highp sampler2D;
		#else
			precision mediump float;
			precision mediump sampler2D;
		#endif
	#endif
	varying vec2 UV;
	uniform sampler2D image_texture;
	uniform sampler1D colormap;
	uniform vec4 c;
	uniform vec4 z;

	void main() {
		vec4 raw_value = texture2D(image_texture, UV);
		raw_value.x -= float(raw_value.x > 0.5) * 1.003921568627451; // 256.0 / 255.0
		float value = dot(c, raw_value - z);
		gl_FragColor = texture1D(colormap, clamp(value, 0.0, 1.0));
	}
	)";

	return source;
}

Int32OpenGL33Plan::Int32OpenGL33Plan(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu):
	AbstractOpenGL33Plan<Int32OpenGL3Texture>("int32-opengl3.3", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 2, 2) {
}

QString Int32OpenGL33Plan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#version 330
	in vec2 UV;
	out vec4 color;
	uniform sampler2D image_texture;
	uniform sampler1D colormap;
	uniform vec2 c;
	uniform vec2 z;

	void main() {
		vec2 raw_value = texture(image_texture, UV).rg;
		raw_value.x -= float(raw_value.x > 0.5) * 1.0000152590218967; // 65536.0 / 65535.0
		float value = dot(c, raw_value - z);
		color = texture(colormap, clamp(value, 0.0, 1.0));
	}
	)";

	return source;
}

Int64OpenGLPlan::Int64OpenGLPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint64>>& hdu):
	AbstractOpenGL2Plan<Int64OpenGLTexture>("int64", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 4, 2) {
}

QString Int64OpenGLPlan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#ifdef GL_ES
		#ifdef GL_FRAGMENT_PRECISION_HIGH
			precision highp float;
			precision highp sampler2D;
		#else
			precision mediump float;
			precision mediump sampler2D;
		#endif
	#endif
	varying vec2 UV;
	uniform sampler2D image_texture;
	uniform sampler1D colormap;
	uniform vec4 c;
	uniform vec4 z;

	void main() {
		vec4 raw_value = texture2D(image_texture, UV);
		raw_value.x -= float(raw_value.x > 0.5) * 1.0000152590218967; // 65536.0 / 65535.0
		float value = dot(c, raw_value - z);
		gl_FragColor = texture1D(colormap, clamp(value, 0.0, 1.0));
	}
	)";

	return source;
}

Int64OpenGL33Plan::Int64OpenGL33Plan(const FITS::HeaderDataUnit<FITS::DataUnit<qint64>>& hdu):
	AbstractOpenGL33Plan<Int64OpenGLTexture>("int64-opengl3.3", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 4, 2) {
}

QString Int64OpenGL33Plan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#version 330
	in vec2 UV;
	out vec4 color;
	uniform sampler2D image_texture;
	uniform sampler1D colormap;
	uniform vec4 c;
	uniform vec4 z;

	void main() {
		vec4 raw_value = texture(image_texture, UV);
		raw_value.x -= float(raw_value.x > 0.5) * 1.0000152590218967; // 65536.0 / 65535.0
		float value = dot(c, raw_value - z);
		color = texture(colormap, clamp(value, 0.0, 1.0));
	}
	)";

	return source;
}

FloatOpenGL33Plan::FloatOpenGL33Plan(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu, const std::pair<double, double>& minmax):
	AbstractOpenGL33Plan<FloatOpenGL3Texture>("float32-opengl3.3", hdu, minmax, minmax, 1, 0) {
}

FloatOpenGL33Plan::FloatOpenGL33Plan(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu):
	FloatOpenGL33Plan(hdu, makeMinMax(hdu)) {
}

QString FloatOpenGL33Plan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#version 330
	in vec2 UV;
	out vec4 color;
	uniform sampler2D image_texture;
	uniform sampler1D colormap;
	uniform float c;
	uniform float z;
	void main() {
		float value = c * (texture(image_texture, UV).r - z);
		color = texture(colormap, clamp(value, 0.0, 1.0));
	}
	)";

	return source;
}

DoubleOpenGL33Plan::DoubleOpenGL33Plan(const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu, const std::pair<double, double>& minmax):
	AbstractOpenGL33Plan<DoubleOpenGL3Texture>("float64-opengl3.3", hdu, minmax, minmax, 1, 0) {
}

DoubleOpenGL33Plan::DoubleOpenGL33Plan(const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu):
	DoubleOpenGL33Plan(hdu, makeMinMax(hdu)) {
}

QString DoubleOpenGL33Plan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#version 330
	#extension GL_ARB_gpu_shader_fp64 : require
	in vec2 UV;
	out vec4 color;
	uniform usampler2D image_texture;
	uniform sampler1D colormap;
	uniform float c;
	uniform float z;
	void main() {
		double raw_value = packDouble2x32(texture(image_texture, UV).gr);
		double value = c * (raw_value - z);
		color = texture(colormap, float(clamp(value, 0.0, 1.0)));
	}
	)";

	return source;
}
