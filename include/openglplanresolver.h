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

#ifndef _OPENGLPLANRESOLVER_H
#define _OPENGLPLANRESOLVER_H

#include <abstractopenglplan.h>
#include <openglfeatures.h>

template<class T> struct resolveOpenGLPlanTraits {
	static bool satisfied(const OpenGLFeatures& features) {
		return T::satisfied(features);
	}

	template<class... Args>
	static T* create(Args&&... args) {
		return new T{std::forward<Args>(args)...};
	}
};

namespace detail {
	template<class T, class... Args> struct doResolveOpenGLPlan {
		template<class... U>
		static AbstractOpenGLPlan* resolve(const OpenGLFeatures& features, U&&... args) {
			auto r = doResolveOpenGLPlan<T>::resolve(features, std::forward<U>(args)...);

			return (r ? r : doResolveOpenGLPlan<Args...>::resolve(features, std::forward<U>(args)...));
		}
	};
	template<class T> struct doResolveOpenGLPlan<T> {
		template<class... U>
		static AbstractOpenGLPlan* resolve(const OpenGLFeatures& features, U&&... args) {
			typedef resolveOpenGLPlanTraits<T> traits;
			auto s = traits::satisfied(features);

			return (s ? traits::create(std::forward<U>(args)...): Q_NULLPTR);
		}
	};

} // detail

template<class... Args, class... U> AbstractOpenGLPlan* resolveOpenGLPlan(const OpenGLFeatures& features, U&&... args) {
	return detail::doResolveOpenGLPlan<Args...>::resolve(features, std::forward<U>(args)...);
}

#endif // _OPENGLPLANRESOLVER_H
