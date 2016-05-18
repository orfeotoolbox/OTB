#ifndef __SimpleParallelTiffWriter_h
#define __SimpleParallelTiffWriter_h


#include "otbImageIOBase.h"
#include "itkProcessObject.h"
#include "otbStreamingManager.h"
#include "otbExtendedFilenameToWriterOptions.h"

// Time probe
#include "itkTimeProbe.h"

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
#include "sptw.h"

// GDAL-OTB datatype brigde
#include "otbGdalDataTypeBridge.h"

namespace otb
{


/** \class SimpleParallelTiffWriter
 * \brief Writes a GeoTiff image using parallel non collective operations.
 *
 * SimpleParallelTiffWriter writes its input data to a single output file.
 * SimpleParallelTiffWriter interfaces with an MPI-IO based class to write out the
 * data whith streaming process and mutliple processing nodes.
 *
 * SimpleParallelTiffWriter will divide the output into several pieces
 * (controlled by SetNumberOfDivisionsStrippedStreaming, SetNumberOfLinesStrippedStreaming,
 * SetAutomaticStrippedStreaming, SetTileDimensionTiledStreaming or SetAutomaticTiledStreaming),
 * and call the upstream pipeline for each piece, tiling the individual outputs into one large
 * output. This reduces the memory footprint for the application since
 * each filter does not have to process the entire dataset at once.
 *
 * SimpleParallelTiffWriter will write directly the streaming buffer in the image file, so
 * that the output image never needs to be completely allocated
 *
 * SimpleParallelTiffWriter implements a version of Simple Parallel Tiff Writer (SPTW,
 * D.M. Mattli, USGS)
 *
 * Splitting strategies are close to those implemented in ImageFileWriter, except
 * layout is optimized for the number of MPI processes for stripped regions.
 * TODO: optimize the splitting layout for tiled regions
 *
 *
 * \sa ImageFileWriter
 */
template <class TInputImage>
class ITK_EXPORT SimpleParallelTiffWriter : public itk::ProcessObject
{
public:
	/** Standard class typedefs. */
	typedef SimpleParallelTiffWriter                                   Self;
	typedef itk::ProcessObject                                Superclass;
	typedef itk::SmartPointer<Self>                           Pointer;
	typedef itk::SmartPointer<const Self>                     ConstPointer;

	/** Method for creation through the object factory. */
	itkNewMacro(Self);

	/** Run-time type information (and related methods). */
	itkTypeMacro(SimpleParallelTiffWriter, itk::ProcessObject);

	/** Some typedefs for the input and output. */
	typedef TInputImage                            InputImageType;
	typedef typename InputImageType::Pointer       InputImagePointer;
	typedef typename InputImageType::RegionType    InputImageRegionType;
	typedef typename InputImageType::PixelType     InputImagePixelType;
	typedef typename InputImageType::IndexType     InputIndexType;

	typedef TInputImage                            OutputImageType;
	typedef typename OutputImageType::Pointer      OutputImagePointer;
	typedef typename OutputImageType::RegionType   OutputImageRegionType;
	typedef typename OutputImageType::PixelType    OutputImagePixelType;
	typedef typename Superclass::DataObjectPointer DataObjectPointer;

	/** The Filename Helper. */
	typedef otb::ExtendedFilenameToWriterOptions            FNameHelperType;

	/** Dimension of input image. */
	itkStaticConstMacro(InputImageDimension, unsigned int,
			InputImageType::ImageDimension);

	/** Streaming manager base class pointer */
	typedef otb::StreamingManager<InputImageType>       StreamingManagerType;
	typedef typename StreamingManagerType::Pointer StreamingManagerPointerType;

	/**  Return the StreamingManager object responsible for dividing
	 *   the region to write */
	StreamingManagerType* GetStreamingManager(void)
	{
		return m_StreamingManager;
	}

	/**  Set a user-specified implementation of StreamingManager
	 *   used to divide the largest possible region in several divisions */
	void SetStreamingManager(StreamingManagerType* streamingManager)
	{
		m_StreamingManager = streamingManager;
	}

	/**  Set the streaming mode to 'stripped' and configure the number of strips
	 *   which will be used to stream the image */
	void SetNumberOfDivisionsStrippedStreaming(unsigned int nbDivisions);

	/**  Set the streaming mode to 'tiled' and configure the number of tiles
	 *   which will be used to stream the image */
	void SetNumberOfDivisionsTiledStreaming(unsigned int nbDivisions);

	/**  Set the streaming mode to 'stripped' and configure the number of strips
	 *   which will be used to stream the image with respect to a number of line
	 *   per strip */
	void SetNumberOfLinesStrippedStreaming(unsigned int nbLinesPerStrip);

	/**  Set the streaming mode to 'stripped' and configure the number of MB
	 *   available. The actual number of divisions is computed automatically
	 *   by estimating the memory consumption of the pipeline.
	 *   Setting the availableRAM parameter to 0 means that the available RAM
	 *   is set from the CMake configuration option.
	 *   The bias parameter is a multiplier applied on the estimated memory size
	 *   of the pipeline and can be used to fine tune the potential gap between
	 *   estimated memory and actual memory used, which can happen because of
	 *   composite filters for example */
	void SetAutomaticStrippedStreaming(unsigned int availableRAM = 0, double bias = 1.0);

	/**  Set the streaming mode to 'tiled' and configure the dimension of the tiles
	 *   in pixels for each dimension (square tiles will be generated) */
	void SetTileDimensionTiledStreaming(unsigned int tileDimension);

