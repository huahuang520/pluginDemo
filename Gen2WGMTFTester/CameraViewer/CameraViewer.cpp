#include "CameraViewer.h"

CameraViewer::CameraViewer(QWidget* parent)
	: QGraphicsView(parent)
{
    m_scene = new QGraphicsScene();
    m_PixmapItem = new QGraphicsPixmapItem();
    this->setScene(m_scene);
    m_scene->addItem(m_PixmapItem);
    setMouseTracking(true);  //Ϊfalseʱ��Ҫ��������Ŵ�������ƶ��¼�

    
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
    ////�����ݺ�Ⱥ�ͼ������ͼ���ģ�����ȡm_PixmapItem������Ȼ��(0, 0)��ע������ͼ���scene�����Ͻ����꣬�����Ҫ��ͼ������Ҫת��
    //QPixmap pix = QPixmap::fromImage(srcImg).scaled(s, Qt::KeepAspectRatio);
    //m_PixmapItem->setPixmap(pix);
}
