import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import net.blumia.pineapple.comic.reader
import net.blumia.pineapple.comic.reader.comicitem

Control {
    id: root
    anchors.fill: parent
    padding: 5

    property var curComicIndex: AppController.currentComicModelIndex()
    property bool startReading: AppController.selectedComicOpened
    readonly property int comicPageCount: dataByRole(ComicItem.PageCountRole)

    function dataByRole(role) {
        return AppController.comicsModel.data(curComicIndex, role)
    }

    contentItem: ColumnLayout {
        enabled: !root.startReading
        Image {
            Layout.fillWidth: true
            Layout.maximumHeight: Math.min(root.width / 2, root.height / 2)
            fillMode: Image.PreserveAspectFit
            source: AppController.coverImageSource(dataByRole(ComicItem.HashRole))
            // sourceSize.width: root.width / 3
            // sourceSize.height: root.width / 2
        }
        Label {
            Layout.preferredWidth: root.width - 10
            text: dataByRole(Qt.DisplayRole)
            wrapMode: Text.Wrap
        }
        Label {
            Layout.preferredWidth: root.width - 10
            text: "Added: " + new Date(Number(`${dataByRole(ComicItem.AddedTimeRole)}000`)).toLocaleString(Qt.locale())
            wrapMode: Text.Wrap
        }
        Label {
            Layout.preferredWidth: root.width - 10
            text: `${comicPageCount} pages`
            wrapMode: Text.Wrap
        }
        Label {
            Layout.preferredWidth: root.width - 10
            text: `Last read at ${dataByRole(ComicItem.CurrentPageRole)} page`
            wrapMode: Text.Wrap
        }
        Button {
            Layout.fillWidth: true
            text: "Read"
            enabled: root.comicPageCount > 0
            onClicked: function() {
                AppController.openComic()
            }
        }
        Button {
            Layout.fillWidth: true
            text: "Back"
            onClicked: function() {
                AppController.selectedComicId = ""
            }
        }
        Item {
            Layout.fillHeight: true
        }
    }

    ComicViewer {
        parent: Overlay.overlay
        visible: root.startReading
        pageCount: dataByRole(ComicItem.PageCountRole)
    }
}
