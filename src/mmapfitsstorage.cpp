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

#include <mmapfitsstorage.h>

MMapFITSStorage::MMapFITSStorage(QFileDevice* file_device):
	AbstractFITSStorage(
		static_cast<quint8*>(file_device->map(static_cast<qint64>(0), file_device->size(), QFileDevice::MapPrivateOption)),
		file_device->size()),
	file_device_(file_device) {
}
MMapFITSStorage::~MMapFITSStorage() {
	file_device_->unmap(static_cast<uchar*>(data()));
}
