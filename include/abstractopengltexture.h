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
	std::pair<double, double> minmax_;
	std::pair<double, double> instrumental_minmax_;
private:
	quint8 channels_;
	quint8 channel_size_;
protected:
	template<class T>
	static inline std::pair<double, double> makeMinMax(const FITS::HeaderDataUnit<FITS::DataUnit<T>>& hdu) {
		using Utils::swap_bytes;

		const auto& dataunit = hdu.data();

		const auto begin = dataunit.data();
		const auto end   = begin + dataunit.length();

		using value_type = decltype(*begin);

		auto e = std::minmax_element(
			begin, end,
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
			[](value_type x, value_type y) { return swap_bytes(x) < swap_bytes(y); }
#else
			[](value_type x, value_type y) { return x < y; }
#endif
		);
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
		return std::make_pair(hdu.FITSToInstrumental(swap_bytes(*(e.first))), hdu.FITSToInstrumental(swap_bytes(*(e.second))));
#else
		return std::make_pair(hdu.FITSToInstrumental(*(e.first)), hdu.FITSToInstrumental(*(e.second)));
#endif
	}

	template<class T>
	static inline std::pair<double, double> makeInstrumentalMinMax(const FITS::HeaderDataUnit<FITS::DataUnit<T>>& hdu) {
		return std::make_pair(hdu.FITSToInstrumental(std::numeric_limits<T>::min()), hdu.FITSToInstrumental(std::numeric_limits<T>::max()));
	}

	virtual void setFormat() = 0;
	virtual void allocateStorage() = 0;
	virtual void setData() = 0;
public:
	AbstractOpenGLTexture(const quint8& channels, const quint8& channel_size);
	virtual ~AbstractOpenGLTexture() = 0;

	virtual void initialize();
	virtual const FITS::AbstractHeaderDataUnit& hdu() const = 0;

	inline const std::pair<double, double>& hduMinMax() const { return minmax_; }
	inline const std::pair<double, double>& instrumentalMinMax() const { return instrumental_minmax_; }
	inline const quint8& channels() const { return channels_; }
	inline const quint8& channel_size() const { return channel_size_; };
};

#endif // _ABSTRACTOPENGLTEXTURE_H
