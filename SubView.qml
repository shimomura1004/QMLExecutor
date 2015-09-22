import QtQuick 2.0

Rectangle {
    objectName: "subrect"
    color: "green"
    width: 300
    height: 300

    MouseArea {
        id: subma
        objectName: "subma"
        anchors.fill: parent
        onClicked: {
            root.substate = false;
            parent.visible = false
        }
    }
}

