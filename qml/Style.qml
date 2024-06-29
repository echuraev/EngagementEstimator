pragma Singleton
import QtQuick 2.0

QtObject {
    // function isMobile() {
    //     return Qt.platform.os === "android" || Qt.platform.os === "ios";
    // }

    // property real screenWidth: isMobile() ? 400 : 800
    // property real screenHeigth: isMobile() ? 900 : 600

    // Common properties
    readonly property int fontSize: 13
    readonly property int blankSpace: 5
    readonly property int buttonWidth: 100
    // TODO: How to set height equals to the space taken by the components
    readonly property int elementHeight: 30
    readonly property color textColor: "black"

    // Main window
    readonly property int mainWindowWidth: 400
    readonly property int mainWindowHeight: 110

    // Screen area selector
    readonly property color screenAreaFrameColor: "red"
    // TODO: Find better color
    //color: "#d390909d"
    readonly property color screenAreaColor: "#00000000"
    readonly property int screenAreaWidth: 1024
    readonly property int screenAreaHeight: 768
    // TODO: enable next 4 parameters
    readonly property int screenAreaMinWidth: 200
    readonly property int screenAreaMinHeight: 200
    readonly property int screenAreaMaxWidth: Screen.width
    readonly property int screenAreaMaxHeight: Screen.height
    readonly property int screenAreaMoveZoneSize: 5

    // Controls window
    readonly property int controlsWindowWidth: 259
    readonly property int controlsWindowHeight: 30
    readonly property int controlsWindowButtonWidth: 120
    readonly property int controlWindowPositionX: (Screen.width - controlsWindowWidth) / 2
    readonly property int controlWindowPositionY: Screen.height - controlsWindowHeight - 50


    // Debug information drawer
    readonly property int debugFontSize: 13
    readonly property color debugTextColor: "yellow"
    readonly property color engagedFrameColor: "green"
    readonly property color distractedFrameColor: "red"
}
