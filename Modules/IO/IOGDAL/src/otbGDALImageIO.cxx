/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2018-2020 CS Systemes d'Information (CS SI)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <fstream>
#include <vector>

#include "otbGDALImageIO.h"
#include "otbMacro.h"
#include "otbSystem.h"
#include "otbStopwatch.h"
#include "itksys/SystemTools.hxx"
#include "otbImage.h"
#include "otb_tinyxml.h"
#include "otbImageKeywordlist.h"

#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"

#include "itkRGBPixel.h"
#include "itkRGBAPixel.h"

#include "cpl_conv.h"
#include "ogr_spatialref.h"
#include "ogr_srs_api.h"


#include "itksys/RegularExpression.hxx"

#include "otbGDALDriverManagerWrapper.h"

#include "otb_boost_string_header.h"

#include "otbOGRHelpers.h"
#include "otbGeometryMetadata.h"
#include "otbConfigure.h"

#include "stdint.h" //needed for uintptr_t

inline unsigned int uint_ceildivpow2(unsigned int a, unsigned int b)
{
  return (a + (1 << b) - 1) >> b;
}

namespace otb
{

class GDALDataTypeWrapper
{
public:
  GDALDataTypeWrapper() : pixType(GDT_Byte)
  {
  }
  ~GDALDataTypeWrapper()
  {
  }
  GDALDataTypeWrapper(const GDALDataTypeWrapper& w)
  {
    pixType = w.pixType;
  }
  GDALDataTypeWrapper& operator=(GDALDataTypeWrapper w)
  {
    pixType = w.pixType;
    return *this;
  }
  GDALDataType pixType;
}; // end of GDALDataTypeWrapper


GDALImageIO::GDALImageIO()
{
  // By default set number of dimensions to two.
  this->SetNumberOfDimensions(2);

  // By default set pixel type to scalar.
  m_PixelType = SCALAR;

  // By default set component type to unsigned char
  m_ComponentType    = UCHAR;
  m_UseCompression   = false;
  m_CompressionLevel = 4; // Range 0-9; 0 = no file compression, 9 = maximum file compression

  // Set default spacing to one
  m_Spacing[0] = 1.0;
  m_Spacing[1] = 1.0;
  // Set default origin to half a pixel (centered pixel convention)
  m_Origin[0] = 0.5;
  m_Origin[1] = 0.5;

  m_IsIndexed     = false;
  m_DatasetNumber = 0;

  m_NbBands                   = 0;
  m_FlagWriteImageInformation = true;

  m_CanStreamWrite = false;
  m_IsComplex      = false;
  m_IsVectorImage  = false;

  m_PxType = new GDALDataTypeWrapper;

  m_NumberOfOverviews = 0;
  m_ResolutionFactor  = 0;
  m_BytePerPixel      = 0;
  m_WriteRPCTags      = true;

  m_epsgCode          = 0;
}

GDALImageIO::~GDALImageIO()
{
  delete m_PxType;
}

// Tell only if the file can be read with GDAL.
bool GDALImageIO::CanReadFile(const char* file)
{
  // First check the extension
  if (file == nullptr)
  {
    return false;
  }
  m_Dataset = GDALDriverManagerWrapper::GetInstance().Open(file);
  return m_Dataset.IsNotNull();
}

// Used to print information about this object
void GDALImageIO::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Compression Level : " << m_CompressionLevel << "\n";
  os << indent << "IsComplex (otb side) : " << m_IsComplex << "\n";
  os << indent << "Byte per pixel : " << m_BytePerPixel << "\n";
}

// Read a 3D image (or event more bands)... not implemented yet
void GDALImageIO::ReadVolume(void*)
{
}

// Read image with GDAL
void GDALImageIO::Read(void* buffer)
{
  // Convert buffer from void * to unsigned char *
  unsigned char* p = static_cast<unsigned char*>(buffer);

  // Check if conversion succeed
  if (p == nullptr)
  {
    itkExceptionMacro(<< "Buffer passed to GDALImageIO for reading is NULL.");
    return;
  }

  // Get the origin of the region to read
  int lFirstLineRegion   = this->GetIORegion().GetIndex()[1];
  int lFirstColumnRegion = this->GetIORegion().GetIndex()[0];

  // Get nb. of lines and columns of the region to read
  int lNbLinesRegion   = this->GetIORegion().GetSize()[1];
  int lNbColumnsRegion = this->GetIORegion().GetSize()[0];

  // Compute the origin of the image region to read at the initial resolution
  int lFirstLine   = lFirstLineRegion * (1 << m_ResolutionFactor);
  int lFirstColumn = lFirstColumnRegion * (1 << m_ResolutionFactor);

  // Compute the size of the image region to read at the initial resolution
  int lNbLines   = lNbLinesRegion * (1 << m_ResolutionFactor);
  int lNbColumns = lNbColumnsRegion * (1 << m_ResolutionFactor);

  // Check if the image region is correct
  if (lFirstLine + lNbLines > static_cast<int>(m_OriginalDimensions[1]))
    lNbLines = static_cast<int>(m_OriginalDimensions[1] - lFirstLine);
  if (lFirstColumn + lNbColumns > static_cast<int>(m_OriginalDimensions[0]))
    lNbColumns = static_cast<int>(m_OriginalDimensions[0] - lFirstColumn);

  GDALDataset* dataset = m_Dataset->GetDataSet();

  // In the indexed case, one has to retrieve the index image and the
  // color table, and translate p to a 4 components color values buffer
  if (m_IsIndexed)
  {
    // TODO: This is a very special case and seems to be working only
    // for unsigned char pixels. There might be a gdal method to do
    // the work in a cleaner way
    std::streamoff                           lNbPixels   = (static_cast<std::streamoff>(lNbColumnsRegion)) * (static_cast<std::streamoff>(lNbLinesRegion));
    std::streamoff                           lBufferSize = static_cast<std::streamoff>(m_BytePerPixel) * lNbPixels;
    itk::VariableLengthVector<unsigned char> value(lBufferSize);

    std::streamoff step = static_cast<std::streamoff>(this->GetNumberOfComponents()) * static_cast<std::streamoff>(m_BytePerPixel);
    otbLogMacro(Debug, << "GDAL reads [" << lFirstColumn << ", " << lFirstColumn + lNbColumns - 1 << "]x[" << lFirstLine << ", " << lFirstLine + lNbLines - 1
                       << "] indexed color image of type " << GDALGetDataTypeName(m_PxType->pixType) << " from file " << m_FileName);
    otb::Stopwatch chrono = otb::Stopwatch::StartNew();
    CPLErr         lCrGdal =
        dataset->GetRasterBand(1)->RasterIO(GF_Read, lFirstColumn, lFirstLine, lNbColumns, lNbLines, const_cast<unsigned char*>(value.GetDataPointer()),
                                            lNbColumnsRegion, lNbLinesRegion, m_PxType->pixType, 0, 0);
    chrono.Stop();

    if (lCrGdal == CE_Failure)
    {
      itkExceptionMacro(<< "Error while reading image (GDAL format) '" << m_FileName << "' : " << CPLGetLastErrorMsg());
    }

    otbLogMacro(Debug, << "GDAL read took " << chrono.GetElapsedMilliseconds() << " ms")

        // Interpret index as color
        std::streamoff cpt(0);
    GDALColorTable*    colorTable = dataset->GetRasterBand(1)->GetColorTable();
    for (std::streamoff i = 0; i < lBufferSize; i = i + static_cast<std::streamoff>(m_BytePerPixel))
    {
      GDALColorEntry color;
      colorTable->GetColorEntryAsRGB(value[i], &color);
      p[cpt]     = color.c1;
      p[cpt + 1] = color.c2;
      p[cpt + 2] = color.c3;
      p[cpt + 3] = color.c4;
      cpt += step;
    }
  }
  else
  {
    /********  Nominal case ***********/
    int pixelOffset = m_BytePerPixel * m_NbBands;
    int lineOffset  = m_BytePerPixel * m_NbBands * lNbColumnsRegion;
    int bandOffset  = m_BytePerPixel;
    int nbBands     = m_NbBands;

    // In some cases, we need to change some parameters for RasterIO
    if (!GDALDataTypeIsComplex(m_PxType->pixType) && m_IsComplex && m_IsVectorImage && (m_NbBands > 1))
    {
      pixelOffset = m_BytePerPixel * 2;
      lineOffset  = pixelOffset * lNbColumnsRegion;
      bandOffset  = m_BytePerPixel;
    }

    // keep it for the moment
    otbLogMacro(Debug, << "GDAL reads [" << lFirstColumn << ", " << lFirstColumnRegion + lNbColumnsRegion - 1 << "]x[" << lFirstLineRegion << ", "
                       << lFirstLineRegion + lNbLinesRegion - 1 << "] x " << nbBands << " bands of type " << GDALGetDataTypeName(m_PxType->pixType)
                       << " from file " << m_FileName);

    otb::Stopwatch chrono  = otb::Stopwatch::StartNew();
    CPLErr         lCrGdal = m_Dataset->GetDataSet()->RasterIO(GF_Read, lFirstColumn, lFirstLine, lNbColumns, lNbLines, p, lNbColumnsRegion, lNbLinesRegion,
                                                       m_PxType->pixType, nbBands,
                                                       // We want to read all bands
                                                       nullptr, pixelOffset, lineOffset, bandOffset);
    chrono.Stop();
    // Check if gdal call succeed
    if (lCrGdal == CE_Failure)
    {
      itkExceptionMacro(<< "Error while reading image (GDAL format) '" << m_FileName << "' : " << CPLGetLastErrorMsg());
      return;
    }

    otbLogMacro(Debug, << "GDAL read took " << chrono.GetElapsedMilliseconds() << " ms")
  }
}

