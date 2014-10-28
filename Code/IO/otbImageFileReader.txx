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
#ifndef __otbImageFileReader_txx
#define __otbImageFileReader_txx

#include "otbImageFileReader.h"

#include "otbSystem.h"
#include <itksys/SystemTools.hxx>
#include <fstream>

#include "itkImageIOFactory.h"
#include "itkPixelTraits.h"
#include "itkVectorImage.h"
#include "itkMetaDataObject.h"

#include "otbConvertPixelBuffer.h"
#include "otbImageIOFactory.h"
#include "otbMetaDataKey.h"

#include "otbGDALImageIO.h" //FIXME avoid requiring GDALImageIO here
#if defined(OTB_USE_JPEG2000)
#include "otbJPEG2000ImageIO.h" //FIXME avoid requiring JPEG2000ImageIO here
#endif
#include "otbTileMapImageIO.h" //FIXME avoid requiring TileMapImageIO here
#include "otbCurlHelper.h"

namespace otb
{

template<class T>
bool PixelIsComplex(const std::complex<T>& /*dummy*/)
{
  return true;
}
template<class T>
bool PixelIsComplex(const T& /*dummy*/)
{
  return false;
}

template <class TOutputImage, class ConvertPixelTraits>
ImageFileReader<TOutputImage, ConvertPixelTraits>
::ImageFileReader()
 : m_ImageIO(),
   m_UserSpecifiedImageIO(false),
   m_FileName(""),
   m_UseStreaming(true),
   m_ExceptionMessage(""),
   m_ActualIORegion(),
   m_FilenameHelper(FNameHelperType::New()),
   m_Curl(CurlHelper::New()),
   m_AdditionalNumber(0)
{
}

template <class TOutputImage, class ConvertPixelTraits>
ImageFileReader<TOutputImage, ConvertPixelTraits>
::~ImageFileReader()
{
}

template <class TOutputImage, class ConvertPixelTraits>
void ImageFileReader<TOutputImage, ConvertPixelTraits>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  if (this->m_ImageIO)
    {
    os << indent << "ImageIO: \n";
    this->m_ImageIO->Print(os, indent.GetNextIndent());
    }
  else
    {
    os << indent << "ImageIO: (null)" << "\n";
    }

  os << indent << "UserSpecifiedImageIO flag: " << this->m_UserSpecifiedImageIO << "\n";
  os << indent << "m_FileName: " << this->m_FileName << "\n";
  os << indent << "m_UseStreaming flag: " << this->m_UseStreaming << "\n";
  os << indent << "m_ActualIORegion: " << this->m_ActualIORegion << "\n";
  os << indent << "m_AdditionalNumber: " << this->m_AdditionalNumber << "\n";
}

template <class TOutputImage, class ConvertPixelTraits>
void
ImageFileReader<TOutputImage, ConvertPixelTraits>
::SetImageIO( otb::ImageIOBase * imageIO)
{
  itkDebugMacro("setting ImageIO to " << imageIO );
  if (this->m_ImageIO != imageIO )
    {
    this->m_ImageIO = imageIO;
    this->Modified();
    }
  m_UserSpecifiedImageIO = true;
}

