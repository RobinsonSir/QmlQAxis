#include <QPainter>
#include <QDebug>
#include "math.h"

#include "qaxis.h"

QAxis::QAxis(QQuickPaintedItem *parent) : QQuickPaintedItem(parent)
{
    m_xPosMin = 20;
    m_xPosMax = 365;
    m_yPosMin = 20;
    m_yPosMax = 365;

    m_xRange = m_xPosMax - m_xPosMin - 20;
    m_yRange = m_yPosMax - m_yPosMin - 20;

    //default setting
    m_gridFlag = false;
    m_xAutoScale = false;
    m_yAutoScale = false;
    //m_direction = "up";
    m_direction = "down";
    m_xTitle = "x";
    m_yTitle = "y";
    m_xLabelLinePenSize = 0.5;
    m_xLabelTextPenSize = 9;
    m_yLabelLinePenSize = 0.5;
    m_yLabelTextPenSize = 9;
    m_dataSize = 0;
    m_curveList.clear();
    m_xLabelsPos.clear();
    m_yLabelsPos.clear();

    m_xMin = 0;
    m_xMax = 100;
    m_yMin = 0;
    m_yMax = 100;

    m_firstPoint = false;

    m_testData.append(QPointF(0, 0));
    m_testData.append(QPointF(100, 0.04));
}

QAxis::~QAxis()
{

    QMap<QString, Curve*>::iterator cur;
    for(cur=m_curveList.begin(); cur!=m_curveList.end(); cur++)
    {
        delete cur.value();
        cur.value() = NULL;
    }

    m_curveList.clear();

}

void QAxis::paint(QPainter *painter)
{

    m_painter = painter;

    //随坐标轴取值范围变化，更新坐标标签及其位置
    if(m_xAutoScale)
        upDateXLabels();
    if(m_yAutoScale)
        upDateYLabels();

    initXYState();

    m_painter->setRenderHint(QPainter::Antialiasing);

    QMap<QString, Curve*>::const_iterator cur;
    for(cur=m_curveList.begin(); cur!=m_curveList.end(); cur++)
    {       
        for(int j=0; j<cur.value()->getDataSize(); j++)
        {
            QPointF tmp = cur.value()->getPosData(j);
            m_painter->setPen(cur.value()->getPointPen());
            m_painter->drawEllipse(tmp.x()-1, tmp.y()-1, 2, 2);

            //draw curve title
            if(j == cur.value()->getDataSize() - 1)
            {
                if(cur.value()->getTitleShow())
                {
                    QPen pen;
                    pen.setColor(Qt::red);
                    m_painter->setPen(pen);

                    double x = tmp.x()+8;
                    double y = tmp.y();
                    if(x > m_xPosMax - 20)
                        x = x - 25;
                    if(y > m_yPosMax - 20)
                        y = y - 25;

                    m_painter->drawText(QPointF(x, y), cur.value()->getTitle());
                }

                break;
            }

            m_painter->setPen(cur.value()->getLinePen());
            m_painter->drawLine(cur.value()->getPosData(j), cur.value()->getPosData(j+1));
        }
    }
}

void QAxis::clearChart()
{

    QMap<QString, Curve*>::iterator cur;
    for(cur=m_curveList.begin(); cur!=m_curveList.end(); cur++)
    {
        delete cur.value();
        cur.value() = NULL;
    }

    m_curveList.clear();

    upDate();
}

void QAxis::addCurve(QString title)
{
    m_curveList.insert(title, new Curve(title));
}

void QAxis::addData(QString curveName, double tmpX, double tmpY)
{ 
    if(m_xAutoScale)
    {
        if(tmpX > m_xMax)
        {
            m_xMax = tmpX;
            m_xExpandFlag = true;
        }
        if(tmpX < m_xMin)
            m_xMin = tmpX;
    }

    if(m_yAutoScale)
    {
        if(tmpY > m_yMax)
        {
            m_yMax = tmpY;
            m_yExpandFlag = true;
        }
        if(tmpY < m_yMin)
            m_yMin = tmpY;
    }

    double x = (tmpX - m_xMin)/m_xValueRange * m_xRange + m_xPosMin/* - 0.5*/;
    double y;

    if( m_direction == "down")
        y = (tmpY - m_yMin)/m_yValueRange * m_yRange + m_yPosMin/* - 0.5*/;
    else
        y = -(tmpY - m_yMin)/m_yValueRange * m_yRange /* + m_yPosMin + 0.5 */;

    QPointF tmpPoint(x, y);
    m_curveList[curveName]->addPosData(tmpPoint);
    m_curveList[curveName]->addData(QPointF(tmpX, tmpY));
    m_firstPoint = true;

    upDate();
}

