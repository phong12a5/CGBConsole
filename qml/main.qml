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
            if(currentAppName === appNameModel[i].toLowerCase()){
                return i
            }
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
                    AppModel.testMode = !AppModel.testMode
                }
            }
        }
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
                font.pixelSize: 16
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                width: contentWidth > 300? contentWidth : 300
                horizontalAlignment: Text.AlignHCenter
                onTextChanged: {
                    AppModel.token = text
                    background.border.color = activeFocus? (AppModel.validToken? "#0066ff" : "#cc0000"):"#bdbdbd"
                }
                height: 30
                onActiveFocusChanged: {
                    background.border.color = activeFocus? (AppModel.validToken? "#0066ff" : "#cc0000"):"#0066ff"
                }
                Component.onCompleted: {
                    background.border.color = activeFocus? (AppModel.validToken? "#0066ff" : "#cc0000"):"#0066ff"
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

            Rectangle{
                anchors.left: token.left
                anchors.top: token.bottom
                anchors.topMargin: 5
                width: 6
                height: 6
                radius: width/2
                color: AppModel.validToken? "#0066ff" : "#cc0000"
                Text {
                    text: AppModel.validToken? "Valid token" : "Invalid token"
                    font.pixelSize: 12
                    anchors.left: parent.right
                    anchors.leftMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                    opacity: 0.6
                    color: AppModel.validToken? "#0066ff" : "#cc0000"
                }
            }
        }

        Item{
            id: appNameItem
            width: tokenItem.width
            height: 50
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: tokenItem.bottom
            anchors.topMargin:  5
            enabled: false && AppModel.token != "" && parent.enabled  &&  (!AppModel.appStarted)
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
                    AppModel.appName = appNameModel[currentIndex].toLowerCase()
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
            enabled: AppModel.token != ""

            Text {
                id: threadSelTitle
                text: qsTr("Thread: ")
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 17
                font.bold: true
            }

            CustomizedInputField{
                enabled: !AppModel.isLaunchMutiTask
                value: AppModel.amountOfThread
                maxValue: AppModel.deviceCount > AppModel.maxNumberThread? AppModel.maxNumberThread : AppModel.deviceCount
                minValue: 0
                valStep: 1
                width: 200
                height: appNameComb.height
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                onValueChanged: {
                    AppModel.amountOfThread = value
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
            enabled: AppModel.token != ""

            Text {
                id: vmTitle
                text: qsTr("Max LD: ")
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 17
                font.bold: true
            }

            CustomizedInputField{
                enabled: !AppModel.isLaunchMutiTask
                value: AppModel.deviceCount
                maxValue: AppModel.maxVMCount
                minValue: 0
                valStep: 5
                width: 200
                height: appNameComb.height
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                onValueChanged: {
                    AppModel.deviceCount = value
                }
            }
        }

        Item {
            id: screenoptionItem
            width: tokenItem.width
            visible: AppModel.testMode
            height: 50
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: vmItem.bottom
            anchors.topMargin: 5
            enabled: AppModel.token != ""

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
            enabled: false && AppModel.token != ""

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

    Text{
        anchors.horizontalCenter: parent.horizontalCenter
        visible: AppModel.testMode
        text: "TEST MODE"
        opacity: 0.2
        font.pixelSize: 30
        anchors.bottom: startBtn.top
        anchors.bottomMargin: 20
    }

    Button{
        id: startBtn
        property bool selected: AppModel.appStarted
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: ads.bottom
        anchors.topMargin: -20
        font.pixelSize: 17
        width: 100
        height: 50
        text: selected? "Stop" : "Start"
        enabled: AppModel.validToken
        onClicked: {
            AppModel.token = token.text
            if(AppModel.token != ""){
                AppModel.appStarted = !AppModel.appStarted
                if(AppModel.appStarted){
                    AppModel.initializing = true
                    AppModel.startProgram()
                }
                else
                    AppModel.stopProgarm()
            }
        }
    }


    Rectangle{
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        width: parent.width
        height: 30
        visible: AppModel.devicesRunningList.length > 0 && inprogressItem.visible == false
        opacity: 0.8
        gradient: Gradient {
            GradientStop { position: 0.0; color: "transparent" }
            GradientStop { position: 1.0; color: "#63ff42" }
        }
        Text {
            id: name
            text: "Running list: " + AppModel.devicesRunningList.length
            anchors.centerIn: parent
            font.pixelSize: 15
            color: "black"
        }
    }


    Item{
        id: inprogressItem
        width: parent.width
        height: 40
        anchors.bottom: parent.bottom
        visible:  AppModel.taskInProgress !== ""
        Text {
            id: txt
            text: AppModel.taskInProgress
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 5
            font.pixelSize: 13
            color: "#006600"
        }
        ProgressBar {
            id: control
            width: parent.width
            height: 20
            indeterminate: true
            anchors.bottom: parent.bottom
        }
    }



    RestartPopup{
        id: startPopup
        visible: AppModel.isShowRestartPopup
    }

    InfoItem{
        height: 20
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: inprogressItem.visible? 10 : 0
    }

    Component.onDestruction: AppMain.closingApp()
}
