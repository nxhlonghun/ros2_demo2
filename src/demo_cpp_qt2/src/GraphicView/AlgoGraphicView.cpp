#include "AlgoGraphicView.h"
#include <QScrollBar>
#include <QDebug>
#include "AlgoGraphicViewControlItem.h"
#include "AlgoGraphicViewBaseItem.h"
#include <QFileDialog>
#include <QAction>
#include <QLabel>
#include "AlgoGraphicViewScene.h"
#include <QHBoxLayout>
#include <QSettings>
#include <QCoreApplication>
#include "AlgoGraphicViewImageItem.h"
#include <QMessageBox>

AlgoGraphicView::AlgoGraphicView(QWidget *parent) : QGraphicsView(parent)
{
	// 解决拖动是背景图片残影
	this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	setDragMode(QGraphicsView::ScrollHandDrag);
	drawBg();
	// 隐藏水平/竖直滚动条
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setBackgroundBrush(Qt::gray);

	// 设置场景范围
	setSceneRect(INT_MIN / 2, INT_MIN / 2, INT_MAX, INT_MAX);
	// setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));

	// 反锯齿
	setRenderHints(QPainter::Antialiasing);
	item = new AlgoGraphicViewImageItem;
	scene = new AlgoGraphicViewScene;
	scene->addItem(item);
	this->setScene(scene);

	drawItem = new DrawItem;
	AddItems(drawItem);

	QAction *action = new QAction(tr("打开图片"), this);
	addAction(action);
	this->connect(action, SIGNAL(triggered(bool)), this, SLOT(onOpenImage()));

	action = new QAction(tr("保存图片"), this);
	addAction(action);
	this->connect(action, SIGNAL(triggered(bool)), this, SLOT(onSaveImage()));

	action = new QAction(tr("计数清零"), this);
	addAction(action);
	this->connect(action, SIGNAL(triggered(bool)), this, SLOT(onClearImageCount()));

	m_showCenterAction = new QAction(tr("显示中心"), this);
	addAction(m_showCenterAction);
	this->connect(m_showCenterAction, SIGNAL(triggered(bool)), this, SLOT(onShowHideCenterCross()));

	action = new QAction(tr("运动到当前坐标"), this);
	addAction(action);
	this->connect(action, SIGNAL(triggered(bool)), this, SLOT(onMoveToCurPos()));

	setContextMenuPolicy(Qt::ActionsContextMenu);

	// 显示灰度值
	GrayValue = new QLabel(this);
	GrayValue->setStyleSheet("color:rgb(50,255,0); background-color:rgba(80,80,80,180); font-size: 16px;font-weight: 400px;");
	GrayValue->setVisible(false);
	GrayValue->setFixedWidth(200);
	// 上方操作 显示区域
	fWidget = new QWidget(this);
	fWidget->setFixedHeight(60);
	auto fLayout = new QHBoxLayout(fWidget);
	fLayout->setSpacing(3);
	// fLayout->setMargin(0);

	auto tmpLabel = new QLabel;
	fLayout->addWidget(GrayValue);
	fLayout->addStretch();
	// 信息背景色
	tmpLabel->setStyleSheet("color:rgb(0,255,0); background-color:rgb(80,80,80); font-size: 18px;font-weight: 400px;");
	tmpLabel->setText("GrabNum :");
	// OK标志
	OKLab = new QLabel(this);
	QFont fontOk("Microsoft YaHei", 30, 75);
	OKLab->setFont(fontOk);
	OKLab->setStyleSheet("color:rgb(0,255,0);");
	OKLab->move(0, 100);
	OKLab->setText("OK");
	OKLab->setVisible(false);
	// NG 标志
	NGLab = new QLabel(this);
	QFont fontNG("Microsoft YaHei", 30, 75);
	NGLab->setFont(fontNG);
	NGLab->setStyleSheet("color:rgb(255,0,0);");
	NGLab->move(0, 40);
	NGLab->setText("NG");
	NGLab->setVisible(false);

	// 矩形框
	for (int i = 0; i < 64; i++)
	{
		QGraphicsRectItem *rectItem = new QGraphicsRectItem();
		m_rectItemsList.append(rectItem);
		rectItem->setPen(QPen(Qt::red, 0.5));
		QColor brushColor(Qt::blue);
		brushColor.setAlpha(50);
		rectItem->setBrush(brushColor);
		rectItem->setVisible(false);
		scene->addItem(rectItem);
	}

	// 圆形框
	for (int i = 0; i < 64; i++)
	{
		QGraphicsEllipseItem *circleItem = new QGraphicsEllipseItem();
		m_circleItemsList.append(circleItem);
		circleItem->setPen(QPen(Qt::red, 0.5));
		QColor brushColor(Qt::blue);
		brushColor.setAlpha(50);
		circleItem->setBrush(brushColor);
		circleItem->setVisible(false);
		scene->addItem(circleItem);
	}

	// 文字
	for (int i = 0; i < 64; i++)
	{
		QGraphicsTextItem *textItem = new QGraphicsTextItem(QString::number(i + 1));
		m_textItemsList.append(textItem);
		textItem->setVisible(false);
		scene->addItem(textItem);
	}

	connect(item, &AlgoGraphicViewImageItem::RGBValue, this, [&](QString InfoVal, QPointF point)
			{
		m_mutex.lock();
		QString info = QString::number(m_imgIndex) + "-"  + InfoVal;
        GrayValue->setText(info);
		m_curMousePos = point;
		m_mutex.unlock(); });

	SetType(false, false);

	m_itemStack.clear();
}