void QAxis::addVectorData(QString curveName, const QVector<QPointF> &data)
{
    int beginIndex = m_curveList[curveName]->getDataSize();
    for(int i=beginIndex; i<data.size(); i++)
    {
        double tmpX = data.at(i).x();
        double tmpY = data.at(i).y();
        if(m_xAutoScale)
        {
            if(!m_firstPoint)
            {
                m_xMax = tmpX;
                m_xMin = tmpX;
            }
            else
            {
                if(tmpX > m_xMax)
                    m_xMax = tmpX;
                if(tmpX < m_xMin)
                    m_xMin = tmpX;
            }
        }

        if(m_yAutoScale)
        {
            if(!m_firstPoint)
            {
                m_yMax = tmpY;
                m_yMin = tmpX;
            }
            else
            {
                if(tmpY > m_yMax)
                    m_yMax = tmpY;
                if(tmpY < m_yMin)
                    m_yMin = tmpY;
            }
        }

        double x = (tmpX - m_xMin)/m_xValueRange * m_xRange + m_xPosMin - 0.5;
        double y;

        if( m_direction == "down")
            y = (tmpY - m_yMin)/m_yValueRange * m_yRange + m_yPosMin - 0.5;
        else
            y = -(tmpY - m_yMin)/m_yValueRange * m_yRange + m_yPosMin + 0.5;

        QPointF tmpPoint(x, y);
        m_curveList[curveName]->addPosData(tmpPoint);
        m_curveList[curveName]->addData(QPointF(tmpX, tmpY));
        m_firstPoint = true;
        upDate();

    }
}

void QAxis::clearCurveData(QString curveName)
{
    m_curveList[curveName]->clearData();
    upDate();
}

void QAxis::upDate()
{
    update();
}

void QAxis::setXValueRange(double min, double max)
{
    m_xMin = min;
    m_xMax = max;

    m_xValueRange = m_xMax - m_xMin;

    QMap<QString, Curve*>::const_iterator cur;
    for(cur=m_curveList.begin(); cur!=m_curveList.end(); cur++)
    {
        for(int j=0; j<cur.value()->getDataSize(); j++)
        {
            double x = cur.value()->getData(j).x();
            double y = cur.value()->getPosData(j).y();

            x = (x - m_xMin)/m_xValueRange * m_xRange + m_xPosMin/* - 0.5*/;

            cur.value()->resetPosData(j, QPointF(x, y));
        }
    }
}

void QAxis::setYValueRange(double min, double max)
{
    m_yMin = min;
    m_yMax = max;

    m_yValueRange = m_yMax - m_yMin;

    QMap<QString, Curve*>::const_iterator cur;
    for(cur=m_curveList.begin(); cur!=m_curveList.end(); cur++)
    {
        for(int j=0; j<cur.value()->getDataSize(); j++)
        {
            double x = cur.value()->getPosData(j).x();
            double y = cur.value()->getData(j).y();

            if( m_direction == "down")
                y = (y - m_yMin)/m_yValueRange * m_yRange + m_yPosMin/* - 0.5*/;
            else
                y = -(y - m_yMin)/m_yValueRange * m_yRange /* + m_yPosMin + 0.5 */;

            cur.value()->resetPosData(j, QPointF(x, y));
        }
    }
}

void QAxis::setXLabels(QString labels)
{
    m_xLabelsPos.clear();
    m_xLabels = labels;
}

void QAxis::setYLabels(QString labels)
{
    m_yLabelsPos.clear();
    m_yLabels = labels;
}

void QAxis::setXLabels(QString labels , QList<double> pos)
{
    m_xLabels = labels;
    m_xLabelsPos = pos;
}

void QAxis::setYLabels(QString labels, QList<double> pos)
{
    m_yLabels = labels;
    m_yLabelsPos = pos;
}

void QAxis::setXTitle(QString title)
{
    m_xTitle = title;
}

void QAxis::setYTitle(QString title)
{
    m_yTitle = title;
}

