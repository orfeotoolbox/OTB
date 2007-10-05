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
#ifndef __otbStreamingStatisticsVectorImageFilter_h
#define __otbStreamingStatisticsVectorImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkNumericTraits.h"
#include "itkArray.h"
#include "itkSimpleDataObjectDecorator.h"
#include "otbStreamingTraits.h"
#include "itkImageRegionSplitter.h"
#include "itkVariableSizeMatrix.h"
#include "itkVariableLengthVector.h"


namespace otb {

/** \class StreamingStatisticsVectorImageFilter
 * \brief Compute min. max, covariance of a large image using streaming
 *
 * This filter Computes the same statistics as the StatisticsImageFilter, but it 
 * supports large image since it processes reasonnable pieces of the input image one
 * afther the other. It supports the same streaming mode than the StreamingImageVectorFileWriter.
 *
 * Of course streaming at the end of a pipeline is only available when each filter in the pipeline
 * supports streaming. This filter will also perform multithreading if possible.
 *
 * \note The output image has no sense at all and should not be used.
 *
 * \sa StreamingTraits
 * \sa StreamingImageVectorFileWriter
 * \sa StreamingImageFileWriter
 * \sa StatisticsImageFilter
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup MathematicalStatisticsImageFilters
 */
template<class TInputImage>
class ITK_EXPORT StreamingStatisticsVectorImageFilter :
  public itk::ImageToImageFilter<TInputImage, TInputImage>
  {
  public:
    /** Standard Self typedef */
    typedef StreamingStatisticsVectorImageFilter              Self;
    typedef itk::ImageToImageFilter<TInputImage,TInputImage>  Superclass;
    typedef itk::SmartPointer<Self>                           Pointer;
    typedef itk::SmartPointer<const Self>                     ConstPointer;
    
    /** Method for creation through the object factory. */
    itkNewMacro(Self);
    
    /** Runtime information support. */
    itkTypeMacro(StreamingStatisticsVectorImageFilter, ImageToImageFilter);
    
    /** Image related typedefs. */
    typedef typename TInputImage::Pointer                                          InputImagePointer;
    typedef typename TInputImage::RegionType                                       RegionType;
    typedef typename TInputImage::SizeType                                         SizeType;
    typedef typename TInputImage::IndexType                                        IndexType;
    typedef typename TInputImage::PixelType                                        PixelType;
    typedef typename TInputImage::InternalPixelType                                InternalPixelType;
    typedef StreamingTraits<TInputImage>                                           StreamingTraitsType;
    typedef StreamingMode                                                          StreamingModeType;
    typedef itk::ImageRegionSplitter<itkGetStaticConstMacro(InputImageDimension)>  SplitterType;
    typedef typename SplitterType::Pointer                                         SplitterPointer;
 
    itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
    
    /** Streaming-related accessors */
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
    itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension );
    
    /** Type to use for computations. */
    typedef typename itk::NumericTraits<InternalPixelType>::RealType RealType;
    typedef itk::VariableLengthVector<RealType>                      RealPixelType;
    
    /** Smart Pointer type to a DataObject. */
    typedef typename itk::DataObject::Pointer DataObjectPointer;
    
    /** Type of DataObjects used for scalar outputs */
    typedef typename itk::VariableSizeMatrix<RealType>    MatrixType;
    typedef typename std::vector<MatrixType>              ArrayMatrixType;
    typedef typename itk::Array<long>                     ArrayLongPixelType;
    typedef typename std::vector<RealPixelType>           ArrayRealPixelType;
    typedef typename std::vector<PixelType>               ArrayPixelType;
    typedef itk::SimpleDataObjectDecorator<RealPixelType> RealPixelObjectType;
    typedef itk::SimpleDataObjectDecorator<PixelType>     PixelObjectType;
    typedef itk::SimpleDataObjectDecorator<MatrixType>    MatrixObjectType;


    /** Return the computed Minimum. */
    PixelType GetMinimum() const { return this->GetMinimumOutput()->Get(); };
    PixelObjectType* GetMinimumOutput();
    const PixelObjectType* GetMinimumOutput() const;
    
    /** Return the computed Maximum. */
    PixelType GetMaximum() const { return this->GetMaximumOutput()->Get(); };
    PixelObjectType* GetMaximumOutput();
    const PixelObjectType* GetMaximumOutput() const;
    
    /** Return the computed Mean. */
    RealPixelType GetMean() const { return this->GetMeanOutput()->Get(); };
    RealPixelObjectType* GetMeanOutput();
    const RealPixelObjectType* GetMeanOutput() const;

    /** Return the compute Sum. */
    RealPixelType GetSum() const { return this->GetSumOutput()->Get(); };
    RealPixelObjectType* GetSumOutput();
    const RealPixelObjectType* GetSumOutput() const;
    
    /** Return the computed Covariance. */
    MatrixType GetCovariance() const { return this->GetCovarianceOutput()->Get(); };
    MatrixObjectType* GetCovarianceOutput();
    const MatrixObjectType* GetCovarianceOutput() const;


    /** Make a DataObject of the correct type to be used as the specified
     * output. 
     */
    virtual DataObjectPointer MakeOutput(unsigned int idx);
    
    /** Pass the input through unmodified. Do this by Grafting in the
     *  AllocateOutputs method.
     */
    void AllocateOutputs();
    void GenerateOutputInformation();
    
  protected:
    StreamingStatisticsVectorImageFilter();
    ~StreamingStatisticsVectorImageFilter(){};
    void PrintSelf(std::ostream& os, itk::Indent indent) const;
    /** Initialize some accumulators before the threads run. */
    void BeforeThreadedGenerateData ();
    /** Do final mean and variance computation from data accumulated in threads. */
    void AfterThreadedGenerateData ();
    /** Multi-thread version GenerateData. */
    void  ThreadedGenerateData (const RegionType& outputRegionForThread,int threadId);
    // Override since the filter needs all the data for the algorithm
    void GenerateInputRequestedRegion();
    
  private:
    StreamingStatisticsVectorImageFilter(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented
    
    ArrayRealPixelType m_ThreadSum;
    ArrayLongPixelType m_Count;
    ArrayPixelType     m_ThreadMin;
    ArrayPixelType     m_ThreadMax;
    ArrayMatrixType    m_XX;

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
#include "otbStreamingStatisticsVectorImageFilter.txx"
#endif

#endif
