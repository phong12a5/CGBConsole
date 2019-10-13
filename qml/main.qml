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
    title: qsTr("CGBConsole")

    /* --------------- Properties -------------- */
    property var appNameModel: ["Facebook","FBLite","Zalo","Instagram","Pinterest","Twitter"]

    /* --------------- functions -------------- */
    function getThreadsModel(maxThread){
        var data = []
        for(var j = 0; j < maxThread; j++){
            data.push(j + 1 + "Thread")
        }
        return data
    }

    function getVmModel(maxVmCount) {
        var data = []
        for(var j = 0; j < maxVmCount; j++){
            data.push(j + 1)
        }
        return data;
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
                font.bold: true
            }

            TextField {
                id: token
                text: AppModel.token
                font.pixelSize: 15
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                width: contentWidth > 300? contentWidth : 300
                horizontalAlignment: Text.AlignHCenter
                onAccepted: {
                    AppModel.token = text
                }
                onFocusChanged: {
                    if(!focus)
                        AppModel.token = text
                }
            }

            Text {
                id: hintText
                text: qsTr("Enter token")
                visible: token.text == ""
                anchors.left: token.left
                anchors.leftMargin: token.leftPadding
                anchors.verticalCenter: token.verticalCenter
                font.pixelSize: 15
                opacity: 0.2
            }

        }

        Item{
            id: appNameItem
            width: tokenItem.width
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
                font.bold: true
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


        Item {
            id: threadItem
            width: tokenItem.width
            height: 50
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: appNameItem.bottom
            anchors.topMargin: 20

            Text {
                id: threadSelTitle
                text: qsTr("Thread: ")
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 15
                font.bold: true
            }

            ComboBox {
                id: threadSelItem
                enabled: !AppModel.isLaunchMutiTask
                currentIndex: AppModel.amountOfThread - 1
                model: getThreadsModel(AppModel.maxNumberThread)
                width: 200
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                onActivated: {
                    AppModel.amountOfThread = currentIndex + 1
                }
            }
        }

        Item {
            id: vmItem
            width: tokenItem.width
            height: 50
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: threadItem.bottom
            anchors.topMargin: 20

            Text {
                id: vmTitle
                text: qsTr("VM: ")
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 15
                font.bold: true
            }

            ComboBox {
                id: vmCount
                enabled: !AppModel.isLaunchMutiTask
                currentIndex: AppModel.deviceCount - 1
                model: getVmModel(AppModel.maxVMCount)
                width: 200
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                onActivated: {
                    AppModel.deviceCount = currentIndex + 1
                    currentIndex = AppModel.deviceCount - 1
                }
            }
        }

        Item {
            id: emulatorOption
            width: tokenItem.width
            height: 50
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: vmItem.bottom
            anchors.topMargin: 20
            enabled: false

            Text {
                text: qsTr("LD Player")
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: ldOption.left
                anchors.rightMargin: 10
                enabled: parent.enabled
            }
            CheckBox{
                id: ldOption
                indicator.width: 30
                indicator.height: 30
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                checked: true
                enabled: parent.enabled
            }

            Text {
                text: qsTr("Nox Player")
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: noxOption.left
                anchors.rightMargin: 10
                enabled: parent.enabled
            }
            CheckBox{
                id: noxOption
                indicator.width: 30
                indicator.height: 30
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                checked: false
                enabled: parent.enabled
            }
        }

        Button{
            id: startBtn

            property bool selected: false
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: emulatorOption.bottom
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
        font.pixelSize: 20
        lineHeightMode: Text.FixedHeight
        lineHeight: 30
        visible: AppModel.initializing
        horizontalAlignment: Text.AlignHCenter
    }

    ExpiredPopup{
        visible: false //AppModel.walletEmpty
    }

    Component.onCompleted: {
        if(AppModel.ldIntallFolder == ""){
            settingPage.startAnimation()
        }
    }
}
