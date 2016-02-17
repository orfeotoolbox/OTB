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


#include "itkObjectFactory.h"

#include "otbFragmentShader.h"
#include "otbImageSettings.h"

namespace otb
{

typedef enum
{
  SHADER_STANDARD,
  SHADER_LOCAL_CONTRAST,
  SHADER_LOCAL_ALPHA,
  SHADER_ALPHA_GRID,
  SHADER_ALPHA_SLIDER,
  SHADER_SPECTRAL_ANGLE,
  SHADER_GRADIENT
} ShaderType;


class StandardShader 
  : public FragmentShader
{
public:
  typedef StandardShader                                  Self;
  typedef FragmentShader                                  Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  typedef itk::Point<float,2>                             PointType;

  itkSetObjectMacro( ImageSettings, ImageSettings );
  itkGetObjectMacro( ImageSettings, ImageSettings );
  itkGetConstObjectMacro( ImageSettings, ImageSettings );

  itkSetMacro(LocalContrastRange,double);
  itkGetMacro(LocalContrastRange,double);

  itkSetMacro(SpectralAngleRange,double);
  itkGetMacro(SpectralAngleRange,double);

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

  ImageSettings::Pointer m_ImageSettings;

  double m_LocalContrastRange;
  double m_SpectralAngleRange;

  PointType m_Center;

  double m_Radius;

  double m_ChessboardSize;

  double m_SliderPosition;

  bool m_VerticalSlider;

  ShaderType m_ShaderType;
}; // End class StandardShader

} // End namespace otb

#endif
