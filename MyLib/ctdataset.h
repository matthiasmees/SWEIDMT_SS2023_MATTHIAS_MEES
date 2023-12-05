#ifndef CTDATASET_H
#define CTDATASET_H

#include <QFile>
#include <QFileDialog>
#include <QElapsedTimer>
#include <QDebug>
#include <math.h>
#include <QMouseEvent>
#include <QTest>

#if defined(MYLIB_EXPORT)
# define MYLIB_EXPORT_IMPORT Q_DECL_EXPORT
#else
# define MYLIB_EXPORT_IMPORT Q_DECL_IMPORT
#endif

#pragma once

/**
 * @brief The ReturnCode enum is the enum for the error codes which are given to the widget class Aufgabe2_1
 */
enum class ReturnCode
{
    OK,
    HU_OUT_OF_RANGE,
    CENTER_OUT_OF_RANGE,
    WIDTH_OUT_OF_RANGE,
    BYTE_ERROR,
    UNREADABLE
};

/**
 * @brief The CTDataset class calculates everything for the widget class Aufgabe2_1
 */
class MYLIB_EXPORT_IMPORT CTDataset
{

public:
    /// Constructor
    CTDataset();
    /// Destructor
    ~CTDataset();

    /// Loads Image according to its size and returns whethter it was successfull
    ReturnCode load(QString imagePath, bool active3D);

    /// Calculation of the depth buffer
    static int calculateDepthBuffer(short* inputData, int width, int height,
                             int layers, int threshold, short* depthBuffer);
    /// Rendering of rhe depth buffer
    static int renderDepthBuffer(const short* depthBuffer, int width, int height, short* shadedBuffer);

    /// Windowing function without greyValue
    static ReturnCode windowing(int HU_value, int center, int width);

    /// Windowing function with greyValue
    static ReturnCode windowing(int HU_value, int center, int width, int &greyValue);

    /// Return function for data that was read with the load function
    short* data();

private:
    /// Private Image data storage for calculations inside the class
    short* m_pImageData;

};

#endif // CTDATASET_H
