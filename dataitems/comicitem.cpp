#include "comicitem.h"

ComicItem::ComicItem(QJsonObject jsonObj, const QString &name)
    : QStandardItem(name)
{
    setData(jsonObj["id"].toString(), IdRole);
    setData(jsonObj["hash"].toString(), HashRole);
    setData(jsonObj["num_pages"].toInt(), PageCountRole);
    setData(jsonObj["current_page"].toInt(), CurrentPageRole);
    setData(jsonObj["type"].toInt(), TypeRole);
}
