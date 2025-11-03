#include "AlgoGraphicViewBaseItem.h"
#include <math.h>
#include <QtDebug>

#define PI 3.141592653

bool AlgoGraphicViewBaseItem::isShowCenter = false;
QPointF AlgoGraphicViewBaseItem::centerPoint = QPointF(0,0);
double AlgoGraphicViewBaseItem::centerPointLen = 300;
QColor AlgoGraphicViewBaseItem::centerPointColor = Qt::red;

qreal AlgoGraphicViewBaseItem::ContrSize=8;
void AlgoGraphicViewBaseItem::SetIndex(int num)
{
     if(ControlList.length()>0)
     {
         ControlList[0]->index=num;
     }
}
void AlgoGraphicViewBaseItem::SetScale(double value)
{

}
AlgoGraphicViewBaseItem::AlgoGraphicViewBaseItem(QPointF center,ItemType type):center(center),types(type)
{  
    setHandlesChildEvents(false);//设置后才能将事件传递到子元素
    noSelected.setColor(QColor(0, 100, 200));
    noSelected.setWidth(LineWidth);
    isSelected.setColor(QColor(0, 180, 0));
    isSelected.setWidth(LineWidth);
    fillColor=QColor(0, 160, 230,50);//填充颜色


    thisPen=noSelected;
    if(type!=AutoItem)       //模式选择 自定义模式用于显示亚像素轮廓和Region  不设定任何属性
    {
        this->setFlags(QGraphicsItem::ItemIsSelectable |
                       QGraphicsItem::ItemIsMovable |
                       QGraphicsItem::ItemIsFocusable);
    }
}
void AlgoGraphicViewBaseItem::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    thisPen=isSelected;
    for(int i=1;i<ControlList.length();i++)
    {
        ControlList[i]->setVisible(true);
    }
}
void AlgoGraphicViewBaseItem::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    thisPen=noSelected;
    for(int i=1;i<ControlList.length();i++)
    {
        ControlList[i]->setVisible(false);
    }
}
void AlgoGraphicViewBaseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //缩放控制点尺寸
    for(int i=0;i<ControlList.size();i++)
    {
        ControlList[i]->setScale(1/(*scale));
    }
    //绘制ROI的文本描述
    painter->save();
    painter->setBrush(Qt::NoBrush);
    QFont font;
    font.setPointSizeF(15/(*scale));//字体大小
    painter->setFont(font);
    painter->setPen(Qt::red);
    painter->drawText(ControlList[0]->GetPoint()+QPointF(-ContrSize/(*scale),-(ContrSize+3)/(*scale)),ItemDiscrib);
    painter->restore();
    //子类绘制时状态
    LineWidth=1/(*scale);
    thisPen.setWidthF(LineWidth);
    painter->setPen(thisPen);
}

void AlgoGraphicViewBaseItem::setCenterPoint(QPointF point, double len, QColor color)
{
	centerPoint = point;
	centerPointColor = color;
	centerPointLen = len;
}

void AlgoGraphicViewBaseItem::showCenterPoint(bool value)
{
	isShowCenter = value;
}

//**************************************绘制结果数据***************************************************
DrawItem::DrawItem():AlgoGraphicViewBaseItem(QPointF(0,0),AutoItem)
{
    returnRec=QRectF(0,0,5000,5000);

}
void DrawItem::Refresh()
{
    this->update();
}

void DrawItem::AddRegion(QVector<QPointF>& Plist)
{ 
	ROIList.push_back(Plist);
}

void DrawItem::AddPoint(QVector<QPointF>& Plist, QColor color)
{
	PointList.push_back(Plist);
	PointColorList.push_back(color);
}

void DrawItem::ClearAll()
{ 
	LineList.clear(); 
	ROIList.clear(); 
	PointList.clear();
	PointColorList.clear();
}

void DrawItem::ClearPoint()
{
	PointList.clear();
	PointColorList.clear();
}

