import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import "unit"
import "page"

Window {
    id: root
    title: "CGB Console"
    visible: true
    width: 400
    height: 540

    maximumHeight: height
    maximumWidth: width

    minimumHeight: height
    minimumWidth: width

    Button{
        id: startBtn
        property bool selected: AppModel.appStarted
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        font.pixelSize: 17
        width: 100
        height: 50
        text: selected? "Stop" : "Start"
        enabled: true
        onClicked: {
            AppModel.appStarted = !AppModel.appStarted
            if(AppModel.appStarted){
                AppMain.startProgram()
            } else {
                AppMain.stopProgarm()
            }
        }
    }

    Component.onDestruction: AppMain.closingApp()
}
