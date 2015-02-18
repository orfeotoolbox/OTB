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
#ifndef __otbTextureImageFunction_h
#define __otbTextureImageFunction_h

#include "itkImageFunction.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkVariableLengthVector.h"

namespace otb
{

/**
 * \class TextureImageFunction
 * \brief Calculate a texture over an image.
 *
 * This class is templated with the input image and the Functor
 * that will compute the texture.
 *
 * \ingroup ImageFunctions
   * \ingroup Textures
 */
template <class TInputImage, class TFunctor, class TCoordRep = float>
class ITK_EXPORT TextureImageFunction :
  public itk::ImageFunction<TInputImage, typename itk::NumericTraits<typename TInputImage::PixelType>::RealType,
      TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef TextureImageFunction Self;
  typedef itk::ImageFunction<TInputImage, typename itk::NumericTraits<typename TInputImage::PixelType>::RealType,
      TCoordRep> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(TextureImageFunction, itk::ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** typedef support. */
  typedef TInputImage                                                               InputImageType;
  typedef TFunctor                                                                  FunctorType;
  typedef typename InputImageType::OffsetType                                       OffsetType;
  typedef typename InputImageType::SizeType                                         SizeType;
  typedef typename InputImageType::PixelType                                        PixelType;
  typedef typename Superclass::OutputType                                           OutputType;
  typedef typename Superclass::IndexType                                            IndexType;
  typedef typename Superclass::ContinuousIndexType                                  ContinuousIndexType;
  typedef typename Superclass::PointType                                            PointType;
  typedef itk::ConstNeighborhoodIterator<InputImageType>                            IteratorType;
  typedef typename IteratorType::NeighborhoodType                                   NeighborhoodType;
  typedef typename itk::NumericTraits<typename InputImageType::PixelType>::RealType RealType;

  /** Dimension of the underlying image. */
  itkStaticConstMacro(ImageDimension, unsigned int, InputImageType::ImageDimension);

  /** Evalulate the function at specified index */
  virtual RealType EvaluateAtIndex(const IndexType& index) const;

  /** Evaluate the function at non-integer positions */
  virtual RealType Evaluate(const PointType& point) const
  {
    IndexType index;
    this->ConvertPointToNearestIndex(point, index);
    return this->EvaluateAtIndex(index);
  }
  virtual RealType EvaluateAtContinuousIndex(
    const ContinuousIndexType& cindex) const
  {
    IndexType index;
    this->ConvertContinuousIndexToNearestIndex(cindex, index);
    return this->EvaluateAtIndex(index);
  }

  /** Get/Set the radius of the neighborhood over which the
      statistics are evaluated */
  void SetRadius(SizeType& rad)
  {
    m_Radius = rad;
    this->Modified();
  }

  void SetOffset(OffsetType& off)
  {
    m_Offset = off;
    this->Modified();
  }

  itkGetMacro(Radius, SizeType);
  itkGetMacro(Offset, OffsetType);

  /** Return the texture name */
  virtual std::string GetName() const
  {
    FunctorType funct;
    return funct.GetName();
  }

protected:
  TextureImageFunction();
  virtual ~TextureImageFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  TextureImageFunction(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

  SizeType   m_Radius;
  OffsetType m_Offset;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
# include "otbTextureImageFunction.txx"
#endif

#endif
