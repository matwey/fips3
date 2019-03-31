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

#include <algorithm>
#include <limits>
#include <utility>

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QString>

#include <abstractopengltexture.h>
#include <fits.h>
#include <openglplane.h>
#include <openglshaderprogram.h>
#include <utils/minmax.h>
#include <utils/swapbytes.h>

class AbstractOpenGLPlan:
	protected QOpenGLFunctions {
private:
	class AbstractDrawer {
	private:
		QString name_;
	public:
		explicit AbstractDrawer(const QString& name);
		virtual ~AbstractDrawer() = 0;

		virtual bool initialize(AbstractOpenGLPlan& plan) const = 0;
		virtual void draw(AbstractOpenGLPlan& plan) const = 0;

		inline const QString& name() const { return name_; }
	};

	class SimpleDrawer:
		public AbstractDrawer {
	public:
		SimpleDrawer();
		~SimpleDrawer() override = default;

		bool initialize(AbstractOpenGLPlan& plan) const override;
		void draw(AbstractOpenGLPlan& plan) const override;
	};

	class VAODrawer:
		public AbstractDrawer {
	public:
		VAODrawer();
		~VAODrawer() override = default;

		bool initialize(AbstractOpenGLPlan& plan) const override;
		void draw(AbstractOpenGLPlan& plan) const override;
	};
private:
	QString name_;
	OpenGLPlane plane_;
	QOpenGLVertexArrayObject vao_;
	OpenGLShaderProgram program_;
	std::pair<double, double> hdu_minmax_;
	std::pair<double, double> instrumental_minmax_;
	quint8 channels_;
	quint8 channel_size_;
	const AbstractDrawer* drawer_;
private:
	static const SimpleDrawer simple_drawer;
	static const VAODrawer vao_drawer;
protected:
	template<class T>
	static inline std::pair<double, double> makeMinMax(const FITS::HeaderDataUnit<FITS::DataUnit<T>>& hdu) {
		const auto& dataunit = hdu.data();

		const auto begin = dataunit.data();
		const auto end   = begin + dataunit.length();

		auto e = std::accumulate(
			begin, end,
			std::make_pair(std::numeric_limits<T>::max(), std::numeric_limits<T>::lowest()),
			[](const std::pair<T, T>& acc, const T& element) {
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
				const T& x = Utils::swap_bytes(element);
#else
				const T& x = element;
#endif
				return std::make_pair(Utils::min(acc.first, x), Utils::max(acc.second, x));
			}
		);
		// Swap max and lowest if data contains only NaNs
		if (e.first > e.second) {
			std::swap(e.first, e.second);
		}

		return std::make_pair(hdu.FITSToInstrumental(e.first), hdu.FITSToInstrumental(e.second));
	}

	template<class T>
	static inline std::pair<double, double> makeInstrumentalMinMax(const FITS::HeaderDataUnit<FITS::DataUnit<T>>& hdu) {
		return std::make_pair(hdu.FITSToInstrumental(std::numeric_limits<T>::min()), hdu.FITSToInstrumental(std::numeric_limits<T>::max()));
	}
public:
	AbstractOpenGLPlan(const QString& name, const FITS::AbstractHeaderDataUnit& hdu,
		const std::pair<double, double>& hdu_minmax,
		const std::pair<double, double>& instrumental_minmax,
		quint8 channels, quint8 channel_size);
	virtual ~AbstractOpenGLPlan() = 0;

	inline const QString& name() const { return name_; }
	inline const QString& drawerName() const { return drawer_->name(); }
	inline OpenGLPlane& plane() { return plane_; }
	inline OpenGLShaderProgram& program() { return program_; }

	inline const std::pair<double, double>& hduMinMax() const { return hdu_minmax_; }
	inline const std::pair<double, double>& instrumentalMinMax() const { return instrumental_minmax_; }
	inline const quint8& channels() const { return channels_; }
	inline const quint8& channel_size() const { return channel_size_; };

	virtual QString fragmentShaderSourceCode() const = 0;
	virtual QString vertexShaderSourceCode() const = 0;
	virtual AbstractOpenGLTexture& imageTexture() = 0;
	virtual const AbstractOpenGLTexture& imageTexture() const = 0;
	inline const FITS::AbstractHeaderDataUnit& hdu() const {
		return imageTexture().hdu();
	};

	bool initialize();
	virtual void draw() {
		Q_ASSERT(drawer_ != Q_NULLPTR);

		drawer_->draw(*this);
	}
};