QRectF DrawItem::boundingRect() const
{
    //有坑、当不设定Rect时超出区域会发生不显示的情况
    return returnRec;
}
void DrawItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    LineWidth=2/(*scale);
    DrawPen.setWidthF(LineWidth);
    DrawPen.setColor(QColor(255,0,0));
    painter->setPen(DrawPen);

    for(int i=0;i<LineList.size();i++)
    {
        if(LineList[i].size() == 2)
        {
			painter->drawLine(LineList[i][0], LineList[i][1]);
        }
    }

	if (isShowCenter)
	{
		QPointF p = centerPoint;
		QPointF p1 = QPointF(p.x() - centerPointLen, p.y());
		QPointF p2 = QPointF(p.x() + centerPointLen, p.y());

		painter->drawLine(p1, p2);

		QPointF p3 = QPointF(p.x(), p.y() - centerPointLen);
		QPointF p4 = QPointF(p.x(), p.y() + centerPointLen);

		painter->drawLine(p3, p4);

        painter->drawText(p, QString::number(p.x()) + " " + QString::number(p.y()));  
	}

	if (PointList.size() != PointColorList.size())
	{
		return;
	}

    for(int i=0; i< PointList.size(); i++)
    {
		QColor color = PointColorList[i];

		DrawPen.setColor(color);
		painter->setPen(DrawPen);

		if (PointList[i].size() == 2)
		{
			QPointF p = PointList[i][0];
			QPointF param = PointList[i][1];
			qreal len = param.x();
			qreal angel = param.y();

            painter->drawText(p, QString::number(p.x()) + " " + QString::number(p.y()));  

			if (angel == 0)
			{
				QPointF p1 = QPointF(p.x() - len, p.y());
				QPointF p2 = QPointF(p.x() + len, p.y());

				painter->drawLine(p1, p2);

				QPointF p3 = QPointF(p.x(), p.y() - len);
				QPointF p4 = QPointF(p.x(), p.y() + len);

				painter->drawLine(p3, p4);
			}
			else
			{
				QPointF p1 = QPointF(p.x() - len, p.y() - len);
				QPointF p2 = QPointF(p.x() + len, p.y() + len);

				painter->drawLine(p1, p2);

				QPointF p3 = QPointF(p.x() - len, p.y() + len);
				QPointF p4 = QPointF(p.x() + len, p.y() - len);

				painter->drawLine(p3, p4);
			}
		}
    }

}
//**************************************矩形*******************************************************
RectangleItem::RectangleItem(qreal x, qreal y, qreal width, qreal height):AlgoGraphicViewBaseItem(QPointF(x,y),Rectangle)
{
    ControlList<<new AlgoGraphicViewControlItem(this,QPointF(x+width/2,y+height/2),0); // 中心
    ControlList<<new AlgoGraphicViewControlItem(this,QPointF(x,y),1); // 左上
    ControlList<<new AlgoGraphicViewControlItem(this,QPointF(x+width,y),2); // 右上
    ControlList<<new AlgoGraphicViewControlItem(this,QPointF(x+width,y+height),3); // 右下
    ControlList<<new AlgoGraphicViewControlItem(this,QPointF(x,y+height),4); // 左下
}
QRectF RectangleItem::boundingRect() const
{
    return  QRectF(ControlList[1]->GetPoint(),ControlList[3]->GetPoint());
}
bool RectangleItem::UpDate(int index)
{
    QPointF Pf=ControlList[index]->GetPoint();
    //角点分情况 变更
    switch (index)
    {
        case 1:
        ControlList[2]->SetPoint(QPointF(ControlList[2]->GetPoint().x(),Pf.y()));
        ControlList[4]->SetPoint(QPointF(Pf.x(),ControlList[4]->GetPoint().y()));
            break;
        case 2:
        ControlList[1]->SetPoint(QPointF(ControlList[1]->GetPoint().x(),Pf.y()));
        ControlList[3]->SetPoint(QPointF(Pf.x(),ControlList[3]->GetPoint().y()));
            break;
        case 3:
        ControlList[2]->SetPoint(QPointF(Pf.x(),ControlList[2]->GetPoint().y()));
        ControlList[4]->SetPoint(QPointF(ControlList[4]->GetPoint().x(),Pf.y()));
            break;
        case 4:
        ControlList[1]->SetPoint(QPointF(Pf.x(),ControlList[1]->GetPoint().y()));
        ControlList[3]->SetPoint(QPointF(ControlList[3]->GetPoint().x(),Pf.y()));
            break;
        default:
            break;
    }
    //中心点变更
    ControlList[0]->SetPoint(QPointF((ControlList[1]->GetPoint().x()+ControlList[2]->GetPoint().x())/2,(ControlList[2]->GetPoint().y()+ControlList[3]->GetPoint().y())/2));
    return true;
}
void RectangleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    AlgoGraphicViewBaseItem::paint(painter,option,widget);
    QRectF ret(ControlList[1]->GetPoint(),ControlList[3]->GetPoint());
    painter->drawRect(ret);
}
void RectangleItem::SetRect(MRectangle Rec)
{
    QPointF set=this->mapFromScene(Rec.cols,Rec.rows);
    ControlList[0]->SetPoint(set+QPointF(Rec.width/2,Rec.height/2));
    ControlList[1]->SetPoint(set);
    ControlList[2]->SetPoint(set+QPointF(Rec.width,0));
    ControlList[3]->SetPoint(set+QPointF(Rec.width,Rec.height));
    ControlList[4]->SetPoint(set+QPointF(0,Rec.height));
    this->setFocus();
}
void RectangleItem::GetRect(MRectangle& Rec)
{
    QPointF GetRec=this->mapToScene(ControlList[1]->x(),ControlList[1]->y());
    Rec.cols=GetRec.x();
    Rec.rows=GetRec.y();
    Rec.width=ControlList[2]->GetPoint().x()-ControlList[1]->GetPoint().x();
    Rec.height=ControlList[3]->GetPoint().y()-ControlList[1]->GetPoint().y();
}

