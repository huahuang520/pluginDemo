#include "ToolPage.h"
#include "TabBarPlugin.h"
#include "PluginCore.h"
#include <QTabBar>

ToolPage::ToolPage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initToolPage();
}

ToolPage::~ToolPage()
{
}

void ToolPage::setToolBoxState()
{
	disconnect(ui.LevelAdjust, SIGNAL(stateChanged(int)), this, SLOT(gradienterStateChanged(int)));

	for (auto it = tabWidgetState.begin(); it != tabWidgetState.end(); it++)
	{
		if (it->first->windowTitle() == "LevelAdjust")
		{
			if (it->second && !ui.LevelAdjust->isChecked())
				ui.LevelAdjust->setChecked(true);
			else if (!it->second && ui.LevelAdjust->isChecked())
				ui.LevelAdjust->setChecked(false);
		}
	}

	connect(ui.LevelAdjust, SIGNAL(stateChanged(int)), this, SLOT(gradienterStateChanged(int)));
}

void ToolPage::initToolPage()
{
	//init all checkboxs status
	ui.LevelAdjust->setChecked(true);

	TabBarPlugin* tabBarPlugin = PluginCore::getinstance()->GetPlugin<TabBarPlugin>("TabBarPlugin");
	if (tabBarPlugin)
	{
		QObject* obj = dynamic_cast<QObject*>(tabBarPlugin);
		connect(obj, SIGNAL(updateTabWidgetPage(QTabWidget*)), this, SLOT(showTabWidgetPage(QTabWidget*)));
		connect(this, SIGNAL(updateTabWidgetShow(std::map<QWidget*, bool>)), obj, SIGNAL(updateTabWidgetShowSignal(std::map<QWidget*, bool>)));
	}
}

void ToolPage::showTabWidgetPage(QTabWidget* tabWidget)
{
	m_tabWidget = tabWidget;

	for (auto it = tabWidgetState.begin(); it != tabWidgetState.end(); it++)
	{
		tabWidgetState[it->first] = false;
	}
	for (int i = 0; i < tabWidget->tabBar()->count(); i++)
	{
		tabWidgetState[tabWidget->widget(i)] = true;
	}

	setToolBoxState();
}

void ToolPage::gradienterStateChanged(int state)
{
	for (auto it = tabWidgetState.begin(); it != tabWidgetState.end(); it++)
	{
		if (it->first->windowTitle() == "LevelAdjust")
			tabWidgetState[it->first] = state;
	}
	emit updateTabWidgetShow(tabWidgetState);
}