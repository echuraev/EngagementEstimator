import QtQuick 2.0
import QtQuick.Window 2.15
import ru.hse.debugInfoDrawer 1.0

Window {
    id: screenAreaSelectorWindows
    width: Style.screenAreaWidth
    height: Style.screenAreaHeight
    visible: true
    title: qsTr("Select area")
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WA_TranslucentBackground | Qt.WindowStaysOnTopHint
    color: Style.screenAreaColor

    // Declare properties that will store the position of the mouse cursor
    property int previousX
    property int previousY
    property var controlWindow
    readonly property int moveZoneSize: Style.screenAreaMoveZoneSize
    // The following properties are used in Controls.qml
    readonly property int minWidth: Style.screenAreaMinWidth
    readonly property int minHeight: Style.screenAreaMinHeight
    readonly property int maxWidth: Style.screenAreaMaxWidth
    readonly property int maxHeight: Style.screenAreaMaxHeight
    property bool movable: true

    Component.onCompleted: {
        x = Screen.width / 2 - width / 2
        y = Screen.height / 2 - height / 2
        var component = Qt.createComponent("Controls.qml")
        controlWindow = component.createObject(this)
        controlWindow.show()
    }

    Canvas {
        id: border
        anchors.fill: parent
        onPaint: {
            var ctx = getContext("2d");
            ctx.lineWidth = 4;
            ctx.setLineDash([3]);
            ctx.strokeStyle = Style.screenAreaFrameColor

            ctx.beginPath();
            ctx.moveTo(0, 0);
            ctx.lineTo(screenAreaSelectorWindows.width, 0);
            ctx.lineTo(screenAreaSelectorWindows.width, screenAreaSelectorWindows.height);
            ctx.lineTo(0, screenAreaSelectorWindows.height);
            ctx.lineTo(0, 0);
            ctx.stroke();
         }

        DebugInfoDrawer {
            id: debugInfoDrawer
            anchors.fill: parent
        }
    }

    MouseArea {
        id: topLeftCorner
        height: moveZoneSize
        width: moveZoneSize
        anchors {
            top: parent.top
            left: parent.left
        }
        // We set the shape of the cursor so that it is clear that this resizing
        cursorShape: Qt.SizeFDiagCursor
    }

    MouseArea {
        id: topRightCorner
        height: moveZoneSize
        width: moveZoneSize
        anchors {
            top: parent.top
            right: parent.right
        }
        // We set the shape of the cursor so that it is clear that this resizing
        cursorShape: Qt.SizeBDiagCursor
    }

    MouseArea {
        id: bottomRightCorner
        height: moveZoneSize
        width: moveZoneSize
        anchors {
            bottom: parent.bottom
            right: parent.right
        }
        // We set the shape of the cursor so that it is clear that this resizing
        cursorShape: Qt.SizeFDiagCursor
    }

    MouseArea {
        id: bottomLeftCorner
        height: moveZoneSize
        width: moveZoneSize
        anchors {
            bottom: parent.bottom
            left: parent.left
        }
        // We set the shape of the cursor so that it is clear that this resizing
        cursorShape: Qt.SizeBDiagCursor
    }

    MouseArea {
        id: topArea
        height: moveZoneSize
        anchors {
            top: parent.top
            left: topLeftCorner.right
            right: topRightCorner.left
        }
        // We set the shape of the cursor so that it is clear that this resizing
        cursorShape: Qt.SizeVerCursor
    }

    // Similar calculations for the remaining three areas of resize
    MouseArea {
        id: bottomArea
        height: moveZoneSize
        anchors {
            bottom: parent.bottom
            left: bottomLeftCorner.right
            right: bottomRightCorner.left
        }
        cursorShape: Qt.SizeVerCursor
    }

    MouseArea {
        id: leftArea
        width: moveZoneSize
        anchors {
            top: topLeftCorner.bottom
            bottom: bottomLeftCorner.top
            left: parent.left
        }
        cursorShape: Qt.SizeHorCursor
    }

    MouseArea {
        id: rightArea
        width: moveZoneSize
        anchors {
            top: topRightCorner.bottom
            bottom: bottomRightCorner.top
            right: parent.right
        }
        cursorShape:  Qt.SizeHorCursor
    }

    // The central area for moving the application window
    // Here you already need to use the position both along the X axis and the Y axis
    MouseArea {
        anchors {
            top: topArea.bottom
            bottom: bottomArea.top
            left: leftArea.right
            right: rightArea.left
        }
        cursorShape:  Qt.SizeAllCursor

        onPressed: {
            previousX = mouseX
            previousY = mouseY
        }

        onMouseXChanged: {
            if (screenAreaSelectorWindows.movable) {
                var dx = mouseX - previousX
                var new_x = Math.max(screenAreaSelectorWindows.x + dx, 0)
                // TODO: do the same think for right border and for changing sizes
                screenAreaSelectorWindows.setX(new_x)
            }
        }

        onMouseYChanged: {
            if (screenAreaSelectorWindows.movable) {
                var dy = mouseY - previousY
                var new_y = Math.max(screenAreaSelectorWindows.y + dy, 0)
                screenAreaSelectorWindows.setY(new_y)
            }
        }

        onReleased: {
            // Hack for creating controlWindow always on top
            controlWindow.visible = false
            controlWindow.visible = true
        }
    }
}
