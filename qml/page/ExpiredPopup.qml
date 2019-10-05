import QtQuick 2.0

Rectangle {
    id: root
    color: "black"
    opacity: 0.8
    anchors.fill: parent
    Text {
        text: qsTr("Hết tiền rồi --- Nộp tiền đi ^^")
        color: "white"
        anchors.centerIn: parent
        font.pixelSize: 15
        horizontalAlignment: Text.AlignHCenter
    }
}
