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

#ifndef otbImageFileWriter_hxx
#define otbImageFileWriter_hxx

#include "otbImageFileWriter.h"
#include "itkImageFileWriter.h"

#include "itkObjectFactoryBase.h"

#include "itkImageRegionMultidimensionalSplitter.h"
#include "otbImageIOFactory.h"

#include "itkImageRegionIterator.h"

#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"
#include "otbMetaDataKey.h"

#include "otbConfigure.h"

#include "otbNumberOfDivisionsStrippedStreamingManager.h"
#include "otbNumberOfDivisionsTiledStreamingManager.h"
#include "otbNumberOfLinesStrippedStreamingManager.h"
#include "otbRAMDrivenStrippedStreamingManager.h"
#include "otbTileDimensionTiledStreamingManager.h"
#include "otbRAMDrivenTiledStreamingManager.h"
#include "otbRAMDrivenAdaptativeStreamingManager.h"

#include "otb_boost_tokenizer_header.h"

#include "otbStringUtils.h"
#include "otbUtils.h"

namespace otb
{

/**
 *
 */
template <class TInputImage>
ImageFileWriter<TInputImage>::ImageFileWriter()
  : m_NumberOfDivisions(0),
    m_CurrentDivision(0),
    m_DivisionProgress(0.0),
    m_UserSpecifiedImageIO(true),
    m_UserSpecifiedIORegion(false),
    m_FactorySpecifiedImageIO(false),
    m_UseCompression(false),
    m_UseInputMetaDataDictionary(false),
    m_WriteGeomFile(false),
    m_FilenameHelper(),
    m_IsObserving(true),
    m_ObserverID(0),
    m_IOComponents(0)
{
  // Init output index shift
  m_ShiftOutputIndex.Fill(0);

  // By default, we use tiled streaming, with automatic tile size
  // We don't set any parameter, so the memory size is retrieved from the OTB configuration options
  this->SetAutomaticAdaptativeStreaming();

  m_FilenameHelper = FNameHelperType::New();
}

/**
 *
 */
template <class TInputImage>
ImageFileWriter<TInputImage>::~ImageFileWriter()
{
}

template <class TInputImage>
void ImageFileWriter<TInputImage>::SetNumberOfDivisionsStrippedStreaming(unsigned int nbDivisions)
{
  typedef NumberOfDivisionsStrippedStreamingManager<TInputImage>  NumberOfDivisionsStrippedStreamingManagerType;
  typename NumberOfDivisionsStrippedStreamingManagerType::Pointer streamingManager = NumberOfDivisionsStrippedStreamingManagerType::New();
  streamingManager->SetNumberOfDivisions(nbDivisions);
  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void ImageFileWriter<TInputImage>::SetNumberOfDivisionsTiledStreaming(unsigned int nbDivisions)
{
  typedef NumberOfDivisionsTiledStreamingManager<TInputImage>  NumberOfDivisionsTiledStreamingManagerType;
  typename NumberOfDivisionsTiledStreamingManagerType::Pointer streamingManager = NumberOfDivisionsTiledStreamingManagerType::New();
  streamingManager->SetNumberOfDivisions(nbDivisions);
  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void ImageFileWriter<TInputImage>::SetNumberOfLinesStrippedStreaming(unsigned int nbLinesPerStrip)
{
  typedef NumberOfLinesStrippedStreamingManager<TInputImage>  NumberOfLinesStrippedStreamingManagerType;
  typename NumberOfLinesStrippedStreamingManagerType::Pointer streamingManager = NumberOfLinesStrippedStreamingManagerType::New();
  streamingManager->SetNumberOfLinesPerStrip(nbLinesPerStrip);
  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void ImageFileWriter<TInputImage>::SetAutomaticStrippedStreaming(unsigned int availableRAM, double bias)
{
  typedef RAMDrivenStrippedStreamingManager<TInputImage>  RAMDrivenStrippedStreamingManagerType;
  typename RAMDrivenStrippedStreamingManagerType::Pointer streamingManager = RAMDrivenStrippedStreamingManagerType::New();
  streamingManager->SetAvailableRAMInMB(availableRAM);
  streamingManager->SetBias(bias);
  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void ImageFileWriter<TInputImage>::SetTileDimensionTiledStreaming(unsigned int tileDimension)
{
  typedef TileDimensionTiledStreamingManager<TInputImage>  TileDimensionTiledStreamingManagerType;
  typename TileDimensionTiledStreamingManagerType::Pointer streamingManager = TileDimensionTiledStreamingManagerType::New();
  streamingManager->SetTileDimension(tileDimension);
  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void ImageFileWriter<TInputImage>::SetAutomaticTiledStreaming(unsigned int availableRAM, double bias)
{
  typedef RAMDrivenTiledStreamingManager<TInputImage>  RAMDrivenTiledStreamingManagerType;
  typename RAMDrivenTiledStreamingManagerType::Pointer streamingManager = RAMDrivenTiledStreamingManagerType::New();
  streamingManager->SetAvailableRAMInMB(availableRAM);
  streamingManager->SetBias(bias);
  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void ImageFileWriter<TInputImage>::SetAutomaticAdaptativeStreaming(unsigned int availableRAM, double bias)
{
  typedef RAMDrivenAdaptativeStreamingManager<TInputImage>  RAMDrivenAdaptativeStreamingManagerType;
  typename RAMDrivenAdaptativeStreamingManagerType::Pointer streamingManager = RAMDrivenAdaptativeStreamingManagerType::New();
  streamingManager->SetAvailableRAMInMB(availableRAM);
  streamingManager->SetBias(bias);
  m_StreamingManager = streamingManager;
}

/**
 *
 */
template <class TInputImage>
void ImageFileWriter<TInputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "File Name: " << (m_FileName.data() ? m_FileName.data() : "(none)") << std::endl;

  os << indent << "Image IO: ";
  if (m_ImageIO.IsNull())
  {
    os << "(none)\n";
  }
  else
  {
    os << m_ImageIO << "\n";
  }

  os << indent << "IO Region: " << m_IORegion << "\n";

  if (m_UseCompression)
  {
    os << indent << "Compression: On\n";
  }
  else
  {
    os << indent << "Compression: Off\n";
  }

  if (m_UseInputMetaDataDictionary)
  {
    os << indent << "UseInputMetaDataDictionary: On\n";
  }
  else
  {
    os << indent << "UseInputMetaDataDictionary: Off\n";
  }

  if (m_FactorySpecifiedImageIO)
  {
    os << indent << "FactorySpecifiedmageIO: On\n";
  }
  else
  {
    os << indent << "FactorySpecifiedmageIO: Off\n";
  }
}

//---------------------------------------------------------
template <class TInputImage>
void ImageFileWriter<TInputImage>::SetIORegion(const itk::ImageIORegion& region)
{
  if (m_IORegion != region)
  {
    m_IORegion = region;
    this->Modified();
    m_UserSpecifiedIORegion = true;
  }
}

template <class TInputImage>
void ImageFileWriter<TInputImage>::SetInput(const InputImageType* input)
{
  this->ProcessObject::SetNthInput(0, const_cast<InputImageType*>(input));
}

template <class TInputImage>
const TInputImage* ImageFileWriter<TInputImage>::GetInput()
{
  if (this->GetNumberOfInputs() < 1)
  {
    return nullptr;
  }

  return static_cast<const InputImageType*>(this->ProcessObject::GetInput(0));
}

/** Prepare everything and call m_ImageIO.WriteInformation() */
template <class TInputImage>
void ImageFileWriter<TInputImage>::GenerateOutputInformation(void)
{
  // Update output information on input image
  InputImagePointer inputPtr = const_cast<InputImageType*>(this->GetInput());

  // Make sure input is available
  if (inputPtr.IsNull())
  {
    itkExceptionMacro(<< "No input to writer");
  }

  otb::Logger::Instance()->LogSetupInformation();

  /** Parse streaming modes */
  if (m_FilenameHelper->StreamingTypeIsSet())
  {
    otbLogMacro(
        Warning,
        << "Streaming configuration through extended filename is used. Any previous streaming configuration (ram value, streaming mode ...) will be ignored.");

    std::string type = m_FilenameHelper->GetStreamingType();

    std::string sizemode = "auto";

    if (m_FilenameHelper->StreamingSizeModeIsSet())
    {
      sizemode = m_FilenameHelper->GetStreamingSizeMode();
    }

    unsigned int sizevalue = 0;
    // Save the DefaultRAM value for later
    unsigned int oldDefaultRAM = m_StreamingManager->GetDefaultRAM();
    if (sizemode == "auto")
    {
      sizevalue = oldDefaultRAM;
    }

    if (m_FilenameHelper->StreamingSizeValueIsSet())
    {
      sizevalue = static_cast<unsigned int>(m_FilenameHelper->GetStreamingSizeValue());
    }

    if (type == "auto")
    {
      if (sizemode != "auto")
      {
        otbLogMacro(Warning, << "In auto streaming type, the sizemode option will be ignored.");
      }
      if (sizevalue == 0)
      {
        otbLogMacro(Warning, << "sizemode is auto but sizevalue is 0. Value will be fetched from the OTB_MAX_RAM_HINT environment variable if set, or else use "
                                "the default value");
      }
      this->SetAutomaticAdaptativeStreaming(sizevalue);
    }
    else if (type == "tiled")
    {
      if (sizemode == "auto")
      {
        if (sizevalue == 0)
        {
          otbLogMacro(Warning, << "sizemode is auto but sizevalue is 0. Value will be fetched from the OTB_MAX_RAM_HINT environment variable if set, or else "
                                  "use the default value");
        }
        this->SetAutomaticTiledStreaming(sizevalue);
      }
      else if (sizemode == "nbsplits")
      {
        if (sizevalue == 0)
        {
          otbLogMacro(Warning, << "Streaming sizemode is set to nbsplits but sizevalue is 0. This will result in undefined behaviour. Please consider setting "
                                  "the sizevalue by using &streaming:sizevalue=x.");
        }
        this->SetNumberOfDivisionsTiledStreaming(sizevalue);
      }
      else if (sizemode == "height")
      {
        if (sizevalue == 0)
        {
          otbLogMacro(Warning, << "Streaming sizemode is set to height but sizevalue is 0. This will result in undefined behaviour. Please consider setting "
                                  "the sizevalue by using &streaming:sizevalue=x.");
        }

        this->SetTileDimensionTiledStreaming(sizevalue);
      }
    }
    else if (type == "stripped")
    {
      if (sizemode == "auto")
      {
        if (sizevalue == 0)
        {
          otbLogMacro(
              Warning, << "sizemode is auto but sizevalue is 0. Value will be fetched from configuration file if any, or from cmake configuration otherwise.");
        }

        this->SetAutomaticStrippedStreaming(sizevalue);
      }
      else if (sizemode == "nbsplits")
      {
        if (sizevalue == 0)
        {
          otbLogMacro(Warning, << "Streaming sizemode is set to nbsplits but sizevalue is 0. This will result in undefined behaviour. Please consider setting "
                                  "the sizevalue by using &streaming:sizevalue=x.");
        }
        this->SetNumberOfDivisionsStrippedStreaming(sizevalue);
      }
      else if (sizemode == "height")
      {
        if (sizevalue == 0)
        {
          otbLogMacro(Warning, << "Streaming sizemode is set to height but sizevalue is 0. This will result in undefined behaviour. Please consider setting "
                                  "the sizevalue by using &streaming:sizevalue=x.");
        }
        this->SetNumberOfLinesStrippedStreaming(sizevalue);
      }
    }
    else if (type == "none")
    {
      if (sizemode != "" || sizevalue != 0)
      {
        otbLogMacro(Warning, << "Streaming is explicitly disabled, sizemode and sizevalue will be ignored.");
      }
      this->SetNumberOfDivisionsTiledStreaming(0);
    }

    // since we change the m_StreamingManager under the hood, we copy the DefaultRAM
    // value to the new streamingManager.
    m_StreamingManager->SetDefaultRAM(oldDefaultRAM);
  }
  else
  {
    if (m_FilenameHelper->StreamingSizeValueIsSet() || m_FilenameHelper->StreamingSizeModeIsSet())
    {
      otbLogMacro(Warning, << "No streaming type is set, streaming sizemode and sizevalue will be ignored.");
    }
  }

  /** Prepare ImageIO  : create ImageFactory */

  if (m_FileName == "")
  {
    // Make sure that we can write the file given the name
    itkExceptionMacro(<< "No filename was specified");
  }


  // Make sure CanWriteFile is called
  //  either from ImageIOFactory::CreateImageIO or directly in this file
  // GDALImageIO uses it to store the filename
  //  and later answer to CanStreamWrite()
  // This is a needed workaround to a defect in the itk::ImageIO interface

  if (m_ImageIO.IsNull()) // try creating via factory
  {
    this->SetImageIO(ImageIOFactory::CreateImageIO(m_FileName.c_str(), otb::ImageIOFactory::WriteMode));

    m_FactorySpecifiedImageIO = true;
  }
  else
  {
    if (!m_ImageIO->CanWriteFile(m_FileName.c_str()))
    {
      if (m_FactorySpecifiedImageIO)
      {
        m_ImageIO                 = ImageIOFactory::CreateImageIO(m_FileName.c_str(), otb::ImageIOFactory::WriteMode);
        m_FactorySpecifiedImageIO = true;
      }
    }
  }

  if (m_ImageIO.IsNull())
  {
    itk::ImageFileWriterException e(__FILE__, __LINE__);
    std::ostringstream            msg;
    msg << "Cannot write image " << m_FileName << ". Probably unsupported format or incorrect filename extension.";
    e.SetDescription(msg.str());
    e.SetLocation(ITK_LOCATION);
    throw e;
  }

  // Manage extended filename
  if ((strcmp(m_ImageIO->GetNameOfClass(), "GDALImageIO") == 0) &&
      (m_FilenameHelper->gdalCreationOptionsIsSet() || m_FilenameHelper->WriteRPCTagsIsSet() || m_FilenameHelper->NoDataValueIsSet() || m_FilenameHelper->SrsValueIsSet()))
  {
    typename GDALImageIO::Pointer imageIO = dynamic_cast<GDALImageIO*>(m_ImageIO.GetPointer());

    if (imageIO.IsNull())
    {
      itk::ImageFileWriterException e(__FILE__, __LINE__);
      std::ostringstream            msg;
      msg << " ImageIO is of kind GDALImageIO, but fails to dynamic_cast (this should never happen)." << std::endl;
      e.SetDescription(msg.str());
      throw e;
    }

    imageIO->SetOptions(m_FilenameHelper->GetgdalCreationOptions());
    imageIO->SetWriteRPCTags(m_FilenameHelper->GetWriteRPCTags());
    if (m_FilenameHelper->NoDataValueIsSet())
      imageIO->SetNoDataList(m_FilenameHelper->GetNoDataList());
    if  (m_FilenameHelper->SrsValueIsSet())
	  imageIO->SetEpsgCode(m_FilenameHelper->GetSrsValue());
  }


  /** End of Prepare ImageIO  : create ImageFactory */

  /**
   * Grab the input
   */
  InputImageRegionType inputRegion = inputPtr->GetLargestPossibleRegion();

  /** Parse region size modes */
  if (m_FilenameHelper->BoxIsSet())
  {
    std::vector<unsigned int> boxVector;
    Utils::ConvertStringToVector(m_FilenameHelper->GetBox(), boxVector, "ExtendedFileName:box", ":");

    if (boxVector.size() != 4)
    {
      itk::ImageFileWriterException e(__FILE__, __LINE__);
      std::ostringstream            msg;
      msg << "Invalid box option " << m_FilenameHelper->GetBox() << ". The box should contains four elements: startx:starty:sizex:sizey";
      e.SetDescription(msg.str());
      e.SetLocation(ITK_LOCATION);
      throw e;
    }

    typename InputImageRegionType::IndexType start;
    typename InputImageRegionType::SizeType  size;
    start[0] = boxVector[0]; // first index on X
    start[1] = boxVector[1]; // first index on Y
    size[0]  = boxVector[2]; // size along X
    size[1]  = boxVector[3]; // size along Y

    inputRegion.SetSize(size);
    inputRegion.SetIndex(start);

    if (!inputRegion.Crop(inputPtr->GetLargestPossibleRegion()))
    {
      // Couldn't crop the region (requested region is outside the largest
      // possible region).  Throw an exception.

      // build an exception
      itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
      e.SetLocation(ITK_LOCATION);
      e.SetDescription("Requested box region is (at least partially) outside the largest possible region.");
      e.SetDataObject(inputPtr);
      throw e;
    }
    otbLogMacro(Info, << "Writing user defined region [" << start[0] << ", " << start[0] + size[0] - 1 << "]x[" << start[1] << ", " << start[1] + size[1]
                      << "]");
  }
  m_ShiftOutputIndex = inputRegion.GetIndex();

  /**
   * Determine of number of pieces to divide the input.  This will be the
   * minimum of what the user specified via SetNumberOfDivisionsStrippedStreaming()
   * and what the Splitter thinks is a reasonable value.
   */

  /** Control if the ImageIO is CanStreamWrite */
  if (m_ImageIO->CanStreamWrite() == false)
  {
    otbLogMacro(Warning, << "The file format of " << m_FileName << " does not support streaming. All data will be loaded to memory");
    this->SetNumberOfDivisionsStrippedStreaming(1);
  }

  /** Compare the buffered region  with the inputRegion which is the largest
  * possible region or a user defined region through extended filename
  * Not sure that if this modification is needed  */
  else if (inputPtr->GetBufferedRegion() == inputRegion)
  {
    otbLogMacro(Debug, << "Buffered region is the largest possible region, there is no need for streaming.");
    this->SetNumberOfDivisionsStrippedStreaming(1);
  }
  m_StreamingManager->PrepareStreaming(inputPtr, inputRegion);
  m_NumberOfDivisions = m_StreamingManager->GetNumberOfSplits();

  const auto firstSplitSize = m_StreamingManager->GetSplit(0).GetSize();
  otbLogMacro(Info, << "File " << m_FileName << " will be written in " << m_NumberOfDivisions << " blocks of " << firstSplitSize[0] << "x" << firstSplitSize[1]
                    << " pixels");

  //
  // Setup the ImageIO with information from inputPtr
  //
  typename TInputImage::PointType origin;
  inputPtr->TransformIndexToPhysicalPoint(inputRegion.GetIndex(), origin);
  const typename TInputImage::SpacingType&   spacing   = inputPtr->GetSpacing();
  const typename TInputImage::DirectionType& direction = inputPtr->GetDirection();
  m_ImageIO->SetNumberOfDimensions(TInputImage::ImageDimension);
  int direction_sign(0);
  for (unsigned int i = 0; i < TInputImage::ImageDimension; ++i)
  {
    if (direction[i][i] < 0)
      direction_sign = -1;
    else
      direction_sign = 1;
    // Final image size
    m_ImageIO->SetDimensions(i, inputRegion.GetSize(i));
    m_ImageIO->SetSpacing(i, direction_sign * spacing[i]);
    m_ImageIO->SetOrigin(i, origin[i]);

    vnl_vector<double> axisDirection(TInputImage::ImageDimension);
    // Please note: direction cosines are stored as columns of the
    // direction matrix
    for (unsigned int j = 0; j < TInputImage::ImageDimension; ++j)
    {
      axisDirection[j] = direction_sign * direction[j][i];
    }
    m_ImageIO->SetDirection(i, axisDirection);
  }

  m_ImageIO->SetUseCompression(m_UseCompression);
  m_ImageIO->SetMetaDataDictionary(inputPtr->GetMetaDataDictionary());

  /** Create Image file */
  // Setup the image IO for writing.
  //
  m_ImageIO->SetFileName(m_FileName);

  m_ImageIO->WriteImageInformation();
}

/**
 * Update method : update output information of input and write to file
 */
template <class TInputImage>
void ImageFileWriter<TInputImage>::Update()
{
  this->UpdateOutputInformation();

  this->SetAbortGenerateData(0);
  this->SetProgress(0.0);

  /**
   * Tell all Observers that the filter is starting
   */
  this->InvokeEvent(itk::StartEvent());

  this->UpdateProgress(0);
  m_CurrentDivision  = 0;
  m_DivisionProgress = 0;

  // Get the source process object
  InputImagePointer   inputPtr = const_cast<InputImageType*>(this->GetInput());
  itk::ProcessObject* source   = inputPtr->GetSource();
  m_IsObserving                = false;
  m_ObserverID                 = 0;

  // Check if source exists
  if (source)
  {
    typedef itk::MemberCommand<Self>      CommandType;
    typedef typename CommandType::Pointer CommandPointerType;

    CommandPointerType command = CommandType::New();
    command->SetCallbackFunction(this, &Self::ObserveSourceFilterProgress);

    m_ObserverID  = source->AddObserver(itk::ProgressEvent(), command);
    m_IsObserving = true;
  }
  else
  {
    otbLogMacro(Warning, << "Could not get the source process object. Progress report might be buggy");
  }

  /**
   * Loop over the number of pieces, execute the upstream pipeline on each
   * piece, and copy the results into the output image.
   */
  InputImageRegionType streamRegion;

  for (m_CurrentDivision = 0; m_CurrentDivision < m_NumberOfDivisions && !this->GetAbortGenerateData();
       m_CurrentDivision++, m_DivisionProgress = 0, this->UpdateFilterProgress())
  {
    streamRegion = m_StreamingManager->GetSplit(m_CurrentDivision);

    inputPtr->SetRequestedRegion(streamRegion);
    inputPtr->PropagateRequestedRegion();
    inputPtr->UpdateOutputData();

    // Write the whole image
    itk::ImageIORegion ioRegion(TInputImage::ImageDimension);
    for (unsigned int i = 0; i < TInputImage::ImageDimension; ++i)
    {
      ioRegion.SetSize(i, streamRegion.GetSize(i));
      // Set the ioRegion index using the shifted index ( (0,0 without box parameter))
      ioRegion.SetIndex(i, streamRegion.GetIndex(i) - m_ShiftOutputIndex[i]);
    }
    this->SetIORegion(ioRegion);
    m_ImageIO->SetIORegion(m_IORegion);

    // Start writing stream region in the image file
    this->GenerateData();
  }

  /**
   * If we ended due to aborting, push the progress up to 1.0 (since
   * it probably didn't end there)
   */
  if (!this->GetAbortGenerateData())
  {
    this->UpdateProgress(1.0);
  }
  else
  {
    itk::ProcessAborted e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Image writing has been aborted");
    throw e;
  }

  // Notify end event observers
  this->InvokeEvent(itk::EndEvent());

  if (m_IsObserving)
  {
    m_IsObserving = false;
    source->RemoveObserver(m_ObserverID);
  }

  // Write the image keyword list if any
  // ossimKeywordlist geom_kwl;
  // ImageKeywordlist otb_kwl;

  // itk::MetaDataDictionary dict = this->GetInput()->GetMetaDataDictionary();
  // itk::ExposeMetaData<ImageKeywordlist>(dict, MetaDataKey::OSSIMKeywordlistKey, otb_kwl);
  // otb_kwl.convertToOSSIMKeywordlist(geom_kwl);
  // FIXME: why nothing is done with otb_kwl in that case???
  // If required, put a call to WriteGeometry() here

  /**
   * Release any inputs if marked for release
   */
  this->ReleaseInputs();

  // Reset global shift on input region (box parameter)
  // It allows calling multiple update over the writer
  m_ShiftOutputIndex = inputPtr->GetLargestPossibleRegion().GetIndex();
}


/**
 *
 */
template <class TInputImage>
void ImageFileWriter<TInputImage>::GenerateData(void)
{
  const InputImageType* input = this->GetInput();
  InputImagePointer     cacheImage;

  // Make sure that the image is the right type and no more than
  // four components.
  typedef typename InputImageType::PixelType ImagePixelType;

  if (strcmp(input->GetNameOfClass(), "VectorImage") == 0)
  {
    typedef typename InputImageType::InternalPixelType VectorImagePixelType;
    m_ImageIO->SetPixelTypeInfo(typeid(VectorImagePixelType));

    typedef typename InputImageType::AccessorFunctorType AccessorFunctorType;
    m_ImageIO->SetNumberOfComponents(AccessorFunctorType::GetVectorLength(input));

    m_IOComponents = m_ImageIO->GetNumberOfComponents();
    m_BandList.clear();
    if (m_FilenameHelper->BandRangeIsSet())
    {
      // get band range
      bool retBandRange = m_FilenameHelper->ResolveBandRange(m_FilenameHelper->GetBandRange(), m_IOComponents, m_BandList);
      if (retBandRange == false || m_BandList.empty())
      {
        // invalid range
        itkGenericExceptionMacro("The given band range is either empty or invalid for a " << m_IOComponents << " bands input image!");
      }
    }
  }
  else
  {
    // Set the pixel and component type; the number of components.
    m_ImageIO->SetPixelTypeInfo(typeid(ImagePixelType));
  }

  // Setup the image IO for writing.
  //
  // okay, now extract the data as a raw buffer pointer
  const void* dataPtr = (const void*)input->GetBufferPointer();

  // check that the image's buffered region is the same as
  // ImageIO is expecting and we requested
  InputImageRegionType ioRegion;

  // No shift of the ioRegion from the buffered region is expected
  itk::ImageIORegionAdaptor<TInputImage::ImageDimension>::Convert(m_ImageIO->GetIORegion(), ioRegion, m_ShiftOutputIndex);
  InputImageRegionType bufferedRegion = input->GetBufferedRegion();

  // before this test, bad stuff would happened when they don't match.
  // In case of the buffer has not enough components, adapt the region.
  if ((bufferedRegion != ioRegion) || (m_FilenameHelper->BandRangeIsSet() && (m_IOComponents < m_BandList.size())))
  {
    if (m_NumberOfDivisions > 1 || m_UserSpecifiedIORegion)
    {
      cacheImage = InputImageType::New();
      cacheImage->CopyInformation(input);

      // set number of components at the band range size
      if (m_FilenameHelper->BandRangeIsSet() && (m_IOComponents < m_BandList.size()))
      {
        cacheImage->SetNumberOfComponentsPerPixel(m_BandList.size());
      }

      cacheImage->SetBufferedRegion(ioRegion);
      cacheImage->Allocate();

      // set number of components at the initial size
      if (m_FilenameHelper->BandRangeIsSet() && (m_IOComponents < m_BandList.size()))
      {
        cacheImage->SetNumberOfComponentsPerPixel(m_IOComponents);
      }

      typedef itk::ImageRegionConstIterator<TInputImage> ConstIteratorType;
      typedef itk::ImageRegionIterator<TInputImage>      IteratorType;

      ConstIteratorType in(input, ioRegion);
      IteratorType      out(cacheImage, ioRegion);

      // copy the data into a buffer to match the ioregion
      for (in.GoToBegin(), out.GoToBegin(); !in.IsAtEnd(); ++in, ++out)
      {
        out.Set(in.Get());
      }

      dataPtr = (const void*)cacheImage->GetBufferPointer();
    }
    else
    {
      itk::ImageFileWriterException e(__FILE__, __LINE__);
      std::ostringstream            msg;
      msg << "Did not get requested region!" << std::endl;
      msg << "Requested:" << std::endl;
      msg << ioRegion;
      msg << "Actual:" << std::endl;
      msg << bufferedRegion;
      e.SetDescription(msg.str());
      e.SetLocation(ITK_LOCATION);
      throw e;
    }
  }

  if (m_FilenameHelper->BandRangeIsSet() && (!m_BandList.empty()))
  {
    // Adapt the image size with the region and take into account a potential
    // remapping of the components. m_BandList is empty if no band range is set
    m_ImageIO->DoMapBuffer(const_cast<void*>(dataPtr), bufferedRegion.GetNumberOfPixels(), this->m_BandList);
    m_ImageIO->SetNumberOfComponents(m_BandList.size());
  }

  m_ImageIO->Write(dataPtr);

  if (m_WriteGeomFile || m_FilenameHelper->GetWriteGEOMFile())
  {
    ImageKeywordlist        otb_kwl;
    itk::MetaDataDictionary dict = this->GetInput()->GetMetaDataDictionary();
    itk::ExposeMetaData<ImageKeywordlist>(dict, MetaDataKey::OSSIMKeywordlistKey, otb_kwl);
    WriteGeometry(otb_kwl, this->GetFileName());
  }
}

template <class TInputImage>
void ImageFileWriter<TInputImage>::SetFileName(const std::string& extendedFileName)
{
  this->m_FilenameHelper->SetExtendedFileName(extendedFileName);
  m_FileName = this->m_FilenameHelper->GetSimpleFileName();
  m_ImageIO  = nullptr;
  this->Modified();
}

template <class TInputImage>
const char* ImageFileWriter<TInputImage>::GetFileName() const
{
  return this->m_FilenameHelper->GetSimpleFileName();
}

template <class TInputImage>
const bool& ImageFileWriter<TInputImage>::GetAbortGenerateData() const
{
  m_Lock.Lock();
  // protected read here
  bool ret = Superclass::GetAbortGenerateData();
  m_Lock.Unlock();
  if (ret)
    return otb::Utils::TrueConstant;
  return otb::Utils::FalseConstant;
}

template <class TInputImage>
void ImageFileWriter<TInputImage>::SetAbortGenerateData(bool val)
{
  m_Lock.Lock();
  Superclass::SetAbortGenerateData(val);
  m_Lock.Unlock();
}

} // end namespace otb

#endif
