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
  typedef GenericRSResampleImageFilter                          Self;
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
    
  /** The Deformation field spacing & size */
  void SetDeformationFieldSpacing(const SpacingType & spacing)
  {
    m_Resampler->SetDeformationFieldSpacing(spacing);
    this->Modified();
  }
  otbGetObjectMemberConstReferenceMacro(Resampler,DeformationFieldSpacing,SpacingType);
  
  /** The resampled image parameters */
  // Output Origin
  void SetOutputOrigin(const OriginType & origin)
  {
    m_Resampler->SetOutputOrigin(origin);
    this->Modified();
  }
  otbGetObjectMemberConstReferenceMacro(Resampler,OutputOrigin,OriginType);
  
  // Output Start index
  otbSetObjectMemberMacro(Resampler,OutputStartIndex,IndexType);
  otbGetObjectMemberConstReferenceMacro(Resampler,OutputStartIndex,IndexType);
  
  // Output Size
  otbSetObjectMemberMacro(Resampler,OutputSize,SizeType);
  otbGetObjectMemberConstReferenceMacro(Resampler,OutputSize,SizeType);
  
  // Output Spacing
  otbSetObjectMemberMacro(Resampler,OutputSpacing,SpacingType);
  otbGetObjectMemberConstReferenceMacro(Resampler,OutputSpacing,SpacingType);
  

  /** Methods to Set/Get the interpolator */
  void SetInterpolator(InterpolatorType * interpolator)
  {
    m_Resampler->SetInterpolator(interpolator);
    this->Modified();
  }
  otbGetObjectMemberConstMacro(Resampler, Interpolator, const InterpolatorType *);
  
  /** Set/Get for input and output projections.  */
  itkSetStringMacro(InputProjectionRef);
  itkGetStringMacro(InputProjectionRef);
  
  itkSetStringMacro(OutputProjectionRef);
  itkGetStringMacro(OutputProjectionRef);

  /** Set/Get Input Keywordlist*/
  void SetInputKeywordList(const ImageKeywordlist& kwl)
  {
    m_Transform->SetOutputKeywordList(kwl);
    this->Modified();
  }
  otbGetObjectMemberConstMacro(Transform,InputKeywordList,ImageKeywordlist);
  
  /** Set/Get output Keywordlist*/
  void SetOutputKeywordList(const ImageKeywordlist& kwl)
  {
    m_Transform->SetInputKeywordList(kwl);
    this->Modified();
  }
  otbGetObjectMemberConstMacro(Transform,OutputKeywordList,ImageKeywordlist);
  
  /** Set/Get the DEMDirectory*/
    void SetDEMDirectory(const std::string&  dem)
  {
    m_Transform->SetDEMDirectory(dem);
    this->Modified();
  }
  otbGetObjectMemberConstMacro(Transform,DEMDirectory,std::string);
  
  void SetOutputParametersFromImage(const InputImageType * image);
  
protected:
  GenericRSResampleImageFilter();
  /** Destructor */
  virtual ~GenericRSResampleImageFilter() {};

  virtual void GenerateOutputInformation();
  
  virtual void GenerateInputRequestedRegion();

  // Method to instanciate the Generic RS transform
  void UpdateTransform();

private:
  GenericRSResampleImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  // GenericRSTransform Parameters
  std::string          m_InputProjectionRef;
  std::string          m_OutputProjectionRef;

  // Filters pointers
  ResamplerPointerType                   m_Resampler;
  GenericRSTransformPointerType          m_Transform;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGenericRSResampleImageFilter.txx"
#endif

#endif
