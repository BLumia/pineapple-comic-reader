#include "libraryitem.h"

LibraryItem::LibraryItem(int id, const QString &name, const QString &uuid)
    : QStandardItem(QIcon::fromTheme(QIcon::ThemeIcon::FolderOpen), name)
{
    setData(id, IdRole);
    setData(uuid, UuidRole);
}
