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

typedef enum
{
  SHADER_STANDARD,
  SHADER_LOCAL_CONTRAST,
  SHADER_LOCAL_ALPHA,
  SHADER_ALPHA_GRID,
  SHADER_ALPHA_SLIDER,
} ShaderType;


class StandardShader 
  : public FragmentShader
{
public:
  typedef StandardShader                                  Self;
  typedef itk::Object                                     Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  typedef itk::Point<float,2>                             PointType;

  
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

  itkSetMacro(CurrentRed,double);
  itkSetMacro(CurrentGreen,double);
  itkSetMacro(CurrentBlue,double);

  itkGetMacro(CurrentRed,double);
  itkGetMacro(CurrentGreen,double);
  itkGetMacro(CurrentBlue,double);
  
  itkSetMacro(LocalContrastRange,double);
  itkGetMacro(LocalContrastRange,double);

  itkSetMacro(Gamma,double);
  itkGetMacro(Gamma,double);

  itkSetMacro(Alpha,double);
  itkGetMacro(Alpha,double);

  itkSetMacro(ShaderType,ShaderType);
  itkGetMacro(ShaderType,ShaderType);

  itkSetMacro(Radius,double);
  itkGetConstReferenceMacro(Radius,double);

  itkSetMacro(ChessboardSize,double);
  itkGetConstReferenceMacro(ChessboardSize,double);
  
  itkSetMacro(SliderPosition,double);
  itkGetConstReferenceMacro(SliderPosition,double);
  
  itkSetMacro(VerticalSlider,bool);
  itkGetMacro(VerticalSlider,bool);

  itkSetMacro(Center,PointType);
  itkGetConstReferenceMacro(Center,PointType);

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
  double m_Gamma;
  double m_Alpha;
  double m_CurrentRed;
  double m_CurrentGreen;
  double m_CurrentBlue;
  double m_LocalContrastRange;

  PointType m_Center;

  double m_Radius;

  double m_ChessboardSize;

  double m_SliderPosition;

  bool m_VerticalSlider;

  ShaderType m_ShaderType;

}; // End class StandardShader

} // End namespace otb

#endif
