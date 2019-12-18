import QtQuick 2.0

Item {
    Image {
        id: backGround
        source: "qrc:/image/background.jpg"
        opacity: 0.8
        scale: 0.8
        height: parent.height
        width: height
        anchors.right: parent.horizontalCenter
        smooth: true
        antialiasing: true
    }

    Text {
        anchors.left: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        text: "Viber/WhatsApp/Zalo \n+84394386880"
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 20
        color: "blue"
    }
}
