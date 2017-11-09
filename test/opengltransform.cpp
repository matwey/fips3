#include <QtTest/QtTest>

#include <openglplane.h>
#include <opengltransform.h>

class TestWidgetToFitsOpenGLTransform: public QObject
{
Q_OBJECT
private slots:
	void test_1x1();
	void test_1x1_rot90();
	void test_1x2();
};

void TestWidgetToFitsOpenGLTransform::test_1x1() {
	const QSize image_size{3, 3};
	const QSize widget_size{3, 3};
	const QRectF view_rect{-1, -1, 2, 2};

	OpenGLPlane p(image_size);
	WidgetToFitsOpenGLTransform t(image_size, p.scale(), widget_size, view_rect);

	{
		auto f = t.transform(0, 0);
		QCOMPARE(static_cast<int>(f.x()), 0);
		QCOMPARE(static_cast<int>(f.y()), 2);
	}
	{
		auto f = t.transform(1, 1);
		QCOMPARE(static_cast<int>(f.x()), 1);
		QCOMPARE(static_cast<int>(f.y()), 1);
	}
	{
		auto f = t.transform(2, 2);
		QCOMPARE(static_cast<int>(f.x()), 2);
		QCOMPARE(static_cast<int>(f.y()), 0);
	}
	{
		auto f = t.transform(0, 2);
		QCOMPARE(static_cast<int>(f.x()), 0);
		QCOMPARE(static_cast<int>(f.y()), 0);
	}
	{
		auto f = t.transform(2, 0);
		QCOMPARE(static_cast<int>(f.x()), 2);
		QCOMPARE(static_cast<int>(f.y()), 2);
	}
}

void TestWidgetToFitsOpenGLTransform::test_1x1_rot90() {
	const QSize image_size{3, 3};
	const QSize widget_size{3, 3};
	const QRectF view_rect{-1, -1, 2, 2};

	OpenGLPlane p(image_size);
	WidgetToFitsOpenGLTransform t(image_size, p.scale(), widget_size, view_rect);
	t.setRotation(90);

	{
		auto f = t.transform(0, 0);
		QCOMPARE(static_cast<int>(f.x()), 2);
		QCOMPARE(static_cast<int>(f.y()), 2);
	}
	{
		auto f = t.transform(1, 1);
		QCOMPARE(static_cast<int>(f.x()), 1);
		QCOMPARE(static_cast<int>(f.y()), 1);
	}
	{
		auto f = t.transform(2, 2);
		QCOMPARE(static_cast<int>(f.x()), 0);
		QCOMPARE(static_cast<int>(f.y()), 0);
	}
	{
		auto f = t.transform(0, 2);
		QCOMPARE(static_cast<int>(f.x()), 0);
		QCOMPARE(static_cast<int>(f.y()), 2);
	}
	{
		auto f = t.transform(2, 0);
		QCOMPARE(static_cast<int>(f.x()), 2);
		QCOMPARE(static_cast<int>(f.y()), 0);
	}
}

void TestWidgetToFitsOpenGLTransform::test_1x2() {
	const QSize image_size{3, 3};
	const QSize widget_size{6, 6};
	const QRectF view_rect{-1, -1, 2, 2};

	OpenGLPlane p(image_size);
	WidgetToFitsOpenGLTransform t(image_size, p.scale(), widget_size, view_rect);

	{
		auto f = t.transform(0, 0);
		QCOMPARE(static_cast<int>(f.x()), 0);
		QCOMPARE(static_cast<int>(f.y()), 2);
	}
	{
		auto f = t.transform(1, 1);
		QCOMPARE(static_cast<int>(f.x()), 0);
		QCOMPARE(static_cast<int>(f.y()), 2);
	}
	{
		auto f = t.transform(2, 2);
		QCOMPARE(static_cast<int>(f.x()), 1);
		QCOMPARE(static_cast<int>(f.y()), 1);
	}
	{
		auto f = t.transform(3, 3);
		QCOMPARE(static_cast<int>(f.x()), 1);
		QCOMPARE(static_cast<int>(f.y()), 1);
	}
	{
		auto f = t.transform(4, 4);
		QCOMPARE(static_cast<int>(f.x()), 2);
		QCOMPARE(static_cast<int>(f.y()), 0);
	}
	{
		auto f = t.transform(5, 5);
		QCOMPARE(static_cast<int>(f.x()), 2);
		QCOMPARE(static_cast<int>(f.y()), 0);
	}
	{
		auto f = t.transform(0, 5);
		QCOMPARE(static_cast<int>(f.x()), 0);
		QCOMPARE(static_cast<int>(f.y()), 0);
	}
	{
		auto f = t.transform(5, 0);
		QCOMPARE(static_cast<int>(f.x()), 2);
		QCOMPARE(static_cast<int>(f.y()), 2);
	}
}

QTEST_GUILESS_MAIN(TestWidgetToFitsOpenGLTransform)
#include "opengltransform.moc"