void AlgoGraphicView::ResetRotRect()
{
	for (int i = 0; i < m_rectItemsList.size(); i++)
	{
		if (m_rectItemsList[i])
		{
			m_rectItemsList[i]->setVisible(false);
		}
	}

	m_rectItemIndex = 0;
}

void AlgoGraphicView::ResetCircle()
{
	for (int i = 0; i < m_circleItemsList.size(); i++)
	{
		if (m_circleItemsList[i])
		{
			m_circleItemsList[i]->setVisible(false);
		}
	}

	m_circleItemIndex = 0;
}

void AlgoGraphicView::ResetText()
{
	for (int i = 0; i < m_textItemsList.size(); i++)
	{
		if (m_textItemsList[i])
		{
			m_textItemsList[i]->setVisible(false);
		}
	}

	m_textItemIndex = 0;
}

void AlgoGraphicView::DispRotRect(double x, double y, double angel, double width, double height)
{
	if (m_rectItemIndex >= 64 || m_rectItemIndex < 0)
		return;

	m_rectItemsList[m_rectItemIndex]->setRect(-width / 2.0, -height / 2.0, width, height);
	m_rectItemsList[m_rectItemIndex]->setRotation(angel);
	m_rectItemsList[m_rectItemIndex]->setPos(x, y);
	m_rectItemsList[m_rectItemIndex]->setVisible(true);

	m_rectItemIndex++;
}

void AlgoGraphicView::DispCircle(double x, double y, double radius)
{
	if (m_circleItemIndex >= 64 || m_circleItemIndex < 0)
		return;

	m_circleItemsList[m_circleItemIndex]->setRect(x - radius / 2, y - radius / 2, radius, radius);
	m_circleItemsList[m_circleItemIndex]->setVisible(true);

	m_circleItemIndex++;
}

void AlgoGraphicView::DispText(double x, double y, QString text, int size, QColor color)
{
	if (m_textItemIndex >= 64 || m_textItemIndex < 0)
		return;

	QFont font("Arial", size, QFont::Bold);
	m_textItemsList[m_textItemIndex]->setFont(font);
	m_textItemsList[m_textItemIndex]->setDefaultTextColor(color);

	m_textItemsList[m_textItemIndex]->setPos(x, y);
	m_textItemsList[m_textItemIndex]->setVisible(true);

	m_textItemsList[m_textItemIndex]->setPlainText(text);

	m_textItemIndex++;
}

void AlgoGraphicView::onOpenImage()
{
	QSettings setting(QCoreApplication::applicationDirPath() + "/AlgoGraphicView.ini", QSettings::IniFormat);
	QString fileName = QFileDialog::getOpenFileName(nullptr, tr("加载图片"), setting.value("last_open_path", ".").toString(),
													tr("图片文件(*.bmp;*.png;*.jpg);"));
	if (QFileInfo::exists(fileName))
	{
		setting.setValue("last_open_path", fileName);

		QImage img(fileName);
		DispImage(img);

		// QPointF point(img.width() / 2, img.height() / 2);

		// AlgoGraphicViewBaseItem::setCenterPoint(point, img.width() / 2 - 100);
	}
	else
		return;
}

void AlgoGraphicView::onSaveImage()
{
	QSettings setting(QCoreApplication::applicationDirPath() + "AlgoGraphicView.ini", QSettings::IniFormat);
	QString fileName = QFileDialog::getSaveFileName(nullptr, tr("保存图片"),
													setting.value("last_save_file", ".").toString(),
													tr("图片 (*.bmp;*.png;*.jpg);"));

	if (fileName == "")
		return;

	setting.setValue("last_save_file", fileName);

	if (ImageCatch.save(fileName, "bmp", 100))
	{
		QMessageBox::information(NULL, tr("提示"), tr("保存成功"), QMessageBox::Yes);
	}
}

