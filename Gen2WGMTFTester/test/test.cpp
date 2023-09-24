#include "test.h"
#include "CollimatorPlugin.h"
#include "Collimator.h"

test::test(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    test_Collimator();

    //test_Collimator2();
}

test::~test()
{}

void test::test_Collimator()
{
    CollimatorPlugin* collPlugin = new Collimator();
    collPlugin->Open("");
}

void test::test_Collimator2()
{
}