bool GDALImageIO::GetSubDatasetInfo(std::vector<std::string>& names, std::vector<std::string>& desc)
{
  // Note: we assume that the subdatasets are in order : SUBDATASET_ID_NAME, SUBDATASET_ID_DESC, SUBDATASET_ID+1_NAME, SUBDATASET_ID+1_DESC
  char** papszMetadata;
  papszMetadata = m_Dataset->GetDataSet()->GetMetadata("SUBDATASETS");

  // Have we find some dataSet ?
  // This feature is supported only for hdf4 and hdf5 file (regards to the bug 270)
  if ((CSLCount(papszMetadata) > 0) && ((strcmp(m_Dataset->GetDataSet()->GetDriver()->GetDescription(), "HDF4") == 0) ||
                                        (strcmp(m_Dataset->GetDataSet()->GetDriver()->GetDescription(), "HDF5") == 0) ||
                                        (strcmp(m_Dataset->GetDataSet()->GetDriver()->GetDescription(), "SENTINEL2") == 0)))
  {
    for (int cpt = 0; papszMetadata[cpt] != nullptr; ++cpt)
    {
      std::string key, name;
      if (System::ParseHdfSubsetName(papszMetadata[cpt], key, name))
      {
        // check if this is a dataset name
        if (key.find("_NAME") != std::string::npos)
          names.push_back(name);
        // check if this is a dataset descriptor
        if (key.find("_DESC") != std::string::npos)
          desc.push_back(name);
      }
    }
  }
  else
  {
    return false;
  }
  if (names.empty() || desc.empty())
    return false;
  if (names.size() != desc.size())
  {
    names.clear();
    desc.clear();
    return false;
  }

  return true;
}

bool GDALImageIO::GDALPixelTypeIsComplex()
{
  return GDALDataTypeIsComplex(m_PxType->pixType);
}

void GDALImageIO::ReadImageInformation()
{
  // std::ifstream file;
  this->InternalReadImageInformation();
}

unsigned int GDALImageIO::GetOverviewsCount()
{
  GDALDataset* dataset = m_Dataset->GetDataSet();

  // JPEG2000 case : use the number of overviews actually in the dataset
  if (m_Dataset->IsJPEG2000())
  {
    // Include the full resolution in overviews count
    return dataset->GetRasterBand(1)->GetOverviewCount() + 1;
  }

  if (dataset->GetRasterBand(1)->GetOverviewCount())
    // Include the full resolution in overviews count
    return dataset->GetRasterBand(1)->GetOverviewCount() + 1;

  // default case: compute overviews until one of the dimensions is 1
  bool         flagStop              = false;
  unsigned int possibleOverviewCount = 0;
  while (!flagStop)
  {
    unsigned int tDimX = uint_ceildivpow2(dataset->GetRasterXSize(), possibleOverviewCount);
    unsigned int tDimY = uint_ceildivpow2(dataset->GetRasterYSize(), possibleOverviewCount);

    possibleOverviewCount++;
    if ((tDimX == 1) || (tDimY == 1))
    {
      flagStop = true;
    }
  }
  return possibleOverviewCount;
}


std::vector<std::string> GDALImageIO::GetOverviewsInfo()
{
  std::vector<std::string> desc;

  // This should never happen, according to implementation of GetOverviewCount()
  if (this->GetOverviewsCount() == 0)
    return desc;

  std::ostringstream oss;

  // If gdal exposes actual overviews
  unsigned int lOverviewsCount = m_Dataset->GetDataSet()->GetRasterBand(1)->GetOverviewCount();

  if (lOverviewsCount)
  {
    unsigned int x = m_OriginalDimensions[0];
    unsigned int y = m_OriginalDimensions[1];

    oss.str("");
    oss << "Resolution: 0 (Image [w x h]: " << x << "x" << y << ")";
    desc.push_back(oss.str());

    for (unsigned int iOverview = 0; iOverview < lOverviewsCount; iOverview++)
    {
      x = m_Dataset->GetDataSet()->GetRasterBand(1)->GetOverview(iOverview)->GetXSize();
      y = m_Dataset->GetDataSet()->GetRasterBand(1)->GetOverview(iOverview)->GetYSize();
      oss.str("");
      oss << "Resolution: " << iOverview + 1 << " (Image [w x h]: " << x << "x" << y << ")";
      desc.push_back(oss.str());
    }
  }
  else
  {
    // Fall back to gdal implicit overviews
    lOverviewsCount = this->GetOverviewsCount();

    unsigned int originalWidth  = m_OriginalDimensions[0];
    unsigned int originalHeight = m_OriginalDimensions[1];

    // Get the overview sizes
    for (unsigned int iOverview = 0; iOverview < lOverviewsCount; iOverview++)
    {
      // For each resolution we will compute the tile dim and image dim
      unsigned int w = uint_ceildivpow2(originalWidth, iOverview);
      unsigned int h = uint_ceildivpow2(originalHeight, iOverview);
      oss.str("");
      oss << "Resolution: " << iOverview << " (Image [w x h]: " << w << "x" << h << ")";
      desc.push_back(oss.str());
    }
  }

  return desc;
}

void GDALImageIO::SetEpsgCode(const unsigned int epsgCode)
{
  m_epsgCode = epsgCode;
}

