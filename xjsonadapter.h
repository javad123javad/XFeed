#ifndef XJSONADAPTER_H
#define XJSONADAPTER_H

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QStandardItemModel>

class XJSonAdapter : public QObject
{
    Q_OBJECT
public:
    friend class XFeedTest;
    explicit XJSonAdapter(const QString &file = "", QObject *parent = nullptr);
private:
    void readJsonFile();
    void exportToJson(const QString file = "");
public:
    std::unique_ptr<QStandardItemModel> createModelFromJson();
    const QJsonDocument getJsonDoc() const;


private:
    QString fileName_;
    QFile dbFile_;
    QJsonDocument jdoc_;


signals:
};

#endif // XJSONADAPTER_H
