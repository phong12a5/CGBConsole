import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: root
    property string iconSource: ""
    property alias imgIcon: icon
    property bool isPressed: false

    signal buttonClicked()

    Image {
        id: icon
        source: iconSource
        x: (parent.width - width)/2
        y: (parent.height - height)/2
        visible: false
    }

    PropertyAnimation{
        id: iconAni
        target: root
        property: "rotation"
        from: 0
        to: 180
        duration: 1000
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
            iconAni.start()
            buttonClicked()
        }
        onCanceled: isPressed = false
    }
}