template <class TOutputImage, class ConvertPixelTraits>
void
ImageFileReader<TOutputImage, ConvertPixelTraits>
::GenerateData()
{

  typename TOutputImage::Pointer output = this->GetOutput();

  // allocate the output buffer
  output->SetBufferedRegion(output->GetRequestedRegion());
  output->Allocate();

  // Test if the file exist and if it can be open.
  // An exception will be thrown otherwise.
  this->TestFileExistanceAndReadability();

  // Tell the ImageIO to read the file
  OutputImagePixelType *buffer =
    output->GetPixelContainer()->GetBufferPointer();
  this->m_ImageIO->SetFileName(this->m_FileName.c_str());

  itk::ImageIORegion ioRegion(TOutputImage::ImageDimension);

  itk::ImageIORegion::SizeType  ioSize = ioRegion.GetSize();
  itk::ImageIORegion::IndexType ioStart = ioRegion.GetIndex();

  /* Init IORegion with size or streaming size */
  SizeType dimSize;
  for (unsigned int i = 0; i < TOutputImage::ImageDimension; ++i)
    {
    if (i < this->m_ImageIO->GetNumberOfDimensions())
      {
      if (!this->m_ImageIO->CanStreamRead()) dimSize[i] = this->m_ImageIO->GetDimensions(i);
      else dimSize[i] = output->GetRequestedRegion().GetSize()[i];
      }
    else
      {
      // Number of dimensions in the output is more than number of dimensions
      // in the ImageIO object (the file).  Use default values for the size,
      // spacing, and origin for the final (degenerate) dimensions.
      dimSize[i] = 1;
      }
    }

  for (unsigned int i = 0; i < dimSize.GetSizeDimension(); ++i)
    {
    ioSize[i] = dimSize[i];
    }

  typedef typename TOutputImage::IndexType IndexType;
  IndexType start;
  if (!this->m_ImageIO->CanStreamRead()) start.Fill(0);
  else start = output->GetRequestedRegion().GetIndex();
  for (unsigned int i = 0; i < start.GetIndexDimension(); ++i)
    {
    ioStart[i] = start[i];
    }

  ioRegion.SetSize(ioSize);
  ioRegion.SetIndex(ioStart);

  this->m_ImageIO->SetIORegion(ioRegion);

  typedef otb::DefaultConvertPixelTraits<typename TOutputImage::IOPixelType> ConvertIOPixelTraits;
  typedef otb::DefaultConvertPixelTraits<typename TOutputImage::PixelType>   ConvertOutputPixelTraits;

  if (this->m_ImageIO->GetComponentTypeInfo()
      == typeid(typename ConvertOutputPixelTraits::ComponentType)
      && (this->m_ImageIO->GetNumberOfComponents()
          == ConvertIOPixelTraits::GetNumberOfComponents()))
    {
    // Have the ImageIO read directly into the allocated buffer
    this->m_ImageIO->Read(buffer);
    return;
    }
  else // a type conversion is necessary
    {
    // note: char is used here because the buffer is read in bytes
    // regardless of the actual type of the pixels.
    ImageRegionType region = output->GetBufferedRegion();

    // Adapt the image size with the region
    std::streamoff nbBytes = (this->m_ImageIO->GetComponentSize() * this->m_ImageIO->GetNumberOfComponents())
                             * static_cast<std::streamoff>(region.GetNumberOfPixels());

    char * loadBuffer = new char[nbBytes];

    otbMsgDevMacro(<< "size of Buffer to GDALImageIO::read = " << nbBytes << " = \n"
        << "ComponentSize ("<< this->m_ImageIO->GetComponentSize() << ") x " \
        << "Nb of Component (" << this->m_ImageIO->GetNumberOfComponents() << ") x " \
        << "Nb of Pixel to read (" << region.GetNumberOfPixels() << ")" );

    this->m_ImageIO->Read(loadBuffer);

    this->DoConvertBuffer(loadBuffer, region.GetNumberOfPixels());

    delete[] loadBuffer;
    }
}

template <class TOutputImage, class ConvertPixelTraits>
void
ImageFileReader<TOutputImage, ConvertPixelTraits>
::EnlargeOutputRequestedRegion(itk::DataObject *output)
{
  typename TOutputImage::Pointer out = dynamic_cast<TOutputImage*>(output);

  // If the ImageIO object cannot stream, then set the RequestedRegion to the
  // LargestPossibleRegion
  if (!this->m_ImageIO->CanStreamRead())
    {
    if (out)
      {
      out->SetRequestedRegion(out->GetLargestPossibleRegion());
      }
    else
      {
      throw otb::ImageFileReaderException(__FILE__, __LINE__,
                                          "Invalid output object type");
      }
    }
}

