#include <utils/openglrowalign.h>

namespace Utils {

int row_align(const quint64 bytes_width) {
	unsigned long index;
#if _MSC_VER
	_BitScanForward(&index, static_cast<quint32>(bytes_width)); // We are interesting on lower 4 bits only.
#else
	index = __builtin_ctzll(bytes_width);
#endif
	return 1 << std::min(3ul, index);
}

} // Utils
