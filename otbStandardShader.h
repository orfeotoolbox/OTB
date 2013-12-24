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
#ifndef otb_StandardShader_h
#define otb_StandardShader_h

#include "otbFragmentShader.h"
#include "itkObjectFactory.h"

namespace otb
{

class StandardShader 
  : public FragmentShader
{
public:
  typedef StandardShader                                  Self;
  typedef itk::Object                                     Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  itkSetMacro(MinRed,double);
  itkSetMacro(MinGreen,double);
  itkSetMacro(MinBlue,double);
  itkGetMacro(MinRed,double);
  itkGetMacro(MinGreen,double);
  itkGetMacro(MinBlue,double);

  itkSetMacro(MaxRed,double);
  itkSetMacro(MaxGreen,double);
  itkSetMacro(MaxBlue,double);
  itkGetMacro(MaxRed,double);
  itkGetMacro(MaxGreen,double);
  itkGetMacro(MaxBlue,double);  

  virtual void SetupShader();

  itkNewMacro(Self);

protected:
  StandardShader();

  virtual ~StandardShader();

  virtual std::string GetSource() const;

  virtual std::string GetName() const;

private:
  // prevent implementation
  StandardShader(const Self&);
  void operator=(const Self&);

  double m_MinRed;
  double m_MaxRed;
  double m_MinGreen;
  double m_MaxGreen;
  double m_MinBlue;
  double m_MaxBlue;


}; // End class StandardShader

} // End namespace otb

#endif
