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

#ifndef _OPENGLPLAN_H
#define _OPENGLPLAN_H

#include <abstractopenglplan.h>

class Uint8OpenGLPlan:
	public AbstractOpenGLPlan {
public:
	Uint8OpenGLPlan(QObject* parent = Q_NULLPTR);
	virtual ~Uint8OpenGLPlan() override = default;
};

class Int16OpenGLPlan:
	public AbstractOpenGLPlan {
public:
	Int16OpenGLPlan(QObject* parent = Q_NULLPTR);
	virtual ~Int16OpenGLPlan() override = default;
};

class Int32OpenGLPlan:
	public AbstractOpenGLPlan {
public:
	Int32OpenGLPlan(QObject* parent = Q_NULLPTR);
	virtual ~Int32OpenGLPlan() override = default;
};

class Int64OpenGLPlan:
	public AbstractOpenGLPlan {
public:
	Int64OpenGLPlan(QObject* parent = Q_NULLPTR);
	virtual ~Int64OpenGLPlan() override = default;
};

class FloatOpenGLPlan:
	public AbstractOpenGLPlan {
public:
	FloatOpenGLPlan(QObject* parent = Q_NULLPTR);
	virtual ~FloatOpenGLPlan() override = default;
};

#endif // _OPENGLPLAN_H
