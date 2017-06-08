/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#include "otbVectorDataFileReader.h"

#include "otbWrapperParameter.h"
#include "otbObjectList.h"

namespace otb
{
namespace Wrapper
{
/** \class InputVectorDataListParameter
 *  \brief This class represents a list of VectorData parameter
 *
 * \ingroup OTBApplicationEngine
 */

class OTBApplicationEngine_EXPORT InputVectorDataListParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef InputVectorDataListParameter           Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef otb::VectorDataFileReader<VectorDataType>  VectorDataFileReaderType;
  typedef otb::ObjectList<VectorDataFileReaderType>  VectorDataFileReaderListType;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(InputVectorDataListParameter, Parameter);

  /** Set image form a list of filename */
  bool SetListFromFileName(const std::vector<std::string> & filenames);

  /** Add null element to lists. */
  void AddNullElement();

  /** Add an image from a filename */
  bool AddFromFileName(const std::string & filename);

  /** Set one specific stored image filename. */
  bool SetNthFileName( const unsigned int id, const std::string & filename );


  /** Get the stored image filename list */
  std::vector<std::string> GetFileNameList() const;

 /** Get one specific stored image filename. */
  std::string GetNthFileName( unsigned int i ) const;

  /** Get one list of the stored image. */
  VectorDataListType* GetVectorDataList() const;

  /** Get one specific stored image. */
  VectorDataType* GetNthVectorData(unsigned int i) const;

  /** Set the list of image. */
  void SetVectorDataList(VectorDataListType* vdList);

  /** Add an image to the list. */
  void AddVectorData(VectorDataType* image);

  bool HasValue() const override;


  /** Erase one element of the list. */
  void Erase( unsigned int id );

 /** Clear all the list. */
  void ClearValue() override;


protected:
  /** Constructor */
  InputVectorDataListParameter();

  /** Destructor */
  ~InputVectorDataListParameter() override;

  VectorDataListType::Pointer m_VectorDataList;
  VectorDataFileReaderListType::Pointer  m_ReaderList;

private:
  InputVectorDataListParameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

}; // End class InputVectorDataList Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
