import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import Qt.labs.platform

Window {
    id: mainWindow
    width: 400
    height: 400
    visible: true
    // TODO: size should be fixed
    flags: Qt.Window
    property int index: 0
    title: qsTr("Involvement Estimator")

    Column {
        id: column
        anchors.fill: parent

        Row {
            Text {
                text: qsTr("Frame rate: ")
                font.pixelSize: Style.fontSize
            }

            SpinBox {
                id: frameRate
                from: 3
                to: 30
                value: 5
            }

            Text {
                text: qsTr("fps")
                font.pixelSize: Style.fontSize
            }
        }

        Row {
            CheckBox {
                id: autoFrameRate
                text: qsTr("Detect frame rate automatically")
                checkable: true
                onToggled: function() {
                    if (autoFrameRate.checked == true) {
                        frameRate.enabled = false
                    } else {
                        frameRate.enabled = true
                    }
                }
            }
        }

        Row {
            TextField {
                id: outputPath
                placeholderText: qsTr("Select output directory...")
                readOnly: true
                //width: parent.width * 0.8
            }
            Button {
                //width: parent.width * 0.2
                //height: Style.height
                //width: Style.widthMedium
                //background: StyleRectangle { anchors.fill: parent }
                //onClicked: root.capturesVisible = !root.capturesVisible
                text: qsTr("Browse")
                font.pixelSize: Style.fontSize
                onClicked: folderDialog.open()
            }

            FolderDialog {
                id: folderDialog
                onAccepted: outputPath.text = folderDialog.folder
            }
        }

        Row {
            id: row1

            Button {
                //height: Style.height
                //width: Style.widthMedium
                //background: StyleRectangle { anchors.fill: parent }
                //onClicked: root.capturesVisible = !root.capturesVisible
                text: qsTr("Select area")
                font.pixelSize: Style.fontSize
                onClicked: {
                    if (Style.debugMod) {
                        console.log("WARNING!!!")
                        console.log("You run application in debug mode")
                        console.log("WARNING!!!")
                    }
                    var component = Qt.createComponent("ScreenAreaSelector.qml")
                    var window = component.createObject(this)
                    window.show()
                    mainWindow.visible = false
                }
            }
        }
    }
}
