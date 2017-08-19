#include <QtTest/QtTest>

#include <openglshaderunifroms.h>

class TestOpenGLShaderUniforms: public QObject
{
Q_OBJECT
private slots:
	void test_a11();
	void test_a22();
	void test_a10();
	void test_z21();
	void test_512_514();
	void test_kgo_image();
};

void TestOpenGLShaderUniforms::test_a11() {
	OpenGLShaderUniforms su(1, 1, 0, 1);
	QCOMPARE(255, static_cast<int>(su.get_a()[0]));
};

void TestOpenGLShaderUniforms::test_a22() {
	OpenGLShaderUniforms su(2, 2, 0, 1);
	QCOMPARE(static_cast<quint64>(su.get_a()[0]), static_cast<quint64>(65535) * 65536);
	QCOMPARE(static_cast<quint64>(su.get_a()[1]), static_cast<quint64>(65535));
};

void TestOpenGLShaderUniforms::test_a10() {
	OpenGLShaderUniforms su(1, 0, 0, 1);
	QCOMPARE(static_cast<int>(su.get_a()[0]), 1);
}

void TestOpenGLShaderUniforms::test_z21() {
	OpenGLShaderUniforms su(2, 1, 0, 1);
	const double min = 512;
	const double max = 514;
	su.setMinMax(std::make_pair(min, max));
	su.setColorMapSize(2);
	/* Actual min = 511 = 0x01FF (taking ColorMapSize into account) */
	QCOMPARE(static_cast<int>(su.get_z()[0] * 255), 1);
	QCOMPARE(static_cast<int>(su.get_z()[1] * 255), 255);
	QCOMPARE(static_cast<int>(su.get_z()[0] * su.get_a()[0] + su.get_z()[1] * su.get_a()[1]), static_cast<int>(min-1));
}

void TestOpenGLShaderUniforms::test_512_514() {
	OpenGLShaderUniforms su(2, 1, 0, 1);
	const double min = 512;
	const double max = 514;
	const int colormap_size = 2;
	su.setMinMax(std::make_pair(min, max));
	su.setColorMapSize(colormap_size);
	const double actual_min = min - (max - min) / (2.0 * (colormap_size - 1.0));
	const double actual_max = max + (max - min) / (2.0 * (colormap_size - 1.0));
	for (int x = 512; x < 516; ++x) {
		const auto x0_1 = (x % 256) / 255.0;
		const auto x0_0 = ((x / 256) % 256) / 255.0;
		const auto x2_expected = (x - actual_min) / (actual_max - actual_min);
		const auto x2_actual = su.get_a()[0] / (actual_max - actual_min) * (x0_0 - su.get_z()[0])
				+ su.get_a()[1] / (actual_max - actual_min) * (x0_1 - su.get_z()[1]);
		QVERIFY(qAbs(x2_actual - x2_expected) < 0.5 / su.base);
	}
}

void TestOpenGLShaderUniforms::test_kgo_image() {
	const double bzero = std::pow(2, 31);
	OpenGLShaderUniforms su(4, 1, bzero, 1);
	const double min = 7567;
	const double max = 753404;
	const int colormap_size = 2;
	su.setMinMax(std::make_pair(min, max));
	su.setColorMapSize(colormap_size);
	const double actual_min = min - (max - min) / (2.0 * (colormap_size - 1.0));
	const double actual_max = max + (max - min) / (2.0 * (colormap_size - 1.0));
	const double d_expected = -actual_min / (actual_max - actual_min) + bzero / (actual_max - actual_min);
	double d_actual = 0;
 	for (int j = 0; j < 4; ++j) {
		d_actual += -su.get_z()[j] * su.get_a()[j] / (actual_max - actual_min);
	}
	QVERIFY(qAbs(d_actual - d_expected) < 0.5);
}


QTEST_MAIN(TestOpenGLShaderUniforms)
#include "openglshaderuniforms.moc"
