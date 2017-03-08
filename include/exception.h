#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <QException>

class Exception: public QException {
public:
	virtual void raise() const override;
	virtual QException* clone() const override;
};

#endif // _EXCEPTION_H_
