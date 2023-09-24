#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_test.h"

class test : public QMainWindow
{
    Q_OBJECT

public:
    test(QWidget *parent = nullptr);
    ~test();

    void test_Collimator();
    void test_Collimator2();

private:
    Ui::testClass ui;
};
