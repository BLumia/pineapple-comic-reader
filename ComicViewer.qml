import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import net.blumia.pineapple.comic.reader

Pane {
    id: root
    required property int pageCount

    property bool osdVisible: false

    anchors.fill: parent

    SwipeView {
        id: view
        anchors.fill: parent

        onVisibleChanged: function() {
            if (visible) {
                forceActiveFocus()
            }
        }

        Repeater {
            model: root.pageCount
            Loader {
                active: root.visible && (SwipeView.isCurrentItem || SwipeView.isNextItem || SwipeView.isPreviousItem)
                sourceComponent: Image {
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    source: AppController.comicImageSource(index)
                    onStatusChanged: function() {
                        if (status == Image.Error) {
                            reloadTimer.start()
                        }
                    }
                    Timer {
                        id: reloadTimer
                        interval: 500
                        onTriggered: {
                            console.log("reload")
                            let origSrc = parent.source
                            parent.source = ""
                            parent.source = origSrc
                        }
                    }
                }
            }
        }
    }

    Pane {
        visible: root.osdVisible
        anchors.fill: parent
        opacity: 0.65
    }

    ColumnLayout {
        visible: root.osdVisible
        enabled: root.osdVisible
        anchors.fill: parent
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            TapHandler {
                onTapped: function() {
                    root.osdVisible = !root.osdVisible
                }
            }
        }
        Button {
            text: "Close Comic"
            Layout.fillWidth: true
            onClicked: function() {
                root.osdVisible = false
                AppController.closeComic()
            }
        }
    }

    PageIndicator {
        id: indicator

        visible: !root.osdVisible
        count: view.count
        currentIndex: view.currentIndex

        anchors.top: view.top
        anchors.horizontalCenter: parent.horizontalCenter

        TapHandler {
            onTapped: function() {
                root.osdVisible = !root.osdVisible
            }
        }
    }

    Keys.onEscapePressed: function(event) {
        root.osdVisible = !root.osdVisible
    }
}
