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

public:
    std::shared_ptr<QStandardItemModel> createModelFromJson();
    const QJsonDocument getJsonDoc() const;
    void setDbFileName(const QString& dBFile);
    void exportToJson(const QJsonDocument& jdoc);
    void readJsonFile();


private:
    QString fileName_;
    QFile dbFile_;
    QJsonDocument jdoc_;


signals:
};

#endif // XJSONADAPTER_H