void QAxis::setCurveTitleShow(QString curveName, bool f)
{
    m_curveList[curveName]->setTitleShow(f);
}

Curve* QAxis::getCurve(QString name)
{
    return m_curveList[name];
}

void QAxis::setCurveLineStyle(QString name, Qt::PenStyle style)
{
    m_curveList[name]->setCurveStyle(style);
}

void QAxis::setCurvePointSize(QString name, double size)
{
    m_curveList[name]->setPointSize(size);
}

void QAxis::setGridShow(bool f)
{
    if( f != m_gridFlag)
    {
        m_gridFlag = f;
        upDate();
    }
}

void QAxis::setDirection(QString d)
{
    if(d != m_direction)
    {
        double x ;
        double y ;

        QMap<QString, Curve*>::const_iterator cur;
        for(cur=m_curveList.begin(); cur!=m_curveList.end(); cur++)
        {
            for(int j=0; j<cur.value()->getDataSize(); j++)
            {
                x = cur.value()->getPosData(j).x();
                y = cur.value()->getPosData(j).y();
                y = 2*m_yPosMin - y;
                cur.value()->resetPosData(j, QPointF(x, y));
            }
        }

     m_direction = d;

     upDate();
    }

}

void QAxis::setAutoScale(QString axis, bool f)
{
    if(axis == "X")
        m_xAutoScale = f;
    else if(axis == "Y")
        m_yAutoScale = f;
    else
        return;
}

void QAxis::setAxisSize(double width, double height)
{
    if(width < 100)
       m_xPosMax = 100;
    else
       m_xPosMax = width;

    if(height < 100)
       m_yPosMax = 100;
    else
       m_yPosMax = height;

    m_xPosMax = m_xPosMax - 40;
    m_yPosMax = m_yPosMax - 40;

    m_xRange = m_xPosMax - m_xPosMin - 20;
    m_yRange = m_yPosMax - m_yPosMin - 20;

    QMap<QString, Curve*>::const_iterator cur;
    for(cur=m_curveList.begin(); cur!=m_curveList.end(); cur++)
    {
        for(int j=0; j<cur.value()->getDataSize(); j++)
        {
            double x = cur.value()->getData(j).x();
            double y = cur.value()->getData(j).y();

            x = (x - m_xMin)/m_xValueRange * m_xRange + m_xPosMin/* - 0.5*/;

            if( m_direction == "down")
                y = (y - m_yMin)/m_yValueRange * m_yRange + m_yPosMin/* - 0.5*/;
            else
                y = -(y - m_yMin)/m_yValueRange * m_yRange /* + m_yPosMin + 0.5 */;

            cur.value()->resetPosData(j, QPointF(x, y));
        }
    }

    upDate();

}

