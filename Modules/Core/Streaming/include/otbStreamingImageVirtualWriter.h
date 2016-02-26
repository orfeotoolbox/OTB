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
#ifndef __otbStreamingImageVirtualWriter_h
#define __otbStreamingImageVirtualWriter_h

#include "itkMacro.h"
#include "itkImageToImageFilter.h"
#include "otbStreamingManager.h"

namespace otb
{

/** \class StreamingImageVirtualWriter
 *  \brief This class acts like a StreamingImageFileWriter, but without actually
 *  writing data to the disk.
 *
 *  This allows streaming the whole image through persistent filters such as
 *  PersitentStatisticsImageFilter in order to get the global statistics of an image.
 *
 *  This filter is not intended to be used with classic ImageToImageFilter, though it
 *  will not generate any error.
 *
 *  The computation of divisions can be done following different strategies
 *  by setting the StreamingManager attributes with SetStreamingManager.
 *  A set of behaviors are available with the following methods :
 *  - SetNumberOfLinesStrippedStreaming : divide by strips, according to a number of lines
 *  - SetAutomaticStrippedStreaming : divide by strips, according to available RAM
 *  - SetTileDimensionTiledStreaming : divide by tiles, according to a desired tile dimension
 *  - SetAutomaticTiledStreaming : divide by tiles, according to available RAM
 *
 *  It is used in the PersistentFilterStreamingDecorator helper class to propose an easy
 *  way to stream an image through a persistent filter.
 *
 * \sa PersistentImageFilter
 * \sa PersistentStatisticsImageFilter
 * \sa PersistentImageStreamingDecorator.
 *
 * \ingroup OTBStreaming
 */
template <class TInputImage >
class ITK_EXPORT StreamingImageVirtualWriter : public itk::ImageToImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef StreamingImageVirtualWriter                       Self;
  typedef itk::ImageToImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(StreamingImageVirtualWriter, itk::ImageToImageFilter);

  /** Some typedefs for the input and output. */
  typedef TInputImage                         InputImageType;
  typedef typename InputImageType::Pointer    InputImagePointer;
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename InputImageType::PixelType  InputImagePixelType;

  /** Streaming manager base class pointer */
  typedef StreamingManager<InputImageType>       StreamingManagerType;
  typedef typename StreamingManagerType::Pointer StreamingManagerPointerType;

  /** Dimension of input image. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      InputImageType::ImageDimension);

  /**  Return the StreamingManager object responsible for dividing
   *   the region to write */
  StreamingManagerType* GetStreamingManager(void)
    {
    return m_StreamingManager;
    }

  /**  Set a user-specific implementation of StreamingManager
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
   *   which will be used to stream the image */
  void SetNumberOfLinesStrippedStreaming(unsigned int nbLinesPerStrip);

  /**  Set the streaming mode to 'stripped' and configure the number of MB
   *   available. The actual number of divisions is computed automatically
   *   by estimating the memory consumption of the pipeline.
   *   Setting the availableRAM parameter to 0 means that the available RAM
   *   is set from the CMake configuration option */
  void SetAutomaticStrippedStreaming(unsigned int availableRAM = 0, double bias = 1.0);

  /**  Set the streaming mode to 'tiled' and configure the dimension of the tiles
   *   in pixels for each dimension (square tiles will be generated) */
  void SetTileDimensionTiledStreaming(unsigned int tileDimension);

  /**  Set the streaming mode to 'tiled' and configure the number of MB
   *   available. The actual number of divisions is computed automatically
   *   by estimating the memory consumption of the pipeline.
   *   Tiles will be square.
   *   Setting the availableRAM parameter to 0 means that the available RAM
   *   is set from the CMake configuration option */
  void SetAutomaticTiledStreaming(unsigned int availableRAM = 0, double bias = 1.0);

  /**  Set the streaming mode to 'adaptative' and configure the number of MB
   *   available. The actual number of divisions is computed automatically
   *   by estimating the memory consumption of the pipeline.
   *   Tiles will try to match the input file tile scheme.
   *   Setting the availableRAM parameter to 0 means that the available RAM
   *   is set from the CMake configuration option */
  void SetAutomaticAdaptativeStreaming(unsigned int availableRAM = 0, double bias = 1.0);

  /** Override Update() from ProcessObject
   *  This filter does not produce an output */
  virtual void Update();

protected:
  StreamingImageVirtualWriter();

  virtual ~StreamingImageVirtualWriter();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void GenerateData(void);

  virtual void GenerateInputRequestedRegion(void);

private:
  StreamingImageVirtualWriter(const StreamingImageVirtualWriter &); //purposely not implemented
  void operator =(const StreamingImageVirtualWriter&); //purposely not implemented

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

  unsigned int m_NumberOfDivisions;
  unsigned int m_CurrentDivision;
  float m_DivisionProgress;

  StreamingManagerPointerType m_StreamingManager;

  bool          m_IsObserving;
  unsigned long m_ObserverID;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingImageVirtualWriter.txx"
#endif

#endif
