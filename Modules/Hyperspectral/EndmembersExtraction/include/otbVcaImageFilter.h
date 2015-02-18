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
#ifndef __otbVCAImageFilter_h
#define __otbVCAImageFilter_h

#include "otbImage.h"
#include "itkAbsImageFilter.h"
#include "otbDotProductImageFilter.h"
#include "otbProjectiveProjectionImageFilter.h"
#include "otbVectorImageToMatrixImageFilter.h"
#include "otbStreamingMinMaxImageFilter.h"
#include "otbStreamingStatisticsImageFilter.h"
#include "otbPCAImageFilter.h"
#include "otbVectorImageToAmplitudeImageFilter.h"
#include "otbConcatenateScalarValueImageFilter.h"

#include "itkMersenneTwisterRandomVariateGenerator.h"
#include "vnl/algo/vnl_svd.h"

namespace otb {

/** \class VCAImageFilter
 * \brief Estimates the endmembers present in a hyperspectral image
 *
 * This filter applies the VCA (Vertex Component Analysis) algorithm to a
 * hyperspectral image and outputs the estimated endmembers.
 * The output image of this filter is a one-line VectorImage containing,
 * at each pixel location, one of the estimated endmember.
 *
 * This code is a C++ port of the Matlab implementation
 * available in the Matlab Hyperspectral Toolbox.
 * Most notably it supports streaming and is fully multi-threaded,
 * so it can be run seamlessly on full hyperspectral scenes.
 *
 * References :
 * "Vertex Component Analysis: A Fast Algorithm to Unmix Hyperspectral Data",
 * Jos\'e M. P. Nascimento, and Jos\'e M. Bioucas Dias,
 * IEEE Transactions on Geoscience and Remote Sensing, Vol. 43, No. 4, April 2005
 *
 * \ingroup Hyperspectral
 * \ingroup Streamed
 * \ingroup Threaded
 */
template <class TVectorImage>
class ITK_EXPORT VCAImageFilter : public itk::ImageToImageFilter<TVectorImage, TVectorImage>
{
public:
  typedef VCAImageFilter                                            Self;
  typedef itk::ImageToImageFilter<TVectorImage, TVectorImage>       Superclass;
  typedef itk::SmartPointer<Self>                                   Pointer;
  typedef itk::SmartPointer<const Self>                             ConstPointer;

  typedef TVectorImage                                 VectorImageType;
  typedef typename VectorImageType::IndexType          IndexType;
  typedef typename VectorImageType::SizeType           SizeType;
  typedef typename VectorImageType::RegionType         RegionType;
  typedef typename VectorImageType::PixelType          PixelType;
  typedef typename VectorImageType::InternalPixelType  InternalPixelType;
  typedef InternalPixelType                            PrecisionType;

  typedef otb::Image<InternalPixelType>                ImageType;

  typedef itk::AbsImageFilter<ImageType, ImageType>                                             AbsImageFilterType;
  typedef otb::ProjectiveProjectionImageFilter<VectorImageType, VectorImageType, PrecisionType> ProjectiveProjectionImageFilterType;
  typedef otb::DotProductImageFilter<VectorImageType, ImageType>                                DotProductImageFilterType;
  typedef otb::MatrixImageFilter<VectorImageType, VectorImageType>                              MatrixImageFilterType;
  typedef otb::VectorImageToMatrixImageFilter<VectorImageType>                                  VectorImageToMatrixImageFilterType;
  typedef otb::StreamingMinMaxImageFilter<ImageType>                                            StreamingMinMaxImageFilterType;
  typedef otb::StreamingStatisticsVectorImageFilter<VectorImageType, PrecisionType>             StreamingStatisticsVectorImageFilterType;
  typedef otb::StreamingStatisticsImageFilter<ImageType>                                        StreamingStatisticsImageFilterType;
  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator                                RandomVariateGeneratorType;
  typedef otb::PCAImageFilter< VectorImageType, VectorImageType, otb::Transform::FORWARD >      ForwardPCAImageFilterType;
  typedef otb::PCAImageFilter< VectorImageType, VectorImageType, otb::Transform::INVERSE >      InversePCAImageFilterType;
  typedef otb::VectorImageToAmplitudeImageFilter< VectorImageType, ImageType >                  VectorImageToAmplitudeImageFilterType;
  typedef otb::ConcatenateScalarValueImageFilter< VectorImageType, VectorImageType >            ConcatenateScalarValueImageFilterType;

  // creation of SmartPointer
  itkNewMacro(Self);
  // runtime type information
  itkTypeMacro(VCAImageFilter, itk::ImageToImageFilter);

  itkGetMacro( NumberOfEndmembers, unsigned int );
  itkSetMacro( NumberOfEndmembers, unsigned int );

  virtual void Update()
  {
    this->GenerateData();
  }

  virtual void EnlargeOutputRequestedRegion(itk::DataObject *itkNotUsed(output))
  {
    this->GetOutput()
      ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
  }

protected:
  VCAImageFilter();

  virtual ~VCAImageFilter();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void GenerateOutputInformation();

  virtual void GenerateData();

private:
  VCAImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  unsigned int m_NumberOfEndmembers;
};

} // end namesapce otb

#include "otbVcaImageFilter.txx"

#endif /* __otbVCAImageFilter_h */
