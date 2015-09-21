import QtQuick 2.0

Rectangle {
    width: 300
    height: 300
    color: "red"

    Rectangle {
        objectName: "unclickable"
        x: 0
        y: 0
        width: 50
        height: 50
        color: "gray"
        Rectangle {
            objectName: "unclickable_child"

            MouseArea {
                objectName: "hidden_1"
                anchors.fill: parent
                onClicked: console.log("hidden1")
            }
        }

        MouseArea {
            objectName: "hidden_2"
            x: 0
            y: 0
            width: 10
            height: 10
            onClicked: console.log("hidden2")
        }
    }

    Rectangle {
        objectName: "clickable_1"
        x: 0
        y: 0
        width: 300
        height: 100
        color: "blue"
        Rectangle {
            objectName: "clickable_1_1"
            anchors.fill: parent
            color: "green"
        }
        MouseArea {
            objectName: "ma_1"
            anchors.fill: parent
            onClicked: console.log("ma_1")

            MouseArea {
                objectName: "ma_1_1"
                anchors.fill: parent
                onClicked: console.log("ma_1_1")
            }
        }
    }

    Rectangle {
        objectName: "clickable_2"
        x: 100
        y: 50
        width: 200
        height: 100
        color: "yellow"

        Rectangle {
            objectName: "clickable_2_1"
            x: 10
            y: 0
            width: 10
            height: 10
            color: "orange"

            MouseArea {
                objectName: "ma_2_1_1"
                anchors.fill: parent
                onClicked: console.log("ma_2_1_1")
            }
        }
    }

}

