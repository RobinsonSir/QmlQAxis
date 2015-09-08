#ifndef CURVE_H
#define CURVE_H

#include <QPen>
#include <QString>

class Curve
{
    //Q_OBJECT
public:
    Curve(QString title);

    void setTitle(QString title);
    void setTitleShow(bool flag);
    void setLinePen(QPen pen);
    void setPointSize(double size);
    void setPointPen(QPen pen);
    void setPointColor(const QColor color);
    void setCurveColor(const QColor color);
    void setCurveStyle(Qt::PenStyle style);
    void addData(QPointF d);
    void addPosData(QPointF p);
    void setRawData(const QVector<QPointF>& data, int size);
    void resetPosData(int index, QPointF p);

    QString getTitle();
    bool getTitleShow();
    QPointF getData(int index);
    QPointF getPosData(int index);
    QPen getLinePen();
    QPen getPointPen();
    int getDataSize();

    void clearData();

private:
    QString m_title;
    bool m_titleShow;
    QPen m_linePen;
    QPen m_pointPen;
    int m_penStyle;
    QVector<QPointF> m_data;
    QVector<QPointF> m_posData;

    int m_dataSize;
};

#endif
