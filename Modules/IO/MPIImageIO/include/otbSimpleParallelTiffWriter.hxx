#ifndef __SimpleParallelTiffWriter_hxx
#define __SimpleParallelTiffWriter_hxx

#include "otbSimpleParallelTiffWriter.h"

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

#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

// GDAL
#include <gdal.h>
#include <ogr_spatialref.h>

// SPTW
#include <algorithm>
#include <vector>
#include <SPTW/sptw.h>

// GDAL-OTB datatype brigde
#include "otbGdalDataTypeBridge.h"

using std::vector;

using sptw::PTIFF;
using sptw::open_raster;
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
   m_UseCompression(false),
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
	m_TiffTiledMode = false;
	m_Verbose = true;
	m_VirtualMode = false;

	// By default, we use tiled streaming, with automatic tile size
	// We don't set any parameter, so the memory size is retrieved from the OTB configuration options
	this->SetAutomaticAdaptativeStreaming();

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
	}
	std::string::size_type idx;
	idx = m_FileName.rfind('.');
	if(idx != std::string::npos)
	{
		std::string extension = m_FileName.substr(idx+1);
		if (boost::iequals(extension, "tif"))
		{
			// Extension is TIF
		}
		else
		{
			// Bad extension
			itkExceptionMacro(<<"Filename must have .tif extension !");
			MPI_Abort(MPI_COMM_WORLD, 1);
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
	 *  	Raster creation with GDAL and SPTW routines
	 ************************************************************************/

	GDALAllRegister();

	// TODO: find a good tile size, wich give a better cluster load

	// Check that the geotiff tile size is not too big
	int inputRegionMinSize = std::min(inputRegion.GetSize()[0],	inputRegion.GetSize()[1]);
	if (m_TiffTileSize > inputRegionMinSize)
	{
		// Find the nearest (floor) power of 2
		m_TiffTileSize = (int) std::pow(2, std::floor(std::log((double) inputRegionMinSize)/std::log(2.0)));
		itkWarningMacro(<<"GeoTiff tile size is bigger than image. Setting to " << m_TiffTileSize);
	}

	// If we are the process with rank 0 we are responsible for the creation of the output raster.
	if (m_MyRank == 0 && !m_VirtualMode) {

		// Get GeoTiff driver
		GDALAllRegister();
		GDALDriver *driver = GetGDALDriverManager()->GetDriverByName("GTiff");
		if (driver == NULL) {
			fprintf(stderr, "Error opening GTiff driver.\n");
			MPI_Abort(MPI_COMM_WORLD, 1);
		}

		// Set driver options
		char **options = NULL;
		options = CSLSetNameValue(options, "INTERLEAVE", "PIXEL");
		options = CSLSetNameValue(options, "BIGTIFF", "YES");
		options = CSLSetNameValue(options, "COMPRESS", "NONE");
		options = CSLSetNameValue(options, "SPARSE_OK", "YES");
		if (m_TiffTiledMode)
		{
			std::stringstream ts;
			ts << m_TiffTileSize;
			options = CSLSetNameValue(options, "TILED", "YES");
			options = CSLSetNameValue(options, "BLOCKXSIZE", ts.str().c_str());
			options = CSLSetNameValue(options, "BLOCKYSIZE", ts.str().c_str());
		}
		else
		{
			m_TiffTileSize=inputRegion.GetSize()[0];
		}

		// Create output raster
		GDALDataset *output =
				driver->Create(m_FileName.c_str(),
						inputRegion.GetSize()[0],
						inputRegion.GetSize()[1],
						nBands,
						dataType,
						options);
		if (output == NULL) {
			fprintf(stderr, "driver->Create call failed.\n");
		}

		// Set GeoTransform
		double gt[6];
		gt[0] = inputPtr->GetOrigin()[0];
		gt[1] = inputPtr->GetSpacing()[0];
		gt[2] = 0.0;
		gt[3] = inputPtr->GetOrigin()[1];
		gt[4] = 0.0;
		gt[5] = inputPtr->GetSpacing()[1];
		output->SetGeoTransform(gt);

		// Set projection
		OGRSpatialReference out_sr;
		char *wkt = NULL;
		out_sr.SetFromUserInput(inputPtr->GetProjectionRef().c_str());
		out_sr.exportToWkt(&wkt);
		output->SetProjection(wkt);

		// Write first and last pixel in the raster
		double *data = new double(sizeof(*data) * 4 * output->GetRasterCount());
		output->RasterIO(GF_Write,
				0,
				0,
				1,
				1,
				data,
				1,
				1,
				output->GetRasterBand(1)->GetRasterDataType(),
				output->GetRasterCount(),
				NULL,
				0,
				0,
				0);

		output->RasterIO(GF_Write,
				inputRegion.GetSize()[0]-1,
				inputRegion.GetSize()[1]-1,
				1,
				1,
				data,
				1,
				1,
				output->GetRasterBand(1)->GetRasterDataType(),
				output->GetRasterCount(),
				NULL,
				0,
				0,
				0);
		delete data;

		// Clean stuff
		OGRFree(wkt);
		CSLDestroy(options);
		GDALClose(output);
	}

	// Wait for rank 0 to finish creating the file
	MPI_Barrier(MPI_COMM_WORLD);

	// Open raster (if not in virtual mode)
	PTIFF* output_raster = NULL;
	if (!m_VirtualMode)
	{
		output_raster = open_raster(m_FileName);

		// First we have to populate blocks offsets
		if (m_MyRank == 0) {
			SPTW_ERROR sperr = populate_tile_offsets(output_raster,
					m_TiffTileSize,
					m_TiffTiledMode);
			if (sperr != sptw::SP_None) {
				fprintf(stderr, "\nError populating tile offsets\n");
			}
		}

		// Wait for tile offsets to be populated
		MPI_Barrier(MPI_COMM_WORLD);
		close_raster(output_raster);

		// Now open it
		output_raster = open_raster(m_FileName);
		MPI_Barrier(MPI_COMM_WORLD);

		if (output_raster == NULL) {
			fprintf(stderr, "Could not open output raster\n");
			MPI_Abort(MPI_COMM_WORLD, 1);
		}
	}
	/************************************************************************
	 *  	Raster update with SPTW
	 ************************************************************************/

	// Check that streaming is relevant
	m_StreamingManager->PrepareStreaming(inputPtr, inputRegion);
	m_NumberOfDivisions = m_StreamingManager->GetNumberOfSplits();
	//	if (inputPtr->GetBufferedRegion() == inputRegion)
	//	{
	//		otbMsgDevMacro(<< "Buffered region is the largest possible region, there is no need for streaming.");
	//		this->SetNumberOfDivisionsStrippedStreaming(1);
	//	}
	//	else if (m_NumberOfDivisions < m_NProcs)
	if (m_NumberOfDivisions < m_NProcs)
	{
		itkWarningMacro(<< "Number of divisions ("<< m_NumberOfDivisions << ") < process count ("
				<< m_NProcs << "). Setting Number of divisions to " << m_NProcs);
		this->SetNumberOfDivisionsStrippedStreaming(m_NProcs);
		m_StreamingManager->PrepareStreaming(inputPtr, inputRegion);
		m_NumberOfDivisions = m_StreamingManager->GetNumberOfSplits();
	}

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
	double processDuration, writeDuration;
	double processStart, writeStart;
	InputImageRegionType streamRegion;
	for (m_CurrentDivision = 0;
			m_CurrentDivision < m_NumberOfDivisions && !this->GetAbortGenerateData();
			m_CurrentDivision++, m_DivisionProgress = 0, this->UpdateFilterProgress())
	{
		streamRegion = m_StreamingManager->GetSplit(m_CurrentDivision);
		if (MyCall() )
		{
			/*
			 * Processing
			 */
			processStart = MPI_Wtime();
			inputPtr->SetRequestedRegion(streamRegion);
			inputPtr->PropagateRequestedRegion();
			inputPtr->UpdateOutputData();
			processDuration += (MPI_Wtime() - processStart);

			/*
			 * Writing using SPTW
			 */
			writeStart = MPI_Wtime();
			if (!m_VirtualMode)
			{
				sptw::write_area(output_raster,
						inputPtr->GetBufferPointer(),
						streamRegion.GetIndex()[0],
						streamRegion.GetIndex()[1],
						streamRegion.GetIndex()[0] + streamRegion.GetSize()[0] -1,
						streamRegion.GetIndex()[1] + streamRegion.GetSize()[1] -1);
			}
			writeDuration += (MPI_Wtime() - writeStart);
		}
	}

	// Clean up
	close_raster(output_raster);
	output_raster = NULL;

	// We wait for other process
	MPI_Barrier(MPI_COMM_WORLD);

	// Get timings
	const int nValues = 2;
	double runtimes[nValues] = {processDuration, writeDuration};
	std::vector<double> process_runtimes(m_NProcs*nValues);
	MPI_Gather(runtimes,
			nValues,
			MPI_DOUBLE,
			&(process_runtimes[0]),
			nValues,
			MPI_DOUBLE,
			0,
			MPI_COMM_WORLD);


	if (m_MyRank == 0 && m_Verbose) {

		std::cout << "Runtimes, in seconds" << std::endl;
		std::cout <<"Process Id\tProcessing\tWriting" << std::endl;
		for (unsigned int i = 0; i < process_runtimes.size(); i+=2) {
			std::cout << (int (i/2)) << "\t" << process_runtimes[i] << "\t" << process_runtimes[i+1] << std::endl;
		}

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

	/*
	 * Writting the geom
	 */
	if (m_WriteGeomFile  || m_FilenameHelper->GetWriteGEOMFile())
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
