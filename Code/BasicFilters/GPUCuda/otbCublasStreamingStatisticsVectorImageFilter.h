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
#ifndef __otbCublasStreamingStatisticsVectorImageFilter_h
#define __otbCublasStreamingStatisticsVectorImageFilter_h

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "itkNumericTraits.h"
#include "itkArray.h"
#include "itkSimpleDataObjectDecorator.h"
#include "itkImageRegionSplitter.h"
#include "itkVariableSizeMatrix.h"
#include "itkVariableLengthVector.h"

#include "cublas.h"

namespace otb
{

/** \class PersistentCublasStreamingStatisticsVectorImageFilter
 * \brief Compute mean, covariance & correlation of a large image using streaming
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
class ITK_EXPORT PersistentCublasStreamingStatisticsVectorImageFilter :
  public PersistentImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentCublasStreamingStatisticsVectorImageFilter           Self;
  typedef PersistentImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentCublasStreamingStatisticsVectorImageFilter, PersistentImageFilter);

  /** Image related typedefs. */
  typedef TInputImage                             ImageType;
  typedef typename TInputImage::Pointer           InputImagePointer;
  typedef typename TInputImage::RegionType        RegionType;
  typedef typename TInputImage::SizeType          SizeType;
  typedef typename TInputImage::IndexType         IndexType;
  typedef typename TInputImage::PixelType         PixelType;
  typedef typename TInputImage::InternalPixelType InternalPixelType;

  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);

  /** Image related typedefs. */
  itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension);

  /** Type to use for computations. */
  //typedef typename itk::NumericTraits<InternalPixelType>::RealType RealType;
  typedef float RealType;
  typedef itk::VariableLengthVector<RealType>                      RealPixelType;

  /** Smart Pointer type to a DataObject. */
  typedef typename itk::DataObject::Pointer DataObjectPointer;

  /** Type of DataObjects used for scalar outputs */
  typedef itk::VariableSizeMatrix<RealType>    MatrixType;
  typedef std::vector<MatrixType>              ArrayMatrixType;
  typedef itk::Array<long>                     ArrayLongPixelType;
  typedef std::vector<RealPixelType>           ArrayRealPixelType;
  typedef std::vector<PixelType>               ArrayPixelType;
  typedef itk::SimpleDataObjectDecorator<RealPixelType> RealPixelObjectType;
  typedef itk::SimpleDataObjectDecorator<PixelType>     PixelObjectType;
  typedef itk::SimpleDataObjectDecorator<MatrixType>    MatrixObjectType;

  /** Return the computed Mean. */
  RealPixelType GetMean() const
  {
    return this->GetMeanOutput()->Get();
  }
  RealPixelObjectType* GetMeanOutput();
  const RealPixelObjectType* GetMeanOutput() const;

  /** Return the computed Covariance. */
  MatrixType GetCovariance() const
  {
    return this->GetCovarianceOutput()->Get();
  }
  MatrixObjectType* GetCovarianceOutput();
  const MatrixObjectType* GetCovarianceOutput() const;

  /** Return the computed Covariance. */
  MatrixType GetCorrelation() const
  {
    return this->GetCorrelation()->Get();
  }
  MatrixObjectType* GetCorrelationOutput();
  const MatrixObjectType* GetCorrelationOutput() const;

  /** Make a DataObject of the correct type to be used as the specified
   * output.
   */
  virtual DataObjectPointer MakeOutput(unsigned int idx);

  /** Pass the input through unmodified. Do this by Grafting in the
   *  AllocateOutputs method.
   */
  virtual void AllocateOutputs();
  virtual void GenerateOutputInformation();
  virtual void Synthetize(void);
  virtual void Reset(void);

protected:
  PersistentCublasStreamingStatisticsVectorImageFilter();
  virtual ~PersistentCublasStreamingStatisticsVectorImageFilter();

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Multi-thread version GenerateData. */
  //void  ThreadedGenerateData(const RegionType& outputRegionForThread, int threadId);
  void  GenerateData();

private:
  PersistentCublasStreamingStatisticsVectorImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  RealPixelType m_FirstOrderAccumulator;
  MatrixType    m_SecondOrderAccumulator;

  float*   m_GPUImage;
  SizeType m_GPUImageSize;
  float*   m_GPUSecondOrderAccumulator;

}; // end of class PersistentCublasStreamingStatisticsVectorImageFilter