void GDALImageIO::InternalReadImageInformation()
{
  itk::ExposeMetaData<unsigned int>(this->GetMetaDataDictionary(), MetaDataKey::ResolutionFactor, m_ResolutionFactor);

  itk::ExposeMetaData<unsigned int>(this->GetMetaDataDictionary(), MetaDataKey::SubDatasetIndex, m_DatasetNumber);

  // Detecting if we are in the case of an image with subdatasets
  // example: hdf Modis data
  // in this situation, we are going to change the filename to the
  // supported gdal format using the m_DatasetNumber value
  // HDF4_SDS:UNKNOWN:"myfile.hdf":2
  // and make m_Dataset point to it.
  if (m_Dataset->GetDataSet()->GetRasterCount() == 0 || m_DatasetNumber > 0)
  {
    // this happen in the case of a hdf file with SUBDATASETS
    // Note: we assume that the datasets are in order
    char** papszMetadata;
    papszMetadata = m_Dataset->GetDataSet()->GetMetadata("SUBDATASETS");
    // TODO: we might want to keep the list of names somewhere, at least the number of datasets
    std::vector<std::string> names;
    if (CSLCount(papszMetadata) > 0)
    {
      for (int cpt = 0; papszMetadata[cpt] != nullptr; ++cpt)
      {
        std::string key, name;
        if (System::ParseHdfSubsetName(papszMetadata[cpt], key, name))
        {
          // check if this is a dataset name
          if (key.find("_NAME") != std::string::npos)
            names.push_back(name);
        }
      }
    }
    if (m_DatasetNumber < names.size())
    {
      m_Dataset = GDALDriverManagerWrapper::GetInstance().Open(names[m_DatasetNumber]);
    }
    else
    {
      itkExceptionMacro(<< "Dataset requested does not exist (" << names.size() << " datasets)");
    }
  }

  GDALDataset* dataset = m_Dataset->GetDataSet();

  // Get image dimensions
  if (dataset->GetRasterXSize() == 0 || dataset->GetRasterYSize() == 0)
  {
    itkExceptionMacro(<< "Dimension is undefined.");
  }

  // Set image dimensions into IO
  m_Dimensions[0] = uint_ceildivpow2(dataset->GetRasterXSize(), m_ResolutionFactor);
  m_Dimensions[1] = uint_ceildivpow2(dataset->GetRasterYSize(), m_ResolutionFactor);

  // Keep the original dimension of the image
  m_OriginalDimensions.push_back(dataset->GetRasterXSize());
  m_OriginalDimensions.push_back(dataset->GetRasterYSize());

  // Get Number of Bands
  m_NbBands = dataset->GetRasterCount();

  // Get the number of overviews of the file (based on the first band)
  m_NumberOfOverviews = dataset->GetRasterBand(1)->GetOverviewCount();

  // Get the overview sizes
  for (unsigned int iOverview = 0; iOverview < m_NumberOfOverviews; iOverview++)
  {
    std::pair<unsigned int, unsigned int> tempSize;
    tempSize.first  = GDALGetRasterBandXSize(dataset->GetRasterBand(1)->GetOverview(iOverview));
    tempSize.second = GDALGetRasterBandYSize(dataset->GetRasterBand(1)->GetOverview(iOverview));
    m_OverviewsSize.push_back(tempSize);

    /*std::cout << "Overviews size of input file" << m_FileName << ": "
              <<  m_OverviewsSize.back().first << " x " << m_OverviewsSize.back().second <<   std::endl; */
  }

  this->SetNumberOfComponents(m_NbBands);

  // Set the number of dimensions (verify for the dim )
  this->SetNumberOfDimensions(2);

  // Automatically set the Type to Binary for GDAL data
  this->SetFileTypeToBinary();

  // Get Data Type
  // Consider only the data type given by the first band
  // Maybe be could changed (to check)
  m_PxType->pixType = dataset->GetRasterBand(1)->GetRasterDataType();

  if (m_PxType->pixType == GDT_Byte)
  {
    SetComponentType(UCHAR);
  }
  else if (m_PxType->pixType == GDT_UInt16)
  {
    SetComponentType(USHORT);
  }
  else if (m_PxType->pixType == GDT_Int16)
  {
    SetComponentType(SHORT);
  }
  else if (m_PxType->pixType == GDT_UInt32)
  {
    SetComponentType(UINT);
  }
  else if (m_PxType->pixType == GDT_Int32)
  {
    SetComponentType(INT);
  }
  else if (m_PxType->pixType == GDT_Float32)
  {
    SetComponentType(FLOAT);
  }
  else if (m_PxType->pixType == GDT_Float64)
  {
    SetComponentType(DOUBLE);
  }
  else if (m_PxType->pixType == GDT_CInt16)
  {
    SetComponentType(CSHORT);
  }
  else if (m_PxType->pixType == GDT_CInt32)
  {
    SetComponentType(CINT);
  }
  else if (m_PxType->pixType == GDT_CFloat32)
  {
    SetComponentType(CFLOAT);
  }
  else if (m_PxType->pixType == GDT_CFloat64)
  {
    SetComponentType(CDOUBLE);
  }
  else
  {
    itkExceptionMacro(<< "Pixel type unknown");
  }

  if (this->GetComponentType() == CHAR)
  {
    m_BytePerPixel = 1;
  }
  else if (this->GetComponentType() == UCHAR)
  {
    m_BytePerPixel = 1;
  }
  else if (this->GetComponentType() == USHORT)
  {
    m_BytePerPixel = 2;
  }
  else if (this->GetComponentType() == SHORT)
  {
    m_BytePerPixel = 2;
  }
  else if (this->GetComponentType() == INT)
  {
    m_BytePerPixel = 4;
  }
  else if (this->GetComponentType() == UINT)
  {
    m_BytePerPixel = 4;
  }
  else if (this->GetComponentType() == LONG)
  {
    m_BytePerPixel = sizeof(long);
  }
  else if (this->GetComponentType() == ULONG)
  {
    m_BytePerPixel = sizeof(unsigned long);
  }
  else if (this->GetComponentType() == FLOAT)
  {
    m_BytePerPixel = 4;
  }
  else if (this->GetComponentType() == DOUBLE)
  {
    m_BytePerPixel = 8;
  }
  else if (this->GetComponentType() == CSHORT)
  {
    m_BytePerPixel = sizeof(std::complex<short>);
  }
  else if (this->GetComponentType() == CINT)
  {
    m_BytePerPixel = sizeof(std::complex<int>);
  }
  else if (this->GetComponentType() == CFLOAT)
  {
    /*if (m_PxType->pixType == GDT_CInt16)
      m_BytePerPixel = sizeof(std::complex<short>);
    else if (m_PxType->pixType == GDT_CInt32)
      m_BytePerPixel = sizeof(std::complex<int>);
    else*/
    m_BytePerPixel = sizeof(std::complex<float>);
  }
  else if (this->GetComponentType() == CDOUBLE)
  {
    m_BytePerPixel = sizeof(std::complex<double>);
  }
  else
  {
    itkExceptionMacro(<< "Component type unknown");
  }

  /******************************************************************/
  // Set the pixel type with some special cases linked to the fact
  //  we read some data with complex type.
  if (GDALDataTypeIsComplex(m_PxType->pixType)) // Try to read data with complex type with GDAL
  {
    if (!m_IsComplex && m_IsVectorImage)
    {
      // we are reading a complex data set into an image where the pixel
      // type is Vector<real>: we have to double the number of component
      // for that to work
      otbLogMacro(Warning, << "Encoding of file (" << m_FileName
                           << ") is complex but will be read as a VectorImage of scalar type, with twice the number of bands.");
      this->SetNumberOfComponents(m_NbBands * 2);
      this->SetPixelType(VECTOR);
    }
    else
    {
      this->SetPixelType(COMPLEX);
    }
  }
  else // Try to read data with scalar type with GDAL
  {
    this->SetNumberOfComponents(m_NbBands);
    if (this->GetNumberOfComponents() == 1)
    {
      this->SetPixelType(SCALAR);
    }
    else
    {
      this->SetPixelType(VECTOR);
    }
  }

  // get list of other files part of the same dataset
  char** datasetFileList = dataset->GetFileList();
  m_AttachedFileNames.clear();
  if (datasetFileList != nullptr)
  {
    char** currentFile = datasetFileList;
    while (*currentFile != nullptr)
    {
      if (m_FileName.compare(*currentFile) != 0)
      {
        m_AttachedFileNames.emplace_back(*currentFile);
        otbLogMacro(Debug, << "Found attached file : " << *currentFile);
      }
      currentFile++;
    }
    CSLDestroy(datasetFileList);
  }

  /*----------------------------------------------------------------------*/
  /*-------------------------- METADATA ----------------------------------*/
  /*----------------------------------------------------------------------*/

  // Now initialize the itk dictionary
  itk::MetaDataDictionary& dict = this->GetMetaDataDictionary();

  // Initialize the ImageMetadata structure
  ImageMetadata imd;
  m_Imd = imd;

  // Report the typical block size if possible
  if (dataset->GetRasterCount() > 0)
  {
    int blockSizeX = 0;
    int blockSizeY = 0;

    dataset->GetRasterBand(1)->GetBlockSize(&blockSizeX, &blockSizeY);

    if (blockSizeX > 0 && blockSizeY > 0)
    {
      blockSizeX = uint_ceildivpow2(blockSizeX, m_ResolutionFactor);
      if (m_Dataset->IsJPEG2000())
      {
        // Jpeg2000 case : use the real block size Y
        blockSizeY = uint_ceildivpow2(blockSizeY, m_ResolutionFactor);
      }
      else
      {
        // Try to keep the GDAL block memory constant
        blockSizeY = blockSizeY * (1 << m_ResolutionFactor);
      }

      itk::EncapsulateMetaData<unsigned int>(dict, MetaDataKey::TileHintX, blockSizeX);
      itk::EncapsulateMetaData<unsigned int>(dict, MetaDataKey::TileHintY, blockSizeY);

      m_Imd.NumericKeys[MDNum::TileHintX] = blockSizeX;
      m_Imd.NumericKeys[MDNum::TileHintY] = blockSizeY;
    }
  }

  /* -------------------------------------------------------------------- */
  /*  Get Pixel type                                                      */
  /* -------------------------------------------------------------------- */

  itk::EncapsulateMetaData<IOComponentType>(dict, MetaDataKey::DataType, this->GetComponentType());

  m_Imd.NumericKeys[MDNum::DataType] = this->GetComponentType();

  /* -------------------------------------------------------------------- */
  /*  Get Spacing                                                         */
  /* -------------------------------------------------------------------- */

  // Default Spacing
  m_Spacing[0] = 1;
  m_Spacing[1] = 1;

  // Reset origin to GDAL convention default
  m_Origin[0] = 0.0;
  m_Origin[1] = 0.0;

  // flag to detect images in sensor geometry
  bool isSensor = false;

  if (m_NumberOfDimensions == 3)
    m_Spacing[2] = 1;

  char** papszMetadata = dataset->GetMetadata(nullptr);

  /* -------------------------------------------------------------------- */
  /*      Report general info.                                            */
  /* -------------------------------------------------------------------- */
  GDALDriverH hDriver;

  hDriver = dataset->GetDriver();

  std::string driverShortName = static_cast<std::string>(GDALGetDriverShortName(hDriver));
  std::string driverLongName  = static_cast<std::string>(GDALGetDriverLongName(hDriver));

  itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::DriverShortNameKey, driverShortName);
  itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::DriverLongNameKey, driverLongName);

  if (m_Dataset->IsJPEG2000())
  {
    // store the cache size used for Jpeg2000 files
    itk::EncapsulateMetaData<unsigned int>(dict, MetaDataKey::CacheSizeInBytes, GDALGetCacheMax64());
  }

  /* -------------------------------------------------------------------- */
  /* Get the projection coordinate system of the image : ProjectionRef  */
  /* -------------------------------------------------------------------- */
  const char* pszProjection = dataset->GetProjectionRef();
  if (pszProjection != nullptr && !std::string(pszProjection).empty())
  {
    OGRSpatialReferenceH pSR = OSRNewSpatialReference(nullptr);
    if (strncmp(pszProjection, "LOCAL_CS",8) == 0)
      {
      // skip local coordinate system as they will cause crashed later
      // In GDAL 3, they begin to do special processing for Transmercator local
      // coordinate system
      otbLogMacro(Debug, << "Skipping LOCAL_CS projection")
      }
    else if (OSRImportFromWkt(pSR, (char**)(&pszProjection)) == OGRERR_NONE)
    {
      char* pszPrettyWkt = nullptr;
      OSRExportToPrettyWkt(pSR, &pszPrettyWkt, FALSE);

      itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, static_cast<std::string>(pszPrettyWkt));

      m_Imd.Add(MDGeom::ProjectionWKT, std::string(pszPrettyWkt));

      CPLFree(pszPrettyWkt);
    }
    else
    {
      itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, static_cast<std::string>(pszProjection));
      m_Imd.Add(MDGeom::ProjectionWKT, std::string(pszProjection));
    }

    if (pSR != nullptr)
    {
      OSRRelease(pSR);
      pSR = nullptr;
    }
  }
  else
  {
    // Special case for Jpeg2000 files : try to read the origin in the GML box
    if (m_Dataset->IsJPEG2000())
    {
      isSensor = GetOriginFromGMLBox(m_Origin);
    }
  }

  /* -------------------------------------------------------------------- */
  /* Get the GCP projection coordinates of the image : GCPProjection  */
  /* -------------------------------------------------------------------- */

  unsigned int gcpCount = 0;
  gcpCount              = dataset->GetGCPCount();
  if (gcpCount > 0)
  {
    std::string gcpProjectionKey;
    Projection::GCPParam gcps;

    {
      // Declare gcpProj in local scope. So, it won't be available outside.
      const char* gcpProj = dataset->GetGCPProjection();

      // assert( gcpProj!=NULL );

      if (gcpProj != nullptr)
        gcpProjectionKey = gcpProj;
    }

    itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::GCPProjectionKey, gcpProjectionKey);

    gcps.GCPProjection = gcpProjectionKey;

    if (gcpProjectionKey.empty())
    {
      gcpCount = 0; // fix for uninitialized gcpCount in gdal (when
      // reading Palsar image)
    }

    std::string key;

    itk::EncapsulateMetaData<unsigned int>(dict, MetaDataKey::GCPCountKey, gcpCount);

    for (unsigned int cpt = 0; cpt < gcpCount; ++cpt)
    {

      const GDAL_GCP* psGCP;
      psGCP = dataset->GetGCPs() + cpt;

      OTB_GCP pOtbGCP;
      pOtbGCP.m_Id     = std::string(psGCP->pszId);
      pOtbGCP.m_Info   = std::string(psGCP->pszInfo);
      pOtbGCP.m_GCPRow = psGCP->dfGCPLine;
      pOtbGCP.m_GCPCol = psGCP->dfGCPPixel;
      pOtbGCP.m_GCPX   = psGCP->dfGCPX;
      pOtbGCP.m_GCPY   = psGCP->dfGCPY;
      pOtbGCP.m_GCPZ   = psGCP->dfGCPZ;

      // Complete the key with the GCP number : GCP_i
      std::ostringstream lStream;
      lStream << MetaDataKey::GCPParametersKey << cpt;
      key = lStream.str();

      itk::EncapsulateMetaData<OTB_GCP>(dict, key, pOtbGCP);
      gcps.GCPs.push_back(pOtbGCP);
    }
    m_Imd.Add(MDGeom::GCP, gcps);
  }

  /* -------------------------------------------------------------------- */
  /*  Get the six coefficients of affine geoTtransform      */
  /* -------------------------------------------------------------------- */

  double                  adfGeoTransform[6];
  MetaDataKey::VectorType VadfGeoTransform;

  if (dataset->GetGeoTransform(adfGeoTransform) == CE_None)
  {
    for (int cpt = 0; cpt < 6; ++cpt)
      {
      VadfGeoTransform.push_back(adfGeoTransform[cpt]);
      //~ m_Imd.GeoTransform[cpt] = adfGeoTransform[cpt];
      }

    itk::EncapsulateMetaData<MetaDataKey::VectorType>(dict, MetaDataKey::GeoTransformKey, VadfGeoTransform);

    if (!isSensor)
    {
      /// retrieve origin and spacing from the geo transform
      m_Spacing[0] = VadfGeoTransform[1];
      m_Spacing[1] = VadfGeoTransform[5];

      if (m_Spacing[0] == 0 || m_Spacing[1] == 0)
      {
        // Manage case where axis are not standard
        if (VadfGeoTransform[2] != 0 && VadfGeoTransform[4] != 0)
        {
          m_Spacing[0] = VadfGeoTransform[2];
          m_Spacing[1] = VadfGeoTransform[4];
        }
        else
        {
          otbLogMacro(Warning, << "Geotransform reported by GDAL is invalid (spacing = 0)");
          m_Spacing[0] = 1;
          m_Spacing[1] = 1;
        }
      }
      // Geotransforms with a non-null rotation are not supported
      // Beware : GDAL origin is at the corner of the top-left pixel
      // whereas OTB/ITK origin is at the centre of the top-left pixel
      // The origin computed here is in GDAL convention for now
      m_Origin[0] = VadfGeoTransform[0];
      m_Origin[1] = VadfGeoTransform[3];
    }
  }

  // Compute final spacing with the resolution factor
  m_Spacing[0] *= std::pow(2.0, static_cast<double>(m_ResolutionFactor));
  m_Spacing[1] *= std::pow(2.0, static_cast<double>(m_ResolutionFactor));
  // Now that the spacing is known, apply the half-pixel shift
  m_Origin[0] += 0.5 * m_Spacing[0];
  m_Origin[1] += 0.5 * m_Spacing[1];

  /* -------------------------------------------------------------------- */
  /*      Report metadata.                                                */
  /* -------------------------------------------------------------------- */

  papszMetadata = dataset->GetMetadata(nullptr);
  if (CSLCount(papszMetadata) > 0)
  {
    std::string key;

    for (int cpt = 0; papszMetadata[cpt] != nullptr; ++cpt)
    {
      std::ostringstream lStream;
      lStream << MetaDataKey::MetadataKey << cpt;
      key = lStream.str();

      itk::EncapsulateMetaData<std::string>(dict, key, static_cast<std::string>(papszMetadata[cpt]));
    }
  }

  /* Special case for JPEG2000, also look in the GML boxes */
  if (m_Dataset->IsJPEG2000())
  {
    char**          gmlMetadata = nullptr;
    GDALJP2Metadata jp2Metadata;
    if (jp2Metadata.ReadAndParse(m_FileName.c_str()))
    {
      gmlMetadata = jp2Metadata.papszGMLMetadata;
    }

    if (gmlMetadata)
    {
      if (CSLCount(gmlMetadata) > 0)
      {
        std::string key;
        int         cptOffset = CSLCount(papszMetadata);

        for (int cpt = 0; gmlMetadata[cpt] != nullptr; ++cpt)
        {
          std::ostringstream lStream;
          lStream << MetaDataKey::MetadataKey << (cpt + cptOffset);
          key = lStream.str();

          itk::EncapsulateMetaData<std::string>(dict, key, static_cast<std::string>(gmlMetadata[cpt]));
        }
      }
    }
  }


  /* -------------------------------------------------------------------- */
  /*      Report subdatasets.                                             */
  /* -------------------------------------------------------------------- */

  papszMetadata = dataset->GetMetadata("SUBDATASETS");
  if (CSLCount(papszMetadata) > 0)
  {
    std::string key;

    for (int cpt = 0; papszMetadata[cpt] != nullptr; ++cpt)
    {
      std::ostringstream lStream;
      lStream << MetaDataKey::SubMetadataKey << cpt;
      key = lStream.str();

      itk::EncapsulateMetaData<std::string>(dict, key, static_cast<std::string>(papszMetadata[cpt]));
    }
  }

  /* -------------------------------------------------------------------- */
  /* Report corners              */
  /* -------------------------------------------------------------------- */

  double                  GeoX(0), GeoY(0);
  MetaDataKey::VectorType VGeo;

  GDALInfoReportCorner("Upper Left", 0.0, 0.0, GeoX, GeoY);
  VGeo.push_back(GeoX);
  VGeo.push_back(GeoY);

  itk::EncapsulateMetaData<MetaDataKey::VectorType>(dict, MetaDataKey::UpperLeftCornerKey, VGeo);
  //~ m_Imd.ULX = GeoX;
  //~ m_Imd.ULY = GeoY;

  VGeo.clear();

  GDALInfoReportCorner("Upper Right", m_Dimensions[0], 0.0, GeoX, GeoY);
  VGeo.push_back(GeoX);
  VGeo.push_back(GeoY);

  itk::EncapsulateMetaData<MetaDataKey::VectorType>(dict, MetaDataKey::UpperRightCornerKey, VGeo);
  //~ m_Imd.URX = GeoX;
  //~ m_Imd.URY = GeoY;

  VGeo.clear();

  GDALInfoReportCorner("Lower Left", 0.0, m_Dimensions[1], GeoX, GeoY);
  VGeo.push_back(GeoX);
  VGeo.push_back(GeoY);

  itk::EncapsulateMetaData<MetaDataKey::VectorType>(dict, MetaDataKey::LowerLeftCornerKey, VGeo);
  //~ m_Imd.LLX = GeoX;
  //~ m_Imd.LLY = GeoY;

  VGeo.clear();

  GDALInfoReportCorner("Lower Right", m_Dimensions[0], m_Dimensions[1], GeoX, GeoY);
  VGeo.push_back(GeoX);
  VGeo.push_back(GeoY);

  itk::EncapsulateMetaData<MetaDataKey::VectorType>(dict, MetaDataKey::LowerRightCornerKey, VGeo);
  //~ m_Imd.LRX = GeoX;
  //~ m_Imd.LRY = GeoY;

  VGeo.clear();

  /* -------------------------------------------------------------------- */
  /* Color Table                                                          */
  /* -------------------------------------------------------------------- */

  for (int iBand = 0; iBand < dataset->GetRasterCount(); iBand++)
  {
    GDALColorTableH hTable;
    GDALRasterBandH hBand;
    hBand = GDALGetRasterBand(dataset, iBand + 1);
    if ((GDALGetRasterColorInterpretation(hBand) == GCI_PaletteIndex) && (hTable = GDALGetRasterColorTable(hBand)) != nullptr)
    {

      // Mantis: 1049 : OTB does not handle tif with NBITS=1 properly
      // When a palette is available and pixel type is Byte, the image is
      // automatically read as a color image (using the palette). Perhaps this
      // behaviour should be restricted.  Comment color table interpretation in
      // gdalimageio

      // FIXME: Better support of color table in OTB
      // - disable palette conversion in GDALImageIO (the comments in this part
      // of the code are rather careful)
      // - GDALImageIO should report the palette to ImageFileReader (as a metadata ?
      // a kind of LUT ?).
      // - ImageFileReader should use a kind of adapter filter to convert the mono
      // image into color.

      // Do not set indexed image attribute to true
      // m_IsIndexed = true;

      unsigned int ColorEntryCount = GDALGetColorEntryCount(hTable);

      itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ColorTableNameKey,
                                            static_cast<std::string>(GDALGetPaletteInterpretationName(GDALGetPaletteInterpretation(hTable))));

      itk::EncapsulateMetaData<unsigned int>(dict, MetaDataKey::ColorEntryCountKey, ColorEntryCount);

      for (int i = 0; i < GDALGetColorEntryCount(hTable); ++i)
      {
        GDALColorEntry          sEntry;
        MetaDataKey::VectorType VColorEntry;

        GDALGetColorEntryAsRGB(hTable, i, &sEntry);

        VColorEntry.push_back(sEntry.c1);
        VColorEntry.push_back(sEntry.c2);
        VColorEntry.push_back(sEntry.c3);
        VColorEntry.push_back(sEntry.c4);

        itk::EncapsulateMetaData<MetaDataKey::VectorType>(dict, MetaDataKey::ColorEntryAsRGBKey, VColorEntry);
      }
    }
  }
  if (m_IsIndexed)
  {
    m_NbBands *= 4;
    this->SetNumberOfComponents(m_NbBands);
    this->SetPixelType(VECTOR);
  }


  // Read no data value if present
  std::vector<bool>   isNoDataAvailable(dataset->GetRasterCount(), false);
  std::vector<double> noDataValues(dataset->GetRasterCount(), 0);

  bool noDataFound = false;
  ImageMetadataBase bmd;

  for (int iBand = 0; iBand < dataset->GetRasterCount(); iBand++)
  {
    GDALRasterBandH hBand = GDALGetRasterBand(dataset, iBand + 1);

    int success;

    double ndv = GDALGetRasterNoDataValue(hBand, &success);

    if (success)
    {
      noDataFound              = true;
      isNoDataAvailable[iBand] = true;
      noDataValues[iBand]      = ndv;
      bmd.Add(MDNum::NoData, ndv);
    }
    m_Imd.Bands.push_back(bmd);
  }

  if (noDataFound)
  {
    itk::EncapsulateMetaData<MetaDataKey::BoolVectorType>(dict, MetaDataKey::NoDataValueAvailable, isNoDataAvailable);
    itk::EncapsulateMetaData<MetaDataKey::VectorType>(dict, MetaDataKey::NoDataValue, noDataValues);
  }
}

