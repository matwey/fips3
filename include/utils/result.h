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

#ifndef _RESULT_H
#define _RESULT_H

#include <utility>

namespace Utils {

template<class R, class F> class Result;

template<class R, class F> class Result {
public:
	using result_type = R;
	using function_type = F;
private:
	result_type result_;
	function_type fun_;
public:
	Result(function_type fun): fun_(std::move(fun)) {}

	template<class... Args>
	inline void operator() (Args&&... args) {
		result_ = fun_(std::forward<Args>(args)...);
	}

	inline result_type&& result() { return std::move(result_); }
};

template<class F> class Result<void, F> {
public:
	using result_type = void;
	using function_type = F;
private:
	function_type fun_;
public:
	Result(function_type fun): fun_(std::move(fun)) {}

	template<class... Args>
	inline void operator() (Args&&... args) {
		fun_(std::forward<Args>(args)...);
	}

	inline void result() {}
};

} // Utils

#endif // _RESULT_H
