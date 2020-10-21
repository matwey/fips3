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

Uint8OpenGLArrayPlan::Uint8OpenGLArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu):
	AbstractOpenGL2Plan<Uint8OpenGLTextureArray>("uint8-array", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 1, 1) {
}

QString Uint8OpenGLArrayPlan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#ifdef GL_ES
		#ifdef GL_FRAGMENT_PRECISION_HIGH
			precision highp float;
			precision highp sampler2DArray;
		#else
			precision mediump float;
			precision mediump sampler2DArray;
		#endif
	#endif
	#extension GL_EXT_texture_array : require
	varying vec2 UV;
	uniform sampler2DArray image_texture;
	uniform sampler1D colormap;
	uniform float c;
	uniform float z;
	uniform float layer;

	void main() {
		float value = c * (texture2DArray(image_texture, vec3(UV, layer)).a - z);
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

Uint8OpenGL30ArrayPlan::Uint8OpenGL30ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu):
	AbstractOpenGL2Plan<Uint8OpenGL3TextureArray>("uint8-opengl3.0-array", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 1, 1) {
}

QString Uint8OpenGL30ArrayPlan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#ifdef GL_ES
		#ifdef GL_FRAGMENT_PRECISION_HIGH
			precision highp float;
			precision highp sampler2DArray;
		#else
			precision mediump float;
			precision mediump sampler2DArray;
		#endif
	#endif
	#extension GL_EXT_texture_array : require
	varying vec2 UV;
	uniform sampler2DArray image_texture;
	uniform sampler1D colormap;
	uniform float c;
	uniform float z;
	uniform float layer;

	void main() {
		float value = c * (texture2DArray(image_texture, vec3(UV, layer)).r - z);
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

Uint8OpenGL33ArrayPlan::Uint8OpenGL33ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu):
	AbstractOpenGL33Plan<Uint8OpenGL3TextureArray>("uint8-opengl3.3-array", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 1, 1) {
}

QString Uint8OpenGL33ArrayPlan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#version 330
	in vec2 UV;
	out vec4 color;
	uniform sampler2DArray image_texture;
	uniform sampler1D colormap;
	uniform float c;
	uniform float z;
	uniform float layer;

	void main() {
		float value = c * (texture(image_texture, vec3(UV, layer)).r - z);
		color = texture(colormap, clamp(value, 0.0, 1.0));
	}
	)";

	return source;
}

Uint8OpenGLES30Plan::Uint8OpenGLES30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu):
	AbstractOpenGLES30Plan<Uint8OpenGLES3Texture>("uint8-opengles3.0", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 1, 1) {
}

QString Uint8OpenGLES30Plan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#version 300 es
	#ifdef GL_ES
		#ifdef GL_FRAGMENT_PRECISION_HIGH
			precision highp float;
			precision highp sampler2D;
		#else
			precision mediump float;
			precision mediump sampler2D;
		#endif
	#endif
	in vec2 UV;
	out vec4 color;
	uniform sampler2D image_texture;
	uniform sampler2D colormap;
	uniform float c;
	uniform float z;

	void main() {
		float value = c * (texture(image_texture, UV).r - z);
		color = texture(colormap, vec2(clamp(value, 0.0, 1.0), 0.0));
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

Int16OpenGLArrayPlan::Int16OpenGLArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu):
	AbstractOpenGL2Plan<Int16OpenGLTextureArray>("int16-array", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 2, 1) {
}

QString Int16OpenGLArrayPlan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#ifdef GL_ES
		#ifdef GL_FRAGMENT_PRECISION_HIGH
			precision highp float;
			precision highp sampler2DArray;
		#else
			precision mediump float;
			precision mediump sampler2DArray;
		#endif
	#endif
	#extension GL_EXT_texture_array : require
	varying vec2 UV;
	uniform sampler2DArray image_texture;
	uniform sampler1D colormap;
	uniform vec2 c;
	uniform vec2 z;
	uniform float layer;

	void main() {
		vec2 raw_value = texture2DArray(image_texture, vec3(UV, layer)).ga;
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


Int16OpenGL30ArrayPlan::Int16OpenGL30ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu):
	AbstractOpenGL2Plan<Int16OpenGL3TextureArray>("int16-opengl3.0-array", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 1, 2) {
}