bool GDALImageIO::CanWriteFile(const char* name)
{
  // First check the filename
  if (name == nullptr)
  {
    return false;
  }

  m_FileName = name;

  // Get the GDAL format ID from the name
  std::string gdalDriverShortName = FilenameToGdalDriverShortName(name);
  if (gdalDriverShortName == "NOT-FOUND")
  {
    return false;
  }

  // Check the driver for support of Create or at least CreateCopy
  GDALDriver* driver = GDALDriverManagerWrapper::GetInstance().GetDriverByName(gdalDriverShortName);
  if (GDALGetMetadataItem(driver, GDAL_DCAP_CREATE, nullptr) == nullptr && GDALGetMetadataItem(driver, GDAL_DCAP_CREATECOPY, nullptr) == nullptr)
  {
    otbLogMacro(Warning, << "GDAL driver " << GDALGetDriverShortName(driver) << " does not support writing");
    return false;
  }
  return true;
}

bool GDALImageIO::CanStreamWrite()
{
  // Get the GDAL format ID from the name
  std::string gdalDriverShortName = FilenameToGdalDriverShortName(m_FileName);
  GDALDriver* driver              = GDALDriverManagerWrapper::GetInstance().GetDriverByName(gdalDriverShortName);

  if (driver == nullptr)
  {
    m_CanStreamWrite = false;
  }
  if (GDALGetMetadataItem(driver, GDAL_DCAP_CREATE, nullptr) != nullptr)
  {
    m_CanStreamWrite = true;
  }
  else
  {
    m_CanStreamWrite = false;
  }
  return m_CanStreamWrite;
}

