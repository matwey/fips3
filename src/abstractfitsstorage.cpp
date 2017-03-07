#include <abstractfitsstorage.h>

AbstractFITSStorage::AbstractFITSStorage(quint8* data, qint64 size):
	data_(data),
	size_(size) {
}
AbstractFITSStorage::~AbstractFITSStorage() = default;
