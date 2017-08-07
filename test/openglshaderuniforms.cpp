#include <QtTest/QtTest>

#include <limits>

#include <openglshaderunifroms.h>

class TestOpenGLShaderUniforms: public QObject
{
Q_OBJECT
private slots:
	void test_a11();
	void test_a22();
	void test_a10();
	void test_z21();
//	void test_512_514();
};

void TestOpenGLShaderUniforms::test_a11() {
	OpenGLShaderUniforms su(1, 1, 0, 1);
	QCOMPARE(255, static_cast<int>(su.get_a()[0]));
};


void TestOpenGLShaderUniforms::test_a22() {
	OpenGLShaderUniforms su(2, 2, 0, 1);
	QCOMPARE(static_cast<quint64>(su.get_a()[0]), static_cast<quint64>(65535));
	QCOMPARE(static_cast<quint64>(su.get_a()[1]), static_cast<quint64>(65535) * 65536);
};

void TestOpenGLShaderUniforms::test_a10() {
	OpenGLShaderUniforms su(1, 0, 0, 1);
	QCOMPARE(static_cast<int>(su.get_a()[0]), 1);
}

void TestOpenGLShaderUniforms::test_z21() {
	OpenGLShaderUniforms su(2, 1, 0, 1);
	const double min = 512;
	const double max = 514;
	su.setMinMax(min, max);
	QCOMPARE(static_cast<int>(su.get_z()[0] * 255), 0);
	QCOMPARE(static_cast<int>(su.get_z()[1] * 255), 2);
	QCOMPARE(static_cast<int>(min), static_cast<int>(su.get_z()[0] * su.get_a()[0] + su.get_z()[1] * su.get_a()[1]));
}

/*
void TestOpenGLShaderUniforms::test_512_514() {
	OpenGLShaderUniforms su(4, 1, 0, 1);
	const double min = 512;
	const double max = 514;
	su.setMinMax(min, max);
	for (int x = 512; x < 516; ++x) {
		const auto x0_0 = (x % 256) / 255.0;
		const auto x0_1 = ((x / 256) % 256) / 255.0;
		const auto x2_actual = (x - min) / (max - min);
		const auto x2_expected = su.get_a()[0] / (max - min) * (x0_0 - su.get_z()[0])
				+ su.get_a()[1] / (max - min) * (x0_1 - su.get_z()[1])
				+ su.get_a()[2] / (max - min) * (0 - su.get_z()[2])
				+ su.get_a()[3] / (max - min) * (0 - su.get_z()[3]);
		qDebug() << x2_actual << x2_expected;
		QVERIFY(qAbs(x2_actual - x2_expected) < 8 * std::numeric_limits<GLfloat>::epsilon());
	}
}
*/

QTEST_MAIN(TestOpenGLShaderUniforms)
#include "openglshaderuniforms.moc"