void QAxis::initXYState()
{
    if(m_direction == "down")
    {
        m_painter->translate(m_xPosMin, m_yPosMin); //origin point
        m_painter->drawLine(15, m_yPosMin, m_xPosMax, m_yPosMin); //x
        m_painter->drawLine(m_xPosMin, 15, m_xPosMin, m_yPosMax); //y
        m_painter->drawText(m_xPosMax-5, m_yPosMin+6, ">");

        m_painter->save();
        m_painter->rotate(90);
        m_painter->drawText(m_yPosMax-5, m_yPosMin-35, ">");
        m_painter->restore();

        QFont font = m_painter->font();
        font.setPointSizeF(9);
        m_painter->setFont(font);
        m_painter->drawText(m_xPosMax-18, m_yPosMin+20, m_xTitle);
        m_painter->drawText(m_xPosMin+15, m_yPosMax+10, m_yTitle);

        //x labels
        QStringList xList = m_xLabels.split(',');
        int num = xList.size();
        double y = m_yPosMin;
        double x = 0.0;
        double lastX;

        double endPointX;
        if(m_xLabelsPos.size() > 0)
            endPointX = m_xLabelsPos.at(num-1);
        else
            endPointX = xList.at(num-1).toDouble();
        endPointX = (endPointX - m_xMin)/m_xValueRange * m_xRange + m_xPosMin;

        for(int i=0; i<num; i++)
        {
            if(m_xLabelsPos.size() > 0)
                x = m_xLabelsPos.at(i);
            else
                x = xList.at(i).toDouble();

            x = (x - m_xMin)/m_xValueRange * m_xRange + m_xPosMin;

            QPen penLine(Qt::black, m_xLabelLinePenSize);
            m_painter->setPen(penLine);
            m_painter->drawLine(QPointF(x, y), QPointF(x, y-5));

            QFont font = m_painter->font();
            font.setPointSizeF(m_xLabelTextPenSize);
            font.setFamily("Calibri Light");
            m_painter->setFont(font);

            QString tmp = xList.at(i);
            QFontMetricsF tmpM(font);
            double w = tmpM.width(tmp);
            QRect rc(x-w/2, y-20, 50, 50);

            if(i == 0)
            {
                lastX = x;          
                m_painter->drawText(rc, Qt::TextWrapAnywhere, xList.at(i));
            }
            else
            {
                if(x-lastX < 25)
                {
                    m_painter->drawText(rc, Qt::TextWrapAnywhere, "");
                }
                else
                {
                    if(endPointX-x < 25)
                    {
                        if(i == num-1)
                            m_painter->drawText(rc, Qt::TextWrapAnywhere, xList.at(i));
                        else
                            m_painter->drawText(rc, Qt::TextWrapAnywhere, "");
                    }
                    else
                    {
                        lastX = x;
                        m_painter->drawText(rc, Qt::TextWrapAnywhere, xList.at(i));
                    }
                }
            }

            if(m_gridFlag)
            {
                if(i == 0)
                    continue;

                QPen pen;
                pen.setColor("darkCyan");
                pen.setStyle(Qt::DashDotLine);
                m_painter->setPen(pen);
                m_painter->drawLine(QPointF(x, y), QPointF(x, m_yPosMin+m_yRange));
            }
        }

        //y labels
        QStringList yList = m_yLabels.split(',');
        num = yList.size();
        double xMax = x;
        x = m_xPosMin;
        y = 0.0;
        double lastY;

        double endPointY;
        if(m_yLabelsPos.size() > 0)
            endPointY = m_yLabelsPos.at(num-1);
        else
            endPointY = yList.at(num-1).toDouble();
        endPointY = (endPointY - m_yMin)/m_yValueRange * m_yRange + m_yPosMin;

        for(int i=0; i<num; i++)
        {
            if(m_yLabelsPos.size() > 0)
                y = m_yLabelsPos.at(i);
            else
                y = yList.at(i).toDouble();

            y = (y - m_yMin)/m_yValueRange * m_yRange + m_yPosMin;

            QPen penLine(Qt::black, m_yLabelLinePenSize);
            m_painter->setPen(penLine);
            m_painter->drawLine(QPointF(x, y), QPointF(x-5, y));

            QFont font = m_painter->font();
            font.setPointSizeF(m_yLabelTextPenSize);
            m_painter->setFont(font);

            if(i == 0)
            {
                lastY = y;
                m_painter->drawText(QPointF(x-35, y+5), yList.at(i));
            }
            else
            {
                QFontMetricsF tmpM(font);
                double h = tmpM.height();

                if(y-lastY < h+5)
                    m_painter->drawText(QPointF(x-35, y+5), "");
                else
                {
                    if(endPointY-y < h+5)
                    {
                        if(i == num-1)
                            m_painter->drawText(QPointF(x-35, y+5), yList.at(i));
                        else
                            m_painter->drawText(QPointF(x-35, y+5), "");
                    }
                    else
                    {
                        lastY = y;
                        m_painter->drawText(QPointF(x-35, y+5), yList.at(i));
                    }
                }
            }

            if(m_gridFlag)
            {
                if(i == 0)
                    continue;

                QPen pen;
                //pen.setWidthF(0.1);
                pen.setColor("darkCyan");
                pen.setStyle(Qt::DashDotLine);
                m_painter->setPen(pen);
                //m_painter->drawLine(QPointF(x, y), QPointF(m_xPosMin+m_xRange, y));
                m_painter->drawLine(QPointF(x, y), QPointF(xMax, y));
            }
        }


    }
    else//up
    {
        m_painter->translate(m_xPosMin, m_yPosMax); //origin point
        m_painter->drawLine(15, 0, m_xPosMax, 0); //x
        m_painter->drawLine(m_xPosMin, m_yPosMin-15, m_xPosMin, -m_yPosMax+10); //y
        m_painter->drawText(m_xPosMax-5, 6, ">");

        m_painter->save();
        m_painter->rotate(-90);
        m_painter->drawText(m_yPosMax-18, m_yPosMin+6, ">");
        m_painter->restore();

        QFont font = m_painter->font();
        font.setPointSizeF(7.5);
        m_painter->setFont(font);
        m_painter->drawText(m_xPosMax-18, -10, m_xTitle);
        m_painter->drawText(m_xPosMin+15, -m_yPosMax+15, m_yTitle);

        //x labels
        QStringList xList = m_xLabels.split(',');
        int num = xList.size();
        double y = 0;
        double x = 0.0;
        double lastX;

        double endPointX;
        if(m_xLabelsPos.size() > 0)
            endPointX = m_xLabelsPos.at(num-1);
        else
            endPointX = xList.at(num-1).toDouble();
        endPointX = (endPointX - m_xMin)/m_xValueRange * m_xRange + m_xPosMin;

        for(int i=0; i<num; i++)
        {
            if(m_xLabelsPos.size() > 0)
                x = m_xLabelsPos.at(i);
            else
                x = xList.at(i).toDouble();

            x = (x - m_xMin)/m_xValueRange * m_xRange + m_xPosMin;

            QPen penLine(Qt::black, m_xLabelLinePenSize);
            m_painter->setPen(penLine);
            m_painter->drawLine(QPointF(x, y), QPointF(x, y+5));

            QFont font = m_painter->font();
            font.setPointSizeF(m_xLabelTextPenSize);
            m_painter->setFont(font);

            QString tmp = xList.at(i);
            QFontMetricsF tmpM(font);
            double w = tmpM.width(tmp);
            QRect rc(x-w/2, y+7, 50, 50);

            if(i == 0)
            {
                lastX = x;
                m_painter->drawText(rc, Qt::TextWrapAnywhere, xList.at(i));
            }
            else
            {
                if(x-lastX < 25)
                {
                    m_painter->drawText(rc, Qt::TextWrapAnywhere, "");
                }
                else
                {
                    if(endPointX-x < 25)
                    {
                        if(i == num-1)
                            m_painter->drawText(rc, Qt::TextWrapAnywhere, xList.at(i));
                        else
                            m_painter->drawText(rc, Qt::TextWrapAnywhere, "");
                    }
                    else
                    {
                        lastX = x;
                        m_painter->drawText(rc, Qt::TextWrapAnywhere, xList.at(i));
                    }
                }

            }

            if(m_gridFlag)
            {
                if(i == 0)
                    continue;

                QPen pen;
                pen.setColor("darkCyan");
                pen.setStyle(Qt::DashDotLine);
                //pen.setWidthF(0.01);
                m_painter->setPen(pen);
                m_painter->drawLine(QPointF(x, y), QPointF(x, -m_yRange));
            }
        }

        //y labels
        QStringList yList = m_yLabels.split(',');
        num = yList.size();
        double xMax = x;
        x = m_xPosMin;
        y = 0.0;
        double lastY;

        double endPointY;
        if(m_yLabelsPos.size() > 0)
            endPointY = m_yLabelsPos.at(num-1);
        else
            endPointY = yList.at(num-1).toDouble();
        endPointY = -(endPointY - m_yMin)/m_yValueRange * m_yRange;

        for(int i=0; i<num; i++)
        {
            if(m_yLabelsPos.size() > 0)
                y = m_yLabelsPos.at(i);
            else
                y = yList.at(i).toDouble();

            y = -(y - m_yMin)/m_yValueRange * m_yRange;

            QPen penLine(Qt::black, m_yLabelLinePenSize);
            m_painter->setPen(penLine);
            m_painter->drawLine(QPointF(x, y), QPointF(x-5, y));

            QFont font = m_painter->font();
            font.setPointSizeF(m_yLabelTextPenSize);
            m_painter->setFont(font);          

            QRect rc(x-35, y-7, 50, 50);

            if(i == 0)
            {
                lastY = y;
                m_painter->drawText(rc, Qt::TextWrapAnywhere, yList.at(i));
            }
            else
            {
                QFontMetricsF tmpM(font);
                double h = tmpM.height();

                if(-(y-lastY) < h+5)
                {
                    m_painter->drawText(rc, Qt::TextWrapAnywhere, "");
                }
                else
                {
                    if(-(endPointY-y) < h+5)
                    {
                        if(i == num-1)
                            m_painter->drawText(rc, Qt::TextWrapAnywhere, yList.at(i));
                        else
                            m_painter->drawText(rc, Qt::TextWrapAnywhere, "");
                    }
                    else
                    {
                        lastY = y;
                        m_painter->drawText(rc, Qt::TextWrapAnywhere, yList.at(i));
                    }
                }

            }

            if(m_gridFlag)
            {
                if(i == 0)
                    continue;

                QPen pen;
                //pen.setWidthF(0.01);
                pen.setColor("darkCyan");
                pen.setStyle(Qt::DashDotLine);
                m_painter->setPen(pen);
                //m_painter->drawLine(QPointF(x, y), QPointF(m_xPosMin+m_xRange, y));
                m_painter->drawLine(QPointF(x, y), QPointF(xMax, y));
            }
        }

    }
}

