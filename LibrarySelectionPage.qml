import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import net.blumia.pineapple.comic.reader
import net.blumia.pineapple.comic.reader.libraryitem

Control {
    anchors.fill: parent
    padding: 5
    contentItem: ColumnLayout {
        Label {
            text: "Libraries"
            font.pixelSize: 20
        }
        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: DelegateModel {
                model: AppController.librariesModel
                delegate: ItemDelegate {
                    text: model.display
                    width: ListView.view.width
                    onClicked: function() {
                        AppController.selectedLibraryId = model.libraryId
                    }
                }
                onCountChanged: function() {
                    if (count == 1) {
                        AppController.selectedLibraryId = model.data(modelIndex(0), LibraryItem.IdRole)
                    }
                }
            }
        }

        Component.onCompleted: function() {
            AppController.updateLibraries()
        }
    }
}
