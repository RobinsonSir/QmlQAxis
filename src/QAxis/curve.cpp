#include "curve.h"
#include <QDebug>

Curve::Curve(QString title)
{
    m_title = title;
    m_linePen.setColor(Qt::black);
    m_linePen.setWidthF(1);
    m_pointPen.setColor(Qt::black);
    m_pointPen.setWidthF(3);
    m_titleShow = true;

    m_dataSize = 0;
}

void Curve::setTitle(QString title)
{
    m_title = title;
}

void Curve::setTitleShow(bool flag)
{
    m_titleShow = flag;
}

void Curve::setLinePen(QPen pen)
{
    m_linePen = pen;
}

void Curve::setPointSize(double size)
{
    m_pointPen.setWidthF(size);
}

void Curve::setPointPen(QPen pen)
{
    m_pointPen = pen;
}

void Curve::setPointColor(const QColor color)
{
    m_pointPen.setColor(color);
}

void Curve::setCurveColor(const QColor color)
{
    m_linePen.setColor(color);
}

void Curve::setCurveStyle(Qt::PenStyle style)
{
    m_linePen.setStyle(style);
}

void Curve::addData(QPointF d)
{
    m_data << d;
    m_dataSize = m_data.size();
}

void Curve::addPosData(QPointF p)
{
    m_posData << p;
}

void Curve::setRawData(const QVector<QPointF> &data, int size)
{
    m_data = data;
    m_dataSize = size;
}

void Curve::resetPosData(int index, QPointF p)
{
    m_posData.replace(index, p);
}

void Curve::clearData()
{
    m_data.clear();
    m_posData.clear();
    m_dataSize = 0;
}

QString Curve::getTitle()
{
    return m_title;
}

bool Curve::getTitleShow()
{
    return m_titleShow;
}

QPointF Curve::getData(int index)
{
    return m_data.at(index);
}

QPointF Curve::getPosData(int index)
{
    return m_posData.at(index);
}

int Curve::getDataSize()
{
    //return m_data.size();
    return m_dataSize;
}

QPen Curve::getLinePen()
{
    return m_linePen;
}

QPen Curve::getPointPen()
{
    return m_pointPen;
}
