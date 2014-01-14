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
#ifndef otb_IceViewer_h
#define otb_IceViewer_h

#include <stdlib.h>
#include <GLFW/glfw3.h>
#include "otbGlView.h"
#include "otbGlImageActor.h"
#include "otbViewSettings.h"
#include "otbDEMHandler.h"
#include "otbStandardShader.h"
#include <GL/freeglut.h>


namespace otb
{

class IceViewer 
  : public itk::Object
{
public:
  typedef IceViewer                                          Self;
  typedef itk::Object                                     Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  itkNewMacro(Self);

  void AddImage(const std::string & fname, const std::string & name);

  void Initialize(unsigned int w, unsigned int h, const std::string & name = "OTB viewer for geeks");

  void Refresh();

  void Start();

protected:


  IceViewer();

  virtual ~IceViewer();

private:
  // Static callbacks
  static void static_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void static_framebuffer_size_callback(GLFWwindow* window, int width, int height);  
  static void static_mouse_button_callback(GLFWwindow * window, int button, int action, int mode);  
  static void static_cursor_pos_callback(GLFWwindow * window, double xpos, double ypos); 
  static void static_scroll_callback(GLFWwindow * window, double xoffset, double yoffset);
  static void error_callback(int error, const char* description);

  // Non-static callbacks
  void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);
  void cursor_pos_callback(GLFWwindow * window, double xpos, double ypos);
  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
  void mouse_button_callback(GLFWwindow * window, int button, int action, int mode);
  void framebuffer_size_callback(GLFWwindow* window, int width, int height);

  // Render Hud
  void DrawHud();
  
  // Render help
  void DrawHelp();

  // prevent implementation
  IceViewer(const Self&);
  void operator=(const Self&);

  GLFWwindow * m_Window;

  otb::GlView::Pointer m_View;

  bool m_FastRendering;

  std::string m_SelectedActor;

  std::string m_ReferenceActor;

  double m_Factor;

  bool m_Dragging;

  ViewSettings::PointType m_StartDrag;
  ViewSettings::PointType m_DeltaDrag;
  ViewSettings::PointType m_OriginDrag;

  bool m_DisplayHud;
  bool m_DisplayHelp;

}; // End class IceViewer

} // End namespace otb

#endif
