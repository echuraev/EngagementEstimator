import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import screenCapture 1.0

Window {
    id: mainWindow
    width: 400
    height: 400
    visible: true
    // TODO: size should be fixed
    flags: Qt.Window
    property int index: 0
    title: qsTr("Involvement Estimator")

    /*function shoot() {
        screenCapture.capture('sample'+index+'.png')
        if(index == 4) {
            delay.stop()
            Qt.quit()
        }
        index += 1
    }

    Timer {
        id: delay
        interval: 10000/4 // 5 frames, each 96x96 px
        repeat: true
        triggeredOnStart: true
        onTriggered: shoot()
    }*/

    //Connections {
    //    target: screenCapture // Specify the target to connect
    //    /* Declare and implement the function as a parameter
    //     * object and with a name similar to the name of the signal
    //     * The difference is that we add on at the beginning and then write
    //     * capitalized
    //     * */
    //    onDisplayScreenshot: {
    //        console.log("uePosDatabase.isConnected()");
    //        image.source = "file:/home/echuraev/Workspace/HSE/build-InvolvementEstimator-Desktop_Qt_6_2_4_GCC_64bit-Debug/tmp.jpg";
    //        //labelCount.text = count // Set the counter to the text label
    //    }
    //}

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
                    //onClicked: screenCapture.capture()
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
