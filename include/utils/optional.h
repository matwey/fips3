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

#ifndef _OPTIONAL_H
#define _OPTIONAL_H

#include <memory>
#include <type_traits>

#include <utils/exception.h>

namespace Utils {

class BadOptionalAccess: public Exception {
public:
	BadOptionalAccess(): Exception(QString("Cannot get value from empty optional")) {}
	virtual QException* clone() const override {
		return new BadOptionalAccess(*this);
	}
};

template<class T>
class Optional {
private:
	typename std::aligned_storage<sizeof(T), alignof(T)>::type value_;
	bool empty_;
public:
	inline Optional(): empty_(true) {
	};
	Optional(const Optional& other) {
		emplace(*other);
	}
	Optional(Optional&& other) {
		emplace(std::move(*other));
	}
	Optional(const T& value) {
		emplace(value);
	}
	Optional(T&& value) {
		emplace(std::move(value));
	}
	~Optional() {
		reset();
	}

	Optional& operator=(const Optional<T>& other) {
		if (empty_) {
			if (!other.empty_) {
				emplace(*other);
			}
		} else {
			if (!other.empty_) {
				this->operator*() = *other;
			} else {
				reset();
			}
		}

		return *this;
	}
	Optional& operator=(Optional<T>&& other) {
		if (empty_) {
			if (!other.empty_) {
				emplace(std::move(*other));
			}
		} else {
			if (!other.empty_) {
				this->operator*() = std::move(*other);
			} else {
				reset();
			}
		}

		return *this;
	}

	Optional& operator=(const T& value) {
		emplace(value);
		return *this;
	}
	Optional& operator=(T&& value) {
		emplace(std::move(value));
		return *this;
	}

	template<class... Args> T& emplace(Args&&... args) {
		reset();
		new (reinterpret_cast<T*>(&value_)) T(std::forward<Args...>(args...));
		empty_ = false;
		return *(reinterpret_cast<T*>(&value_));
	}

	void reset() noexcept {
		if (empty_) return;

		reinterpret_cast<T*>(&value_)->~T();
		empty_ = true;
	}

	inline const T& operator*() const& { return *reinterpret_cast<const T*>(&value_); }
	inline       T& operator*() &      { return *reinterpret_cast<T*>(&value_); }
	inline const T&& operator*() const&& { return *reinterpret_cast<const T*>(&value_); }
	inline       T&& operator*() &&      { return *reinterpret_cast<T*>(&value_); }

	inline const T* operator->() const { return reinterpret_cast<const T*>(&value_); }
	inline       T* operator->()       { return reinterpret_cast<T*>(&value_); }

	inline const T& value() const {
		if (empty_) throw BadOptionalAccess();

		return this->operator*();
	}
	inline T& value() {
		if (empty_) throw BadOptionalAccess();

		return this->operator*();
	}

	inline explicit operator bool() const { return !empty_; }
	inline bool operator!() const { return empty_; }

	template<class U> T value_or(U&& default_value) const& {
		return !empty_ ? **this : static_cast<T>(std::forward<U>(default_value));
	}
	template<class U> T value_or(U&& default_value) && {
		return !empty_ ? std::move(**this) : static_cast<T>(std::forward<U>(default_value));
	}
};

template<class T>
Optional<T> make_optional(const T& x) {
	return Optional<T>(x);
}

} // Utils

#endif //_OPTIONAL_H
