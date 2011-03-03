/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkConnectedComponentFunctorImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2008-10-13 18:54:27 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkConnectedComponentFunctorImageFilter_h
#define __itkConnectedComponentFunctorImageFilter_h

#include "itkConnectedComponentImageFilter.h"
#include "itkImage.h"

namespace itk
{

/**
 * \class ConnectedComponentFunctorImageFilter
 * \brief A generic connected components filter that labels the
 * objects in an artibitrary image.
 *
 * ConnectedComponentFunctorImageFilter labels the objects in an arbitrary
 * image. Each distinct object is assigned a unique label. The filter makes
 * three passes through the image.  The first pass initializes the
 * output.  The second pass labels each foreground pixel such that all
 * the pixels associated with an object either have the same label or
 * have had their labels entered into a equivalency table.  The third
 * pass through the image flattens the equivalency table such that all
 * pixels for an object have the same label.
 *
 * The functor specifies the criteria to join neighboring pixels.  For
 * example a simple intensity threshold difference might be used for
 * scalar imagery.
 *
 * The final object labels are in no particular order (and some object
 * labels may not be used on the final objects).  You can reorder the
 * labels such that object labels are consecutive and sorted based on
 * object size by passing the output of this filter to a
 * RelabelComponentImageFilter. 
 *
 * \sa ImageToImageFilter
 */

template <class TInputImage, class TOutputImage, class TFunctor, class TMaskImage=TInputImage>
class ITK_EXPORT ConnectedComponentFunctorImageFilter : 
    public ImageToImageFilter< TInputImage, TOutputImage > 
{
public:
  /**
   * Standard "Self" & Superclass typedef.
   */
  typedef ConnectedComponentFunctorImageFilter Self;
  typedef ImageToImageFilter< TInputImage, TOutputImage > Superclass;

  /**
   * Types from the Superclass
   */
  typedef typename Superclass::InputImagePointer InputImagePointer;

  /**
   * Extract some information from the image types.  Dimensionality
   * of the two images is assumed to be the same.
   */
  typedef typename TOutputImage::PixelType          OutputPixelType;
  typedef typename TOutputImage::InternalPixelType  OutputInternalPixelType;
  typedef typename TInputImage::PixelType           InputPixelType;
  typedef typename TInputImage::InternalPixelType   InputInternalPixelType;
  typedef typename TMaskImage::PixelType            MaskPixelType;
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  
  /**
   * Image typedef support
   */
  typedef TFunctor     FunctorType;
  typedef TInputImage  InputImageType;
  typedef TMaskImage   MaskImageType;
  typedef TOutputImage OutputImageType;

  typedef   typename TInputImage::IndexType       IndexType;
  typedef   typename TInputImage::SizeType        SizeType;
  typedef   typename TOutputImage::RegionType     RegionType;
  typedef   std::list<IndexType>                  ListType;

  typedef typename MaskImageType::Pointer MaskImagePointer;

  /** 
   * Smart pointer typedef support 
   */
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  /**
   * Run-time type information (and related methods)
   */
  itkTypeMacro(ConnectedComponentFunctorImageFilter, ImageToImageFilter);
  
  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);

  /** Get the functor object.  The functor is returned by reference.
   * (Functors do not have to derive from itk::LightObject, so they do
   * not necessarily have a reference count. So we cannot return a
   * SmartPointer.) */
  FunctorType& GetFunctor() { return m_Functor; };
  const FunctorType& GetFunctor() const { return m_Functor; }

  /** Set the functor object.  This replaces the current Functor with a
   * copy of the specified Functor. This allows the user to specify a
   * functor that has ivars set differently than the default functor.
   * This method requires an operator!=() be defined on the functor
   * (or the compiler's default implementation of operator!=() being
   * appropriate). */
  void SetFunctor(const FunctorType& functor)
    {
    m_Functor = functor;
    this->Modified();
    }

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(SameDimensionCheck,
    (Concept::SameDimension<InputImageDimension, ImageDimension>));
  itkConceptMacro(InputEqualityComparableCheck,
    (Concept::EqualityComparable<InputPixelType>));
  itkConceptMacro(OutputEqualityComparableCheck,
    (Concept::EqualityComparable<OutputPixelType>));
  itkConceptMacro(OutputConvertibleToUnsignedIntCheck,
    (Concept::Convertible<OutputPixelType, unsigned int>));
  itkConceptMacro(OutputConvertibleToUnsignedLongCheck,
    (Concept::Convertible<OutputPixelType, unsigned long>));
  itkConceptMacro(OutputConvertibleToLongCheck,
    (Concept::Convertible<OutputPixelType, long>));
  itkConceptMacro(UnsignedLongConvertibleToOutputCheck,
    (Concept::Convertible<unsigned long, OutputPixelType>));
  itkConceptMacro(OutputIncrementDecrementOperatorsCheck,
    (Concept::IncrementDecrementOperators<OutputPixelType>));
  /** End concept checking */
#endif
  
  
  /**
   * Set/Get whether the connected components are defined strictly by
   * face connectivity or by face+edge+vertex connectivity.  Default is
   * FullyConnectedOff.  For objects that are 1 pixel wide, use
   * FullyConnectedOn.
   */
  itkSetMacro(FullyConnected, bool);
  itkGetConstReferenceMacro(FullyConnected, bool);
  itkBooleanMacro(FullyConnected);

  void SetMaskImage(TMaskImage* mask)
  {
    this->SetNthInput(1, const_cast<TMaskImage *>( mask ));
  }

  const TMaskImage* GetMaskImage() const
  {
    return (static_cast<const TMaskImage*>(this->ProcessObject::GetInput(1)));
  }

protected:
  ConnectedComponentFunctorImageFilter() 
    {
    m_FullyConnected = false;
    }
  virtual ~ConnectedComponentFunctorImageFilter() {}
  ConnectedComponentFunctorImageFilter(const Self&) {}

  FunctorType m_Functor;

  /**
   * Standard pipeline method. 
   */
  void GenerateData();

  bool m_FullyConnected;


};
  
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkConnectedComponentFunctorImageFilter.txx"
#endif

#endif
