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
#ifndef __otbPanTexTextureImageFunction_h
#define __otbPanTexTextureImageFunction_h

#include "itkNumericTraits.h"
#include "itkVariableLengthVector.h"
#include "itkConstNeighborhoodIterator.h"
#include "otbPanTexTextureFunctor.h"
#include "otbTextureImageFunction.h"
#include "otbFunctionWithNeighborhoodToImageFilter.h"

namespace otb
{

/**
 * \class PanTexTextureImageFunction
 * \brief Calculate a texture over an image.
 *
 * This class is templated with the input image and the Functor
 * that will compute the texture.
 *
 * \ingroup ImageFunctions
 */

template <class TInputImage, class TOutputImage, class TCoordRep = float >
class ITK_EXPORT PanTexTextureImageFunctionFilter : 
public FunctionWithNeighborhoodToImageFilter< TInputImage, TOutputImage, 
                                              TextureImageFunction<TInputImage, ITK_TYPENAME Functor::PanTexTextureFunctor< ITK_TYPENAME itk::ConstNeighborhoodIterator<TInputImage>, itk::VariableLengthVector<double> >, TCoordRep  >      >

{
  public:
  /** Standard class typedefs. */
  typedef PanTexTextureImageFunctionFilter Self;
  typedef FunctionWithNeighborhoodToImageFilter< TInputImage, TOutputImage, 
                                              TextureImageFunction<TInputImage, typename Functor::PanTexTextureFunctor< typename itk::ConstNeighborhoodIterator<TInputImage>, itk::VariableLengthVector<double> >, TCoordRep  >      >  Superclass; 
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(PanTexTextureImageFunctionFilter, TextureImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** typedef support. */
  typedef TInputImage                              InputImageType;
  typedef TOutputImage                              OutputImageType;
  //typedef TFunctor                                 FunctorType;
  typedef typename InputImageType::OffsetType      OffsetType;
  typedef typename InputImageType::SizeType        SizeType;
  /*
  typedef typename InputImageType::PixelType       PixelType;
  typedef typename Superclass::OutputType          OutputType;
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::PointType           PointType;
  typedef itk::ConstNeighborhoodIterator<InputImageType>                             IteratorType;
  typedef typename IteratorType::NeighborhoodType                                    NeighborhoodType;
  typedef typename itk::NumericTraits<typename InputImageType::PixelType>::RealType  RealType;
  */

  /** Dimension of the underlying image. */
  itkStaticConstMacro(ImageDimension, unsigned int,InputImageType::ImageDimension);


  /** Evalulate the function at specified index */
  //virtual RealType EvaluateAtIndex( const IndexType& index ) const;

  /** Evaluate the function at non-integer positions */
 /*  virtual RealType Evaluate( const PointType& point ) const */
/*   { */
/*     IndexType index; */
/*     this->ConvertPointToNearestIndex( point, index ); */
/*     return this->EvaluateAtIndex( index ); */
/*   }; */
/*   virtual RealType EvaluateAtContinuousIndex( */
/*     const ContinuousIndexType& cindex ) const */
/*   { */
/*     IndexType index; */
/*     this->ConvertContinuousIndexToNearestIndex( cindex, index ); */
/*     return this->EvaluateAtIndex( index ) ; */
/*   }; */
  

protected:
  PanTexTextureImageFunctionFilter()
  {
    OffsetType off;
    off.Fill(2);
    this->SetOffset(off);
    SizeType radius;
    radius.Fill(4);
    this->SetRadius(radius);
    
  };
  ~PanTexTextureImageFunctionFilter() {};
  //void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  PanTexTextureImageFunctionFilter( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  virtual void SetOffset( OffsetType off )
  {
    Superclass::SetOffset(off);
  }
};

} // end namespace otb



#endif

