import QtQuick 2.0

Text {
    id: name
    text: qsTr("Version ") + AppModel.versionCode + ".0"
    font.pixelSize: 12
    opacity: 0.6
    width: contentWidth
    horizontalAlignment: Text.AlignRight
}
