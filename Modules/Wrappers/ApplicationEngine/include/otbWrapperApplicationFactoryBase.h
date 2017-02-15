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
#ifndef otbWrapperApplicationFactoryBase_h
#define otbWrapperApplicationFactoryBase_h

#include "OTBApplicationEngineExport.h"
#include "itkObjectFactoryBase.h"
#include "otbWrapperApplication.h"

namespace otb
{
namespace Wrapper
{

class OTBApplicationEngine_EXPORT ApplicationFactoryBase : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef ApplicationFactoryBase        Self;
  typedef itk::ObjectFactoryBase        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(ApplicationFactoryBase, itk::ObjectFactoryBase);
  
  /** Create an application using the CreateObject() method of the current factory */
  Application::Pointer CreateApplication(const char* name);

protected:
  ApplicationFactoryBase(){}

  ~ApplicationFactoryBase() ITK_OVERRIDE{}

private:
  ApplicationFactoryBase(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace Wrapper
} //end namespace otb

#endif
