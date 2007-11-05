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
     the implied warranty of MERCHANT2ABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbStreamingStatisticsVectorImageFilter_h
#define __otbStreamingStatisticsVectorImageFilter_h


#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "itkNumericTraits.h"
#include "itkArray.h"
#include "itkSimpleDataObjectDecorator.h"
#include "itkImageRegionSplitter.h"
#include "itkVariableSizeMatrix.h"
#include "itkVariableLengthVector.h"


namespace otb {

/** \class StreamingStatisticsVectorImageFilter
 * \brief Compute min. max, covariance of a large image using streaming
 *
 *  This filter persists its temporary data. It means that if you Update it n times on n different
 * requested regions, the output statistics will be the statitics of the whole set of n regions.
 *
 * To reset the temporary data, one should call the Reset() function. 
 *
 * To get the statistics once the regions have been processed via the pipeline, use the Synthetize() method.
 *
 * \sa PersistentImageFilter
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup MathematicalStatisticsImageFilters
 *
 */
template<class TInputImage>
class ITK_EXPORT PersistentStatisticsVectorImageFilter :
  public PersistentImageFilter<TInputImage, TInputImage>
  {
  public:
    /** Standard Self typedef */
    typedef PersistentStatisticsVectorImageFilter              Self;
    typedef PersistentImageFilter<TInputImage,TInputImage>  Superclass;
    typedef itk::SmartPointer<Self>                           Pointer;
    typedef itk::SmartPointer<const Self>                     ConstPointer;
    
    /** Method for creation through the object factory. */
    itkNewMacro(Self);
    
    /** Runtime information support. */
    itkTypeMacro(PersistentStatisticsVectorImageFilter,PersistentImageFilter);
    
    /** Image related typedefs. */
    typedef TInputImage                                                            ImageType;
    typedef typename TInputImage::Pointer                                          InputImagePointer;
    typedef typename TInputImage::RegionType                                       RegionType;
    typedef typename TInputImage::SizeType                                         SizeType;
    typedef typename TInputImage::IndexType                                        IndexType;
    typedef typename TInputImage::PixelType                                        PixelType;
    typedef typename TInputImage::InternalPixelType                                InternalPixelType;
  
    itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
    
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
    void Synthetize(void);
    void Reset(void);

  protected:
    PersistentStatisticsVectorImageFilter();
    ~PersistentStatisticsVectorImageFilter(){};
    void PrintSelf(std::ostream& os, itk::Indent indent) const;
     /** Multi-thread version GenerateData. */
    void  ThreadedGenerateData (const RegionType& outputRegionForThread,int threadId);
    // Override since the filter needs all the data for the algorithm
    //void GenerateInputRequestedRegion();
    
  private:
    PersistentStatisticsVectorImageFilter(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented
    
    ArrayRealPixelType m_ThreadSum;
    ArrayLongPixelType m_Count;
    ArrayPixelType     m_ThreadMin;
    ArrayPixelType     m_ThreadMax;
    ArrayMatrixType    m_XX;

    /** Use to define the method used to calculate number of divisions */ 
    //unsigned long m_BufferMemorySize;
    //unsigned long m_BufferNumberOfLinesDivisions;
    //unsigned long m_NumberOfStreamDivisions;
    
    //SplitterPointer m_RegionSplitter;
    
    /** Use to determine method of calculation number of divisions */
    //StreamingModeType  m_StreamingMode;
  }; // end of class PersistentStatisticsVectorImageFilter

/**===========================================================================*/

/** \class StreamingStatisticsVectorImageFilter
 * \brief This class streams the whole input image through the PersistentStatisticsImageFilter.
 *
 * This way, it allows to compute the first order global statistics of this image. It calls the
 * Reset() method of the PersistentStatisticsImageFilter before streaming the image and the 
 * Synthetize() method of the PersistentStatisticsImageFilter after having streamed the image
 * to compute the statistics. The accessor on the results are wrapping the accessors of the 
 * internal PersistentStatisticsImageFilter.
 *
 * \sa PersistentStatisticsVectorImageFilter
 * \sa PersistentImageFilter
 * \sa PersistentFilterStreamingDecorator
 * \sa StreamingImageVirtualWriter
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup MathematicalStatisticsImageFilters
 */

template<class TInputImage>
class ITK_EXPORT StreamingStatisticsVectorImageFilter :
    public PersistentFilterStreamingDecorator< PersistentStatisticsVectorImageFilter<TInputImage> >
{
public:
  /** Standard Self typedef */
  typedef StreamingStatisticsVectorImageFilter             Self;
  typedef PersistentFilterStreamingDecorator
    < PersistentStatisticsVectorImageFilter<TInputImage> > Superclass;
  typedef itk::SmartPointer<Self>                          Pointer;
  typedef itk::SmartPointer<const Self>                    ConstPointer;


  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(StreamingStatisticsVectorImageFilter,PersistentFilterStreamingDecorator);

  typedef TInputImage InputImageType;
  typedef typename Superclass::FilterType StatFilterType;
  typedef typename StatFilterType::PixelType PixelType;
  typedef typename StatFilterType::RealType RealType;
  typedef typename StatFilterType::RealPixelType RealPixelType;
  typedef typename StatFilterType::MatrixType MatrixType;
  typedef typename StatFilterType::ArrayMatrixType ArrayMatrixType;
  typedef typename StatFilterType::ArrayLongPixelType ArrayLongPixelType;
  typedef typename StatFilterType::ArrayRealPixelType ArrayRealPixelType;
  typedef typename StatFilterType::ArrayPixelType ArrayPixelType;
  typedef typename StatFilterType::PixelObjectType PixelObjectType;
  
  
  
  /** Type of DataObjects used for scalar outputs */
  typedef typename StatFilterType::RealPixelObjectType RealPixelObjectType;
  typedef typename StatFilterType::PixelObjectType PixelObjectType;
  typedef typename StatFilterType::MatrixObjectType MatrixObjectType;
 
  void SetInput(TInputImage * input)
    {
      this->GetFilter()->SetInput(input);
    }
  TInputImage * GetInput()
    {
      return this->GetFilter()->GetInput();
    }


  /** Return the computed Minimum. */
  PixelType GetMinimum() const { return this->GetFilter()->GetMinimumOutput()->Get(); };
  PixelObjectType* GetMinimumOutput(){ return this->GetFilter()->MinimumOutput(); };
  const PixelObjectType* GetMinimumOutput() const{ return this->GetFilter()->MinimumOutput(); };
  
  /** Return the computed Maximum. */
  PixelType GetMaximum() const { return this->GetFilter()->GetMaximumOutput()->Get(); };
  PixelObjectType* GetMaximumOutput(){ return this->GetFilter()->MaximumOutput(); };
  const PixelObjectType* GetMaximumOutput() const{ return this->GetFilter()->MaximumOutput(); };
  
  
  /** Return the computed Mean. */
  RealPixelType GetMean() const { return this->GetFilter()->GetMeanOutput()->Get(); };
  RealPixelObjectType* GetMeanOutput(){ return this->GetFilter()->GetMeanOutput(); };
  const RealPixelObjectType* GetMeanOutput() const {return this->GetFilter()->GetMeanOutput();};
  
  /** Return the compute Sum. */
  RealPixelType GetSum() const { return this->GetFilter()->GetSumOutput()->Get(); };
  RealPixelObjectType* GetSumOutput(){ return this->GetFilter()->GetSumOutput();};
  const RealPixelObjectType* GetSumOutput() const {return this->GetFilter()->GetSumOutput();};
  
  /** Return the computed Covariance. */
  MatrixType GetCovariance() const { return this->GetFilter()->GetCovarianceOutput()->Get(); };
  MatrixObjectType* GetCovarianceOutput(){ return this->GetFilter()->GetCovarianceOutput(); };
  const MatrixObjectType* GetCovarianceOutput() const { return this->GetFilter()->GetCovarianceOutput(); };

 protected:
  /** Constructor */
  StreamingStatisticsVectorImageFilter(){};
  /** Destructor */
  virtual ~StreamingStatisticsVectorImageFilter(){};
  
private:
  StreamingStatisticsVectorImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingStatisticsVectorImageFilter.txx"
#endif

#endif
