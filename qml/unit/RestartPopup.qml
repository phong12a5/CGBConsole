import QtQuick 2.9
import QtQuick.Controls 2.0

Rectangle {
    id: root
    anchors.fill: parent
    color: "black"
    opacity: 0.8

    Text {
        id: guide
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 15
        color: "white"
        text: qsTr("New version is just updated.\nDo you want to restart this program right now?")
        lineHeightMode: Text.FixedHeight
        lineHeight: 20
        horizontalAlignment: Text.AlignHCenter
        anchors.bottom: closeBtn.top
        anchors.bottomMargin: 20
    }

    Rectangle{
        id: closeBtn
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.horizontalCenter
        anchors.rightMargin: 5
        width: 100
        height: 50
        color: "white"
        radius: 5
        Text {
            text: qsTr("Close")
            font.pixelSize: 15
            anchors.centerIn: parent
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                AppModel.isShowRestartPopup = false
            }
        }
    }

    Rectangle{
        id: restartBtn
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.horizontalCenter
        anchors.leftMargin: 5
        width: 100
        height: 50
        color: "white"
        radius: 5
        Text {
            text: qsTr("Restart")
            font.pixelSize: 15
            anchors.centerIn: parent
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                AppMain.restartApplication()
            }
        }
    }
}
