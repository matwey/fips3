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

#include <openglplan.h>

Uint8OpenGLPlan::Uint8OpenGLPlan(QObject* parent):
	AbstractOpenGLPlan("uint8", parent) {
}

Int16OpenGLPlan::Int16OpenGLPlan(QObject* parent):
	AbstractOpenGLPlan("int16", parent) {
}

Int32OpenGLPlan::Int32OpenGLPlan(QObject* parent):
	AbstractOpenGLPlan("int32", parent) {
}

Int64OpenGLPlan::Int64OpenGLPlan(QObject* parent):
	AbstractOpenGLPlan("int64", parent) {
}

FloatOpenGLPlan::FloatOpenGLPlan(QObject* parent):
	AbstractOpenGLPlan("float32", parent) {
}