QString Int16OpenGL30ArrayPlan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#ifdef GL_ES
		#ifdef GL_FRAGMENT_PRECISION_HIGH
			precision highp float;
			precision highp sampler2DArray;
		#else
			precision mediump float;
			precision mediump sampler2DArray;
		#endif
	#endif
	#extension GL_EXT_texture_array : require
	varying vec2 UV;
	uniform sampler2DArray image_texture;
	uniform sampler1D colormap;
	uniform float c;
	uniform float z;
	uniform float layer;

	void main() {
		float raw_value = texture2DArray(image_texture, vec3(UV, layer)).r;
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

Int16OpenGL33ArrayPlan::Int16OpenGL33ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu):
	AbstractOpenGL33Plan<Int16OpenGL3TextureArray>("int16-opengl3.3-array", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 1, 2) {
}

QString Int16OpenGL33ArrayPlan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#version 330
	in vec2 UV;
	out vec4 color;
	uniform sampler2DArray image_texture;
	uniform sampler1D colormap;
	uniform float c;
	uniform float z;
	uniform float layer;

	void main() {
		float raw_value = texture(image_texture, vec3(UV, layer)).r;
		raw_value -= float(raw_value > 0.5) * 1.0000152590218967; // 65536.0 / 65535.0
		float value = c * (raw_value - z);
		color = texture(colormap, clamp(value, 0.0, 1.0));
	}
	)";

	return source;
}

Int16OpenGLES30Plan::Int16OpenGLES30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu):
	AbstractOpenGLES30Plan<Int16OpenGLES3Texture>("int16-opengles3.0", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 2, 1) {
}

QString Int16OpenGLES30Plan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#version 300 es
	#ifdef GL_ES
		#ifdef GL_FRAGMENT_PRECISION_HIGH
			precision highp float;
			precision highp sampler2D;
		#else
			precision mediump float;
			precision mediump sampler2D;
		#endif
	#endif
	in vec2 UV;
	out vec4 color;
	uniform sampler2D image_texture;
	uniform sampler2D colormap;
	uniform vec2 c;
	uniform vec2 z;

	void main() {
		vec2 raw_value = texture(image_texture, UV).rg;
		raw_value.x -= float(raw_value.x > 0.5) * 1.003921568627451; // 256.0 / 255.0
		float value = dot(c, raw_value - z);
		color = texture(colormap, vec2(clamp(value, 0.0, 1.0), 0.0));
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

Int32OpenGLArrayPlan::Int32OpenGLArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu):
	AbstractOpenGL2Plan<Int32OpenGLTextureArray>("int32-array", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 4, 1) {
}

QString Int32OpenGLArrayPlan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#ifdef GL_ES
		#ifdef GL_FRAGMENT_PRECISION_HIGH
			precision highp float;
			precision highp sampler2DArray;
		#else
			precision mediump float;
			precision mediump sampler2DArray;
		#endif
	#endif
	#extension GL_EXT_texture_array : require
	varying vec2 UV;
	uniform sampler2DArray image_texture;
	uniform sampler1D colormap;
	uniform vec4 c;
	uniform vec4 z;
	uniform float layer;

	void main() {
		vec4 raw_value = texture2DArray(image_texture, vec3(UV, layer));
		raw_value.x -= float(raw_value.x > 0.5) * 1.003921568627451; // 256.0 / 255.0
		float value = dot(c, raw_value - z);
		gl_FragColor = texture1D(colormap, clamp(value, 0.0, 1.0));
	}
	)";

	return source;
}

