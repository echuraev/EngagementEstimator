import QtQuick 2.15
import QtQuick.Controls 2.15

import involvementEstimator 1.0

Window {
    id: controlsWindow
    width: 400
    height: 50
    visible: true
    title: qsTr("Screen capture controls")
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WA_TranslucentBackground | Qt.WindowStaysOnTopHint
    x: (Screen.width - width) / 2
    y: Screen.height - height - 50


    Row {
        id: row
        anchors.fill: parent

        Timer {
            id: timer
            interval: 1000 / frameRate.value;
            repeat: true
            onTriggered: {
                involvementEstimator.run(screenAreaSelectorWindows.x, screenAreaSelectorWindows.y,
                                         screenAreaSelectorWindows.width, screenAreaSelectorWindows.height);
            }
         }

        Button {
            //height: Style.height
            //width: Style.widthMedium
            //background: StyleRectangle { anchors.fill: parent }
            text: qsTr("Start capturing")
            font.pixelSize: Style.fontSize
            onClicked: {
                if (timer.running) {
                    screenAreaSelectorWindows.visible = true
                    text = qsTr("Start capturing")
                    timer.stop()
                } else {
                    screenAreaSelectorWindows.visible = false
                    text = qsTr("Stop capturing")
                    timer.start()
                }
            }
        }

        Button {
            //height: Style.height
            //width: Style.widthMedium
            //background: StyleRectangle { anchors.fill: parent }
            //onClicked: root.capturesVisible = !root.capturesVisible
            text: qsTr("Close")
            font.pixelSize: Style.fontSize
            onClicked: {
                timer.stop()
                mainWindow.visible = true
                screenAreaSelectorWindows.close()
                controlsWindow.close()
            }
        }
    }
}
