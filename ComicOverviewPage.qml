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

    function dataByRole(role) {
        return AppController.comicsModel.data(curComicIndex, role)
    }

    contentItem: ColumnLayout {
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
            text: `${dataByRole(ComicItem.PageCountRole)} pages`
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
        z: 2
        visible: root.startReading
        pageCount: dataByRole(ComicItem.PageCountRole)
    }
}
