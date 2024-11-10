import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import net.blumia.pineapple.comic.reader

Item {
    anchors.fill: parent

    Drawer {
        id: drawer
        width: Math.min(0.66 * parent.width, 320)
        height: parent.height

        ListView {
            id: listView
            clip: true
            anchors.fill: parent
            model: AppController.foldersModel
            delegate: ItemDelegate {
                text: model.display
                width: listView.width
                onClicked: function() {
                    AppController.updateComicsInFolder(model.folderId)
                }
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            ToolButton {
                flat: true
                icon.name: "folder-open"
                onClicked: function() {
                    drawer.open();
                }
            }
            Label {
                text: "Comics"
                font.pixelSize: 20
            }
        }
        GridView {
            id: gridView
            clip: true
            cellHeight: cellWidth * 3 / 2
            cellWidth: width / Math.max(Math.floor(width / (65 * 2)), 1)
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: AppController.comicsModel
            delegate: Button {
                width: GridView.view.cellWidth
                height: GridView.view.cellHeight
                Column {
                    Image {
                        source: AppController.coverImageSource(model.hash)
                        width: gridView.cellWidth
                        height: gridView.cellHeight
                        fillMode: Image.PreserveAspectFit
                        retainWhileLoading: true
                    }
                }

                onClicked: function() {
                    AppController.selectedComicId = model.comicId
                }
            }
            ScrollBar.vertical: ScrollBar { }
        }

        Component.onCompleted: function() {
            AppController.updateComicsInFolder()
        }
    }
}
