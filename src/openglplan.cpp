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

Uint8OpenGLPlan::Uint8OpenGLPlan(QObject* parent):
	AbstractOpenGLPlan("uint8", parent) {
}

QString Uint8OpenGLPlan::fragmentShaderSourceCode() {
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
	uniform sampler2D texture;
	uniform sampler1D colormap;
	uniform float c;
	uniform float z;

	void main() {
		float value = c * (texture2D(texture, UV).a - z);
		gl_FragColor = texture1D(colormap, clamp(value, 0.0, 1.0));
	}
	)";

	return source;
}

Int16OpenGLPlan::Int16OpenGLPlan(QObject* parent):
	AbstractOpenGLPlan("int16", parent) {
}

QString Int16OpenGLPlan::fragmentShaderSourceCode() {
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
	uniform sampler2D texture;
	uniform sampler1D colormap;
	uniform vec2 c;
	uniform vec2 z;

	void main() {
		vec2 raw_value = texture2D(texture, UV).ga;
		raw_value.x -= float(raw_value.x > 0.5) * 1.003921568627451; // 256.0 / 255.0
		float value = dot(c, raw_value - z);
		gl_FragColor = texture1D(colormap, clamp(value, 0.0, 1.0));
	}
	)";

	return source;
}

Int32OpenGLPlan::Int32OpenGLPlan(QObject* parent):
	AbstractOpenGLPlan("int32", parent) {
}

QString Int32OpenGLPlan::fragmentShaderSourceCode() {
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
	uniform sampler2D texture;
	uniform sampler1D colormap;
	uniform vec4 c;
	uniform vec4 z;

	void main() {
		vec4 raw_value = texture2D(texture, UV);
		raw_value.x -= float(raw_value.x > 0.5) * 1.003921568627451; // 256.0 / 255.0
		float value = dot(c, raw_value - z);
		gl_FragColor = texture1D(colormap, clamp(value, 0.0, 1.0));
	}
	)";

	return source;
}

Int64OpenGLPlan::Int64OpenGLPlan(QObject* parent):
	AbstractOpenGLPlan("int64", parent) {
}

QString Int64OpenGLPlan::fragmentShaderSourceCode() {
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
	uniform sampler2D texture;
	uniform sampler1D colormap;
	uniform vec4 c;
	uniform vec4 z;

	void main() {
		vec4 raw_value = texture2D(texture, UV);
		raw_value.x -= float(raw_value.x > 0.5) * 1.0000152590218967; // 65536.0 / 65535.0
		float value = dot(c, raw_value - z);
		gl_FragColor = texture1D(colormap, clamp(value, 0.0, 1.0));
	}
	)";

	return source;
}

FloatOpenGLPlan::FloatOpenGLPlan(QObject* parent):
	AbstractOpenGLPlan("float32", parent) {
}

QString FloatOpenGLPlan::fragmentShaderSourceCode() {
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
	uniform sampler2D texture;
	uniform sampler1D colormap;
	uniform float c;
	uniform float z;
	void main() {
		float value = c * (texture2D(texture, UV).a - z);
		gl_FragColor = texture1D(colormap, clamp(value, 0.0, 1.0));
	}
	)";

	return source;
}
