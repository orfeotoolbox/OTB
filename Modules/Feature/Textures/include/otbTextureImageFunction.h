/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbTextureImageFunction_h
#define otbTextureImageFunction_h

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
 *
 * \ingroup OTBTextures
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
  RealType EvaluateAtIndex(const IndexType& index) const override;

  /** Evaluate the function at non-integer positions */
  RealType Evaluate(const PointType& point) const override
  {
    IndexType index;
    this->ConvertPointToNearestIndex(point, index);
    return this->EvaluateAtIndex(index);
  }
  RealType EvaluateAtContinuousIndex(
    const ContinuousIndexType& cindex) const override
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
  ~TextureImageFunction() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  TextureImageFunction(const Self &) = delete;
  void operator =(const Self&) = delete;

  SizeType   m_Radius;
  OffsetType m_Offset;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
# include "otbTextureImageFunction.hxx"
#endif

#endif
