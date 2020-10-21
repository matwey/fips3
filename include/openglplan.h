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

struct OpenGL21BasedArrayPlan {
	constexpr static bool satisfied(const OpenGLFeatures& features) {
		return OpenGLRequiredFeatures{}.requireOpenGL21().requireEXT_texture_array().satisfied(features);
	}
};

struct OpenGL30BasedPlan {
	constexpr static bool satisfied(const OpenGLFeatures& features) {
		return OpenGLRequiredFeatures{}.requireOpenGL30().satisfied(features);
	}
};

struct OpenGL33BasedPlan {
	constexpr static bool satisfied(const OpenGLFeatures& features) {
		return OpenGLRequiredFeatures{}.requireOpenGL33().satisfied(features);
	}
};

struct OpenGL33fp64BasedPlan {
	constexpr static bool satisfied(const ::OpenGLFeatures& features) {
		return OpenGLRequiredFeatures{}.requireOpenGL33().requireARB_gpu_shader_fp64().satisfied(features);
	}
};

struct OpenGLES30BasedPlan {
	constexpr static bool satisfied(const OpenGLFeatures& features) {
		return OpenGLRequiredFeatures{}.requireOpenGLES30().satisfied(features);
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

class Uint8OpenGLArrayPlan:
	public AbstractOpenGL2Plan<Uint8OpenGLTextureArray>,
	public detail::OpenGL21BasedArrayPlan {
public:
	explicit Uint8OpenGLArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu);
	virtual ~Uint8OpenGLArrayPlan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class Uint8OpenGL30Plan:
	public AbstractOpenGL2Plan<Uint8OpenGL3Texture>,
	public detail::OpenGL30BasedPlan {
public:
	explicit Uint8OpenGL30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu);
	virtual ~Uint8OpenGL30Plan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class Uint8OpenGL30ArrayPlan:
	public AbstractOpenGL2Plan<Uint8OpenGL3TextureArray>,
	public detail::OpenGL30BasedPlan {
public:
	explicit Uint8OpenGL30ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu);
	virtual ~Uint8OpenGL30ArrayPlan() override = default;

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

class Uint8OpenGL33ArrayPlan:
	public AbstractOpenGL33Plan<Uint8OpenGL3TextureArray>,
	public detail::OpenGL33BasedPlan {
public:
	explicit Uint8OpenGL33ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu);
	virtual ~Uint8OpenGL33ArrayPlan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class Uint8OpenGLES30Plan:
	public AbstractOpenGLES30Plan<Uint8OpenGLES3Texture>,
	public detail::OpenGLES30BasedPlan {
public:
	explicit Uint8OpenGLES30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu);
	virtual ~Uint8OpenGLES30Plan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class Uint8OpenGLES30ArrayPlan:
	public AbstractOpenGLES30Plan<Uint8OpenGLES3TextureArray>,
	public detail::OpenGLES30BasedPlan {
public:
	explicit Uint8OpenGLES30ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu);
	virtual ~Uint8OpenGLES30ArrayPlan() override = default;

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

class Int16OpenGLArrayPlan:
	public AbstractOpenGL2Plan<Int16OpenGLTextureArray>,
	public detail::OpenGL21BasedArrayPlan {
public:
	explicit Int16OpenGLArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu);
	virtual ~Int16OpenGLArrayPlan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class Int16OpenGL30Plan:
	public AbstractOpenGL2Plan<Int16OpenGL3Texture>,
	public detail::OpenGL30BasedPlan {
public:
	explicit Int16OpenGL30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu);
	virtual ~Int16OpenGL30Plan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class Int16OpenGL30ArrayPlan:
	public AbstractOpenGL2Plan<Int16OpenGL3TextureArray>,
	public detail::OpenGL30BasedPlan {
public:
	explicit Int16OpenGL30ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu);
	virtual ~Int16OpenGL30ArrayPlan() override = default;

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

class Int16OpenGL33ArrayPlan:
	public AbstractOpenGL33Plan<Int16OpenGL3TextureArray>,
	public detail::OpenGL33BasedPlan {
public:
	explicit Int16OpenGL33ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu);
	virtual ~Int16OpenGL33ArrayPlan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class Int16OpenGLES30Plan:
	public AbstractOpenGLES30Plan<Int16OpenGLES3Texture>,
	public detail::OpenGLES30BasedPlan {
public:
	explicit Int16OpenGLES30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu);
	virtual ~Int16OpenGLES30Plan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class Int16OpenGLES30ArrayPlan:
	public AbstractOpenGLES30Plan<Int16OpenGLES3TextureArray>,
	public detail::OpenGLES30BasedPlan {
public:
	explicit Int16OpenGLES30ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu);
	virtual ~Int16OpenGLES30ArrayPlan() override = default;

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

class Int32OpenGLArrayPlan:
	public AbstractOpenGL2Plan<Int32OpenGLTextureArray>,
	public detail::OpenGL21BasedArrayPlan {
public:
	explicit Int32OpenGLArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu);
	virtual ~Int32OpenGLArrayPlan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class Int32OpenGL30Plan:
	public AbstractOpenGL2Plan<Int32OpenGL3Texture>,
	public detail::OpenGL30BasedPlan {
public:
	explicit Int32OpenGL30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu);
	virtual ~Int32OpenGL30Plan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class Int32OpenGL30ArrayPlan:
	public AbstractOpenGL2Plan<Int32OpenGL3TextureArray>,
	public detail::OpenGL30BasedPlan {
public:
	explicit Int32OpenGL30ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu);
	virtual ~Int32OpenGL30ArrayPlan() override = default;

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

class Int32OpenGL33ArrayPlan:
	public AbstractOpenGL33Plan<Int32OpenGL3TextureArray>,
	public detail::OpenGL33BasedPlan {
public:
	explicit Int32OpenGL33ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu);
	virtual ~Int32OpenGL33ArrayPlan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class Int32OpenGLES30Plan:
	public AbstractOpenGLES30Plan<Int32OpenGLES3Texture>,
	public detail::OpenGLES30BasedPlan {
public:
	explicit Int32OpenGLES30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu);
	virtual ~Int32OpenGLES30Plan() override = default;

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

class Int64OpenGLArrayPlan:
	public AbstractOpenGL2Plan<Int64OpenGLTextureArray>,
	public detail::OpenGL21BasedArrayPlan {
public:
	explicit Int64OpenGLArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint64>>& hdu);
	virtual ~Int64OpenGLArrayPlan() override = default;

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

class Int64OpenGL33ArrayPlan:
	public AbstractOpenGL33Plan<Int64OpenGLTextureArray>,
	public detail::OpenGL33BasedPlan {
public:
	explicit Int64OpenGL33ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<qint64>>& hdu);
	virtual ~Int64OpenGL33ArrayPlan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class FloatOpenGL30Plan:
	public AbstractOpenGL2Plan<FloatOpenGL3Texture>,
	public detail::OpenGL30BasedPlan {
private:
	FloatOpenGL30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu, const std::pair<double, double>& minmax);
public:
	explicit FloatOpenGL30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu);
	virtual ~FloatOpenGL30Plan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class FloatOpenGL30ArrayPlan:
	public AbstractOpenGL2Plan<FloatOpenGL3TextureArray>,
	public detail::OpenGL30BasedPlan {
private:
	FloatOpenGL30ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu, const std::pair<double, double>& minmax);
public:
	explicit FloatOpenGL30ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu);
	virtual ~FloatOpenGL30ArrayPlan() override = default;

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

