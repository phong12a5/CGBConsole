import QtQuick 2.0

Rectangle {
    id: root
    color: "green"
    state: "invisible"
    width: parent.width
    height: parent.height

    onStateChanged: console.log("state: " + state)

    Component.onCompleted: {
        state = "visible"
    }

    states: [
        State {
            name: "invisible"
            PropertyChanges {
                target: root
                opacity: 0
            }
        },
        State {
            name: "visible"
            PropertyChanges {
                target: root
                opacity: 1
            }
        }
    ]

    transitions: Transition {
        from: "invisible"
        to: "visible"
        PropertyAnimation{
            target: root
            property: "opacity"
            duration: 4000
            from: 0
            to: 1
        }
    }
}
