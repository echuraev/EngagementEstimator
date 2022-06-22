import QtQuick 2.15
import QtQuick.Controls 2.15

import screenCapture 1.0

// TODO: it should be on top of ScreenAreaSelector
Window {
    id: controlsWindow
    width: 400
    height: 50
    visible: true
    title: qsTr("Screen capture controls")
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WA_TranslucentBackground | Qt.WindowStaysOnTopHint
    x: Screen.width / 2 - width / 2
    y: Screen.height - height - 50

    Row {
        id: row
        anchors.fill: parent

        Button {
            //height: Style.height
            //width: Style.widthMedium
            //background: StyleRectangle { anchors.fill: parent }
            //onClicked: root.capturesVisible = !root.capturesVisible
            text: qsTr("Start capturing")
            font.pixelSize: Style.fontSize
            onClicked: {
                screenAreaSelectorWindows.visible = false
                screenCapture.capture(screenAreaSelectorWindows.x, screenAreaSelectorWindows.y,
                                      screenAreaSelectorWindows.width, screenAreaSelectorWindows.height,
                                      frameRate.value);
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
                console.log("The windoses should be cloased and main window shoudl be displayed");
                mainWindow.visible = true
                screenAreaSelectorWindows.close()
                controlsWindow.close()
            }
        }
    }
    //color: "#d390909d"
}
