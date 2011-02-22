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

  char **metadata = poDataset->GetMetadata(""); // Retrieve Attributes
  char **metadataSDS = poDataset->GetMetadata("SUBDATASETS"); // Retrieving child objects

  int selectedDSNameID = 2;
  std::string subDatasetName (metadataSDS[2 * (selectedDSNameID -1) ]); // +1 for i_DESC

  size_t found = subDatasetName.find("=");
  if (found == std::string::npos)
    {
    return 1;
    }
  else
    {
    subDatasetName.erase ( subDatasetName.begin(), subDatasetName.begin() + found +1);
    }

  poDatasetSubDS = (GDALDataset *) GDALOpen(subDatasetName.c_str(), GA_ReadOnly);
  if (poDatasetSubDS == NULL)
    {
    return 1;
    }

  char **metadataSubDS = poDatasetSubDS->GetMetadata(""); // Retrieve Attributes

  int xSize = poDatasetSubDS->GetRasterXSize();
  int ySize = poDatasetSubDS->GetRasterYSize();
  int nbBand = poDatasetSubDS->GetRasterCount();

  GDALRasterBand *rb = poDatasetSubDS->GetRasterBand(1);
  GDALDataType pxlType = rb->GetRasterDataType();

  int xSizeROI = xSize ;
  int ySizeROI = ySize ;

  if (pxlType == GDT_Byte)
    {
    typedef unsigned char UCHAR;
    UCHAR *buffer = new UCHAR[xSizeROI * ySizeROI];
    CPLErr lCrGdal = rb->RasterIO(GF_Read, 0, 0, xSizeROI, ySizeROI, buffer, xSizeROI, ySizeROI, pxlType, 0, 0);
    if (lCrGdal == CE_Failure)
      {
      return 1;
      }
    delete[] buffer;
      }
  else if (pxlType == GDT_Int16)
    {
    typedef short int SHORT;
    SHORT *buffer = new SHORT[xSizeROI * ySizeROI];
    CPLErr lCrGdal = rb->RasterIO(GF_Read, 0, 0, xSizeROI, ySizeROI, buffer, xSizeROI, ySizeROI, pxlType, 0, 0);
    if (lCrGdal == CE_Failure)
      {
      return 1;
      }
    delete[] buffer;
    }
  else if (pxlType == GDT_Float32)
    {
    float *buffer = new float[xSizeROI * ySizeROI];
    CPLErr lCrGdal =  rb->RasterIO(GF_Read, 0, 0, xSizeROI, ySizeROI, buffer, xSizeROI, ySizeROI, pxlType, 0, 0);
    if (lCrGdal == CE_Failure)
      {
      return 1;
      }
    delete[] buffer;
    }

  GDALClose(poDatasetSubDS);
  GDALClose(poDataset);

  return 0;
}
