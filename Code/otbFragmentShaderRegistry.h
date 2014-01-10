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
#ifndef otb_FragmentShaderRegistry_h
#define otb_FragmentShaderRegistry_h

#include "itkObject.h"
#include "itkObjectFactory.h"

namespace otb
{

class FragmentShaderRegistry 
  : public itk::Object
{
public:
  typedef FragmentShaderRegistry                          Self;
  typedef itk::Object                                     Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  static Pointer Instance();

  void RegisterShader(const std::string& name, const std::string& source);

  bool UnregisterShader(const std::string& name);

  bool IsShaderRegistered(const std::string& name) const;

  bool LoadShader(const std::string& name);

  void UnloadShader();

  void ClearShaders();

  unsigned int GetShaderProgram(const std::string& name);

protected:
  FragmentShaderRegistry();

  virtual ~FragmentShaderRegistry();

private:
  typedef std::map<std::string, std::pair<unsigned int, unsigned int> > ShaderMapType;

  itkNewMacro(Self);

  // prevent implementation
  FragmentShaderRegistry(const Self&);
  void operator=(const Self&);

  static Pointer m_Instance;

  ShaderMapType m_ShaderMap;

}; // End class FragmentShaderRegistry

} // End namespace otb

#endif