	/**  Set the streaming mode to 'tiled' and configure the number of MB
	 *   available. The actual number of divisions is computed automatically
	 *   by estimating the memory consumption of the pipeline.
	 *   Tiles will be square.
	 *   Setting the availableRAM parameter to 0 means that the available RAM
	 *   is set from the CMake configuration option
	 *   The bias parameter is a multiplier applied on the estimated memory size
	 *   of the pipeline and can be used to fine tune the potential gap between
	 *   estimated memory and actual memory used, which can happen because of
	 *   composite filters for example */
	void SetAutomaticTiledStreaming(unsigned int availableRAM = 0, double bias = 1.0);

	/**  Set the streaming mode to 'adaptative' and configure the number of MB
	 *   available. The actual number of divisions is computed automatically
	 *   by estimating the memory consumption of the pipeline.
	 *   Tiles will try to match the input file tile scheme.
	 *   Setting the availableRAM parameter to 0 means that the available RAM
	 *   is set from the CMake configuration option */
	void SetAutomaticAdaptativeStreaming(unsigned int availableRAM = 0, double bias = 1.0);

	/** Set the only input of the writer */
	using Superclass::SetInput;
	virtual void SetInput(const InputImageType *input);

	/** Get writer only input */
	const InputImageType* GetInput();

	/** Does the real work. */
	virtual void Update();

	/** SimpleParallelTiffWriter Methods */
	virtual void SetFileName(const char* extendedFileName);
	virtual void SetFileName(std::string extendedFileName);
	virtual const char* GetFileName () const;

	/** Specify the region to write. If left NULL, then the whole image
	 * is written. */
	void SetIORegion(const itk::ImageIORegion& region);
	itkGetConstReferenceMacro(IORegion, itk::ImageIORegion);

	/** Set the compression On or Off */
	itkSetMacro(UseCompression, bool);
	itkGetConstReferenceMacro(UseCompression, bool);
	itkBooleanMacro(UseCompression);

	/** By default the MetaDataDictionary is taken from the input image and
	 *  passed to the ImageIO. In some cases, however, a user may prefer to
	 *  introduce her/his own MetaDataDictionary. This is often the case of
	 *  the ImageSeriesWriter. This flag defined whether the MetaDataDictionary
	 *  to use will be the one from the input image or the one already set in
	 *  the ImageIO object. */
	itkSetMacro(UseInputMetaDataDictionary, bool);
	itkGetConstReferenceMacro(UseInputMetaDataDictionary, bool);
	itkBooleanMacro(UseInputMetaDataDictionary);

	itkSetObjectMacro(ImageIO, otb::ImageIOBase);
	itkGetObjectMacro(ImageIO, otb::ImageIOBase);
	itkGetConstObjectMacro(ImageIO, otb::ImageIOBase);

	/* MPI parameters accessors */
	itkSetMacro(MyRank, int);
	itkGetMacro(MyRank, int);
	itkSetMacro(NProcs, int);
	itkGetMacro(NProcs, int);

	/* Writer modes */
	itkSetMacro(Verbose, bool);
	itkGetMacro(Verbose, bool);
	itkSetMacro(VirtualMode, bool);
	itkGetMacro(VirtualMode, bool);

	/* GeoTiff options */
	itkSetMacro(TiffTileSize, int);
	itkGetMacro(TiffTileSize, int);
	itkSetMacro(TiffTiledMode, bool);
	itkGetMacro(TiffTiledMode, bool);

protected:
	SimpleParallelTiffWriter();
	virtual ~SimpleParallelTiffWriter();
	void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
	SimpleParallelTiffWriter(const SimpleParallelTiffWriter &); //purposely not implemented
	void operator =(const SimpleParallelTiffWriter&); //purposely not implemented

	void ObserveSourceFilterProgress(itk::Object* object, const itk::EventObject & event )
	{
		if (typeid(event) != typeid(itk::ProgressEvent))
		{
			return;
		}

		itk::ProcessObject* processObject = dynamic_cast<itk::ProcessObject*>(object);
		if (processObject)
		{
			m_DivisionProgress = processObject->GetProgress();
		}

		this->UpdateFilterProgress();
	}

	void UpdateFilterProgress()
	{
		this->UpdateProgress( (m_DivisionProgress + m_CurrentDivision) / m_NumberOfDivisions );
	}

	/*
	 * Returns the process id which process a given region
	 */
	int GetProcFromDivision(int regionIndex)
	{
		if (m_NProcs==0)
			return 0;
		return ( regionIndex % m_NProcs);
	}

	/*
	 * Arranges the splitting layout to match the number of MPI processes
	 */
	unsigned int OptimizeStrippedSplittingLayout(unsigned int n);

	unsigned int m_NumberOfDivisions;
	unsigned int m_CurrentDivision;
	float m_DivisionProgress;

	/** SimpleParallelTiffWriter Parameters */
	std::string m_FileName;

	otb::ImageIOBase::Pointer m_ImageIO;

	bool m_UserSpecifiedImageIO; //track whether the ImageIO is user specified

	itk::ImageIORegion m_IORegion;
	bool               m_UserSpecifiedIORegion; // track whether the region is user specified
	bool m_FactorySpecifiedImageIO; //track whether the factory mechanism set the ImageIO
	bool m_UseCompression;
	bool m_UseInputMetaDataDictionary; // whether to use the
	// MetaDataDictionary from the
	// input or not.

	bool m_WriteGeomFile;              // Write a geom file to store the
	// kwl

	FNameHelperType::Pointer m_FilenameHelper;

	StreamingManagerPointerType m_StreamingManager;

	bool          m_IsObserving;
	unsigned long m_ObserverID;
	InputIndexType m_ShiftOutputIndex;

	int m_MyRank;
	int m_NProcs;
	int m_TiffTileSize;
	bool m_Verbose;
	bool m_VirtualMode;
	bool m_TiffTiledMode;
};


} // end namespace itk



#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSimpleParallelTiffWriter.txx"
#endif

#endif
