/*
 *  Copyright (C) 2017  Matwey V. Kornilov <matwey.kornilov@gmail.com>
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

#include <openglerrors.h>

OpenGLException::OpenGLException(const QString &what, GLenum gl_error_code):
	Exception(what + ": " + glErrorString(gl_error_code)) {}
void OpenGLException::raise() const {
	throw *this;
}
QException* OpenGLException::clone() const {
	return new OpenGLException(*this);
}
// From OpenGL ES 2.0 documentation:
// https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glGetError.xml
QString OpenGLException::glErrorString(GLenum gl_error_code) {
	switch (gl_error_code) {
//		case GL_NO_ERROR:
//			return "No error has been recorded. The value of this symbolic constant is guaranteed to be 0.";
		case GL_INVALID_ENUM:
			return "An unacceptable value is specified for an enumerated argument. "
					"The offending command is ignored and has no other side effect than to set the error flag.";
		case GL_INVALID_VALUE:
			return "A numeric argument is out of range. "
					"The offending command is ignored and has no other side effect than to set the error flag.";
		case GL_INVALID_OPERATION:
			return "The specified operation is not allowed in the current state. "
					"The offending command is ignored and has no other side effect than to set the error flag.";
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			return "The command is trying to render to or read from the framebuffer while the currently bound framebuffer is not framebuffer complete (i.e. the return value from glCheckFramebufferStatus is not GL_FRAMEBUFFER_COMPLETE). "
					"The offending command is ignored and has no other side effect than to set the error flag.";
		case GL_OUT_OF_MEMORY:
			return "There is not enough memory left to execute the command. "
					"The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
		default:
			return "Unknown error";
	}
}
