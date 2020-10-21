#include <openglplanfactory.h>
#include <openglplan.h>
#include <openglplanresolver.h>

OpenGLPlanFactory::OpenGLPlanFactory(const QOpenGLContext& opengl_context):
	features_{opengl_context} {
}

AbstractOpenGLPlan* OpenGLPlanFactory::operator() (const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu) const {
	if (hdu.data().depth() > 1)
		return resolveOpenGLPlan<Uint8OpenGL33ArrayPlan, Uint8OpenGL30ArrayPlan, Uint8OpenGLArrayPlan>(features_, hdu);

	return resolveOpenGLPlan<Uint8OpenGL33Plan, Uint8OpenGL30Plan, Uint8OpenGLPlan, Uint8OpenGLES30Plan>(features_, hdu);
}

AbstractOpenGLPlan* OpenGLPlanFactory::operator() (const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu) const {
	if (hdu.data().depth() > 1)
		return resolveOpenGLPlan<Int16OpenGL33ArrayPlan, Int16OpenGL30ArrayPlan, Int16OpenGLArrayPlan>(features_, hdu);

	return resolveOpenGLPlan<Int16OpenGL33Plan, Int16OpenGL30Plan, Int16OpenGLPlan, Int16OpenGLES30Plan>(features_, hdu);
}

AbstractOpenGLPlan* OpenGLPlanFactory::operator() (const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu) const {
	if (hdu.data().depth() > 1)
		return resolveOpenGLPlan<Int32OpenGL33ArrayPlan, Int32OpenGL30ArrayPlan, Int32OpenGLArrayPlan>(features_, hdu);

	return resolveOpenGLPlan<Int32OpenGL33Plan, Int32OpenGL30Plan, Int32OpenGLPlan, Int32OpenGLES30Plan>(features_, hdu);
}

AbstractOpenGLPlan* OpenGLPlanFactory::operator() (const FITS::HeaderDataUnit<FITS::DataUnit<qint64>>& hdu) const {
	if (hdu.data().depth() > 1)
		return resolveOpenGLPlan<Int64OpenGL33ArrayPlan, Int64OpenGLArrayPlan>(features_, hdu);

	return resolveOpenGLPlan<Int64OpenGL33Plan, Int64OpenGLPlan>(features_, hdu);
}

AbstractOpenGLPlan* OpenGLPlanFactory::operator() (const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu) const {
	if (hdu.data().depth() > 1)
		return resolveOpenGLPlan<FloatOpenGL33ArrayPlan, FloatOpenGL30ArrayPlan>(features_, hdu);

	return resolveOpenGLPlan<FloatOpenGL33Plan, FloatOpenGL30Plan>(features_, hdu);
}

AbstractOpenGLPlan* OpenGLPlanFactory::operator() (const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu) const {
	if (hdu.data().depth() > 1)
		return resolveOpenGLPlan<DoubleOpenGL33ArrayPlan, DoubleOpenGL30ArrayPlan>(features_, hdu);

	return resolveOpenGLPlan<DoubleOpenGL33Plan, DoubleOpenGL30Plan>(features_, hdu);
}

AbstractOpenGLPlan* OpenGLPlanFactory::operator() (const FITS::HeaderDataUnit<FITS::EmptyDataUnit>&) const {
	return Q_NULLPTR;
}
