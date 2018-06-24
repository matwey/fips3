#include <openglplanfactory.h>
#include <openglplan.h>
#include <openglplanresolver.h>

OpenGLPlanFactory::OpenGLPlanFactory(const QOpenGLContext& opengl_context):
	features_{opengl_context} {
}

AbstractOpenGLPlan* OpenGLPlanFactory::operator() (const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu) const {
	return resolveOpenGLPlan<Uint8OpenGLPlan>(features_, hdu);
}

AbstractOpenGLPlan* OpenGLPlanFactory::operator() (const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu) const {
	return resolveOpenGLPlan<Int16OpenGLPlan>(features_, hdu);
}

AbstractOpenGLPlan* OpenGLPlanFactory::operator() (const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu) const {
	return resolveOpenGLPlan<Int32OpenGLPlan>(features_, hdu);
}

AbstractOpenGLPlan* OpenGLPlanFactory::operator() (const FITS::HeaderDataUnit<FITS::DataUnit<qint64>>& hdu) const {
	return resolveOpenGLPlan<Int64OpenGLPlan>(features_, hdu);
}

AbstractOpenGLPlan* OpenGLPlanFactory::operator() (const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu) const {
	return Q_NULLPTR;
}

AbstractOpenGLPlan* OpenGLPlanFactory::operator() (const FITS::HeaderDataUnit<FITS::DataUnit<double>>&) const {
	return Q_NULLPTR;
}

AbstractOpenGLPlan* OpenGLPlanFactory::operator() (const FITS::HeaderDataUnit<FITS::EmptyDataUnit>&) const {
	return Q_NULLPTR;
}