void QAxis::upDateXLabels()
{

    m_xLabels.clear();
    m_xLabelsPos.clear();

    // m_xMin = xMin;
    if( m_xExpandFlag)
    {
        m_xMax = 1.1 * m_xMax;
        m_xExpandFlag = false;
    }

    m_xValueRange = m_xMax - m_xMin;

    m_xLabels.append(QString::number(m_xMax, 'f', 0));

    if(m_xMax != 0)
    {
        m_xLabels.clear();
        double tmp = m_xValueRange/10;
        QString t = QString::number(tmp, 'f', 0);
        tmp = t.toDouble();
        for(int i=0; i<10; i++)
        {
            double tmpX = m_xMin + i*tmp;

            if(tmpX > m_xMax)
                break;

            m_xLabels.append(QString::number(tmpX, 'f', 0) + ",");
        }
        m_xLabels.append(QString::number(m_xMin + 10*tmp, 'f', 0));
    }

    QMap<QString, Curve*>::const_iterator cur;
    for(cur=m_curveList.begin(); cur!=m_curveList.end(); cur++)
    {
        for(int j=0; j<cur.value()->getDataSize(); j++)
        {
            double tmpX = cur.value()->getData(j).x();
            double x = (tmpX - m_xMin)/m_xValueRange * m_xRange + m_xPosMin - 0.5;
            double y = cur.value()->getPosData(j).y();

            cur.value()->resetPosData(j, QPointF(x, y));
        }
    }

}