/** \class CublasStreamingStatisticsVectorImageFilter
 * \brief This class streams the whole input image through the PersistentCublasStreamingStatisticsVectorImageFilter.
 *
 * This way, it allows to compute the first and second order global statistics of this image. It calls the
 * Reset() method of the PersistentCublasStreamingStatisticsVectorImageFilter before streaming the image and the
 * Synthetize() method of the PersistentCublasStreamingStatisticsVectorImageFilter after having streamed the image
 * to compute the statistics. The accessor on the results are wrapping the accessors of the
 * internal PersistentCublasStreamingStatisticsVectorImageFilter.
 *
 * \sa PersistentCublasStreamingStatisticsVectorImageFilter
 * \sa PersistentImageFilter
 * \sa PersistentFilterStreamingDecorator
 * \sa StreamingImageVirtualWriter
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup MathematicalStatisticsImageFilters
 */

template<class TInputImage>
class ITK_EXPORT CublasStreamingStatisticsVectorImageFilter :
  public PersistentFilterStreamingDecorator<PersistentCublasStreamingStatisticsVectorImageFilter<TInputImage> >
{
public:
  /** Standard Self typedef */
  typedef CublasStreamingStatisticsVectorImageFilter Self;
  typedef PersistentFilterStreamingDecorator
  <PersistentCublasStreamingStatisticsVectorImageFilter<TInputImage> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(CublasStreamingStatisticsVectorImageFilter, PersistentFilterStreamingDecorator);

  typedef TInputImage                                 InputImageType;
  typedef typename Superclass::FilterType             StatFilterType;
  typedef typename StatFilterType::PixelType          PixelType;
  typedef typename StatFilterType::RealType           RealType;
  typedef typename StatFilterType::RealPixelType      RealPixelType;
  typedef typename StatFilterType::MatrixType         MatrixType;
  typedef typename StatFilterType::ArrayMatrixType    ArrayMatrixType;
  typedef typename StatFilterType::ArrayLongPixelType ArrayLongPixelType;
  typedef typename StatFilterType::ArrayRealPixelType ArrayRealPixelType;
  typedef typename StatFilterType::ArrayPixelType     ArrayPixelType;

  /** Type of DataObjects used for scalar outputs */
  typedef typename StatFilterType::RealPixelObjectType RealPixelObjectType;
  typedef typename StatFilterType::PixelObjectType     PixelObjectType;
  typedef typename StatFilterType::MatrixObjectType    MatrixObjectType;

  void SetInput(InputImageType * input)
  {
    this->GetFilter()->SetInput(input);
  }
  const InputImageType * GetInput()
  {
    return this->GetFilter()->GetInput();
  }

  /** Return the computed Mean. */
  RealPixelType GetMean() const
  {
    return this->GetFilter()->GetMeanOutput()->Get();
  }
  RealPixelObjectType* GetMeanOutput()
  {
    return this->GetFilter()->GetMeanOutput();
  }
  const RealPixelObjectType* GetMeanOutput() const
  {
    return this->GetFilter()->GetMeanOutput();
  }

  /** Return the computed Covariance. */
  MatrixType GetCovariance() const
  {
    return this->GetFilter()->GetCovarianceOutput()->Get();
  }
  MatrixObjectType* GetCovarianceOutput()
  {
    return this->GetFilter()->GetCovarianceOutput();
  }
  const MatrixObjectType* GetCovarianceOutput() const
  {
    return this->GetFilter()->GetCovarianceOutput();
  }

  /** Return the computed Covariance. */
  MatrixType GetCorrelation() const
  {
    return this->GetFilter()->GetCorrelationOutput()->Get();
  }
  MatrixObjectType* GetCorrelationOutput()
  {
    return this->GetFilter()->GetCorrelationOutput();
  }
  const MatrixObjectType* GetCorrelationOutput() const
  {
    return this->GetFilter()->GetCorrelationOutput();
  }

protected:
  /** Constructor */
  CublasStreamingStatisticsVectorImageFilter() {};
  /** Destructor */
  virtual ~CublasStreamingStatisticsVectorImageFilter() {}

private:
  CublasStreamingStatisticsVectorImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbCublasStreamingStatisticsVectorImageFilter.txx"
#endif

#endif
