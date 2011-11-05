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
#if SWIGPYTHON

 %{
#include "itkPyCommand.h"
typedef itk::PyCommand          itkPyCommand;
typedef itk::PyCommand::Pointer itkPyCommand_Pointer;
 %}


class itkPyCommand : public itkCommand
{
public:
  static itkPyCommand_Pointer New();
  virtual char const * GetNameOfClass() const;

  void SetCommandCallable(PyObject *obj);
  PyObject * GetCommandCallable();
  void Execute(itkObject *, const itkEventObject&);
protected:
  itkPyCommand();
  //~itkPyCommand();
};
DECLARE_REF_COUNT_CLASS( itkPyCommand )

#endif
