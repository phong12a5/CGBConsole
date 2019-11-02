import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import "unit"
import "page"

Item {
    id: root
    visible: true
    width: adsItem.width + contentArea.width
    height: 600

    /* --------------- Properties -------------- */
    property var appNameModel: ["Facebook","FBLite","Zalo","Instagram","Pinterest","Twitter"]
    property var resolutionModel: ["540,960,240","720,1280,320","900,1600,320","1080,1920,490"]

    /* --------------- functions -------------- */
    function getThreadsModel(maxThread){
        var data = []
        for(var j = 0; j < maxThread; j++){
            data.push(j + 1 + " Thread")
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

    function getCurrentAppNameIndex(currentAppName) {
        for(var i = 0; i < appNameModel.length; i++) {
            if(currentAppName === appNameModel[i])
                return i
        }
        return 0
    }

    function getCurrentResolutionIndex(currentResolution) {
        for(var i = 0; i < resolutionModel.length; i++) {
            if(currentResolution === resolutionModel[i])
                return i
        }
        return 0
    }


    Item{
        id: adsItem
        width: 300
        height: parent.height
        anchors.right: parent.right
        Image {
            id: backGround
            source: "qrc:/image/background.jpg"
            opacity: 0.8
            y: 70
            width: parent.width
            height: parent.width
            anchors.horizontalCenter: parent.horizontalCenter
        }
        AdsComponent{
            id: ads
            width: 250
            height: 40
            anchors.top: backGround.bottom
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }


    Item{
        id: contentArea
        width: 400
        height: parent.height

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
                currentIndex: getCurrentAppNameIndex(AppModel.appName)
                onActivated: {
                    AppModel.appName = appNameModel[currentIndex]
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
            id: screenoptionItem
            width: tokenItem.width
            height: 50
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: vmItem.bottom
            anchors.topMargin: 20

            Text {
                id: screenoptionTile
                text: qsTr("Resolution: ")
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 15
                font.bold: true
            }

            ComboBox {
                id: screenoption
                enabled: !AppModel.isLaunchMutiTask
                currentIndex: getCurrentResolutionIndex(AppModel.resolution)
                model: resolutionModel
                width: 200
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                onActivated: {
                    AppModel.resolution = resolutionModel[currentIndex]
                }
            }
        }

        Item {
            id: emulatorOption
            width: tokenItem.width
            height: 50
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: screenoptionItem.bottom
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
    }

    Button{
        id: startBtn

        property bool selected: false
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 80
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


    Rectangle{
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: "black"
        width: parent.width
        height: 30
        visible: AppModel.taskInProgress !== ""
        opacity: 0.8
        Text {
            id: name
            text: AppModel.taskInProgress
            anchors.centerIn: parent
            font.pixelSize: 15
            color: "white"
        }
    }

    Component.onDestruction: AppMain.closingApp()
}
