import QtQuick 6.5
import QtQuick.Window 6.5
import QtQuick.Controls 6.5
import Qt.labs.platform

Window {
    id: mainWindow
    width: Style.mainWindowWidth
    height: Style.mainWindowHeight
    visible: true
    maximumHeight: height
    maximumWidth: width
    minimumHeight: height
    minimumWidth: width
    title: qsTr("Engagement Estimator")

    Column {
        id: column
        anchors.fill: parent

        Label {
            text: qsTr("Select output directory:")
            font.pixelSize: Style.fontSize
            color: Style.textColor
            padding: Style.blankSpace
            width: parent.width
        }

        Row {
            width: parent.width

            Label {
                width: Style.blankSpace
            }
            TextField {
                id: outputPath
                text: folderDialog.folder
                readOnly: true
                width: parent.width - (Style.buttonWidth + 3 * Style.blankSpace)
                height: Style.elementHeight
                font.pixelSize: Style.fontSize
                color: Style.textColor
            }
            Label {
                width: Style.blankSpace
            }
            Button {
                id: browseButton
                text: qsTr("Browse")
                width: Style.buttonWidth
                height: Style.elementHeight
                font.pixelSize: Style.fontSize
                // TODO: Enable color for button text
                //color: Style.textColor
                onClicked: folderDialog.open()
                anchors.verticalCenter: outputPath.verticalCenter
            }
            Label {
                width: Style.blankSpace
            }

            FolderDialog {
                id: folderDialog
                onAccepted: outputPath.text = folderDialog.folder
            }
        }

        Row {
            width: parent.width

            Label {
                width: Style.blankSpace
            }
            CheckBox {
                id: debugMod
                text: qsTr("Use debug mode")
                font.pixelSize: Style.fontSize
                // TODO: Enable color for button text
                //color: Style.textColor
                checked: true
                onCheckedChanged: Config.debugMod = checked
            }
        }

        Button {
            //height: Style.height
            //width: Style.widthMedium
            //background: StyleRectangle { anchors.fill: parent }
            //onClicked: root.capturesVisible = !root.capturesVisible
            text: qsTr("Select area")
            width: Style.buttonWidth
            height: Style.elementHeight
            font.pixelSize: Style.fontSize
            onClicked: {
                if (Config.debugMod) {
                    console.log("WARNING!!!")
                    console.log("You run application in debug mode")
                    console.log("WARNING!!!")
                }
                var component = Qt.createComponent("ScreenAreaSelector.qml")
                var window = component.createObject(this)
                window.show()
                mainWindow.visible = false
            }
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
