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
#ifndef otb_FragmentShader_h
#define otb_FragmentShader_h

#include "itkObject.h"

namespace otb
{

class FragmentShader 
  : public itk::Object
{
public:
  typedef FragmentShader                                  Self;
  typedef itk::Object                                     Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  void LoadShader();

  virtual void SetupShader();

  void UnloadShader();

protected:
  FragmentShader();

  virtual ~FragmentShader();

  void BuildShader();

  virtual std::string GetSource() const = 0;

  virtual std::string GetName() const = 0;

private:
  // prevent implementation
  FragmentShader(const Self&);
  void operator=(const Self&);

}; // End class FragmentShader

} // End namespace otb

#endif