Int32OpenGL30Plan::Int32OpenGL30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu):
	AbstractOpenGL2Plan<Int32OpenGL3Texture>("int32-opengl3.0", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 4, 1) {
}

QString Int32OpenGL30Plan::fragmentShaderSourceCode() const {
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

Int32OpenGL30ArrayPlan::Int32OpenGL30ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu):
	AbstractOpenGL2Plan<Int32OpenGL3TextureArray>("int32-opengl3.0-array", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 4, 1) {
}

QString Int32OpenGL30ArrayPlan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#ifdef GL_ES
		#ifdef GL_FRAGMENT_PRECISION_HIGH
			precision highp float;
			precision highp sampler2DArray;
		#else
			precision mediump float;
			precision mediump sampler2DArray;
		#endif
	#endif
	#extension GL_EXT_texture_array : require
	varying vec2 UV;
	uniform sampler2DArray image_texture;
	uniform sampler1D colormap;
	uniform vec4 c;
	uniform vec4 z;
	uniform float layer;

	void main() {
		vec4 raw_value = texture2DArray(image_texture, vec3(UV, layer));
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

Int32OpenGL33ArrayPlan::Int32OpenGL33ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu):
	AbstractOpenGL33Plan<Int32OpenGL3TextureArray>("int32-opengl3.3-array", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 2, 2) {
}

QString Int32OpenGL33ArrayPlan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#version 330
	in vec2 UV;
	out vec4 color;
	uniform sampler2DArray image_texture;
	uniform sampler1D colormap;
	uniform vec2 c;
	uniform vec2 z;
	uniform float layer;

	void main() {
		vec2 raw_value = texture(image_texture, vec3(UV, layer)).rg;
		raw_value.x -= float(raw_value.x > 0.5) * 1.0000152590218967; // 65536.0 / 65535.0
		float value = dot(c, raw_value - z);
		color = texture(colormap, clamp(value, 0.0, 1.0));
	}
	)";

	return source;
}

Int32OpenGLES30Plan::Int32OpenGLES30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu):
	AbstractOpenGLES30Plan<Int32OpenGLES3Texture>("int32-opengles3.0", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 4, 1) {
}

QString Int32OpenGLES30Plan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#version 300 es
	#ifdef GL_ES
		#ifdef GL_FRAGMENT_PRECISION_HIGH
			precision highp float;
			precision highp sampler2D;
		#else
			precision mediump float;
			precision mediump sampler2D;
		#endif
	#endif
	in vec2 UV;
	out vec4 color;
	uniform sampler2D image_texture;
	uniform sampler2D colormap;
	uniform vec4 c;
	uniform vec4 z;

	void main() {
		vec4 raw_value = texture(image_texture, UV);
		raw_value.x -= float(raw_value.x > 0.5) * 1.003921568627451; // 256.0 / 255.0
		float value = dot(c, raw_value - z);
		color = texture(colormap, vec2(clamp(value, 0.0, 1.0), 0.0));
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

Int64OpenGLArrayPlan::Int64OpenGLArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint64>>& hdu):
	AbstractOpenGL2Plan<Int64OpenGLTextureArray>("int64-array", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 4, 2) {
}

QString Int64OpenGLArrayPlan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#ifdef GL_ES
		#ifdef GL_FRAGMENT_PRECISION_HIGH
			precision highp float;
			precision highp sampler2DArray;
		#else
			precision mediump float;
			precision mediump sampler2DArray;
		#endif
	#endif
	#extension GL_EXT_texture_array : require
	varying vec2 UV;
	uniform sampler2DArray image_texture;
	uniform sampler1D colormap;
	uniform vec4 c;
	uniform vec4 z;
	uniform float layer;

	void main() {
		vec4 raw_value = texture2DArray(image_texture, vec3(UV, layer));
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

Int64OpenGL33ArrayPlan::Int64OpenGL33ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint64>>& hdu):
	AbstractOpenGL33Plan<Int64OpenGLTextureArray>("int64-opengl3.3-array", hdu, makeMinMax(hdu), makeInstrumentalMinMax(hdu), 4, 2) {
}

