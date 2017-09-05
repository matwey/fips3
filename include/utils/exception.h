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

#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <QException>
#include <QString>

namespace Utils {

class Exception : public QException {
private:
	QString what_;
public:
	explicit Exception(const QString& what);

	virtual void raise() const override;

	virtual QException* clone() const override;

	virtual const char* what() const noexcept override;
};

} // Utils

#endif // _EXCEPTION_H_
