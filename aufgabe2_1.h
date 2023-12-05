#ifndef AUFGABE2_1_H
#define AUFGABE2_1_H


#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QDebug>
#include <math.h>
#include <QMouseEvent>
#include "ctdataset.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Aufgabe2_1; }
QT_END_NAMESPACE

/**
 * @brief The Aufgabe2_1 class is the graphical interface class with slots and functions to visualize the calculated results from CTDataset class
 */
class Aufgabe2_1 : public QMainWindow
{
    Q_OBJECT

public:
    /// Constructor
    Aufgabe2_1(QWidget *parent = nullptr);
    /// Destructor
    ~Aufgabe2_1();

private:
    /// User Interface
    Ui::Aufgabe2_1 *ui;

    short* m_ptiefenkarte;
    short* m_pShadedBuffer;
    CTDataset dataset;

    bool active3D = false;
    bool bufferRendered = false;

    void updateSliceView();
    void updateSliceView3D();
    void updateDepthView3D();


private slots:
    void loadImage();
    void loadImage3D();
    void Render3D();

    void mousePressEvent(QMouseEvent *event);

    void updatedWindowingCenter(int value);
    void updatedWindowingWidth(int value);
    void updatedSlice(int value);
    void updatedThreshold(int value);
};
#endif // AUFGABE2_1_H
