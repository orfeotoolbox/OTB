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

#ifndef otbWrapperAbstractStringListParameter_h
#define otbWrapperAbstractStringListParameter_h

#include <string>
#include "otbWrapperStringParameter.h"

#include "otbObjectList.h"

namespace otb
{
namespace Wrapper
{
/** \class AbstractStringListParameter
 *  \brief This class represents a list of InputFilename parameter
 *
 * \ingroup OTBApplicationEngine
 */

class OTBApplicationEngine_EXPORT AbstractStringListParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef AbstractStringListParameter    Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef otb::ObjectList<StringParameter>  StringParameterListType;
  typedef StringParameterListType*          StringParameterListPointerType;


  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(AbstractStringListParameter, Parameter);

  /** Set file form a list of filenames */
  bool SetListFromFileName(const std::vector<std::string> & filenames);

  /** Add null element to lists. */
  void AddNullElement();

  /** Add a filename from a filename */
  bool AddFromFileName(const std::string & filename);

  /** Set one specific stored filename. */
  bool SetNthFileName( const unsigned int id, const std::string & filename );

  /** Get the stored filename list */
  std::vector<std::string> GetFileNameList() const;

 /** Get one specific stored filename. */
  std::string GetNthFileName( unsigned int i ) const;

  /** Get one list of the stored files. */
  StringParameterListPointerType GetFileList() const;

  bool HasValue() const ITK_OVERRIDE;


  /** Erase one element of the list. */
  void Erase( unsigned int id );

 /** Clear all the list. */
  void ClearValue() ITK_OVERRIDE;


protected:
  /** Constructor */
  AbstractStringListParameter();

  /** Destructor */
  ~AbstractStringListParameter() ITK_OVERRIDE;


  StringParameterListType::Pointer  m_FilenameList;

private:
  AbstractStringListParameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

}; // End class InputFilenameList Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
