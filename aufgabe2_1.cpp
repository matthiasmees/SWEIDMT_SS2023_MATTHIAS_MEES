#include "aufgabe2_1.h"
#include "./ui_aufgabe2_1.h"



Aufgabe2_1::Aufgabe2_1(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Aufgabe2_1)
{
    ui->setupUi(this);

    connect(ui->load12bitButton, SIGNAL(clicked()), this, SLOT(loadImage()));
    connect(ui->load3DButton, SIGNAL(clicked()), this, SLOT(loadImage3D()));
    connect(ui->render3DButton, SIGNAL(clicked()), this, SLOT(Render3D()));

    connect(ui->sliderWidth, SIGNAL(valueChanged(int)), this, SLOT(updatedWindowingWidth(int)));
    connect(ui->sliderCenter, SIGNAL(valueChanged(int)), this, SLOT(updatedWindowingCenter(int)));
    connect(ui->sliderSlice, SIGNAL(valueChanged(int)), this, SLOT(updatedSlice(int)));
    connect(ui->sliderThreshold, SIGNAL(valueChanged(int)), this, SLOT(updatedThreshold(int)));

    m_ptiefenkarte = new short[512*512];
    m_pShadedBuffer = new short[512*512];
}

Aufgabe2_1::~Aufgabe2_1()
{
    delete ui;
    delete[] m_ptiefenkarte;
    delete[] m_pShadedBuffer;
}

void Aufgabe2_1::loadImage()
{
    active3D = false;
    // open File Dialog to select dataset
    QString imagePath = QFileDialog::getOpenFileName(this,"Open Image", "/Users/matthiasmees/Aufgabe2_1",
                                                     "Raw Image Files (*.raw)");

    dataset.load(imagePath, active3D);
    if(dataset.load(imagePath, active3D) == ReturnCode::BYTE_ERROR) {
        QMessageBox::critical(this, "ACHTUNG", "Bytefehler");
    }
    else if(dataset.load(imagePath, active3D) == ReturnCode::UNREADABLE) {
        QMessageBox::critical(this, "ACHTUNG", "Datei nicht lesbar");
    }
    else if(dataset.load(imagePath, active3D) == ReturnCode::OK) {
        updateSliceView();
    }
}

void Aufgabe2_1::loadImage3D()
{
    active3D = true;
    QString imagePath = QFileDialog::getOpenFileName(this,"Open Image", "/Users/matthiasmees/Aufgabe2_1",
                                                     "Raw Image Files (*.raw)");

    dataset.load(imagePath, active3D);
    if(dataset.load(imagePath, active3D) == ReturnCode::BYTE_ERROR) {
        QMessageBox::critical(this, "ACHTUNG", "Bytefehler");
    }
    else if(dataset.load(imagePath, active3D) == ReturnCode::UNREADABLE) {
        QMessageBox::critical(this, "ACHTUNG", "Datei nicht lesbar");
    }
    else if(dataset.load(imagePath, active3D) == ReturnCode::OK) {
        updateSliceView3D();
    }
}

void Aufgabe2_1::Render3D() {

    active3D = true;
    loadImage3D();
    CTDataset::calculateDepthBuffer(dataset.data(), 512, 512, 130, 0, m_ptiefenkarte);
    CTDataset::renderDepthBuffer(m_ptiefenkarte, 512, 512, m_pShadedBuffer);
    updateDepthView3D();
    bufferRendered = true;
}


// draws picture from current slider values
void Aufgabe2_1::updateSliceView() {

    active3D = false;

    QElapsedTimer timer;
    timer.start();
    QImage image(512, 512, QImage::Format_RGB32);

    // slider values taken
    int greyValue = 0;
    int sliderWidthValue = ui->sliderWidth->value();
    int sliderCenterValue = ui->sliderCenter->value();
    int sliderThresholdValue = ui->sliderThreshold->value();

    // Setze Inhalt des Arrays als Pixel in das Bild
    for(int y=0; y<512; y++)
    {
        for(int x=0; x<512; x++)
        {
            if (CTDataset::windowing(dataset.data()[x+y*512], sliderCenterValue, sliderWidthValue, greyValue) == ReturnCode::OK) {
                if(dataset.data()[y*512+x] >= sliderThresholdValue) {
                    image.setPixel(x,y,qRgb(255, 0, 0));
                }
                if(dataset.data()[y*512+x] < sliderThresholdValue) {
                    image.setPixel(x,y,qRgb(greyValue, greyValue, greyValue));
                }
            }
        }
    }

    // Bild auf Benutzeroberfläche anzeigen
    ui->picLabel->setPixmap(QPixmap::fromImage(image));

    if(active3D == false) {
        ui->sliderSlice->setSliderPosition(0);
        ui->label_slice->setText("Slice: 1");
    }

    qDebug() << timer.nsecsElapsed();
}

