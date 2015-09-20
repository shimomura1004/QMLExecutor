import QtQuick 2.3
import QtQuick.Window 2.2

Window {
    id: root
    property string state: "home"
    property bool substate: true

    visible: true
    width: 200
    height: 300

    Column {
        Rectangle {
            width: 200
            height: 100
            color: "red"

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Button1"
            }

            MouseArea {
                id: ma1
                anchors.fill: parent
                visible: !popup1.visible && !popup2.visible
                onClicked: {
                    root.state = "popup1";
                    popup1.visible = true;
                }
            }
        }

        Rectangle {
            width: 200
            height: 100
            color: "blue"

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Button2"
            }

            MouseArea {
                id: ma2
                anchors.fill: parent
                visible: !popup1.visible && !popup2.visible
                onClicked: {
                    root.state = "popup2";
                    popup2.visible = true;
                }
            }
        }

        Loader {
            width: parent.width
            height: 100
            source: "SubView.qml"
        }
    }

    Rectangle {
        id: popup1
        visible: false
        anchors.fill: parent
        color: "gray"

        Rectangle {
            width: 100
            height: 50
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            color: "orange"

            MouseArea {
                id: ma1_back
                anchors.fill: parent
                visible: popup1.visible
                onClicked: {
                    root.state = "home";
                    popup1.visible = false;
                }
            }
        }
    }

    Rectangle {
        id: popup2
        visible: false
        anchors.fill: parent
        color: "yellow"

        Rectangle {
            width: 100
            height: 50
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            color: "purple"

            MouseArea {
                id: ma2_back
                anchors.fill: parent
                visible: popup2.visible
                onClicked: {
                    root.state = "home";
                    popup2.visible = false;
                }
            }
        }
    }
}

