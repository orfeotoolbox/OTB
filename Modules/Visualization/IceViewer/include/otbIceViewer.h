/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otb_IceViewer_h
#define otb_IceViewer_h

/* Include windows.h before glfw,
 * since GLFW redefines APIENTRY if not already defined,
 * while windef.h unconditionnally defines it
 */
#if defined(WIN32)
#include <windows.h>
#endif

#include <stdlib.h>
#include <GLFW/glfw3.h>
#include "otbGlView.h"
#include "otbGlImageActor.h"
#include "otbGlVectorActor.h"
#include "otbViewSettings.h"
#include "otbDEMHandler.h"
#include "otbStandardShader.h"
#include <GL/freeglut.h>
#include "OTBIceViewerExport.h"
#include <string>

namespace otb
{

class OTBIceViewer_EXPORT IceViewer : public itk::Object
{
public:
  typedef IceViewer                     Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);

  void AddImage(const std::string& fname, const std::string& key, const std::string& name = "");

  void AddVector(const std::string& fname, const std::string& key, const std::string& name = "");

  void Initialize(unsigned int w, unsigned int h, const std::string& name = "OTB viewer for geeks");

  void Refresh();

  virtual void Start();

  void ClearActors();

  void SetActorName(const std::string& key, const std::string& name);

  itkSetMacro(DisplayHud, bool);
  itkGetMacro(DisplayHud, bool);

protected:
  IceViewer();

  ~IceViewer() override;

  // Non-static callbacks
  virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
  virtual bool scroll_callback_image(GLFWwindow* window, double xoffset, double yoffset);
  virtual bool scroll_callback_vector(GLFWwindow* window, double xoffset, double yoffset);
  virtual void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
  virtual void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
  virtual bool key_callback_image(GLFWwindow* window, int key, int scancode, int action, int mods);
  virtual bool key_callback_vector(GLFWwindow* window, int key, int scancode, int action, int mods);
  virtual void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);
  virtual void framebuffer_size_callback(GLFWwindow* window, int width, int height);


  otb::GlActor::Pointer GetActor(const std::string& key);

  void CopyActorStyle(otb::GlActor::Pointer srcActor, otb::GlActor::Pointer dstActor);

  // Update all shaders with current color and position
  void UpdateShaderColorAndPosition();

private:
  // Static callbacks
  static void static_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void static_framebuffer_size_callback(GLFWwindow* window, int width, int height);
  static void static_mouse_button_callback(GLFWwindow* window, int button, int action, int mode);
  static void static_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
  static void static_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
  static void error_callback(int error, const char* description);

  // Render Hud
  void DrawHud();

  // Render help
  void DrawHelp();


  // Update shader with current color and position
  void UpdateShaderColorAndPosition(double vpx, double vpy, otb::GlImageActor* currentActor);

  // prevent implementation
  IceViewer(const Self&);
  void operator=(const Self&);

  GLFWwindow* m_Window;

  otb::GlView::Pointer m_View;

  bool m_FastRendering;

  std::string m_SelectedActor;

  std::string m_ReferenceActor;

  double m_Factor;

  bool m_Dragging;

  ViewSettings::PointType m_StartDrag;
  ViewSettings::PointType m_DeltaDrag;
  ViewSettings::PointType m_OriginDrag;

  double m_StartAngle;

  bool m_DisplayHud;
  bool m_DisplayHelp;

  std::map<std::string, GlVectorActor::ColorType>                 m_ColorMap;
  std::map<std::string, GlVectorActor::ColorType>::const_iterator m_ColorMapIterator;

  bool m_UseGLSL;
}; // End class IceViewer

} // End namespace otb

#endif
