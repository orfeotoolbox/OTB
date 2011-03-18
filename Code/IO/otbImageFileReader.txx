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

#include "itkMetaDataObject.h"

#include "otbMacro.h"
#include "otbSystem.h"
#include "otbImageIOFactory.h"
#include "otbImageKeywordlist.h"
#include "otbMetaDataKey.h"


#include "base/ossimKeywordlist.h"
#include "projection/ossimProjection.h"
#include "ossim/ossimPluginProjectionFactory.h"

#include "imaging/ossimImageHandlerRegistry.h"
#include "imaging/ossimImageHandler.h"
#include "init/ossimInit.h"
#include "projection/ossimProjectionFactoryRegistry.h"


#include "otbTileMapImageIO.h" //FIXME find a better way
#include "otbGDALImageIO.h" //FIXME find a better way
#include "projection/ossimTileMapModel.h"

#include <itksys/SystemTools.hxx>
#include <fstream>

#include "otbCurlHelper.h"

namespace otb
{

template<class T>
bool PixelIsComplex(const std::complex<T>& dummy)
{
  return true;
}
template<class T>
bool PixelIsComplex(const T& dummy)
{
  return false;
}

template <class TOutputImage>
ImageFileReader<TOutputImage>
::ImageFileReader() : itk::ImageFileReader<TOutputImage>(), m_DatasetNumber(0)
{
  m_Curl = CurlHelper::New();
}

template <class TOutputImage>
ImageFileReader<TOutputImage>
::~ImageFileReader()
{
}

template <class TOutputImage>
void ImageFileReader<TOutputImage>
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
}

template <class TOutputImage>
void
ImageFileReader<TOutputImage>
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

  typedef itk::DefaultConvertPixelTraits<ITK_TYPENAME TOutputImage::IOPixelType> ConvertIOPixelTraits;
  typedef itk::DefaultConvertPixelTraits<ITK_TYPENAME TOutputImage::PixelType> ConvertPixelTraits;

  if (this->m_ImageIO->GetComponentTypeInfo()
      == typeid(ITK_TYPENAME ConvertPixelTraits::ComponentType)
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

template <class TOutputImage>
void
ImageFileReader<TOutputImage>
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
      throw itk::ImageFileReaderException(__FILE__, __LINE__,
                                          "Invalid output object type");
      }
    }
}

