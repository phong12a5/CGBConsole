import QtQuick 2.0
import QtQuick.Controls 2.0

ComboBox {
    id: root
    width: 200
    height: 30
    signal clickUp()
    signal clickDown()

    delegate: ItemDelegate {
        width: root.width
        contentItem: Text {
            text: modelData
            color: root.highlightedIndex === index? "#000000" : "#21be2b"
            font: root.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }
        highlighted: root.highlightedIndex === index
    }

    indicator: Item {
        id: indicatorItem
        height: root.height
        width: height
        anchors.right: root.right
        MouseArea{
            anchors.fill: parent
        }

        Image {
            id: upBtn
            width: parent.height/2
            height: width
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.verticalCenter
            source: "qrc:/image/upIcon.png"
            opacity: !enabled? 0.4 : mouseAreaUp.pressed? 0.5 : 1
            enabled: root.currentIndex < root.count - 1
            MouseArea{
                id: mouseAreaUp
                anchors.fill: parent
                onReleased: {
                    clickUp()
                }
            }
        }
        Image {
            id: downBtn
            width: parent.height/2
            height: width
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.verticalCenter
            source: "qrc:/image/downIcon.png"
            opacity: !enabled? 0.4 : mouseAreaDown.pressed? 0.5 : 1
            enabled: root.currentIndex > 0
            MouseArea{
                id: mouseAreaDown
                anchors.fill: parent
                onReleased: {
                    clickDown()
                }
            }
        }
    }
}