QString Int64OpenGL33ArrayPlan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#version 330
	in vec2 UV;
	out vec4 color;
	uniform sampler2DArray image_texture;
	uniform sampler1D colormap;
	uniform vec4 c;
	uniform vec4 z;
	uniform float layer;

	void main() {
		vec4 raw_value = texture(image_texture, vec3(UV, layer));
		raw_value.x -= float(raw_value.x > 0.5) * 1.0000152590218967; // 65536.0 / 65535.0
		float value = dot(c, raw_value - z);
		color = texture(colormap, clamp(value, 0.0, 1.0));
	}
	)";

	return source;
}

FloatOpenGL30Plan::FloatOpenGL30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu, const std::pair<double, double>& minmax):
	AbstractOpenGL2Plan<FloatOpenGL3Texture>("float32-opengl3.0", hdu, minmax, minmax, 1, 0) {
}

FloatOpenGL30Plan::FloatOpenGL30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu):
	FloatOpenGL30Plan(hdu, makeMinMax(hdu)) {
}

QString FloatOpenGL30Plan::fragmentShaderSourceCode() const {
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

FloatOpenGL30ArrayPlan::FloatOpenGL30ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu, const std::pair<double, double>& minmax):
	AbstractOpenGL2Plan<FloatOpenGL3TextureArray>("float32-opengl3.0-array", hdu, minmax, minmax, 1, 0) {
}

FloatOpenGL30ArrayPlan::FloatOpenGL30ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu):
	FloatOpenGL30ArrayPlan(hdu, makeMinMax(hdu)) {
}

QString FloatOpenGL30ArrayPlan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#ifdef GL_ES
		#ifdef GL_FRAGMENT_PRECISION_HIGH
			precision highp float;
			precision highp sampler2DArray;
		#else
			precision mediump float;
			precision mediump sampler2DArray;
		#endif
	#endif
	#extension GL_EXT_texture_array : require
	varying vec2 UV;
	uniform sampler2DArray image_texture;
	uniform sampler1D colormap;
	uniform float c;
	uniform float z;
	uniform float layer;

	void main() {
		float value = c * (texture2DArray(image_texture, vec3(UV, layer)).r - z);
		gl_FragColor = texture1D(colormap, clamp(value, 0.0, 1.0));
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

FloatOpenGL33ArrayPlan::FloatOpenGL33ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu, const std::pair<double, double>& minmax):
	AbstractOpenGL33Plan<FloatOpenGL3TextureArray>("float32-opengl3.3-array", hdu, minmax, minmax, 1, 0) {
}

FloatOpenGL33ArrayPlan::FloatOpenGL33ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu):
	FloatOpenGL33ArrayPlan(hdu, makeMinMax(hdu)) {
}

QString FloatOpenGL33ArrayPlan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#version 330
	in vec2 UV;
	out vec4 color;
	uniform sampler2DArray image_texture;
	uniform sampler1D colormap;
	uniform float c;
	uniform float z;
	uniform float layer;

	void main() {
		float value = c * (texture(image_texture, vec3(UV, layer)).r - z);
		color = texture(colormap, clamp(value, 0.0, 1.0));
	}
	)";

	return source;
}

FloatOpenGLES30Plan::FloatOpenGLES30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu, const std::pair<double, double>& minmax):
	AbstractOpenGLES30Plan<FloatOpenGLES3Texture>("float32-opengles3.0", hdu, minmax, minmax, 1, 0) {
}

FloatOpenGLES30Plan::FloatOpenGLES30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu):
	FloatOpenGLES30Plan(hdu, makeMinMax(hdu)) {
}

