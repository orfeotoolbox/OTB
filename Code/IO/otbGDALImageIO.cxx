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

#include <iostream>
#include <fstream>
#include <string.h>
#include <list>
#include <vector>
#include <math.h>

#include "otbGDALImageIO.h"
#include "otbMacro.h"
#include "otbSystem.h"
#include "otbImage.h"
#include "itkArray.h"

#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"

#include "itkExceptionObject.h"
#include "itkMacro.h"
#include "itkRGBPixel.h"
#include "itkRGBAPixel.h"

namespace otb
{

// only two states : the Pointer is Null or GetDataSet() returns a valid dataset
class GDALDatasetWrapper : public itk::LightObject
{
  friend class GDALDriverManagerWrapper;

public:
  typedef GDALDatasetWrapper      Self;
  typedef itk::LightObject        Superclass;
  typedef itk::SmartPointer<Self> Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GDALImageIO, itk::LightObject);

  /** Easy access to the internal GDALDataset object.
   *  Don't close it, it will be automatic */
  GDALDataset* GetDataSet()
    {
    return m_Dataset;
    }

protected :
  GDALDatasetWrapper()
   : m_Dataset(NULL)
  {
  }

  ~GDALDatasetWrapper()
  {
    if (m_Dataset)
      {
      GDALClose(m_Dataset);
      }
  }

private:
  GDALDataset* m_Dataset;
};


// Wraps the GdalDriverManager so that GDALAllRegister is called automatically
class GDALDriverManagerWrapper
{
public:
  // GetInstance returns a reference to a GDALDriverManagerWrapper
  // This is the only entry point to interact with this class
  static GDALDriverManagerWrapper& GetInstance()
  {

    // Declare a static method variable of type GDALDriverManagerWrapper
    // so that it is constructed and initialized only on the first call
    // to GetInstance(), and so try to avoid static initialization order problems

    static GDALDriverManagerWrapper theUniqueInstance;
    return theUniqueInstance;
  }

  // Open the file for reading and returns a smart dataset pointer
  GDALDatasetWrapper::Pointer Open( std::string filename )
  {
    GDALDatasetWrapper::Pointer datasetWrapper;
    GDALDatasetH dataset = GDALOpen(filename.c_str(), GA_ReadOnly);
    if (dataset != NULL)
      {
      datasetWrapper = GDALDatasetWrapper::New();
      datasetWrapper->m_Dataset = static_cast<GDALDataset*>(dataset);
      }
    return datasetWrapper;
  }

  // Open the new  file for writing and returns a smart dataset pointer
  GDALDatasetWrapper::Pointer Create( std::string driverShortName, std::string filename,
                                      int nXSize, int nYSize, int nBands,
                                      GDALDataType eType, char ** papszOptions )
  {
    GDALDatasetWrapper::Pointer datasetWrapper;

    GDALDriver*  driver = GetDriverByName( driverShortName );
    if(driver != NULL)
      {
      GDALDataset* dataset = driver->Create(filename.c_str(),
                                            nXSize, nYSize,
                                            nBands, eType,
                                            papszOptions );

      if (dataset != NULL)
        {
        datasetWrapper = GDALDatasetWrapper::New();
        datasetWrapper->m_Dataset = dataset;
        }
      }
    return datasetWrapper;
  }


  GDALDriver* GetDriverByName( std::string driverShortName )
  {
    return GetGDALDriverManager()->GetDriverByName(driverShortName.c_str());
  }

private :
  // private constructor so that this class is allocated only inside GetInstance
  GDALDriverManagerWrapper()
  {
    GDALAllRegister();
  }

  ~GDALDriverManagerWrapper()
  {
    GDALDestroyDriverManager();
  }
};

GDALImageIO::GDALImageIO()
{
  // By default set number of dimensions to two.
  this->SetNumberOfDimensions(2);

  // By default set pixel type to scalar.
  m_PixelType = SCALAR;

  // By default set component type to unsigned char
  m_ComponentType = UCHAR;
  m_UseCompression = false;
  m_CompressionLevel = 4; // Range 0-9; 0 = no file compression, 9 = maximum file compression

  // Set default spacing to one
  m_Spacing[0] = 1.0;
  m_Spacing[1] = 1.0;
  // Set default origin to zero
  m_Origin[0] = 0.0;
  m_Origin[1] = 0.0;

  m_IsIndexed   = false;
  m_currentfile = NULL;
  //m_poBands     = NULL;
  //m_hDriver     = NULL;
  //m_poDataset   = NULL;

  m_NbBands = 0;
  m_FlagWriteImageInformation = true;

  m_CanStreamWrite = false;
}

GDALImageIO::~GDALImageIO()
{
}

