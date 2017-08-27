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

#ifndef _OPENGLTEXTURE_H
#define _OPENGLTEXTURE_H

#include <QOpenGLTexture>
#include <QOpenGLWidget>

#include <algorithm>

#include <fits.h>
#include <openglerrors.h>

class OpenGLTexture: public QOpenGLTexture {
public:
	class TextureCreateError: public OpenGLException {
	public:
		TextureCreateError(GLenum gl_error_code);

		virtual void raise() const override;
		virtual QException* clone() const override;
	};

private:
	const FITS::HeaderDataUnit* hdu_;
	quint8 channels_;  // Number of color channels
	quint8 channel_size_;  // Bytes per channel for integral texture, 0 for float one
	std::pair<double, double> minmax_;
	std::pair<double, double> instrumental_minmax_;
	QOpenGLTexture::TextureFormat texture_format_;
	QOpenGLTexture::PixelFormat pixel_format_;
	QOpenGLTexture::PixelType pixel_type_;
	bool swap_bytes_enabled_;

public:
	explicit OpenGLTexture(const FITS::HeaderDataUnit* hdu);

	void initialize();
	inline const std::pair<double, double>& hdu_minmax() const { return minmax_; }
	inline const std::pair<double, double>& instrumental_minmax() const { return instrumental_minmax_; }
	inline quint8 channels() const { return channels_; }
	inline quint8 channel_size() const { return channel_size_; };
};

#endif //_OPENGLTEXTURE_H
