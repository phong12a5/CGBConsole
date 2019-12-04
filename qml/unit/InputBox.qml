import QtQuick 2.0
import QtQuick.Controls 2.0

TextField {
    id: root
    width: 200
    height: 30
    text: value
    font.pixelSize: 15
    placeholderText: ""


    property int value: 0
    property int minVal: 0
    property int maxVal: 10

    signal releasedDown()
    signal releasedUp()

    Image {
        id: upBtn
        width: parent.height/2
        height: width
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.bottom: parent.verticalCenter
        source: "qrc:/image/upIcon.png"
        opacity: !enabled? 0.4 : mouseAreaUp.pressed? 0.5 : 1
        enabled: value < maxVal
        MouseArea{
            id: mouseAreaUp
            onReleased: {
                releasedUp()
            }
            anchors.fill: parent
        }
    }
    Image {
        id: downBtn
        width: parent.height/2
        height: width
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.top: parent.verticalCenter
        source: "qrc:/image/downIcon.png"
        opacity: !enabled? 0.4 : mouseAreaDown.pressed? 0.5 : 1
        enabled: value > minVal
        MouseArea{
            id: mouseAreaDown
            onReleased: {
                releasedDown()
            }
            anchors.fill: parent
        }
    }
}
