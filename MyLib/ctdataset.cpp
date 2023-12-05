#include "ctdataset.h"


/** * @brief CTDataset::CTDataset constructor for the CTDataset class
 *    Initializes storage for the imported Image Data from file
 */
CTDataset::CTDataset()
{
    m_pImageData = new short[130*512*512];
}
/**
 * @brief CTDataset::~CTDataset Destructor of class CTDataset destroys the class and empties up reserved storage for image data
 */
CTDataset::~CTDataset()
{
    delete[] m_pImageData;
}

/**
 * @brief CTDataset::load loads image data file into reserved storage image data according to file size into 2D or 3D array
 * tells the widget class Aufgabe2_1 whether the loading state is correct and enables the export of the image data to the widget class via data()
 * @param imagePath path for located image file on hard drive
 * @param active3D bool value for deciding whether the imported file is a 3D array or 2D array for correct reading
 * @return ReturnCode for telling whether data file was imported correctly according to its size and data type
 */
ReturnCode CTDataset::load(QString imagePath, bool active3D)
{
    if(active3D == false) {
        // Create Object from type QImage for label to represent pic
        QImage image(512, 512, QImage::Format_RGB32);

        // initial black fill
        image.fill(qRgb(0, 0, 0));

        // create datafile from given path
        QFile dataFile(imagePath);

        // Check for accessibility
        bool bFileOpen = dataFile.open(QIODevice::ReadOnly);
        if (!bFileOpen)
        {
                return ReturnCode::UNREADABLE;
        }

        // read file in array
        int iFileSize = dataFile.size();
        int iNumberBytesRead = dataFile.read((char*)m_pImageData, 512*512*sizeof(short));

        // check for byte error
        if(iFileSize!=iNumberBytesRead)
        {
                return ReturnCode::BYTE_ERROR;
        }

        // close file
        dataFile.close();
        return ReturnCode::OK;
    }
    else if(active3D == true) {
        // create object from type QImage for label to represent pic
        QImage image(512, 512, QImage::Format_RGB32);

        // initially fill with black
        image.fill(qRgb(0, 0, 0));

        // create file from image path
        QFile dataFile(imagePath);

        // readable check
        bool bFileOpen = dataFile.open(QIODevice::ReadOnly);
        if (!bFileOpen)
        {
             return ReturnCode::UNREADABLE;
        }

        // read datafile into array
        int iFileSize = dataFile.size();
        int iNumberBytesRead = dataFile.read((char*)m_pImageData, 130*512*512*sizeof(short));

        // Byte error check
        if(iFileSize!=iNumberBytesRead)
        {
            return ReturnCode::BYTE_ERROR;
        }

        // close datafile
        dataFile.close();
        return ReturnCode::OK;
    }
    return ReturnCode::OK;
}

/**
 * @brief CTDataset::calculateDepthBuffer calculates the depth buffer for the 3D visualization
 * @param inputData short array from the read in image data array
 * @param width width of the image
 * @param height height of the image
 * @param layers number of layers from the read in file
 * @param threshold threshold value which determines what the breaking condition is for the depth
 * @param depthBuffer new created data array of the depth buffer with saved values from z-axis
 * @return
 */
int CTDataset::calculateDepthBuffer(short* inputData, int width, int height,
                                     int layers, int threshold, short* depthBuffer) {
    // for whole height
    for(int y=0; y<height; y++)
    {
        // for whole width
        for(int x=0; x<width; x++)
        {
            // go through the layers
            for(int z=0; z<layers; z++) {
                // check whether data is above set threshold
                if(inputData[512*512*z+y*512+x] >= threshold) {
                    // depthbuffer array saves depth z for which the threshold is above
                    depthBuffer[x+y*height] = z;
                    break;
                }
            }
        }
    }
    return 0;
}

/**
 * @brief CTDataset::renderDepthBuffer renders the shaded buffer with help of mathematical function and the depth buffer array from calculateDepthBuffer()
 * @param depthBuffer calculated depth buffer from calculateDepthBuffer() with saved z-axis values for which the threshold is not fulfilled
 * @param width width of printed picture
 * @param height height of printed picture
 * @param shadedBuffer created data array of the 3D versioned shaded buffer as short data array
 * @return
 */
