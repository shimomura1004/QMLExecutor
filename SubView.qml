import QtQuick 2.0

Rectangle {
    color: "green"
    width: 300
    height: 300

    MouseArea {
        id: subma
        anchors.fill: parent
        visible: parent.visible
        onClicked: {
            root.substate = false;
            parent.visible = false
        }
    }
}

