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

#ifndef otbVectorizationPathListFilter_h
#define otbVectorizationPathListFilter_h

#include "otbPathListSource.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodIterator.h"
#include "itkConstantBoundaryCondition.h"
#include "otbImage.h"

namespace otb
{
/** \class VectorizationPathListFilter
 *  \brief This filter performs a vectorization from a line detector modulus and direction outputs.
 *
 *  The output of this filter is an otb::ObjectList<TOutputPath>.
 *
 *  This filter performs vectorization at non-grid position by using the barycenter of the few pixels with
 *  non-null intensity in the given direction. A threshold can be set to tune the sensibility of the first point
 *  detection. Path with less than three vertices are not kept by the filter.
 *
 * \ingroup PathListSource
 *
 * \ingroup OTBPath
 */
template <class TInputModulus, class TInputDirection, class TOutputPath>
class ITK_EXPORT VectorizationPathListFilter
  : public PathListSource<TOutputPath>
{
public:
  /** Standard typedefs */
  typedef VectorizationPathListFilter   Self;
  typedef PathListSource<TOutputPath>   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(VectorizationPathListFilter, PathListSource);

  /** Template parameters typedefs */
  typedef TInputModulus                           InputModulusType;
  typedef typename InputModulusType::ConstPointer InputModulusConstPointerType;
  typedef typename InputModulusType::PixelType    InputPixelType;
  typedef typename InputModulusType::PointType    PointType;
  typedef typename InputModulusType::IndexType    IndexType;

  typedef TInputDirection                              InputDirectionType;
  typedef typename InputDirectionType::ConstPointer    InputDirectionConstPointerType;
  typedef TOutputPath                                  OutputPathType;
  typedef typename OutputPathType::Pointer             OutputPathPointerType;
  typedef typename OutputPathType::ContinuousIndexType VertexType;

  /** Derived typedefs */
  typedef typename Superclass::OutputPathListType        OutputPathListType;
  typedef typename Superclass::OutputPathListPointerType OutputPathListPointerType;

  /** Set/get the input modulus */
  using Superclass::SetInput;
  void SetInput(InputModulusType * inputModulus);
  const InputModulusType * GetInput(void);

  /** Set/get the input direction */
  void SetInputDirection(InputDirectionType * inputDirection);
  const InputDirectionType * GetInputDirection(void);

  itkSetMacro(AmplitudeThreshold, InputPixelType);
  itkGetMacro(AmplitudeThreshold, InputPixelType);

protected:

  /** Other internal useful typedefs */
  typedef otb::Image<bool, InputModulusType::ImageDimension> FlagImageType;
  typedef typename FlagImageType::Pointer                    FlagImagePointerType;

  typedef itk::ImageRegionConstIterator<InputModulusType>   ModRegionIteratorType;
  typedef itk::ImageRegionConstIterator<InputDirectionType> DirRegionIteratorType;
  typedef itk::ImageRegionIterator<FlagImageType>           FlagRegionIteratorType;

  typedef itk::ConstantBoundaryCondition<InputModulusType>   ModBCType;
  typedef itk::ConstantBoundaryCondition<InputDirectionType> DirBCType;
  typedef itk::ConstantBoundaryCondition<FlagImageType>      FlagBCType;

  typedef itk::ConstNeighborhoodIterator<InputModulusType, ModBCType>   ModNeighborhoodIteratorType;
  typedef itk::ConstNeighborhoodIterator<InputDirectionType, ModBCType> DirNeighborhoodIteratorType;
  typedef itk::NeighborhoodIterator<FlagImageType, FlagBCType>          FlagNeighborhoodIteratorType;

  typedef typename ModNeighborhoodIteratorType::RadiusType RadiusType;
  typedef typename ModNeighborhoodIteratorType::OffsetType OffsetType;
  typedef std::vector<OffsetType>                          OffsetVectorType;

  typedef typename OutputPathType::VertexListType VertexListType;
  typedef typename VertexListType::ConstPointer   VertexListPointerType;
  typedef typename VertexListType::ConstIterator  VertexIteratorType;

  /** Constructor */
  VectorizationPathListFilter();
  /** Destructor */
  ~VectorizationPathListFilter() override {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  /** Main computation method */
  void GenerateData(void) override;
  /**
   * Compute a vector of the 8  neighbors to explore from the direction and the type of search (forward or backward).
   * \param direction The direction
   * \param flagReverse The type of search
   * \return The neighborhood vector
   */
  OffsetVectorType GetEightNeighborOffsetFromDirection(double direction, unsigned int flagReverse);
  /**
   * Compute a vector of the 3  neighbors to explore from the direction and the type of search (forward or backward).
   * \param direction The direction
   * \param flagReverse The type of search
   * \return The neighborhood vector
   */
  OffsetVectorType GetThreeNeighborOffsetFromDirection(double direction, unsigned int flagReverse);
private:
  VectorizationPathListFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
  /** Amplitude threshold to start following a path */
  InputPixelType m_AmplitudeThreshold;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorizationPathListFilter.hxx"
#endif

#endif
