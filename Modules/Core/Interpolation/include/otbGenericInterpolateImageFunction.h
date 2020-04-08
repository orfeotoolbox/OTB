/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbGenericInterpolateImageFunction_h
#define otbGenericInterpolateImageFunction_h

#include "itkInterpolateImageFunction.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkConstantBoundaryCondition.h"

namespace otb
{

/** \class GenericInterpolateImageFunction
 * \brief Generic interpolation of an otb::Image.
 *
 * GenericInterpolateImageFunction interpolates image intensity according to a
 * resampling profil.
 *
 * The Initialize() method need to be call to create the filter.
 *
 * \ingroup ImageFunctions ImageInterpolators
 *
 * \ingroup OTBInterpolation
 */
template <class TInputImage, class TFunction, class TBoundaryCondition = itk::ZeroFluxNeumannBoundaryCondition<TInputImage>, class TCoordRep = double>
class ITK_EXPORT GenericInterpolateImageFunction : public itk::InterpolateImageFunction<TInputImage, TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef GenericInterpolateImageFunction Self;
  typedef itk::InterpolateImageFunction<TInputImage, TCoordRep> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(GenericInterpolateImageFunction, itk::InterpolateImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Input and output images typedef definition. */
  typedef typename Superclass::OutputType     OutputType;
  typedef typename Superclass::InputImageType InputImageType;

  /** Dimension underlying input image. */
  // itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

  /** Index and typedef support. */
  typedef typename Superclass::IndexType    IndexType;
  typedef typename InputImageType::SizeType SizeType;
  typedef typename Superclass::RealType     RealType;
  typedef TFunction                         FunctionType;
  typedef itk::ConstNeighborhoodIterator<InputImageType, TBoundaryCondition> IteratorType;

  /** ContinuousIndex typedef support. */
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;

  /** Dimension underlying input image. */
  itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

  /** Evaluate the function at a ContinuousIndex position
   *
   * Returns the interpolated image intensity at a
   * specified point position. No bounds checking is done.
   * The point is assume to lie within the image buffer.
   *
   * ImageFunction::IsInsideBuffer() can be used to check bounds before
   * calling the method. */
  OutputType EvaluateAtContinuousIndex(const ContinuousIndexType& index) const override;

  /** Set/Get the window radius*/
  virtual void SetRadius(unsigned int rad);
  virtual unsigned int GetRadius() const
  {
    return m_Function.GetRadius();
  }
  // unsigned int GetRadius() { return this->GetFunction().GetRadius(); };

  /** Set/Get the window radius*/
  // Don't have to be used here, just declared for the inheritance classes.
  // virtual void SetWindowSize(unsigned int win){ m_WindowSize = win; };

  /** Get the functor list */
  virtual FunctionType& GetFunction(void)
  {
    return m_Function;
  }

  /** Initialize tables: need to be call explicitly */
  virtual void Initialize();

  /** Weights normalization accessors*/
  itkSetMacro(NormalizeWeight, bool);
  itkGetMacro(NormalizeWeight, bool);

protected:
  GenericInterpolateImageFunction();
  ~GenericInterpolateImageFunction() override;
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Call the superclass implementation and set the TablesHaveBeenGenerated
   * flag to false */
  void Modified(void) const override;

  /** Delete tables.*/
  virtual void ResetOffsetTable();
  /** Initialize used tables*/
  virtual void InitializeTables();
  /** Fill the weight offset table*/
  virtual void FillWeightOffsetTable();

private:
  GenericInterpolateImageFunction(const Self&) = delete;
  void operator=(const Self&) = delete;
  /** Store the window radius. */
  // unsigned int m_Radius;
  // Constant to store twice the radius
  unsigned int m_WindowSize;

  /** Used function */
  FunctionType m_Function;
  /** Store the image dimension.*/
  unsigned int m_ImageDimension;

  /** These members are declared mutable so that they can be
  regenerated seamlessly inside the EvaluateAtContinuousIndex method if
  they need to */
  /** Size of the offset table */
  mutable unsigned int m_OffsetTableSize;
  /** The offset array, used to keep a list of relevant
   * offsets in the neihborhoodIterator */
  mutable unsigned int* m_OffsetTable;
  /** Index into the weights array for each offset */
  mutable unsigned int** m_WeightOffsetTable;
  /** True if internal statistics have been generated */
  mutable bool m_TablesHaveBeenGenerated;
  /** Weights normalization */
  bool m_NormalizeWeight;
};

} // end namespace itk

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGenericInterpolateImageFunction.hxx"
#endif

#endif
