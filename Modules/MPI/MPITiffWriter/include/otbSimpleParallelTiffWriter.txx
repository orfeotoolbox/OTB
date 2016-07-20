/*=========================================================================
  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  TODO: Add copyright notice for Remi Cresson (IRSTEA)

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otbSimpleParallelTiffWriter_txx
#define otbSimpleParallelTiffWriter_txx

#include "otbSimpleParallelTiffWriter.h"
#include "itkTimeProbe.h"

using std::vector;

using sptw::PTIFF;
using sptw::open_raster;
using sptw::create_generic_raster;
using sptw::SPTW_ERROR;

namespace otb
{

/**
 *
 */
template <class TInputImage>
SimpleParallelTiffWriter<TInputImage>
::SimpleParallelTiffWriter()
 : m_NumberOfDivisions(0),
   m_CurrentDivision(0),
   m_DivisionProgress(0.0),
   m_UserSpecifiedImageIO(true),
   m_UserSpecifiedIORegion(false),
   m_FactorySpecifiedImageIO(false),
   m_UseInputMetaDataDictionary(false),
   m_WriteGeomFile(false),
   m_FilenameHelper(),
   m_IsObserving(true),
   m_ObserverID(0)
   {
  //Init output index shift
  m_ShiftOutputIndex.Fill(0);

  // Default TIFF tile size, can be modified later
  m_TiffTileSize = 1024;

  // Strip blocks
  m_TiffTiledMode = false;

  // Verbose
  m_Verbose = false;

  // Virtual mode
  m_VirtualMode = false;

  // By default, we use striped streaming, with automatic region size
  // We don't set any parameter, so the memory size is retrieved from the OTB configuration options
  //this->SetAutomaticAdaptativeStreaming();
  this->SetAutomaticStrippedStreaming();

  m_FilenameHelper = FNameHelperType::New();
   }

/**
 *
 */
template <class TInputImage>
SimpleParallelTiffWriter<TInputImage>
::~SimpleParallelTiffWriter()
{
}

template <class TInputImage>
unsigned int
SimpleParallelTiffWriter<TInputImage>
::GetProcFromDivision(unsigned int regionIndex)
 {
  if (otb::MPIConfig::Instance()->GetNbProcs()==0)
    return 0;
  return ( regionIndex % otb::MPIConfig::Instance()->GetNbProcs());
 }

/*
 * Arranges the splitting layout to match the number of MPI processes
 */
template <class TInputImage>
unsigned int
SimpleParallelTiffWriter<TInputImage>
::OptimizeStrippedSplittingLayout(unsigned int n)
 {
  unsigned int m = static_cast<unsigned int >(otb::MPIConfig::Instance()->GetNbProcs());
  if (n > m)
    {
    float div = static_cast<float>(n) / static_cast<float>(otb::MPIConfig::Instance()->GetNbProcs());
    m *= static_cast<unsigned int>(div);
    }
  itkDebugMacro( "Changing number of split from " << n << " to " << m );
  return m;

 }

template <class TInputImage>
void
SimpleParallelTiffWriter<TInputImage>
::SetNumberOfDivisionsStrippedStreaming(unsigned int nbDivisions)
 {
  typedef otb::NumberOfDivisionsStrippedStreamingManager<TInputImage> NumberOfDivisionsStrippedStreamingManagerType;
  typename NumberOfDivisionsStrippedStreamingManagerType::Pointer streamingManager = NumberOfDivisionsStrippedStreamingManagerType::New();
  streamingManager->SetNumberOfDivisions(nbDivisions);

  m_StreamingManager = streamingManager;
 }

template <class TInputImage>
void
SimpleParallelTiffWriter<TInputImage>
::SetNumberOfDivisionsTiledStreaming(unsigned int nbDivisions)
 {
  typedef otb::NumberOfDivisionsTiledStreamingManager<TInputImage> NumberOfDivisionsTiledStreamingManagerType;
  typename NumberOfDivisionsTiledStreamingManagerType::Pointer streamingManager = NumberOfDivisionsTiledStreamingManagerType::New();
  streamingManager->SetNumberOfDivisions(nbDivisions);

  m_StreamingManager = streamingManager;
 }

template <class TInputImage>
void
SimpleParallelTiffWriter<TInputImage>
::SetNumberOfLinesStrippedStreaming(unsigned int nbLinesPerStrip)
 {
  typedef otb::NumberOfLinesStrippedStreamingManager<TInputImage> NumberOfLinesStrippedStreamingManagerType;
  typename NumberOfLinesStrippedStreamingManagerType::Pointer streamingManager = NumberOfLinesStrippedStreamingManagerType::New();
  streamingManager->SetNumberOfLinesPerStrip(nbLinesPerStrip);

  m_StreamingManager = streamingManager;
 }

