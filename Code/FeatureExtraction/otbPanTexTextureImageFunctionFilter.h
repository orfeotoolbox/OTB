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
      TextureImageFunction<TInputImage, ITK_TYPENAME Functor::PanTexTextureFunctor< ITK_TYPENAME TInputImage::PixelType, ITK_TYPENAME TOutputImage::PixelType>, TCoordRep  > >

{
public:
  /** Standard class typedefs. */
  typedef PanTexTextureImageFunctionFilter Self;
  typedef FunctionWithNeighborhoodToImageFilter< TInputImage, TOutputImage,
  TextureImageFunction<TInputImage, typename Functor::PanTexTextureFunctor< typename TInputImage::PixelType, typename TOutputImage::PixelType >, TCoordRep  > >  Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(PanTexTextureImageFunctionFilter, TextureImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** typedef support. */
  typedef TInputImage                          InputImageType;
  typedef TOutputImage                         OutputImageType;
  typedef typename InputImageType::OffsetType  OffsetType;
  typedef typename InputImageType::SizeType    SizeType;

  /** Dimension of the underlying image. */
  itkStaticConstMacro(ImageDimension, unsigned int,InputImageType::ImageDimension);


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

