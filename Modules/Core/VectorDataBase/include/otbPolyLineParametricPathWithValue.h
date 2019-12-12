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

#ifndef otbPolyLineParametricPathWithValue_h
#define otbPolyLineParametricPathWithValue_h

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "itkPolyLineParametricPath.h"
#pragma GCC diagnostic pop
#else
#include "itkPolyLineParametricPath.h"
#endif

#include "itkMetaDataObject.h"
#include "otbRemoteSensingRegion.h"
#include "otbMacro.h"
#include <string>

namespace otb
{
/** \class PolyLineParametricPathWithValue
 *  \brief This class implement a PolyLineParametricPath for which a value can be set.
 * The value is stored in the itk::MetaDataDictionary.
 * The precision of the value can be set.
 *
 * \sa itk::PolyLineParametricPath.
 *
 * \ingroup OTBVectorDataBase
 */
template <class TValue, unsigned int VDimension = 2>
class ITK_EXPORT PolyLineParametricPathWithValue : public itk::PolyLineParametricPath<VDimension>
{
public:
  /** Standard typedefs */
  typedef PolyLineParametricPathWithValue         Self;
  typedef itk::PolyLineParametricPath<VDimension> Superclass;
  typedef itk::SmartPointer<Self>                 Pointer;
  typedef itk::SmartPointer<const Self>           ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(PolyLineParametricPath, MySuperclass);

  /** Template parameters typedefs */
  typedef TValue ValueType;

  /** Derived typedefs */
  typedef typename Superclass::VertexType          VertexType;
  typedef typename Superclass::VertexListType      VertexListType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename VertexListType::ConstIterator   VertexListConstIteratorType;

  //   typedef itk::ImageRegion<2>                       RegionType;
  typedef otb::RemoteSensingRegion<double> RegionType;
  typedef typename RegionType::SizeType    SizeType;
  typedef typename RegionType::IndexType   IndexType;

  itkGetMacro(Key, std::string);

  void SetValue(ValueType value)
  {
    itk::MetaDataDictionary& dict = this->GetMetaDataDictionary();
    itk::EncapsulateMetaData<ValueType>(dict, m_Key, value);
  }

  ValueType GetValue(void) const
  {
    ValueType                      resp(0);
    const itk::MetaDataDictionary& dict = this->GetMetaDataDictionary();
    if (dict.HasKey(m_Key))
    {
      itk::ExposeMetaData<ValueType>(dict, m_Key, resp);
    }
    else
    {
      itkGenericExceptionMacro(<< "Key " << m_Key << " not found in metadata dictionary !");
    }
    return resp;
  }

  /**
   * Return the path length (perimeter).
   * \return The length.
   */
  virtual double GetLength() const;

  virtual void AddVertex(const ContinuousIndexType& vertex);

  /**
 * Compute the path bounding region.
 * \return The region.
 */
  virtual RegionType GetBoundingRegion(void) const;

protected:
  /** Constructor */
  PolyLineParametricPathWithValue();
  /** Destructor */
  ~PolyLineParametricPathWithValue() override
  {
  }
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  virtual void ComputeLength() const;
  virtual void ComputeBoundingRegion() const;
  void         Modified() const override;

private:
  PolyLineParametricPathWithValue(const Self&) = delete;
  void               operator=(const Self&) = delete;
  std::string        m_Key;
  mutable double     m_Length;
  mutable bool       m_LengthIsValid;
  mutable RegionType m_BoundingRegion;
  mutable bool       m_BoundingRegionIsValid;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPolyLineParametricPathWithValue.hxx"
#endif

#endif
