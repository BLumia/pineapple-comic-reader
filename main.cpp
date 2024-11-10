#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlNetworkAccessManagerFactory>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

#include "dataitems/libraryitem.h"
#include "dataitems/comicitem.h"
#include "dataitems/folderitem.h"

class QmlNetworkAccessManager : public QNetworkAccessManager
{
public:
    QmlNetworkAccessManager(QObject *parent = nullptr) : QNetworkAccessManager(parent) {}

protected:
    QNetworkReply *createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData = nullptr) override
    {
        QNetworkRequest newRequest(request);
        newRequest.setRawHeader("X-Request-Id", "114514");
        return QNetworkAccessManager::createRequest(op, newRequest, outgoingData);
    }
};

class QmlNetworkAccessManagerFactory : public QQmlNetworkAccessManagerFactory
{
public:
    inline QNetworkAccessManager *create(QObject *parent) override
    {
        return new QmlNetworkAccessManager(parent);
    }
};

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qputenv("QT_QUICK_CONTROLS_STYLE", QByteArray("FluentWinUI3"));

    qmlRegisterUncreatableMetaObject(LibraryItem::staticMetaObject, "net.blumia.pineapple.comic.reader.libraryitem", 1, 0, "LibraryItem", "enum");
    qmlRegisterUncreatableMetaObject(ComicItem::staticMetaObject, "net.blumia.pineapple.comic.reader.comicitem", 1, 0, "ComicItem", "enum");
    qmlRegisterUncreatableMetaObject(FolderItem::staticMetaObject, "net.blumia.pineapple.comic.reader.folderitem", 1, 0, "FolderItem", "enum");

    QmlNetworkAccessManagerFactory namFactory;

    QQmlApplicationEngine engine;
    engine.setNetworkAccessManagerFactory(&namFactory);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("net.blumia.pineapple.comic.reader", "Main");

    return app.exec();
}