void GDALImageIO::Write(const void* buffer)
{
  // Check if we have to write the image information
  if (m_FlagWriteImageInformation == true)
  {
    this->InternalWriteImageInformation(buffer);
    m_FlagWriteImageInformation = false;
  }

  // Check if conversion succeed
  if (buffer == nullptr)
  {
    itkExceptionMacro(<< "Null buffer passed to GDALImageIO for writing.");
    return;
  }

  // Compute offset and size
  unsigned int lNbLines     = this->GetIORegion().GetSize()[1];
  unsigned int lNbColumns   = this->GetIORegion().GetSize()[0];
  int          lFirstLine   = this->GetIORegion().GetIndex()[1]; // [1... ]
  int          lFirstColumn = this->GetIORegion().GetIndex()[0]; // [1... ]

  // Particular case: checking that the written region is the same size
  // of the entire image
  // starting at offset 0 (when no streaming)
  if ((lNbLines == m_Dimensions[1]) && (lNbColumns == m_Dimensions[0]))
  {
    lFirstLine   = 0;
    lFirstColumn = 0;
  }

  // If needed, set the coordinate reference
  if (m_epsgCode != 0)
  {
    auto spatialReference = SpatialReference::FromEPSG(m_epsgCode);
    m_Dataset->GetDataSet()->SetProjection(spatialReference.ToWkt().c_str());
  }

  // Convert buffer from void * to unsigned char *
  // unsigned char *p = static_cast<unsigned char*>( const_cast<void *>(buffer));
  // printDataBuffer(p,  m_PxType->pixType, m_NbBands, 10*2); // Buffer incorrect

  // If driver supports streaming
  if (m_CanStreamWrite)
  {
    otbLogMacro(Debug, << "GDAL writes [" << lFirstColumn << ", " << lFirstColumn + lNbColumns - 1 << "]x[" << lFirstLine << ", " << lFirstLine + lNbLines - 1
                       << "] x " << m_NbBands << " bands of type " << GDALGetDataTypeName(m_PxType->pixType) << " to file " << m_FileName);

    otb::Stopwatch chrono  = otb::Stopwatch::StartNew();
    CPLErr         lCrGdal = m_Dataset->GetDataSet()->RasterIO(GF_Write, lFirstColumn, lFirstLine, lNbColumns, lNbLines, const_cast<void*>(buffer), lNbColumns,
                                                       lNbLines, m_PxType->pixType, m_NbBands,
                                                       // We want to write all bands
                                                       nullptr,
                                                       // Pixel offset
                                                       // is nbComp * BytePerPixel
                                                       m_BytePerPixel * m_NbBands,
                                                       // Line offset
                                                       // is pixelOffset * nbColumns
                                                       m_BytePerPixel * m_NbBands * lNbColumns,
                                                       // Band offset is BytePerPixel
                                                       m_BytePerPixel);
    chrono.Stop();

    // Check if writing succeed
    if (lCrGdal == CE_Failure)
    {
      itkExceptionMacro(<< "Error while writing image (GDAL format) '" << m_FileName << "' : " << CPLGetLastErrorMsg());
    }

    otbLogMacro(Debug, << "GDAL write took " << chrono.GetElapsedMilliseconds() << " ms")

        // Flush dataset cache
        m_Dataset->GetDataSet()
            ->FlushCache();
  }
  else
  {
    // We only wrote data to the memory dataset
    // Now write it to the real file with CreateCopy()
    std::string gdalDriverShortName = FilenameToGdalDriverShortName(m_FileName);
    std::string realFileName        = GetGdalWriteImageFileName(gdalDriverShortName, m_FileName);

    GDALDriver* driver = GDALDriverManagerWrapper::GetInstance().GetDriverByName(gdalDriverShortName);
    if (driver == nullptr)
    {
      itkExceptionMacro(<< "Unable to instantiate driver " << gdalDriverShortName << " to write " << m_FileName);
    }

    GDALCreationOptionsType creationOptions = m_CreationOptions;
    GDALDataset*            hOutputDS =
        driver->CreateCopy(realFileName.c_str(), m_Dataset->GetDataSet(), FALSE, otb::ogr::StringListConverter(creationOptions).to_ogr(), nullptr, nullptr);
    if (!hOutputDS)
    {
      itkExceptionMacro(<< "Error while writing image (GDAL format) '" << m_FileName << "' : " << CPLGetLastErrorMsg());
    }
    else
    {
      GDALClose(hOutputDS);
    }
  }


  if (lFirstLine + lNbLines == m_Dimensions[1] && lFirstColumn + lNbColumns == m_Dimensions[0])
  {
    // Last pixel written
    // Reinitialize to close the file
    m_Dataset = GDALDatasetWrapperPointer();
  }
}

/** TODO : Methode WriteImageInformation non implementee */
void GDALImageIO::WriteImageInformation()
{
}

