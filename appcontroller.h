#pragma once

#include <QQmlEngine>
#include <QNetworkRequestFactory>
#include <QRestAccessManager>
#include <QStandardItemModel>

class AppController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    enum ConnectionState {
        NotConnected,
        Connecting,
        Connected
    };
    Q_ENUM(ConnectionState)

    Q_PROPERTY(ConnectionState connectionState MEMBER m_connectionState NOTIFY connectionStateChanged FINAL)
    Q_PROPERTY(int selectedLibraryId MEMBER m_currentLibraryId NOTIFY currentLibraryIdChanged FINAL)
    Q_PROPERTY(QString selectedComicId MEMBER m_currentComicId NOTIFY currentComicIdChanged FINAL)
    Q_PROPERTY(bool selectedComicOpened MEMBER m_currentComicOpened NOTIFY currentComicOpenedChanged FINAL)
    Q_PROPERTY(QStandardItemModel * librariesModel MEMBER m_librariesModel CONSTANT FINAL)
    Q_PROPERTY(QStandardItemModel * comicsModel MEMBER m_comicsModel CONSTANT FINAL)

    AppController(QObject *parent = nullptr);

    Q_INVOKABLE void connectServer(QUrl serverBaseUrl);
    Q_INVOKABLE void updateLibraries();
    Q_INVOKABLE void updateComicsInFolder(int folderId = 1);
    Q_INVOKABLE void openComic();
    Q_INVOKABLE void closeComic();
    Q_INVOKABLE QString coverImageSource(QString comicHash);
    Q_INVOKABLE QModelIndex currentComicModelIndex();
    Q_INVOKABLE QString comicImageSource(int page);

signals:
    void connectionStateChanged(ConnectionState newState);
    void currentLibraryIdChanged(int newLibraryId);
    void currentComicIdChanged(QString newComicId);
    void currentComicOpenedChanged(bool opened);

private:
    QNetworkRequestFactory apiFactory() const;

    ConnectionState m_connectionState = NotConnected;
    int m_currentLibraryId = -1;
    QString m_currentComicId;
    bool m_currentComicOpened = false;
    QNetworkRequestFactory m_requestFactory;
    QNetworkAccessManager * m_networkAccessManager;
    QRestAccessManager * m_restAccessManager;
    QStandardItemModel * m_librariesModel;
    QStandardItemModel * m_comicsModel;
};
