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

    /* --------------- Properties -------------- */
    property var appNameModel: ["Facebook","FBLite","Zalo","Instagram","Pinterest","Twitter"]
    property var resolutionModel: ["540,960,240","720,1280,320","900,1600,320","1080,1920,490", "Random"]

    /* --------------- functions -------------- */

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

    Item {
        id: invisualBtn
        width: 50
        height: 50
        MouseArea{
            property int count: 0
            anchors.fill: parent
            onReleased: {
                count ++
                if(count >= 5) {
                    count = 0
                    screenoptionItem.visible = !screenoptionItem.visible
                }
            }
        }
    }

    InfoItem{
        height: 20
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.bottom: parent.bottom
    }

    Item{
        id: contentArea
        width: parent.width
        anchors.top: root.bottom
        anchors.topMargin: 20
        height: emulatorOption.y + emulatorOption.height

        Item {
            id: tokenItem
            height: 50
            width: title.width + token.width
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 10

            Text {
                id: title
                text: qsTr("Token: ")
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                font.pixelSize: 17
                font.bold: true
            }

            TextField {
                id: token
                text: AppModel.token
                font.pixelSize: 17
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
                font.pixelSize: 17
                opacity: 0.2
            }

        }

        Item{
            id: appNameItem
            width: tokenItem.width
            height: 50
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: tokenItem.bottom
            anchors.topMargin:  5
            Text {
                id: appnameTitle
                text: qsTr("App Name: ")
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 17
                font.bold: true
            }

            CustomizedComboBox {
                id: appNameComb
                model: appNameModel
                width: 200
                font.pixelSize: 17
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
            anchors.topMargin: 5

            Text {
                id: threadSelTitle
                text: qsTr("Thread: ")
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 17
                font.bold: true
            }

            CustomizedComboBox {
                id: threadSelItem
                enabled: !AppModel.isLaunchMutiTask
                currentIndex: AppModel.amountOfThread
                model: AppModel.maxNumberThread
                font.pixelSize: 17
                width: 200
                editable: true
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                onUpdateValue: AppModel.amountOfThread = currentIndex
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                onEditTextChanged: {
                    if(isNaN(parseInt(editText,10))){
                        editText = 0
                    }else{
                        var value = parseInt(editText,10)
                        if(value > AppModel.maxNumberThread){
                            value = AppModel.maxNumberThread
                        }else if(value < 0){
                            value = 0
                        }

                        editText = value
                        AppModel.amountOfThread = value
                    }
                }

            }
        }

        Item {
            id: vmItem
            width: tokenItem.width
            height: 50
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: threadItem.bottom
            anchors.topMargin: 5

            Text {
                id: vmTitle
                text: qsTr("VM: ")
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 17
                font.bold: true
            }

            CustomizedComboBox {
                id: vmCount
                enabled: !AppModel.isLaunchMutiTask
                currentIndex: AppModel.deviceCount
                model: AppModel.maxVMCount
                width: 200
                font.pixelSize: 17
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                editable: true
                onUpdateValue: AppModel.deviceCount = currentIndex
                onEditTextChanged: {
                    if(isNaN(parseInt(editText,10))){
                        editText = 0
                    }else{
                        var value = parseInt(editText,10)
                        if(value > AppModel.maxVMCount){
                            value = AppModel.maxVMCount
                        }else if(value < 0){
                            value = 0
                        }

                        editText = value
                        AppModel.deviceCount = value
                    }
                }
            }
        }

        Item {
            id: screenoptionItem
            width: tokenItem.width
            visible: false
            height: 50
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: vmItem.bottom
            anchors.topMargin: 5

            Text {
                id: screenoptionTile
                text: qsTr("Resolution: ")
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 17
                font.bold: true
            }

            CustomizedComboBox {
                id: screenoption
                enabled: !AppModel.isLaunchMutiTask
                currentIndex: getCurrentResolutionIndex(AppModel.resolution)
                model: resolutionModel
                width: 200
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 17
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
            anchors.top: screenoptionItem.visible? screenoptionItem.bottom : vmItem.bottom
            anchors.topMargin: 5
            enabled: false

            Text {
                text: qsTr("LD Player")
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: ldOption.left
                anchors.rightMargin: 10
                font.pixelSize: 17
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
                font.pixelSize: 17
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

    AdsComponent{
        id: ads
        width: parent.width
        height: 200
        anchors.top: contentArea.bottom
        anchors.topMargin: -10
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Button{
        id: startBtn
        property bool selected: false
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: ads.bottom
        anchors.topMargin: -20
        font.pixelSize: 17
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

    RestartPopup{
        id: startPopup
        visible: AppModel.isShowRestartPopup
    }

    Component.onDestruction: AppMain.closingApp()
}