void AlgoGraphicView::onClearImageCount()
{
	m_mutex.lock();
	m_imgIndex = 0;
	GrayValue->setText(QString::number(m_imgIndex) + "\r\n");
	m_mutex.unlock();
}

void AlgoGraphicView::onMoveToCurPos()
{
	emit moveToPosSignal(m_curMousePos);
}

void AlgoGraphicView::onShowHideCenterCross()
{
	if (AlgoGraphicViewBaseItem::isShowCenter)
	{
		AlgoGraphicViewBaseItem::isShowCenter = false;
		m_showCenterAction->setText("显示中心");
	}
	else
	{
		AlgoGraphicViewBaseItem::isShowCenter = true;
		m_showCenterAction->setText("隐藏中心");
	}
}

void AlgoGraphicView::DispImage(QImage &Image)
{
	//	m_mutex.lock();
	m_imgIndex++;

	ImageCatch = Image.copy();
	image = QPixmap::fromImage(Image);
	item->setPixmap(image);
	GetFit();

	GrayValue->setText(QString::number(m_imgIndex) + "\r\n");

	QPointF point(Image.width() / 2, Image.height() / 2);
	AlgoGraphicViewBaseItem::setCenterPoint(point, Image.width() / 2 - 100);

	//	m_mutex.unlock();
}

void AlgoGraphicView::ClearAll()
{
	drawItem->ClearAll();
	m_rectItemIndex = 0;
}

void AlgoGraphicView::ClearPoint()
{
	drawItem->ClearPoint();
}

void AlgoGraphicView::DispPoint(QVector<QPointF> &list, QColor color)
{
	drawItem->AddPoint(list, color);
}

void AlgoGraphicView::DispLine(QVector<QPointF> &list, QColor color)
{
	drawItem->AddLine(list);
}

void AlgoGraphicView::AddItems(AlgoGraphicViewBaseItem *item)
{
	item->scale = &ZoomValue;
	this->scene->addItem(item);

	item->setItemDiscrib("ROI" + QString::number(m_itemStack.size() + 1));

	m_itemStack.push(item);
}

void AlgoGraphicView::deleteLastItem()
{
	if (m_itemStack.size() <= 0)
		return;

	AlgoGraphicViewBaseItem *item = m_itemStack.pop();
	if (item)
		scene->removeItem(item);
}

// 信息模块设定内容
void AlgoGraphicView::DispInfo(int statue, int Result, QString str, bool All)
{
	if (All)
	{
		((QLabel *)StatueInfo)->setText(str);
		switch (Result)
		{
		case -1:
			NGLab->setVisible(true);
			OKLab->setVisible(false);
			break;
		case 0:
			NGLab->setVisible(false);
			OKLab->setVisible(false);
			break;
		case 1:
			NGLab->setVisible(false);
			OKLab->setVisible(true);
			break;
		default:
			break;
		}
	}
	else
	{
		//        ((LightButton*)StatueLight)->SetStatue(statue);
	}
}
// 设定信息模块是否显示
void AlgoGraphicView::SetType(bool InfoFlg, bool GrayValueFlg)
{
	item->setAcceptHoverEvents(!InfoFlg);
	GrayValue->setVisible(!InfoFlg);
	// fWidget->setVisible(InfoFlg);
}

void AlgoGraphicView::SetToFit()
{
	ZoomFrame(ZoomFit);
	QScrollBar *pHbar = this->horizontalScrollBar();
	pHbar->setSliderPosition(PixX);
	QScrollBar *pVbar = this->verticalScrollBar();
	pVbar->setSliderPosition(PixY);
}

void AlgoGraphicView::ClearObj()
{
	foreach (auto item, scene->items())
	{
		if (item->type() == 10)
		{
			scene->removeItem(item);
		}
	}

	m_itemStack.clear();
}

void AlgoGraphicView::ZoomFrame(double value)
{
	double tmp = value / ZoomValue;
	// 绝对缩放
	ZoomValue *= tmp;
	// 相对上一次缩放
	this->scale(tmp, tmp);
}
void AlgoGraphicView::GetFit()
{
	if (this->width() < 1 || image.width() < 1)
	{
		return;
	}
	// 图片自适应方法
	double winWidth = this->width();
	double winHeight = this->height();
	double ScaleWidth = (image.width() + 1) / winWidth;
	double ScaleHeight = (image.height() + 1) / winHeight;
	double row1, column1;
	double s = 0;
	if (ScaleWidth >= ScaleHeight)
	{

		row1 = -(1) * ((winHeight * ScaleWidth) - image.height()) / 2;
		column1 = 0;
		s = 1 / ScaleWidth;
	}
	else
	{
		row1 = 0;
		column1 = -(1.0) * ((winWidth * ScaleHeight) - image.width()) / 2;
		s = 1 / ScaleHeight;
	}

	if (ZoomFit != s || PixX != column1 * s)
	{
		ZoomFit = s;
		PixX = column1 * s;
		PixY = row1 * s;
		SetToFit();
	}
}

