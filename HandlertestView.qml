import QtQuick 2.3
import QtQuick.Window 2.2

Window {
    id: root
    property string state: "home"

    visible: true
    width: 200
    height: 300

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (Executor.branch(true)) {
                console.log("true");
                root.state = "true";
            }
            else {
                console.log("false");
                root.state = "false";
            }
        }
    }
}

