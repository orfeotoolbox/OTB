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
#ifndef __otbGenericRSResampleImageFilter_h
#define __otbGenericRSResampleImageFilter_h

#include "itkImageToImageFilter.h"
#include "otbOptResampleImageFilter.h"
#include "itkLinearInterpolateImageFunction.h"

#include "otbGenericRSTransform.h"

namespace otb
{

/** \class GenericRSResampleImageFilter
 *  \brief This class is a composite filter 
 *
 * 
 *
 *
 * \ingroup Projection
 *
 **/

template <class TInputImage, class TOutputImage, class TDeormationField>
class ITK_EXPORT GenericRSResampleImageFilter :
    public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef GenericRSResampleImageFilter                                Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage>    Superclass;
  typedef itk::SmartPointer<Self>                               Pointer;
  typedef itk::SmartPointer<const Self>                         ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GenericRSResampleImageFilter,itk::ImageToImageFilter);

  /** Typedef parameters*/
  typedef TInputImage                        InputImageType;
  typedef typename InputImageType::PointType PointType;
  typedef TOutputImage                       OutputImageType;
  typedef TDeormationField                   DeformationFieldType;
  
  /** Internal filters typedefs*/
  typedef OptResampleImageFilter<InputImageType,OutputImageType,DeformationFieldType >    ResamplerType;
  typedef typename ResamplerType::Pointer             ResamplerPointerType;
  typedef typename ResamplerType::TransformType       TransformType;
  typedef typename ResamplerType::SizeType            SizeType;
  typedef typename ResamplerType::SpacingType         SpacingType;
  typedef typename ResamplerType::OriginType          OriginType;
  typedef typename ResamplerType::IndexType           IndexType;
  typedef typename ResamplerType::RegionType          RegionType;
  typedef typename ResamplerType::InterpolatorType    InterpolatorType;
  typedef typename ResamplerType::DefaultInterpolatorType    DefaultInterpolatorType;
  

  /** Specialisation of OptResampleFilter with a remote 
    * sensing  transform 
    */
  typedef GenericRSTransform<>                       GenericRSTransformType;
  typedef typename GenericRSTransformType::Pointer   GenericRSTransformPointerType;
  
  /** Public Method prototypes */
  virtual void GenerateData();
  
  /** Accessors to internal filters parameters */
  itkSetObjectMacro(Transform,GenericRSTransformType);
  itkGetObjectMacro(Transform,GenericRSTransformType);

  /** The Deformation field spacing & size */
  itkSetMacro(DeformationFieldSpacing,SpacingType);
  
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
  
  /** Methods to Set/Get the interpolator */
  itkSetObjectMacro( Interpolator, InterpolatorType );
  itkGetObjectMacro( Interpolator, InterpolatorType );


  /** Set/Get for input and output projections.  */
  itkSetStringMacro(InputProjectionRef);
  itkGetStringMacro(InputProjectionRef);
  
  itkSetStringMacro(OutputProjectionRef);
  itkGetStringMacro(OutputProjectionRef);

  /** Set/Get Keywordlist*/
  itkGetMacro(InputKeywordList, ImageKeywordlist);
  void SetInputKeywordList(const ImageKeywordlist& kwl)
  {
    this->m_InputKeywordList = kwl;
    this->Modified();
  }

  itkGetMacro(OutputKeywordList, ImageKeywordlist);
  void SetOutputKeywordList(const ImageKeywordlist& kwl)
  {
    this->m_OutputKeywordList = kwl;
    this->Modified();
  }
  
  itkSetStringMacro(DEMDirectory);
  itkGetStringMacro(DEMDirectory);
  
  itkSetMacro(AverageElevation, double);
  itkGetMacro(AverageElevation, double);

  virtual void GenerateOutputInformation();
  
  virtual void GenerateInputRequestedRegion();

  void SetOutputParametersFromImage(const InputImageType * image);
  
protected:
  GenericRSResampleImageFilter();
  /** Destructor */
  virtual ~GenericRSResampleImageFilter() {};

  // called in the GenerateInputRequestedRegion cause the transform
  // is needed there and have to be set once.
  void UpdateTransform();

private:
  GenericRSResampleImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  // GenericRSTransform Parameters
  ImageKeywordlist     m_InputKeywordList;
  ImageKeywordlist     m_OutputKeywordList;
  std::string          m_InputProjectionRef;
  std::string          m_OutputProjectionRef;
  std::string          m_DEMDirectory;
  double               m_AverageElevation;
  unsigned int         m_InterpolatorNeighborhoodRadius;

  unsigned int        m_AddedRadius ;
  // Filters pointers
  ResamplerPointerType                   m_Resampler;
  GenericRSTransformPointerType          m_Transform;

  // Output parameters
  SpacingType                            m_DeformationFieldSpacing;
  SizeType                               m_OutputSize;
  OriginType                             m_OutputOrigin;
  IndexType                              m_OutputIndex;
  SpacingType                            m_OutputSpacing;
  
  // smartPointer on the interpolator
  typename InterpolatorType::Pointer     m_Interpolator;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGenericRSResampleImageFilter.txx"
#endif

#endif