template <class TInputImage>
void
SimpleParallelTiffWriter<TInputImage>
::SetAutomaticStrippedStreaming(unsigned int availableRAM, double bias)
 {
  typedef otb::RAMDrivenStrippedStreamingManager<TInputImage> RAMDrivenStrippedStreamingManagerType;
  typename RAMDrivenStrippedStreamingManagerType::Pointer streamingManager = RAMDrivenStrippedStreamingManagerType::New();
  streamingManager->SetAvailableRAMInMB(availableRAM);
  streamingManager->SetBias(bias);

  m_StreamingManager = streamingManager;
 }

template <class TInputImage>
void
SimpleParallelTiffWriter<TInputImage>
::SetTileDimensionTiledStreaming(unsigned int tileDimension)
 {
  typedef otb::TileDimensionTiledStreamingManager<TInputImage> TileDimensionTiledStreamingManagerType;
  typename TileDimensionTiledStreamingManagerType::Pointer streamingManager = TileDimensionTiledStreamingManagerType::New();
  streamingManager->SetTileDimension(tileDimension);

  m_StreamingManager = streamingManager;
 }

template <class TInputImage>
void
SimpleParallelTiffWriter<TInputImage>
::SetAutomaticTiledStreaming(unsigned int availableRAM, double bias)
 {
  typedef otb::RAMDrivenTiledStreamingManager<TInputImage> RAMDrivenTiledStreamingManagerType;
  typename RAMDrivenTiledStreamingManagerType::Pointer streamingManager = RAMDrivenTiledStreamingManagerType::New();
  streamingManager->SetAvailableRAMInMB(availableRAM);
  streamingManager->SetBias(bias);

  m_StreamingManager = streamingManager;
 }

