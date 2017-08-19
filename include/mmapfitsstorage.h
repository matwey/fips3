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

#ifndef _MMAPFITSSTORAGE_H_
#define _MMAPFITSSTORAGE_H_

#include <QFileDevice>

#include <memory>

#include <abstractfitsstorage.h>

class MMapFITSStorage: public AbstractFITSStorage {
private:
	std::unique_ptr<QFileDevice> file_device_;
public:
	MMapFITSStorage(QFileDevice* file_device);
	virtual ~MMapFITSStorage() override;
};

#endif // _MMAPFITSSTORAGE_H_
