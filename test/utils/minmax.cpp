#include <cmath>
#include <limits>
#include <QtTest/QtTest>
#include <utils/minmax.h>

class TestMinMax: public QObject
{
	Q_OBJECT
private slots:
	void min8();
	void min32_signed();
	void min32_float();
	void min64_float();
	void max8();
	void max32_signed();
	void max32_float();
	void max64_float();
};

void TestMinMax::min8() {
	quint8 a = 0;
	quint8 b = 1;
	QCOMPARE(Utils::min(a, b), a);
	QCOMPARE(Utils::min(b, a), a);
}

void TestMinMax::min32_signed() {
	qint32 a = 0;
	qint32 b = 1;
	QCOMPARE(Utils::min(a, b), a);
	QCOMPARE(Utils::min(b, a), a);
}

void TestMinMax::min32_float() {
	float a = 0;
	float b = 1;
	float nan = NAN;
	QCOMPARE(Utils::min(a, b), a);
	QCOMPARE(Utils::min(b, a), a);
	QCOMPARE(Utils::min(a, nan), a);
	QCOMPARE(Utils::min(nan, a), a);
	QVERIFY(std::isnan(Utils::min(nan, nan)));
}

void TestMinMax::min64_float() {
	double a = 0;
	double b = 1;
	double nan = NAN;
	QCOMPARE(Utils::min(a, b), a);
	QCOMPARE(Utils::min(b, a), a);
	QCOMPARE(Utils::min(a, nan), a);
	QCOMPARE(Utils::min(nan, a), a);
	QVERIFY(std::isnan(Utils::min(nan, nan)));
}

void TestMinMax::max8() {
	quint8 a = 1;
	quint8 b = 0;
	QCOMPARE(Utils::max(a, b), a);
	QCOMPARE(Utils::max(b, a), a);
}

void TestMinMax::max32_signed() {
	qint32 a = 1;
	qint32 b = 0;
	QCOMPARE(Utils::max(a, b), a);
	QCOMPARE(Utils::max(b, a), a);
}

void TestMinMax::max32_float() {
	float a = 1;
	float b = 0;
	float nan = std::numeric_limits<float>::quiet_NaN();
	QCOMPARE(Utils::max(a, b), a);
	QCOMPARE(Utils::max(b, a), a);
	QCOMPARE(Utils::max(a, nan), a);
	QCOMPARE(Utils::max(nan, a), a);
	QVERIFY(std::isnan(Utils::max(nan, nan)));
}

void TestMinMax::max64_float() {
	double a = 1;
	double b = 0;
	double nan = std::numeric_limits<double>::quiet_NaN();
	QCOMPARE(Utils::max(a, b), a);
	QCOMPARE(Utils::max(b, a), a);
	QCOMPARE(Utils::max(a, nan), a);
	QCOMPARE(Utils::max(nan, a), a);
	QVERIFY(std::isnan(Utils::max(nan, nan)));
}

QTEST_MAIN(TestMinMax)
#include "minmax.moc"
