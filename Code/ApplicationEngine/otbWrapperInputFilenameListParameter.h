/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbWrapperInputFilenameListParameter_h
#define __otbWrapperInputFilenameListParameter_h

#include <string>
#include "otbWrapperStringParameter.h"

#include "otbWrapperParameter.h"
#include "otbObjectList.h"

namespace otb
{
namespace Wrapper
{
/** \class InputFilenameListParameter
 *  \brief This class represents a list of InputFilename parameter
 */

class ITK_EXPORT InputFilenameListParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef InputFilenameListParameter    Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef otb::ObjectList<StringParameter>  StringParameterListType;
  typedef StringParameterListType*          StringParameterListPointerType;


  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(InputFilenameListParameter, Parameter);

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

  bool HasValue() const;


  /** Erase one element of the list. */
  void Erase( unsigned int id );

 /** Clear all the list. */
  virtual void ClearValue();


protected:
  /** Constructor */
  InputFilenameListParameter();

  /** Destructor */
  virtual ~InputFilenameListParameter();



  StringParameterListType::Pointer  m_FilenameList;

private:
  InputFilenameListParameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

}; // End class InputFilenameList Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
