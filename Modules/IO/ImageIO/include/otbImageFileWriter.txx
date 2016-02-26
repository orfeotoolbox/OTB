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
#ifndef __otbImageFileWriter_txx
#define __otbImageFileWriter_txx

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

namespace otb
{

/**
 *
 */
template <class TInputImage>
ImageFileWriter<TInputImage>
::ImageFileWriter()
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
    m_ObserverID(0)
{
  //Init output index shift
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
ImageFileWriter<TInputImage>
::~ImageFileWriter()
{
}

template <class TInputImage>
void
ImageFileWriter<TInputImage>
::SetNumberOfDivisionsStrippedStreaming(unsigned int nbDivisions)
{
  typedef NumberOfDivisionsStrippedStreamingManager<TInputImage> NumberOfDivisionsStrippedStreamingManagerType;
  typename NumberOfDivisionsStrippedStreamingManagerType::Pointer streamingManager = NumberOfDivisionsStrippedStreamingManagerType::New();
  streamingManager->SetNumberOfDivisions(nbDivisions);

  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void
ImageFileWriter<TInputImage>
::SetNumberOfDivisionsTiledStreaming(unsigned int nbDivisions)
{
  typedef NumberOfDivisionsTiledStreamingManager<TInputImage> NumberOfDivisionsTiledStreamingManagerType;
  typename NumberOfDivisionsTiledStreamingManagerType::Pointer streamingManager = NumberOfDivisionsTiledStreamingManagerType::New();
  streamingManager->SetNumberOfDivisions(nbDivisions);

  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void
ImageFileWriter<TInputImage>
::SetNumberOfLinesStrippedStreaming(unsigned int nbLinesPerStrip)
{
  typedef NumberOfLinesStrippedStreamingManager<TInputImage> NumberOfLinesStrippedStreamingManagerType;
  typename NumberOfLinesStrippedStreamingManagerType::Pointer streamingManager = NumberOfLinesStrippedStreamingManagerType::New();
  streamingManager->SetNumberOfLinesPerStrip(nbLinesPerStrip);

  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void
ImageFileWriter<TInputImage>
::SetAutomaticStrippedStreaming(unsigned int availableRAM, double bias)
{
  typedef RAMDrivenStrippedStreamingManager<TInputImage> RAMDrivenStrippedStreamingManagerType;
  typename RAMDrivenStrippedStreamingManagerType::Pointer streamingManager = RAMDrivenStrippedStreamingManagerType::New();
  streamingManager->SetAvailableRAMInMB(availableRAM);
  streamingManager->SetBias(bias);

  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void
ImageFileWriter<TInputImage>
::SetTileDimensionTiledStreaming(unsigned int tileDimension)
{
  typedef TileDimensionTiledStreamingManager<TInputImage> TileDimensionTiledStreamingManagerType;
  typename TileDimensionTiledStreamingManagerType::Pointer streamingManager = TileDimensionTiledStreamingManagerType::New();
  streamingManager->SetTileDimension(tileDimension);

  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void
ImageFileWriter<TInputImage>
::SetAutomaticTiledStreaming(unsigned int availableRAM, double bias)
{
  typedef RAMDrivenTiledStreamingManager<TInputImage> RAMDrivenTiledStreamingManagerType;
  typename RAMDrivenTiledStreamingManagerType::Pointer streamingManager = RAMDrivenTiledStreamingManagerType::New();
  streamingManager->SetAvailableRAMInMB(availableRAM);
  streamingManager->SetBias(bias);
  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void
ImageFileWriter<TInputImage>
::SetAutomaticAdaptativeStreaming(unsigned int availableRAM, double bias)
{
  typedef RAMDrivenAdaptativeStreamingManager<TInputImage> RAMDrivenAdaptativeStreamingManagerType;
  typename RAMDrivenAdaptativeStreamingManagerType::Pointer streamingManager = RAMDrivenAdaptativeStreamingManagerType::New();
  streamingManager->SetAvailableRAMInMB(availableRAM);
  streamingManager->SetBias(bias);
  m_StreamingManager = streamingManager;
}

#ifndef ITK_LEGACY_REMOVE

#endif // ITK_LEGACY_REMOVE
/**
 *
 */
template <class TInputImage>
void
ImageFileWriter<TInputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "File Name: "
     << (m_FileName.data() ? m_FileName.data() : "(none)") << std::endl;

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
template<class TInputImage>
void
ImageFileWriter<TInputImage>
::SetIORegion(const itk::ImageIORegion& region)
{
  itkDebugMacro("setting IORegion to " << region);
  if (m_IORegion != region)
    {
    m_IORegion = region;
    this->Modified();
    m_UserSpecifiedIORegion = true;
    }
}

template<class TInputImage>
void
ImageFileWriter<TInputImage>
::SetInput(const InputImageType* input)
{
  this->ProcessObject::SetNthInput(0,const_cast<InputImageType*>(input));
}

template<class TInputImage>
const TInputImage*
ImageFileWriter<TInputImage>
::GetInput()
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const InputImageType*>(this->ProcessObject::GetInput(0));
}

/**
 * Update method : update output information of input and write to file
 */
template<class TInputImage>
void
ImageFileWriter<TInputImage>
::Update()
{
  // Update output information on input image
  InputImagePointer inputPtr =
    const_cast<InputImageType *>(this->GetInput());

  // Make sure input is available
  if ( inputPtr.IsNull() )
    {
    itkExceptionMacro(<< "No input to writer");
    }

  /** Parse streaming modes */
  if(m_FilenameHelper->StreamingTypeIsSet())
    {
    itkWarningMacro(<<"Streaming configuration through extended filename is used. Any previous streaming configuration (ram value, streaming mode ...) will be ignored.");

    std::string type = m_FilenameHelper->GetStreamingType();

    std::string sizemode = "auto";

    if(m_FilenameHelper->StreamingSizeModeIsSet())
      {
      sizemode = m_FilenameHelper->GetStreamingSizeMode();
      }

    double sizevalue = 0.;

    if(m_FilenameHelper->StreamingSizeValueIsSet())
      {
      sizevalue = m_FilenameHelper->GetStreamingSizeValue();
      }

    if(type == "auto")
      {
      if(sizemode != "auto")
        {
        itkWarningMacro(<<"In auto streaming type, the sizemode option will be ignored.");
        }
      if(sizevalue == 0.)
        {
        itkWarningMacro("sizemode is auto but sizevalue is 0. Value will be fetched from the OTB_MAX_RAM_HINT environment variable if set, or else use the default value");
        }
      this->SetAutomaticAdaptativeStreaming(sizevalue);
      }
    else if(type == "tiled")
      {
      if(sizemode == "auto")
        {
        if(sizevalue == 0.)
          {
          itkWarningMacro("sizemode is auto but sizevalue is 0. Value will be fetched from the OTB_MAX_RAM_HINT environment variable if set, or else use the default value");
          }
        this->SetAutomaticTiledStreaming(sizevalue);
        }
      else if(sizemode == "nbsplits")
        {
        if(sizevalue == 0.)
          {
          itkWarningMacro("Streaming sizemode is set to nbsplits but sizevalue is 0. This will result in upredicted behaviour. Please consider setting the sizevalue by using &streaming:sizevalue=x.");
          }
        this->SetNumberOfDivisionsTiledStreaming(static_cast<unsigned int>(sizevalue));
        }
      else if(sizemode == "height")
        {
        if(sizevalue == 0.)
          {
          itkWarningMacro("Streaming sizemode is set to height but sizevalue is 0. This will result in upredicted behaviour. Please consider setting the sizevalue by using &streaming:sizevalue=x.");
          }

        this->SetTileDimensionTiledStreaming(static_cast<unsigned int>(sizevalue));
        }
      }
    else if(type == "stripped")
      {
      if(sizemode == "auto")
        {
        if(sizevalue == 0.)
          {
          itkWarningMacro("sizemode is auto but sizevalue is 0. Value will be fetched from configuration file if any, or from cmake configuration otherwise.");
          }

        this->SetAutomaticStrippedStreaming(sizevalue);
        }
      else if(sizemode == "nbsplits")
        {
        if(sizevalue == 0.)
          {
          itkWarningMacro("Streaming sizemode is set to nbsplits but sizevalue is 0. This will result in upredicted behaviour. Please consider setting the sizevalue by using &streaming:sizevalue=x.");
          }
        this->SetNumberOfDivisionsStrippedStreaming(static_cast<unsigned int>(sizevalue));
        }
      else if(sizemode == "height")
        {
        if(sizevalue == 0.)
          {
          itkWarningMacro("Streaming sizemode is set to height but sizevalue is 0. This will result in upredicted behaviour. Please consider setting the sizevalue by using &streaming:sizevalue=x.");
          }
        this->SetNumberOfLinesStrippedStreaming(static_cast<unsigned int>(sizevalue));
        }

      }
    else if (type == "none")
      {
      if(sizemode!="" || sizevalue!=0.)
        {
        itkWarningMacro("Streaming is explicitly disabled, sizemode and sizevalue will be ignored.");
        }
      this->SetNumberOfDivisionsTiledStreaming(0);
      }
    }
  else
    {
    if(m_FilenameHelper->StreamingSizeValueIsSet() || m_FilenameHelper->StreamingSizeModeIsSet())
      {
      itkWarningMacro(<<"No streaming type is set, streaming sizemode and sizevalue will be ignored.");
      }
    }

  this->SetAbortGenerateData(0);
  this->SetProgress(0.0);

  /**
   * Tell all Observers that the filter is starting
   */
  this->InvokeEvent(itk::StartEvent());

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

  if (m_ImageIO.IsNull())   //try creating via factory
    {
    itkDebugMacro(<< "Attempting factory creation of ImageIO for file: "
                  << m_FileName);
    this->SetImageIO(ImageIOFactory::CreateImageIO(m_FileName.c_str(),
                                                   otb::ImageIOFactory::WriteMode));

    m_FactorySpecifiedImageIO = true;
    }
  else
    {
    if (!m_ImageIO->CanWriteFile(m_FileName.c_str()))
      {
      itkDebugMacro(<< "ImageIO exists but doesn't know how to write file:"
                    << m_FileName);

      if (m_FactorySpecifiedImageIO)
        {
        itkDebugMacro(<< "Attempting creation of ImageIO with a factory for file:"
                      << m_FileName);
        m_ImageIO = ImageIOFactory::CreateImageIO(m_FileName.c_str(),
                                                  otb::ImageIOFactory::WriteMode);
        m_FactorySpecifiedImageIO = true;
        }
      }

    }

  if (m_ImageIO.IsNull())
    {
    itk::ImageFileWriterException e(__FILE__, __LINE__);
    std::ostringstream msg;
    msg << " Could not create IO object for file "
        << m_FileName.c_str() << std::endl;
    msg << "  Tried to create one of the following:" << std::endl;
    std::list<itk::LightObject::Pointer> allobjects =
      itk::ObjectFactoryBase::CreateAllInstance("otbImageIOBase");
    for (std::list<itk::LightObject::Pointer>::iterator i = allobjects.begin();
         i != allobjects.end(); ++i)
      {
      otb::ImageIOBase* io = dynamic_cast<otb::ImageIOBase*>(i->GetPointer());
      if(io)
        msg << "    " << io->GetNameOfClass() << std::endl;
      }
    msg << "  You probably failed to set a file suffix, or" << std::endl;
    msg << "    set the suffix to an unsupported type." << std::endl;
    e.SetDescription(msg.str().c_str());
    e.SetLocation(ITK_LOCATION);
    throw e;
    }

  // Manage extended filename
  if ((strcmp(m_ImageIO->GetNameOfClass(), "GDALImageIO") == 0)
      && m_FilenameHelper->gdalCreationOptionsIsSet()   )
    {
    typename GDALImageIO::Pointer imageIO = dynamic_cast<GDALImageIO*>(m_ImageIO.GetPointer());

    if(imageIO.IsNull())
      {
      itk::ImageFileWriterException e(__FILE__, __LINE__);
      std::ostringstream msg;
      msg << " ImageIO is of kind GDALImageIO, but fails to dynamic_cast (this should never happen)."<< std::endl;
      e.SetDescription(msg.str().c_str());
      throw e;
      }

    imageIO->SetOptions(m_FilenameHelper->GetgdalCreationOptions());
    }


  /** End of Prepare ImageIO  : create ImageFactory */

  /**
   * Grab the input
   */
  inputPtr->UpdateOutputInformation();
  InputImageRegionType inputRegion = inputPtr->GetLargestPossibleRegion();

  /** Parse region size modes */
  if(m_FilenameHelper->BoxIsSet())
    {
    typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;

    boost::char_separator<char> sep(":");
    Tokenizer tokens(m_FilenameHelper->GetBox(), sep);

    Tokenizer::iterator it = tokens.begin();
    typename InputImageRegionType::IndexType start;
    typename InputImageRegionType::SizeType  size;

    start[0] = atoi(it->c_str());  // first index on X
    ++it;
    start[1] = atoi(it->c_str());  // first index on Y
    ++it;
    size[0]  = atoi(it->c_str());  // size along X
    ++it;
    size[1]  = atoi(it->c_str());  // size along Y

    inputRegion.SetSize(size);

    m_ShiftOutputIndex = start;
    inputRegion.SetIndex(m_ShiftOutputIndex);

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
    otbMsgDevMacro(<< "inputRegion " << inputRegion);
    }

  /**
   * Determine of number of pieces to divide the input.  This will be the
   * minimum of what the user specified via SetNumberOfDivisionsStrippedStreaming()
   * and what the Splitter thinks is a reasonable value.
   */

  /** Control if the ImageIO is CanStreamWrite */
  if (m_ImageIO->CanStreamWrite() == false)
    {
    otbWarningMacro(
      << "The ImageFactory selected for the image file <" << m_FileName.c_str() <<
      "> does not support streaming.");
    this->SetNumberOfDivisionsStrippedStreaming(1);
    }

  /** Compare the buffered region  with the inputRegion which is the largest
  * possible region or a user defined region through extended filename
  * Not sure that if this modification is needed  */
  else if (inputPtr->GetBufferedRegion() == inputRegion)
    {
    otbMsgDevMacro(<< "Buffered region is the largest possible region, there is no need for streaming.");
    this->SetNumberOfDivisionsStrippedStreaming(1);
    }
  m_StreamingManager->PrepareStreaming(inputPtr, inputRegion);
  m_NumberOfDivisions = m_StreamingManager->GetNumberOfSplits();
  otbMsgDebugMacro(<< "Number Of Stream Divisions : " << m_NumberOfDivisions);

  /**
   * Loop over the number of pieces, execute the upstream pipeline on each
   * piece, and copy the results into the output image.
   */
  InputImageRegionType streamRegion;

  //
  // Setup the ImageIO with information from inputPtr
  //
  m_ImageIO->SetNumberOfDimensions(TInputImage::ImageDimension);
  const typename TInputImage::SpacingType&   spacing = inputPtr->GetSpacing();
  const typename TInputImage::PointType&     origin = inputPtr->GetOrigin();
  const typename TInputImage::DirectionType& direction = inputPtr->GetDirection();

  for (unsigned int i = 0; i < TInputImage::ImageDimension; ++i)
    {
    // Final image size
    m_ImageIO->SetDimensions(i, inputRegion.GetSize(i));
    m_ImageIO->SetSpacing(i, spacing[i]);
    m_ImageIO->SetOrigin(i, origin[i] + static_cast<double>(inputRegion.GetIndex()[i]) * spacing[i]);

    vnl_vector<double> axisDirection(TInputImage::ImageDimension);
    // Please note: direction cosines are stored as columns of the
    // direction matrix
    for (unsigned int j = 0; j < TInputImage::ImageDimension; ++j)
      {
      axisDirection[j] = direction[j][i];
      }
    m_ImageIO->SetDirection(i, axisDirection);
    }

  m_ImageIO->SetUseCompression(m_UseCompression);
  m_ImageIO->SetMetaDataDictionary(inputPtr->GetMetaDataDictionary());

  /** Create Image file */
  // Setup the image IO for writing.
  //
  m_ImageIO->SetFileName(m_FileName.c_str());

  m_ImageIO->WriteImageInformation();

  this->UpdateProgress(0);
  m_CurrentDivision = 0;
  m_DivisionProgress = 0;

  // Get the source process object
  itk::ProcessObject* source = inputPtr->GetSource();
  m_IsObserving = false;
  m_ObserverID = 0;

  // Check if source exists
  if(source)
    {
    typedef itk::MemberCommand<Self>      CommandType;
    typedef typename CommandType::Pointer CommandPointerType;

    CommandPointerType command = CommandType::New();
    command->SetCallbackFunction(this, &Self::ObserveSourceFilterProgress);

    m_ObserverID = source->AddObserver(itk::ProgressEvent(), command);
    m_IsObserving = true;
    }
  else
    {
    itkWarningMacro(<< "Could not get the source process object. Progress report might be buggy");
    }

  for (m_CurrentDivision = 0;
       m_CurrentDivision < m_NumberOfDivisions && !this->GetAbortGenerateData();
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
      ioRegion.SetIndex(i, streamRegion.GetIndex(i));
      //Set the ioRegion index using the shifted index ( (0,0 without box parameter))
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
  //FIXME: why nothing is done with otb_kwl in that case???
  // If required, put a call to WriteGeometry() here

  /**
   * Release any inputs if marked for release
   */
  this->ReleaseInputs();

  //Reset global shift on input region (box parameter)
  //It allows calling multiple update over the writer
  m_ShiftOutputIndex.Fill(0);
}


/**
 *
 */
template<class TInputImage>
void
ImageFileWriter<TInputImage>
::GenerateData(void)
{
  const InputImageType * input = this->GetInput();
  InputImagePointer cacheImage;

  // Make sure that the image is the right type and no more than
  // four components.
  typedef typename InputImageType::PixelType ImagePixelType;

  if (strcmp(input->GetNameOfClass(), "VectorImage") == 0)
    {
    typedef typename InputImageType::InternalPixelType VectorImagePixelType;
    m_ImageIO->SetPixelTypeInfo(typeid(VectorImagePixelType));

    typedef typename InputImageType::AccessorFunctorType AccessorFunctorType;
    m_ImageIO->SetNumberOfComponents(AccessorFunctorType::GetVectorLength(input));
    }
  else
    {
    // Set the pixel and component type; the number of components.
    m_ImageIO->SetPixelTypeInfo(typeid(ImagePixelType));
    }

  // Setup the image IO for writing.
  //
  //okay, now extract the data as a raw buffer pointer
  const void* dataPtr = (const void*) input->GetBufferPointer();

  // check that the image's buffered region is the same as
  // ImageIO is expecting and we requested
  InputImageRegionType ioRegion;

  // No shift of the ioRegion from the buffered region is expected
  typename InputImageRegionType::IndexType tmpIndex;
  tmpIndex.Fill(0);
  itk::ImageIORegionAdaptor<TInputImage::ImageDimension>::
    //Convert(m_ImageIO->GetIORegion(), ioRegion, tmpIndex);
    Convert(m_ImageIO->GetIORegion(), ioRegion, m_ShiftOutputIndex);
  InputImageRegionType bufferedRegion = input->GetBufferedRegion();

  // before this test, bad stuff would happend when they don't match
  if (bufferedRegion != ioRegion)
    {
    if ( m_NumberOfDivisions > 1 || m_UserSpecifiedIORegion)
      {
      itkDebugMacro("Requested stream region does not match generated output");
      itkDebugMacro("input filter may not support streaming well");

      cacheImage = InputImageType::New();
      cacheImage->CopyInformation(input);
      cacheImage->SetBufferedRegion(ioRegion);
      cacheImage->Allocate();

      typedef itk::ImageRegionConstIterator<TInputImage> ConstIteratorType;
      typedef itk::ImageRegionIterator<TInputImage>      IteratorType;

      ConstIteratorType in(input, ioRegion);
      IteratorType out(cacheImage, ioRegion);

      // copy the data into a buffer to match the ioregion
      for (in.GoToBegin(), out.GoToBegin(); !in.IsAtEnd(); ++in, ++out)
        {
        out.Set(in.Get());
        }

      dataPtr = (const void*) cacheImage->GetBufferPointer();

      }
    else
      {
      itk::ImageFileWriterException e(__FILE__, __LINE__);
      std::ostringstream msg;
      msg << "Did not get requested region!" << std::endl;
      msg << "Requested:" << std::endl;
      msg << ioRegion;
      msg << "Actual:" << std::endl;
      msg << bufferedRegion;
      e.SetDescription(msg.str().c_str());
      e.SetLocation(ITK_LOCATION);
      throw e;
      }
    }

  m_ImageIO->Write(dataPtr);

  if (m_WriteGeomFile  || m_FilenameHelper->GetWriteGEOMFile())
    {
    ImageKeywordlist otb_kwl;
    itk::MetaDataDictionary dict = this->GetInput()->GetMetaDataDictionary();
    itk::ExposeMetaData<ImageKeywordlist>(dict, MetaDataKey::OSSIMKeywordlistKey, otb_kwl);
    WriteGeometry(otb_kwl, this->GetFileName());
    }
}

template <class TInputImage>
void
ImageFileWriter<TInputImage>
::SetFileName(std::string extendedFileName)
{
  this->SetFileName(extendedFileName.c_str());
}

template <class TInputImage>
void
ImageFileWriter<TInputImage>
::SetFileName(const char* extendedFileName)
{
  this->m_FilenameHelper->SetExtendedFileName(extendedFileName);
  m_FileName = this->m_FilenameHelper->GetSimpleFileName();
  m_ImageIO = NULL;
  this->Modified();
}

template <class TInputImage>
const char*
ImageFileWriter<TInputImage>
::GetFileName () const
{
return this->m_FilenameHelper->GetSimpleFileName();
}

} // end namespace otb

#endif
