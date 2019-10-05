import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle {
    id: root
    property string iconSource: ""
    property string label: ""
    property alias imgIcon: icon
    property bool isPressed: false

    signal buttonClicked()

    color: "transparent"
    border.width: 1
    border.color: "grey"

    Text {
        id: name
        text: label
        font.pixelSize: 15
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.verticalCenter: parent.verticalCenter
        color: parent.isPressed? "blue" : "black"
    }

    Image {
        id: icon
        source: iconSource
        visible: false
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 5
    }

    ColorOverlay{
        anchors.fill: icon
        color: parent.isPressed? "blue" : "black"
        source: icon
    }


    MouseArea{
        id: mouseArea
        anchors.fill: parent
        onPressed: isPressed = true
        onPressAndHold: isPressed = true
        onReleased: {
            isPressed = false
            buttonClicked()
        }
        onCanceled: isPressed = false
    }
}
