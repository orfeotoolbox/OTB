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
#ifndef __otbCountImageFunction_h
#define __otbCountImageFunction_h

#include "itkImageFunction.h"
#include "itkNumericTraits.h"
#include "itkProcessObject.h"

namespace otb
{

/**
 * \class CountImageFunction
 * \brief Calculate the density in the neighborhood of a pixel
 *
 *
 * \ingroup ImageFunctions
 */
template <class TInputImage, class  TDetector, class  TCount >
 class ITK_EXPORT CountImageFunction :
  public itk::ImageFunction< TInputImage, typename itk::NumericTraits<typename TInputImage::PixelType>::RealType>
{
public:
  /** Standard class typedefs. */
  typedef CountImageFunction                  Self;
  typedef itk::ImageFunction<TInputImage, typename itk::NumericTraits<typename TInputImage::PixelType>::RealType >      
                                              Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(CountImageFunction, itk::ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInputImage InputImageType;

  /** Index typedef support. */
  typedef typename Superclass::IndexType IndexType;
  
  /** ContinuousIndex typedef support. */
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;

  /** Point typedef support. */
  typedef typename Superclass::PointType PointType;
  
  /** Datatype used for the density */
  typedef typename itk::NumericTraits<typename InputImageType::PixelType>::RealType   RealType;

  /** Detector typedef support*/
  typedef TDetector                             DetectorType;
  typedef typename DetectorType::Pointer        DetectorPointerType;

  /** Count Method type*/
  typedef TCount                             CountType; 
  

  /** Dimension of the underlying image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      InputImageType::ImageDimension);



  /** Evalulate the function at specified index */
  virtual RealType EvaluateAtIndex( const IndexType& index );
   virtual RealType EvaluateAtIndex( const IndexType& index ) const;
  /** Evaluate the function at non-integer positions */
  virtual RealType Evaluate( const PointType& point ) const
    { 
      IndexType index;
      this->ConvertPointToNearestIndex( point, index );
      return this->EvaluateAtIndex( index ); 
    }
  virtual RealType EvaluateAtContinuousIndex( 
    const ContinuousIndexType& cindex ) const
    { 
      IndexType index;
      this->ConvertContinuousIndexToNearestIndex( cindex, index );
      return this->EvaluateAtIndex( index ) ; 
    }

  /**SetDetector */
  virtual void SetDetector( DetectorType* detector) ;
  
  /**GetDetector */
  virtual DetectorType * GetDetector() ;

  /** Get/Set the radius of the neighborhood over which the
      statistics are evaluated */
  itkSetMacro( NeighborhoodRadius, unsigned int );
  itkGetConstReferenceMacro( NeighborhoodRadius, unsigned int );
 
protected:
  CountImageFunction();
  ~CountImageFunction(){};
  
  /**Update method*/
  virtual void Modified();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  

private:
  CountImageFunction( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  /** Detector implementation */
  DetectorPointerType m_Detector;

  unsigned int m_NeighborhoodRadius;
  bool m_HasBeenGenerated;
};

} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION 
#include "otbCountImageFunction.txx"
#endif

#endif

