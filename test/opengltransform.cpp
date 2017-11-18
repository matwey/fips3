#include <QtTest/QtTest>

#include <openglplane.h>
#include <opengltransform.h>

class TestWidgetToFitsOpenGLTransform: public QObject
{
Q_OBJECT
private slots:
	void test_1x1();
	void test_1x1a();
	void test_1x1_rot90();
	void test_1x1_vr2();
	void test_1x1_vr2a();
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

void TestWidgetToFitsOpenGLTransform::test_1x1a() {
	const QSize image_size{6, 3};
	const QSize widget_size{6, 3};
	const QRectF view_rect{-1, -0.5, 2, 1};

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
	{
		auto f = t.transform(5, 0);
		QCOMPARE(static_cast<int>(f.x()), 5);
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

void TestWidgetToFitsOpenGLTransform::test_1x1_vr2() {
	const QSize image_size{4, 4};
	const QSize widget_size{2, 2};
	const QRectF view_rect{-1, -1, 1, 1};

	OpenGLPlane p(image_size);
	WidgetToFitsOpenGLTransform t(image_size, p.scale(), widget_size, view_rect);

	{
		auto f = t.transform(0, 0);
		QCOMPARE(static_cast<int>(f.x()), 0);
		QCOMPARE(static_cast<int>(f.y()), 3);
	}
	{
		auto f = t.transform(0, 1);
		QCOMPARE(static_cast<int>(f.x()), 0);
		QCOMPARE(static_cast<int>(f.y()), 2);
	}
	{
		auto f = t.transform(1, 0);
		QCOMPARE(static_cast<int>(f.x()), 1);
		QCOMPARE(static_cast<int>(f.y()), 3);
	}
	{
		auto f = t.transform(1, 1);
		QCOMPARE(static_cast<int>(f.x()), 1);
		QCOMPARE(static_cast<int>(f.y()), 2);
	}
}

void TestWidgetToFitsOpenGLTransform::test_1x1_vr2a() {
	const QSize image_size{8, 4};
	const QSize widget_size{4, 2};
	const QRectF view_rect{-1, -0.5, 1, 0.5};

	OpenGLPlane p(image_size);
	WidgetToFitsOpenGLTransform t(image_size, p.scale(), widget_size, view_rect);

	{
		auto f = t.transform(0, 0);
		QCOMPARE(static_cast<int>(f.x()), 0);
		QCOMPARE(static_cast<int>(f.y()), 3);
	}
	{
		auto f = t.transform(0, 1);
		QCOMPARE(static_cast<int>(f.x()), 0);
		QCOMPARE(static_cast<int>(f.y()), 2);
	}
	{
		auto f = t.transform(2, 0);
		QCOMPARE(static_cast<int>(f.x()), 2);
		QCOMPARE(static_cast<int>(f.y()), 3);
	}
	{
		auto f = t.transform(2, 1);
		QCOMPARE(static_cast<int>(f.x()), 2);
		QCOMPARE(static_cast<int>(f.y()), 2);
	}
	{
		auto f = t.transform(3, 1);
		QCOMPARE(static_cast<int>(f.x()), 3);
		QCOMPARE(static_cast<int>(f.y()), 2);
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
