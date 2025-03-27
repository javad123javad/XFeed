#include "xfeedxmlreader.h"
#include <QDebug>
#include <QStandardItemModel>
#include <QMap>
#include <QUrl>
#include <QTextDocument>
#include <QRegularExpression>
XFeedXMLReader::XFeedXMLReader() {}

void XFeedXMLReader::parseFeed(QIODevice* feed_data, QStandardItemModel& feedDataModel)
{
    xml_.setDevice(feed_data);

    // Set table headers
    feedDataModel.clear();
    feedDataModel.setColumnCount(3); // Add a column for images
    feedDataModel.setHorizontalHeaderLabels({"Title", "Link", "Pub Date"});

    while (!xml_.atEnd()) {
        QXmlStreamReader::TokenType token = xml_.readNext();

        if (token == QXmlStreamReader::StartElement && xml_.name() == "item") {
            QList<QStandardItem*> rowItems;
            QStandardItem *titleItem = new QStandardItem();
            QStandardItem *linkItem = new QStandardItem();
            QStandardItem *dateItem = new QStandardItem();
            QString descriptionText;
            QString linkText;

            while (!(xml_.tokenType() == QXmlStreamReader::EndElement && xml_.name() == "item")) {
                xml_.readNext();

                if (xml_.tokenType() == QXmlStreamReader::StartElement) {
                    QString localName = xml_.name().toString();
                    QString namespaceUri = xml_.namespaceUri().toString();
                    QString text = xml_.readElementText(QXmlStreamReader::IncludeChildElements);

                    if (localName == "title") {
                        titleItem->setText(text);
                    }
                    else if (localName == "link") {
                        linkText = text;
                        linkItem->setText(text);
                    }
                    else if (localName == "description" ||
                             (localName == "encoded" && namespaceUri.contains("content"))) {
                        descriptionText = text;
                    }
                    else if (localName == "pubDate" ||
                             (localName == "date" && namespaceUri.contains("dc"))) {
                        dateItem->setText(text);
                    }
                }
            }

            // Store the full HTML description in the title item for QTextBrowser
            titleItem->setData(descriptionText, Qt::UserRole);
            linkItem->setData(linkText, Qt::UserRole);

            rowItems << titleItem << linkItem << dateItem;
            feedDataModel.appendRow(rowItems);
        }
    }

    if (xml_.hasError()) {
        qDebug() << "XML Error:" << xml_.errorString();
    }
}