QJsonObject RectangleItem::writeToJson()
{
	MRectangle rec;
	GetRect(rec);

	QJsonObject obj;
	obj["type"] = "RectangleItem";
	obj["x"] = rec.cols;
	obj["y"] = rec.rows;
	obj["width"] = rec.width;
	obj["height"] = rec.height;
	return obj;
}
//**************************************矩形 旋转*******************************************************
RectangleRItem::RectangleRItem(qreal x, qreal y, qreal Lenth1, qreal Lenth2, qreal Pi):AlgoGraphicViewBaseItem(QPointF(x,y),RectangleR)
{
    angle=Pi;
    lenth1=Lenth1/2;
    lenth2=Lenth2/2;
//    Pa1=center+QPointF(0,lenth2);
//    Pa2=center+QPointF(lenth1,0);
//    Pa3=center+QPointF(0,-lenth2);
//    Pa4=center+QPointF(-lenth1,0);

    qreal s = sin(-angle);
    qreal c = cos(-angle);
    Pa1=center+QPointF( 0 * c - lenth2 * s,0 * s + lenth2 * c);
    Pa2=center+QPointF( lenth1 * c - 0 * s,lenth1 * s + 0 * c);
    Pa3=center+QPointF( 0 * c + lenth2 * s,0 * s - lenth2 * c);
    Pa4=center+QPointF( -lenth1 * c - 0 * s,-lenth1 * s + 0 * c);
    PArrow=center+QPointF( (lenth1+20) * c - 0 * s,(lenth1+20) * s + 0 * c);

    //中心
    ControlList<<new AlgoGraphicViewControlItem(this,center,0);
    //中心线控制点
    ControlList<<new AlgoGraphicViewControlItem(this,Pa1,1);
    ControlList<<new AlgoGraphicViewControlItem(this,Pa2,2);
    ControlList<<new AlgoGraphicViewControlItem(this,Pa3,3);
    ControlList<<new AlgoGraphicViewControlItem(this,Pa4,4);
}
QRectF RectangleRItem::boundingRect() const
{
    qreal tmp=(lenth1>lenth2?lenth2:lenth1);
    return QRectF(center.x()-tmp,center.y()-tmp,tmp*2,tmp*2);
}
bool RectangleRItem::UpDate(int index)
{
    QPointF Pf=ControlList[index]->GetPoint();

    qreal dx = Pf.x() - center.x();
    qreal dy = Pf.y() - center.y();
    if ( dx >= 0 && dy < 0 )
    {
        angle = atan2( (-1)*(dy), dx );
    }
    else if ( dx < 0 && dy < 0 )
    {
        angle = atan2( (-1)*dy, dx );
    }
    else if ( dx < 0 && dy >= 0 )
    {
        angle = PI*2 + atan2( (-1)*dy, dx );
    }
    else if ( dx >= 0 && dy >= 0 )
    {
        angle = PI*2 - atan2( dy, dx ) ;
    }

    //角度补偿 角度方向逆时针 控制点排列方向 顺时针
    switch (index)
    {
        case 1:
        angle+=PI/2;
            break;
        case 3:
        angle+=PI*3/2;
            break;
        case 4:
        angle+=PI;
            break;
    }

    qreal s = sin(-angle);
    qreal c = cos(-angle);
    if(index==2||index==4)
    {
       lenth1=sqrt(dx*dx+dy*dy);
    }else if (index==1||index==3)
    {
       lenth2=sqrt(dx*dx+dy*dy);
    }


    Pa1=center+QPointF( 0 * c - lenth2 * s,0 * s + lenth2 * c);
    Pa2=center+QPointF( lenth1 * c - 0 * s,lenth1 * s + 0 * c);
    Pa3=center+QPointF( 0 * c + lenth2 * s,0 * s - lenth2 * c);
    Pa4=center+QPointF( -lenth1 * c - 0 * s,-lenth1 * s + 0 * c);

    PArrow=center+QPointF( (lenth1+20) * c - 0 * s,(lenth1+20) * s + 0 * c);

    ControlList[1]->SetPoint(Pa1);
    ControlList[2]->SetPoint(Pa2);
    ControlList[3]->SetPoint(Pa3);
    ControlList[4]->SetPoint(Pa4);
    return true;
}
void RectangleRItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    AlgoGraphicViewBaseItem::paint(painter,option,widget);

    //画角度方向线
    painter->drawLine(PArrow,Pa2);
    //绘制方向箭头
    float l = 15.0;//箭头长度
    double atn1=atan2((PArrow.y() - Pa2.y()) , (PArrow.x() - Pa2.x()));
    double atn2=atan2((PArrow.x() - Pa2.x()) , (PArrow.y() - Pa2.y()));
    QPointF Arrow1(PArrow.x() - l * cos(atn1 - 0.5),PArrow.y() - l * sin(atn1 - 0.5));
    QPointF Arrow2(PArrow.x() - l * sin(atn2 - 0.5),PArrow.y() - l * cos(atn2 - 0.5));
    painter->drawLine(PArrow,Arrow1);
    painter->drawLine(PArrow,Arrow2);

    //绘制旋转矩形
    painter->save();
    painter->translate(center);
    painter->rotate(-angle*180/PI);
    //painter-> drawEllipse(QRectF(-lenth1,-lenth2,lenth1*2,lenth2*2));
    painter->drawRect(QRectF(-lenth1,-lenth2,lenth1*2,lenth2*2));
    painter->restore();

}

