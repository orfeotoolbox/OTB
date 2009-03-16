/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryAttributeOpeningImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBinaryAttributeOpeningImageFilter_h
#define __itkBinaryAttributeOpeningImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkLabelMap.h"
#include "itkBinaryImageToLabelMapFilter.h"
#include "itkAttributeOpeningLabelMapFilter.h"
#include "itkLabelMapToBinaryImageFilter.h"
#include "itkAttributeLabelObject.h"


namespace itk {

/** \class BinaryAttributeOpeningImageFilter
 * \brief remove the objects in a binary image according to a generic attribute
 *
 * BinaryAttributeOpeningImageFilter remove the objects in a binary image
 * with an attribute value smaller or greater than a threshold called Lambda.
 * The attribute is valued with and attribute valuator given in template parameter
 * and is accessed with an attribute accessor also given in template parameter.
 *
 * Used in combination with AttributeLabelObject, and a specilized attribute valuator,
 * this class is the most efficient way to perform an attribute opening in a binary
 * image.
 *
 * \author Gaëtan Lehmann. Biologie du Développement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa AttributeLabelObject, InPlaceLabelMapFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage, class TLabelObject, class TLabelObjectValuator, class TAttributeAccessor=
    typename Functor::AttributeLabelObjectAccessor< typename TImage::LabelObjectType > >
class ITK_EXPORT BinaryAttributeOpeningImageFilter : 
    public ImageToImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef BinaryAttributeOpeningImageFilter Self;
  typedef ImageToImageFilter<TInputImage, TInputImage>
  Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage InputImageType;
  typedef TInputImage OutputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::RegionType      InputImageRegionType;
  typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::ConstPointer   OutputImageConstPointer;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;
  typedef typename OutputImageType::PixelType      OutputImagePixelType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  typedef TLabelObject LabelObjectType;
  typedef typename itk::LabelMap< LabelObjectType > LabelMapType;
  typedef typename itk::BinaryImageToLabelMapFilter< InputImageType, LabelMapType > LabelizerType;
  typedef TLabelObjectValuator LabelObjectValuatorType;
  typedef TAttributeAccessor AttributeAccessorType;
  typedef typename AttributeAccessorType::AttributeValueType AttributeValueType;
  typedef typename itk::AttributeOpeningLabelMapFilter< LabelMapType, AttributeAccessorType > OpeningType;
  typedef typename itk::LabelMapToBinaryImageFilter< LabelMapType, OutputImageType > BinarizerType;

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(BinaryAttributeOpeningImageFilter, 
               ImageToImageFilter);

  /**
   * Set/Get whether the connected components are defined strictly by
   * face connectivity or by face+edge+vertex connectivity.  Default is
   * FullyConnectedOff.  For objects that are 1 pixel wide, use
   * FullyConnectedOn.
   */
  itkSetMacro(FullyConnected, bool);
  itkGetConstReferenceMacro(FullyConnected, bool);
  itkBooleanMacro(FullyConnected);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputEqualityComparableCheck,
    (Concept::EqualityComparable<InputImagePixelType>));
  itkConceptMacro(IntConvertibleToInputCheck,
    (Concept::Convertible<int, InputImagePixelType>));
  itkConceptMacro(InputOStreamWritableCheck,
    (Concept::OStreamWritable<InputImagePixelType>));
  /** End concept checking */
#endif

  /**
   * Set/Get the value used as "background" in the output image.
   * Defaults to NumericTraits<PixelType>::NonpositiveMin().
   */
  itkSetMacro(BackgroundValue, OutputImagePixelType);
  itkGetConstMacro(BackgroundValue, OutputImagePixelType);

  /**
   * Set/Get the value used as "foreground" in the output image.
   * Defaults to NumericTraits<PixelType>::max().
   */
  itkSetMacro(ForegroundValue, OutputImagePixelType);
  itkGetConstMacro(ForegroundValue, OutputImagePixelType);

  /**
   * Set/Get the threshold used to keep or remove the objects.
   */
  itkGetConstMacro(Lambda, AttributeValueType);
  itkSetMacro(Lambda, AttributeValueType);

  /**
   * Set/Get the ordering of the objects. By default, the objects with
   * an attribute value smaller than Lamba are removed. Turning ReverseOrdering
   * to true make this filter remove the object with an attribute value greater
   * than Lambda instead.
   */
  itkGetConstMacro( ReverseOrdering, bool );
  itkSetMacro( ReverseOrdering, bool );
  itkBooleanMacro( ReverseOrdering );

protected:
  BinaryAttributeOpeningImageFilter();
  ~BinaryAttributeOpeningImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** BinaryAttributeOpeningImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() ;

  /** BinaryAttributeOpeningImageFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));
  
  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  
  virtual void CustomizeInternalFilters( LabelizerType *, LabelObjectValuatorType *, OpeningType *, BinarizerType* ) {};


private:
  BinaryAttributeOpeningImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool                m_FullyConnected;
  OutputImagePixelType m_BackgroundValue;
  OutputImagePixelType m_ForegroundValue;
  AttributeValueType m_Lambda;
  bool m_ReverseOrdering;
} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBinaryAttributeOpeningImageFilter.txx"
#endif

#endif


