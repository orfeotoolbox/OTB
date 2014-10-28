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
#ifndef __otbFastICAInternalOptimizerVectorImageFilter_h
#define __otbFastICAInternalOptimizerVectorImageFilter_h

#include "otbMacro.h"
#include "otbMatrixImageFilter.h"
#include "otbStreamingStatisticsVectorImageFilter.h"

namespace otb
{
/** \class FastICAInternalOptimizerVectorImageFilter
 * \brief Internal optimisation of the FastICA unmixing filter
 *
 * This class implements the internal search for the unmixing matrix W
 * in the FastICA technique.
 *
 * The class takes 2 inputs (initial image and its projection with the W matrix).
 *
 * \ingroup Multithreaded
 * \sa FastICAImageFilter
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT FastICAInternalOptimizerVectorImageFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef FastICAInternalOptimizerVectorImageFilter          Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(FastICAInternalOptimizerVectorImageFilter, ImageToImageFilter);

  /** Dimension */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** Template parameters typedefs */
  typedef TInputImage  InputImageType;
  typedef typename InputImageType::RegionType InputRegionType;

  typedef TOutputImage OutputImageType;
  typedef typename OutputImageType::RegionType OutputRegionType;

  /** Filter types and related */
  typedef StreamingStatisticsVectorImageFilter< InputImageType > CovarianceEstimatorFilterType;
  typedef typename CovarianceEstimatorFilterType::Pointer CovarianceEstimatorFilterPointerType;

  typedef typename CovarianceEstimatorFilterType::RealType RealType;
  typedef typename CovarianceEstimatorFilterType::RealPixelType VectorType;
  typedef typename CovarianceEstimatorFilterType::MatrixObjectType MatrixObjectType;
  typedef typename MatrixObjectType::ComponentType MatrixType;
  typedef typename MatrixType::InternalMatrixType InternalMatrixType;
  typedef typename InternalMatrixType::element_type MatrixElementType;

  typedef MatrixImageFilter< TInputImage, TOutputImage > TransformFilterType;
  typedef typename TransformFilterType::Pointer TransformFilterPointerType;

  typedef double (*ContrastFunctionType) ( double );

  itkSetMacro(CurrentBandForLoop, unsigned int);
  itkGetMacro(CurrentBandForLoop, unsigned int);

  itkGetMacro(W, InternalMatrixType);
  itkSetMacro(W, InternalMatrixType);

  itkSetMacro(ContrastFunction, ContrastFunctionType);
  itkGetMacro(Beta, double);
  itkGetMacro(Den, double);

protected:
  FastICAInternalOptimizerVectorImageFilter();
  virtual ~FastICAInternalOptimizerVectorImageFilter() { }

  virtual void GenerateOutputInformation();
  virtual void BeforeThreadedGenerateData ();
  virtual void ThreadedGenerateData ( const OutputRegionType &, itk::ThreadIdType );
  virtual void AfterThreadedGenerateData();

  unsigned int m_CurrentBandForLoop;

  std::vector<double> m_BetaVector;
  std::vector<double> m_DenVector;
  std::vector<double> m_NbSamples;

  double m_Beta;
  double m_Den;

  InternalMatrixType m_W;
  ContrastFunctionType m_ContrastFunction;
  TransformFilterPointerType m_TransformFilter;
private:
  FastICAInternalOptimizerVectorImageFilter( const Self & ); // not implemented
  void operator= ( const Self & ); // not implemented
}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFastICAInternalOptimizerVectorImageFilter.txx"
#endif


#endif


