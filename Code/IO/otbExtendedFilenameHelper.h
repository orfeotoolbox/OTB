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
#ifndef __otbExtendedFilenameHelper_h
#define __otbExtendedFilenameHelper_h

#include "itkObject.h"
#include "itkObjectFactory.h"

namespace otb
{

/** \class ExtendedFilenameHelper
 *  \brief Helper to handle extended filenames.
 *
 *  \sa ImageFileReader
 */

class ITK_EXPORT ExtendedFilenameHelper : public itk::Object
{
public:
/** Standard class typedefs. */
  typedef ExtendedFilenameHelper        Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef itk::Object                   Superclass;

  itkTypeMacro(ExtendedFilenameHelper, itk::Object);
  itkNewMacro(Self);

  typedef std::map< std::string, std::string > OptionMapType;

  void SetExtendedFileName(const char * extFname);
  std::string GetExtendedFileName(void) const;
  std::string GetSimpleFileName(void) const;
  OptionMapType GetOptionMap(void) const;

protected:
  ExtendedFilenameHelper() {}
  virtual ~ExtendedFilenameHelper() {}

private:
  ExtendedFilenameHelper(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

  void SetSimpleFileName(const std::string & fname);

  std::string                          m_ExtendedFilename;
  std::string                          m_SimpleFilename;
  OptionMapType                        m_OptionMap;

};
} // end namespace otb

#endif // __otbExtendedFilenameHelper_h
