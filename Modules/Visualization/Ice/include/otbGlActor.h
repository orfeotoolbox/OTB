/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otb_GlActor_h
#define otb_GlActor_h

#include <itkObject.h>

#include "otbViewSettings.h"
#include "otbShader.h"
#include "OTBIceExport.h"
#include <string>

namespace otb
{

class OTBIce_EXPORT GlActor
  : public itk::Object
{
public:
  typedef GlActor                                          Self;
  typedef itk::Object                                     Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  itkSetObjectMacro(Settings,ViewSettings);
  itkGetObjectMacro(Settings,ViewSettings);
  itkGetConstObjectMacro(Settings,ViewSettings);

  itkSetMacro(Visible,bool);
  itkGetMacro(Visible,bool);
  itkBooleanMacro(Visible);

  itkSetStringMacro(Name);
  itkGetStringMacro(Name);

  itkSetMacro( GeometryChanged, bool );
  itkGetMacro( GeometryChanged, bool );
  itkBooleanMacro( GeometryChanged );

  itkSetMacro( Overlay, bool );
  itkGetMacro( Overlay, bool );
  itkBooleanMacro( Overlay );

  itkGetObjectMacro( Shader, Shader );
  itkSetObjectMacro( Shader, Shader );

  // Retrieve the full extent of the actor
  virtual void GetExtent(double & ulx, double & uly, double & lrx, double & lry) const = 0;

  // Update internal actor state with respect to ViewSettings
  virtual void ProcessViewSettings() = 0;

  // Heavy load/unload operations of data
  virtual void UpdateData() = 0;

  // Gl rendering of current state
  virtual void Render() = 0;

  /** create the shader (no shader created by default) */
  virtual void CreateShader();

protected:
  GlActor();

  ~GlActor() override;

  Shader::Pointer m_Shader;

private:
  // prevent implementation
  GlActor( const Self & ) = delete;
  void operator=( const Self & ) = delete;

  ViewSettings::Pointer m_Settings;

  // Name field
  std::string m_Name;

  // Visibility flag
  bool m_Visible: 1;

  /**
   */
  bool m_GeometryChanged: 1;

  /**
   */
  bool m_Overlay: 1;

}; // End class GlActor

} // End namespace otb

#endif
