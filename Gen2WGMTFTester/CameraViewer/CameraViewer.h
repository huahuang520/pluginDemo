#pragma once

#include <QWidget>
#include "cameraviewer_global.h"
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QGraphicsItem>
#include <QStandardPaths>
#include <QPixmap>
#include <QContextMenuEvent>
#include <QMenu>
#include <QFileDialog>
#include <QDebug>
//#include "DrawROI.h"
//#include "GraphicsViewTool.h"

class CAMERAVIEWER_EXPORT CameraViewer : public QGraphicsView
{
    Q_OBJECT
public:
    CameraViewer(QWidget* parent = nullptr);
    ~CameraViewer();

protected:
    void paintEvent(QPaintEvent* event);

private:
    QGraphicsScene* m_scene;
    QGraphicsPixmapItem* m_PixmapItem;
};
