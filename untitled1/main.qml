import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Loader{
        id: testLoader
        source: "qrc:/Item1.qml"
        anchors.fill: parent
    }

    Timer{
        running: true
        interval: 2000
        onTriggered: {
            testLoader.source = ""
        }
    }
}
