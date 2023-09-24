#include "ConfigPage.h"
#include <QIcon>
#include <QDialog>
#include "ConfigurationPlugin.h"
#include "PluginCore.h"
ConfigPage::ConfigPage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	m_isConfigPageClosed = true;
	InitUI();
}

ConfigPage::~ConfigPage()
{}

void ConfigPage::IsConfigPageClosed(int close)
{
	if (!close)
		m_isConfigPageClosed = true;
}

void ConfigPage::InitUI()
{
	ui.config->setIcon(QIcon(":/Gen2WGMTFTester/images/routine/configFile.png"));
	ui.config->setText("config");
	ui.config->setIconSize(QSize(32,32));
	ui.config->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	ui.config->setStyleSheet("QToolButton{border: none; color: red;}"
		"QToolButton::hover{color: blue;background: rgb(198,198,198);}"
		//"QToolButton::pressed{color: gray;}"
	);
	ui.config->setToolTip("Modify all configuration information");
}

void ConfigPage::on_config_clicked()
{
	ConfigurationPlugin* configurationPlugin = PluginCore::getinstance()->GetPlugin<ConfigurationPlugin>("ConfigurationPlugin");
	if (configurationPlugin && m_isConfigPageClosed)
	{
		QDialog* dialog = new QDialog();
		m_isConfigPageClosed = false;
		connect(dialog, SIGNAL(finished(int)), this, SLOT(IsConfigPageClosed(int)));
		//dialog->setAttribute(Qt::WA_DeleteOnClose); 
		dialog->setWindowFlags(Qt::Dialog | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
		QVBoxLayout* layout = new QVBoxLayout(dialog);
		dialog->setLayout(layout);
		layout->setMargin(0);
		//layout->setContentsMargins(0,0,0,0);
		layout->addWidget(configurationPlugin->GetWidget());

		dialog->setWindowTitle(configurationPlugin->GetWidget()->windowTitle());
		dialog->resize(500, 500);
		dialog->show();
	}
	
}