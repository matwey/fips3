#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <QException>
#include <QString>

class Exception: public QException {
private:
	QString what_;
public:
	explicit Exception(const QString& what);

	virtual void raise() const override;
	virtual QException* clone() const override;

	virtual const char* what() const noexcept override;
};

#endif // _EXCEPTION_H_