template<class T>
class AbstractOpenGLPlanTexture:
	public AbstractOpenGLPlan {
public:
	typedef T texture_type;
private:
	texture_type image_texture_;
public:
	template<class U>
	AbstractOpenGLPlanTexture(const QString& name, const FITS::HeaderDataUnit<FITS::DataUnit<U>>& hdu,
		const std::pair<double, double>& hdu_minmax,
		const std::pair<double, double>& instrumental_minmax,
		quint8 channels, quint8 channel_size):
		AbstractOpenGLPlan(name, hdu, hdu_minmax, instrumental_minmax, channels, channel_size),
		image_texture_(hdu) {
	}
	virtual ~AbstractOpenGLPlanTexture() override = 0;

	virtual texture_type& imageTexture() override {
		return image_texture_;
	}
	virtual const texture_type& imageTexture() const override {
		return image_texture_;
	}
};

template<class T>
AbstractOpenGLPlanTexture<T>::~AbstractOpenGLPlanTexture() = default;

template<class T>
class AbstractOpenGL2Plan:
	public AbstractOpenGLPlanTexture<T> {
public:
	template<class U>
	AbstractOpenGL2Plan(const QString& name, const FITS::HeaderDataUnit<FITS::DataUnit<U>>& hdu,
		const std::pair<double, double>& hdu_minmax,
		const std::pair<double, double>& instrumental_minmax,
		quint8 channels, quint8 channel_size):
		AbstractOpenGLPlanTexture<T>(name, hdu, hdu_minmax, instrumental_minmax, channels, channel_size) {
	}
	virtual ~AbstractOpenGL2Plan() override = 0;

	virtual QString vertexShaderSourceCode() const override;
};

template<class T>
QString AbstractOpenGL2Plan<T>::vertexShaderSourceCode() const {
	static const QString source = R"(
		attribute vec2 vertexCoord;
		attribute vec2 vertexUV;
		varying vec2 UV;
		uniform mat4 MVP;

		void main() {
			gl_Position = MVP * vec4(vertexCoord,0,1);
			UV = vertexUV;
		}
	)";

	return source;
}

template<class T>
AbstractOpenGL2Plan<T>::~AbstractOpenGL2Plan() = default;

template<class T>
class AbstractOpenGL33Plan:
	public AbstractOpenGLPlanTexture<T> {
public:
	template<class U>
	AbstractOpenGL33Plan(const QString& name, const FITS::HeaderDataUnit<FITS::DataUnit<U>>& hdu,
		const std::pair<double, double>& hdu_minmax,
		const std::pair<double, double>& instrumental_minmax,
		quint8 channels, quint8 channel_size):
		AbstractOpenGLPlanTexture<T>(name, hdu, hdu_minmax, instrumental_minmax, channels, channel_size) {
	}
	virtual ~AbstractOpenGL33Plan() override = 0;

	virtual QString vertexShaderSourceCode() const override;
};

template<class T>
QString AbstractOpenGL33Plan<T>::vertexShaderSourceCode() const {
	static const QString source = R"(
		#version 330

		in vec2 vertexCoord;
		in vec2 vertexUV;
		out vec2 UV;
		uniform mat4 MVP;

		void main() {
			gl_Position = MVP * vec4(vertexCoord,0,1);
			UV = vertexUV;
		}
	)";

	return source;
}

template<class T>
AbstractOpenGL33Plan<T>::~AbstractOpenGL33Plan() = default;

#endif // _ABSTRACTOPENGLPLAN_H