template <class TOutputImage, class ConvertPixelTraits>
void
ImageFileReader<TOutputImage, ConvertPixelTraits>
::GenerateOutputInformation(void)
{

  typename TOutputImage::Pointer output = this->GetOutput();

  itkDebugMacro(<< "Reading file for GenerateOutputInformation()" << this->m_FileName);

  // Check to see if we can read the file given the name or prefix
  //
  if (this->m_FileName == "")
    {
    throw otb::ImageFileReaderException(__FILE__, __LINE__, "FileName must be specified");
    }

  // Find real image file name
  // !!!!  Update FileName
  std::string lFileName;
  bool        found = GetGdalReadImageFileName(this->m_FileName, lFileName);
  if (found == false)
    {
    otbMsgDebugMacro(<< "Filename was NOT unknown. May be recognized by a Image factory ! ");
    }
  // Update FileName
  this->m_FileName = lFileName;

  std::string lFileNameOssimKeywordlist = this->m_FileName;

  // Test if the file exists and if it can be opened.
  // An exception will be thrown otherwise.
  // We catch the exception because some ImageIO's may not actually
  // open a file. Still reports file error if no ImageIO is loaded.

  try
    {
    m_ExceptionMessage = "";
    this->TestFileExistanceAndReadability();
    }
  catch (itk::ExceptionObject & err)
    {
    m_ExceptionMessage = err.GetDescription();
    }

  if (this->m_UserSpecifiedImageIO == false)   //try creating via factory
    {
    this->m_ImageIO = ImageIOFactory::CreateImageIO(this->m_FileName.c_str(), otb::ImageIOFactory::ReadMode);
    }

  if (this->m_ImageIO.IsNull())
    {
    this->Print(std::cerr);
    otb::ImageFileReaderException e(__FILE__, __LINE__);
    std::ostringstream msg;
    msg << " Could not create IO object for file "
        << this->m_FileName.c_str() << std::endl;
    msg << "  Tried to create one of the following:" << std::endl;
    std::list<itk::LightObject::Pointer> allobjects =
      itk::ObjectFactoryBase::CreateAllInstance("otbImageIOBase");
    for (std::list<itk::LightObject::Pointer>::iterator i = allobjects.begin();
         i != allobjects.end(); ++i)
      {
      otb::ImageIOBase* io = dynamic_cast<otb::ImageIOBase*>(i->GetPointer());
      msg << "    " << io->GetNameOfClass() << std::endl;
      }
    msg << "  You probably failed to set a file suffix, or" << std::endl;
    msg << "    set the suffix to an unsupported type." << std::endl;
    e.SetDescription(msg.str().c_str());
    throw e;
    return;
    }


  // Get the ImageIO MetaData Dictionary
  itk::MetaDataDictionary& dict = this->m_ImageIO->GetMetaDataDictionary();

  // Special actions for the gdal image IO
  if (strcmp(this->m_ImageIO->GetNameOfClass(), "GDALImageIO") == 0)
    {
    typename GDALImageIO::Pointer imageIO = dynamic_cast<GDALImageIO*>(this->GetImageIO());

    // Hint the IO whether the OTB image type takes complex pixels
    // this will determine the strategy to fill up a vector image
    OutputImagePixelType dummy;
    imageIO->SetIsComplex(PixelIsComplex(dummy));

    // VectorImage ??
    if (strcmp(output->GetNameOfClass(), "VectorImage") == 0)
      imageIO->SetIsVectorImage(true);
    else
      imageIO->SetIsVectorImage(false);

    // Pass the dataset number (used for hdf files for example)
    if (m_FilenameHelper->SubDatasetIndexIsSet())
      {
      imageIO->SetDatasetNumber(m_FilenameHelper->GetSubDatasetIndex());
      }
    else
      {
      imageIO->SetDatasetNumber(m_AdditionalNumber);
      }
    }


  if (m_FilenameHelper->ResolutionFactorIsSet())
    {
    itk::EncapsulateMetaData<unsigned int>(dict, MetaDataKey::ResolutionFactor, m_FilenameHelper->GetResolutionFactor());
    }
  else
    {
    itk::EncapsulateMetaData<unsigned int>(dict, MetaDataKey::ResolutionFactor, m_AdditionalNumber);
    }

    // Special actions for the JPEG2000ImageIO
  if (strcmp(this->m_ImageIO->GetNameOfClass(), "JPEG2000ImageIO") == 0)
    {
    itk::EncapsulateMetaData<unsigned int>(dict, MetaDataKey::CacheSizeInBytes, 135000000);
    }

  // Got to allocate space for the image. Determine the characteristics of
  // the image.
  //
  this->m_ImageIO->SetFileName(this->m_FileName.c_str());
  this->m_ImageIO->ReadImageInformation();
  // Initialization du nombre de Composante par pixel
// THOMAS ceci n'est pas dans ITK !!
//  output->SetNumberOfComponentsPerPixel(this->m_ImageIO->GetNumberOfComponents());

  SizeType                             dimSize;
  double                               spacing[TOutputImage::ImageDimension];
  double                               origin[TOutputImage::ImageDimension];
  typename TOutputImage::DirectionType direction;
  std::vector<double>                  axis;

  for (unsigned int i = 0; i < TOutputImage::ImageDimension; ++i)
    {
    if (i < this->m_ImageIO->GetNumberOfDimensions())
      {
      dimSize[i] = this->m_ImageIO->GetDimensions(i);
      spacing[i] = this->m_ImageIO->GetSpacing(i);
      origin[i]  = this->m_ImageIO->GetOrigin(i);
// Please note: direction cosines are stored as columns of the
// direction matrix
      axis = this->m_ImageIO->GetDirection(i);
      for (unsigned j = 0; j < TOutputImage::ImageDimension; ++j)
        {
        if (j < this->m_ImageIO->GetNumberOfDimensions())
          {
          direction[j][i] = axis[j];
          }
        else
          {
          direction[j][i] = 0.0;
          }
        }
      }
    else
      {
      // Number of dimensions in the output is more than number of dimensions
      // in the ImageIO object (the file).  Use default values for the size,
      // spacing, origin and direction for the final (degenerate) dimensions.
      dimSize[i] = 1;
      spacing[i] = 1.0;
      origin[i] = 0.5;
      for (unsigned j = 0; j < TOutputImage::ImageDimension; ++j)
        {
        if (i == j)
          {
          direction[j][i] = 1.0;
          }
        else
          {
          direction[j][i] = 0.0;
          }
        }
      }
    }

  if (m_FilenameHelper->GetSkipCarto())
    {
    for (unsigned int i = 0; i < TOutputImage::ImageDimension; ++i)
      {
      if ( m_FilenameHelper->GetResolutionFactor() != 0 )
        {
        spacing[i] = 1.0*vcl_pow((double)2, (double)m_FilenameHelper->GetResolutionFactor());
        }
      else
        {
        spacing[i] = 1.0;
        }
      origin[i] = 0.5*spacing[i];
      }
    }

  output->SetSpacing(spacing);       // Set the image spacing
  output->SetOrigin(origin);         // Set the image origin
  output->SetDirection(direction);   // Set the image direction cosines

  // Update otb Keywordlist
  ImageKeywordlist otb_kwl;
  if (!m_FilenameHelper->ExtGEOMFileNameIsSet())
    {
    otb_kwl = ReadGeometryFromImage(lFileNameOssimKeywordlist,!m_FilenameHelper->GetSkipRpcTag());
    otbMsgDevMacro(<< "Loading internal kwl");
    }
  else
    {
    otb_kwl = ReadGeometryFromGEOMFile(m_FilenameHelper->GetExtGEOMFileName());
    otbMsgDevMacro(<< "Loading external kwl");
    }

  // Pass the depth parameter from the tilemap around
  if (strcmp(this->m_ImageIO->GetNameOfClass(), "TileMapImageIO") == 0)
    {
    typename TileMapImageIO::Pointer imageIO = dynamic_cast<TileMapImageIO*>(this->GetImageIO());
    std::ostringstream depth;
    depth << imageIO->GetDepth();
    otb_kwl.AddKey("depth", depth.str());
    }

  otbMsgDevMacro(<< otb_kwl);

  // Don't add an empty ossim keyword list
  if( otb_kwl.GetSize() != 0 )
    {
      itk::EncapsulateMetaData<ImageKeywordlist>(dict,
                                                 MetaDataKey::OSSIMKeywordlistKey, otb_kwl);
    }
  else
    {
    //
    // For image with world file, we have a non-null GeoTransform, an empty kwl, but no projection ref.
    // Decision made here : if the keywordlist is empty, and the geotransform is not the identity,
    // then assume the file is in WGS84
    //
    std::string projRef;
    itk::ExposeMetaData(dict, MetaDataKey::ProjectionRefKey, projRef);

    // Compute spacing for an identity geotransform at current resolution
    unsigned int resolution = 0;
    itk::ExposeMetaData<unsigned int>(dict,
                                      MetaDataKey::ResolutionFactor,
                                      resolution);
    double idSpacing = 1.0;
    if (resolution != 0)
      idSpacing = 1.0 * vcl_pow((double)2.0, (double)resolution);

    const double Epsilon = 1.0E-12;
    if ( projRef.empty()
         && vcl_abs(origin[0] - 0.5 * spacing[0]) > Epsilon
         && vcl_abs(origin[1] - 0.5 * spacing[1]) > Epsilon
         && vcl_abs(spacing[0] - idSpacing) > Epsilon
         && vcl_abs(spacing[1] - idSpacing) > Epsilon)
      {
      std::string wgs84ProjRef =
              "GEOGCS[\"GCS_WGS_1984\", DATUM[\"D_WGS_1984\", SPHEROID[\"WGS_1984\", 6378137, 298.257223563]],"
              "PRIMEM[\"Greenwich\", 0], UNIT[\"Degree\", 0.017453292519943295]]";

      itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, wgs84ProjRef);
      }
    }

  // If Skip ProjectionRef is activated, remove ProjRef from dict
  if (m_FilenameHelper->GetSkipCarto())
    {
    itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, "");
    }

  //Copy MetaDataDictionary from instantiated reader to output image.
  if (!m_FilenameHelper->GetSkipGeom())
    {
    output->SetMetaDataDictionary(this->m_ImageIO->GetMetaDataDictionary());
    this->SetMetaDataDictionary(this->m_ImageIO->GetMetaDataDictionary());
    }
  else
    {
    itk::MetaDataDictionary dictLight;
    std::string projRef;
    itk::ExposeMetaData(dict, MetaDataKey::ProjectionRefKey, projRef);
    itk::EncapsulateMetaData<std::string>(dictLight, MetaDataKey::ProjectionRefKey, projRef);
    output->SetMetaDataDictionary(dictLight);
    this->SetMetaDataDictionary(dictLight);
    }

  typedef typename TOutputImage::IndexType IndexType;

  IndexType start;
  start.Fill(0);

  ImageRegionType region;
  region.SetSize(dimSize);
  region.SetIndex(start);

