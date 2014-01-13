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
#include "otbIceViewer.h"
#include <algorithm>
#include <otbImageMetadataInterfaceFactory.h>

namespace otb
{

IceViewer::IceViewer()
  : m_Window(NULL),
    m_View(),
    m_FastRendering(false),
    m_SelectedActor(""),
    m_ReferenceActor(""),
    m_Factor(1.1),
    m_Dragging(false),
    m_StartDrag(),
    m_DeltaDrag(),
    m_OriginDrag(),
    m_DisplayHud(true),
    m_DisplayHelp(false)
{}

IceViewer::~IceViewer()
{
  if(m_Window != NULL)
    {
    glfwDestroyWindow(m_Window);
    }

  glfwTerminate();
}

void IceViewer::AddImage(const std::string & fname, const std::string & name)
{
  if(m_View.IsNull())
    {
    itkExceptionMacro("Viewer has not been initialized. Call Initialize method first.");
    }
  
  otb::GlImageActor::Pointer actor = otb::GlImageActor::New();
  actor->Initialize(fname);
  actor->SetVisible(true);

  // Get advised colors
  ImageMetadataInterfaceBase::Pointer imi = ImageMetadataInterfaceFactory::CreateIMI(actor->GetMetaDataDictionary());
  
  actor->SetRedIdx(imi->GetDefaultDisplay()[0]+1);
  actor->SetGreenIdx(imi->GetDefaultDisplay()[1]+1);
  actor->SetBlueIdx(imi->GetDefaultDisplay()[2]+1);

  otb::StandardShader::Pointer shader = static_cast<otb::StandardShader *>(actor->GetShader());
  double minRed,maxRed,minGreen,maxGreen,minBlue,maxBlue;

  actor->AutoColorAdjustment(minRed,maxRed,minGreen,maxGreen,minBlue,maxBlue);

  shader->SetMinRed(minRed);
  shader->SetMinGreen(minGreen);
  shader->SetMinBlue(minBlue);
  shader->SetMaxRed(maxRed);
  shader->SetMaxGreen(maxGreen);
  shader->SetMaxBlue(maxBlue);
  
  // TODO: Implement this
  // actor->UpdateColorBalance();
  
  m_View->AddActor(actor,name);
}

void IceViewer::Initialize(unsigned int w, unsigned int h, const std::string & name)
{
  // Initialize glfw
  if(!glfwInit())
    {
    itkExceptionMacro(<<"Could not initalize glfw!");
    }

  // Initialize glut (ugly workaround)
  int fake_argc = 1;
  char fake_name[] = "fake";
  char * fake_argv[] = {fake_name,NULL};
  glutInit(&fake_argc,fake_argv);

  // Set glfw errors callback
  glfwSetErrorCallback(error_callback);
  
  // Create the window
  if(m_Window != NULL)
    {
    glfwDestroyWindow(m_Window);
    }
  
  m_Window = glfwCreateWindow(w, h,"I see", NULL, NULL);
   if (!m_Window)
     {
     itkExceptionMacro(<<"Could not create glfw window.");
     }

   glfwMakeContextCurrent(m_Window);

   glfwSetWindowUserPointer(m_Window,this);
   
   glfwSetKeyCallback(m_Window, &IceViewer::static_key_callback);
   glfwSetFramebufferSizeCallback(m_Window,&IceViewer::static_framebuffer_size_callback);
   glfwSetMouseButtonCallback(m_Window,&IceViewer::static_mouse_button_callback);
   glfwSetCursorPosCallback(m_Window,&IceViewer::static_cursor_pos_callback);
   glfwSetScrollCallback(m_Window,&IceViewer::static_scroll_callback);

   // Create view
   m_View = GlView::New();
   m_View->Initialize(w,h);
}

void IceViewer::Refresh()
{
  int width, height;
  glfwGetFramebufferSize(m_Window, &width, &height);
  
  m_View->BeforeRendering();
  
  if(m_FastRendering || m_Dragging)
    {
    m_View->LightRender();
    }
  else
    {
    m_View->HeavyRender();
    }
  
  m_View->AfterRendering();  

  if(m_DisplayHelp)
    {
    DrawHelp();
    }
  else if(m_DisplayHud)
    {
    DrawHud();
    }
}

void IceViewer::Start()
{
  // Define viewport geometry as first actor geometry
  std::vector<std::string> renderingOrder = m_View->GetRenderingOrder();

  if(renderingOrder.empty())
    {
    itkExceptionMacro(<<"No actors have been loaded to the application.");
    }
  
  std::string firstActorKey = renderingOrder.front();
  otb::GlImageActor::Pointer firstActor = dynamic_cast<otb::GlImageActor*>(m_View->GetActor(firstActorKey).GetPointer());

  m_View->GetSettings()->SetOrigin(firstActor->GetOrigin());
  m_View->GetSettings()->SetSpacing(firstActor->GetSpacing());
  m_View->GetSettings()->SetWkt(firstActor->GetWkt());
  m_View->GetSettings()->SetKeywordList(firstActor->GetKwl());
  m_View->GetSettings()->UseProjectionOn();

  m_SelectedActor = firstActorKey;
  m_ReferenceActor = firstActorKey;
 
   double ulx,uly,lrx,lry;
   firstActor->GetExtent(ulx,uly,lrx,lry);

   otb::ViewSettings::PointType center;
   center[0] = 0.5*(ulx+lrx);
   center[1] = 0.5*(uly+lry);
   
   m_View->GetSettings()->Center(center);

   // Start events loop
   while (!glfwWindowShouldClose(m_Window))
     {
     this->Refresh();

     // Swap buffers
     glfwSwapBuffers(m_Window);
     
     glfwWaitEvents();
     }
}

void IceViewer::DrawHud()
{
  double posx, posy, vpx, vpy,hudposx,hudposy,hudvpx,hudvpy;
  glfwGetCursorPos(m_Window,&posx,&posy);
  m_View->GetSettings()->ScreenToViewPortTransform(posx,posy,vpx,vpy);
  
  std::vector<std::string> renderingOrder = m_View->GetRenderingOrder();
  
  std::ostringstream oss;
  
  for(std::vector<std::string>::iterator it = renderingOrder.begin();
      it!=renderingOrder.end();++it)
    {
    // Retrieve actor information
    otb::GlImageActor::Pointer currentActor = dynamic_cast<otb::GlImageActor*>(m_View->GetActor(*it).GetPointer());
    otb::GlImageActor::PointType pin,poutphys,poutidx;
    pin[0]=vpx;
    pin[1]=vpy;
    poutphys = currentActor->ViewportToImageTransform(pin,true);
    poutidx = currentActor->ViewportToImageTransform(pin,false);
    otb::GlImageActor::PixelType pixel;
    bool pixelAvail = currentActor->GetPixelFromViewport(pin,pixel);
    
    oss<<(currentActor->GetVisible()?"+ ":"- ");
    oss<<((*it)==m_ReferenceActor?"*":" ");
    oss<<((*it)==m_SelectedActor?"[":" ");
    oss<<(*it);
    oss<<((*it)==m_SelectedActor?"]":" ");
    oss<<((*it)==m_ReferenceActor?"*":" ");
    oss<<std::endl;

    otb::StandardShader::Pointer shader = static_cast<otb::StandardShader *>(currentActor->GetShader());

    oss<<"    Rendering: "<<"R="<<currentActor->GetRedIdx()<<":["<<shader->GetMinRed()<<", "<<shader->GetMaxRed()<<"] G="<<currentActor->GetGreenIdx()<<":["<<shader->GetMinGreen()<<", "<<shader->GetMaxGreen()<<"] B="<<currentActor->GetBlueIdx()<<":["<<shader->GetMinBlue()<<", "<<shader->GetMaxBlue()<<"]"<<"\n";
    oss<<"    Shader: ";

    switch(shader->GetShaderType())
      {
      case SHADER_STANDARD:
        oss<<" standard";
        break;
      case SHADER_LOCAL_CONTRAST:
        oss<<" local contrast - radius="<<shader->GetRadius()<<", range=["<<-shader->GetLocalContrastRange()<<", "<<shader->GetLocalContrastRange()<<"]";
        break;
      case SHADER_LOCAL_ALPHA:
        oss<<" local transparency - radius="<<shader->GetRadius();
        break;
      case SHADER_ALPHA_GRID:
        oss<<" chessboard transparency - grid="<<shader->GetChessboardSize();
        break;
      case SHADER_ALPHA_SLIDER:
        oss<<" swipe transparency - "<<(shader->GetVerticalSlider()?"vertical":"horizontal")<<", position="<<shader->GetSliderPosition();
        break;
        // Do nothing
      }
    oss<<"\n";

    oss<<"    Position - index: ("<<poutidx[0]<<", "<<poutidx[1]<<"), phys: "<<poutphys[0]<<", "<<poutphys[1]<<")";
    oss<<", resolution: "<<currentActor->GetCurrentResolution()<<"\n";
    if(pixelAvail)
      {
      oss<<"    Pixel value: ["<<pixel[0];
      for(unsigned int i = 1; i<pixel.Size();++i)
        {
        oss<<", "<<pixel[i];
        }
      oss<<"]";
      }
    oss<<"\n"<<"\n";
    }
  
  // Find the size of the hud
  std::string hud_string = oss.str();
  
  unsigned int nbline = 0;
  unsigned int max_line_widh = 0;
  unsigned int char_count = 0;

  for(std::string::iterator sit = hud_string.begin();
      sit!=hud_string.end();++sit)
    {
    if((*sit)=='\n')
      {
      nbline+=1;

      if(char_count>max_line_widh)
        {
        max_line_widh = char_count;
        }

      char_count = 0;
      }
    char_count+=1;
    }
  
  double hud_width = 8*max_line_widh;
  double hud_height = 13*nbline;
  hudposx = 10;
  hudposy = m_View->GetSettings()->GetViewportSize()[1]-hud_height-10;
  
  double x,y;

  glEnable(GL_BLEND);
  glColor4f(0.,0.,0.,0.75);
  glBegin (GL_QUADS);
  m_View->GetSettings()->ScreenToViewPortTransform(hudposx- 5,hudposy-5-13,x,y);
  glVertex2f(x,y);
  m_View->GetSettings()->ScreenToViewPortTransform(hudposx + 5 + hud_width,hudposy-5-13,x,y);
  glVertex2f(x,y);
  m_View->GetSettings()->ScreenToViewPortTransform(hudposx + 5 + hud_width, hudposy + hud_height + 5,x,y);
  glVertex2f(x,y);
  m_View->GetSettings()->ScreenToViewPortTransform(hudposx - 5, hudposy + hud_height + 5,x,y);
  glVertex2f(x,y);
  glEnd ();
  glDisable(GL_BLEND);

  glColor3f(1.0f,1.0f,1.0f);
  m_View->GetSettings()->ScreenToViewPortTransform(hudposx,hudposy,hudvpx,hudvpy);
  glRasterPos2f(hudvpx,hudvpy);
  glutBitmapString(GLUT_BITMAP_8_BY_13,(unsigned char *) oss.str().c_str());
}

void IceViewer::DrawHelp()
{
  
  std::ostringstream oss;

  oss<<"This is ice viewer help page. Press ESC key to exit."<<std::endl;
  oss<<"Here are the main available actions:"<<std::endl;
  oss<<std::endl;

  oss<<"General: "<<std::endl;
  oss<<"- Show/hide image list with D key"<<std::endl;
  oss<<"- Exit with ESC key"<<std::endl;
  oss<<std::endl;
    
  oss<<"Navigation:"<<std::endl;
  oss<<"- Move with mouse left button drag & drop or keyboard arrows"<<std::endl;
  oss<<"- Zoom in/out with CTRL + mouse wheel"<<std::endl;
  oss<<std::endl;

  oss<<"Multiple images handling:"<<std::endl;
  oss<<"- Rotate rendering order with mouse wheel (order displayed in image list)"<<std::endl;
  oss<<"- Change selected image with PAGE UP / PAGE DOWN (note that selected dataset is highlighted with [ ]"<<std::endl;
  oss<<"- Show/hide image dataset with space bar"<<std::endl;
  oss<<"- Note that all images are reprojected in the first loaded image geometry (highligthed with * *)"<<std::endl;
  oss<<std::endl;

  oss<<"Color range:"<<std::endl;
  oss<<"- Rotate red/green/blue channel of selected image with R, G and B keys"<<std::endl;
  oss<<"- Reset color range of selected image to 2% histogram clipping of full image with F key"<<std::endl;
  oss<<"- Reset color range of selected image to 2% histogram clipping of current area with G key"<<std::endl;
  oss<<"- Hold Right MAJ key + mouse wheel to adjust gamma correction of selected image"<<std::endl;
  oss<<"- Hol Right CTRM key + mouse wheel to adjust transparency of selected image"<<std::endl;
  oss<<std::endl;

  oss<<"Shaders of selected image:"<<std::endl;
  oss<<"- Switch to standard shader with S key. In this mode, use Left SHIFT + mouse wheel and Left ALT + mouse wheel to adjust contrast"<<std::endl;
  oss<<"- Switch to local contrast enhancement with L key. In this mode, use Right SHIFT + mouse wheel to adjust gain and Left ALT + mouse wheel to adjust circle radius"<<std::endl;
  oss<<"- Switch to local transparency with U key. "<<std::endl;
  
  // Find the size of the help
  std::string help_string = oss.str();
  
  unsigned int nbline = 0;
  unsigned int max_line_widh = 0;
  unsigned int char_count = 0;

  for(std::string::iterator sit = help_string.begin();
      sit!=help_string.end();++sit)
    {
    if((*sit)=='\n')
      {
      nbline+=1;

      if(char_count>max_line_widh)
        {
        max_line_widh = char_count;
        }

      char_count = 0;
      }
    char_count+=1;
    }
  
  double help_width = 8*max_line_widh;
  double help_height = 13*nbline;
  double helpposx = 10;
  double helpposy = 20;
  double helpvpx,helpvpy;
  double x,y;

  glEnable(GL_BLEND);
  glColor4f(0.,0.,0.,0.75);
  glBegin (GL_QUADS);
  m_View->GetSettings()->ScreenToViewPortTransform(helpposx- 5,helpposy-5-13,x,y);
  glVertex2f(x,y);
  m_View->GetSettings()->ScreenToViewPortTransform(helpposx + 5 + help_width,helpposy-5-13,x,y);
  glVertex2f(x,y);
  m_View->GetSettings()->ScreenToViewPortTransform(helpposx + 5 + help_width, helpposy + help_height + 5,x,y);
  glVertex2f(x,y);
  m_View->GetSettings()->ScreenToViewPortTransform(helpposx - 5, helpposy + help_height + 5,x,y);
  glVertex2f(x,y);
  glEnd ();
  glDisable(GL_BLEND);

  glColor3f(1.0f,1.0f,1.0f);
  m_View->GetSettings()->ScreenToViewPortTransform(helpposx,helpposy,helpvpx,helpvpy);
  glRasterPos2f(helpvpx,helpvpy);
  glutBitmapString(GLUT_BITMAP_8_BY_13,(unsigned char *) oss.str().c_str());
}

// Static callbacks
void IceViewer::static_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  Self * instance = static_cast<Self*>(glfwGetWindowUserPointer(window));
  instance->key_callback(window, key, scancode, action, mods);
}

