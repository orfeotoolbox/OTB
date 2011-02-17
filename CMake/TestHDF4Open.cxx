/*
 * TestHDF4Open.cxx
 *
 *  Created on: 14 févr. 2011
 *      Author: Mickael Savinaud
 */

#include <iostream>
#include <string>

#include <gdal_priv.h>
#include <cpl_port.h>

int main(int argc, char * argv[])
{
  if (argc != 2)
    {
    std::cerr << "Usage : " << argv[0] << " <HDF4_filename> <ID number of the dataset>" << std::endl;
    return 1;
    }

  // Open File
  GDALDataset  *poDataset;
  GDALDataset  *poDatasetSubDS;

  GDALAllRegister();

  poDataset = (GDALDataset *) GDALOpen(argv[1], GA_ReadOnly);
  if (poDataset == NULL)
    {
    return 1;
    }

  std::cout << "HDF4 File : " << argv[1] << std::endl;

  char **metadata = poDataset->GetMetadata(""); // Retrieve Attributes
  char **metadataSDS = poDataset->GetMetadata("SUBDATASETS"); // Retrieving child objects

  int selectedDSNameID = 2;
  std::cout << "ID of the subdataset : " << selectedDSNameID << std::endl;
  std::string subDatasetName (metadataSDS[2 * (selectedDSNameID -1) ]); // +1 for i_DESC

  size_t found = subDatasetName.find("=");
  if (found == std::string::npos)
    {
    return 1;
    }
  else
    {
    subDatasetName.erase ( subDatasetName.begin(), subDatasetName.begin() + found +1);
    std::cout << "Name the subdataset : " << subDatasetName <<std::endl;
    }

  poDatasetSubDS = (GDALDataset *) GDALOpen(subDatasetName.c_str(), GA_ReadOnly);
  if (poDatasetSubDS == NULL)
    {
    return 1;
    }
  std::cout << "Subdataset in the HDF4 file is open " << std::endl;

  char **metadataSubDS = poDatasetSubDS->GetMetadata(""); // Retrieve Attributes
  std::cout << "Metadata[0] in SubDS :  " <<  metadataSubDS[0]<< std::endl;

  int xSize = poDatasetSubDS->GetRasterXSize();
  int ySize = poDatasetSubDS->GetRasterYSize();
  int nbBand = poDatasetSubDS->GetRasterCount();
  std::cout << "xSize = " << xSize << ", ySize = " << ySize << ", nbBand = " << nbBand << std::endl;

  GDALRasterBand *rb = poDatasetSubDS->GetRasterBand(1);
  GDALDataType pxlType = rb->GetRasterDataType();
  std::cout << "Type GDAL = " << GDALGetDataTypeName(pxlType) << std::endl;

  int xSizeROI = xSize ;
  int ySizeROI = ySize ;

  if (pxlType == GDT_Byte)
    {
    std::cout << "byte case : " << sizeof(unsigned char) << std::endl;
    typedef unsigned char UCHAR;
    UCHAR *buffer = new UCHAR[xSizeROI * ySizeROI];
    rb->RasterIO(GF_Read, 0, 0, xSizeROI, ySizeROI, buffer, xSizeROI, ySizeROI, pxlType, 0, 0);
    if ( CPLGetLastErrorNo() > 0 )
      {
      return 1;
      }
    std::cout << "value = "<<  static_cast<UCHAR>(*(buffer)) << std::endl;
    delete[] buffer;
      }
  else if (pxlType == GDT_Int16)
    {
    std::cout << "Int16 case : "<< sizeof(short int) << std::endl;
    typedef short int SHORT;
    SHORT *buffer = new SHORT[xSizeROI * ySizeROI];
    rb->RasterIO(GF_Read, 0, 0, xSizeROI, ySizeROI, buffer, xSizeROI, ySizeROI, pxlType, 0, 0);
    if ( CPLGetLastErrorNo() > 0 )
      {
      return 1;
      }
    std::cout << "value = "<<static_cast<SHORT>(*(buffer)) << std::endl;
    delete[] buffer;
    }
  else if (pxlType == GDT_Float32)
    {
    std::cout << "float case : "<< sizeof(float) << std::endl;
    float *buffer = new float[xSizeROI * ySizeROI];
    rb->RasterIO(GF_Read, 0, 0, xSizeROI, ySizeROI, buffer, xSizeROI, ySizeROI, pxlType, 0, 0);
    if ( CPLGetLastErrorNo() > 0 )
      {
      return 1;
      }
    std::cout << "value = "<<static_cast<float>(*(buffer)) << std::endl;
    delete[] buffer;
    }

  GDALClose(poDatasetSubDS);
  GDALClose(poDataset);

  return 0;
}

