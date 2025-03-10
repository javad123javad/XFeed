#ifndef XFEEDMODEL_H
#define XFEEDMODEL_H
/**
 * This class is the main Model in MVC architecture. Its responisibility
 * is:
 * Update data: 1- Add/Remove folder, 2- Add/Remove channels
 * Notify Controller to update related componnents
 * Notift widgets that the data is updated.
 */

#include <QObject>
#include "xjsonadapter.h"
#include "XCommon.h"

class XFeedModel : public QObject
{
    Q_OBJECT
public:
    explicit XFeedModel(QObject *parent = nullptr);

    xfeed_model_t getModelFromData(const QString& dbPath);
    void addFolder(const QString& folderName);

private:
    XJSonAdapter jsonAdapter_;
    xfeed_model_t model_;

signals:
};

#endif // XFEEDMODEL_H
