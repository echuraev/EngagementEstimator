import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

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

        /*Row {
            id: row

            Image {
                id: image
                width: 100
                height: 100
                source: "qrc:/qtquickplugin/images/template_image.png"
                fillMode: Image.PreserveAspectFit
            }
        }*/

        Row {
            id: row2

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
            id: row1

            Button {
                    //height: Style.height
                    //width: Style.widthMedium
                    //background: StyleRectangle { anchors.fill: parent }
                    //onClicked: root.capturesVisible = !root.capturesVisible
                    text: qsTr("Select area")
                    font.pixelSize: Style.fontSize
                    onClicked: {
                        var component = Qt.createComponent("ScreenAreaSelector.qml")
                        var window = component.createObject(this)
                        window.show()
                        mainWindow.visible = false
                    }
            }
        }

    }
}
