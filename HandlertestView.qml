import QtQuick 2.3
import QtQuick.Window 2.2

/*
  (1)
  A -[]-> (A, [])
  queue = [(mousearea, [])]
  states = [A]

  (2)
  A -[mousearea]-> (B, [(mousearea, [true])])
                   (C, [(mousearea, [false])])
  queue = [ [(mousearea, [true])],
            [(mousearea, [false])]
          ]
  states = [A, B, C]

  (3)
  A -[mousearea]-> (B, [(mousearea, [true])])
  B -[mousearea]-> (B, [(mousearea, [true]), (mousearea, [true])])
                   (C, [(mousearea, [true]), (mousearea, [false])])
  queue = [ [(mousearea, [false])],
            [(mousearea, [true]), (mousearea, [false])]
          ]
  states = [A, B, C]

  (4)
  A -[mousearea]-> (C, [(mousearea, [false])])
  C -[mousearea]-> (B, [(mousearea, [false]), (mousearea, [true])])
                   (C, [(mousearea, [false]), (mousearea, [false])])
  queue = [ [(mousearea, [true]), (mousearea, [false])],
            [(mousearea, [false]), (mousearea, [true])],
            [(mousearea, [false]), (mousearea, [false])]
          ]
  states = [A, B, C]

  (5)
  A -[mousearea]-> (B, [(mousearea, [true])])
  B -[mousearea]-> (C, [(mousearea, [true]), (mousearea, [false])])
  queue = [ [(mousearea, [false]), (mousearea, [true])],
            [(mousearea, [false]), (mousearea, [false])]
          ]
  states = [A, B, C]

  (6)
  A -[mousearea]-> (C, [(mousearea, [false])])
  C -[mousearea]-> (B, [(mousearea, [false]), (mousearea, [true])])
  queue = [ [(mousearea, [false]), (mousearea, [false])]
          ]
  states = [A, B, C]

  (7)
  A -[mousearea]-> (C, [(mousearea, [false])])
  C -[mousearea]-> (C, [(mousearea, [false]), (mousearea, [false])])
  queue = []
  states = [A, B, C]
*/

Window {
    id: root
    property string state: "A"

    visible: true
    width: 200
    height: 300

    MouseArea {
        id: mousearea
        anchors.fill: parent
        onClicked: {
            if (Executor.branch(true)) {
                root.state = "B";
            }
            else {
                root.state = "C";
            }
        }
    }
}

