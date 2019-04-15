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

#ifndef otbNDVIDataNodeFeatureFunction_h
#define otbNDVIDataNodeFeatureFunction_h

#include "otbDataNodeImageFunction.h"

#include "otbVegetationIndicesFunctor.h"
#include "otbPolyLineImageConstIterator.h"

namespace otb
{
/** \class NDVIDataNodeFeatureFunction
  * \brief Compute a NDVI based feature alongside a
  * datanode.
  *
  * This function compute a feature alongside a datanode.
  * The feature is (NDVI >= threshold) per cent along the
  * tested datanode.
  *
  * \ingroup Functions
  * \sa DataNodeImageFunction
  * \sa SpectralAngleDataNodeFeatureFunction
 *
 * \ingroup OTBIndices
  */

template <
class TImage,
class TCoordRep = double,
class TPrecision = double
>
class ITK_EXPORT NDVIDataNodeFeatureFunction :
    public DataNodeImageFunction<TImage, std::vector<TPrecision>, TCoordRep, TPrecision>
{
public:
  /** Standard class typedefs. */
  typedef NDVIDataNodeFeatureFunction                         Self;
  typedef DataNodeImageFunction<TImage, std::vector<TPrecision>, TCoordRep, TPrecision>
                                                              Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(NDVIDataNodeFeatureFunction, DataNodeImageFunction);

  /** Some typedefs. */
  typedef typename Superclass::DataNodeType           DataNodeType;
  typedef typename DataNodeType::LineType             LineType;

  typedef TImage                                      InputImageType;
  typedef typename InputImageType::ConstPointer       InputImageConstPointer;
  typedef typename InputImageType::PixelType          PixelType;
  typedef typename InputImageType::InternalPixelType  ScalarRealType;
  typedef typename InputImageType::IndexType          IndexType;
  typedef typename InputImageType::IndexValueType     IndexValueType;

  typedef TCoordRep                                   CoordRepType;

  typedef TPrecision                                  PrecisionType;

  typedef PolyLineImageConstIterator<InputImageType, LineType>
                                                      ImageLineIteratorType;

  typedef Functor::NDVI<ScalarRealType, ScalarRealType>
                                                      NDVIFunctorType;

  typedef std::vector<PrecisionType>                  OutputType;

  OutputType Evaluate( const DataNodeType& node ) const override;

  /** Set/Get methods */
  itkGetConstMacro(NDVIThreshold, PrecisionType);
  itkSetMacro(NDVIThreshold, PrecisionType);

  //TODO replace by metadata parsing
  unsigned int GetREDChannelIndex() const
  {
    return m_NDVIFunctor.GetBandIndex(CommonBandNames::RED);
  }

  void SetREDChannelIndex(unsigned int id)
  {
    m_NDVIFunctor.SetBandIndex(CommonBandNames::RED,id);
  }

  unsigned int GetNIRChannelIndex() const
  {
    return m_NDVIFunctor.GetBandIndex(CommonBandNames::NIR);
  }

  void SetNIRChannelIndex(unsigned int id)
  {
    m_NDVIFunctor.SetBandIndex(CommonBandNames::NIR,id);
  }

protected:
  NDVIDataNodeFeatureFunction();
  ~NDVIDataNodeFeatureFunction() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  NDVIDataNodeFeatureFunction(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** NDVI Threshold & Functor */
  PrecisionType               m_NDVIThreshold;
  NDVIFunctorType             m_NDVIFunctor;
};

}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNDVIDataNodeFeatureFunction.hxx"
#endif

#endif
