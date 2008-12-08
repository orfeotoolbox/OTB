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
#include "itkImageRegionSplitter.h"
#include "otbStreamingTraits.h"

namespace otb
{

/** \class StreamingImageVirtualWriter
 *  \brief This class acts like a StreamingImageFileWriter, but without actually
 *  writing data to the disk.
 *
 *  This allows to stream the whole image through persistent filters such as
 *  PersitentStatisticsImageFilter in order to get the global statistics of an image.
 *
 *  This filter is not intended to be used with classic ImageToImageFilter, though it
 *  will not generate any error.
 *
 *  This filter proposes the same streaming setup methods than the StreamingImageFileWriter.
 *
 *  It is used in the PersistentFilterStreamingDecorator helper class to propose an easy
 *  way to stream an image through a persistent filter.
 *
 * \sa PersistentImageFilter
 * \sa PersistentStatisticsImageFilter
 * \sa PersistentImageStreamingDecorator.
 */
template <class TInputImage>
  class ITK_EXPORT StreamingImageVirtualWriter : public itk::ImageToImageFilter<TInputImage,TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef StreamingImageVirtualWriter  Self;
  typedef itk::ImageToImageFilter<TInputImage,TInputImage>  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(StreamingImageVirtualWriter,itk::ImageToImageFilter);

  /** Some typedefs for the input and output. */
  typedef TInputImage InputImageType;
  typedef typename InputImageType::Pointer InputImagePointer;
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename InputImageType::PixelType InputImagePixelType;

  /** Streaming traits helper typedef */
  typedef StreamingTraits<InputImageType> StreamingTraitsType;

  /** Dimension of input image. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      InputImageType::ImageDimension);

  /** Set/Get the image input of this writer.  */
  void SetInput(const InputImageType *input);
  const InputImageType * GetInput(void);
  const InputImageType * GetInput(unsigned int idx);

  /** SmartPointer to a region splitting object */
  typedef itk::ImageRegionSplitter<itkGetStaticConstMacro(InputImageDimension)>  SplitterType;
  typedef typename SplitterType::Pointer RegionSplitterPointer;

  /**  Set buffer memory size (in bytes) use to calculate the number of stream divisions */
  void SetBufferMemorySize(unsigned long );

  /**  Set the buffer number of lines use to calculate the number of stream divisions */
  void SetBufferNumberOfLinesDivisions(unsigned long);

  /**  The number of stream divisions is calculate by using
   * OTB_STREAM_IMAGE_SIZE_TO_ACTIVATE_STREAMING and
   * OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING cmake variables.
   */
  void SetAutomaticNumberOfStreamDivisions(void);

  /** Set the tiling automatic mode for streaming division */
  void SetTilingStreamDivisions(void);
  /** Choose number of divisions in tiling streaming division */
  void SetTilingStreamDivisions(unsigned long);

  /** Return the string to indicate the method use to calculate number of stream divisions. */
  std::string GetMethodUseToCalculateNumberOfStreamDivisions(void);

  /** Set the number of pieces to divide the input.  The upstream pipeline
   * will be executed this many times. */
  void SetNumberOfStreamDivisions(unsigned long);

  /** Get the number of pieces to divide the input. The upstream pipeline
   * will be executed this many times. */
  unsigned long GetNumberOfStreamDivisions(void);

  /** Set the helper class for dividing the input into chunks. */
  itkSetObjectMacro(RegionSplitter, SplitterType);

  /** Get the helper class for dividing the input into chunks. */
  itkGetObjectMacro(RegionSplitter, SplitterType);


  /** Type use to define number of divisions */
  typedef StreamingMode CalculationDivisionEnumType;


  virtual void GenerateInputRequestedRegion(void);

protected:
  StreamingImageVirtualWriter();
  ~StreamingImageVirtualWriter();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void GenerateData(void);

private:
  StreamingImageVirtualWriter(const StreamingImageVirtualWriter&); //purposely not implemented
  void operator=(const StreamingImageVirtualWriter&); //purposely not implemented

  /** This method calculate the number of stream divisions, by using the CalculationDivision type */
  unsigned long CalculateNumberOfStreamDivisions(void);

  /** Use to define the method used to calculate number of divisions */
  unsigned long m_BufferMemorySize;
  unsigned long m_BufferNumberOfLinesDivisions;
  unsigned long m_NumberOfStreamDivisions;

  RegionSplitterPointer m_RegionSplitter;

  /** Use to determine method of calculation number of divisions */
  CalculationDivisionEnumType m_CalculationDivision;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingImageVirtualWriter.txx"
#endif

#endif
