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
#ifndef __otbStreamingManager_h
#define __otbStreamingManager_h

#include "otbMacro.h"
#include "otbConfigure.h"

#include "itkLightObject.h"
#include "itkImageRegionSplitter.h"

namespace otb
{

namespace StreamingManagement
{
  enum StreamingMode
  {
    /** Estimates the memory used by the pipeline
     * and set the strips size to fit the specified
     * available RAM
     */
    STRIPPED_AVAILABLE_RAM,

    /** Force the use of a specific number of lines per strips
     */
    STRIPPED_SET_NUMBEROFLINES,

    /** Estimates the memory used that will be used during the pipeline
     * execution and set the tile size to fit the specified
     * available RAM.
     */
    TILED_AVAILABLE_RAM,

    /** Force the use of a specific tile dimension
     * The associated parameter is the size used in each dimension
     */
    TILED_SET_TILE_SIZE,
  };
}

/** \class StreamingManager
 *  \brief This class handles the streaming process used in the writers implementation
 *
 *  The streaming mode can be chosen with either SetStrippedRAMStreamingMode, SetStrippedNumberOfLinesStreamingMode,
 *  SetTiledRAMStreamingMode, or SetTiledTileDimensionStreamingMode.
 *
 *  Then, PrepareStreaming must be called so that the stream type and dimensions are computed
 *  This involves passing the actual DataObject who will be written, since it will be used
 *  during memory estimation for some specific streaming modes.
 *
 *  After PrepareStreaming has been called, the actual number of splits and streaming mode which will be used
 *  can be retrieved with GetStreamingMode and GetNumberOfSplits.
 *  The different splits can be retrieved with GetSplit
 *
 * \sa StreamingImageFileWriter
 * \sa StreamingImageVirtualFileWriter
 */
template<class TImage>
class ITK_EXPORT StreamingManager : public itk::LightObject
{
public:
  /** Standard class typedefs. */
  typedef StreamingManager               Self;
  typedef itk::LightObject               Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  typedef TImage                                 ImageType;
  typedef typename ImageType::Pointer            ImagePointerType;
  typedef typename ImageType::RegionType         RegionType;
  typedef typename RegionType::IndexType         IndexType;
  typedef typename RegionType::SizeType          SizeType;
  typedef typename ImageType::InternalPixelType  PixelType;

  typedef StreamingManagement::StreamingMode     StreamingModeType;

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Type macro */
  itkTypeMacro(StreamingManager, itk::LightObject);

  /** Dimension of input image. */
  itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

  /** Use stripped mode.
   * The number of lines of the strips are computed by estimating the
   * memory footprint of the pipeline, and chosen so that no more than
   * availableRAM MBytes of memory are used.
   * If no parameter is given, then the available RAM is retrieved from the
   * OTB configuration file or build configuration option */
  virtual void SetStrippedRAMStreamingMode( unsigned int availableRAMInMB = 0 );

  /** Use stripped mode.
   * The number of lines of the strips are explicitely specified */
  virtual void SetStrippedNumberOfLinesStreamingMode( unsigned int numberOfLines );

  /** Use tiled mode.
   * The dimensions of the tiles are computed by estimating the
   * memory footprint of the pipeline, and chosen so that no more than
   * availableRAM MBytes of memory are used.
   *
   * If no parameter is given, then the available RAM is retrieved from the
   * OTB configuration file or build configuration option.
   *
   * The tiles are set to be square, with dimension aligned with multiple of 16 */
  virtual void SetTiledRAMStreamingMode( unsigned int availableRAMInMB = 0 );

  /** Use tiled mode.
   * The dimension of the tile are explicitely specified.
   * The parameter specifies the size of the tile in each dimension.  */
  virtual void SetTiledTileDimensionStreamingMode( unsigned int tileDimension );

  /** Actually computes the stream divisions, accorfing to the specified streaming mode,
   * eventually using the input parameter to estimate memory consumption */
  virtual void PrepareStreaming(itk::DataObject * input, const RegionType &region);

  /** Returns the actual streaming mode that will be used to process the image.
   * PrepareStreaming() must have been called before.
   * This can be different than the required streaming mode. For example, if
   * the input passed to PrepareStreaming() is fully buffered, then
   * the STRIPPED_SET_NUMBEROFLINES mode is used with only one strip */
  virtual StreamingManagement::StreamingMode GetStreamingMode();

  /** Returns the actual number of pieces that will be used to process the image.
   * PrepareStreaming() must have been called before.
   * This can be different than the requested number */
  virtual unsigned int GetNumberOfSplits();

  /** Get a region definition that represents the ith piece a specified region.
   * The "numberOfPieces" must be equal to what
   * GetNumberOfSplits() returns. */
  virtual RegionType GetSplit(unsigned int i);


public:
  StreamingManager();
  virtual ~StreamingManager();

  virtual unsigned int EstimateOptimalNumberOfDivisions(itk::DataObject * input, const RegionType &region);

  /** The desired streaming mode specified by the user */
  StreamingManagement::StreamingMode m_DesiredMode;

  /** The actual streaming mode which will be used */
  StreamingManagement::StreamingMode m_ActualMode;

  /** The available RAM set as parameter when using the STRIPPED_AVAILABLE_RAM or TILED_AVAILABLE_RAM mode */
  unsigned int m_AvailableRAMInMB;

  /** The desired number of lines when using the STRIPPED_SET_NUMBEROFLINES streaming mode */
  unsigned int m_DesiredNumberOfLines;

  /** The desired tile dimension when using the TILED_SET_TILE_SIZE streaming mode */
  unsigned int m_DesiredTileDimension;

  /** The computed number of splits after PrepareStreaming has been called */
  unsigned int m_ComputedNumberOfSplits;

  /** The region to stream */
  RegionType m_Region;

  /** The splitter used to compute the different strips */
  typedef itk::ImageRegionSplitter<itkGetStaticConstMacro(ImageDimension)> AbstractSplitterType;
  typedef typename AbstractSplitterType::Pointer AbstractSplitterPointerType;
  AbstractSplitterPointerType m_Splitter;

private:
  StreamingManager(const StreamingManager &); //purposely not implemented
  void operator =(const StreamingManager&); //purposely not implemented

};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingManager.txx"
#endif

#endif
