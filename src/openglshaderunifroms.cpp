#include <openglshaderunifroms.h>

OpenGLShaderUniforms::OpenGLShaderUniforms(quint8 channels, quint8 channel_size, double bzero, double bscale) :
		channels(channels),
		channel_size(channel_size),
		bzero(bzero),
		bscale(bscale),
		base(1 << (8 * channel_size)) {
	if (channel_size > 0) {
		for (quint8 i = 0; i < channels; ++i) {
			const auto j = channels - i - 1;
			a_[j] = static_cast<GLfloat >(std::pow(2, 8 * channel_size * i) * (std::pow(2, 8 * channel_size) - 1));
		}
	} else {
		a_[0] = 1;
	}
}

void OpenGLShaderUniforms::setMinMax(double minimum, double maximum) {
	const auto alpha = 1 / (maximum - minimum);
	auto minus_d = minimum - bzero;
	if (channel_size > 0) {
		for (quint8 i = 0; i < channels; ++i) {
			const auto j = channels - i - 1;
			c_[j] = static_cast<GLfloat>(bscale * alpha * a_[j]);
			z_[j] = static_cast<GLfloat>(std::fmod(minus_d, base) / (base - 1));
			minus_d = std::floor(minus_d / base);
		}
	} else {
		c_[0] = static_cast<GLfloat>(bscale * alpha * a_[0]);
		z_[0] = static_cast<GLfloat>(minus_d / a_[0]);
	}
}