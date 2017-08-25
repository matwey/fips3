#include <utils/openglrowalign.h>

namespace Utils {

int row_align(const quint64 bytes_width) {
	const quint64 max_align_mask = 8 - 1;

	Q_ASSERT(bytes_width);

	return (((bytes_width ^ (bytes_width - 1)) >> 1) & max_align_mask) + 1;
}

} // Utils
