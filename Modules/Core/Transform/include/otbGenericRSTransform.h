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

#ifndef otbGenericRSTransform_h
#define otbGenericRSTransform_h

#include "otbCompositeTransform.h"
#include <string>

namespace otb
{
namespace Projection
{
enum TransformAccuracy
{
  UNKNOWN,
  ESTIMATE,
  PRECISE
};
}
/** \class GenericRSTransform
 *  \brief This is the class to handle generic remote sensing transform
 *
 * The transformation will transform the coordinate from any map projection or sensor
 * model to any map projection or sensor model.
 *
 * If one of the projection (output or input) is a map projection, it can be
 * specified using the WKT or the EPSG code.
 *
 * \ingroup Projection
 *
 *
 * \ingroup OTBTransform
 **/

template <class TScalarType = double, unsigned int NInputDimensions = 2,
          unsigned int NOutputDimensions = 2>
class ITK_EXPORT       GenericRSTransform : public otb::Transform<TScalarType, // Data type for scalars
                                                            NInputDimensions,  // Number of dimensions in the input space
                                                            NOutputDimensions> // Number of dimensions in the output space
{
public:
  /**\name Standard ITK typedefs */
  //@{
  typedef otb::Transform<TScalarType, NInputDimensions, NOutputDimensions> Superclass;
  typedef GenericRSTransform            Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  //@}

  typedef typename Superclass::ScalarType   ScalarType;
  typedef typename Superclass::JacobianType JacobianType;
  typedef itk::Point<ScalarType, NInputDimensions>  InputPointType;
  typedef itk::Point<ScalarType, NOutputDimensions> OutputPointType;

  typedef itk::Vector<double, 2> SpacingType;
  typedef itk::Point<double, 2>  OriginType;

  typedef itk::Transform<double, NInputDimensions, NOutputDimensions> GenericTransformType;
  typedef typename GenericTransformType::Pointer GenericTransformPointerType;
  typedef otb::CompositeTransform<GenericTransformType, GenericTransformType> TransformType;
  typedef typename TransformType::Pointer TransformPointerType;

  typedef typename Superclass::InverseTransformBasePointer InverseTransformBasePointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GenericRSTransform, Transform);

  typedef TransformDirection::TransformationDirection DirectionOfMappingEnumType;

  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
  itkStaticConstMacro(SpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(ParametersDimension, unsigned int, NInputDimensions*(NInputDimensions + 1));

  /** Set/Get for input and output projections.  */
  itkSetStringMacro(InputProjectionRef);
  itkGetStringMacro(InputProjectionRef);

  itkSetStringMacro(OutputProjectionRef);
  itkGetStringMacro(OutputProjectionRef);

  /** Set/Get Dictionary*/
  const itk::MetaDataDictionary& GetInputDictionary() const
  {
    return m_InputDictionary;
  }

  void SetInputDictionary(const itk::MetaDataDictionary& dictionary)
  {
    m_InputDictionary = dictionary;
    this->Modified();
  }

  const itk::MetaDataDictionary& GetOutputDictionary() const
  {
    return m_OutputDictionary;
  }

  void SetOutputDictionary(const itk::MetaDataDictionary& dictionary)
  {
    m_OutputDictionary = dictionary;
    this->Modified();
  }

  /** Set/Get Keywordlist*/

  itkGetMacro(InputKeywordList, ImageKeywordlist);
  void SetInputKeywordList(const ImageKeywordlist& kwl)
  {
    this->m_InputKeywordList = kwl;
    this->Modified();
  }

  itkGetMacro(OutputKeywordList, ImageKeywordlist);
  void SetOutputKeywordList(const ImageKeywordlist& kwl)
  {
    this->m_OutputKeywordList = kwl;
    this->Modified();
  }

  /** Set the origin of the vector data.
  * \sa GetOrigin() */
  itkSetMacro(InputOrigin, OriginType);
  itkGetConstReferenceMacro(InputOrigin, OriginType);

  /** Set the spacing (size of a pixel) of the vector data.
    * \sa GetSignedSpacing() */
  itkSetMacro(InputSpacing, SpacingType);
  itkGetConstReferenceMacro(InputSpacing, SpacingType);

  /** Set the origin of the vector data.
  * \sa GetOrigin() */
  itkSetMacro(OutputOrigin, OriginType);
  itkGetConstReferenceMacro(OutputOrigin, OriginType);

  /** Set the spacing (size of a pixel) of the vector data.
  * \sa GetSignedSpacing() */
  itkSetMacro(OutputSpacing, SpacingType);
  itkGetConstReferenceMacro(OutputSpacing, SpacingType);

  /** Check if the transform is up to date */
  virtual bool IsUpToDate()
  {
    return m_TransformUpToDate;
  }

  /** Get Transform accuracy */
  itkGetMacro(TransformAccuracy, Projection::TransformAccuracy);

  /** Methods prototypes */
  virtual const TransformType* GetTransform() const;

  OutputPointType TransformPoint(const InputPointType& point) const override;

  virtual void InstantiateTransform();

  // Get inverse methods
  bool GetInverse(Self* inverseTransform) const;
  InverseTransformBasePointer GetInverseTransform() const override;

  // Dummy set parameter method
  void SetParameters(const typename Superclass::ParametersType&) override
  {
  }

  // Dummy ComputeJacobianWithRespectToParameters method
  void ComputeJacobianWithRespectToParameters(const InputPointType&, JacobianType&) const override
  {
  }

protected:
  GenericRSTransform();
  ~GenericRSTransform() override
  {
  }

  void Modified() const override
  {
    this->Superclass::Modified();
    m_TransformUpToDate = false;
  }

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  GenericRSTransform(const Self&) = delete;
  void operator=(const Self&) = delete;

  ImageKeywordlist m_InputKeywordList;
  ImageKeywordlist m_OutputKeywordList;

  itk::MetaDataDictionary m_InputDictionary;
  itk::MetaDataDictionary m_OutputDictionary;

  std::string m_InputProjectionRef;
  std::string m_OutputProjectionRef;

  SpacingType m_InputSpacing;
  OriginType  m_InputOrigin;
  SpacingType m_OutputSpacing;
  OriginType  m_OutputOrigin;

  TransformPointerType m_Transform;

  GenericTransformPointerType   m_InputTransform;
  GenericTransformPointerType   m_OutputTransform;
  mutable bool                  m_TransformUpToDate;
  Projection::TransformAccuracy m_TransformAccuracy;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGenericRSTransform.hxx"
#endif

#endif
