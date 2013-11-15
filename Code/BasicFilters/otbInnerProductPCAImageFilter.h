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
#ifndef __otbInnerProductPCAImageFilter_h
#define __otbInnerProductPCAImageFilter_h

#include "itkImageToImageFilter.h"
#include "otbImage.h"
#include "otbMacro.h"
#include "otbEstimateInnerProductPCAImageFilter.h"
#include "otbNormalizeInnerProductPCAImageFilter.h"
#include "otbMeanFunctor.h"
#include "itkUnaryFunctorImageFilter.h"
#include "otbImageToVectorImageCastFilter.h"
#include "otbConcatenateVectorImageFilter.h"
#include "otbImageToVectorImageCastFilter.h"

namespace otb
{
/** \class InnerProductPCAImageFilter
 *  \brief Performs a PCA with streaming capabilities
 *
 * InnerProductPCAImageFilter performs a principal component analysis (as
 * itk::ImagePCAShapeModelEstimator) (PCA) on a vector image in streaming capabilities.
 * Alls channels of the input vector image are used for training images
 * and also the number of desired largest principal components needed.
 * The number of channels of the output image are the user specified number of desired
 * largest principal components.
 *
 * The algorithm uses the VNL library to perform the eigen analysis. To speed
 * the computation of the instead of performing the eigen analysis of the
 * covariance vector A*A' where A is a matrix with p x t, p = number of
 * pixels or voxels in each images and t = number of training images, we
 * calculate the eigen vectors of the inner product matrix A'*A. The resulting
 * eigen vectors (E) are then multiplied with the the matrix A to get the
 * principal compoenets. The covariance matrix has a dimension of p x p. Since
 * number of pixels in any image being typically very high the eigen
 * decomposition becomes computationally expensive. The inner product on the
 * other hand has the dimension of t x t, where t is typically much smaller
 * that p. Hence the eigen decomposition (most compute intensive part) is an
 * orders of magnitude faster.
 *
 * The Update() function enables the calculation of the various models, creates
 * the membership function objects and populates them.
 *
 * \sa itk::ImagePCAShapeModelEstimator
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT InnerProductPCAImageFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef InnerProductPCAImageFilter                         Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(InnerProductPCAImageFilter, ImageToImageFilter);

  /** Template parameters typedefs */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  typedef EstimateInnerProductPCAImageFilter<InputImageType, OutputImageType>   EstimatePCAFilterType;
  typedef typename EstimatePCAFilterType::Pointer                               EstimatePCAFilterPointer;
  typedef NormalizeInnerProductPCAImageFilter<OutputImageType, OutputImageType> NormalizePCAFilterType;
  typedef typename NormalizePCAFilterType::Pointer                              NormalizePCAFilterPointer;

  /** Template parameters typedefs for mean component generation */
  typedef Image<typename InputImageType::InternalPixelType> InternalImageType;
  typedef Functor::MeanFunctor<typename InputImageType::PixelType, typename InternalImageType::PixelType>
  MeanFunctorType;
  typedef itk::UnaryFunctorImageFilter<InputImageType, InternalImageType, MeanFunctorType> MeanFilterType;
  typedef typename MeanFilterType::Pointer                                                 MeanFilterPointer;
  typedef ImageToVectorImageCastFilter<InternalImageType, OutputImageType>                 CastFilterType;
  typedef typename CastFilterType::Pointer                                                 CastFilterPointer;
  typedef ConcatenateVectorImageFilter<OutputImageType, OutputImageType, OutputImageType>  ConcatenateFilterType;
  typedef typename ConcatenateFilterType::Pointer                                          ConcatenateFilterPointer;

  /** Set/Get the number of required largest principal components. The filter produces
   the required number of principal components plus one outputs. Output index 0 represents
   the mean image and the remaining outputs the requested principal components.*/
  itkSetMacro(NumberOfPrincipalComponentsRequired, unsigned int);
  itkGetMacro(NumberOfPrincipalComponentsRequired, unsigned int);

  /** Enable/Disable the generation of the mean component output */
  itkSetMacro(GenerateMeanComponent, bool);
  itkGetMacro(GenerateMeanComponent, bool);
  itkBooleanMacro(GenerateMeanComponent);

  /** Enable/Disable center data */
  itkSetMacro(CenterData, bool);
  itkGetMacro(CenterData, bool);
  itkBooleanMacro(CenterData);

  /** Get the NormalizeInnerProductPCAImage filter */
  otbSetObjectMemberMacro(NormalizePCAFilter, UseUnbiasedEstimator, bool);
  otbGetObjectMemberMacro(NormalizePCAFilter, UseUnbiasedEstimator, bool);

protected:
  /** GenerateData */
  virtual void GenerateData(void);
  /** Constructor */
  InnerProductPCAImageFilter();
  /** Destructor */
  virtual ~InnerProductPCAImageFilter() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** NormalizeInnerProductPCAImageFilter can produce an image which is a different
   * resolution than its input image.  As such, NormalizeInnerProductPCAImageFilter
   * needs to provide an implementation for
   * GenerateOutputInformation() in order to inform the pipeline
   * execution model.  The original documentation of this method is
   * below.
   *
   * \sa ProcessObject::GenerateOutputInformaton()  */
  virtual void GenerateOutputInformation();

private:
  InnerProductPCAImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** The number of largest principal components  */
  unsigned int m_NumberOfPrincipalComponentsRequired;

  /** Radius of the structuring element */
  EstimatePCAFilterPointer m_EstimatePCAFilter;

  /** Opening filter */
  NormalizePCAFilterPointer m_NormalizePCAFilter;

  /** Enable/Disable center data */
  bool m_CenterData;

  /** Flag to specify the generation or not of the mean component output */
  bool m_GenerateMeanComponent;

  /** Internals filters use to generate mean component output */
  MeanFilterPointer        m_MeanFilter;
  CastFilterPointer        m_CastFilter;
  ConcatenateFilterPointer m_ConcatenateFilter;
};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbInnerProductPCAImageFilter.txx"
#endif

#endif
