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

#ifndef _OPENGLFEATURES_H
#define _OPENGLFEATURES_H

#include <QtGlobal>
#include <QOpenGLContext>

class OpenGLFeatures {
protected:
	quint64 bitmask_;

	constexpr static const quint64 mask_hasOpenGL21 = Q_UINT64_C(1) << 1;
	constexpr static const quint64 mask_hasOpenGL30 = Q_UINT64_C(1) << 2;
	constexpr static const quint64 mask_hasOpenGL33 = Q_UINT64_C(1) << 3;
	constexpr static const quint64 mask_hasOpenGLES20 = Q_UINT64_C(1) << 8;
	constexpr static const quint64 mask_hasOpenGLES30 = Q_UINT64_C(1) << 9;
	constexpr static const quint64 mask_hasARB_gpu_shader_fp64 = Q_UINT64_C(1) << 16;
	constexpr static const quint64 mask_hasEXT_texture_array = Q_UINT64_C(1) << 17;
private:
	bool checkHasOpenGLHelper(const QOpenGLContext& opengl_context, const int major, const int minor, const quint64 mask, const bool es = false);

	bool checkHasOpenGL33(const QOpenGLContext& opengl_context);
	bool checkHasOpenGL30(const QOpenGLContext& opengl_context);
	bool checkHasOpenGL21(const QOpenGLContext& opengl_context);
	bool checkHasOpenGLES30(const QOpenGLContext& opengl_context);
	bool checkHasOpenGLES20(const QOpenGLContext& opengl_context);

	bool checkHasExtension(const QOpenGLContext& opengl_context, const char* extension_name, const quint64 mask);
	bool checkHasARB_gpu_shader_fp64(const QOpenGLContext& opengl_context);
	bool checkHasEXT_texture_array(const QOpenGLContext& opengl_context);
protected:
	constexpr inline OpenGLFeatures(const quint64 bitmask = 0): bitmask_(bitmask) {}
public:
	explicit OpenGLFeatures(const QOpenGLContext& opengl_context);

	OpenGLFeatures(const OpenGLFeatures&) = default;
	OpenGLFeatures(OpenGLFeatures&&) = default;
	OpenGLFeatures& operator=(const OpenGLFeatures&) = default;
	OpenGLFeatures& operator=(OpenGLFeatures&&) = default;

	constexpr inline bool operator==(const OpenGLFeatures& other) const {
		return bitmask_ == other.bitmask_;
	}

	constexpr inline OpenGLFeatures operator& (const OpenGLFeatures& other) const {
		return OpenGLFeatures{bitmask_ & other.bitmask_};
	}
};

class OpenGLRequiredFeatures:
	public OpenGLFeatures {
private:
	constexpr inline OpenGLRequiredFeatures(const quint64 bitmask): OpenGLFeatures(bitmask) {}
public:
	constexpr inline OpenGLRequiredFeatures(): OpenGLFeatures() {}

	OpenGLRequiredFeatures(const OpenGLRequiredFeatures&) = default;
	OpenGLRequiredFeatures(OpenGLRequiredFeatures&&) = default;
	OpenGLRequiredFeatures& operator=(const OpenGLRequiredFeatures&) = default;
	OpenGLRequiredFeatures& operator=(OpenGLRequiredFeatures&&) = default;

	constexpr inline bool satisfied(const OpenGLFeatures& features) const {
		return (features & *this) == *this;
	}

	constexpr inline OpenGLRequiredFeatures requireOpenGL21() const {
		return OpenGLRequiredFeatures{bitmask_ | mask_hasOpenGL21};
	}
	constexpr inline OpenGLRequiredFeatures requireOpenGL30() const {
		return OpenGLRequiredFeatures{bitmask_ | mask_hasOpenGL30};
	}
	constexpr inline OpenGLRequiredFeatures requireOpenGL33() const {
		return OpenGLRequiredFeatures{bitmask_ | mask_hasOpenGL33};
	}
	constexpr inline OpenGLRequiredFeatures requireARB_gpu_shader_fp64() const {
		return OpenGLRequiredFeatures{bitmask_ | mask_hasARB_gpu_shader_fp64};
	}
	constexpr inline OpenGLRequiredFeatures requireEXT_texture_array() const {
		return OpenGLRequiredFeatures{bitmask_ | mask_hasEXT_texture_array};
	}
};

#endif // _OPENGLFEATURES_H
