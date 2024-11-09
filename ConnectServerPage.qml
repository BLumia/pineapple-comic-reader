import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import net.blumia.pineapple.comic.reader

Control {
    anchors.fill: parent
    padding: 5
    contentItem: ColumnLayout {
        Label {
            text: "Pineapple Comic Reader"
            font.pixelSize: 20
        }
        Item {
            Layout.fillHeight: true
            Layout.verticalStretchFactor: 2
        }
        Label {
            text: "YACReader Library Server URL:"
        }
        TextField {
            id: baseUrlEdit
            Layout.fillWidth: true
            enabled: AppController.connectionState === AppController.NotConnected
        }
        Item {
            Layout.fillHeight: true
            Layout.verticalStretchFactor: 1
        }
        Button {
            Layout.fillWidth: true
            text: AppController.connectionState === AppController.NotConnected ? "Connect" : "Connecting"
            enabled: baseUrlEdit.enabled
            onClicked: function() {
                AppController.connectServer(baseUrlEdit.text)
            }
        }
        Item {
            Layout.fillHeight: true
            Layout.verticalStretchFactor: 3
        }
    }
}
