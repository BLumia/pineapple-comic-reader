#include "appcontroller.h"

#include "dataitems/libraryitem.h"
#include "dataitems/comicitem.h"
#include "dataitems/folderitem.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QRestAccessManager>
#include <QRestReply>
#include <QSettings>

AppController::AppController(QObject *parent)
    : QObject(parent)
    , m_networkAccessManager(new QNetworkAccessManager(this))
    , m_restAccessManager(new QRestAccessManager(m_networkAccessManager, this))
    , m_librariesModel(new QStandardItemModel(this))
    , m_comicsModel(new QStandardItemModel(this))
    , m_foldersModel(new QStandardItemModel(this))
{
    m_librariesModel->setItemRoleNames(LibraryItem::roleNames());
    m_comicsModel->setItemRoleNames(ComicItem::roleNames());
    m_foldersModel->setItemRoleNames(FolderItem::roleNames());
}

void AppController::connectServer(QUrl serverBaseUrl)
{
    setProperty("connectionState", Connecting);
    serverBaseUrl.setPath("/v2/");
    QNetworkRequestFactory api(serverBaseUrl);
    m_restAccessManager->get(api.createRequest("version"), this, [=](QRestReply &reply){
        if (reply.isSuccess()) {
            qDebug() << reply.readText();
            m_requestFactory.setBaseUrl(serverBaseUrl);
            m_requestFactory.clearCommonHeaders();
            QHttpHeaders commonHeaders;
            commonHeaders.append("X-Request-Id", "114514");
            m_requestFactory.setCommonHeaders(commonHeaders);
            setProperty("connectionState", Connected);
        } else {
            setProperty("connectionState", NotConnected);
        }
    });
}

void AppController::updateLibraries()
{
    m_restAccessManager->get(apiFactory().createRequest("libraries"), this, [=](QRestReply &reply){
        if (reply.isSuccess()) {
            std::optional<QJsonDocument> libraries = reply.readJson();
            if (libraries && !(*libraries).isEmpty() && (*libraries).isArray()) {
                const QJsonArray array = (*libraries).array();
                m_librariesModel->clear();
                for (const QJsonValue & value : array) {
                    QJsonObject libraryObj = value.toObject();
                    m_librariesModel->appendRow(new LibraryItem(libraryObj["id"].toInt(),
                                                                libraryObj["name"].toString(),
                                                                libraryObj["uuid"].toString()));
                }
            }
        }
    });
}

void AppController::updateComicsInFolder(QString folderId)
{
    qDebug() << folderId;
    m_restAccessManager->get(apiFactory().createRequest(QString("library/%1/folder/%2/content")
                                                            .arg(m_currentLibraryId).arg(folderId)),
                             this, [=](QRestReply &reply){
        qDebug() << m_currentLibraryId << folderId << reply.httpStatus() << reply.errorString();
        if (reply.isSuccess()) {
            std::optional<QJsonDocument> libraries = reply.readJson();
            if (libraries && !(*libraries).isEmpty() && (*libraries).isArray()) {
                const QJsonArray array = (*libraries).array();
                m_comicsModel->clear();
                m_foldersModel->clear();
                // always add a top-leven folder entry...for now
                QJsonObject root;
                root["id"] = "1";
                m_foldersModel->appendRow(new FolderItem(root, "Root"));
                for (const QJsonValue & value : array) {
                    QJsonObject obj = value.toObject();
                    // qDebug() << obj;
                    if (obj["type"].toString() == QLatin1String("folder")) {
                        m_foldersModel->appendRow(new FolderItem(obj, obj["folder_name"].toString()));
                    } else {
                        m_comicsModel->appendRow(new ComicItem(obj, obj["file_name"].toString()));
                    }
                }
            }
        }
    });
}

void AppController::openComic()
{
    m_restAccessManager->get(apiFactory().createRequest(QString("library/%1/comic/%2/remote")
                                                            .arg(m_currentLibraryId).arg(m_currentComicId)),
                            this, [=](QRestReply &reply){
        qDebug() << m_currentLibraryId << m_currentComicId << reply.httpStatus() << reply.errorString();
        if (reply.isSuccess()) {
            setProperty("selectedComicOpened", true);
        } else {
            setProperty("selectedComicOpened", false);
        }
    });
}

void AppController::closeComic()
{
    // the api seems not proceed by the server, so we simply do nothing but set the property
    setProperty("selectedComicOpened", false);
}

QString AppController::coverImageSource(QString comicHash)
{
    QUrl url(apiFactory().baseUrl());
    url.setPath(url.path() + "library/" + QString::number(m_currentLibraryId) + "/cover/" + comicHash + ".jpg");
    return url.toString();
}

QModelIndex AppController::currentComicModelIndex()
{
    if (m_currentComicId.isEmpty()) return QModelIndex();
    for (int i = 0; i < m_comicsModel->rowCount(); i++) {
        QModelIndex index = m_comicsModel->index(i, 0);
        if (m_comicsModel->data(index, ComicItem::IdRole) == m_currentComicId) {
            return index;
        }
    }
    return QModelIndex();
}

QString AppController::comicImageSource(int page)
{
    QNetworkRequest req = apiFactory().createRequest(QString("library/%1/comic/%2/page/%3/remote")
                                                            .arg(m_currentLibraryId)
                                                            .arg(m_currentComicId)
                                                            .arg(page));

    return req.url().toString();
}

QNetworkRequestFactory AppController::apiFactory() const
{
    return m_requestFactory;
}
