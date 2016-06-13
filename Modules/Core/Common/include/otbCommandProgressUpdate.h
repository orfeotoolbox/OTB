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
#ifndef otbCommandProgressUpdate_h
#define otbCommandProgressUpdate_h

#include "itkCommand.h"
#include <sstream>
#include <iostream>

namespace otb
{

/**
* \class CommandProgressUpdate
* \brief TODO
*
 *
 * \ingroup OTBCommon
*/
template <class TFilter>
class CommandProgressUpdate : public itk::Command
{
public:
  typedef  CommandProgressUpdate   Self;
  typedef  itk::Command            Superclass;
  typedef  itk::SmartPointer<Self> Pointer;
  itkNewMacro(Self);
protected:
  CommandProgressUpdate() {}
public:
  typedef   const TFilter * FilterPointer;

  void Execute(itk::Object *caller, const itk::EventObject& event) ITK_OVERRIDE;

  void Execute(const itk::Object * object, const itk::EventObject& event) ITK_OVERRIDE;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbCommandProgressUpdate.txx"
#endif

#endif