// THOMAS : ajout
// If a VectorImage, this requires us to set the
// VectorLength before allocate
  if (strcmp(output->GetNameOfClass(), "VectorImage") == 0)
    {
    typedef typename TOutputImage::AccessorFunctorType AccessorFunctorType;
    AccessorFunctorType::SetVectorLength(output, this->m_ImageIO->GetNumberOfComponents());
    }

  output->SetLargestPossibleRegion(region);

}

template <class TOutputImage, class ConvertPixelTraits>
void
ImageFileReader<TOutputImage, ConvertPixelTraits>
::TestFileExistanceAndReadability()
{
  // Test if the file a server name
  if  (this->m_FileName[0] == 'h'
       && this->m_FileName[1] == 't'
       && this->m_FileName[2] == 't'
       && this->m_FileName[3] == 'p')
    {
    // If the url is not available
    if (!m_Curl->TestUrlAvailability(this->m_FileName))
      {
      otb::ImageFileReaderException e(__FILE__, __LINE__);
      std::ostringstream msg;
      msg << "File name is an http address, but curl fails to connect to it "
          << std::endl << "Filename = " << this->m_FileName
          << std::endl;
      e.SetDescription(msg.str().c_str());
      throw e;
      }
    return;
    }

  // Test if the file exists.
  if (!itksys::SystemTools::FileExists(this->m_FileName.c_str()))
    {
    otb::ImageFileReaderException e(__FILE__, __LINE__);
    std::ostringstream msg;
    msg << "The file doesn't exist. "
        << std::endl << "Filename = " << this->m_FileName
        << std::endl;
    e.SetDescription(msg.str().c_str());
    throw e;
    return;
    }

  // Test if the file can be open for reading access.
  //Only if m_FileName specify a filename (not a dirname)
  if (itksys::SystemTools::FileExists(this->m_FileName.c_str(), true) == true)
    {
    std::ifstream readTester;
    readTester.open(this->m_FileName.c_str());
    if (readTester.fail())
      {
      readTester.close();
      std::ostringstream msg;
      msg << "The file couldn't be opened for reading. "
          << std::endl << "Filename: " << this->m_FileName
          << std::endl;
      otb::ImageFileReaderException e(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
      throw e;
      return;

      }
    readTester.close();
    }
}

template <class TOutputImage, class ConvertPixelTraits>
bool
ImageFileReader<TOutputImage, ConvertPixelTraits>
::GetGdalReadImageFileName(const std::string& filename, std::string& GdalFileName)
{
  std::vector<std::string> listFileSearch;
  listFileSearch.push_back("DAT_01.001");
  listFileSearch.push_back("dat_01.001"); // RADARSAT or SAR_ERS2
  listFileSearch.push_back("IMAGERY.TIF");
  listFileSearch.push_back("imagery.tif"); //For format SPOT5TIF
// Not recognized as a supported file format by GDAL.
//        listFileSearch.push_back("IMAGERY.BIL"); listFileSearch.push_back("imagery.bil"); //For format SPOT5BIL
  listFileSearch.push_back("IMAG_01.DAT");
  listFileSearch.push_back("imag_01.dat"); //For format SPOT4

  std::string str_FileName;
  bool fic_trouve(false);

  // Si c'est un repertoire, on regarde le contenu pour voir si c'est pas du RADARSAT, ERS
  std::vector<std::string> listFileFind;
  listFileFind = System::Readdir(filename);
  if (listFileFind.empty() == false)
    {
    unsigned int cpt(0);
    while ((cpt < listFileFind.size()) && (fic_trouve == false))
      {
      str_FileName = std::string(listFileFind[cpt]);
      for (unsigned int i = 0; i < listFileSearch.size(); ++i)
        {
        if (str_FileName.compare(listFileSearch[i]) == 0)
          {
          GdalFileName = std::string(filename) + str_FileName; //listFileSearch[i];
          fic_trouve = true;
          }
        }
      ++cpt;
      }
    }
  else
    {
    std::string strFileName(filename);

    std::string extension = itksys::SystemTools::GetFilenameLastExtension(strFileName);
    if ((extension == ".HDR") || (extension == ".hdr"))
      {
      //Supprime l'extension
      GdalFileName = System::GetRootName(strFileName);
      }

    else
      {
      // Sinon le filename est le nom du fichier a ouvrir
      GdalFileName = std::string(filename);
      }
    fic_trouve = true;
    }
  otbMsgDevMacro(<< "lFileNameGdal : " << GdalFileName.c_str());
  otbMsgDevMacro(<< "fic_trouve : " << fic_trouve);
  return (fic_trouve);
}

template <class TOutputImage, class ConvertPixelTraits>
void
ImageFileReader<TOutputImage, ConvertPixelTraits>
::SetFileName(std::string extendedFileName)
{
  this->SetFileName(extendedFileName.c_str());
}

template <class TOutputImage, class ConvertPixelTraits>
void
ImageFileReader<TOutputImage, ConvertPixelTraits>
::SetFileName(const char* extendedFileName)
{
  if (extendedFileName)
    {
    this->m_FilenameHelper->SetExtendedFileName(extendedFileName);
    this->m_FileName = this->m_FilenameHelper->GetSimpleFileName();
    this->Modified();
    }
}

template <class TOutputImage, class ConvertPixelTraits>
const char*
ImageFileReader<TOutputImage, ConvertPixelTraits>
::GetFileName () const
{
return this->m_FilenameHelper->GetSimpleFileName();
}

template <class TOutputImage, class ConvertPixelTraits>
std::vector<unsigned int>
ImageFileReader<TOutputImage, ConvertPixelTraits>
::GetAvailableResolutions()
 {
  this->UpdateOutputInformation();

  std::vector<unsigned int> res;
  // GDAL image IO
  if (strcmp(this->m_ImageIO->GetNameOfClass(), "GDALImageIO") == 0)
    {
    typename GDALImageIO::Pointer imageIO = dynamic_cast<GDALImageIO*>(this->GetImageIO());
    imageIO->GetAvailableResolutions(res);
    return res;
    }

#if defined(OTB_USE_JPEG2000)
  // JPEG2000 image IO
  if (strcmp(this->m_ImageIO->GetNameOfClass(), "JPEG2000ImageIO") == 0)
    {
    typename JPEG2000ImageIO::Pointer imageIO = dynamic_cast<JPEG2000ImageIO*>(this->GetImageIO());
    imageIO->GetAvailableResolutions(res);
    return res;
    }
#endif

  // other imageIO
  res.clear();
  res.push_back(0);
  return res;
 }

template <class TOutputImage, class ConvertPixelTraits>
bool
ImageFileReader<TOutputImage, ConvertPixelTraits>
::GetResolutionsInfo( std::vector<unsigned int>& res, std::vector<std::string>& desc)
 {
  this->UpdateOutputInformation();

  // GDAL image IO
  if (strcmp(this->m_ImageIO->GetNameOfClass(), "GDALImageIO") == 0)
    {
    typename GDALImageIO::Pointer imageIO = dynamic_cast<GDALImageIO*>(this->GetImageIO());
    imageIO->GetResolutionInfo(res,desc);
    return true;
    }

#if defined(OTB_USE_JPEG2000)
  // JPEG2000 image IO
  if (strcmp(this->m_ImageIO->GetNameOfClass(), "JPEG2000ImageIO") == 0)
    {
    typename JPEG2000ImageIO::Pointer imageIO = dynamic_cast<JPEG2000ImageIO*>(this->GetImageIO());
    imageIO->GetResolutionInfo(res,desc);
    return true;
    }
#endif

  // other imageIO
  res.clear();
  desc.clear();

  res.push_back(0);

  // TODO MSD : manage the tile information or not ?
  std::ostringstream oss;
  oss << "Resolution: " << 0 << " (Image [w x h]: "
      << this->m_ImageIO->GetDimensions(0) << "x" << this->m_ImageIO->GetDimensions(1)
      << ", Tile [w x h]: " <<  "not defined x not defined" << ")";
  desc.push_back(oss.str());

  return true;
 }

template <class TOutputImage, class ConvertPixelTraits>
unsigned int
ImageFileReader<TOutputImage, ConvertPixelTraits>
::GetNbOfAvailableOverviews()
 {
  this->UpdateOutputInformation();

  // GDAL image IO
  if (strcmp(this->m_ImageIO->GetNameOfClass(), "GDALImageIO") == 0)
    {
    typename GDALImageIO::Pointer imageIO = dynamic_cast<GDALImageIO*>(this->GetImageIO());
    return imageIO->GetNumberOfOverviews();
    }

#if defined(OTB_USE_JPEG2000)
  // JPEG2000 image IO
  if (strcmp(this->m_ImageIO->GetNameOfClass(), "JPEG2000ImageIO") == 0)
    {
    typename JPEG2000ImageIO::Pointer imageIO = dynamic_cast<JPEG2000ImageIO*>(this->GetImageIO());
    return imageIO->GetNumberOfOverviews();
    }
#endif

  // other imageIO whi
  return 0;
 }

template <class TOutputImage, class ConvertPixelTraits>
bool
ImageFileReader<TOutputImage, ConvertPixelTraits>
::HasOverviewsSupport()
 {
  this->UpdateOutputInformation();

  // GDAL image IO
  if (strcmp(this->m_ImageIO->GetNameOfClass(), "GDALImageIO") == 0)
    {
    return true;
    }

#if defined(OTB_USE_JPEG2000)
  // JPEG2000 image IO
  if (strcmp(this->m_ImageIO->GetNameOfClass(), "JPEG2000ImageIO") == 0)
    {
    return true;
    }
#endif

  // Other imageIO
  return false;
 }

template <class TOutputImage, class ConvertPixelTraits>
void
ImageFileReader<TOutputImage, ConvertPixelTraits>
::DoConvertBuffer(void* inputData,
                  size_t numberOfPixels)
{
  // get the pointer to the destination buffer
  OutputImagePixelType *outputData =
    this->GetOutput()->GetPixelContainer()->GetBufferPointer();

  // TODO:
  // Pass down the PixelType (RGB, VECTOR, etc.) so that any vector to
  // scalar conversion be type specific. i.e. RGB to scalar would use
  // a formula to convert to luminance, VECTOR to scalar would use
  // vector magnitude.


  // Create a macro as this code is a bit lengthy and repetitive
  // if the ImageIO pixel type is typeid(type) then use the ConvertPixelBuffer
  // class to convert the data block to TOutputImage's pixel type
  // see DefaultConvertPixelTraits and ConvertPixelBuffer

  // The first else if block applies only to images of type itk::VectorImage
  // VectorImage needs to copy out the buffer differently.. The buffer is of
  // type InternalPixelType, but each pixel is really 'k' consecutive pixels.

#define OTB_CONVERT_BUFFER_IF_BLOCK(type)               \
 else if( m_ImageIO->GetComponentTypeInfo() == typeid(type) )   \
   {   \
   if( strcmp( this->GetOutput()->GetNameOfClass(), "VectorImage" ) == 0 ) \
     { \
     ConvertPixelBuffer<                                 \
      type,                                             \
      OutputImagePixelType,                             \
      ConvertPixelTraits                                \
      >                                                 \
      ::ConvertVectorImage(                             \
       static_cast<type*>(inputData),                  \
       m_ImageIO->GetNumberOfComponents(),             \
       outputData,                                     \
       numberOfPixels);              \
     } \
   else \
     { \
     ConvertPixelBuffer<                                 \
      type,                                             \
      OutputImagePixelType,                             \
      ConvertPixelTraits                                \
      >                                                 \
      ::Convert(                                        \
        static_cast<type*>(inputData),                  \
        m_ImageIO->GetNumberOfComponents(),             \
        outputData,                                     \
        numberOfPixels);              \
      } \
    }
#define OTB_CONVERT_CBUFFER_IF_BLOCK(type)               \
 else if( m_ImageIO->GetComponentTypeInfo() == typeid(type) )   \
   {  \
   if( strcmp( this->GetOutput()->GetNameOfClass(), "VectorImage" ) == 0 ) \
     { \
     if( (typeid(OutputImagePixelType) == typeid(std::complex<double>))     \
         || (typeid(OutputImagePixelType) == typeid(std::complex<float>))   \
         || (typeid(OutputImagePixelType) == typeid(std::complex<int>))     \
         || (typeid(OutputImagePixelType) == typeid(std::complex<short>)) ) \
       {\
       ConvertPixelBuffer<                                 \
        type::value_type,        \
        OutputImagePixelType,                             \
        ConvertPixelTraits                                \
        >                                                 \
        ::ConvertComplexVectorImageToVectorImageComplex(                             \
         static_cast<type*>(inputData),                \
         m_ImageIO->GetNumberOfComponents(),             \
         outputData,                                     \
         numberOfPixels); \
       }\
     else\
       {\
       ConvertPixelBuffer<                                 \
        type::value_type,        \
        OutputImagePixelType,                             \
        ConvertPixelTraits                                \
        >                                                  \
        ::ConvertComplexVectorImageToVectorImage(                             \
         static_cast<type*>(inputData),                \
         m_ImageIO->GetNumberOfComponents(),             \
         outputData,                                     \
         numberOfPixels);              \
       }\
     } \
   else \
     { \
     ConvertPixelBuffer<                                 \
      type::value_type,        \
      OutputImagePixelType,                             \
      ConvertPixelTraits                                \
      >                                                 \
      ::ConvertComplexToGray(                                        \
       static_cast<type*>(inputData),                  \
       m_ImageIO->GetNumberOfComponents(),             \
       outputData,                                     \
       numberOfPixels);              \
     } \
   }

  if(0)
    {
    }
  OTB_CONVERT_BUFFER_IF_BLOCK(unsigned char)
  OTB_CONVERT_BUFFER_IF_BLOCK(char)
  OTB_CONVERT_BUFFER_IF_BLOCK(unsigned short)
  OTB_CONVERT_BUFFER_IF_BLOCK(short)
  OTB_CONVERT_BUFFER_IF_BLOCK(unsigned int)
  OTB_CONVERT_BUFFER_IF_BLOCK(int)
  OTB_CONVERT_BUFFER_IF_BLOCK(unsigned long)
  OTB_CONVERT_BUFFER_IF_BLOCK(long)
  OTB_CONVERT_BUFFER_IF_BLOCK(float)
  OTB_CONVERT_BUFFER_IF_BLOCK(double)
  OTB_CONVERT_CBUFFER_IF_BLOCK(std::complex<short>)
  OTB_CONVERT_CBUFFER_IF_BLOCK(std::complex<int>)
  OTB_CONVERT_CBUFFER_IF_BLOCK(std::complex<float>)
  OTB_CONVERT_CBUFFER_IF_BLOCK(std::complex<double>)
  else
    {
    otb::ImageFileReaderException e(__FILE__, __LINE__);
    std::ostringstream msg;
    msg <<"Couldn't convert component type: "
        << std::endl << "    "
        << m_ImageIO->GetComponentTypeAsString(m_ImageIO->GetComponentType())
        << std::endl << "to one of: "
        << std::endl << "    " << typeid(unsigned char).name()
        << std::endl << "    " << typeid(char).name()
        << std::endl << "    " << typeid(unsigned short).name()
        << std::endl << "    " << typeid(short).name()
        << std::endl << "    " << typeid(unsigned int).name()
        << std::endl << "    " << typeid(int).name()
        << std::endl << "    " << typeid(unsigned long).name()
        << std::endl << "    " << typeid(long).name()
        << std::endl << "    " << typeid(float).name()
        << std::endl << "    " << typeid(double).name()
        << std::endl;
    e.SetDescription(msg.str().c_str());
    e.SetLocation(ITK_LOCATION);
    throw e;
    return;
    }
#undef OTB_CONVERT_BUFFER_IF_BLOCK
#undef OTB_CONVERT_CBUFFER_IF_BLOCK
}

} //namespace otb

#endif