void QAxis::upDateYLabels()
{

    m_yLabels.clear();
    m_yLabelsPos.clear();

   // m_yMin = yMin;
    if( m_yExpandFlag)
    {
        m_yMax = 1.1 * m_yMax;
        m_yExpandFlag = false;
    }

    m_yValueRange = m_yMax - m_yMin;

    m_yLabels.append(QString::number(m_yMax, 'f', 2));

    if(m_yMax != 0)
    {
        m_yLabels.clear();
        double tmp = m_yValueRange/10;      
        QString t = QString::number(tmp, 'f', 3);
        tmp = t.toDouble();
        for(int i=0; i<10; i++)
        {
            double tmpY = m_yMin + i*tmp;

            if(tmpY > m_yMax)
                break;

            m_yLabels.append(QString::number(tmpY, 'f', 2) + ",");
        }
        m_yLabels.append(QString::number(m_yMin + 10*tmp, 'f', 2));
    }

    QMap<QString, Curve*>::const_iterator cur;
    for(cur=m_curveList.begin(); cur!=m_curveList.end(); cur++)
    {
        for(int j=0; j<cur.value()->getDataSize(); j++)
        {
            QPointF tmp = cur.value()->getPosData(j);
            double x = tmp.x();
            double y = cur.value()->getData(j).y();
            if( m_direction == "down")
                y = (y - m_yMin)/m_yValueRange * m_yRange + m_yPosMin/* - 0.5*/;
            else
                y = -(y - m_yMin)/m_yValueRange * m_yRange /* + m_yPosMin + 0.5 */ ;

            cur.value()->resetPosData(j, QPointF(x, y));
        }
    }

}

void QAxis::addTargetLine(QString lineName)
{
    m_curveList.insert(lineName, new Curve(lineName));
    m_curveList[lineName]->setCurveColor(Qt::red);
    setCurvePointSize(lineName, 0.05);
}
