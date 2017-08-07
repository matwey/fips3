#include <openglshaderunifroms.h>

OpenGLShaderUniforms::OpenGLShaderUniforms(quint8 channels, quint8 channel_size, double bzero, double bscale) :
		channels(channels),
		channel_size(channel_size),
		bzero(bzero),
		bscale(bscale) {
	if (channel_size > 0) {
		for (quint8 i = 0; i < channels; ++i) {
			a_[i] = static_cast<GLfloat >(std::pow(2, 8 * channel_size * i) * (std::pow(2, 8 * channel_size) - 1));
		}
	} else {
		a_[0] = 1;
	}
}
void OpenGLShaderUniforms::setMinMax(double minimum, double maximum) {
	const auto alpha = 1 / (maximum - minimum);
	auto minus_d = minimum - bzero;
	if (channel_size > 0) {
		double minus_d_mod_alpha_a;
		for (quint8 i = 0; i < channels; ++i) {
			c_[i] = static_cast<GLfloat>(bscale * alpha * a_[i]);
			const auto base = 1 << (8 * channel_size);
			z_[i] = static_cast<GLfloat>(std::fmod(minus_d, base) / 255);
			minus_d = std::floor(minus_d / base);
		}
	} else {
		c_[0] = static_cast<GLfloat>(bscale * alpha * a_[0]);
		z_[0] = static_cast<GLfloat>(minus_d / a_[0]);
	}
}