// draws picture from current slider values
void Aufgabe2_1::updateSliceView3D() {

    active3D = true;
    QElapsedTimer timer;
    timer.start();
    QImage image(512, 512, QImage::Format_RGB32);

    // slider values taken
    int greyValue = 0;
    int sliderWidthValue = ui->sliderWidth->value();
    int sliderCenterValue = ui->sliderCenter->value();
    int sliderSliceValue = ui->sliderSlice->value();

    // Setze Inhalt des Arrays als Pixel in das Bild
    for(int y=0; y<512; y++)
    {
        for(int x=0; x<512; x++)
        {
            if (CTDataset::windowing(dataset.data()[512*512*sliderSliceValue+x+y*512], sliderCenterValue, sliderWidthValue, greyValue) == ReturnCode::OK) {
                image.setPixel(x,y,qRgb(greyValue, greyValue, greyValue));
            }
        }
    }

    // Bild auf Benutzeroberfläche anzeigen
    ui->picLabel->setPixmap(QPixmap::fromImage(image));

    if(active3D == true) {
        ui->sliderThreshold->setSliderPosition(0);
        ui->label_threshold->setText("Threshold: 0");
    }

    qDebug() << timer.nsecsElapsed();

}

// draws picture from current slider values
void Aufgabe2_1::updateDepthView3D() {

    active3D = true;
    QElapsedTimer timer;
    timer.start();
    QImage image(512, 512, QImage::Format_RGB32);

    // slider values taken
    int greyValue = 0;
    int sliderWidthValue = ui->sliderWidth->value();
    int sliderCenterValue = ui->sliderCenter->value();
    //int sliderSliceValue = ui->sliderSlice->value();

    // Setze Inhalt des Arrays als Pixel in das Bild
    for(int y=0; y<512; y++)
    {
        for(int x=0; x<512; x++)
        {
            if (CTDataset::windowing(m_pShadedBuffer[y*512+x], sliderCenterValue, sliderWidthValue, greyValue) == ReturnCode::OK) {
                image.setPixel(x,y,qRgb(greyValue, greyValue, greyValue));
            }
        }
    }

    // set depth view to 3d image label
    ui->label_image3D->setPixmap(QPixmap::fromImage(image));

    qDebug() << timer.nsecsElapsed();
}

void Aufgabe2_1::updatedWindowingCenter(int value) {

    ui->label_center->setText("Center: " + QString::number(value));

    if(active3D == true) {
        updateSliceView3D();
    }

    if(active3D == false) {
        updateSliceView();
    }
}

void Aufgabe2_1::updatedWindowingWidth(int value) {

    ui->label_width->setText("Width: " + QString::number(value));

    if(active3D == true) {
        updateSliceView3D();
    }

    if(active3D == false) {
        updateSliceView();
    }
}

void Aufgabe2_1::updatedSlice(int value) {

    if(active3D == false) {
        ui->sliderSlice->setSliderPosition(0);
        ui->label_slice->setText("Slice: 1");
    }

    if(active3D == true) {
        ui->label_slice->setText("Slice: " + QString::number(value+1));
        updateSliceView3D();
    }
}

void Aufgabe2_1::updatedThreshold(int value) {

    ui->label_threshold->setText("Threshold: " + QString::number(value));
    if(active3D == false) {
        updateSliceView();
    }
    if(active3D == true) {
        ui->sliderThreshold->setSliderPosition(0);
        ui->label_threshold->setText("Threshold: 0");
    }
}

void Aufgabe2_1::mousePressEvent(QMouseEvent *event) {

    QPoint globalPos = event->pos();
    QPoint localPos = ui->label_image3D->mapFromParent(globalPos);

    if (bufferRendered==true) {
        if (ui->label_image3D->rect().contains(localPos))
        {
            ui->xposLabel->setText("x: " + QString::number(localPos.x()));
            ui->yposLabel->setText("y: " + QString::number(localPos.y()));
            ui->zposLabel->setText("z: " + QString::number(m_pShadedBuffer[512*localPos.y()+localPos.x()]));
        }
    }


}

