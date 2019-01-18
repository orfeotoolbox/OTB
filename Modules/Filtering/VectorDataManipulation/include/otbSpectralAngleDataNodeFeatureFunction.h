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

#ifndef otbSpectralAngleDataNodeFeatureFunction_h
#define otbSpectralAngleDataNodeFeatureFunction_h

#include "otbDataNodeImageFunction.h"

#include "otbBinarySpectralAngleFunctor.h"
#include "otbPolyLineImageConstIterator.h"
#include "itkLineConstIterator.h"
#include "itkVariableLengthVector.h"

namespace otb
{
/** \class SpectralAngleDataNodeFeatureFunction
  * \brief Compute a spectral angle based feature alongside a
  * datanode.
  *
  * This function compute a spectral angle alongside a datanode.
  * The feature is the mean spectral angle regarding a
  * reference pixel alongside the tested datanode.
  *
  * The output has three elements:
  * - #0: mean spectral angle of a neighborhood around the datanode, weighted to fit in [0, 1] interval
  * - #1: accumulator of the spectral angle alongside the datanode
  * - #2: number of visited pixel
  *
  * \ingroup Functions
  * \sa DataNodeImageFunction
  * \sa NDVIDataNodeFeatureFunction
 *
 * \ingroup OTBVectorDataManipulation
  */

template <
class TImage,
class TCoordRep = double,
class TPrecision = double
>
class ITK_EXPORT SpectralAngleDataNodeFeatureFunction :
    public DataNodeImageFunction<TImage, std::vector<TPrecision>, TCoordRep, TPrecision>
{
public:
  /** Standard class typedefs. */
  typedef SpectralAngleDataNodeFeatureFunction                Self;
  typedef DataNodeImageFunction<TImage, std::vector<TPrecision>, TCoordRep, TPrecision>
                                                              Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SpectralAngleDataNodeFeatureFunction, DataNodeImageFunction);

  /** Some typedefs. */
  typedef typename Superclass::DataNodeType           DataNodeType;
  typedef typename DataNodeType::LineType             LineType;
  typedef typename LineType::Pointer                  LinePointer;
  typedef typename LineType::ContinuousIndexType      ContinuousIndexType;
  typedef typename LineType::VertexListConstIteratorType VertexListConstIteratorType;

  typedef TImage                                      InputImageType;
  typedef typename InputImageType::ConstPointer       InputImageConstPointer;
  typedef typename InputImageType::PixelType          PixelType;
  typedef typename InputImageType::InternalPixelType  ScalarRealType;
  typedef typename InputImageType::IndexType          IndexType;
  typedef typename InputImageType::IndexValueType     IndexValueType;

  typedef TCoordRep                                   CoordRepType;

  typedef TPrecision                                  PrecisionType;

  typedef itk::VariableLengthVector<PrecisionType>    ReferencePixelType;

  typedef PolyLineImageConstIterator<InputImageType, LineType> ImageLineIteratorType;
  typedef itk::LineConstIterator<InputImageType>      LineIteratorType;

  typedef Functor::BinarySpectralAngleFunctor<PixelType, ReferencePixelType, PrecisionType>
                                                      SpectralAngleFunctorType;

  typedef std::pair<IndexType, IndexType>             IndexPairType;
  typedef std::vector<PrecisionType>                  OutputType;

  OutputType Evaluate( const DataNodeType& node ) const override;

  /** Set/Get methods */
  itkGetConstMacro(RefPixel, PixelType);
  itkSetMacro(RefPixel, PixelType);

  itkGetConstMacro(Radius, unsigned int);
  itkSetMacro(Radius, unsigned int);

protected:
  SpectralAngleDataNodeFeatureFunction();
  ~SpectralAngleDataNodeFeatureFunction() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  SpectralAngleDataNodeFeatureFunction(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** SpectralAngle Functor & ReferencePixel*/
  ReferencePixelType          m_RefPixel;
  SpectralAngleFunctorType    m_SpectralAngleFunctor;

  /** Neighborhood radius */
  unsigned int m_Radius;
};

}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSpectralAngleDataNodeFeatureFunction.hxx"
#endif

#endif
