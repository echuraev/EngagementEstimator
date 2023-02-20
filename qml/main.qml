import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import Qt.labs.platform

Window {
    id: mainWindow
    width: 300
    height: 100
    visible: true
    maximumHeight: height
    maximumWidth: width
    minimumHeight: height
    minimumWidth: width
    title: qsTr("Engagement Estimator")

    Column {
        id: column
        anchors.fill: parent

        Row {
            TextField {
                id: outputPath
                placeholderText: qsTr("Select output directory...")
                readOnly: true
                width: 230
            }
            Button {
                width: 70
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
            CheckBox {
                text: qsTr("Enter path manually")
                ToolTip.visible: hovered
                ToolTip.text: qsTr("It is a bug in the latest MacOS. Dialog window is crashed. This option can be used to specify output path manually.")
                onCheckedChanged: outputPath.readOnly = !checked
            }
        }

        Row {
            CheckBox {
                id: debugMod
                text: qsTr("Use debug mode")
                checked: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("For now it is just a placeholer and compilation flag is used. Bun in the future it should be fixed.")
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
                    if (DEBUG_MOD) {
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
