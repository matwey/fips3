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

#ifndef _OPENGLPLANFACTORY_H
#define _OPENGLPLANFACTORY_H

#include <openglplan.h>
#include <openglfeatures.h>

class OpenGLPlanFactory {
private:
	OpenGLFeatures features_;
public:
	explicit OpenGLPlanFactory(const QOpenGLContext& opengl_context);
	OpenGLPlanFactory(const OpenGLPlanFactory&) = default;
	OpenGLPlanFactory(OpenGLPlanFactory&&) = default;

	OpenGLPlanFactory& operator=(const OpenGLPlanFactory&) = default;
	OpenGLPlanFactory& operator=(OpenGLPlanFactory&&) = default;

	AbstractOpenGLPlan* operator() (const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu) const;
	AbstractOpenGLPlan* operator() (const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu) const;
	AbstractOpenGLPlan* operator() (const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu) const;
	AbstractOpenGLPlan* operator() (const FITS::HeaderDataUnit<FITS::DataUnit<qint64>>& hdu) const;
	AbstractOpenGLPlan* operator() (const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu) const;
	AbstractOpenGLPlan* operator() (const FITS::HeaderDataUnit<FITS::DataUnit<double>>&) const;
	AbstractOpenGLPlan* operator() (const FITS::HeaderDataUnit<FITS::EmptyDataUnit>&) const;
};

#endif // _OPENGLPLANFACTORY_H
