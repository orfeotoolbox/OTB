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
#ifndef __otbMorphologicalPyramidResampler_h
#define __otbMorphologicalPyramidResampler_h

#include "itkImageToImageFilter.h"

namespace otb
{
namespace MorphologicalPyramid
{
/**
 * \class Resampler
 * \brief This class performs the resampling of the given image to the given size.
 *
 * This class performs the resampling of the given image to the given size.It is used
 * in the MorphologicalPyramidAnalyseFilter and MorphologicalPyramidSynthesisFilter.
 *
 * The resampling is done by the itk::ResampleImageFilter, templated with the interpolator
 * itk::LinearInterpolateImageFunction and the transform itk::ScaleTransform.
 *
 * \sa MorphologicalPyramidAnalyseFilter
 * \sa MorphologicalPyramidSynthesisFilter
 * \sa ResampleImageFilter
 * \sa LinearInterpolateImageFunction
 * \sa ScaleTransform
 *
 * \ingroup OTBMorphologicalPyramid
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT Resampler
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef Resampler                                          Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;
  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Type macro */
  itkTypeMacro(Resampler, ImageToImageFilter);
  /** Template parameters typedefs */
  typedef TInputImage                           InputImageType;
  typedef TOutputImage                          OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointer;
  typedef typename InputImageType::Pointer      InputImagePointer;
  typedef typename OutputImageType::RegionType  OutputImageRegionType;
  typedef typename TInputImage::PixelType       PixelType;
  typedef typename InputImageType::ConstPointer InputImageConstPointer;
  typedef typename OutputImageType::SizeType    SizeType;
  typedef typename OutputImageType::SpacingType SpacingType;
  /** Size parameter accessor */
  itkSetMacro(Size, SizeType);
  itkGetMacro(Size, SizeType);

protected:
  /** Constructor */
  Resampler();
  /** Destructor */
  ~Resampler() ITK_OVERRIDE {}
  /** Generate output information */
  void GenerateOutputInformation() ITK_OVERRIDE;
  /** Configure input requested region to be the largest possible region */
  void GenerateInputRequestedRegion() ITK_OVERRIDE;
  /** Configure output requested region to be the largest possible region */
  void EnlargeOutputRequestedRegion(itk::DataObject * itkNotUsed(output)) ITK_OVERRIDE;
  /** Main computation method */
  void GenerateData() ITK_OVERRIDE;
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;
private:
  Resampler(const Self &); // purposely not implemented
  void operator =(const Self&); // purposely not implemented
  /// Output image size
  SizeType m_Size;
};
} // End namespace MorphologicalPyramid
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMorphologicalPyramidResampler.txx"
#endif

#endif
