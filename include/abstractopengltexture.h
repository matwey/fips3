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

#ifndef _ABSTRACTOPENGLTEXTURE_H
#define _ABSTRACTOPENGLTEXTURE_H

#include <QOpenGLTexture>

#include <fits.h>
#include <openglerrors.h>
#include <utils/swapbytes.h>
#include <utility>

class AbstractOpenGLTexture: public QOpenGLTexture {
public:
	class TextureCreateError: public OpenGLException {
	public:
		TextureCreateError(GLenum gl_error_code);

		virtual void raise() const override;
		virtual QException* clone() const override;
	};
protected:
	virtual void setFormat() = 0;
	virtual void allocateStorage() = 0;
	virtual void setData() = 0;
public:
	AbstractOpenGLTexture();
	virtual ~AbstractOpenGLTexture() = 0;

	virtual void initialize();
	virtual const FITS::AbstractHeaderDataUnit& hdu() const = 0;
};

#endif // _ABSTRACTOPENGLTEXTURE_H
