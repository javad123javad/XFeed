#ifndef XJSONADAPTER_H
#define XJSONADAPTER_H

#include <QObject>

class XJSonAdapter : public QObject
{
    Q_OBJECT
public:
    explicit XJSonAdapter(QObject *parent = nullptr);

signals:
};

#endif // XJSONADAPTER_H
