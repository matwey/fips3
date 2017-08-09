#ifndef _PALETTEWIDGET_H
#define _PALETTEWIDGET_H

#include <QtGui/qopengl.h>
#include <QButtonGroup>
#include <QDebug>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QWidget>

#include <memory>

class PaletteWidget: public QWidget {
public:
	enum PaletteType {
		GrayPalette,
		HalfHSLCirclePalette,
		PurpleBluePalette
	};

private:
	std::unique_ptr<QButtonGroup> button_group_;

public:
	explicit PaletteWidget(QWidget* parent);

	inline QButtonGroup* buttonGroup() const { return button_group_.get(); }
};

#endif //_PALETTEWIDGET_H