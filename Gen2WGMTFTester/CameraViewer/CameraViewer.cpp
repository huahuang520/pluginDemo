#include "CameraViewer.h"

CameraViewer::CameraViewer(QWidget* parent)
	: QGraphicsView(parent)
{
    m_scene = new QGraphicsScene();
    m_PixmapItem = new QGraphicsPixmapItem();
    this->setScene(m_scene);
    m_scene->addItem(m_PixmapItem);
    setMouseTracking(true);  //为false时需要点击下鼠标才触发鼠标移动事件

    
}

CameraViewer::~CameraViewer()
{
    
}

void CameraViewer::paintEvent(QPaintEvent* event)
{
    //Call the parent class painting function
    //QGraphicsView::paintEvent(event);
    //QImage srcImg("E:\\Gen2\\gen2-wg-mtf-tester\\Gen2WGMTFTester\\Gen2WGMTFTester\\images\\ShowBackground.png");
    //QSize s = this->size();
    //s.setWidth(s.width() / 2.7);
    //s.setHeight(s.height() / 2.7);
    ////保持纵横比后图像在视图中心，但获取m_PixmapItem坐标依然是(0, 0)，注意这是图像和scene的左上角坐标，如果想要视图坐标需要转化
    //QPixmap pix = QPixmap::fromImage(srcImg).scaled(s, Qt::KeepAspectRatio);
    //m_PixmapItem->setPixmap(pix);
}
