#include "ConfigWidget.h"
#include <QTableWidgetItem>
#include <QFile>
#include <QTextStream>
ConfigWidget::ConfigWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.LevelAdjustTable->verticalHeader()->setVisible(true);

	QString qss;
	QFile file(":/Gen2WGMTFTester/css/style.css");

	if (file.open(QFile::ReadOnly)) {
		// 用QTextStream读取样式文件不用区分文件编码 带bom也行
		QStringList list;
		QTextStream in(&file);
		while (!in.atEnd()) {
			QString line;
			in >> line;
			list << line;
		}

		file.close();
		qss = list.join("\n");
		QString paletteColor = qss.mid(20, 7);
		ui.LevelAdjustTable->setPalette(QPalette(paletteColor));
		// 用时主要在下面这句
		ui.LevelAdjustTable->setStyleSheet(qss);
	}
}

ConfigWidget::~ConfigWidget()
{}

void ConfigWidget::ReadLevelAdjustConfig(AllConfig allConfig)
{
	m_allConfig = allConfig;
	LevelAdjustConfig levelAdjustConfig = allConfig.levelAdjustConfig;
	ui.LevelAdjustTable->setItem(0, 0, new QTableWidgetItem(QString::number(levelAdjustConfig.motion3DX), Qt::AlignCenter));
	ui.LevelAdjustTable->item(0, 0)->setTextAlignment(Qt::AlignCenter);
	ui.LevelAdjustTable->setItem(0, 1, new QTableWidgetItem(QString::number(levelAdjustConfig.motion3DY), Qt::AlignCenter));
	ui.LevelAdjustTable->item(0, 1)->setTextAlignment(Qt::AlignCenter);
	ui.LevelAdjustTable->setItem(0, 2, new QTableWidgetItem(QString::number(levelAdjustConfig.motion3DZ), Qt::AlignCenter));
	ui.LevelAdjustTable->item(0, 2)->setTextAlignment(Qt::AlignCenter);
	ui.LevelAdjustTable->setItem(1, 0, new QTableWidgetItem(QString::number(levelAdjustConfig.angleRange), Qt::AlignCenter));
	ui.LevelAdjustTable->item(1, 0)->setTextAlignment(Qt::AlignCenter);
	ui.LevelAdjustTable->setItem(2, 0, new QTableWidgetItem(QString::number(levelAdjustConfig.adjustTimesMax), Qt::AlignCenter));
	ui.LevelAdjustTable->item(2, 0)->setTextAlignment(Qt::AlignCenter);
	ui.LevelAdjustTable->setItem(3, 0, new QTableWidgetItem(QString::number(levelAdjustConfig.pulseConversionAngle_X, 'f', 8), Qt::AlignCenter));
	ui.LevelAdjustTable->item(3, 0)->setTextAlignment(Qt::AlignCenter);
	ui.LevelAdjustTable->setItem(4, 0, new QTableWidgetItem(QString::number(levelAdjustConfig.pulseConversionAngle_Y, 'f', 9),  Qt::AlignCenter));
	ui.LevelAdjustTable->item(4, 0)->setTextAlignment(Qt::AlignCenter);
	connect(ui.LevelAdjustTable, SIGNAL(cellChanged(int, int)), this, SLOT(updateLevelAdjustConfig(int, int)));
}

void ConfigWidget::updateLevelAdjustConfig(int currentRow, int currentColumn)
{
	LevelAdjustConfig levelAdjustConfig;
	levelAdjustConfig.motion3DX = ui.LevelAdjustTable->item(0, 0)->text().toDouble();
	levelAdjustConfig.motion3DY = ui.LevelAdjustTable->item(0, 1)->text().toDouble();
	levelAdjustConfig.motion3DZ = ui.LevelAdjustTable->item(0, 2)->text().toDouble();
	levelAdjustConfig.angleRange = ui.LevelAdjustTable->item(1, 0)->text().toInt();
	levelAdjustConfig.adjustTimesMax = ui.LevelAdjustTable->item(2, 0)->text().toInt();
	levelAdjustConfig.pulseConversionAngle_X = ui.LevelAdjustTable->item(3, 0)->text().toDouble();
	levelAdjustConfig.pulseConversionAngle_Y = ui.LevelAdjustTable->item(4, 0)->text().toDouble();
	m_allConfig.levelAdjustConfig = levelAdjustConfig;

	emit updateAllConfig(m_allConfig, ConfigMode::LevelAdjust_Config);
}