// Tell only if the file can be read with GDAL.
bool GDALImageIO::CanReadFile(const char* file)
{
  // First check the extension
  if (file == NULL)
    {
    itkDebugMacro(<< "No filename specified.");
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
}

// Read a 3D image (or event more bands)... not implemented yet
void GDALImageIO::ReadVolume(void*)
{
}

// Read image with GDAL
void GDALImageIO::Read(void* buffer)
{
  std::streamoff  step = static_cast<std::streamoff>(this->GetNumberOfComponents());
  unsigned char * p = static_cast<unsigned char *>(buffer);
  if (p == NULL)
    {
    itkExceptionMacro(<< "Memory allocation error");
    return;
    }

  int lNbLines     = this->GetIORegion().GetSize()[1];
  int lNbColumns   = this->GetIORegion().GetSize()[0];
  int lFirstLine   = this->GetIORegion().GetIndex()[1]; // [1... ]
  int lFirstColumn = this->GetIORegion().GetIndex()[0]; // [1... ]

  std::streamoff lNbPixels = (static_cast<std::streamoff>(lNbColumns)) * (static_cast<std::streamoff>(lNbLines));
  std::streamoff lBufferSize = static_cast<std::streamoff>(m_NbOctetPixel) * lNbPixels;

  itk::Array<unsigned char> value(lBufferSize);

  CPLErr         lCrGdal;
  std::streamoff cpt(0);
  GDALDataset* dataset = m_Dataset->GetDataSet();

  if (GDALDataTypeIsComplex(m_PxType))
    {
    lCrGdal = dataset->GetRasterBand(1)->RasterIO(GF_Read,
                                                 lFirstColumn,
                                                 lFirstLine,
                                                 lNbColumns,
                                                 lNbLines,
                                                 value.data_block(),
                                                 lNbColumns,
                                                 lNbLines,
                                                 m_PxType,
                                                 0,
                                                 0);

    if (lCrGdal == CE_Failure)
      {
      itkExceptionMacro(<< "Error while reading image (GDAL format) " << m_FileName );
      }
    cpt = 0;
    for (std::streamoff i = 0; i < lBufferSize; i = i + static_cast<std::streamoff>(m_NbOctetPixel))
      {
      memcpy((void*) (&(p[cpt])), (const void*) (&(value[i])), (size_t) (m_NbOctetPixel));
      cpt += static_cast<std::streamoff>(m_NbOctetPixel);
      }
    }
  else if (m_IsIndexed)
    {
    step = step * static_cast<std::streamoff>(m_NbOctetPixel);

    lCrGdal = dataset->GetRasterBand(1)->RasterIO(GF_Read,
                                     lFirstColumn,
                                     lFirstLine,
                                     lNbColumns,
                                     lNbLines,
                                     value.data_block(),
                                     lNbColumns,
                                     lNbLines,
                                     m_PxType,
                                     0,
                                     0);
    if (lCrGdal == CE_Failure)
      {
      itkExceptionMacro(<< "Error while reading image (GDAL format) " << m_FileName.c_str() << ".");
      }
    // Recopie dans le buffer
    cpt = 0;
    GDALColorTable* colorTable = dataset->GetRasterBand(1)->GetColorTable();
    for (std::streamoff i = 0; i < lBufferSize; i = i + static_cast<std::streamoff>(m_NbOctetPixel))
      {
      GDALColorEntry color;
      colorTable->GetColorEntryAsRGB(value[i], &color);
      p[cpt] = color.c1;
      p[cpt + 1] = color.c2;
      p[cpt + 2] = color.c3;
      p[cpt + 3] = color.c4;
      cpt += step;
      }
    }
  else
    {
    // Mise a jour du step
    step = step * static_cast<std::streamoff>(m_NbOctetPixel);

    for (unsigned int nbComponents = 0; nbComponents < this->GetNumberOfComponents(); ++nbComponents)
      {
      lCrGdal = dataset->GetRasterBand(nbComponents+1)->RasterIO(GF_Read,
                                                                lFirstColumn,
                                                                lFirstLine,
                                                                lNbColumns,
                                                                lNbLines,
                                                                value.data_block(),
                                                                lNbColumns,
                                                                lNbLines,
                                                                m_PxType,
                                                                0,
                                                                0);
      if (lCrGdal == CE_Failure)
        {
        itkExceptionMacro(<< "Error while reading image (GDAL format) " << m_FileName.c_str() << ".");
        }
      // Recopie dans le buffer
      cpt = static_cast<std::streamoff>(nbComponents) * static_cast<std::streamoff>(m_NbOctetPixel);
      for (std::streamoff i = 0; i < lBufferSize; i = i + static_cast<std::streamoff>(m_NbOctetPixel))
        {
        memcpy((void*) (&(p[cpt])), (const void*) (&(value[i])), (size_t) (m_NbOctetPixel));
        cpt += step;
        }
      }
    }
}

void GDALImageIO::ReadImageInformation()
{
  //std::ifstream file;
  this->InternalReadImageInformation();
}

void GDALImageIO::InternalReadImageInformation()
{
  GDALDataset* dataset = m_Dataset->GetDataSet();
  otbMsgDevMacro(<< "  GCPCount (original): " << m_poDataset->GetGCPCount());

  // Get image dimensions
  if ( dataset->GetRasterXSize() == 0 || dataset->GetRasterYSize() == 0 )
    {
    itkExceptionMacro(<< "Dimension is undefined.");
    }

  // Set image dimensions into IO
  m_Dimensions[0] = dataset->GetRasterXSize();
  m_Dimensions[1] = dataset->GetRasterYSize();

  // Get Number of Bands
  m_NbBands = dataset->GetRasterCount();
  if (m_NbBands == 0)
    {
//FIXME this happen in the case of a hdf file with SUBDATASETS
// in this situation, at least the first dataset should be open (ideally all in an imagelist)
//         char** papszMetadata;
//         papszMetadata = m_poDataset->GetMetadata("SUBDATASETS");
//         if( CSLCount(papszMetadata) > 0 )
//         {
//           std::string key;
//           itk::MetaDataDictionary & dict = this->GetMetaDataDictionary();
//           for( int cpt = 0; papszMetadata[cpt] != NULL; cpt++ )
//           {
//             std::cout << papszMetadata[cpt] << std::endl;
//               ::itk::OStringStream lStream;
//               lStream << MetaDataKey::SubMetadataKey << cpt;
//               key = lStream.str();
//
//               itk::EncapsulateMetaData<std::string>(dict, key,
//                   static_cast<std::string>( papszMetadata[cpt] ) );
//           }
//           std::cout << dict[dict.GetKeys()[0]] << std::endl;
//           std::cout << key << std::endl;
//         }

    itkExceptionMacro(<< "Zero band found in the dataset");
    return;
    }

  this->SetNumberOfComponents(m_NbBands);

  // Set the number of dimensions (verify for the dim )
  this->SetNumberOfDimensions(2);

  otbMsgDevMacro(<< "Nb of Dimensions : " << m_NumberOfDimensions);

  // Automatically set the Type to Binary for GDAL data
  this->SetFileTypeToBinary();

  // Get Data Type
  // Consider only the data type given by the first band
  // Maybe be could changed (to check)
  m_PxType = dataset->GetRasterBand(1)->GetRasterDataType();

  // Following the data type given by GDAL we set it for ImageIO
  // BE CAREFUL !!!! At this time the complex data type are regarded
  // as integer data type in hope that ITK uses that kind of system
  // (take time to check !!)
  if (m_PxType == GDT_Byte)
    {
    SetComponentType(UCHAR);
    }
  else if (m_PxType == GDT_UInt16)
    {
    SetComponentType(USHORT);
    }
  else if ((m_PxType == GDT_Int16) || (m_PxType == GDT_CInt16))
    {
    SetComponentType(SHORT);
    }
  else if (m_PxType == GDT_UInt32)
    {
    SetComponentType(UINT);
    }
  else if ((m_PxType == GDT_Int32) || (m_PxType == GDT_CInt32))
    {
    SetComponentType(INT);
    }
  else if ((m_PxType == GDT_Float32) || (m_PxType == GDT_CFloat32))
    {
    SetComponentType(FLOAT);
    }
  else if ((m_PxType == GDT_Float64) || (m_PxType == GDT_CFloat64))
    {
    SetComponentType(DOUBLE);
    }
  else
    {
    itkExceptionMacro(<< "Pixel type unknown");
    }

  if (this->GetComponentType() == CHAR)
    {
    m_NbOctetPixel = 1;
    }
  else if (this->GetComponentType() == UCHAR)
    {
    m_NbOctetPixel = 1;
    }
  else if (this->GetComponentType() == USHORT)
    {
    m_NbOctetPixel = 2;
    }
  else if (this->GetComponentType() == SHORT)
    {
    m_NbOctetPixel = 2;
    }
  else if (this->GetComponentType() == INT)
    {
    m_NbOctetPixel = 4;
    }
  else if (this->GetComponentType() == UINT)
    {
    m_NbOctetPixel = 4;
    }
  else if (this->GetComponentType() == FLOAT)
    {
    m_NbOctetPixel = 4;
    }
  else if (this->GetComponentType() == DOUBLE)
    {
    m_NbOctetPixel = 8;
    }
  else
    {
    itkExceptionMacro(<< "Component type unknown");
    }

  /******************************************************************/
  // Pixel Type always set to Scalar for GDAL ? maybe also to vector ?

  // Modif Patrick: LIRE LES IMAGES COMPLEXES
  if (GDALDataTypeIsComplex(m_PxType))
    {
    m_NbOctetPixel = m_NbOctetPixel * 2;
    this->SetNumberOfComponents(2);
    this->SetPixelType(COMPLEX);
    // Is this necessary ?
    //if(m_NbBands !=1) itkExceptionMacro(<<"GDALImageIO::InternalReadImageInformation() Can read only one band image ");
    }
  else
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

  /*----------------------------------------------------------------------*/
  /*-------------------------- METADATA ----------------------------------*/
  /*----------------------------------------------------------------------*/

  // Now initialize the itk dictionary
  itk::MetaDataDictionary& dict = this->GetMetaDataDictionary();

  /* -------------------------------------------------------------------- */
  /*  Get Spacing                */
  /* -------------------------------------------------------------------- */

  // Default Spacing
  m_Spacing[0] = 1;
  m_Spacing[1] = 1;
  if (m_NumberOfDimensions == 3) m_Spacing[2] = 1;

  char** papszMetadata;
  papszMetadata =  dataset->GetMetadata(NULL);

  /* -------------------------------------------------------------------- */
  /*      Report general info.                                            */
  /* -------------------------------------------------------------------- */
  GDALDriverH hDriver;

  hDriver = dataset->GetDriver();

  std::string driverShortName =  static_cast<std::string>(GDALGetDriverShortName(hDriver));
  std::string driverLongName  =  static_cast<std::string>(GDALGetDriverLongName(hDriver));

  itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::DriverShortNameKey, driverShortName);
  itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::DriverLongNameKey,  driverLongName);

  /* -------------------------------------------------------------------- */
  /* Get the projection coordinate system of the image : ProjectionRef  */
  /* -------------------------------------------------------------------- */

  if (dataset->GetProjectionRef() != NULL && !std::string(dataset->GetProjectionRef()).empty())
    {
    OGRSpatialReferenceH pSR = OSRNewSpatialReference(NULL);

    const char *         pszProjection = NULL;
    pszProjection =  dataset->GetProjectionRef();

    if (OSRImportFromWkt(pSR,(char **) (&pszProjection)) == OGRERR_NONE)
      {
      char * pszPrettyWkt = NULL;
      OSRExportToPrettyWkt(pSR,&pszPrettyWkt,FALSE);

      itk::EncapsulateMetaData<std::string> (dict, MetaDataKey::ProjectionRefKey,
                                             static_cast<std::string>(pszPrettyWkt));

      CPLFree(pszPrettyWkt);
      }
    else
      itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey,
                                            static_cast<std::string>(dataset->GetProjectionRef()));

    if (pSR != NULL)
      {
      OSRRelease(pSR);
      pSR = NULL;
      }
    }

  /* -------------------------------------------------------------------- */
  /* Get the GCP projection coordinates of the image : GCPProjection  */
  /* -------------------------------------------------------------------- */

  unsigned int gcpCount = 0;
  gcpCount = dataset->GetGCPCount();
  if (gcpCount > 0)
    {
    std::string gcpProjectionKey = static_cast<std::string>(dataset->GetGCPProjection());
    itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::GCPProjectionKey, gcpProjectionKey);

    if (gcpProjectionKey.empty())
      {
      gcpCount = 0; //fix for uninitialized gcpCount in gdal (when
                    //reading Palsar image)
      }

    std::string key;

    itk::EncapsulateMetaData<unsigned int>(dict, MetaDataKey::GCPCountKey, gcpCount);

    for (unsigned int cpt = 0; cpt < gcpCount; cpt++)
      {

      const GDAL_GCP *psGCP;
      psGCP = dataset->GetGCPs() + cpt;

      OTB_GCP pOtbGCP(psGCP);

      // Complete the key with the GCP number : GCP_i
      ::itk::OStringStream lStream;
      lStream << MetaDataKey::GCPParametersKey << cpt;
      key = lStream.str();

      itk::EncapsulateMetaData<OTB_GCP>(dict, key, pOtbGCP);

      }

    }

  /* -------------------------------------------------------------------- */
  /*  Get the six coefficients of affine geoTtransform      */
  /* -------------------------------------------------------------------- */

  double     adfGeoTransform[6];
  MetaDataKey::VectorType VadfGeoTransform;

  if (dataset->GetGeoTransform(adfGeoTransform) == CE_None)
    {
    for (int cpt = 0; cpt < 6; cpt++)
      VadfGeoTransform.push_back(adfGeoTransform[cpt]);

    itk::EncapsulateMetaData<MetaDataKey::VectorType>(dict, MetaDataKey::GeoTransformKey, VadfGeoTransform);

    /// retrieve orgin and spacing from the geo transform
    m_Origin[0] = VadfGeoTransform[0];
    m_Origin[1] = VadfGeoTransform[3];
    m_Spacing[0] = VadfGeoTransform[1];
    m_Spacing[1] = VadfGeoTransform[5];

    //In this case, we are in a geographic projection if no other cartographic information was already
    // available
    // FIXME is there any way to know if we are in WGS 84 ???
    std::string projRef;
    itk::ExposeMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, projRef);
    if (projRef.empty())
      {
      projRef =
        "GEOGCS[\"GCS_WGS_1984\",DATUM[\"D_WGS_1984\",SPHEROID[\"WGS_1984\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.017453292519943295]]";

      itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, projRef);
      }
    }

  /* -------------------------------------------------------------------- */
  /*      Report metadata.                                                */
  /* -------------------------------------------------------------------- */

  papszMetadata = dataset->GetMetadata(NULL);
  if (CSLCount(papszMetadata) > 0)
    {
    std::string key;

    for (int cpt = 0; papszMetadata[cpt] != NULL; cpt++)
      {
      ::itk::OStringStream lStream;
      lStream << MetaDataKey::MetadataKey << cpt;
      key = lStream.str();

      itk::EncapsulateMetaData<std::string>(dict, key,
                                            static_cast<std::string>(papszMetadata[cpt]));
      }
    }

  /* -------------------------------------------------------------------- */
  /*      Report subdatasets.                                             */
  /* -------------------------------------------------------------------- */

  papszMetadata = dataset->GetMetadata("SUBDATASETS");
  if (CSLCount(papszMetadata) > 0)
    {
    std::string key;

    for (int cpt = 0; papszMetadata[cpt] != NULL; cpt++)
      {
      ::itk::OStringStream lStream;
      lStream << MetaDataKey::SubMetadataKey << cpt;
      key = lStream.str();

      itk::EncapsulateMetaData<std::string>(dict, key,
                                            static_cast<std::string>(papszMetadata[cpt]));
      }
    }

  /* -------------------------------------------------------------------- */
  /* Report corners              */
  /* -------------------------------------------------------------------- */

  double     GeoX(0), GeoY(0);
  MetaDataKey::VectorType VGeo;

  GDALInfoReportCorner("Upper Left", 0.0, 0.0, GeoX, GeoY);
  VGeo.push_back(GeoX);
  VGeo.push_back(GeoY);

  itk::EncapsulateMetaData<MetaDataKey::VectorType>(dict, MetaDataKey::UpperLeftCornerKey, VGeo);

  VGeo.clear();

  GDALInfoReportCorner("Upper Right", m_Dimensions[0], 0.0, GeoX, GeoY);
  VGeo.push_back(GeoX);
  VGeo.push_back(GeoY);

  itk::EncapsulateMetaData<MetaDataKey::VectorType>(dict, MetaDataKey::UpperRightCornerKey, VGeo);

  VGeo.clear();

  GDALInfoReportCorner("Lower Left", 0.0, m_Dimensions[1], GeoX, GeoY);
  VGeo.push_back(GeoX);
  VGeo.push_back(GeoY);

  itk::EncapsulateMetaData<MetaDataKey::VectorType>(dict, MetaDataKey::LowerLeftCornerKey, VGeo);

  VGeo.clear();

  GDALInfoReportCorner("Lower Right", m_Dimensions[0], m_Dimensions[1], GeoX, GeoY);
  VGeo.push_back(GeoX);
  VGeo.push_back(GeoY);

  itk::EncapsulateMetaData<MetaDataKey::VectorType>(dict, MetaDataKey::LowerRightCornerKey, VGeo);

  VGeo.clear();

  /* -------------------------------------------------------------------- */
  /* Color Table                                                          */
  /* -------------------------------------------------------------------- */

  for (int iBand = 0; iBand < dataset->GetRasterCount(); iBand++)
    {
    GDALColorTableH hTable;
    GDALRasterBandH hBand;
    hBand = GDALGetRasterBand(dataset, iBand + 1);
    if ((GDALGetRasterColorInterpretation(hBand) == GCI_PaletteIndex)
        && (hTable = GDALGetRasterColorTable(hBand)) != NULL)
      {
      m_IsIndexed = true;

      unsigned int ColorEntryCount = GDALGetColorEntryCount(hTable);

      itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ColorTableNameKey,
                                            static_cast<std::string>(GDALGetPaletteInterpretationName(
                                                                       GDALGetPaletteInterpretation(hTable))));

      itk::EncapsulateMetaData<unsigned int>(dict, MetaDataKey::ColorEntryCountKey, ColorEntryCount);

      for (int i = 0; i < GDALGetColorEntryCount(hTable); ++i)
        {
        GDALColorEntry sEntry;
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
}

bool GDALImageIO::CanWriteFile(const char* name)
{
  // First check the filename
  if (name == NULL)
    {
    itkDebugMacro(<< "No filename specified.");
    return false;
    }

  // Get the GDAL format ID from the name
  std::string gdalDriverShortName = FilenameToGdalDriverShortName(name);
  if (gdalDriverShortName == "NOT-FOUND")
    {
    return false;
    }

  // Check the driver for support of Create or at least CreateCopy
  GDALDriver* driver = GDALDriverManagerWrapper::GetInstance().GetDriverByName(gdalDriverShortName);
  if ( GDALGetMetadataItem( driver, GDAL_DCAP_CREATE, NULL ) == NULL
       && GDALGetMetadataItem( driver, GDAL_DCAP_CREATECOPY, NULL ) == NULL )
    {
    itkDebugMacro(<< "The driver " << GDALGetDriverShortName(driver) << " does not support writing");
    return false;
    }
  return true;
}

bool GDALImageIO::CanStreamWrite()
{
  // Get the GDAL format ID from the name
  std::string gdalDriverShortName = FilenameToGdalDriverShortName(m_FileName);
  GDALDriver* driver = GDALDriverManagerWrapper::GetInstance().GetDriverByName(gdalDriverShortName);
  if (driver == NULL)
    {
    itkDebugMacro(<< "Unable to instantiate driver " << gdalDriverShortName);
    m_CanStreamWrite = false;
    }

  if ( GDALGetMetadataItem( driver, GDAL_DCAP_CREATE, NULL ) != NULL )
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
  if (m_FlagWriteImageInformation == true)
    {
    this->InternalWriteImageInformation(buffer);
    m_FlagWriteImageInformation = false;
    }

  const unsigned char *p = static_cast<const unsigned char *>(buffer);
  if (p == NULL)
    {
    itkExceptionMacro(<< "GDAL : Bad alloc");
    return;
    }

  unsigned int lNbLines = this->GetIORegion().GetSize()[1];
  unsigned int lNbColumns = this->GetIORegion().GetSize()[0];
  int lFirstLine = this->GetIORegion().GetIndex()[1]; // [1... ]
  int lFirstColumn = this->GetIORegion().GetIndex()[0]; // [1... ]

  // Particular case: checking that the written region is the same size
  // of the entire image
  // starting at offset 0 (when no streaming)
  if ((lNbLines == m_Dimensions[1]) && (lNbColumns == m_Dimensions[0]))
    {
    lFirstLine = 0;
    lFirstColumn = 0;
    }

  std::streamoff lNbPixels = static_cast<std::streamoff> (lNbColumns) * static_cast<std::streamoff> (lNbLines);
  std::streamoff lBufferSize = static_cast<std::streamoff> (m_NbOctetPixel) * lNbPixels;
  otbMsgDevMacro(<< " BufferSize allocated : " << lBufferSize);

  itk::Array<unsigned char> value(lBufferSize);

  if (m_CanStreamWrite)
    {
    // Update Step
    std::streamoff step = static_cast<std::streamoff> (m_NbBands);
    step = step * static_cast<std::streamoff> (m_NbOctetPixel);

    CPLErr lCrGdal;

    std::streamoff cpt(0);
    for (int nbComponents = 0; nbComponents < m_NbBands; ++nbComponents)
      {
      cpt = static_cast<std::streamoff> (nbComponents) * static_cast<std::streamoff> (m_NbOctetPixel);

      for (std::streamoff i = 0; i < lBufferSize; i = i + static_cast<std::streamoff> (m_NbOctetPixel))
        {
        memcpy((void*) (&(value[i])), (const void*) (&(p[cpt])), (size_t) (m_NbOctetPixel));
        cpt += step;
        }
      GDALRasterBand *poBand = m_Dataset->GetDataSet()->GetRasterBand(nbComponents+1);

      lCrGdal = poBand->RasterIO(GF_Write, lFirstColumn, lFirstLine, lNbColumns, lNbLines, value.data_block(),
                                 lNbColumns, lNbLines, m_PxType, 0, 0);
      if (lCrGdal == CE_Failure)
        {
        itkExceptionMacro(<< "Error while writing image (GDAL format) " << m_FileName.c_str() << ".");
        }

      poBand->FlushCache();
      }
    m_Dataset->GetDataSet()->FlushCache();
    }
  else
    {
    // We only wrote data to the memory dataset
    // Now write it to the real file with CreateCopy()
    std::string gdalDriverShortName = FilenameToGdalDriverShortName(m_FileName);
    std::string realFileName = GetGdalWriteImageFileName(gdalDriverShortName, m_FileName);

    GDALDriver* driver = GDALDriverManagerWrapper::GetInstance().GetDriverByName(gdalDriverShortName);
    if (driver == NULL)
      {
      itkExceptionMacro(<< "Unable to instantiate driver " << gdalDriverShortName << " to write " << m_FileName);
      }

    GDALDataset* hOutputDS = driver->CreateCopy( realFileName.c_str(), m_Dataset->GetDataSet(), FALSE,
                                                 NULL, NULL, NULL );
    GDALClose(hOutputDS);
    }
}

/** TODO : Methode WriteImageInformation non implementee */
void GDALImageIO::WriteImageInformation()
{
}

void GDALImageIO::InternalWriteImageInformation(const void* buffer)
{
  char **     papszOptions = NULL;
  std::string driverShortName;
  m_NbBands = this->GetNumberOfComponents();

  if ((m_Dimensions[0] == 0) && (m_Dimensions[1] == 0))
    {
    itkExceptionMacro(<< "Dimensions are not defined.");
    }

  if ((this->GetPixelType() == COMPLEX) && (m_NbBands / 2 > 0))
    {
    m_NbBands /= 2;

    if (this->GetComponentType() == SHORT)
      {
      m_NbOctetPixel = 4;
      m_PxType = GDT_CInt16;
      }
    else if (this->GetComponentType() == INT)
      {
      m_NbOctetPixel = 8;
      m_PxType = GDT_CInt32;
      }
    else if (this->GetComponentType() == DOUBLE)
      {
      m_NbOctetPixel = 16;
      m_PxType = GDT_CFloat64;
      }
    else if (this->GetComponentType() == FLOAT)
      {
      m_NbOctetPixel = 8;
      m_PxType = GDT_CFloat32;
      }
    else
      {
      this->SetComponentType(FLOAT);
      m_NbOctetPixel = 8;
      m_PxType = GDT_CFloat32;
      }
    }
  else
    {
    if (this->GetComponentType() == CHAR)
      {
      m_NbOctetPixel = 1;
      m_PxType = GDT_Byte;
      }
    else if (this->GetComponentType() == UCHAR)
      {
      m_NbOctetPixel = 1;
      m_PxType = GDT_Byte;
      }
    else if (this->GetComponentType() == USHORT)
      {
      m_NbOctetPixel = 2;
      m_PxType = GDT_UInt16;
      }
    else if (this->GetComponentType() == SHORT)
      {
      m_NbOctetPixel = 2;
      m_PxType = GDT_Int16;
      }
    else if (this->GetComponentType() == INT)
      {
      m_NbOctetPixel = 4;
      m_PxType = GDT_Int32;
      }
    else if (this->GetComponentType() == UINT)
      {
      m_NbOctetPixel = 4;
      m_PxType = GDT_UInt32;
      }
    else if (this->GetComponentType() == FLOAT)
      {
      m_NbOctetPixel = 4;
      m_PxType = GDT_Float32;
      }
    else if (this->GetComponentType() == DOUBLE)
      {
      m_NbOctetPixel = 8;
      m_PxType = GDT_Float64;
      }
    else
      {
      m_NbOctetPixel = 1;
      m_PxType = GDT_Byte;
      }
    }

  // Automatically set the Type to Binary for GDAL data
  this->SetFileTypeToBinary();

  driverShortName = FilenameToGdalDriverShortName(m_FileName);
  if (driverShortName == "NOT-FOUND")
    {
    itkExceptionMacro(
      << "GDAL Writing failed : the image file name '" << m_FileName.c_str() << "' is not recognized by GDAL.");
    }

  if (m_CanStreamWrite)
    {
    m_Dataset = GDALDriverManagerWrapper::GetInstance().Create(
                     driverShortName,
                     GetGdalWriteImageFileName(driverShortName, m_FileName),
                     m_Dimensions[0],m_Dimensions[1],
                     m_NbBands, m_PxType,
                     papszOptions);
    }
  else
    {
    std::ostringstream stream;
    stream << "MEM:::"
           <<  "DATAPOINTER=" << buffer << ","
           <<  "PIXELS=" << m_Dimensions[0] << ","
           <<  "LINES=" << m_Dimensions[1] << ","
           <<  "BANDS=" << m_NbBands << ","
           <<  "DATATYPE=" << GDALGetDataTypeName(m_PxType) << ","
           <<  "PIXELOFFSET=" << m_NbOctetPixel * m_NbBands << ","
           <<  "LINEOFFSET=" << m_NbOctetPixel * m_NbBands * m_Dimensions[0] << ","
           <<  "BANDOFFSET=" << m_NbOctetPixel;

    m_Dataset = GDALDriverManagerWrapper::GetInstance().Open(stream.str());
    }

  if (m_Dataset.IsNull())
    {
    itkExceptionMacro(
      << "GDAL Writing failed : Impossible to create the image file name '" << m_FileName << "'.");
    }

  /*----------------------------------------------------------------------*/
  /*-------------------------- METADATA ----------------------------------*/
  /*----------------------------------------------------------------------*/

  // Now initialize the itk dictionary
  itk::MetaDataDictionary& dict = this->GetMetaDataDictionary();
  itk::OStringStream oss;
  GDALDataset* dataset = m_Dataset->GetDataSet();

  /* -------------------------------------------------------------------- */
  /* Set the GCPs                                                          */
  /* -------------------------------------------------------------------- */

  unsigned int gcpCount = 0;
  itk::ExposeMetaData<unsigned int>(dict, MetaDataKey::GCPCountKey, gcpCount);

  if (gcpCount > 0)
    {

    GDAL_GCP * gdalGcps = new GDAL_GCP[gcpCount];

    for (unsigned int gcpIndex = 0; gcpIndex < gcpCount; ++gcpIndex)
      {
      //Build the GCP string in the form of GCP_n
      itk::OStringStream lStream;
      lStream << MetaDataKey::GCPParametersKey << gcpIndex;
      std::string key = lStream.str();

      OTB_GCP gcp;
      itk::ExposeMetaData<OTB_GCP>(dict, key, gcp);

      gdalGcps[gcpIndex].pszId = const_cast<char *>(gcp.m_Id.c_str());
      gdalGcps[gcpIndex].pszInfo = const_cast<char *>(gcp.m_Info.c_str());
      gdalGcps[gcpIndex].dfGCPPixel = gcp.m_GCPCol;
      gdalGcps[gcpIndex].dfGCPLine = gcp.m_GCPRow;
      gdalGcps[gcpIndex].dfGCPX = gcp.m_GCPX;
      gdalGcps[gcpIndex].dfGCPY = gcp.m_GCPY;
      gdalGcps[gcpIndex].dfGCPZ = gcp.m_GCPZ;

      }

    std::string gcpProjectionRef;
    itk::ExposeMetaData<std::string>(dict, MetaDataKey::GCPProjectionKey, gcpProjectionRef);
    dataset->SetGCPs(gcpCount, gdalGcps, gcpProjectionRef.c_str());

    delete[] gdalGcps;
    }

  /* -------------------------------------------------------------------- */
  /* Set the projection coordinate system of the image : ProjectionRef  */
  /* -------------------------------------------------------------------- */

  std::string projectionRef;
  itk::ExposeMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, projectionRef);
  if (!projectionRef.empty())
    {
    dataset->SetProjection(projectionRef.c_str());
    }

  /* -------------------------------------------------------------------- */
  /*  Set the six coefficients of affine geoTtransform      */
  /* -------------------------------------------------------------------- */

  itk::Array<double> geoTransform(6);
  /// Reporting origin and spacing
  geoTransform[0] = m_Origin[0];
  geoTransform[3] = m_Origin[1];
  geoTransform[1] = m_Spacing[0];
  geoTransform[5] = m_Spacing[1];

  // FIXME: Here component 1 and 4 should be replaced by the orientation parameters
  geoTransform[2] = 0.;
  geoTransform[4] = 0.;
  dataset->SetGeoTransform(geoTransform.data_block());

  /* -------------------------------------------------------------------- */
  /*      Report metadata.                                                */
  /* -------------------------------------------------------------------- */

  std::string              svalue = "";
  std::vector<std::string> keys = dict.GetKeys();
  MetaDataKey              key;

  for (unsigned int itkey = 0; itkey < keys.size(); ++itkey)
    {
    if (keys[itkey].compare(0, key.MetadataKey.length(), key.MetadataKey) == 0)
      {
      itk::ExposeMetaData<std::string>(dict, keys[itkey], svalue);
      unsigned int equalityPos = svalue.find_first_of('=');
      std::string  tag = svalue.substr(0, equalityPos);
      std::string  value = svalue.substr(equalityPos + 1);
      otbMsgDevMacro(<< "Metadata: " << tag << "=" << value);
      dataset->SetMetadataItem(tag.c_str(), value.c_str(), NULL);
      }
    }
  // END

}