void GDALImageIO::InternalWriteImageInformation(const void* buffer)
{
  // char **     papszOptions = NULL;
  std::string driverShortName;
  m_NbBands = this->GetNumberOfComponents();
  
  // If the band mapping is different from the one of the input (e.g. because an extended filename
  // has been set, the bands in the imageMetadata object needs to be reorganized.
  if (!m_BandList.empty())
  {
    ImageMetadata::ImageMetadataBandsType bandRangeMetadata;
    for (auto elem: m_BandList)
    {
      bandRangeMetadata.push_back(m_Imd.Bands[elem]);
    }
    m_Imd.Bands = bandRangeMetadata;
  }

  // TODO : this should be a warning instead of an exception
  // For complex pixels the number of bands is twice the number of compnents (in GDAL sense)
  if ( !m_Imd.Bands.empty() 
    && static_cast<std::size_t>(m_NbBands) != m_Imd.Bands.size()
    && !((m_Imd.Bands.size() == static_cast<std::size_t>(2 * m_NbBands)) && this->GetPixelType() == COMPLEX))
  {
      itkExceptionMacro(<< "Number of bands in metadata inconsistent with actual image.");
  }

  if ((m_Dimensions[0] == 0) && (m_Dimensions[1] == 0))
  {
    itkExceptionMacro(<< "Dimensions are not defined.");
  }

  if ((this->GetPixelType() == COMPLEX) /*&& (m_NbBands / 2 > 0)*/)
  {
    // m_NbBands /= 2;

    if (this->GetComponentType() == CSHORT)
    {
      m_BytePerPixel    = 4;
      m_PxType->pixType = GDT_CInt16;
    }
    else if (this->GetComponentType() == CINT)
    {
      m_BytePerPixel    = 8;
      m_PxType->pixType = GDT_CInt32;
    }
    else if (this->GetComponentType() == CFLOAT)
    {
      m_BytePerPixel    = 8;
      m_PxType->pixType = GDT_CFloat32;
    }
    else if (this->GetComponentType() == CDOUBLE)
    {
      m_BytePerPixel    = 16;
      m_PxType->pixType = GDT_CFloat64;
    }
    else
    {
      itkExceptionMacro(<< "This complex type is not defined :" << ImageIOBase::GetPixelTypeAsString(this->GetPixelType()));
    }
  }
  else
  {
    if (this->GetComponentType() == CHAR)
    {
      m_BytePerPixel    = 1;
      m_PxType->pixType = GDT_Byte;
    }
    else if (this->GetComponentType() == UCHAR)
    {
      m_BytePerPixel    = 1;
      m_PxType->pixType = GDT_Byte;
    }
    else if (this->GetComponentType() == USHORT)
    {
      m_BytePerPixel    = 2;
      m_PxType->pixType = GDT_UInt16;
    }
    else if (this->GetComponentType() == SHORT)
    {
      m_BytePerPixel    = 2;
      m_PxType->pixType = GDT_Int16;
    }
    else if (this->GetComponentType() == INT)
    {
      m_BytePerPixel    = 4;
      m_PxType->pixType = GDT_Int32;
    }
    else if (this->GetComponentType() == UINT)
    {
      m_BytePerPixel    = 4;
      m_PxType->pixType = GDT_UInt32;
    }
    else if (this->GetComponentType() == LONG)
    {
      m_BytePerPixel = sizeof(long);
      if (m_BytePerPixel == 8)
      {
        itkWarningMacro(<< "Cast a long (64 bits) image into an int (32 bits) one.")
      }
      m_PxType->pixType = GDT_Int32;
    }
    else if (this->GetComponentType() == ULONG)
    {
      m_BytePerPixel = sizeof(unsigned long);
      if (m_BytePerPixel == 8)
      {
        itkWarningMacro(<< "Cast an unsigned long (64 bits) image into an unsigned int (32 bits) one.")
      }
      m_PxType->pixType = GDT_UInt32;
    }
    else if (this->GetComponentType() == FLOAT)
    {
      m_BytePerPixel    = 4;
      m_PxType->pixType = GDT_Float32;
    }
    else if (this->GetComponentType() == DOUBLE)
    {
      m_BytePerPixel    = 8;
      m_PxType->pixType = GDT_Float64;
    }
    else
    {
      m_BytePerPixel    = 1;
      m_PxType->pixType = GDT_Byte;
    }
  }

  // Automatically set the Type to Binary for GDAL data
  this->SetFileTypeToBinary();

  driverShortName = FilenameToGdalDriverShortName(m_FileName);
  if (driverShortName == "NOT-FOUND")
  {
    itkExceptionMacro(<< "GDAL Writing failed: the image file name '" << m_FileName << "' is not recognized by GDAL.");
  }

  if (m_CanStreamWrite)
  {
    GDALCreationOptionsType creationOptions = m_CreationOptions;
    m_Dataset =
        GDALDriverManagerWrapper::GetInstance().Create(driverShortName, GetGdalWriteImageFileName(driverShortName, m_FileName), m_Dimensions[0],
                                                       m_Dimensions[1], m_NbBands, m_PxType->pixType, otb::ogr::StringListConverter(creationOptions).to_ogr());
  }
  else
  {
    // buffer casted in unsigned long cause under Win32 the address
    // doesn't begin with 0x, the address in not interpreted as
    // hexadecimal but alpha numeric value, then the conversion to
    // integer make us pointing to an non allowed memory block => Crash.
    // use intptr_t to cast void* to unsigned long. included stdint.h for
    // uintptr_t typedef.
    std::ostringstream stream;
    stream << "MEM:::"
           << "DATAPOINTER=" << (uintptr_t)(buffer) << ","
           << "PIXELS=" << m_Dimensions[0] << ","
           << "LINES=" << m_Dimensions[1] << ","
           << "BANDS=" << m_NbBands << ","
           << "DATATYPE=" << GDALGetDataTypeName(m_PxType->pixType) << ","
           << "PIXELOFFSET=" << m_BytePerPixel * m_NbBands << ","
           << "LINEOFFSET=" << m_BytePerPixel * m_NbBands * m_Dimensions[0] << ","
           << "BANDOFFSET=" << m_BytePerPixel;

    m_Dataset = GDALDriverManagerWrapper::GetInstance().Open(stream.str());
  }

  if (m_Dataset.IsNull())
  {
    itkExceptionMacro(<< CPLGetLastErrorMsg());
  }

  /*----------------------------------------------------------------------*/
  /*-------------------------- METADATA ----------------------------------*/
  /*----------------------------------------------------------------------*/

  std::ostringstream       oss;
  GDALDataset*             dataset = m_Dataset->GetDataSet();

  // In OTB we can have simultaneously projection ref, sensor keywordlist, GCPs
  // but GDAL can't handle both geotransform and GCP (see issue #303). Here is the priority
  // order we will be using in OTB:
  // [ProjRef+geotransform] > [SensorKeywordlist+geotransform] > [GCPs]

  /* -------------------------------------------------------------------- */
  /* Pre-compute geotransform                                             */
  /* -------------------------------------------------------------------- */
  const double Epsilon = 1E-10;
  double geoTransform[6];
  geoTransform[0] = m_Origin[0] - 0.5 * m_Spacing[0] * m_Direction[0][0];
  geoTransform[3] = m_Origin[1] - 0.5 * m_Spacing[1] * m_Direction[1][1];
  geoTransform[1] = m_Spacing[0] * m_Direction[0][0];
  geoTransform[5] = m_Spacing[1] * m_Direction[1][1];
  // FIXME: Here component 1 and 4 should be replaced by the orientation parameters
  geoTransform[2] = 0.;
  geoTransform[4] = 0.;
  // only write geotransform if it has non-default values
  bool writeGeotransform =
    std::abs(geoTransform[0]) > Epsilon ||
    std::abs(geoTransform[1] - 1.0) > Epsilon ||
    std::abs(geoTransform[2]) > Epsilon ||
    std::abs(geoTransform[3]) > Epsilon ||
    std::abs(geoTransform[4]) > Epsilon ||
    std::abs(geoTransform[5] - 1.0) > Epsilon;

  itk::MetaDataDictionary& dict = this->GetMetaDataDictionary();
  ImageKeywordlist otb_kwl;
  itk::ExposeMetaData<ImageKeywordlist>(dict, MetaDataKey::OSSIMKeywordlistKey, otb_kwl);

  /* -------------------------------------------------------------------- */
  /* Case 1: Set the projection coordinate system of the image            */
  /* -------------------------------------------------------------------- */
  if (m_Imd.Has(MDGeom::ProjectionWKT))
    {
    std::string projectionRef( m_Imd.GetProjectionWKT() );
    dataset->SetProjection(projectionRef.c_str());
    }
  /* -------------------------------------------------------------------- */
  /* Case 2: Sensor geometry                                              */
  /* -------------------------------------------------------------------- */
  else if (m_Imd.HasSensorGeometry())
    {
    /* -------------------------------------------------------------------- */
    /* Set the RPC coeffs (since GDAL 1.10.0)                               */
    /* -------------------------------------------------------------------- */
    if (m_WriteRPCTags && m_Imd.Has(MDGeom::RPC))
      {
      const Projection::RPCParam & rpc = boost::any_cast<const Projection::RPCParam&>(m_Imd[MDGeom::RPC]);
      otbLogMacro(Debug, << "Saving RPC to file (" << m_FileName << ")")
      GDALRPCInfo gdalRpcStruct;
      gdalRpcStruct.dfSAMP_OFF     = rpc.SampleOffset;
      gdalRpcStruct.dfLINE_OFF     = rpc.LineOffset;
      gdalRpcStruct.dfSAMP_SCALE   = rpc.SampleScale;
      gdalRpcStruct.dfLINE_SCALE   = rpc.LineScale;
      gdalRpcStruct.dfLAT_OFF      = rpc.LatOffset;
      gdalRpcStruct.dfLONG_OFF     = rpc.LonOffset;
      gdalRpcStruct.dfHEIGHT_OFF   = rpc.HeightOffset;
      gdalRpcStruct.dfLAT_SCALE    = rpc.LatScale;
      gdalRpcStruct.dfLONG_SCALE   = rpc.LonScale;
      gdalRpcStruct.dfHEIGHT_SCALE = rpc.HeightScale;

      memcpy(gdalRpcStruct.adfLINE_NUM_COEFF, rpc.LineNum, sizeof(double) * 20);
      memcpy(gdalRpcStruct.adfLINE_DEN_COEFF, rpc.LineDen, sizeof(double) * 20);
      memcpy(gdalRpcStruct.adfSAMP_NUM_COEFF, rpc.SampleNum, sizeof(double) * 20);
      memcpy(gdalRpcStruct.adfSAMP_DEN_COEFF, rpc.SampleDen, sizeof(double) * 20);
      char** rpcMetadata = RPCInfoToMD(&gdalRpcStruct);
      dataset->SetMetadata(rpcMetadata, "RPC");
      CSLDestroy(rpcMetadata);
      }
    }
  // ToDo : remove this part. This case is here for compatibility for images
  // that still use Ossim for managing the sensor model (with OSSIMKeywordList).
  else if (otb_kwl.GetSize())
    {
    /* -------------------------------------------------------------------- */
    /* Set the RPC coeffs (since GDAL 1.10.0)                               */
    /* -------------------------------------------------------------------- */
    if (m_WriteRPCTags)
      {
      GDALRPCInfo gdalRpcStruct;
      if (otb_kwl.convertToGDALRPC(gdalRpcStruct))
        {
        otbLogMacro(Debug, << "Saving RPC to file (" << m_FileName << ")")
        char** rpcMetadata = RPCInfoToMD(&gdalRpcStruct);
        dataset->SetMetadata(rpcMetadata, "RPC");
        CSLDestroy(rpcMetadata);
        }
      }
    }
  /* -------------------------------------------------------------------- */
  /* Case 3: Set the GCPs                                                 */
  /* -------------------------------------------------------------------- */
  else if (m_Imd.Has(MDGeom::GCP))
    {
    otbLogMacro(Debug, << "Saving GCPs to file (" << m_FileName << ")")
    const Projection::GCPParam & gcpPrm =
      boost::any_cast<const Projection::GCPParam&>(m_Imd[MDGeom::GCP]);
    std::vector<GDAL_GCP> gdalGcps(gcpPrm.GCPs.size());
    for (unsigned int gcpIndex = 0; gcpIndex < gdalGcps.size(); ++gcpIndex)
      {
      const OTB_GCP &gcp = gcpPrm.GCPs[gcpIndex];

      gdalGcps[gcpIndex].pszId      = const_cast<char*>(gcp.m_Id.c_str());
      gdalGcps[gcpIndex].pszInfo    = const_cast<char*>(gcp.m_Info.c_str());
      gdalGcps[gcpIndex].dfGCPPixel = gcp.m_GCPCol;
      gdalGcps[gcpIndex].dfGCPLine  = gcp.m_GCPRow;
      gdalGcps[gcpIndex].dfGCPX     = gcp.m_GCPX;
      gdalGcps[gcpIndex].dfGCPY     = gcp.m_GCPY;
      gdalGcps[gcpIndex].dfGCPZ     = gcp.m_GCPZ;

      if (writeGeotransform)
        {
        // we need to transform GCP col and row accordingly
        // WARNING: assume no rotation is there
        gdalGcps[gcpIndex].dfGCPPixel = (gcp.m_GCPCol - geoTransform[0]) / geoTransform[1];
        gdalGcps[gcpIndex].dfGCPLine  = (gcp.m_GCPRow - geoTransform[3]) / geoTransform[5];
        }
      }

    const std::string & gcpProjectionRef = gcpPrm.GCPProjection;
    dataset->SetGCPs(gdalGcps.size(), gdalGcps.data(), gcpProjectionRef.c_str());

    // disable geotransform with GCP
    writeGeotransform = false;
    }

  /* -------------------------------------------------------------------- */
  /*  Save geotransform if needed.                                        */
  /* -------------------------------------------------------------------- */
  if (writeGeotransform)
    {
    if ( driverShortName == "ENVI" && geoTransform[5] > 0.)
      {
      // Error if writing to a ENVI file with a positive Y spacing
      itkExceptionMacro(<< "Can not write to ENVI file format with a positive Y spacing (" << m_FileName << ")");
      }
    dataset->SetGeoTransform(geoTransform);
    }

  /* -------------------------------------------------------------------- */
  /*      Report metadata.                                                */
  /* -------------------------------------------------------------------- */
  ExportMetadata();

  /* -------------------------------------------------------------------- */
  /*      No Data.                                                        */
  /* -------------------------------------------------------------------- */
  
  // Write no-data flags from ImageMetadata
  int iBand = 0;
  for (auto const&  bandMD : m_Imd.Bands)
    {
    if (bandMD.Has(MDNum::NoData))
      {
        dataset->GetRasterBand(iBand + 1)->SetNoDataValue(bandMD[MDNum::NoData]);
      }
    ++iBand;
    }

  // Write no-data flags from extended filenames
  for (auto const& noData : m_NoDataList)
    dataset->GetRasterBand(noData.first)->SetNoDataValue(noData.second);
}

