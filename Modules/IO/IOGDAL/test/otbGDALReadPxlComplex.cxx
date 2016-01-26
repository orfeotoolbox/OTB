/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "gdal_priv.h"
#include <iostream>
#include <complex>

// Do all comparison in double precision
const double Epsilon = 1.E-6;

template<class TPixel>
bool IsEqual(TPixel output, TPixel expected)
{
  // avoid division by zero
  return ( (expected == output) || (abs(expected - output) / abs(expected) < Epsilon) );
}

template<class FilePixelType>
int otbGDALReadPxlComplexGeneric(int argc, char * argv[])
{
  typedef std::complex<FilePixelType>              ComplexType;

  // Get Input parameters
  if (argc != 6)
    {
    std::cout << "Invalid Parameters: " << argv[0] << "<filenameIn> <posX> <posY> <sizeX> <sizeY><output filename>" << std::endl;
    return EXIT_FAILURE;
    }
  int posX = (int)atoi(argv[2]);
  int posY = (int)atoi(argv[3]);
  int sizeX = (int)atoi(argv[4]);
  int sizeY = (int)atoi(argv[5]);
  int nbPixelToRead = (sizeX)*(sizeY);

  // Open File
  GDALDataset  *poDataset;

  GDALAllRegister();

  poDataset = (GDALDataset *) GDALOpen( argv[1], GA_ReadOnly );
  if( poDataset == NULL )
     return EXIT_FAILURE;

  // Get some information from file
  int nbBand = poDataset->GetRasterCount();

  GDALDataType pxlTypeInFile = poDataset->GetRasterBand(1)->GetRasterDataType();
  int bytePerPixel = sizeof(ComplexType);

  unsigned int sizeImgX = poDataset->GetRasterXSize();
  unsigned int sizeImgY = poDataset->GetRasterYSize();
  std::cout << "Dimensions of image are " << sizeImgX << ", "<< sizeImgY << ", "<< nbBand << std::endl;
  std::cout << "Type GDAL = " << GDALGetDataTypeName(poDataset->GetRasterBand(1)->GetRasterDataType()) << std::endl;

  //*******************
  // Method 1 : use RasterIO directly with the dataset
  std::cout << "**** METHOD 1 : Use RasterIO to read all selected data in file ****" << std::endl;
  std::streamoff nbBytes = static_cast<std::streamoff>(nbBand) * static_cast<std::streamoff>(nbPixelToRead) * static_cast<std::streamoff>(bytePerPixel);
  std::cout << "nbBytes of the buffer = " << nbBytes << " = " \
      << nbBand << " x " << nbPixelToRead << " x " << bytePerPixel<< std::endl;

  char * loadBuffer = new char[static_cast<unsigned int>(nbBytes)];

  int pixelOffset = bytePerPixel * nbBand;
  int lineOffset  = bytePerPixel * nbBand * sizeX;
  int bandOffset  = bytePerPixel;

  CPLErr lCrGdal = poDataset->RasterIO(GF_Read,
                                       posX, posY, sizeX, sizeY,
                                       loadBuffer, // pData
                                       sizeX, sizeY,
                                       pxlTypeInFile,
                                       nbBand, NULL,
                                       pixelOffset, lineOffset, bandOffset);
  // Check if gdal call succeed
  if (lCrGdal == CE_Failure)
    {
    std::cout << "Error while reading image buffer" << std::endl;
    }

  ComplexType *pxlValue = new ComplexType[nbPixelToRead * nbBand];
  unsigned int count = 0;
  ComplexType expectedValue;
  for (unsigned int itPxl = 0; itPxl < (unsigned int) (nbPixelToRead * nbBand); itPxl++)
    {
    int indY = (int) ( (int)itPxl / ( sizeX * nbBand) );
    int indX = (int) ( (int)itPxl / nbBand ) - indY*sizeX;
    int indB = (int)itPxl - (int)((int) (itPxl) / nbBand) * nbBand;
    count = 2*( sizeImgX*sizeImgY*indB + sizeImgX*(posY + indY) + (posX + indX));
    pxlValue[itPxl] = *(static_cast<ComplexType*>( static_cast<void*>(loadBuffer)) + itPxl );
    expectedValue = ComplexType(static_cast<float>(count), static_cast<float>(count+1));
    std::cout << "loadBuffer["<< itPxl << "] or " \
              << "pxlValue[" << posX + indX << "," << posY + indY << "," << indB << "]" \
              << "= " << pxlValue[itPxl] \
              << " -> expectedValue = " << expectedValue \
              << std::endl;

    if (!IsEqual(pxlValue[itPxl], expectedValue))
      {
      std::cerr << "ERROR at position " << "[" << posX + indX << "," << posY + indY << "," << indB << "]" \
                << ". Got " << pxlValue[itPxl] << ", expected " << expectedValue << std::endl;
      return EXIT_FAILURE;
      }
    }

  delete [] loadBuffer;
  delete [] pxlValue;

  //*******************
  // Method 2 : Use RasterIO to read each band included in the file
  std::cout << "**** METHOD 2 : Use RasterIO to read each band included in the file****" << std::endl;
  GDALRasterBand  *poBand;
  //ComplexType pPixelValue[nbBand][nbPixelToRead];
  ComplexType* pPixelValue = new ComplexType[nbBand*nbPixelToRead];

  for (unsigned int itBand = 0; itBand < (unsigned int) nbBand; itBand++)
    {
    poBand = poDataset->GetRasterBand( itBand +1 );

    poBand->RasterIO( GF_Read, posX, posY, sizeX, sizeY,
                      &(pPixelValue[itBand*nbPixelToRead]), sizeX, sizeY, poBand->GetRasterDataType(),
                      0, 0 );

    for (unsigned int itPxl = 0; itPxl < (unsigned int) nbPixelToRead; itPxl++)
      {
      int indY = (int) ((int)itPxl/sizeX);
      int indX = itPxl - indY * sizeX;
      count = 2*( sizeImgX*sizeImgY*itBand + sizeImgX*(posY + indY) + (posX + indX));
      expectedValue = ComplexType(static_cast<float>(count), static_cast<float>(count+1));
      std::cout << "pixelValue [" << posX + indX << "," << posY + indY << "," << itBand << "] = " \
                << pPixelValue[itBand*nbPixelToRead + indX + indY * sizeX] \
                << " -> expectedValue = " << expectedValue \
                << std::endl;
      if (!IsEqual(pPixelValue[itBand*nbPixelToRead + indX + indY * sizeX], expectedValue))
        {
        std::cerr << "ERROR at position " << "[" << posX + indX << "," << posY + indY << "," << itBand << "]" \
                  << ". Got " << pPixelValue[itBand*nbPixelToRead + indX + indY * sizeX] << ", expected " << expectedValue << std::endl;
        return EXIT_FAILURE;
        }
      }
    }

  delete [] pPixelValue;
  GDALClose(poDataset);

  return EXIT_SUCCESS;
}

int otbGDALReadPxlComplexFloat(int argc, char * argv[])
{
  return otbGDALReadPxlComplexGeneric<float>(argc, argv);
}

int otbGDALReadPxlComplexDouble(int argc, char * argv[])
{
  return otbGDALReadPxlComplexGeneric<double>(argc, argv);
}
