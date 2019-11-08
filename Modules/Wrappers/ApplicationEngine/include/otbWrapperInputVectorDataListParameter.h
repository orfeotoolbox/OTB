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

#ifndef otbWrapperInputVectorDataListParameter_h
#define otbWrapperInputVectorDataListParameter_h


#include "otbWrapperInputVectorDataParameter.h"
#include "otbWrapperParameterList.h"


namespace otb
{


namespace Wrapper
{

/** \class InputVectorDataListParameter
 *  \brief This class represents a list of VectorData parameter
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT InputVectorDataListParameter : public ParameterList<InputVectorDataParameter>
{
public:
  /** Standard class typedef */
  typedef InputVectorDataListParameter            Self;
  typedef ParameterList<InputVectorDataParameter> Superclass;
  typedef itk::SmartPointer<Self>                 Pointer;
  typedef itk::SmartPointer<const Self>           ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(InputVectorDataListParameter, ParameterList);

  /** Get one list of the stored image. */
  const VectorDataListType* GetVectorDataList() const;
  VectorDataListType*       GetVectorDataList();

  /** Get one specific stored image. */
  //
  // FIXME: Definition is not const-correct because
  // InputVectorDataParameter::GetVectorData() is not const-correct!
  const VectorDataType* GetNthVectorData(std::size_t);

  /** Set the list of image. */
  void SetVectorDataList(VectorDataListType*);

  /** Add an image to the list. */
  void AddVectorData(VectorDataType*);

  /** Clear all the list. */
  void ClearValue() override;

  /** */
  using StringListInterface::GetDirection;
  virtual Role GetDirection() const override;

  /** */
  using StringListInterface::GetFilenameFilter;
  const std::string& GetFilenameFilter() const override;

  ParameterType GetType() const override
  {
    return ParameterType_InputVectorDataList;
  }

protected:
  /** Constructor */
  InputVectorDataListParameter();

  /** Destructor */
  ~InputVectorDataListParameter() override;

private:
  InputVectorDataListParameter(const Parameter&) = delete;
  void operator=(const Parameter&) = delete;

  InputVectorDataParameter::Pointer FromVectorData(VectorDataType*);

  InputVectorDataParameter::Pointer& FromVectorData(InputVectorDataParameter::Pointer&, VectorDataType*);

  //
  // Private attributes
private:
  VectorDataListType::Pointer m_VectorDataList;

}; // End class InputVectorDataList Parameter

} // End namespace Wrapper

} // End namespace otb

#endif