void IceViewer::static_framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  Self * instance = static_cast<Self*>(glfwGetWindowUserPointer(window));
  instance->framebuffer_size_callback(window, width, height);
}

void IceViewer::static_mouse_button_callback(GLFWwindow * window, int button, int action, int mode)
{
  Self * instance = static_cast<Self*>(glfwGetWindowUserPointer(window));
  instance->mouse_button_callback(window,button,action,mode);
}

void IceViewer::static_cursor_pos_callback(GLFWwindow * window, double xpos, double ypos)
{
  Self * instance = static_cast<Self*>(glfwGetWindowUserPointer(window));
  instance->cursor_pos_callback(window, xpos, ypos);
}

void IceViewer::static_scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
  Self * instance = static_cast<Self*>(glfwGetWindowUserPointer(window));
  instance->scroll_callback(window,xoffset,yoffset);
}

void IceViewer::error_callback(int error, const char* description)
{
  itkGenericExceptionMacro(<<description);
}

void IceViewer::scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
  double factor = (yoffset>0) ? 1/m_Factor : m_Factor;
  otb::GlImageActor::Pointer currentActor = dynamic_cast<otb::GlImageActor*>(m_View->GetActor(m_SelectedActor).GetPointer());
  otb::StandardShader::Pointer shader = static_cast<otb::StandardShader *>(currentActor->GetShader());

  // Handle zoom
  if(glfwGetKey(window,GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
    double posx, posy,vpx,vpy;
    
    glfwGetCursorPos(m_Window,&posx,&posy);
    
    m_View->GetSettings()->ScreenToViewPortTransform(posx,posy,vpx,vpy);
    
    otb::ViewSettings::PointType zoomCenter;
    zoomCenter[0] = vpx;
    zoomCenter[1] = vpy;
    
    m_View->GetSettings()->Zoom(zoomCenter,factor);
    }
  else if(glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
    if(shader->GetShaderType() == SHADER_STANDARD)
      {
      shader->SetMinRed(shader->GetMinRed()*factor);
      shader->SetMinGreen(shader->GetMinGreen()*factor);
      shader->SetMinBlue(shader->GetMinBlue()*factor);
      shader->SetMaxRed(shader->GetMaxRed()*factor);
      shader->SetMaxGreen(shader->GetMaxGreen()*factor);
      shader->SetMaxBlue(shader->GetMaxBlue()*factor);
      }
    else if(shader->GetShaderType() == SHADER_LOCAL_CONTRAST)
      {
      shader->SetLocalContrastRange(shader->GetLocalContrastRange()*factor);
      }
    }
   else if(glfwGetKey(window,GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
    switch(shader->GetShaderType())
      {
      case SHADER_STANDARD:
        shader->SetMinRed(shader->GetMinRed()/factor);
        shader->SetMinGreen(shader->GetMinGreen()/factor);
        shader->SetMinBlue(shader->GetMinBlue()/factor);
        shader->SetMaxRed(shader->GetMaxRed()*factor);
        shader->SetMaxGreen(shader->GetMaxGreen()*factor);
        shader->SetMaxBlue(shader->GetMaxBlue()*factor);
        break;
      case SHADER_LOCAL_CONTRAST:
        shader->SetRadius(shader->GetRadius()/factor);
        break;
      case SHADER_LOCAL_ALPHA:
        shader->SetRadius(shader->GetRadius()/factor);
        break;
      case SHADER_ALPHA_GRID:
        shader->SetChessboardSize(shader->GetChessboardSize()/factor);
        break;
      // case SHADER_ALPHA_SLIDER:
      //   shader->SetSliderPosition(shader->GetSliderPosition()+(yoffset>0?-25:-25));
      //   break;
      }
    }
   else if(glfwGetKey(window,GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
     {
     shader->SetGamma(shader->GetGamma()/factor);
     }
   else if(glfwGetKey(window,GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
     {
     shader->SetAlpha(shader->GetAlpha()/factor);  
     }
   else
     {
     m_View->RotateRenderingOrder(yoffset>0);
     }
}

void IceViewer::cursor_pos_callback(GLFWwindow * window, double xpos, double ypos)
{
  double posx, posy,vpx,vpy;
  glfwGetCursorPos(m_Window,&posx,&posy);
  
  if(m_Dragging)
    {
    m_DeltaDrag[0] = posx - m_StartDrag[0];
    m_DeltaDrag[1] = m_StartDrag[1]-posy;
    
    otb::ViewSettings::PointType origin;
    origin[0]=m_OriginDrag[0]-m_DeltaDrag[0]*m_View->GetSettings()->GetSpacing()[0];
    origin[1]=m_OriginDrag[1]+m_DeltaDrag[1]*vcl_abs(m_View->GetSettings()->GetSpacing()[1]);
    
    m_View->GetSettings()->SetOrigin(origin);
    }
  
  m_View->GetSettings()->ScreenToViewPortTransform(posx,posy,vpx,vpy);

  std::vector<std::string> renderingOrder = m_View->GetRenderingOrder();

  // Inform all shaders of current color and position
  for(std::vector<std::string>::iterator it = renderingOrder.begin();
      it!=renderingOrder.end();++it)
    {
    // Retrieve actor information
    otb::GlImageActor::Pointer currentActor = dynamic_cast<otb::GlImageActor*>(m_View->GetActor(*it).GetPointer());
    otb::GlImageActor::PointType pin;
    pin[0]=vpx;
    pin[1]=vpy;
    otb::GlImageActor::PixelType pixel;
    bool pixelAvail = currentActor->GetPixelFromViewport(pin,pixel);

    otb::StandardShader::Pointer shader = static_cast<otb::StandardShader *>(currentActor->GetShader());

    if(pixelAvail)
      {
      shader->SetCurrentRed(pixel[currentActor->GetRedIdx()-1]);
      shader->SetCurrentGreen(pixel[currentActor->GetGreenIdx()-1]);
      shader->SetCurrentBlue(pixel[currentActor->GetBlueIdx()-1]);
      }
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    pin[0]=posx;
    pin[1]=height-posy;
    shader->SetCenter(pin);

    if(shader->GetShaderType() == SHADER_ALPHA_SLIDER)
      {
      if(shader->GetVerticalSlider())
        {
        shader->SetSliderPosition(height-posy);
        }
      else
        {
        shader->SetSliderPosition(posx);
        }
      }
    }
}

void IceViewer::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
 

  // Exit on escape
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
    if(m_DisplayHelp)
      {
      m_DisplayHelp = false;
      }
    else
      {
      glfwSetWindowShouldClose(m_Window, GL_TRUE);
      }
    }

  // Fast rendering
  if(key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
    {
    m_FastRendering = true;
    }
  else if(key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE)
    {
    m_FastRendering = false;
    }

  // Arrows moves
  double ulx,uly,lrx,lry;
  m_View->GetSettings()->GetViewportExtent(ulx,uly,lrx,lry);
  double deltax = (lrx-ulx)/4;
  double deltay = (lry-uly)/4;
  otb::ViewSettings::PointType origin = m_View->GetSettings()->GetOrigin();
  
  if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
    origin[0]-=deltax; 
    m_View->GetSettings()->SetOrigin(origin);
    }
  
  if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
    origin[0]+=deltax; 
    m_View->GetSettings()->SetOrigin(origin);
    }
  
  if(key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
    origin[1]-=deltay; 
    m_View->GetSettings()->SetOrigin(origin);
    }
  
  if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
    origin[1]+=deltay; 
    m_View->GetSettings()->SetOrigin(origin);
    }
  
  // Change selected actor
  if(key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS)
    {
    std::vector<std::string> renderingOrder = m_View->GetRenderingOrder();
    std::vector<std::string>::iterator it = std::find(renderingOrder.begin(),renderingOrder.end(),m_SelectedActor);
    
    if(it!=renderingOrder.end())
      {
      unsigned int idx = std::distance(renderingOrder.begin(),it);
      m_SelectedActor = renderingOrder.at((idx-1)%renderingOrder.size());
      }
    
    }
  if(key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS)
    {
    std::vector<std::string> renderingOrder = m_View->GetRenderingOrder();
    std::vector<std::string>::iterator it = std::find(renderingOrder.begin(),renderingOrder.end(),m_SelectedActor);
    
    if(it!=renderingOrder.end())
      {
      unsigned int idx = std::distance(renderingOrder.begin(),it);
      m_SelectedActor = renderingOrder.at((idx+1)%renderingOrder.size());
      }    
    }

  // Move actors
  if(key == GLFW_KEY_KP_ADD && action == GLFW_PRESS )
    {
    m_View->MoveActorInRenderingOrder(m_SelectedActor,false);
    }
  
  if(key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS)
    {
    m_View->MoveActorInRenderingOrder(m_SelectedActor,true);
    }

  // Switch shader mode
  if(key == GLFW_KEY_L && action == GLFW_PRESS)
    {
    double posx,posy,vpx,vpy;
    glfwGetCursorPos(m_Window,&posx,&posy);

    m_View->GetSettings()->ScreenToViewPortTransform(posx,posy,vpx,vpy);

    otb::GlImageActor::Pointer currentActor = dynamic_cast<otb::GlImageActor*>(m_View->GetActor(m_SelectedActor).GetPointer());
    otb::StandardShader::Pointer shader = static_cast<otb::StandardShader *>(currentActor->GetShader());
   
    ViewSettings::PointType pin;
    pin[0]=vpx;
    pin[1]=vpy;

    otb::GlImageActor::PixelType pixel;
    bool pixelAvail = currentActor->GetPixelFromViewport(pin,pixel);

    if(pixelAvail)
      {
      shader->SetCurrentRed(pixel[currentActor->GetRedIdx()-1]);
      shader->SetCurrentGreen(pixel[currentActor->GetGreenIdx()-1]);
      shader->SetCurrentBlue(pixel[currentActor->GetBlueIdx()-1]);
      }
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    pin[0]=posx;
    pin[1]=height-posy;
    shader->SetCenter(pin);

    shader->SetShaderType(SHADER_LOCAL_CONTRAST);
    }
  if(key == GLFW_KEY_U && action == GLFW_PRESS)
    {
    otb::GlImageActor::Pointer currentActor = dynamic_cast<otb::GlImageActor*>(m_View->GetActor(m_SelectedActor).GetPointer());
    otb::StandardShader::Pointer shader = static_cast<otb::StandardShader *>(currentActor->GetShader());
    shader->SetShaderType(SHADER_LOCAL_ALPHA);
    }
  if(key == GLFW_KEY_C && action == GLFW_PRESS)
    {
    otb::GlImageActor::Pointer currentActor = dynamic_cast<otb::GlImageActor*>(m_View->GetActor(m_SelectedActor).GetPointer());
    otb::StandardShader::Pointer shader = static_cast<otb::StandardShader *>(currentActor->GetShader());
    shader->SetShaderType(SHADER_ALPHA_GRID);
    }
  if(key == GLFW_KEY_S && action == GLFW_PRESS)
    {
    otb::GlImageActor::Pointer currentActor = dynamic_cast<otb::GlImageActor*>(m_View->GetActor(m_SelectedActor).GetPointer());
    otb::StandardShader::Pointer shader = static_cast<otb::StandardShader *>(currentActor->GetShader());
    if(shader->GetShaderType() == SHADER_ALPHA_SLIDER)
      {
      shader->SetVerticalSlider(!shader->GetVerticalSlider());
      }
    shader->SetShaderType(SHADER_ALPHA_SLIDER);

    double posx,posy;
    glfwGetCursorPos(m_Window,&posx,&posy);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    if(shader->GetVerticalSlider())
      {
      shader->SetSliderPosition(height-posy);
      }
    else
      {
      shader->SetSliderPosition(posx);
      }
    }
   if(key == GLFW_KEY_X && action == GLFW_PRESS)
    {
    otb::GlImageActor::Pointer currentActor = dynamic_cast<otb::GlImageActor*>(m_View->GetActor(m_SelectedActor).GetPointer());
    otb::StandardShader::Pointer shader = static_cast<otb::StandardShader *>(currentActor->GetShader());
    shader->SetShaderType(SHADER_STANDARD);
    }

   if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
     {
     otb::GlImageActor::Pointer currentActor = dynamic_cast<otb::GlImageActor*>(m_View->GetActor(m_SelectedActor).GetPointer());
     currentActor->SetVisible(!currentActor->GetVisible());
     }
   if(key == GLFW_KEY_D && action == GLFW_PRESS)
     {
     m_DisplayHud = !m_DisplayHud;
     }

   if(key == GLFW_KEY_R && action == GLFW_PRESS)
     {
     otb::GlImageActor::Pointer currentActor = dynamic_cast<otb::GlImageActor*>(m_View->GetActor(m_SelectedActor).GetPointer());
     currentActor->SetRedIdx(1+(currentActor->GetRedIdx())%currentActor->GetNumberOfComponents());
     }
   if(key == GLFW_KEY_G && action == GLFW_PRESS)
     {
     otb::GlImageActor::Pointer currentActor = dynamic_cast<otb::GlImageActor*>(m_View->GetActor(m_SelectedActor).GetPointer());
     currentActor->SetGreenIdx(1+(currentActor->GetGreenIdx())%currentActor->GetNumberOfComponents());
     }
   if(key == GLFW_KEY_B && action == GLFW_PRESS)
     {
     otb::GlImageActor::Pointer currentActor = dynamic_cast<otb::GlImageActor*>(m_View->GetActor(m_SelectedActor).GetPointer());
     currentActor->SetBlueIdx(1+(currentActor->GetBlueIdx())%currentActor->GetNumberOfComponents());
     }

   if(key == GLFW_KEY_F && action == GLFW_PRESS)
     {
     otb::GlImageActor::Pointer currentActor = dynamic_cast<otb::GlImageActor*>(m_View->GetActor(m_SelectedActor).GetPointer());
     otb::StandardShader::Pointer shader = static_cast<otb::StandardShader *>(currentActor->GetShader());

     double minRed,maxRed,minGreen,maxGreen,minBlue,maxBlue;     
     currentActor->AutoColorAdjustment(minRed,maxRed,minGreen,maxGreen,minBlue,maxBlue);
     
     shader->SetMinRed(minRed);
     shader->SetMinGreen(minGreen);
     shader->SetMinBlue(minBlue);
     shader->SetMaxRed(maxRed);
     shader->SetMaxGreen(maxGreen);
     shader->SetMaxBlue(maxBlue);
     }

if(key == GLFW_KEY_V && action == GLFW_PRESS)
     {
     otb::GlImageActor::Pointer currentActor = dynamic_cast<otb::GlImageActor*>(m_View->GetActor(m_SelectedActor).GetPointer());
     otb::StandardShader::Pointer shader = static_cast<otb::StandardShader *>(currentActor->GetShader());

     double minRed,maxRed,minGreen,maxGreen,minBlue,maxBlue;     
     currentActor->AutoColorAdjustment(minRed,maxRed,minGreen,maxGreen,minBlue,maxBlue,false);
     
     shader->SetMinRed(minRed);
     shader->SetMinGreen(minGreen);
     shader->SetMinBlue(minBlue);
     shader->SetMaxRed(maxRed);
     shader->SetMaxGreen(maxGreen);
     shader->SetMaxBlue(maxBlue);
     }
   if(key == GLFW_KEY_F1 && action == GLFW_PRESS)
     {
     m_DisplayHelp = true;
     }

   // // Change viewport geometry to current actor
   // if(key == GLFW_KEY_P && action == GLFW_PRESS)
   //   {
   //   otb::GlImageActor::Pointer currentActor = dynamic_cast<otb::GlImageActor*>(m_View->GetActor(m_SelectedActor).GetPointer());
     
   //   m_View->GetSettings()->SetOrigin(currentActor->GetOrigin());
   //   m_View->GetSettings()->SetSpacing(currentActor->GetSpacing());
   //   m_View->GetSettings()->SetWkt(currentActor->GetWkt());
   //   m_View->GetSettings()->SetKeywordList(currentActor->GetKwl());
   //   m_View->GetSettings()->UseProjectionOn();
   //   m_ReferenceActor = m_SelectedActor;
   //   }
   
}

void IceViewer::mouse_button_callback(GLFWwindow * window, int button, int action, int mode)
{
  if(button == GLFW_MOUSE_BUTTON_1)
    {
    if(action == GLFW_PRESS)
      {
      m_Dragging = true;
      m_FastRendering = true;
      glfwGetCursorPos(m_Window,&m_StartDrag[0],&m_StartDrag[1]);

      // TODO: Move drag to dedicated method in ViewSettings
      m_OriginDrag = m_View->GetSettings()->GetOrigin();
      }
    else if(action == GLFW_RELEASE)
      {
      m_Dragging = false;
      m_FastRendering = false;
      otb::ViewSettings::PointType origin;
      origin[0]=m_OriginDrag[0]-m_DeltaDrag[0]*m_View->GetSettings()->GetSpacing()[0];
      origin[1]=m_OriginDrag[1]+m_DeltaDrag[1]*vcl_abs(m_View->GetSettings()->GetSpacing()[1]);
      
      m_View->GetSettings()->SetOrigin(origin);
      
      m_StartDrag.Fill(0);
      m_DeltaDrag.Fill(0);
      m_OriginDrag.Fill(0);
      }
    }
}

void IceViewer::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  otb::ViewSettings::PointType center = m_View->GetSettings()->GetViewportCenter();

  otb::ViewSettings::SizeType size;
  size[0] = width;
  size[1] = height;
  
  m_View->GetSettings()->SetViewportSize(size);

  m_View->GetSettings()->Center(center);
}

}
