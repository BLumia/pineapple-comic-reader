import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    anchors.fill: parent
    Label {
        text: "Comics"
        font.pixelSize: 20
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
