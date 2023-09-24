#include "MenuBar.h"

MenuBar::MenuBar(QObject* parent): QObject(parent)
{
	m_menuWidget = new MenuWidget();
}

MenuBar::~MenuBar()
{
}

void MenuBar::SetMenuBar(QMainWindow* window)
{
	m_menuWidget->install(window);
}