QString FloatOpenGLES30Plan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#version 300 es
	#ifdef GL_ES
		#ifdef GL_FRAGMENT_PRECISION_HIGH
			precision highp float;
			precision highp usampler2D;
			precision highp sampler2D;
		#else
			precision mediump float;
			precision mediump usampler2D;
			precision mediump sampler2D;
		#endif
	#endif
	in vec2 UV;
	out vec4 color;
	uniform usampler2D image_texture;
	uniform sampler2D colormap;
	uniform float c;
	uniform float z;

	void main() {
		uvec4 raw_vec = texture(image_texture, UV);
		float raw_value = uintBitsToFloat(raw_vec.w + uint(256) * raw_vec.z + uint(65536) * raw_vec.y + uint(16777216) * raw_vec.x);
		float value = dot(c, raw_value - z);
		color = texture(colormap, vec2(clamp(value, 0.0, 1.0), 0.0));
	}
	)";

	return source;
}

DoubleOpenGL30Plan::DoubleOpenGL30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu, const std::pair<double, double>& minmax):
	AbstractOpenGL2Plan<DoubleOpenGL3Texture>("float64-opengl3.0", hdu, minmax, minmax, 1, 0) {
}

DoubleOpenGL30Plan::DoubleOpenGL30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu):
	DoubleOpenGL30Plan(hdu, makeMinMax(hdu)) {
}

QString DoubleOpenGL30Plan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#version 130
	#extension GL_ARB_gpu_shader_fp64 : require
	varying vec2 UV;
	uniform usampler2D image_texture;
	uniform sampler1D colormap;
	uniform float c;
	uniform float z;

	void main() {
		double raw_value = packDouble2x32(texture(image_texture, UV).gr);
		double value = c * (raw_value - z);
		gl_FragColor = texture(colormap, float(clamp(value, 0.0, 1.0)));
	}
	)";

	return source;
}

DoubleOpenGL30ArrayPlan::DoubleOpenGL30ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu, const std::pair<double, double>& minmax):
	AbstractOpenGL2Plan<DoubleOpenGL3TextureArray>("float64-opengl3.0-array", hdu, minmax, minmax, 1, 0) {
}

DoubleOpenGL30ArrayPlan::DoubleOpenGL30ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu):
	DoubleOpenGL30ArrayPlan(hdu, makeMinMax(hdu)) {
}

QString DoubleOpenGL30ArrayPlan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#version 130
	#extension GL_ARB_gpu_shader_fp64 : require
	varying vec2 UV;
	uniform usampler2DArray image_texture;
	uniform sampler1D colormap;
	uniform float c;
	uniform float z;
	uniform float layer;

	void main() {
		double raw_value = packDouble2x32(texture(image_texture, vec3(UV, layer)).gr);
		double value = c * (raw_value - z);
		gl_FragColor = texture(colormap, float(clamp(value, 0.0, 1.0)));
	}
	)"  ;

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

DoubleOpenGL33ArrayPlan::DoubleOpenGL33ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu, const std::pair<double, double>& minmax):
	AbstractOpenGL33Plan<DoubleOpenGL3TextureArray>("float64-opengl3.3-array", hdu, minmax, minmax, 1, 0) {
}

DoubleOpenGL33ArrayPlan::DoubleOpenGL33ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu):
	DoubleOpenGL33ArrayPlan(hdu, makeMinMax(hdu)) {
}

QString DoubleOpenGL33ArrayPlan::fragmentShaderSourceCode() const {
	static const QString source = R"(
	#version 330
	#extension GL_ARB_gpu_shader_fp64 : require
	in vec2 UV;
	out vec4 color;
	uniform usampler2DArray image_texture;
	uniform sampler1D colormap;
	uniform float c;
	uniform float z;
	uniform float layer;

	void main() {
		double raw_value = packDouble2x32(texture(image_texture, vec3(UV, layer)).gr);
		double value = c * (raw_value - z);
		color = texture(colormap, float(clamp(value, 0.0, 1.0)));
	}
	)";

	return source;
}
