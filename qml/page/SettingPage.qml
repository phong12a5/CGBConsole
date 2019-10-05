import QtQuick 2.0
import QtQuick.Dialogs 1.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2

import "../unit/"

Rectangle {
    id: settingPage

    property bool isOpenned: false

    function startAnimation(){
        openSettingAni.start()
    }

    FileDialog {
        id: fileDialog
        title: "Please choose installation folder of LD!"
        selectFolder: true
        onAccepted: {
            AppModel.ldIntallFolder = fileDialog.fileUrl
            startAnimation()
        }
        visible: false
    }

    Text {
        id: guideText
        text: qsTr("Select installation folder of LD.")
        anchors.centerIn: parent
        font.pixelSize: 20
    }

    ButtonTextItem {
        id: browser
        iconSource: "qrc:/image/opened-folder.png"
        label: "Browser"
        width: 100
        height: 40
        imgIcon.width: 30
        imgIcon.height: 30
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 40
        onButtonClicked:{
            fileDialog.visible = true
        }
    }

    PropertyAnimation{
        id: openSettingAni
        target: settingPage
        property: "x"
        from: settingPage.isOpenned? 0 : root.width
        to: settingPage.isOpenned? root.width : 0
        duration: 800
    }
}