std::string GDALImageIO::FilenameToGdalDriverShortName(std::string name)
{
  std::string extension;
  std::string gdalDriverShortName;

  // Get extension in lowercase
  extension = otb::System::SetToLower( System::GetExtension(name) );

  if      ( extension == "tif" || extension == "tiff" )
    gdalDriverShortName = "GTiff";
  else if ( extension == "hdr" )
    gdalDriverShortName = "ENVI";
  else if ( extension == "img" )
    gdalDriverShortName = "HFA";
  else if ( extension == "ntf" )
    gdalDriverShortName = "NITF";
  else if ( extension == "png" )
    gdalDriverShortName="PNG";
  else if ( extension == "jpg" || extension=="jpeg" )
    gdalDriverShortName="JPEG";
  else if ( extension == "pix" )
    gdalDriverShortName="PCIDSK";
  else
    gdalDriverShortName = "NOT-FOUND";

  return gdalDriverShortName;
}

std::string GDALImageIO::GetGdalWriteImageFileName(std::string& gdalDriverShortName, std::string filename)
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

bool GDALImageIO::GDALInfoReportCorner(const char * /*corner_name*/, double x, double y, double& GeoX, double& GeoY)
{
  const char *pszProjection;
  double      adfGeoTransform[6];

  bool IsTrue;

  /* -------------------------------------------------------------------- */
  /*      Transform the point into georeferenced coordinates.             */
  /* -------------------------------------------------------------------- */
  if (m_Dataset->GetDataSet()->GetGeoTransform(adfGeoTransform) == CE_None)
    {
    pszProjection = m_Dataset->GetDataSet()->GetProjectionRef();

    GeoX = adfGeoTransform[0] + adfGeoTransform[1] * x
           + adfGeoTransform[2] * y;
    GeoY = adfGeoTransform[3] + adfGeoTransform[4] * x
           + adfGeoTransform[5] * y;
    IsTrue = true;
    }

  else
    {
    GeoX = x;
    GeoY = y;
    IsTrue = false;
    }

  return IsTrue;
}

} // end namespace otb