void AlgoGraphicView::drawBg()
{
	bgPix.fill(color1);
	QPainter painter(&bgPix);
	painter.fillRect(0, 0, 32, 32, color2);
	painter.fillRect(32, 32, 32, 32, color2);
	painter.end();
	// this->update();
}

void AlgoGraphicView::mousePressEvent(QMouseEvent *event)
{
	QGraphicsView::mousePressEvent(event);

	emit mouseClicked();

	event->accept();
}
void AlgoGraphicView::resizeEvent(QResizeEvent *event)
{
	GetFit();
	QGraphicsView::resizeEvent(event);

	event->accept();
}
void AlgoGraphicView::mouseReleaseEvent(QMouseEvent *event)
{
	QGraphicsView::mouseReleaseEvent(event);

	event->accept();
}
void AlgoGraphicView::mouseMoveEvent(QMouseEvent *event)
{
	QGraphicsView::mouseMoveEvent(event);
	event->accept();
}
void AlgoGraphicView::mouseDoubleClickEvent(QMouseEvent *event)
{
	SetToFit();
	QGraphicsView::mouseDoubleClickEvent(event);

	event->accept();
}
void AlgoGraphicView::wheelEvent(QWheelEvent *event)
{
	QPoint angle = event->angleDelta() / 8;
	int numDegrees = angle.y();
	int numSteps = numDegrees / 15;

	// 最大放大到原始图像的50倍
	if ((numDegrees > 0) && (ZoomValue >= 50))
	{
		return;
	}
	// 图像缩小到自适应大小之后就不继续缩小
	else if ((numDegrees < 0) && (ZoomValue <= 0.01))
	{
		return;
	}
	else
	{
		double tmp = ZoomValue;
		// 鼠标滚轮向前滚动
		if (numDegrees > 0)
		{
			// 每次放大10%
			tmp *= 1.1;
		}
		else
		{
			// 每次缩小10%
			tmp *= 0.9;
		}
		ZoomFrame(tmp);

		qreal x = event->position().x() - this->width() / 2;
		qreal y = event->position().y() - this->height() / 2;
		// 缩放时 鼠标位置跟随
		if (numDegrees > 0)
		{
			QScrollBar *pHbar = this->horizontalScrollBar();
			pHbar->setSliderPosition(pHbar->sliderPosition() + x * 0.1);
			QScrollBar *pVbar = this->verticalScrollBar();
			pVbar->setSliderPosition(pVbar->sliderPosition() + y * 0.1);
		}
		else
		{
			QScrollBar *pHbar = this->horizontalScrollBar();
			pHbar->setSliderPosition(pHbar->sliderPosition() - x * 0.1);
			QScrollBar *pVbar = this->verticalScrollBar();
			pVbar->setSliderPosition(pVbar->sliderPosition() - y * 0.1);
		}
	}

	event->accept();
}

void AlgoGraphicView::drawBackground(QPainter *painter, const QRectF &rect)
{
	Q_UNUSED(rect);
	// 绘制背景曾
	painter->drawTiledPixmap(rect, bgPix);
}

void AlgoGraphicView::updateSceneRect()
{

	QRectF rectTmp = scene->sceneRect();   // 获得当前view scene的区域
	QPointF ptTopLeft = rectTmp.topLeft(); // 获得区域左上点和右下点坐标
	QPointF ptBottomRight = rectTmp.bottomRight();
	QPointF ptW_H = 0.5 * QPointF(rect().width(), rect().height()); // 这一步主要是为了后边放大区域的范围，0.5这个系数可以修改
	ptTopLeft -= ptW_H;												// 左上点向左上方平移
	ptBottomRight += ptW_H;											// 右下点向右下平移
	rectTmp.setTopLeft(ptTopLeft);									// 按照左上左下点修改rect的大小
	rectTmp.setBottomRight(ptBottomRight);
	scene->setSceneRect(rectTmp); // 更新scene rect的范围
	qDebug() << rectTmp;
}

QPointF AlgoGraphicView::getCurrentViewCenter()
{
	QRect viewport_rect(0, 0, viewport()->width(), viewport()->height());
	QRectF visible_scene_rect = mapToScene(viewport_rect).boundingRect();
	return visible_scene_rect.center();
}