template <class TInputImage>
void
SimpleParallelTiffWriter<TInputImage>
::SetAutomaticAdaptativeStreaming(unsigned int availableRAM, double bias)
 {
  typedef otb::RAMDrivenAdaptativeStreamingManager<TInputImage> RAMDrivenAdaptativeStreamingManagerType;
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
SimpleParallelTiffWriter<TInputImage>
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
SimpleParallelTiffWriter<TInputImage>
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
SimpleParallelTiffWriter<TInputImage>
::SetInput(const InputImageType* input)
 {
  this->ProcessObject::SetNthInput(0,const_cast<InputImageType*>(input));
 }

template<class TInputImage>
const TInputImage*
SimpleParallelTiffWriter<TInputImage>
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
SimpleParallelTiffWriter<TInputImage>
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
        itkWarningMacro("sizemode is auto but sizevalue is 0. Value will be fetched from configuration file if any, or from cmake configuration otherwise.");
        }
      this->SetAutomaticAdaptativeStreaming(sizevalue);
      }
    else if(type == "tiled")
      {
      if(sizemode == "auto")
        {
        if(sizevalue == 0.)
          {
          itkWarningMacro("sizemode is auto but sizevalue is 0. Value will be fetched from configuration file if any, or from cmake configuration otherwise.");
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

  /*
   *  Check filename
   */
  if (m_VirtualMode)
    {
    m_FileName = "virtualFile.tif";
    }
  if (m_FileName == "")
    {
    // Make sure that filename is not empty
    itkExceptionMacro(<< "No filename was specified");
    otb::MPIConfig::Instance()->abort(EXIT_FAILURE);
    }
  std::string::size_type idx;
  idx = m_FileName.rfind('.');
  if(idx != std::string::npos)
    {
    std::string extension = m_FileName.substr(idx+1);
    boost::algorithm::to_lower(extension);
    if (boost::iequals(extension, "tif"))
      {
      // Extension is TIF
      }
    else
      {
      // Other extension
      itkExceptionMacro(<<"Filename must have .tif extension !");
      otb::MPIConfig::Instance()->abort(EXIT_FAILURE);
      }
    }
  else
    {
    // Missing extension
    itkWarningMacro(<<"Filename has no extension. Adding <.tif> extension.");
    m_FileName += ".tif";
    }

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

  // Get number of bands & pixel data type
  GDALDataType dataType;
  int nBands = 1;
  typedef typename InputImageType::PixelType ImagePixelType;
  if (strcmp(this->GetInput()->GetNameOfClass(), "VectorImage") == 0)
    {
    // Input is a vector image
    typedef typename InputImageType::InternalPixelType VectorImagePixelType;
    typedef typename InputImageType::AccessorFunctorType AccessorFunctorType;
    dataType = otb::GdalDataTypeBridge::GetGDALDataType<VectorImagePixelType>();
    nBands *= AccessorFunctorType::GetVectorLength(this->GetInput());
    }
  else
    {
    // Input is an image
    dataType = otb::GdalDataTypeBridge::GetGDALDataType<ImagePixelType>();
    }

  /************************************************************************
   *                         Raster creation
   ************************************************************************/

  // First, compute the block size
  int block_size_x = m_TiffTiledMode;

  if (m_TiffTiledMode)
    {
    // When mode is tiled, check the GeoTiff tile size
    int inputRegionMinSize = std::min(inputRegion.GetSize()[0],	inputRegion.GetSize()[1]);
    if (m_TiffTileSize > inputRegionMinSize)
      {
      // Find the nearest (floor) power of 2
      block_size_x = (int) std::pow(2, std::floor(std::log((double) inputRegionMinSize)/std::log(2.0)));
      itkWarningMacro(<<"GeoTiff tile size is bigger than image. Setting to " << block_size_x);
      }
    }
  else
    {
    // When mode is not tiled (i.e. striped)
    block_size_x = inputPtr->GetLargestPossibleRegion().GetSize()[0];
    }

  // Master process (Rank 0) is responsible for the creation of the output raster.
  if (otb::MPIConfig::Instance()->GetMyRank() == 0 && !m_VirtualMode)
    {
    // Set geotransform
    double geotransform[6];
    geotransform[0] = inputPtr->GetOrigin()[0] - 0.5*inputPtr->GetSpacing()[0];
    geotransform[1] = inputPtr->GetSpacing()[0];
    geotransform[2] = 0.0;
    geotransform[3] = inputPtr->GetOrigin()[1] - 0.5*inputPtr->GetSpacing()[1];
    geotransform[4] = 0.0;
    geotransform[5] = inputPtr->GetSpacing()[1];

    // Call SPTW routine that creates the output raster
    SPTW_ERROR sperr = create_generic_raster(m_FileName,
        inputPtr->GetLargestPossibleRegion().GetSize()[0],
        inputPtr->GetLargestPossibleRegion().GetSize()[1],
        nBands,
        dataType,
        geotransform,
        inputPtr->GetProjectionRef(),
        block_size_x,
        m_TiffTiledMode);

    if (sperr != sptw::SP_None)
      {
      itkExceptionMacro(<<"Error creating raster");
      otb::MPIConfig::Instance()->abort(EXIT_FAILURE);
      }
    }

  // Wait for rank 0 to finish creating the output raster
  otb::MPIConfig::Instance()->barrier();

  /************************************************************************
   *                         Raster update: open raster
   ************************************************************************/

  // Open raster (if not in virtual mode)
  PTIFF* output_raster = NULL;
  if (!m_VirtualMode)
    {
    output_raster = open_raster(m_FileName);

    // First, populate blocks offsets
    if (otb::MPIConfig::Instance()->GetMyRank() == 0)
      {
      SPTW_ERROR sperr = populate_tile_offsets(output_raster,
          block_size_x,
          m_TiffTiledMode);
      if (sperr != sptw::SP_None)
        {
        itkExceptionMacro(<<"Error populating tile offsets\n");
        otb::MPIConfig::Instance()->abort(EXIT_FAILURE);
        }
      }

    // Wait for tile offsets to be populated
    otb::MPIConfig::Instance()->barrier();
    close_raster(output_raster);

    // Now open it
    output_raster = open_raster(m_FileName);
    otb::MPIConfig::Instance()->barrier();

    if (output_raster == NULL)
      {
      itkExceptionMacro(<<"Could not open output raster");
      otb::MPIConfig::Instance()->abort(EXIT_FAILURE);
      }
    }

  /************************************************************************
   *                         Raster update: write regions
   ************************************************************************/

  // Time probe for overall process time
  itk::TimeProbe overallTime;
  overallTime.Start();

  // Check that streaming is relevant
  m_StreamingManager->PrepareStreaming(inputPtr, inputRegion);
  m_NumberOfDivisions = m_StreamingManager->GetNumberOfSplits();

  // Recompute a new splitting layout which fits better the MPI number of processes
  // TODO make it work on tiled splits !
  // [dirtycode]
  unsigned int newNumberOfStrippedSplits = OptimizeStrippedSplittingLayout(m_NumberOfDivisions);
  this->SetNumberOfDivisionsStrippedStreaming(newNumberOfStrippedSplits);
  m_StreamingManager->PrepareStreaming(inputPtr, inputRegion);
  m_NumberOfDivisions = m_StreamingManager->GetNumberOfSplits();
  // [/dirtycode]

  // Configure process objects
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

  // Loop on streaming tiles
  double processDuration(0), writeDuration(0), numberOfProcessedRegions(0);
  InputImageRegionType streamRegion;
  for (m_CurrentDivision = 0;
      m_CurrentDivision < m_NumberOfDivisions && !this->GetAbortGenerateData();
      m_CurrentDivision++, m_DivisionProgress = 0, this->UpdateFilterProgress())
    {
    streamRegion = m_StreamingManager->GetSplit(m_CurrentDivision);

    if (GetProcFromDivision(m_CurrentDivision) == otb::MPIConfig::Instance()->GetMyRank())
      {
      /*
       * Processing
       */
      itk::TimeProbe processingTime;
      processingTime.Start();
      inputPtr->SetRequestedRegion(streamRegion);
      inputPtr->PropagateRequestedRegion();
      inputPtr->UpdateOutputData();
      processingTime.Stop();
      processDuration += processingTime.GetTotal();

      /*
       * Writing using SPTW
       */
      itk::TimeProbe writingTime;
      writingTime.Start();
      if (!m_VirtualMode)
        {
        sptw::write_area(output_raster,
            inputPtr->GetBufferPointer(),
            streamRegion.GetIndex()[0],
            streamRegion.GetIndex()[1],
            streamRegion.GetIndex()[0] + streamRegion.GetSize()[0] -1,
            streamRegion.GetIndex()[1] + streamRegion.GetSize()[1] -1);
        }
      writingTime.Stop();
      writeDuration += writingTime.GetTotal();
      numberOfProcessedRegions += 1;
      }
    }

  // Clean up
  close_raster(output_raster);
  output_raster = NULL;

  // We wait for other process
  otb::MPIConfig::Instance()->barrier();
  overallTime.Stop();

  /* TODO: Use MPIConfig helper instead of MPI methods
	// Get timings
	const int nValues = 3;
	double runtimes[nValues] = {processDuration, writeDuration, numberOfProcessedRegions};
	std::vector<double> process_runtimes(otb::MPIConfig::Instance()->GetNbProcs()*nValues);
	MPI_Gather(runtimes,
			nValues,
			MPI_DOUBLE,
			&(process_runtimes[0]),
			nValues,
			MPI_DOUBLE,
			0,
			MPI_COMM_WORLD);

	// Display timings
	if (otb::MPIConfig::Instance()->GetMyRank() == 0 && m_Verbose)
	  {
      itkDebugMacro( "Runtime, in seconds" );
      itkDebugMacro( "Process Id\tProcessing\tWriting" );
	  for (unsigned int i = 0; i < process_runtimes.size(); i+=nValues)
	    {
      itkDebugMacro( <<(int (i/nValues)) << 
	        "\t" << process_runtimes[i] <<
	        "\t" << process_runtimes[i+1] <<
	        "\t("<< process_runtimes[i+2] << " regions)" );
	    }
	  itkDebugMacro( "Overall time:" << overallTime.GetTotal() );
	  }
   */

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

  /*
   * Writting the geom (only master process)
   */
  if ( otb::MPIConfig::Instance()->GetMyRank() == 0 &&
      (m_WriteGeomFile || m_FilenameHelper->GetWriteGEOMFile()) )
    {
    otb::ImageKeywordlist otb_kwl;
    itk::MetaDataDictionary dict = this->GetInput()->GetMetaDataDictionary();
    itk::ExposeMetaData<otb::ImageKeywordlist>(dict, otb::MetaDataKey::OSSIMKeywordlistKey, otb_kwl);
    WriteGeometry(otb_kwl, this->GetFileName());
    }

  /**
   * Release any inputs if marked for release
   */
  this->ReleaseInputs();

  //Reset global shift on input region (box parameter)
  //It allows to call multiple update over the writer
  m_ShiftOutputIndex.Fill(0);

  // Wait for other processes
  otb::MPIConfig::Instance()->barrier();
 }

template <class TInputImage>
void
SimpleParallelTiffWriter<TInputImage>
::SetFileName(std::string extendedFileName)
 {
  this->SetFileName(extendedFileName.c_str());
 }

template <class TInputImage>
void
SimpleParallelTiffWriter<TInputImage>
::SetFileName(const char* extendedFileName)
 {
  this->m_FilenameHelper->SetExtendedFileName(extendedFileName);
  m_FileName = this->m_FilenameHelper->GetSimpleFileName();
  m_ImageIO = NULL;
  this->Modified();
 }

template <class TInputImage>
const char*
SimpleParallelTiffWriter<TInputImage>
::GetFileName () const
 {
  return this->m_FilenameHelper->GetSimpleFileName();
 }

}
#endif
