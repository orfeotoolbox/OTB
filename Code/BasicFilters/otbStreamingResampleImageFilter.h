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
#ifndef __otbStreamingResampleImageFilter_h
#define __otbStreamingResampleImageFilter_h

#include "itkImageToImageFilter.h"
#include "otbStreamingWarpImageFilter.h"
#include "itkTransformToDeformationFieldSource.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkInterpolateImageFunction.h"
#include "otbImage.h"
#include "itkVector.h"

#include "otbMacro.h"

namespace otb
{

/** \class StreamingResampleImageFilter
 *  \brief This class is a composite filter resampling an input image
 *         by setting a transform. The filter computes a deformation
 *         grid using the transform set and used it to warp the input.
 *
 * The otb::StreamingResampleImageFilter allows to resample a
 * otb::VectorImage using a transformation set with SetTransform()
 * method. First, a deformation grid, with a spacing m_DeformationGridSpacing
 * and a size relative to this spacing, is built. Then, the image is
 * wraped using this deformation grid. The size (SetOuputSize()), the
 * spacing (SetOuputSpacing()), the start index (SetOutputIndex()) and
 * the  interpolator (SetInterpolator()) and the origin (SetOrigin())
 * can be set using the method between brackets.
 *
 *
 *
 * \ingroup Projection
 *
 **/

template <class TInputImage, class TOutputImage,
          class TInterpolatorPrecisionType = double>
class ITK_EXPORT StreamingResampleImageFilter :
    public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef StreamingResampleImageFilter                                Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage>    Superclass;
  typedef itk::SmartPointer<Self>                               Pointer;
  typedef itk::SmartPointer<const Self>                         ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(StreamingResampleImageFilter, itk::ImageToImageFilter);

  /** Typedef parameters*/
  typedef TInputImage                InputImageType;
  typedef TOutputImage               OutputImageType;
  
  /** Deformation field used to warp the image*/
  typedef itk::Vector<double,
                      TOutputImage::ImageDimension>             DeformationType;
  typedef otb::Image<DeformationType>                           DeformationFieldType;

  /** filter warping input image using deformation field */
  typedef StreamingWarpImageFilter<InputImageType,
                                   OutputImageType,
                                   DeformationFieldType>        WarpImageFilterType;
  
  /** Internal filters typedefs*/
  typedef itk::TransformToDeformationFieldSource<DeformationFieldType,
                                                 double>        DeformationFieldGeneratorType;
  typedef typename DeformationFieldGeneratorType::TransformType TransformType;
  typedef typename DeformationFieldGeneratorType::SizeType      SizeType;
  typedef typename DeformationFieldGeneratorType::SpacingType   SpacingType;
  typedef typename DeformationFieldGeneratorType::OriginType    OriginType;
  typedef typename DeformationFieldGeneratorType::IndexType     IndexType;
  typedef typename DeformationFieldGeneratorType::RegionType    RegionType;

  /** Interpolator type */
  typedef itk::InterpolateImageFunction<InputImageType,
                                        TInterpolatorPrecisionType>       InterpolatorType;
  typedef typename InterpolatorType::Pointer                              InterpolatorPointerType;
  typedef itk::LinearInterpolateImageFunction<InputImageType,
                                              TInterpolatorPrecisionType> DefaultInterpolatorType;

  /** ImageBase typedef */
  typedef itk::ImageBase<OutputImageType::ImageDimension>     ImageBaseType;
  
  
  /** Accessors to internal filters parameters */
  void SetTransform(TransformType * transform)
  {
    m_DeformationFilter->SetTransform(transform);
    this->Modified();
  }
  otbGetObjectMemberConstMacro(DeformationFilter, Transform, const TransformType*);

  /** The Deformation field spacing & size */
  void SetDeformationFieldSpacing(const SpacingType & spacing)
  {
    m_DeformationFilter->SetOutputSpacing(spacing);
    this->Modified();
  }
  const SpacingType & GetDeformationFieldSpacing() const
  {
   return m_DeformationFilter->GetOutputSpacing();
  }

  /** The resampled image parameters */
  // Output Origin
  void SetOutputOrigin(const OriginType & origin)
  {
    m_DeformationFilter->SetOutputOrigin(origin);
    m_WarpFilter->SetOutputOrigin(origin);
    this->Modified();
  }
  otbGetObjectMemberConstReferenceMacro(WarpFilter, OutputOrigin, OriginType);

  // Output Start index
  otbSetObjectMemberMacro(WarpFilter, OutputStartIndex, IndexType);
  otbGetObjectMemberConstReferenceMacro(WarpFilter, OutputStartIndex, IndexType);

  // Output Size
  otbSetObjectMemberMacro(WarpFilter, OutputSize, SizeType);
  otbGetObjectMemberConstReferenceMacro(WarpFilter, OutputSize, SizeType);

  // Output Spacing
  otbSetObjectMemberMacro(WarpFilter, OutputSpacing, SpacingType);
  otbGetObjectMemberConstReferenceMacro(WarpFilter, OutputSpacing, SpacingType);
  
  /** Methods to Set/Get the interpolator */
  void SetInterpolator(InterpolatorType * interpolator)
  {
    m_WarpFilter->SetInterpolator(interpolator);
    this->Modified();
  }
  otbGetObjectMemberConstMacro(WarpFilter, Interpolator, const InterpolatorType *);

  /** Default Edgepadding value */
  otbSetObjectMemberMacro(WarpFilter,
                          EdgePaddingValue,
                          typename OutputImageType::PixelType);
  otbGetObjectMemberMacro(WarpFilter,
                                        EdgePaddingValue,
                                        typename OutputImageType::PixelType);

  /** Import output parameters from a given image */
  void SetOutputParametersFromImage(const ImageBaseType * image);

  /* Set number of threads for Deformation field generator*/
  void SetDeformationFilterNumberOfThreads(unsigned int nbThread)
  {
    m_DeformationFilter->SetNumberOfThreads(nbThread);
  }

protected:
  StreamingResampleImageFilter();

  /** Destructor */
  virtual ~StreamingResampleImageFilter() {};

  virtual void GenerateData();

  virtual void GenerateOutputInformation();

  virtual void GenerateInputRequestedRegion();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  StreamingResampleImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  typename DeformationFieldGeneratorType::Pointer   m_DeformationFilter;
  typename WarpImageFilterType::Pointer             m_WarpFilter;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingResampleImageFilter.txx"
#endif

#endif