template <class TOutputImage>
void
ImageFileReader<TOutputImage>
::GenerateOutputInformation(void)
{

  typename TOutputImage::Pointer output = this->GetOutput();

  itkDebugMacro(<< "Reading file for GenerateOutputInformation()" << this->m_FileName);

  // Check to see if we can read the file given the name or prefix
  //
  if (this->m_FileName == "")
    {
    throw itk::ImageFileReaderException(__FILE__, __LINE__, "FileName must be specified");
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
    this->m_ImageIO = ImageIOFactory::CreateImageIO(this->m_FileName.c_str(), itk::ImageIOFactory::ReadMode);
    }

  if (this->m_ImageIO.IsNull())
    {
    this->Print(std::cerr);
    itk::ImageFileReaderException e(__FILE__, __LINE__);
    itk::OStringStream msg;
    msg << " Could not create IO object for file "
        << this->m_FileName.c_str() << std::endl;
    msg << "  Tried to create one of the following:" << std::endl;
    std::list<itk::LightObject::Pointer> allobjects =
      itk::ObjectFactoryBase::CreateAllInstance("itkImageIOBase");
    for (std::list<itk::LightObject::Pointer>::iterator i = allobjects.begin();
         i != allobjects.end(); ++i)
      {
      itk::ImageIOBase* io = dynamic_cast<itk::ImageIOBase*>(i->GetPointer());
      msg << "    " << io->GetNameOfClass() << std::endl;
      }
    msg << "  You probably failed to set a file suffix, or" << std::endl;
    msg << "    set the suffix to an unsupported type." << std::endl;
    e.SetDescription(msg.str().c_str());
    throw e;
    return;
    }

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
    imageIO->SetDatasetNumber(m_DatasetNumber);
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
      origin[i] = 0.0;
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

  output->SetSpacing(spacing);       // Set the image spacing
  output->SetOrigin(origin);         // Set the image origin
  output->SetDirection(direction);   // Set the image direction cosines

  // Trying to read ossim MetaData
  bool             hasMetaData = false;
  ossimKeywordlist geom_kwl, tmp_kwl, tmp_kwl2; // = new ossimKeywordlist();

  // Test the plugins factory
  /** Before, the pluginfactory was tested if the ossim one returned false.
      But in the case TSX, the images tif were considered as ossimQuickbirdTiffTileSource
      thus a TSX tif image wasn't read with TSX Model. We don't use the ossimRegisteryFactory
      because the default include factory contains ossimQuickbirdTiffTileSource. */
  ossimProjection * projection = ossimplugins::ossimPluginProjectionFactory::instance()
                                 ->createProjection(ossimFilename(lFileNameOssimKeywordlist.c_str()), 0);

  if (!projection)
    {
    otbMsgDevMacro(<< "OSSIM Instantiate projection FAILED ! ");
    }
  else
    {
    otbMsgDevMacro(<< "OSSIM Instantiate projection SUCCESS ! ");
    hasMetaData = projection->saveState(geom_kwl);

    // Free memory
    delete projection;
    }

  if (!hasMetaData)
    {
    // Add the radar factory
    // ossimImageHandlerRegistry::instance()->addFactory(ossimImageHandlerSarFactory::instance());

    ossimImageHandler* handler = ossimImageHandlerRegistry::instance()
                                 ->open(ossimFilename(lFileNameOssimKeywordlist.c_str()));
    if (!handler)
      {
      otbMsgDevMacro(<< "OSSIM Open Image FAILED ! ");
      }

    else
      {
      otbMsgDevMacro(<< "OSSIM Open Image SUCCESS ! ");
      //     hasMetaData = handler->getImageGeometry(geom_kwl);
      ossimProjection* projection = handler->getImageGeometry()->getProjection();

      if (projection)
        {
        if (projection->getClassName() == "ossimTileMapModel")
          {
          //FIXME find a better way to do that
          //we need to pass the depth information which in on the IO to the projection
          //to be handle throught the kwl
          typename TileMapImageIO::Pointer imageIO = dynamic_cast<TileMapImageIO*>(this->GetImageIO());
          if (imageIO.IsNotNull())
            {
            dynamic_cast<ossimTileMapModel*>(projection)->setDepth(imageIO->GetDepth());
            }
          }
        hasMetaData = projection->saveState(geom_kwl);
//             delete projection; //FIXME find out where this should occur
        }
      }
    // Free memory
    delete handler;
    }

  if (!hasMetaData)
    {
    otbMsgDevMacro(<< "OSSIM MetaData not present ! ");
    }
  else
    {
    otbMsgDevMacro(<< "OSSIM MetaData present ! ");
    otbMsgDevMacro(<< geom_kwl);

    // Update otb Keywordlist
    ImageKeywordlist otb_kwl;
    otb_kwl.SetKeywordlist(geom_kwl);

    // Update itk MetaData Dictionary

    itk::MetaDataDictionary& dict = this->m_ImageIO->GetMetaDataDictionary();

    itk::EncapsulateMetaData<ImageKeywordlist>(dict,
                                               MetaDataKey::OSSIMKeywordlistKey, otb_kwl);

    }

  //Copy MetaDataDictionary from instantiated reader to output image.
  output->SetMetaDataDictionary(this->m_ImageIO->GetMetaDataDictionary());
  this->SetMetaDataDictionary(this->m_ImageIO->GetMetaDataDictionary());

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

template <class TOutputImage>
void
ImageFileReader<TOutputImage>
::TestFileExistanceAndReadability()
{
  // Test if the file a server name
  if  (this->m_FileName[0] == 'h'
       && this->m_FileName[1] == 't'
       && this->m_FileName[2] == 't'
       && this->m_FileName[3] == 'p')
    {
    int res = m_Curl->TestUrlAvailability(this->m_FileName);
    if (res != 0 && res != 63) // 63 stands for filesize exceed
      {
      itk::ImageFileReaderException e(__FILE__, __LINE__);
      itk::OStringStream msg;
      msg << "File name is an http address, but curl fails to connect to it "
          << std::endl << "Filename = " << this->m_FileName
          << std::endl << "Curl error code = " << res
          << std::endl;
      e.SetDescription(msg.str().c_str());
      throw e;
      }
    return;
    }

  // Test if we have an hdf file with dataset spec
  std::string realfile(this->m_FileName);
  unsigned int datasetNum;
  if (System::ParseHdfFileName(this->m_FileName, realfile, datasetNum))
    {
    otbMsgDevMacro(<< "HDF name with dataset specification detected");
    otbMsgDevMacro(<< " - " << realfile);
    otbMsgDevMacro(<< " - " << datasetNum);
    this->m_FileName = realfile;
    m_DatasetNumber = datasetNum;
    }

  // Test if the file exists.
  if (!itksys::SystemTools::FileExists(this->m_FileName.c_str()))
    {
    itk::ImageFileReaderException e(__FILE__, __LINE__);
    itk::OStringStream msg;
    msg << "The file doesn't exist. "
        << std::endl << "Filename = " << this->m_FileName
        << std::endl;
    e.SetDescription(msg.str().c_str());
    throw e;
    return;
    }

  // Test if the file can be open for reading access.
  //Only if m_FileName specify a filename (not a dirname)
  if (System::IsAFileName(this->m_FileName) == true)
    {
    std::ifstream readTester;
    readTester.open(this->m_FileName.c_str());
    if (readTester.fail())
      {
      readTester.close();
      itk::OStringStream msg;
      msg << "The file couldn't be opened for reading. "
          << std::endl << "Filename: " << this->m_FileName
          << std::endl;
      itk::ImageFileReaderException e(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
      throw e;
      return;

      }
    readTester.close();
    }
}

template <class TOutputImage>
bool
ImageFileReader<TOutputImage>
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
      cpt++;
      }
    }
  else
    {
    std::string strFileName(filename);

    std::string extension = System::GetExtension(strFileName);
    if ((extension == "HDR") || (extension == "hdr"))
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

} //namespace otb

#endif
