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

#ifndef otbVectorDataToSpecificDescriptionFilterBase_h
#define otbVectorDataToSpecificDescriptionFilterBase_h

#include "otbVectorDataToVectorDataFilter.h"

namespace otb
{
/** \class VectorDataToSpecificDescriptionFilterBase
  * \brief Base class for VectorDataToSpecificDescriptionFilters.
  *
  * VectorDataToSpecificDescriptionFilterBase is a base class, it is
  * supposed to be used through a inheriting class.
  * Input and output are both VectorDatas. Each data node of the
  * output VectorData contains its score regarding to the associated
  * descriptors.
  *
  *
  * \ingroup VectorDataFilter
  * \sa VectorDataToRoadDescriptionFilter
  * \sa VectorDataToBuildingDescriptionFilter
 *
 * \ingroup OTBVectorDataManipulation
 */

template <class TVectorData>
class ITK_EXPORT VectorDataToSpecificDescriptionFilterBase :
  public VectorDataToVectorDataFilter<TVectorData, TVectorData>
{
public:
  /** Standard class typedefs. */
  typedef VectorDataToSpecificDescriptionFilterBase    Self;
  typedef VectorDataToVectorDataFilter<TVectorData,
    TVectorData>                                       Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataToSpecificDescriptionFilterBase, VectorDataToVectorDataFilter);

  /** Some typedefs. */
  typedef TVectorData                             VectorDataType;
  typedef typename VectorDataType::Pointer        VectorDataPointer;
  typedef typename VectorDataType::DataNodeType   DataNodeType;

  typedef itk::DataObject::Pointer DataObjectPointer;
  typedef itk::DataObject          DataObject;

  /** Support accessors. */
  virtual void AddSupport(const itk::DataObject * support, unsigned int idx);
  const itk::DataObject * GetSupport(unsigned int idx);

protected:
  /** Constructor */
  VectorDataToSpecificDescriptionFilterBase();
  /** Destructor */
  ~VectorDataToSpecificDescriptionFilterBase() override {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  VectorDataToSpecificDescriptionFilterBase(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataToSpecificDescriptionFilterBase.hxx"
#endif

#endif
