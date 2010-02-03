/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelReconstructionLabelMapFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelReconstructionLabelMapFilter_h
#define __itkLabelReconstructionLabelMapFilter_h

#include "itkInPlaceLabelMapFilter.h"
#include "itkAttributeLabelObject.h"

namespace itk {
/** \class LabelReconstructionLabelMapFilter
 * \brief 
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TImage, class TMarkerImage, class TAttributeAccessor=
  typename Functor::AttributeLabelObjectAccessor< typename TImage::LabelObjectType > >
class ITK_EXPORT LabelReconstructionLabelMapFilter : 
    public InPlaceLabelMapFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef LabelReconstructionLabelMapFilter Self;
  typedef InPlaceLabelMapFilter<TImage>
  Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TImage ImageType;
  typedef typename ImageType::Pointer         ImagePointer;
  typedef typename ImageType::ConstPointer    ImageConstPointer;
  typedef typename ImageType::PixelType       PixelType;
  typedef typename ImageType::IndexType       IndexType;
  typedef typename ImageType::LabelObjectType LabelObjectType;
  
  typedef TMarkerImage MarkerImageType;
  typedef typename MarkerImageType::Pointer         MarkerImagePointer;
  typedef typename MarkerImageType::ConstPointer    MarkerImageConstPointer;
  typedef typename MarkerImageType::PixelType       MarkerImagePixelType;
  
  typedef TAttributeAccessor AttributeAccessorType;

  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(LabelReconstructionLabelMapFilter, 
               InPlaceLabelMapFilter);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
/*  itkConceptMacro(InputEqualityComparableCheck,
    (Concept::EqualityComparable<PixelType>));
  itkConceptMacro(IntConvertibleToInputCheck,
    (Concept::Convertible<int, PixelType>));
  itkConceptMacro(InputOStreamWritableCheck,
    (Concept::OStreamWritable<PixelType>));*/
  /** End concept checking */
#endif

   /** Set the marker image */
  void SetMarkerImage(TMarkerImage *input)
     {
     // Process object is not const-correct so the const casting is required.
     this->SetNthInput( 1, const_cast<TMarkerImage *>(input) );
     }

  /** Get the marker image */
  MarkerImageType * GetMarkerImage()
    {
    return static_cast<MarkerImageType*>(const_cast<DataObject *>(this->ProcessObject::GetInput(1)));
    }

   /** Set the input image */
  void SetInput1(TImage *input)
     {
     this->SetInput( input );
     }

   /** Set the marker image */
  void SetInput2(TMarkerImage *input)
     {
     this->SetMarkerImage( input );
     }

protected:
  LabelReconstructionLabelMapFilter();
  ~LabelReconstructionLabelMapFilter() {};

  virtual void ThreadedGenerateData( LabelObjectType * labelObject );

private:
  LabelReconstructionLabelMapFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLabelReconstructionLabelMapFilter.txx"
#endif

#endif


