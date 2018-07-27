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

#ifndef _ABSTRACTOPENGLPLAN_H
#define _ABSTRACTOPENGLPLAN_H

#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QString>

#include <abstractopengltexture.h>
#include <fits.h>
#include <openglplane.h>
#include <openglshaderprogram.h>

class AbstractOpenGLPlan:
	public QObject,
	protected QOpenGLFunctions {
private:
	QString name_;
	OpenGLPlane plane_;
	OpenGLShaderProgram program_;
	std::pair<double, double> hdu_minmax_;
	std::pair<double, double> instrumental_minmax_;
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
public:
	AbstractOpenGLPlan(const QString& name, const FITS::AbstractHeaderDataUnit& hdu,
		const std::pair<double, double>& hdu_minmax,
		const std::pair<double, double>& instrumental_minmax,
		quint8 channels, quint8 channel_size, QObject* parent = Q_NULLPTR);
	virtual ~AbstractOpenGLPlan() = 0;

	inline const QString& name() const { return name_; }
	inline const OpenGLPlane& plane() const { return plane_; }
	inline OpenGLShaderProgram& program() { return program_; }

	inline const std::pair<double, double>& hduMinMax() const { return hdu_minmax_; }
	inline const std::pair<double, double>& instrumentalMinMax() const { return instrumental_minmax_; }
	inline const quint8& channels() const { return channels_; }
	inline const quint8& channel_size() const { return channel_size_; };

	virtual QString fragmentShaderSourceCode() = 0;
	virtual QString vertexShaderSourceCode();
	virtual AbstractOpenGLTexture& imageTexture() = 0;

	bool initialize();
	virtual void draw();
};

#endif // _ABSTRACTOPENGLPLAN_H