class FloatOpenGL33ArrayPlan:
	public AbstractOpenGL33Plan<FloatOpenGL3TextureArray>,
	public detail::OpenGL33BasedPlan {
private:
	FloatOpenGL33ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu, const std::pair<double, double>& minmax);
public:
	explicit FloatOpenGL33ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu);
	virtual ~FloatOpenGL33ArrayPlan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class FloatOpenGLES30Plan:
	public AbstractOpenGLES30Plan<FloatOpenGLES3Texture>,
	public detail::OpenGLES30BasedPlan {
private:
	FloatOpenGLES30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu, const std::pair<double, double>& minmax);
public:
	explicit FloatOpenGLES30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu);
	virtual ~FloatOpenGLES30Plan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class DoubleOpenGL30Plan:
	public AbstractOpenGL2Plan<DoubleOpenGL3Texture> {
private:
	DoubleOpenGL30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu, const std::pair<double, double>& minmax);
public:
	explicit DoubleOpenGL30Plan(const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu);
	virtual ~DoubleOpenGL30Plan() override = default;

	constexpr static bool satisfied(const ::OpenGLFeatures& features) {
		return OpenGLRequiredFeatures{}.requireOpenGL30().requireARB_gpu_shader_fp64().satisfied(features);
	}

	virtual QString fragmentShaderSourceCode() const override;
};

class DoubleOpenGL30ArrayPlan:
	public AbstractOpenGL2Plan<DoubleOpenGL3TextureArray>,
	public detail::OpenGL30BasedPlan {
private:
	DoubleOpenGL30ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu, const std::pair<double, double>& minmax);
public:
	explicit DoubleOpenGL30ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu);
	virtual ~DoubleOpenGL30ArrayPlan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class DoubleOpenGL33Plan:
	public AbstractOpenGL33Plan<DoubleOpenGL3Texture>,
	public detail::OpenGL33fp64BasedPlan {
private:
	DoubleOpenGL33Plan(const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu, const std::pair<double, double>& minmax);
public:
	explicit DoubleOpenGL33Plan(const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu);
	virtual ~DoubleOpenGL33Plan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

class DoubleOpenGL33ArrayPlan:
	public AbstractOpenGL33Plan<DoubleOpenGL3TextureArray>,
	public detail::OpenGL33fp64BasedPlan {
private:
	DoubleOpenGL33ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu, const std::pair<double, double>& minmax);
public:
	explicit DoubleOpenGL33ArrayPlan(const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu);
	virtual ~DoubleOpenGL33ArrayPlan() override = default;

	virtual QString fragmentShaderSourceCode() const override;
};

#endif // _OPENGLPLAN_H
