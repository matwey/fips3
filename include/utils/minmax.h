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

#ifndef _MINMAX_H_
#define _MINMAX_H_

#include <algorithm> // std::min, std::max
#include <cmath> // std::fmin, std::fmax
#include <type_traits> // std::enable_if, std::is_integral, std::is_floating_point

namespace Utils {
template<class T, typename std::enable_if<std::is_integral<T>::value, bool>::type = 0>
const T& min(const T& x, const T& y) {
	return std::min(x, y);
}

template<class T, typename std::enable_if<std::is_floating_point<T>::value, bool>::type = 0>
T min(const T& x, const T& y) {
	return std::fmin(x, y);
}

template<class T, typename std::enable_if<std::is_integral<T>::value, bool>::type = 0>
const T& max(const T& x, const T& y) {
	return std::max(x, y);
}

template<class T, typename std::enable_if<std::is_floating_point<T>::value, bool>::type = 0>
T max(const T& x, const T& y) {
	return std::fmax(x, y);
}
} // namespace Utils

#endif //_MINMAX_H_
