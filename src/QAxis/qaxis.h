#ifndef Curve_H
#define Curve_H

#include <QtQuick/QQuickPaintedItem>
#include <QColor>
#include <QPen>
#include <QTimer>

#include "curve.h"

class QAxis : public QQuickPaintedItem
{
    Q_OBJECT
public:
    /**
     * @brief QAxis class
     *
     * Notice: set x and y range before using addData function
     */
    explicit QAxis(QQuickPaintedItem *parent = 0);
    ~QAxis();
    void paint(QPainter *painter);
    Q_INVOKABLE void clearChart();

    Q_INVOKABLE void addCurve(QString title);
    Q_INVOKABLE void addData(QString curveName, double tmpX, double tmpY);
    Q_INVOKABLE void addVectorData(QString curName, const QVector<QPointF> &data);
    Q_INVOKABLE void clearCurveData(QString curveName);

    Q_INVOKABLE void setXValueRange(double min, double max);
    Q_INVOKABLE void setYValueRange(double min, double max);

    Q_INVOKABLE void setXLabels(QString labels);
    Q_INVOKABLE void setYLabels(QString labels);

    //QPointF.x(): label, QPointF.y(): label position
    Q_INVOKABLE void setXLabels(QString labels, QList<double> pos);
    Q_INVOKABLE void setYLabels(QString labels, QList<double> pos);

    Q_INVOKABLE void setXTitle(QString title);
    Q_INVOKABLE void setYTitle(QString title);
    Q_INVOKABLE void setCurveTitleShow(QString curveName, bool f);

    Q_INVOKABLE Curve* getCurve(QString name);
    Q_INVOKABLE void setCurveLineStyle(QString name, Qt::PenStyle style);
    Q_INVOKABLE void setCurvePointSize(QString name, double size);

    //NOTE: Use it after finishing adding data
    Q_INVOKABLE void setGridShow(bool f);

    Q_INVOKABLE void setDirection(QString d);

    //axis: X or Y
    Q_INVOKABLE void setAutoScale(QString axis, bool f);

    Q_INVOKABLE void setAxisSize(double width, double height);

    Q_INVOKABLE void addTargetLine(QString lineName);

    Q_INVOKABLE void upDate();   

    inline double getAxisXmin()
    {
        return m_xMin;
    }

    inline double getAxisXmax()
    {
        return m_xMax;
    }

    inline double getAxisYmin()
    {
        return m_yMin;
    }

    inline double getAxisYmax()
    {
        return m_yMax;
    }

private:

    void initXYState();
    void upDateXLabels();
    void upDateYLabels();
    QVector<QPointF> m_data;
    //QList<Curve*> m_curveList;
    QMap<QString, Curve*> m_curveList;

    QPainter *m_painter;

    double m_xMin;
    double m_xMax;

    double m_yMin;
    double m_yMax;

    double m_xPosMin;
    double m_xPosMax;

    double m_yPosMin;
    double m_yPosMax;

    double m_xRange;
    double m_yRange;

    double m_xValueRange;
    double m_yValueRange;

    QString m_direction;

    QString m_xTitle;
    QString m_yTitle;

    QString m_xLabels;
    QString m_yLabels;

    QList<double> m_xLabelsPos;
    QList<double> m_yLabelsPos;

    bool m_xAutoScale;
    bool m_yAutoScale;

    bool m_gridFlag;

    bool m_firstPoint;

    double m_xLabelLinePenSize;
    double m_xLabelTextPenSize;
    double m_yLabelLinePenSize;
    double m_yLabelTextPenSize;

    int m_dataSize;

    QVector<QPointF> m_testData;// for test

    bool m_xExpandFlag;
    bool m_yExpandFlag;
};

#endif
