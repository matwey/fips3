#include <QtTest/QtTest>

#include <openglplane.h>

class TestOpenGLPlane: public QObject
{
Q_OBJECT
private slots:
	void test_plane1x1();
	void test_plane1x2();
	void test_plane2x1();
	void test_plane10x10();
	void test_plane10x1000();
};

void TestOpenGLPlane::test_plane1x1() {
	OpenGLPlane pl{QSize{1,1}};
	auto p = pl.planeRect();
	QCOMPARE(p.left(), -1.);
	QCOMPARE(p.top(), -1.);
	QCOMPARE(p.right(), 1.);
	QCOMPARE(p.bottom(), 1.);
	QCOMPARE(pl.scale(), 1.);
}

void TestOpenGLPlane::test_plane1x2() {
	OpenGLPlane pl{QSize{1,2}};
	auto p = pl.planeRect();
	QCOMPARE(p.left(), -0.5);
	QCOMPARE(p.top(), -1.);
	QCOMPARE(p.right(), 0.5);
	QCOMPARE(p.bottom(), 1.);
	QCOMPARE(pl.scale(), 0.5);
}

void TestOpenGLPlane::test_plane2x1() {
	OpenGLPlane pl{QSize{2,1}};
	auto p = pl.planeRect();
	QCOMPARE(p.left(), -1.);
	QCOMPARE(p.top(), -0.5);
	QCOMPARE(p.right(), 1.);
	QCOMPARE(p.bottom(), 0.5);
	QCOMPARE(pl.scale(), 0.5);
}

void TestOpenGLPlane::test_plane10x10() {
	OpenGLPlane pl{QSize{10,10}};
	auto p = pl.planeRect();
	QCOMPARE(p.left(), -1.);
	QCOMPARE(p.top(), -1.);
	QCOMPARE(p.right(), 1.);
	QCOMPARE(p.bottom(), 1.);
	QCOMPARE(static_cast<int>(1.0/pl.scale()), 10);
}

void TestOpenGLPlane::test_plane10x1000() {
	OpenGLPlane pl{QSize{10,1000}};
	auto p = pl.planeRect();
	QCOMPARE(p.left(), -static_cast<qreal>(0.01));
	QCOMPARE(p.top(), -1.);
	QCOMPARE(p.right(), static_cast<qreal>(0.01));
	QCOMPARE(p.bottom(), 1.);
	QCOMPARE(static_cast<int>(1.0/pl.scale()), 1000);
}

QTEST_MAIN(TestOpenGLPlane)
#include "openglplane.moc"
