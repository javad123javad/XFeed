#include "xjsonadapter.h"
#include <QJsonObject>
#include <QJsonArray>
#include "channelmodelbuilder.h"
XJSonAdapter::XJSonAdapter(const QString &file, QObject *parent)
    : fileName_(file), QObject{parent}
{

}

/**
 * @brief XJSonAdapter::readJsonFile
 * throws exception if the file is not valid
 */
void XJSonAdapter::readJsonFile()
{

    qDebug()<<"Data reading finished!";
    dbFile_.setFileName(fileName_);

    if(!dbFile_.open(QIODevice::ReadOnly | QIODevice::Text))
        throw std::logic_error("Unable to open db File");
    QString strJson = dbFile_.readAll();
    dbFile_.close();

    jdoc_ = QJsonDocument::fromJson(strJson.toUtf8());

    if(jdoc_.isEmpty())
        throw std::logic_error("Empty Json file");

}

void XJSonAdapter::exportToJson(const QJsonDocument& jdoc)
{

    QFile jfile(fileName_);
    jfile.open(QIODevice::WriteOnly);
    jfile.write(jdoc.toJson());
    jfile.close();
    //reload again
    readJsonFile();

}

std::shared_ptr<QStandardItemModel> XJSonAdapter::createModelFromJson()
{
    ChannelModelBuilder builder;


    readJsonFile();

    if (jdoc_.isNull()) {
        throw std::logic_error("Invalid JSON Document");
    }

    QJsonObject root = jdoc_.object();
    QJsonArray channelsArray = root["Channels"].toArray();

    /* First fix any problems in the db File! */
    builder.fixChannels(root);
    exportToJson(QJsonDocument(root));

    /* Now add the clean channels */
    readJsonFile();

    builder.addChannels(channelsArray);

    return builder.build();
}

const QJsonDocument XJSonAdapter::getJsonDoc() const
{
    return jdoc_;
}

void XJSonAdapter::setDbFileName(const QString &dBFile)
{
    if(dBFile.isEmpty())
        throw std::invalid_argument("Invalid File Name");

    this->fileName_ = dBFile;
}
