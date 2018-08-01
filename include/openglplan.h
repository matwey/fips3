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

#ifndef _OPENGLPLAN_H
#define _OPENGLPLAN_H

#include <abstractopenglplan.h>
#include <openglfeatures.h>
#include <opengltexture.h>

namespace detail {

struct OpenGL21BasedPlan {
	constexpr static bool satisfied(const OpenGLFeatures& features) {
		return OpenGLRequiredFeatures{}.requireOpenGL21().satisfied(features);
	}
};

struct OpenGL33BasedPlan {
	constexpr static bool satisfied(const OpenGLFeatures& features) {
		return OpenGLRequiredFeatures{}.requireOpenGL33().satisfied(features);
	}
};

} // detail

class Uint8OpenGLPlan:
	public AbstractOpenGL2Plan<Uint8OpenGLTexture>,
	public detail::OpenGL21BasedPlan {
public:
	explicit Uint8OpenGLPlan(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu);
	virtual ~Uint8OpenGLPlan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class Uint8OpenGL33Plan:
	public AbstractOpenGL33Plan<Uint8OpenGL3Texture>,
	public detail::OpenGL33BasedPlan {
public:
	explicit Uint8OpenGL33Plan(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu);
	virtual ~Uint8OpenGL33Plan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class Int16OpenGLPlan:
	public AbstractOpenGL2Plan<Int16OpenGLTexture>,
	public detail::OpenGL21BasedPlan {
public:
	explicit Int16OpenGLPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu);
	virtual ~Int16OpenGLPlan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class Int16OpenGL33Plan:
	public AbstractOpenGL33Plan<Int16OpenGL3Texture>,
	public detail::OpenGL33BasedPlan {
public:
	explicit Int16OpenGL33Plan(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu);
	virtual ~Int16OpenGL33Plan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class Int32OpenGLPlan:
	public AbstractOpenGL2Plan<Int32OpenGLTexture>,
	public detail::OpenGL21BasedPlan {
public:
	explicit Int32OpenGLPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu);
	virtual ~Int32OpenGLPlan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class Int32OpenGL33Plan:
	public AbstractOpenGL33Plan<Int32OpenGL3Texture>,
	public detail::OpenGL33BasedPlan {
public:
	explicit Int32OpenGL33Plan(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu);
	virtual ~Int32OpenGL33Plan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class Int64OpenGLPlan:
	public AbstractOpenGL2Plan<Int64OpenGLTexture>,
	public detail::OpenGL21BasedPlan {
public:
	explicit Int64OpenGLPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint64>>& hdu);
	virtual ~Int64OpenGLPlan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class Int64OpenGL33Plan:
	public AbstractOpenGL33Plan<Int64OpenGLTexture>,
	public detail::OpenGL33BasedPlan {
public:
	explicit Int64OpenGL33Plan(const FITS::HeaderDataUnit<FITS::DataUnit<qint64>>& hdu);
	virtual ~Int64OpenGL33Plan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class FloatOpenGL33Plan:
	public AbstractOpenGL33Plan<FloatOpenGL3Texture>,
	public detail::OpenGL33BasedPlan {
private:
	FloatOpenGL33Plan(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu, const std::pair<double, double>& minmax);
public:
	explicit FloatOpenGL33Plan(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu);
	virtual ~FloatOpenGL33Plan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class DoubleOpenGL33Plan:
	public AbstractOpenGL33Plan<DoubleOpenGL3Texture> {
private:
	DoubleOpenGL33Plan(const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu, const std::pair<double, double>& minmax);
public:
	explicit DoubleOpenGL33Plan(const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu);
	virtual ~DoubleOpenGL33Plan() override = default;

	constexpr static bool satisfied(const ::OpenGLFeatures& features) {
		return OpenGLRequiredFeatures{}.requireOpenGL33().requireARB_gpu_shader_fp64().satisfied(features);
	}

	virtual QString fragmentShaderSourceCode() const override;
};

#endif // _OPENGLPLAN_H
