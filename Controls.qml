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
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WA_TranslucentBackground | Qt.WindowStaysOnTopHint
    //flags: Qt.Window | Qt.FramelessWindowHint | Qt.WA_TranslucentBackground
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
        onResultDebug: function(label, x1, y1, x2, y2) {
            console.log("onResultDebug: l: ", label, ", x1: ", x1)
            screenAreaSelectorWindowsCanvas.requestPaint()
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
                if (Style.debugMod) {
                    // TODO: also hide control window
                    screenAreaSelectorWindows.visible = false
                }
                // TODO: run `run` in separate thread
                involvementEstimator.run(screenAreaSelectorWindows.x, screenAreaSelectorWindows.y,
                                         screenAreaSelectorWindows.width, screenAreaSelectorWindows.height);
                if (Style.debugMod) {
                    screenAreaSelectorWindows.visible = true
                }
            }
         }

        Button {
            //height: Style.height
            //width: Style.widthMedium
            //background: StyleRectangle { anchors.fill: parent }
            text: qsTr("Start capturing")
            font.pixelSize: Style.fontSize
            onClicked: {
                // TODO: or Acync run is running
                if (timer.running) {
                    screenAreaSelectorWindows.visible = true
                    if (autoFrameRate.checked !== true) {
                        timer.stop()
                    } else {
                        console.log("Async run should be stopped")
                    }
                    if (!Style.debugMod) {
                        screenAreaSelectorWindows.visible = true
                    }
                    text = qsTr("Start capturing")
                } else {
                    screenAreaSelectorWindows.visible = false
                    if (!Style.debugMod) {
                        screenAreaSelectorWindows.visible = false
                    }
                    text = qsTr("Stop capturing")
                    timer.start()
                    if (autoFrameRate.checked !== true) {
                        timer.start()
                    } else {
                        console.log("Async run should be started")
                    }
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
