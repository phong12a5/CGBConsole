import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import "unit"
import "page"

Window {
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("LD controller")

    /* --------------- Properties -------------- */
    property var appNameModel: ["Facebook","FBLite","Zalo","Instagram","Pinterest","Twitter"]

    /* --------------- functions -------------- */
    function getThreadsModel(list){
        var data = []
        for(var j = 0; j < AppModel.maxNumberThread; j++){
            data.push(j + 1 + "Thread")
        }
        return data
    }

    Rectangle{
        id: line
        width: 2
        height: parent.height
        anchors.left: parent.right
        color: "black"
    }

    Item{
        id: contentArea
        width: parent.width
        height: parent.height
        anchors.right: parent.right

        Item {
            id: tokenItem
            height: 50
            width: title.width + token.width
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 50

            Text {
                id: title
                text: qsTr("Token: ")
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                font.pixelSize: 15
            }

            TextField {
                id: token
                text: AppModel.token
                font.pixelSize: 15
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                width: contentWidth > 200? contentWidth : 200
            }

            Text {
                id: hintText
                text: qsTr("Enter token")
                visible: token.text == ""
                anchors.left: token.left
                anchors.leftMargin: token.leftPadding
                anchors.verticalCenter: token.verticalCenter
                font.pixelSize: 15
                color: "grey"
                opacity: 0.2
            }

        }

        Item{
            id: appNameItem
            width: appnameTitle.width + appNameComb.width + 20
            height: 50
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: tokenItem.bottom
            anchors.topMargin:  20
            Text {
                id: appnameTitle
                text: qsTr("App Name: ")
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 15
                color: "grey"
            }

            ComboBox {
                id: appNameComb
                model: appNameModel
                width: 200
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                onActivated: {
                    AppModel.appName = appNameModel[currentIndex]
                }
                Component.onCompleted: {
                    for(var i = 0; i < appNameModel.length; i++) {
                        if(AppModel.appName === appNameModel[i])
                            currentIndex = i
                    }
                }
            }
        }



        ComboBox {
            id: threadSelItem
            enabled: !AppModel.isLaunchMutiTask
            currentIndex: AppModel.amountOfThread - 1
            model: getThreadsModel(AppModel.devicesList)
            width: 200
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: appNameItem.bottom
            anchors.topMargin: 20
            onActivated: {
                AppModel.amountOfThread = currentIndex + 1
            }
        }

        Button{
            id: startBtn

            property bool selected: false
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: threadSelItem.bottom
            anchors.topMargin: 20
            width: 100
            height: 50
            enabled: AppModel.token != "" || token.text != ""
            text: selected? "Stop" : "Start"
            onClicked: {
                selected = !selected
                if(selected){
                    AppModel.initializing = true
                    AppModel.token = token.text
                    AppModel.startProgram()
                }
                else
                    AppModel.stopProgarm()
            }
        }

    }

    Rectangle{
        anchors.fill: parent
        opacity: 0.5
        visible: AppModel.ldIntallFolder == ""
        MouseArea{
            anchors.fill: parent
            propagateComposedEvents: false
        }
    }

    SettingPage{
        id: settingPage
        x: parent.width
        y: 0
        width: parent.width
        height: parent.height
        onXChanged: {
            if(x == 0){
                isOpenned = true
            }else if(x == parent.width ){
                isOpenned = false
            }
        }
    }
    ButtonItem{
        id: settingBtn
        anchors.right: parent.right
        anchors.top: parent.top
        iconSource: "qrc:/image/setting.png"
        width: 50
        height: 50
        imgIcon.width: 30
        imgIcon.height: 30

        onButtonClicked: {
            settingPage.startAnimation()
        }
    }

    Rectangle{
        id: initPopup
        anchors.fill: parent
        color: "black"
        opacity: 0.8
        visible: AppModel.initializing
    }

    Text {
        text: qsTr("Initializing devices ........\nAnyway, Don't turn off application!")
        color: "white"
        anchors.centerIn: initPopup
        font.pixelSize: 15
        visible: AppModel.initializing
        horizontalAlignment: Text.AlignHCenter
    }

    ExpiredPopup{
        visible: AppModel.walletEmpty
    }

    Component.onCompleted: {
        if(AppModel.ldIntallFolder == ""){
            settingPage.startAnimation()
        }
    }
}
