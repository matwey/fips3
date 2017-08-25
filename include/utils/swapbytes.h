#ifndef _UTILS_SWAPBYTES_H
#define _UTILS_SWAPBYTES_H

#include <QtGlobal>

namespace Utils {

namespace detail {

template<std::size_t N> struct swap_bytes_traits;

template<> struct swap_bytes_traits<1> {
	using type = quint8;

	static inline quint8 swap_bytes(quint8 x) {
		return x;
	}
};

template<> struct swap_bytes_traits<2> {
	using type = quint16;

	static inline quint16 swap_bytes(quint16 x) {
#if _MSC_VER
		return _byteswap_ushort(x);
#else
		return __builtin_bswap16(x);
#endif
	}
};

template<> struct swap_bytes_traits<4> {
	using type = quint32;

	static inline quint32 swap_bytes(quint32 x) {
#if _MSC_VER
		return _byteswap_ulong(x);
#else
		return __builtin_bswap32(x);
#endif
	}
};

template<> struct swap_bytes_traits<8> {
	using type = quint64;

	static inline quint64 swap_bytes(quint64 x) {
#if _MSC_VER
		return _byteswap_uint64(x);
#else
		return __builtin_bswap64(x);
#endif
	}
};

template<class T> union swap_bytes_union {
	using traits_type = detail::swap_bytes_traits<sizeof(T)>;

	T value;
	alignas(T) typename traits_type::type rep;
};

} // detail

template<class T> inline T swap_bytes(const T& x) {
	using union_type  = detail::swap_bytes_union<T>;
	using traits_type = typename union_type::traits_type;

	const union_type out{
		.rep = traits_type::swap_bytes(reinterpret_cast<const union_type*>(&x)->rep)
	};
	return out.value;
}

} // Utils

#endif // _UTILS_SWAPBYTES_H
