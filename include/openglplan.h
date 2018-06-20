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
	public AbstractOpenGLPlan,
	public detail::OpenGL21BasedPlan {
private:
	Uint8OpenGLTexture image_texture_;
public:
	Uint8OpenGLPlan(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu, QObject* parent = Q_NULLPTR);
	virtual ~Uint8OpenGLPlan() override = default;

	virtual QString fragmentShaderSourceCode() override;
	virtual Uint8OpenGLTexture& imageTexture() override {
		return image_texture_;
	}
};

class Uint8OpenGL33Plan:
	public AbstractOpenGL33Plan,
	public detail::OpenGL33BasedPlan {
private:
	Uint8OpenGL3Texture image_texture_;
public:
	Uint8OpenGL33Plan(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu, QObject* parent = Q_NULLPTR);
	virtual ~Uint8OpenGL33Plan() override = default;

	virtual QString fragmentShaderSourceCode() override;
	virtual Uint8OpenGL3Texture& imageTexture() override {
		return image_texture_;
	}
};

class Int16OpenGLPlan:
	public AbstractOpenGLPlan,
	public detail::OpenGL21BasedPlan {
private:
	Int16OpenGLTexture image_texture_;
public:
	Int16OpenGLPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu, QObject* parent = Q_NULLPTR);
	virtual ~Int16OpenGLPlan() override = default;

	virtual QString fragmentShaderSourceCode() override;
	virtual Int16OpenGLTexture& imageTexture() override {
		return image_texture_;
	}
};

class Int16OpenGL33Plan:
	public AbstractOpenGL33Plan,
	public detail::OpenGL33BasedPlan {
private:
	Int16OpenGL3Texture image_texture_;
public:
	Int16OpenGL33Plan(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu, QObject* parent = Q_NULLPTR);
	virtual ~Int16OpenGL33Plan() override = default;

	virtual QString fragmentShaderSourceCode() override;
	virtual Int16OpenGL3Texture& imageTexture() override {
		return image_texture_;
	}
};

class Int32OpenGLPlan:
	public AbstractOpenGLPlan,
	public detail::OpenGL21BasedPlan {
private:
	Int32OpenGLTexture image_texture_;
public:
	Int32OpenGLPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu, QObject* parent = Q_NULLPTR);
	virtual ~Int32OpenGLPlan() override = default;

	virtual QString fragmentShaderSourceCode() override;
	virtual Int32OpenGLTexture& imageTexture() override {
		return image_texture_;
	}
};

class Int64OpenGLPlan:
	public AbstractOpenGLPlan,
	public detail::OpenGL21BasedPlan {
private:
	Int64OpenGLTexture image_texture_;
public:
	Int64OpenGLPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint64>>& hdu, QObject* parent = Q_NULLPTR);
	virtual ~Int64OpenGLPlan() override = default;

	virtual QString fragmentShaderSourceCode() override;
	virtual Int64OpenGLTexture& imageTexture() override {
		return image_texture_;
	}
};

class FloatOpenGLPlan:
	public AbstractOpenGLPlan,
	public detail::OpenGL21BasedPlan {
private:
	FloatOpenGLTexture image_texture_;

	FloatOpenGLPlan(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu, const std::pair<double, double>& minmax, QObject* parent);
public:
	FloatOpenGLPlan(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu, QObject* parent = Q_NULLPTR);
	virtual ~FloatOpenGLPlan() override = default;

	virtual QString fragmentShaderSourceCode() override;
	virtual FloatOpenGLTexture& imageTexture() override {
		return image_texture_;
	}
};

#endif // _OPENGLPLAN_H