int CTDataset::renderDepthBuffer(const short* depthBuffer, int width, int height, short* shadedBuffer) {
    // step size in x and y axis (default: 2), the bigger the step size, the smoother the result will be, but not that detailed
    int sx=2;
    int sy=2;

    // for all values according to step size which are possible --> neighbors are already looked at in function
    for(int y=0+sy/2; y<height-sy/2; y++)
    {
        for(int x=0+sx/2; x<width-sy/2; x++)
        {
            // calculation of schadedBuffer according to Lambert's law
            shadedBuffer[y*512+x] = 255*sx*sy/(sqrt(pow(sy*(depthBuffer[y*512+x-sx/2] - depthBuffer[y*512+x+sx/2]), 2)
                                                              + pow(sx*(depthBuffer[y*512-sy/2+x] - depthBuffer[y*512+sy/2+x]), 2) + sqrt(pow(sx*sy,2))));

        }
    }
    return 0;
}

/**
 * @brief CTDataset::windowing function to window the values without returning a grey value
 * @param HU_value value of each pixel/voxel as HU value from the data array
 * @param center center of the window which should be illustrated
 * @param width width of the window which should be illustrated
 * @return ReturnCode, whether the used overgiven parameters are valid, OK if everything is fine else description which value is out of bounds
 */
ReturnCode CTDataset::windowing(int HU_value, int center, int width)
{
    //if everything in given boundaries
    if(-1024<=HU_value && HU_value<=3071 && 1<=width && width<=4095 && -1024<=center && center<=3071) {
        return ReturnCode::OK;
    }
    // if HU_value out of bounds
    if(-1024>HU_value || HU_value>3071) {
        return ReturnCode::HU_OUT_OF_RANGE;
    }
    // if width value is negative or higher than range
    if(1<width || width>4095) {
        return ReturnCode::WIDTH_OUT_OF_RANGE;
    }
    // if center is not inside the boundaries
    if(-1024>center || center>3071) {
        return ReturnCode::CENTER_OUT_OF_RANGE;
    }
    return ReturnCode::OK;
}

/**
 * @brief CTDataset::windowing function to window the values without returning a grey value
 * @param HU_value value of each pixel/voxel as HU value from the data array
 * @param center center of the window which should be illustrated
 * @param width width of the window which should be illustrated
 * @param &greyValue reference on calculated greyValue which is given back to the widget by referencing local variable
 * @return ReturnCode, whether the used overgiven parameters are valid, OK if everything is fine else description which value is out of bounds
 */
ReturnCode CTDataset::windowing(int HU_value, int center, int width, int &greyValue)
{
    //if everything in given boundaries
    if(-1024<=HU_value && HU_value<=3071 && 1<=width && width<=4095 && -1024<=center && center<=3071) {
        // calculate left boundary of the window
        int fensterStart = center - width/2;

        // if left outside from boundary greyValue is 0
        if(HU_value<fensterStart)
        {
            greyValue = 0;
            return ReturnCode::OK;
        }

        // if right outside from boundary greyValue is 255
        if(HU_value>(fensterStart + width))
        {
            greyValue = 255;
            return ReturnCode::OK;
        }

        // inside the window --> linear windowing
        greyValue = (HU_value - fensterStart) * 255.0 / width;
        return ReturnCode::OK;
    }
    // if HU_value out of bounds
    if(-1024>HU_value || HU_value>3071) {
        return ReturnCode::HU_OUT_OF_RANGE;
    }
    // if width value is negative or higher than range
    if(1<width || width>4095) {
        return ReturnCode::WIDTH_OUT_OF_RANGE;
    }
    // if center is not inside the boundaries
    if(-1024>center || center>3071) {
        return ReturnCode::CENTER_OUT_OF_RANGE;
    }

    return ReturnCode::OK;
}

/**
 * @brief CTDataset::data returns the image data read from the load() function to the widget
 * @return m_pImageData short data array
 */
short* CTDataset::data()
{
    return m_pImageData;
}

