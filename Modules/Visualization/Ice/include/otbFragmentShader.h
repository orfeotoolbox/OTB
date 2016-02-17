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
#include "itkPoint.h"

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

  typedef itk::Point<float,2>                             PointType;

  void LoadShader();

  virtual void SetupShader();

  void UnloadShader();

  itkSetMacro(UL,PointType);
  itkSetMacro(UR,PointType);
  itkSetMacro(LL,PointType);
  itkSetMacro(LR,PointType);
  itkGetConstReferenceMacro(UL,PointType);
  itkGetConstReferenceMacro(UR,PointType);
  itkGetConstReferenceMacro(LL,PointType);
  itkGetConstReferenceMacro(LR,PointType);

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

  PointType m_UL;
  PointType m_UR;
  PointType m_LL;
  PointType m_LR;

}; // End class FragmentShader

} // End namespace otb

#endif
