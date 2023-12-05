#include <QString>
#include <QTest>
#include "ctdataset.h"
#include <algorithm>

#if defined(MYLIBUNITTESTS_EXPORT)
# define MYLIBUNITTESTS_EXPORT_IMPORT Q_DECL_EXPORT
#else
# define MYLIBUNITTESTS_EXPORT_IMPORT Q_DECL_IMPORT
#endif


class MYLIBUNITTESTS_EXPORT_IMPORT MyLibTestClass : public QObject
{
    Q_OBJECT

public:
    MyLibTestClass() {}

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void windowingTest();
};

void MyLibTestClass::initTestCase()
{
    // if necessary do some initialization stuff here before the tests start
}

void MyLibTestClass::cleanupTestCase()
{
    // if necessary do some clean up stuff here after all tests have finished
}

void MyLibTestClass::windowingTest()
{
    // VALID case 1: testing clean zero for bottom HU boundary
    ReturnCode retCode;
    CTDataset dataset;
    int greyValue = 0;
    retCode = dataset.windowing(-34, -34 + 50, 100, greyValue);
    QVERIFY2(retCode == ReturnCode::OK, "returns an error although input is valid");
    QVERIFY2(greyValue == 0, "windowing function lower bound");

    // VALID case 2: testing center of windowed domain
    greyValue = -1;
    retCode = dataset.windowing(50, 50, 100, greyValue);
    QVERIFY2(retCode == ReturnCode::OK, "returns an error although input is valid");
    QVERIFY2(greyValue == 128, qPrintable(QString("windowing function medium value, was %1 instead of 128").arg(greyValue)));

    // INVALID case 1: HU input too low
    retCode = dataset.windowing(-4100, -1000, 2000, greyValue);
    QVERIFY2(retCode == ReturnCode::HU_OUT_OF_RANGE, "No error code returned although input HU value was <-1024");

    // ADD FURTHER VALID Testcases here
}

//QTEST_APPLESS_MAIN(MyLibTestClass)
#include "tst_mylibunittest.moc"

