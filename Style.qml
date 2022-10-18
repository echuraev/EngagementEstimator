pragma Singleton
import QtQuick 2.0

QtObject {
    function isMobile() {
        return Qt.platform.os === "android" || Qt.platform.os === "ios";
    }

    property real screenWidth: isMobile() ? 400 : 800
    property real screenHeigth: isMobile() ? 900 : 600
    property int fontSize: 12
    property bool debugMod: false
}
