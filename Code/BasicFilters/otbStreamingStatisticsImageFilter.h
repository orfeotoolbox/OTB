/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbStreamingStatisticsImageFilter_h
#define __otbStreamingStatisticsImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkNumericTraits.h"
#include "itkArray.h"
#include "itkSimpleDataObjectDecorator.h"
#include "otbStreamingTraits.h"
// #include "itkImageRegionNonUniformMultidimensionalSplitter.h"
// #include "itkImageRegionMultidimensionalSplitter.h"
#include "itkImageRegionSplitter.h"

namespace otb {

/** \class StreamingStatisticsImageFilter
 * \brief Compute min. max, variance and mean of a large image using streaming
 *
 * This filter Computes the same statistics as the StatisticsImageFilter, but it 
 * supports large image since it processes reasonnable pieces of the input image one
 * afther the other. It supports the same streaming mode than the StreamingImageFileWriter.
 *
 * Of course streaming at the end of a pipeline is only available when each filter in the pipeline
 * supports streaming. This filter will also perform multithreading if possible.
 *
 * \note The output image has no sense at all and should not be used.
 *
 * \sa StreamingTraits
 * \sa StreamingImageFileWriter
 * \sa StatisticsImageFilter
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup MathematicalStatisticsImageFilters
 */
template<class TInputImage>
class ITK_EXPORT StreamingStatisticsImageFilter :
    public itk::ImageToImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard Self typedef */
  typedef StreamingStatisticsImageFilter               Self;
  typedef itk::ImageToImageFilter<TInputImage,TInputImage>  Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(StreamingStatisticsImageFilter, ImageToImageFilter);

  /** Image related typedefs. */
  typedef typename TInputImage::Pointer InputImagePointer;

  typedef typename TInputImage::RegionType RegionType;
  typedef typename TInputImage::SizeType   SizeType;
  typedef typename TInputImage::IndexType  IndexType;
  typedef typename TInputImage::PixelType  PixelType;

  typedef StreamingTraits<TInputImage> StreamingTraitsType;
  typedef StreamingMode StreamingModeType;
  
  
  itkStaticConstMacro(InputImageDimension, unsigned int,
                       TInputImage::ImageDimension);

//   typedef itk::ImageRegionNonUniformMultidimensionalSplitter<itkGetStaticConstMacro(InputImageDimension)>  SplitterType;
  typedef itk::ImageRegionSplitter<itkGetStaticConstMacro(InputImageDimension)>  SplitterType;
  typedef typename SplitterType::Pointer SplitterPointer;
  
  /// Streaming-related accessors
  itkSetMacro(BufferMemorySize, unsigned long);
  itkGetMacro(BufferMemorySize, unsigned long);
  itkSetMacro(BufferNumberOfLinesDivisions, unsigned long);
  itkGetMacro(BufferNumberOfLinesDivisions, unsigned long);
  itkSetMacro(NumberOfStreamDivisions,unsigned long);
  itkGetMacro(NumberOfStreamDivisions,unsigned long);
  itkSetMacro(StreamingMode,StreamingModeType);
  itkGetMacro(StreamingMode,StreamingModeType);
  
  itkSetObjectMacro(RegionSplitter, SplitterType);
  itkGetObjectMacro(RegionSplitter, SplitterType);

  /** Image related typedefs. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension );

  /** Type to use for computations. */
  typedef typename itk::NumericTraits<PixelType>::RealType RealType;

  /** Smart Pointer type to a DataObject. */
  typedef typename itk::DataObject::Pointer DataObjectPointer;

  /** Type of DataObjects used for scalar outputs */
  typedef itk::SimpleDataObjectDecorator<RealType>  RealObjectType;
  typedef itk::SimpleDataObjectDecorator<PixelType> PixelObjectType;

  /** Return the computed Minimum. */
  PixelType GetMinimum() const
    { return this->GetMinimumOutput()->Get(); }
  PixelObjectType* GetMinimumOutput();
  const PixelObjectType* GetMinimumOutput() const;

  /** Return the computed Maximum. */
  PixelType GetMaximum() const
    { return this->GetMaximumOutput()->Get(); }
  PixelObjectType* GetMaximumOutput();
  const PixelObjectType* GetMaximumOutput() const;

  /** Return the computed Mean. */
  RealType GetMean() const
    { return this->GetMeanOutput()->Get(); }
  RealObjectType* GetMeanOutput();
  const RealObjectType* GetMeanOutput() const;

  /** Return the computed Standard Deviation. */
  RealType GetSigma() const
    { return this->GetSigmaOutput()->Get(); }
  RealObjectType* GetSigmaOutput();
  const RealObjectType* GetSigmaOutput() const;

  /** Return the computed Variance. */
  RealType GetVariance() const
    { return this->GetVarianceOutput()->Get(); }
  RealObjectType* GetVarianceOutput();
  const RealObjectType* GetVarianceOutput() const;

  /** Return the compute Sum. */
  RealType GetSum() const
    { return this->GetSumOutput()->Get(); }
  RealObjectType* GetSumOutput();
  const RealObjectType* GetSumOutput() const;

  /** Make a DataObject of the correct type to be used as the specified
   * output. */
  virtual DataObjectPointer MakeOutput(unsigned int idx);

  /** Pass the input through unmodified. Do this by Grafting in the
   *  AllocateOutputs method.
   */
  void AllocateOutputs();
  
  void GenerateOutputInformation();

protected:
  StreamingStatisticsImageFilter();
  ~StreamingStatisticsImageFilter(){};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Initialize some accumulators before the threads run. */
  void BeforeThreadedGenerateData ();

  /** Do final mean and variance computation from data accumulated in threads.
   */
  void AfterThreadedGenerateData ();

  /** Multi-thread version GenerateData. */
  void  ThreadedGenerateData (const RegionType&
                              outputRegionForThread,
                              int threadId);

  // Override since the filter needs all the data for the algorithm
  void GenerateInputRequestedRegion();

private:
  StreamingStatisticsImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  itk::Array<RealType>  m_ThreadSum;
  itk::Array<RealType>  m_SumOfSquares;
  itk::Array<long>      m_Count;
  itk::Array<PixelType> m_ThreadMin;
  itk::Array<PixelType> m_ThreadMax;

  /** Use to define the method used to calculate number of divisions */ 
  unsigned long m_BufferMemorySize;
  unsigned long m_BufferNumberOfLinesDivisions;
  unsigned long m_NumberOfStreamDivisions;
  
  SplitterPointer m_RegionSplitter;
  
 /** Use to determine method of calculation number of divisions */
 StreamingModeType  m_StreamingMode;
}; // end of class

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingStatisticsImageFilter.txx"
#endif

#endif
