import QtQuick 2.3
import QtQuick.Window 2.0
import QtQuick.Controls 1.1
//import QtQuick.Controls.Styles 1.1

import QtQml 2.2

import com.robinson.qaxis 1.0

ApplicationWindow {
    id: mainWindow
    visible: true
    width: Screen.width/2
    height: Screen.height/2

    QAxis {
        id: qAxis
        anchors.fill: parent

        Component.onCompleted: {
            //setDirection("down");
            setXValueRange(0, 10);
            setYValueRange(0, 10);

            setXLabels("0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10");
            setYLabels("0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10");

            setAutoScale("X", true);
            setAutoScale("Y", true);

            addCurve("testCurve");
            setGridShow(true);
            addData("testCurve", 0, 0);
            addData("testCurve", 2, 2);
            addData("testCurve", 5, 3);
        }
    }

    //使坐标轴自适应窗口大小
    onWidthChanged: qAxis.setAxisSize(mainWindow.width, mainWindow.height);
    onHeightChanged: qAxis.setAxisSize(mainWindow.width, mainWindow.height);
}