std::string GDALImageIO::FilenameToGdalDriverShortName(const std::string& name) const
{
  std::string extension;
  std::string gdalDriverShortName;

  // Get extension in lowercase
  extension = itksys::SystemTools::LowerCase(itksys::SystemTools::GetFilenameLastExtension(name));

  if (extension == ".tif" || extension == ".tiff")
    gdalDriverShortName = "GTiff";
  else if (extension == ".hdr")
    gdalDriverShortName = "ENVI";
  else if (extension == ".img")
    gdalDriverShortName = "HFA";
  else if (extension == ".ntf")
    gdalDriverShortName = "NITF";
  else if (extension == ".png")
    gdalDriverShortName = "PNG";
  else if (extension == ".jpg" || extension == ".jpeg")
    gdalDriverShortName = "JPEG";
  else if (extension == ".pix")
    gdalDriverShortName = "PCIDSK";
  else if (extension == ".lbl" || extension == ".pds")
    gdalDriverShortName = "ISIS2";
  else if (extension == ".j2k" || extension == ".jp2" || extension == ".jpx")
  {
    // Try different JPEG2000 drivers
    GDALDriver* driver = nullptr;
    driver             = GDALDriverManagerWrapper::GetInstance().GetDriverByName("JP2OpenJPEG");
    if (driver)
    {
      gdalDriverShortName = "JP2OpenJPEG";
    }

    if (!driver)
    {
      driver = GDALDriverManagerWrapper::GetInstance().GetDriverByName("JP2KAK");
      if (driver)
      {
        gdalDriverShortName = "JP2KAK";
      }
    }

    if (!driver)
    {
      driver = GDALDriverManagerWrapper::GetInstance().GetDriverByName("JP2ECW");
      if (driver)
      {
        gdalDriverShortName = "JP2ECW";
      }
    }

    if (!driver)
    {
      gdalDriverShortName = "NOT-FOUND";
    }
  }

  else
    gdalDriverShortName = "NOT-FOUND";

  return gdalDriverShortName;
}

bool GDALImageIO::GetOriginFromGMLBox(std::vector<double>& origin)
{
  GDALJP2Metadata jp2Metadata;
  if (!jp2Metadata.ReadAndParse(m_FileName.c_str()))
  {
    return false;
  }

  if (!jp2Metadata.papszGMLMetadata)
  {
    return false;
  }

  std::string gmlString = static_cast<std::string>(jp2Metadata.papszGMLMetadata[0]);
  gmlString.erase(0, 18); // We need to remove first part to create a true xml stream

  TiXmlDocument doc;
  doc.Parse(gmlString.c_str()); // Create xml doc from a string

  TiXmlHandle   docHandle(&doc);
  TiXmlElement* originTag = docHandle.FirstChild("gml:FeatureCollection")
                                .FirstChild("gml:featureMember")
                                .FirstChild("gml:FeatureCollection")
                                .FirstChild("gml:featureMember")
                                .FirstChild("gml:GridCoverage")
                                .FirstChild("gml:gridDomain")
                                .FirstChild("gml:Grid")
                                .FirstChild("gml:limits")
                                .FirstChild("gml:GridEnvelope")
                                .FirstChild("gml:low")
                                .ToElement();
  if (!originTag)
  {
    return false;
  }

  std::vector<itksys::String> originValues;
  originValues = itksys::SystemTools::SplitString(originTag->GetText(), ' ', false);

  // Compute origin in GDAL convention (the half-pixel shift is applied later)
  std::istringstream ss0(originValues[0]);
  std::istringstream ss1(originValues[1]);
  ss0 >> origin[1];
  ss1 >> origin[0];
  origin[0] += -1.0;
  origin[1] += -1.0;

  return true;
}

std::string GDALImageIO::GetGdalWriteImageFileName(const std::string& gdalDriverShortName, const std::string& filename) const
{
  std::string gdalFileName;

  gdalFileName = filename;
  // Suppress hdr extension for ENVI format
  if (gdalDriverShortName == "ENVI")
  {
    gdalFileName = System::GetRootName(filename);
  }
  return gdalFileName;
}

bool GDALImageIO::GDALInfoReportCorner(const char* /*corner_name*/, double x, double y, double& GeoX, double& GeoY) const
{
  double adfGeoTransform[6];
  bool   IsTrue;

  /* -------------------------------------------------------------------- */
  /*      Transform the point into georeferenced coordinates.             */
  /* -------------------------------------------------------------------- */
  if (m_Dataset->GetDataSet()->GetGeoTransform(adfGeoTransform) == CE_None)
  {
    GeoX   = adfGeoTransform[0] + adfGeoTransform[1] * x + adfGeoTransform[2] * y;
    GeoY   = adfGeoTransform[3] + adfGeoTransform[4] * x + adfGeoTransform[5] * y;
    IsTrue = true;
  }

  else
  {
    GeoX   = x;
    GeoY   = y;
    IsTrue = false;
  }

  return IsTrue;
}

bool GDALImageIO::CreationOptionContains(std::string partialOption) const
{
  size_t i;
  for (i = 0; i < m_CreationOptions.size(); ++i)
  {
    if (boost::algorithm::starts_with(m_CreationOptions[i], partialOption))
    {
      break;
    }
  }
  return (i != m_CreationOptions.size());
}


std::string GDALImageIO::GetGdalPixelTypeAsString() const
{
  std::string name = GDALGetDataTypeName(m_PxType->pixType);

  return name;
}

int GDALImageIO::GetNbBands() const
{
  return m_Dataset->GetDataSet()->GetRasterCount();
}

