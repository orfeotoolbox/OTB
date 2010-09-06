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
#ifndef __otbOptResampleImageFilter_h
#define __otbOptResampleImageFilter_h

#include "itkImageToImageFilter.h"
#include "otbStreamingWarpImageFilter.h"
#include "itkTransformToDeformationFieldSource.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkInterpolateImageFunction.h"

namespace otb
{

/** \class OptResampleImageFilter
 *  \brief This class is a composite filter resampling an input image
 *         by setting a transform. The filter computes a deformation
 *         grid using the transform set and used it to warp the input.
 *
 * The otb::OptResampleImageFilter allows to resample a
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

template <class TInputImage, class TOutputImage, class TDeformationField>
class ITK_EXPORT OptResampleImageFilter :
    public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef OptResampleImageFilter                                Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage>    Superclass;
  typedef itk::SmartPointer<Self>                               Pointer;
  typedef itk::SmartPointer<const Self>                         ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OptResampleImageFilter,itk::ImageToImageFilter);

  /** Typedef parameters*/
  typedef TInputImage           InputImageType;
  typedef TOutputImage          OutputImageType;
  typedef TDeformationField     DeformationFieldType;

  /** filter warping input image using deformation field */
  typedef StreamingWarpImageFilter<InputImageType, OutputImageType, DeformationFieldType>  WarpImageFilterType;
  
  /** Internal filters typedefs*/
  typedef itk::TransformToDeformationFieldSource<DeformationFieldType, double>      DeformationFieldGeneratorType;
  typedef typename DeformationFieldGeneratorType::Pointer             DeformationFieldGeneratorPointerType;
  typedef typename DeformationFieldGeneratorType::TransformType       TransformType;
  typedef typename DeformationFieldGeneratorType::SizeType            SizeType;
  typedef typename DeformationFieldGeneratorType::SpacingType         SpacingType;
  typedef typename DeformationFieldGeneratorType::OriginType          OriginType;
  typedef typename DeformationFieldGeneratorType::IndexType           IndexType;
  typedef typename DeformationFieldGeneratorType::RegionType          RegionType;

  /** Interpolator type */
  typedef itk::InterpolateImageFunction<InputImageType,double>         InterpolatorType;
  typedef typename InterpolatorType::Pointer                           InterpolatorPointerType;
  typedef itk::LinearInterpolateImageFunction<InputImageType,double>   DefaultInterpolatorType;
  
  /** Public Method prototypes */
  virtual void GenerateData();
  
  /** Accessors to internal filters parameters */
  itkSetObjectMacro(Transform,TransformType);

  /** The Deformation field spacing & size */
  itkSetMacro(DeformationFieldSpacing,SpacingType);
  itkSetMacro(DeformationGridSize,SizeType);
  
  /** The resampled image parameters */
  // Output Origin
  itkSetMacro(OutputOrigin,OriginType);
  itkGetMacro(OutputOrigin,OriginType);
  // Output Start index
  itkSetMacro(OutputIndex,IndexType);
  itkGetMacro(OutputIndex,IndexType);
  // Output Size
  itkSetMacro(OutputSize,SizeType);
  itkGetMacro(OutputSize,SizeType);
  // Output Spacing
  itkSetMacro(OutputSpacing,SpacingType);
  itkGetMacro(OutputSpacing,SpacingType);

  /** Get the deformation grid generated */
  virtual DeformationFieldType * GetDeformationField();
  
  /** Methods to Set/Get the interpolator */
  itkSetObjectMacro( Interpolator, InterpolatorType );
  itkGetObjectMacro( Interpolator, InterpolatorType );

  virtual void GenerateOutputInformation();
  
  virtual void GenerateInputRequestedRegion();

  void SetOutputParametersFromImage(const InputImageType * image);
  
protected:
  OptResampleImageFilter();
  /** Destructor */
  virtual ~OptResampleImageFilter() {};

private:
  OptResampleImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  DeformationFieldGeneratorPointerType   m_DeformationFilter;
  typename TransformType::Pointer        m_Transform;

  SizeType                               m_DeformationGridSize;
  SpacingType                            m_DeformationFieldSpacing;
  SizeType                               m_OutputSize;
  OriginType                             m_OutputOrigin;
  IndexType                              m_OutputIndex;
  SpacingType                            m_OutputSpacing;
  
  typename WarpImageFilterType::Pointer  m_WarpFilter;
  InterpolatorPointerType                m_Interpolator;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOptResampleImageFilter.txx"
#endif

#endif
