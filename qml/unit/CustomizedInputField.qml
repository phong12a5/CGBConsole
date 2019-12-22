import QtQuick 2.0
import QtQuick.Controls 2.10

Item {

    id: root
    width: 200
    height: 30
    property int maxValue: 10
    property int minValue: 0
    property int valStep: 1
    property int value: 0

    TextField{
        id: textField
        height: parent.height
        width: parent.width - indicatorArea.width + 1
        text: value
        font.pixelSize: 17
        onTextChanged: {
            if(isNaN(parseInt(text,10))){
                value = 0
            }else{
                var tmpValue = parseInt(text,10)
                if(tmpValue > maxValue){
                    tmpValue = maxValue
                }else if(tmpValue < minValue){
                    tmpValue = minValue
                }
                value = tmpValue
            }
        }
    }

    Rectangle{
        id: indicatorArea
        height: parent.height
        width: height
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        color: "#e0e0e0"
        Image {
            id: upBtn
            height: parent.height/2
            width: height
            anchors.bottom: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            source: "qrc:/image/upIcon.png"
            opacity: !enabled? 0.4 : mouseAreaUp.pressed? 0.5 : 1
            enabled: value < maxValue
            MouseArea{
                id: mouseAreaUp
                anchors.fill: parent
                onReleased: {
                    value += valStep
                    if(value > maxValue){
                        value = maxValue
                    }
                }
            }
        }
        Image {
            id: downBtn
            height: parent.height/2
            width: height
            anchors.top: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            source: "qrc:/image/downIcon.png"
            opacity: !enabled? 0.4 : mouseAreaDown.pressed? 0.5 : 1
            enabled: value > minValue
            MouseArea{
                id: mouseAreaDown
                anchors.fill: parent
                onReleased: {
                    value -= valStep
                    if(value < minValue){
                        value = minValue
                    }
                }
            }
        }
    }

    onValueChanged: {
        if(String(value) !== textField.text){
            textField.text = String(value)
        }
    }

    onMaxValueChanged: {
        if(value > maxValue){
            value = maxValue
        }
    }

    onMinValueChanged: {
        if(value < minValue){
            value = minValue
        }
    }
}
