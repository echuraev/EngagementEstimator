import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs
import Qt.labs.platform

import ru.hse.involvementEstimator 1.0

Window {
    id: controlsWindow
    width: 200
    height: 30
    visible: true
    title: qsTr("Screen capture controls")
    //flags: Qt.Window | Qt.FramelessWindowHint | Qt.WA_TranslucentBackground | Qt.WindowStaysOnTopHint
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WA_TranslucentBackground
    x: (Screen.width - width) / 2
    y: Screen.height - height - 50

    MessageDialog {
        id: errorDialog;
        // TODO: add positioning of the error message
        //x: Screen.width / 2
        //y: Screen.height / 2
        title: "Error!";

        onAccepted: {
            errorDialog.close();
        }
    }

    InvolvementEstimator {
        id: involvementEstimator
        onError: function(msg) {
            timer.stop()
            mainWindow.visible = true
            screenAreaSelectorWindows.close()
            controlsWindow.close();
            errorDialog.text = msg
            errorDialog.open()
        }
    }

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