QJsonObject RectangleRItem::writeToJson()
{
	QPointF GetRec = mapToScene(center.x(), center.y());

	QJsonObject obj;
	obj["type"] = "RectangleRItem";
	obj["x"] = GetRec.x();
	obj["y"] = GetRec.y();
	obj["len1"] = lenth1;
	obj["len2"] = lenth2;
	obj["angel"] = angle;
	return obj;
}

//**************************************椭圆*******************************************************
EllipseItem::EllipseItem(qreal x, qreal y, qreal Lenth1, qreal Lenth2, qreal Pi):RectangleRItem(x,y,Lenth1,Lenth2,Pi)
{

}
void EllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    AlgoGraphicViewBaseItem::paint(painter,option,widget);

    painter->drawLine(center,Pa2);
    painter->save();
    painter->translate(center);
    painter->rotate(-angle*180/PI);
    painter-> drawEllipse(QRectF(-lenth1,-lenth2,lenth1*2,lenth2*2));
    painter->restore();
}
//**************************************圆*******************************************************
CircleItem::CircleItem(qreal x, qreal y, qreal R):AlgoGraphicViewBaseItem(QPointF(x,y),Circle)
{
    Radius=R;
    ControlList<<new AlgoGraphicViewControlItem(this,center,0);
    ControlList<<new AlgoGraphicViewControlItem(this,center+QPointF(R,0),1);
}
QRectF CircleItem::boundingRect() const
{
    return QRectF(center.x() - Radius, center.y() - Radius, Radius * 2, Radius * 2);
}
bool CircleItem::UpDate(int index)
{
    QPointF Pf=ControlList[index]->GetPoint();
    QPointF tmp=Pf-center;
    Radius=sqrt(tmp.x()*tmp.x()+tmp.y()*tmp.y());
    return true;
}
void CircleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    AlgoGraphicViewBaseItem::paint(painter,option,widget);
    painter-> drawEllipse(QRectF(center.x()-Radius,center.y()-Radius,Radius*2,Radius*2));
}
QJsonObject CircleItem::writeToJson()
{
	QPointF newCenter = this->mapToScene(center.x(), center.y());

	QJsonObject obj;
	obj["type"] = "CircleItem";
	obj["x"] = newCenter.x();
	obj["y"] = newCenter.y();
	obj["radius"] = Radius;
	return obj;
}
//**************************************同心圆*********************************************************
ConcentricCircleItem::ConcentricCircleItem(qreal x, qreal y, qreal radiusMin, qreal radiusMax):AlgoGraphicViewBaseItem(QPointF(x,y),Concentric_Circle)
{
    RadiusMax=radiusMax;
    RadiusMin=radiusMin>radiusMax?radiusMax:radiusMin;
    ControlList<<new AlgoGraphicViewControlItem(this,center,0);
    ControlList<<new AlgoGraphicViewControlItem(this,center+QPointF(RadiusMin,0),1);
    ControlList<<new AlgoGraphicViewControlItem(this,center+QPointF(RadiusMax,0),2);
}
QRectF ConcentricCircleItem::boundingRect() const
{
    return QRectF(center.x() - RadiusMax, center.y() - RadiusMax, RadiusMax * 2, RadiusMax * 2);
}
bool ConcentricCircleItem::UpDate(int index)
{
    QPointF Pf=ControlList[index]->GetPoint();
    QPointF tmp=Pf-center;
    qreal R=sqrt(tmp.x()*tmp.x()+tmp.y()*tmp.y());
    if(index==1)
    {
        if(R>RadiusMax)
        return false;
        RadiusMin=R;
    }else if(index==2)
    {
        if(R<RadiusMin)
        return false;
        RadiusMax=R;
    }
    return true;
}
void ConcentricCircleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    AlgoGraphicViewBaseItem::paint(painter,option,widget);

    QPainterPath pth;
    pth.addEllipse(QRectF(center.x()-RadiusMin,center.y()-RadiusMin,RadiusMin*2,RadiusMin*2));
    pth.addEllipse(QRectF(center.x()-RadiusMax,center.y()-RadiusMax,RadiusMax*2,RadiusMax*2));
    painter->setBrush(QBrush(fillColor));
    painter->drawPath(pth);
}
QJsonObject ConcentricCircleItem::writeToJson()
{
	QPointF newCenter = this->mapToScene(center.x(), center.y());

	QJsonObject obj;
	obj["type"] = "ConcentricCircleItem";
	obj["x"] = newCenter.x();
	obj["y"] = newCenter.y();
	obj["radius_min"] = RadiusMin;
	obj["radius_max"] = RadiusMax;
	return obj;
}
//**************************************饼图********细节需要优化*************************************************
PieItem::PieItem(qreal x, qreal y, qreal radius, qreal startAng, qreal endAng):AlgoGraphicViewBaseItem(QPointF(x,y),Pie)
{
    Radius=radius;
    StartAng=startAng;
    EndAng=endAng; //endAng>StartAng?StartAng:
    qreal ss = sin(StartAng);
    qreal cs = cos(StartAng);
    qreal se = sin(EndAng);
    qreal ce = cos(EndAng);
    ControlList<<new AlgoGraphicViewControlItem(this,center,0);
    ControlList<<new AlgoGraphicViewControlItem(this,center+QPointF(Radius * cs,-Radius * ss),1);
    ControlList<<new AlgoGraphicViewControlItem(this,center+QPointF(Radius * ce,-Radius * se),2);
}
QRectF PieItem::boundingRect() const
{
    return QRectF(center.x() - Radius, center.y() - Radius, Radius * 2, Radius * 2);
}
bool PieItem::UpDate(int index)
{
    QPointF Pf=ControlList[index]->GetPoint();

    qreal dx = Pf.x() - center.x();
    qreal dy = Pf.y() - center.y();
    Radius=sqrt(dx*dx+dy*dy);
    qreal atmp=0;
    if ( dx >= 0 && dy < 0 )
    {
        atmp = atan2( (-1)*(dy), dx );
    }
    else if ( dx < 0 && dy < 0 )
    {
        atmp = atan2( (-1)*dy, dx );
    }
    else if ( dx < 0 && dy >= 0 )
    {
        atmp = PI*2 + atan2( (-1)*dy, dx );
    }
    else if ( dx >= 0 && dy >= 0 )
    {
        atmp = PI*2 - atan2( dy, dx );
    }

    if(index==1)
    {
//        if(atmp<EndAng)
//            return false;
        StartAng=atmp;
    }else if(index==2)
    {
//        if(atmp>StartAng)
//            return false;
        EndAng=atmp;
    }

    qreal ss = sin(StartAng);
    qreal cs = cos(StartAng);
    qreal se = sin(EndAng);
    qreal ce = cos(EndAng);

    ControlList[1]->SetPoint(center+QPointF(Radius * cs,-Radius * ss));
    ControlList[2]->SetPoint(center+QPointF(Radius * ce,-Radius * se));
    return true;
}
void PieItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    AlgoGraphicViewBaseItem::paint(painter,option,widget);
    QRectF ret(center.x() - Radius, center.y() - Radius, Radius * 2, Radius * 2);
    painter->drawPie(ret, 16*EndAng*180/PI, 16*((StartAng-EndAng)*180/PI));
}
//**************************************多边形*********************************************************
PolygonItem::PolygonItem():AlgoGraphicViewBaseItem(QPointF(0,0),Polygon)
{
    ControlList<<new AlgoGraphicViewControlItem(this,center,0);
    ControlList[0]->setVisible(false);
    Finished=false;
}
QPointF PolygonItem::getCentroid(QList<QPointF> list)
{
    qreal x = 0;
    qreal y = 0;
    for (auto &temp : list)
    {
        x += temp.x();
        y += temp.y();
    }
    x = x/list.size();
    y = y/list.size();
    return QPointF(x,y);
}
void PolygonItem::getMaxLength()
{
    QVector<qreal> vec;
    for (int i=1;i<ControlList.length();i++)
    {
        qreal dis = sqrt(pow(center.x() - ControlList[i]->x(), 2) + pow(center.y() - ControlList[i]->y(), 2));
        vec.append(dis);
    }

    qreal ret = 0;
    for (auto &temp : vec)
    {
        if (temp > ret) {
            ret = temp;
        }
    }
    Radius = ret;
}
void PolygonItem::pushPoint(QPointF p, QList<QPointF> list, bool isCenter)
{
    if (!Finished)
    {
        //this->setSelected(true);
        center = getCentroid(list);
        getMaxLength();
        if (isCenter)
        {
            ControlList[0]->SetPoint(center);
            ControlList[0]->setVisible(true);
            Finished=true;
        } else
        {
            auto tmp=new AlgoGraphicViewControlItem(this,p,ControlList.length());
            tmp->setVisible(true);
            ControlList<<tmp;
        }
        this->update();
    }
}
QRectF PolygonItem::boundingRect() const
{
    return QRectF(center.x() - Radius, center.y() - Radius, Radius * 2, Radius * 2);
}
bool PolygonItem::UpDate(int index)
{
    Q_UNUSED(index);
    QList<QPointF> list;
    for(int i=1;i<ControlList.length();i++)
    {
        list<<ControlList[i]->GetPoint();
    }
    center = getCentroid(list);
    ControlList[0]->SetPoint(center);
    return true;
}
void PolygonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    AlgoGraphicViewBaseItem::paint(painter,option,widget);

    if (Finished)
    {
        for (int i = 1; i < ControlList.length() - 1; i++)
        {
            painter->drawLine(ControlList[i]->GetPoint(), ControlList[i+1]->GetPoint());
        }
        painter->drawLine(ControlList[ControlList.length() - 1]->GetPoint(), ControlList[1]->GetPoint());
    } else
    {
        for (int i = 1; i < ControlList.length() - 1; i++)
        {
            painter->drawLine(ControlList[i]->GetPoint(), ControlList[i+1]->GetPoint());
        }
    }
}
//**************************************圆端矩形*********************************************************
RoundRectangle1Item::RoundRectangle1Item(qreal x, qreal y, qreal width, qreal height):AlgoGraphicViewBaseItem(QPointF(0,0),Round_End_Rectangle)
{
    ControlList<<new AlgoGraphicViewControlItem(this,center,0);
    ControlList<<new AlgoGraphicViewControlItem(this,center+QPointF(width/2,height/2),1);

    if(x>0||y>0)
    {
        ControlList[0]->moveBy(x, y);
    }
}
QRectF RoundRectangle1Item::boundingRect() const
{
    auto m_edge=ControlList[1]->GetPoint();
    QRectF ret = QRectF(center.x() - m_edge.x() - m_edge.y(),center.y() - m_edge.y(),
                        abs(m_edge.x()) * 2 + abs(m_edge.y()) * 2,abs(m_edge.y()) * 2);
    if ( m_edge.x() >= 0 && m_edge.y() < 0 )
    {
        ret.moveTo(center.x() - m_edge.x() + m_edge.y(), center.y() + m_edge.y());
    }
    else if ( m_edge.x() < 0 && m_edge.y() < 0 )
    {
        ret.moveTo(center.x() + m_edge.x() + m_edge.y(), center.y() + m_edge.y());
    }
    else if ( m_edge.x() < 0 && m_edge.y() >= 0 )
    {
        ret.moveTo(center.x() + m_edge.x() - m_edge.y(), center.y() - m_edge.y());
    }
    else if ( m_edge.x() >= 0 && m_edge.y() >=0 )
    {
        ret.moveTo(center.x() - m_edge.x() - m_edge.y(), center.y() - m_edge.y());
    }

    return ret;
}
bool RoundRectangle1Item::UpDate(int index)
{
    Q_UNUSED(index);
    return true;
}
void RoundRectangle1Item::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    AlgoGraphicViewBaseItem::paint(painter,option,widget);

    QPointF left_top, left_bottom, right_top, right_bottom;

    auto m_edge=ControlList[1]->GetPoint();
    if ( m_edge.x() >= 0 && m_edge.y() < 0 )
    {
        left_top = QPointF(center.x() - m_edge.x() + m_edge.y(), m_edge.y());
        left_bottom = QPointF(center.x() - m_edge.x() + m_edge.y(), (-1) * m_edge.y());
        right_top = QPointF(center.x() + m_edge.x() - m_edge.y(), m_edge.y());
        right_bottom = QPointF(center.x() + m_edge.x() - m_edge.y(), (-1) * m_edge.y());
    }
    else if ( m_edge.x() < 0 && m_edge.y() < 0 )
    {
        left_top = QPointF(center.x() + m_edge.x() + m_edge.y(), m_edge.y());
        left_bottom = QPointF(center.x() + m_edge.x() + m_edge.y(), (-1) * m_edge.y());
        right_top = QPointF(center.x() - m_edge.x() - m_edge.y(), m_edge.y());
        right_bottom = QPointF(center.x() - m_edge.x() - m_edge.y(), (-1) * m_edge.y());
    }
    else if ( m_edge.x() < 0 && m_edge.y() >= 0 )
    {
        left_top = QPointF(center.x() + m_edge.x() - m_edge.y(), (-1) * m_edge.y());
        left_bottom = QPointF(center.x() + m_edge.x() - m_edge.y(), m_edge.y());
        right_top = QPointF(center.x() - m_edge.x() + m_edge.y(), (-1) * m_edge.y());
        right_bottom = QPointF(center.x() - m_edge.x() + m_edge.y(), m_edge.y());
    }
    else if ( m_edge.x() >= 0 && m_edge.y() >=0 )
    {
        left_top = QPointF(center.x() - m_edge.x() - m_edge.y(), (-1) * m_edge.y());
        left_bottom = QPointF(center.x() - m_edge.x() - m_edge.y(), m_edge.y());
        right_top = QPointF(center.x() + m_edge.x() + m_edge.y(), (-1) * m_edge.y());
        right_bottom = QPointF(center.x() + m_edge.x() + m_edge.y(), m_edge.y());
    }

    int radius = abs(m_edge.y());
    QPointF deltax(radius, 0);
    QPointF deltay(0, radius);

    painter->drawLine(left_top + deltax, right_top - deltax);
    painter->drawLine(left_bottom + deltax, right_bottom - deltax);
    painter->drawLine(left_top + deltay, left_bottom - deltay);
    painter->drawLine(right_top + deltay, right_bottom - deltay);

    painter->drawArc(QRectF(left_top, QSizeF(radius*2, radius*2)), -180 * 16, -90 * 16);
    painter->drawArc(QRectF(left_bottom-deltay*2, QSizeF(radius*2, radius*2)), -90 * 16, -90 * 16);
    painter->drawArc(QRectF(right_top-deltax*2, QSizeF(radius*2, radius*2)), 0 * 16, 90 * 16);
    painter->drawArc(QRectF(right_bottom-deltax*2-deltay*2, QSizeF(radius*2, radius*2)), 0 * 16, -90 * 16);
}
//**************************************圆角矩形*********************************************************
RoundRectangle2Item::RoundRectangle2Item(qreal x, qreal y, qreal width, qreal height, qreal radius):AlgoGraphicViewBaseItem(QPointF(0,0),Rounded_Rectangle)
{
    Width=width;
    Height=height;
    Radius=radius;
    ControlList<<new AlgoGraphicViewControlItem(this,center,0);
    ControlList<<new AlgoGraphicViewControlItem(this,center+QPointF(width/2,height/2),1);
    ControlList<<new AlgoGraphicViewControlItem(this,center+QPointF(width/2,-height/2),2);

    if(x>0||y>0)
    {
        ControlList[0]->moveBy(x, y);
    }
}
QRectF RoundRectangle2Item::boundingRect() const
{
    return QRectF(center-QPointF(Width/2,Height/2),center+QPointF(Width/2,Height/2));
}
bool RoundRectangle2Item::UpDate(int index)
{
    QPointF Pf=ControlList[1]->GetPoint();
    QPointF Pfc=ControlList[2]->GetPoint();

    if(index==1)
    {
        //设置1号控制点
        qreal tmpx=Pf.x() <= Radius?Radius:Pf.x();
        qreal tmpy=Pf.y() <= Radius?Radius:Pf.y();
        ControlList[1]->SetPoint(QPointF(tmpx,tmpy));

        //更新尺寸
        Width=qAbs(tmpx-center.x())*2;
        Height=qAbs(tmpy-center.y())*2;

        //控制点2位置被动更新 需要用旧的位置 懒的修改了 有时间再改
        qreal retX = 0;
        qreal retY = 0;
        if ( tmpx == Pfc.x() )
        {
            retX = tmpx;
            retY = -tmpy+Radius;
        } else {
            retX = tmpx - Radius;
            retY = -tmpy;
        }
        ControlList[2]->SetPoint(QPointF(retX,retY));
    }
    if(index==2)
    {
        //获取控制点2坐标
        Pfc=ControlList[2]->GetBefPoint();
        qreal dx=ControlList[2]->dX();
        qreal dy=ControlList[2]->dY();
        qreal retX = 0;
        qreal retY = 0;
        if ( Pf.x() == Pfc.x() &&(-1) * Pf.y() != Pfc.y() )
        {
            retX = Pfc.x();
            retY = Pfc.y() + dy;
        }
        else if ( Pf.x() != Pfc.x() &&(-1) * Pf.y() == Pfc.y() )
        {
            retX = Pfc.x() + dx;
            retY = Pfc.y();
        }
        else if ( Pf.x() == Pfc.x() &&(-1) * Pf.y() == Pfc.y() )
        {
            if ( abs(dx) >= abs(dy) )
            {
                retX = Pfc.x() + dx;
                retY = Pfc.y();
            } else {
                retX = Pfc.x();
                retY = Pfc.y() + dy;
            }
        }
        if ( retX > Pf.x() )
        {
            retX = Pf.x();
        } else if ( retX < 0 )
        {
            retX = 0;
        }
        if ( retY < (-1) * Pf.y() )
        {
            retY = (-1) * Pf.y();
        } else if ( retY > 0 )
        {
            retY = 0;
        }


        //设定控制点2
        ControlList[2]->SetPoint(QPointF(retX,retY));
        {
            //更新半径
            qreal dx = 0;
            qreal dy = 0;
            qreal absX = abs(Pf.x());
            qreal absY = abs(Pf.y());

            if ( Pfc.x() >= 0 && Pfc.y() < 0 )
            {
                dx = absX - Pfc.x();
                dy = absY + Pfc.y();
            }
            else if ( Pfc.x() < 0 && Pfc.y() < 0 )
            {
                dx = absX + Pfc.x();
                dy = absY + Pfc.y();
            }
            else if ( Pfc.x() < 0 && Pfc.y() >= 0 )
            {
                dx = absX + Pfc.x();
                dy = absY - Pfc.y();
            }
            else if ( Pfc.x() >= 0 && Pfc.y() >= 0 )
            {
                dx = absX - Pfc.x();
                dy = absY - Pfc.y();
            }
            Radius = dx >= dy ? dx : dy;
            Radius=Radius<0?0:Radius;
        }
    }

    return true;
}
void RoundRectangle2Item::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    AlgoGraphicViewBaseItem::paint(painter,option,widget);

    QRectF ret(center-QPointF(Width/2,Height/2),center+QPointF(Width/2,Height/2));
    painter->drawRoundedRect(ret, Radius, Radius);
}
//**************************************直线*********************************************************
LineItem::LineItem(qreal x1, qreal y1, qreal x2, qreal y2):AlgoGraphicViewBaseItem(QPointF((x1+x2)/2,(y1+y2)/2),LineObj)
{
    ControlList<<new AlgoGraphicViewControlItem(this,center,0);
    ControlList<<new AlgoGraphicViewControlItem(this,QPointF(x1,y1),1);
    ControlList<<new AlgoGraphicViewControlItem(this,QPointF(x2,y2),2);
    P1=QPointF(x1,y1);
    P2=QPointF(x2,y2);
}
bool LineItem::UpDate(int index)
{
    if(index==1)
    {
        P1=ControlList[1]->GetPoint();
    }else if(index==2)
    {
        P2=ControlList[2]->GetPoint();
    }
    ControlList[0]->SetPoint(QPointF((P1.x()+P2.x())/2,(P1.y()+P2.y())/2));
    return true;
}
void LineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    AlgoGraphicViewBaseItem::paint(painter,option,widget);
    painter->drawLine(P1,P2);
}
//**************************************卡尺*********************************************************
Caliper::Caliper(qreal x1, qreal y1, qreal x2, qreal y2, qreal height):AlgoGraphicViewBaseItem(QPointF((x1+x2)/2,(y1+y2)/2),LineObj)
{
    P1=QPointF(x1,y1);
    P2=QPointF(x2,y2);
    P1Glob=P1;
    P2Glob=P2;
    Height=height;
    ControlList<<new AlgoGraphicViewControlItem(this,center,0);
    ControlList<<new AlgoGraphicViewControlItem(this,QPointF(x1,y1),1);
    ControlList<<new AlgoGraphicViewControlItem(this,QPointF(x2,y2),2);

    qreal dx = P1.x() - P2.x();
    qreal dy = P1.y() - P2.y();
    if ( dx >= 0 && dy < 0 )
    {
        angle = atan2( (-1)*(dy), dx );
    }
    else if ( dx < 0 && dy < 0 )
    {
        angle = atan2( (-1)*dy, dx );
    }
    else if ( dx < 0 && dy >= 0 )
    {
        angle = PI*2 + atan2( (-1)*dy, dx );
    }
    else if ( dx >= 0 && dy >= 0 )
    {
        angle = PI*2 - atan2( dy, dx ) ;
    }
    Lenth=sqrt(dx*dx+dy*dy);
    qreal s = sin(angle);
    qreal c = cos(angle);

    ControlList<<new AlgoGraphicViewControlItem(this,center+QPointF(s*Height/2,c*Height/2),3);
}
void Caliper::GetPosition(qreal &x1, qreal &y1, qreal &x2, qreal &y2, qreal &height)
{
    x1=P1Glob.x();
    y1=P1Glob.y();
    x2=P2Glob.x();
    y2=P2Glob.y();
    height=Height;
}
QRectF Caliper::boundingRect() const
{
    int min=Lenth<Height?Lenth:Height;
    int size =min<5?5:min;
    QPointF c=(P1+P2)/2;
    return QRectF(c.x() - size, c.y() - size, size, size);
}
bool Caliper::UpDate(int index)
{
    if(index==1)
    {
        P1=ControlList[1]->GetPoint();
    }else if(index==2)
    {
        P2=ControlList[2]->GetPoint();
    }
    else if(index==3)
    {
        QPointF Pf=ControlList[3]->GetPoint();
        qreal dx = Pf.x() - center.x();
        qreal dy = Pf.y() - center.y();
        Height=sqrt(dx*dx+dy*dy)*2;
    }
    center=QPointF((P1.x()+P2.x())/2,(P1.y()+P2.y())/2);
    ControlList[0]->SetPoint(center);
    qreal dx = P1.x() - P2.x();
    qreal dy = P1.y() - P2.y();

    if ( dx >= 0 && dy < 0 )
    {
        angle = atan2( (-1)*(dy), dx );
    }
    else if ( dx < 0 && dy < 0 )
    {
        angle = atan2( (-1)*dy, dx );
    }
    else if ( dx < 0 && dy >= 0 )
    {
        angle = PI*2 + atan2( (-1)*dy, dx );
    }
    else if ( dx >= 0 && dy >= 0 )
    {
        angle = PI*2 - atan2( dy, dx ) ;
    }
    qreal s = sin(angle);
    qreal c = cos(angle);
    ControlList[3]->SetPoint(center+QPointF(s*Height/2,c*Height/2));
    Lenth=sqrt(dx*dx+dy*dy);


    return true;
}
void Caliper::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    AlgoGraphicViewBaseItem::paint(painter,option,widget);
    painter->drawLine(P1,P2);

    P1Glob=this->mapToParent(P1);
    P2Glob=this->mapToParent(P2);

    //绘制方向箭头
    float l = 15.0;//箭头长度
    double atn1=atan2((P2.y() - P1.y()) , (P2.x() - P1.x()));
    double atn2=atan2((P2.x() - P1.x()) , (P2.y() - P1.y()));
    QPointF Arrow1(P2.x() - l * cos(atn1 - 0.5),P2.y() - l * sin(atn1 - 0.5));
    QPointF Arrow2(P2.x() - l * sin(atn2 - 0.5),P2.y() - l * cos(atn2 - 0.5));
    painter->drawLine(P2,Arrow1);
    painter->drawLine(P2,Arrow2);

    painter->save();
    painter->translate(center);
    painter->setBrush(QBrush(fillColor));
    painter->rotate(-angle*180/PI);
    QPen tp=painter->pen();
    tp.setWidthF(0);
    tp.setColor(fillColor);
    painter->setPen(tp);
    thisPen.setWidthF(0);
    painter->drawRect(-Lenth/2,-abs(Height/2),Lenth,abs(Height));
    painter->restore();

}