std::string GDALImageIO::GetResourceFile(std::string str) const
{
  if (str.empty())
    return m_FileName;

  itksys::RegularExpression reg;
  reg.compile(str);
  for (auto & filename : GetResourceFiles())
    if (reg.find(filename))
      return filename;

  return std::string("");
}

std::vector<std::string> GDALImageIO::GetResourceFiles() const
{
  std::vector<std::string> result;
  for (char ** file = this->m_Dataset->GetDataSet()->GetFileList() ; *file != nullptr ; ++ file)
    result.push_back(*file);
  return result;
}

std::string GDALImageIO::GetMetadataValue(const std::string path, bool& hasValue, int band) const
{
  // detect namespace if any
  std::string domain("");
  std::string key(path);
  std::size_t found = path.find_first_of("/");
  if (found != std::string::npos)
  {
    domain = path.substr(0, found);
    key = path.substr(found + 1);
  }

  const char* ret;
  if (band >= 0)
    ret = m_Dataset->GetDataSet()->GetRasterBand(band+1)->GetMetadataItem(key.c_str(), domain.c_str());
  else
    ret = m_Dataset->GetDataSet()->GetMetadataItem(key.c_str(), domain.c_str());
  if (ret)
    hasValue = true;
  else
  {
    hasValue = false;
    ret = "";
  }
  return std::string(ret);
}

void GDALImageIO::SetMetadataValue(const char * path, const char * value, int band)
{
  // detect namespace if any
  const char *slash = strchr(path,'/');
  std::string domain("");
  const char *domain_c = nullptr;
  std::string key(path);
  if (slash)
    {
    domain = std::string(path, (slash-path));
    domain_c = domain.c_str();
    key = std::string(slash+1);
    }
  if (band >= 0)
    {
    m_Dataset->GetDataSet()->GetRasterBand(band+1)->SetMetadataItem(key.c_str(), value, domain_c);
    }
  else
    {
    m_Dataset->GetDataSet()->SetMetadataItem(key.c_str(), value, domain_c);
    }
}


void GDALImageIO::ExportMetadata()
{
  SetMetadataValue("METADATATYPE", "OTB");
  SetMetadataValue("OTB_VERSION", OTB_VERSION_STRING );

  // TODO: finish implementation: filter the keys MDGeom::SensorGeometry that
  // will be exported as '<typename>' (boost::any). The (future) SensorModelFactory should
  // be used to detect and export properly the field MDGeom::SensorGeometry into a string
  // keywordlist. Note that the keys generated for this sensor geometry should
  // be prefixed by: MDGeomNames[MDGeom::SensorGeometry] + '.'
  ImageMetadataBase::Keywordlist kwl;
  m_Imd.ToKeywordlist(kwl);
  KeywordlistToMetadata(kwl);

  int bIdx = 0;
  for (const auto& band : m_Imd.Bands)
  {
    band.ToKeywordlist(kwl);
    KeywordlistToMetadata(kwl, bIdx);
    ++bIdx;
  }
}

void GDALImageIO::ImportMetadata()
{
  // TODO
  // Check special value METADATATYPE=OTB before continue processing
  // Keys Starting with: MDGeomNames[MDGeom::SensorGeometry] + '.' should
  // be decoded by the (future) SensorModelFactory.
  // Use ImageMetadataBase::FromKeywordlist to ingest the metadata
  bool hasValue;
  if (std::string(GetMetadataValue("METADATATYPE", hasValue)) != "OTB")
    return;
  ImageMetadataBase::Keywordlist kwl;
  GDALMetadataToKeywordlist(m_Dataset->GetDataSet()->GetMetadata(), kwl);
  m_Imd.FromKeywordlist(kwl);
  // GCPs are imported directly in the ImageMetadata.
  m_Imd.Add(MDGeom::GCP, m_Dataset->GetGCPParam());
  // Parsing the bands
  for (int band = 0 ; band < m_NbBands ; ++band)
  {
    kwl.clear();
    GDALMetadataToKeywordlist(m_Dataset->GetDataSet()->GetRasterBand(band+1)->GetMetadata(), kwl);
    m_Imd.Bands[band].FromKeywordlist(kwl);
  }
}

void GDALImageIO::KeywordlistToMetadata(ImageMetadataBase::Keywordlist kwl, int band)
{
  for (const auto& kv : kwl)
  {
    if (kv.first == MetaData::MDGeomNames.left.at(MDGeom::SensorGeometry))
    {
      SetMetadataValue("MDGeomNames[MDGeom::SensorGeometry].", kv.second.c_str(), band);
    }
    else if (kv.first == MetaData::MDGeomNames.left.at(MDGeom::RPC))
    {
      // RPC Models are exported directly from the ImageMetadata.
      Projection::RPCParam rpcStruct = boost::any_cast<Projection::RPCParam>(m_Imd[MDGeom::RPC]);
      this->SetAs("RPC/LINE_OFF",   rpcStruct.LineOffset);
      this->SetAs("RPC/SAMP_OFF",   rpcStruct.SampleOffset);
      this->SetAs("RPC/LAT_OFF",    rpcStruct.LatOffset);
      this->SetAs("RPC/LONG_OFF",   rpcStruct.LonOffset);
      this->SetAs("RPC/HEIGHT_OFF", rpcStruct.HeightOffset);

      this->SetAs("RPC/LINE_SCALE",   rpcStruct.LineScale);
      this->SetAs("RPC/SAMP_SCALE",   rpcStruct.SampleScale);
      this->SetAs("RPC/LAT_SCALE",    rpcStruct.LatScale);
      this->SetAs("RPC/LONG_SCALE",   rpcStruct.LonScale);
      this->SetAs("RPC/HEIGHT_SCALE", rpcStruct.HeightScale);

      this->SetAsVector("RPC/LINE_NUM_COEFF", std::vector<double> (rpcStruct.LineNum,   rpcStruct.LineNum   + 20 / sizeof(double)), ' ');
      this->SetAsVector("RPC/LINE_DEN_COEFF", std::vector<double> (rpcStruct.LineDen,   rpcStruct.LineDen   + 20 / sizeof(double)), ' ');
      this->SetAsVector("RPC/SAMP_NUM_COEFF", std::vector<double> (rpcStruct.SampleNum, rpcStruct.SampleNum + 20 / sizeof(double)), ' ');
      this->SetAsVector("RPC/SAMP_DEN_COEFF", std::vector<double> (rpcStruct.SampleDen, rpcStruct.SampleDen + 20 / sizeof(double)), ' ');
    }
    // Note that GCPs have already been exported
    SetMetadataValue(kv.first.c_str(), kv.second.c_str(), band);
  }
}

void GDALImageIO::GDALMetadataToKeywordlist(const char* const* metadataList, ImageMetadataBase::Keywordlist &kwl)
{
  // The GDAL metadata string list is formatted as a “Name=value” list with the last pointer value being NULL.
  for ( ; *metadataList != nullptr ; ++metadataList )
    {
      std::string metadataLine = std::string(*metadataList);
      // The key and the value are separated by the '=' symbol
      std::string::size_type pos = metadataLine.find('=');
      std::string fieldName = metadataLine.substr(0, pos);
      std::string fieldValue = metadataLine.substr(pos+1);

      if((fieldName.size() > 36) && (fieldName.substr(0, 36) == "MDGeomNames[MDGeom::SensorGeometry]."))
      {
        // Sensor Geometry is imported directly in the ImageMetadata.
        // TODO: Keys Starting with: MDGeomNames[MDGeom::SensorGeometry] + '.' should
        // be decoded by the (future) SensorModelFactory.
      }
      else if (fieldName == MetaData::MDGeomNames.left.at(MDGeom::RPC))
      {
        // RPC Models are imported directly in the ImageMetadata.
        Projection::RPCParam rpcStruct;
        rpcStruct.LineOffset    = this->GetAs<double>("RPC/LINE_OFF");
        rpcStruct.SampleOffset  = this->GetAs<double>("RPC/SAMP_OFF");
        rpcStruct.LatOffset     = this->GetAs<double>("RPC/LAT_OFF");
        rpcStruct.LonOffset     = this->GetAs<double>("RPC/LONG_OFF");
        rpcStruct.HeightOffset  = this->GetAs<double>("RPC/HEIGHT_OFF");

        rpcStruct.LineScale    = this->GetAs<double>("RPC/LINE_SCALE");
        rpcStruct.SampleScale  = this->GetAs<double>("RPC/SAMP_SCALE");
        rpcStruct.LatScale     = this->GetAs<double>("RPC/LAT_SCALE");
        rpcStruct.LonScale     = this->GetAs<double>("RPC/LONG_SCALE");
        rpcStruct.HeightScale  = this->GetAs<double>("RPC/HEIGHT_SCALE");

        std::vector<double> coeffs(20);

        coeffs = this->GetAsVector<double>("RPC/LINE_NUM_COEFF",' ',20);
        std::copy(coeffs.begin(), coeffs.end(), rpcStruct.LineNum);

        coeffs = this->GetAsVector<double>("RPC/LINE_DEN_COEFF",' ',20);
        std::copy(coeffs.begin(), coeffs.end(), rpcStruct.LineDen);

        coeffs = this->GetAsVector<double>("RPC/SAMP_NUM_COEFF",' ',20);
        std::copy(coeffs.begin(), coeffs.end(), rpcStruct.SampleNum);

        coeffs = this->GetAsVector<double>("RPC/SAMP_DEN_COEFF",' ',20);
        std::copy(coeffs.begin(), coeffs.end(), rpcStruct.SampleDen);

        m_Imd.Add(MDGeom::RPC, rpcStruct);
      }
      else
        kwl.emplace(fieldName, fieldValue);
    }
}


} // end namespace otb
