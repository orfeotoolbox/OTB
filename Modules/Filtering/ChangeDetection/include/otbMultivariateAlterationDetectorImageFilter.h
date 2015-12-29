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
#ifndef __otbMultivariateAlterationDetectorImageFilter_h
#define __otbMultivariateAlterationDetectorImageFilter_h


#include "otbStreamingStatisticsVectorImageFilter.h"
#include "otbConcatenateVectorImageFilter.h"

#include "vnl/vnl_vector.h"
#include "vnl/vnl_matrix.h"

namespace otb
{

/** \class MultivariateAlterationDetectorImageFilter
 * \brief This filter implements the Multivariate Alteration Detector
 *
 * This filter implements the Multivariate Alteration Detector, based
 * on the following work:
 *
 * A. A. Nielsen and K. Conradsen, "Multivariate alteration detection
 * (mad) in multispectral, bi-temporal image data: a new approach to
 * change detection studies," Remote Sens. Environ., vol. 64,
 * pp. 1-19, (1998)
 *
 * Multivariate Alteration Detector takes two images as inputs and
 * produce a set of N change maps as a VectorImage (where N is the
 * maximum of number of bands in first and second image) with the
 * following properties:
 *
 * - Change maps are differences of a pair of linear combinations of
 * bands from image 1 and bands from image 2 chosen to maximize the
 * correlation.
 * - Each change map is orthogonal to the others.
 *
 * This is a statistical method which can handle different modalities
 * and even differents bands and number of bands between images.
 *
 * If numbers of bands in image 1 and 2 are equal, then change maps
 * are sorted by increasing correlation. If number of bands is
 * different, the change maps are sorted by decreasing correlation.
 *
 * The GetV1() and GetV2() methods allow retrieving the linear
 * combinations used to generate the Mad change maps as a vnl_matrix of
 * double, and the GetRho() method allows retrieving the correlation
 * associated to each Mad change maps as a vnl_vector.
 *
 * This filter has been implemented from the Matlab code kindly made
 * available by the authors here:
 * http://www2.imm.dtu.dk/~aa/software.html
 *
 * Both cases (same and different number of bands) have been validated
 * by comparing the output image to the output produced by the Matlab
 * code, and the reference images for testing have been generated from
 * the Matlab code using Octave.
 *
 * \ingroup Streamed, Multithreaded
 *
 * \ingroup OTBChangeDetection
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT MultivariateAlterationDetectorImageFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef MultivariateAlterationDetectorImageFilter           Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage>  Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MultivariateAlterationDetectorImageFilter, ImageToImageFilter);

  /** Some convenient typedefs. */
  typedef TInputImage                            InputImageType;
  typedef typename InputImageType::Pointer      InputImagePointer;
  typedef typename InputImageType::ConstPointer InputImageConstPointer;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType     InputImagePixelType;
  typedef TOutputImage                           OutputImageType;
  typedef typename OutputImageType::Pointer      OutputImagePointer;
  typedef typename OutputImageType::RegionType   OutputImageRegionType;
  typedef typename OutputImageType::PixelType    OutputImagePixelType;

  /** Internal filters types */
  typedef StreamingStatisticsVectorImageFilter<InputImageType> CovarianceEstimatorType;
  typedef typename CovarianceEstimatorType::Pointer            CovarianceEstimatorPointer;
  typedef otb::ConcatenateVectorImageFilter<InputImageType, InputImageType, InputImageType> ConcatenateImageFilterType;
  typedef typename ConcatenateImageFilterType::Pointer         ConcatenateImageFilterPointer;

  typedef typename CovarianceEstimatorType::MatrixObjectType   MatrixObjectType;
  typedef typename MatrixObjectType::ComponentType             MatrixType;
  typedef typename MatrixType::InternalMatrixType              InternalMatrixType;
  typedef typename CovarianceEstimatorType::RealPixelType      VectorType;

  typedef typename VectorType::ValueType                       RealType;

  typedef vnl_vector<RealType>                                 VnlVectorType;
  typedef vnl_matrix<RealType>                                 VnlMatrixType;

  /** Get the mean of bands from image 1 */
  itkGetMacro(Mean1,VnlVectorType);

  /** Get the mean of bands from image 2 */
  itkGetMacro(Mean2,VnlVectorType);

  /** Get the linear combinations of bands of image 1 associated to
   *  multivariate alteration detector. This is a square matrix of
   *  size nbBand of image 1. */
  itkGetMacro(V1, VnlMatrixType);

  /** Get the linear combinations of bands of image 2 associated to
   *  multivariate alteration detector. This is a square matrix of
   *  size nbBand of image 2. */
  itkGetMacro(V2, VnlMatrixType);

  /** Get the correlation coefficient associated with each mad.*/
  itkGetMacro(Rho, VnlVectorType);

  /** Get the covariance estimator (for progress reporting purposes) */
  itkGetObjectMacro(CovarianceEstimator, CovarianceEstimatorType);

  /** Connect one of the operands for pixel-wise addition */
  void SetInput1(const TInputImage * image1);

  /** Connect one of the operands for pixel-wise addition */
  void SetInput2(const TInputImage * image2);

  /** Get the inputs */
  const TInputImage * GetInput1();
  const TInputImage * GetInput2();

protected:
  MultivariateAlterationDetectorImageFilter();
  virtual ~MultivariateAlterationDetectorImageFilter() {}

  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

  virtual void GenerateOutputInformation();

private:
  MultivariateAlterationDetectorImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  CovarianceEstimatorPointer m_CovarianceEstimator;
  MatrixType                 m_CovarianceMatrix;
  VectorType                 m_MeanValues;

  VnlMatrixType              m_V1;
  VnlMatrixType              m_V2;
  VnlVectorType              m_Mean1;
  VnlVectorType              m_Mean2;
  VnlVectorType              m_Rho;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMultivariateAlterationDetectorImageFilter.txx"
#endif

#endif
