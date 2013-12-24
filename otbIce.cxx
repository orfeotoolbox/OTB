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
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include "otbGlView.h"
#include "otbGlImageActor.h"
#include "otbViewSettings.h"
#include "otbDEMHandler.h"
#include "otbStandardShader.h"


class Manipulator
{
public:
  static Manipulator & GetInstance()
  {
    static Manipulator instance;
    return instance;
  }

  void SetViewSettings(otb::ViewSettings * settings)
  {
    m_ViewSettings = settings;
  }

  void SetActors(const std::vector<otb::GlImageActor::Pointer> & actors, const std::vector<std::string> names)
  {
    m_Actors = actors;
    m_ActorsNames = names;
  }

  void SetView(otb::GlView * view)
  {
    m_View = view;
  }

  bool IsDragging() const
  {
    return m_Drag;
  }

  bool IsFastRendering() const
  {
    return m_FastRendering;
  }

  static void static_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    GetInstance().key_callback(window, key, scancode, action, mods);
  }

  static void static_framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    GetInstance().framebuffer_size_callback(window, width, height);
  }
  
  static void static_mouse_button_callback(GLFWwindow * window, int button, int action, int mode)
  {
    GetInstance().mouse_button_callback(window,button,action,mode);
  }

  static void static_cursor_pos_callback(GLFWwindow * window, double xpos, double ypos)
  {
    GetInstance().cursor_pos_callback(window, xpos, ypos);
  }

  static void static_scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
  {
    GetInstance().scroll_callback(window,xoffset,yoffset);
  }

  void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
  {
    double posx, posy,vpx,vpy;

    glfwGetCursorPos(window,&posx,&posy);

    m_ViewSettings->ScreenToViewPortTransform(posx,posy,vpx,vpy);

    otb::StandardShader::Pointer shader = dynamic_cast<otb::StandardShader *> (m_Actors[m_SelectedActor]->GetShader());

    otb::ViewSettings::PointType zoomCenter;
    zoomCenter[0] = vpx;
    zoomCenter[1] = vpy;

    if(glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
      {
      double delta = yoffset > 0 ? 1/1.1 : 1.1;
      if(m_LocalContrastEnhancement)
        {
        m_LocalContrastWindow*=delta;

        SetupLocalContrastEnhancement(window);
        }
      else
        {
        shader->SetMinRed(shader->GetMinRed()*delta);
        shader->SetMinGreen(shader->GetMinGreen()*delta);
        shader->SetMinBlue(shader->GetMinBlue()*delta);
        shader->SetMaxRed(shader->GetMaxRed()*delta);
        shader->SetMaxGreen(shader->GetMaxGreen()*delta);
        shader->SetMaxBlue(shader->GetMaxBlue()*delta);
        }
    }
    else if(glfwGetKey(window,GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
      {
      double delta = yoffset > 0 ? 1/1.1 : 1.1;
      
      shader->SetMinRed(shader->GetMinRed()*delta);
      shader->SetMinGreen(shader->GetMinGreen()*delta);
      shader->SetMinBlue(shader->GetMinBlue()*delta);
      shader->SetMaxRed(shader->GetMaxRed()/delta);
      shader->SetMaxGreen(shader->GetMaxGreen()/delta);
      shader->SetMaxBlue(shader->GetMaxBlue()/delta);
      }
    else
      {
      if(yoffset>0)
        {
        m_ViewSettings->Zoom(zoomCenter,1/1.1);
        }
      else if(yoffset < 0)
        {
        m_ViewSettings->Zoom(zoomCenter,1.1);
        }
      }
  }

  void mouse_button_callback(GLFWwindow * window, int button, int action, int mode)
  {
    if(button == GLFW_MOUSE_BUTTON_1)
      {
      if(action == GLFW_PRESS)
        {
        m_Drag = true;
        // double posx,posy;
        glfwGetCursorPos(window,&m_StartDragX,&m_StartDragY);
        // std::cout<<posx<<" "<<posy<<std::endl;
        // m_ViewSettings->ScreenToViewPortTransform(posx,posy,m_StartDragX,m_StartDragY);

        // TODO: Move drag to dedicated method in ViewSettings
        otb::ViewSettings::PointType origin = m_ViewSettings->GetOrigin();
        m_OriginDragX = origin[0];
        m_OriginDragY = origin[1];

        }
      else if(action == GLFW_RELEASE)
        {
        m_Drag = false;
        
        otb::ViewSettings::PointType origin;
        origin[0]=m_OriginDragX-m_DeltaDragX*m_ViewSettings->GetSpacing()[0];
        origin[1]=m_OriginDragY+m_DeltaDragY*vcl_abs(m_ViewSettings->GetSpacing()[1]);

        m_ViewSettings->SetOrigin(origin);

        m_StartDragX = 0;
        m_StartDragY = 0;
        m_DeltaDragX = 0;
        m_DeltaDragY = 0;
        m_OriginDragX = 0;
        m_OriginDragY = 0;
        }
      }
  }


  void cursor_pos_callback(GLFWwindow * window, double xpos, double ypos)
  { 
    double posx, posy, vpx, vpy;
    glfwGetCursorPos(window,&posx,&posy);

    if(m_LocalContrastEnhancement)
      {
      SetupLocalContrastEnhancement(window);
      }

    if(m_Drag)
      {
      m_DeltaDragX = posx - m_StartDragX;
      m_DeltaDragY = posy - m_StartDragY;

      otb::ViewSettings::PointType origin;
      origin[0]=m_OriginDragX-m_DeltaDragX*m_ViewSettings->GetSpacing()[0];
      origin[1]=m_OriginDragY+m_DeltaDragY*vcl_abs(m_ViewSettings->GetSpacing()[1]);
      
      m_ViewSettings->SetOrigin(origin);
      } 
  }

  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    // We get events twice for some reason
    // if(m_KeyFilter % 2 == 1)
    //   {
    //   ++m_KeyFilter;
    //   return;
    //   }
    // ++m_KeyFilter;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);

    if(key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
      {
      std::cout<<"Fast rendering on"<<std::endl;
      m_FastRendering = true;
      }
    else if(key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE)
      {
      std::cout<<"Fast rendering off"<<std::endl;
      m_FastRendering = false;
      }
    
    double ulx,uly,lrx,lry;
    m_ViewSettings->GetViewportExtent(ulx,uly,lrx,lry);

    otb::ViewSettings::PointType origin = m_ViewSettings->GetOrigin();

    double deltax = (lrx-ulx)/4;
    double deltay = (lry-uly)/4;
    
    if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
      {
      origin[0]-=deltax; 
      m_ViewSettings->SetOrigin(origin);
      }

    if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
      {
      origin[0]+=deltax; 
      m_ViewSettings->SetOrigin(origin);
      }

    if(key == GLFW_KEY_UP && action == GLFW_PRESS)
      {
      origin[1]+=deltay; 
      m_ViewSettings->SetOrigin(origin);
      }

    if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
      {
      origin[1]-=deltay; 
      m_ViewSettings->SetOrigin(origin);
      }

    if(key == GLFW_KEY_KP_ADD )
      {
      otb::ViewSettings::PointType zoomCenter;
      zoomCenter[0] = 0.5*(ulx+lrx);
      zoomCenter[1] = 0.5*(uly+lry);
      m_ViewSettings->Zoom(zoomCenter,1/1.1);

      }

    if(key == GLFW_KEY_KP_SUBTRACT)
      {
      otb::ViewSettings::PointType zoomCenter;
      zoomCenter[0] = 0.5*(ulx+lrx);
      zoomCenter[1] = 0.5*(uly+lry);
      m_ViewSettings->Zoom(zoomCenter,1.1);
      }

    if(key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS)
      {
      //m_View->RotateRenderingOrder(false);
      std::cout<<"Moving actor to the front"<<std::endl;
      m_View->MoveActorInRenderingOrder(m_ActorsNames[m_SelectedActor],false);
      }

    if(key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS)
      {
      std::cout<<"Moving actor to the back"<<std::endl;
      //m_View->RotateRenderingOrder(true);
      m_View->MoveActorInRenderingOrder(m_ActorsNames[m_SelectedActor],true);
      }
   
    if(key == GLFW_KEY_TAB && action == GLFW_PRESS)
      {
      m_View->RotateRenderingOrder(true);
      }

    if(key == GLFW_KEY_1 && action == GLFW_PRESS)
      {
      if(m_Actors.size() > 0)
        {
        m_SelectedActor = 0;
        std::cout<<"Actor 1 selected"<<" ("<<m_ActorsNames[0]<<")"<<std::endl;
        }
      }
    if(key == GLFW_KEY_2 && action == GLFW_PRESS)
      {
      
      if(m_Actors.size() > 1)
        {
        m_SelectedActor = 1;
        std::cout<<"Actor 2 selected"<<"("<<m_ActorsNames[1]<<")"<<std::endl;
        }
      }
    if(key == GLFW_KEY_3 && action == GLFW_PRESS)
      {
      
      if(m_Actors.size() > 2)
        {
        m_SelectedActor = 2;
        std::cout<<"Actor 3 selected"<<"("<<m_ActorsNames[2]<<")"<<std::endl;
        }
      }
    if(key == GLFW_KEY_4 && action == GLFW_PRESS)
      {
      
      if(m_Actors.size() > 3)
        {
        m_SelectedActor = 3;
        std::cout<<"Actor 4 selected"<<"("<<m_ActorsNames[3]<<")"<<std::endl;
        }
      }
    if(key == GLFW_KEY_4 && action == GLFW_PRESS)
      {
      
      if(m_Actors.size() > 4)
        {
        m_SelectedActor = 4;
        std::cout<<"Actor 5 selected"<<"("<<m_ActorsNames[4]<<")"<<std::endl;
        }
      }

    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
      {
      if(m_Actors.size()>m_SelectedActor)
        {
        std::cout<<"Actor visibility "<<(m_Actors[m_SelectedActor]->GetVisible()?"off":"on")<<std::endl;
        m_Actors[m_SelectedActor]->SetVisible(!m_Actors[m_SelectedActor]->GetVisible());
        }
      }

     if(key == GLFW_KEY_R && action == GLFW_PRESS)
      {
      if(m_Actors.size()>m_SelectedActor)
        {
        unsigned int red = 1+(m_Actors[m_SelectedActor]->GetRedIdx())%m_Actors[m_SelectedActor]->GetNumberOfComponents();
        std::cout<<"Changing actor red component to "<<red<<std::endl;
        m_Actors[m_SelectedActor]->SetRedIdx(red);
        }
      }

     if(key == GLFW_KEY_G && action == GLFW_PRESS)
      {
      if(m_Actors.size()>m_SelectedActor)
        {
        unsigned int green = 1+(m_Actors[m_SelectedActor]->GetGreenIdx())%m_Actors[m_SelectedActor]->GetNumberOfComponents();
        std::cout<<"Changing actor red component to "<<green<<std::endl;
        m_Actors[m_SelectedActor]->SetGreenIdx(green);
        }
      }

     if(key == GLFW_KEY_B && action == GLFW_PRESS)
      {
      if(m_Actors.size()>m_SelectedActor)
        {
        unsigned int blue = 1+(m_Actors[m_SelectedActor]->GetBlueIdx())%m_Actors[m_SelectedActor]->GetNumberOfComponents();
        std::cout<<"Changing actor red component to "<<blue<<std::endl;
        m_Actors[m_SelectedActor]->SetBlueIdx(blue);
        }
      }
     
     if(key == GLFW_KEY_L && action == GLFW_PRESS)
       {
       otb::StandardShader::Pointer shader = static_cast<otb::StandardShader *>(m_Actors[m_SelectedActor]->GetShader());

       std::cout<<"Local contrast enhancement "<<(m_LocalContrastEnhancement?"off":"on")<<std::endl;
       m_LocalContrastEnhancement = !m_LocalContrastEnhancement;
       if(!m_LocalContrastEnhancement)
         {
         shader->SetMinRed(m_MinRed);
         shader->SetMinGreen(m_MinGreen);
         shader->SetMinBlue(m_MinBlue);
         shader->SetMaxRed(m_MaxRed);
         shader->SetMaxGreen(m_MaxGreen);
         shader->SetMaxBlue(m_MaxBlue);
         }
       else
         {
         m_MinRed = shader->GetMinRed();
         m_MinGreen = shader->GetMinGreen();
         m_MinBlue = shader->GetMinBlue();
         m_MaxRed = shader->GetMaxRed();
         m_MaxGreen = shader->GetMaxGreen();
         m_MaxBlue = shader->GetMaxBlue();

         SetupLocalContrastEnhancement(window);
         }
       }

     // Not available anymore with GlImageActor (use NonOptGlImageActor)
     // if(key == GLFW_KEY_S && action == GLFW_PRESS)
     //   {
     //   std::cout<<"Actor shader rendering "<<(m_Actors[m_SelectedActor]->GetUseShader()?"off":"on")<<std::endl;
     //   m_Actors[m_SelectedActor]->SetUseShader(!m_Actors[m_SelectedActor]->GetUseShader());
     //   }
  }


  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    otb::ViewSettings::SizeType size;
    size[0] = width;
    size[1] = height;

    m_ViewSettings->SetViewportSize(size);
  }

  void SetupLocalContrastEnhancement(GLFWwindow* window)
  {
    double posx, posy, vpx, vpy;
    glfwGetCursorPos(window,&posx,&posy);
    glfwGetCursorPos(window,&posx,&posy);
    
    m_ViewSettings->ScreenToViewPortTransform(posx,posy,vpx,vpy);
    
    otb::ViewSettings::PointType p;
    p[0] = vpx;
    p[1] = vpy;
    
    
    otb::GlImageActor::PixelType pixel;
    bool inside = m_Actors[m_SelectedActor]->GetPixelFromViewport(p,pixel);
    
    otb::StandardShader::Pointer shader = static_cast<otb::StandardShader *>(m_Actors[m_SelectedActor]->GetShader());

    if(inside)
      {
      shader->SetMinRed(pixel[m_Actors[m_SelectedActor]->GetRedIdx()-1]-m_LocalContrastWindow);
      shader->SetMinGreen(pixel[m_Actors[m_SelectedActor]->GetGreenIdx()-1]-m_LocalContrastWindow);
      shader->SetMinBlue(pixel[m_Actors[m_SelectedActor]->GetBlueIdx()-1]-m_LocalContrastWindow);
      shader->SetMaxRed(pixel[m_Actors[m_SelectedActor]->GetRedIdx()-1]+m_LocalContrastWindow);
      shader->SetMaxGreen(pixel[m_Actors[m_SelectedActor]->GetGreenIdx()-1]+m_LocalContrastWindow);
      shader->SetMaxBlue(pixel[m_Actors[m_SelectedActor]->GetBlueIdx()-1]+m_LocalContrastWindow);
      }
    else
      {
      shader->SetMinRed(m_MinRed);
      shader->SetMinGreen(m_MinGreen);
      shader->SetMinBlue(m_MinBlue);
      shader->SetMaxRed(m_MaxRed);
      shader->SetMaxGreen(m_MaxGreen);
      shader->SetMaxBlue(m_MaxBlue);
      }
  }

private:
  unsigned int m_KeyFilter;

  Manipulator()
  {
    m_KeyFilter=0;
    m_FastRendering = false;
    m_LocalContrastEnhancement = false;
    m_LocalContrastWindow = 100;
  }

  Manipulator(Manipulator const&);
  void operator=(Manipulator const&);

  otb::ViewSettings::Pointer m_ViewSettings;

  std::vector<otb::GlImageActor::Pointer> m_Actors;
  std::vector<std::string> m_ActorsNames;

  otb::GlView::Pointer m_View;
  
  bool   m_Drag;
  bool   m_FastRendering;
  bool   m_LocalContrastEnhancement;
  double m_LocalContrastWindow;
  double m_StartDragX;
  double m_StartDragY;
  double m_DeltaDragX;
  double m_DeltaDragY;
  double m_OriginDragX;
  double m_OriginDragY;
  double m_ZoomCenterX;
  double m_ZoomCenterY;
  double m_MinRed;
  double m_MinGreen;
  double m_MinBlue;
  double m_MaxRed;
  double m_MaxGreen;
  double m_MaxBlue;
  unsigned int m_SelectedActor;
};



void error_callback(int error, const char* description)
{
  std::cerr<<description<<std::endl;
}

int main(int argc, char * argv[])
{
  if(argc < 4)
    {
    std::cerr<<"Usage: "<<argv[0]<<" init_min init_max img1 [img2 img3 img4 img5]"<<std::endl<<std::endl;
    std::cerr<<"Images can be of arbitrary sizes and geometries, as long as OTB knows how to pass from one geometry to the other (projref, keyworlist or homotethic images). The viewport geometry will be the one of the first image."<<std::endl;
    std::cerr<<"DEM directory and geoid files can be set through the OTB_DEM_DIR and OTB_GEOID_FILE environment variable"<<std::endl;
    std::cerr<<"Hooks:"<<std::endl;
    std::cerr<<"- Mouse drag: navigate"<<std::endl;
    std::cerr<<"- Mouse wheel: zoom in/out"<<std::endl;
    std::cerr<<"- CTRL key presed: fast rendering (no data loading). Useful for fast zoom for instance"<<std::endl;
    std::cerr<<"- Arrows keys: navigate"<<std::endl;
    std::cerr<<"- + or - keys: zoom in/out"<<std::endl;
    std::cerr<<"- 1 to 5 keys: select image actor 1 to 5 (ordered as received in command-line"<<std::endl;
    std::cerr<<"- SPACE bar: toogle selected actor visiblity"<<std::endl;
    std::cerr<<"- SHIFT key + mouse wheel: tune intensity of current actor"<<std::endl;
    std::cerr<<"- ALT key + mouse wheel: tune contrast of current actor"<<std::endl;
    std::cerr<<"- r key: change red channel of current actor"<<std::endl;
    std::cerr<<"- g key: change green channel of current actor"<<std::endl;
    std::cerr<<"- b key: change blue channel of current actor"<<std::endl;
    // std::cerr<<"- s key: enable / disable shader rendering of
    // current actor (compare speed!)"<<std::endl;
    std::cerr<<"- L key: activate/deactivate pixel under mouse local contrast enhancement (in this mode, SHIFT + mouse wheel allows to tune the contrast enhancement range"<<std::endl;
    std::cerr<<"- PAGE_UP/PAGE_DOWN key: move actor backward or forward in rendering order"<<std::endl;
    std::cerr<<"- TAB key: rotate rendering order"<<std::endl;
    return EXIT_FAILURE;
    }

  char * demdir = getenv("OTB_DEM_DIR");
  char * geoidfile = getenv("OTB_GEOID_FILE");

  otb::DEMHandler::Pointer demHandler = otb::DEMHandler::Instance();
  
  if(demdir != NULL)
    {
    std::cout<<"Configuring DEM directory: "<<demdir<<std::endl;
    demHandler->OpenDEMDirectory(demdir);
    }

  if(geoidfile != NULL)
    {
    std::cout<<"Configuring geoid file: "<<geoidfile<<std::endl;
    demHandler->OpenGeoidFile(geoidfile);
    }

  if(!glfwInit())
    {
    std::cerr<<"Could not initalize glfw!"<<std::endl;
    return EXIT_FAILURE;
    }

  glfwSetErrorCallback(error_callback);

// Open a window and create its OpenGL context
   GLFWwindow* window;

   window = glfwCreateWindow(640, 480,"I see", NULL, NULL);
   if (!window)
     {
     glfwTerminate();
     exit(EXIT_FAILURE);
     }

   glfwMakeContextCurrent(window);
   
   glfwSetKeyCallback(window, &Manipulator::static_key_callback);
   glfwSetFramebufferSizeCallback(window,&Manipulator::static_framebuffer_size_callback);
   glfwSetMouseButtonCallback(window,&Manipulator::static_mouse_button_callback);
   glfwSetCursorPosCallback(window,&Manipulator::static_cursor_pos_callback);
   glfwSetScrollCallback(window,&Manipulator::static_scroll_callback);

   otb::GlView::Pointer glView = otb::GlView::New();
   glView->Initialize(640,480);

   Manipulator::GetInstance().SetViewSettings(glView->GetSettings());

   double min = atof(argv[1]);
   double max = atof(argv[2]);

   std::vector<otb::GlImageActor::Pointer> actors;
   std::vector<std::string> actorsNames;

   otb::GlImageActor::Pointer mainActor = otb::GlImageActor::New();
   mainActor->Initialize(argv[3]);
   otb::StandardShader::Pointer shader = static_cast<otb::StandardShader *>(mainActor->GetShader());

   mainActor->SetVisible(true);
   shader->SetMinRed(min);
   shader->SetMinGreen(min);
   shader->SetMinBlue(min);
   shader->SetMaxRed(max);
   shader->SetMaxGreen(max);
   shader->SetMaxBlue(max);
   glView->AddActor(mainActor,std::string(argv[3]));
   actorsNames.push_back(std::string(argv[3]));

   // Initialize view with respect to first actor
   glView->GetSettings()->SetOrigin(mainActor->GetOrigin());
   glView->GetSettings()->SetSpacing(mainActor->GetSpacing());
   glView->GetSettings()->SetWkt(mainActor->GetWkt());
   glView->GetSettings()->SetKeywordList(mainActor->GetKwl());
   glView->GetSettings()->UseProjectionOn();

   double ulx,uly,lrx,lry;

   mainActor->GetExtent(ulx,uly,lrx,lry);
   actors.push_back(mainActor);

   for(unsigned int i = 4; i < argc;++i)
     {
     otb::GlImageActor::Pointer actor = otb::GlImageActor::New();
     actor->Initialize(argv[i]);
     actor->SetVisible(true);
     
     shader = static_cast<otb::StandardShader *>(actor->GetShader());

     shader->SetMinRed(min);
     shader->SetMinGreen(min);
     shader->SetMinBlue(min);
     shader->SetMaxRed(max);
     shader->SetMaxGreen(max);
     shader->SetMaxBlue(max);
     glView->AddActor(actor,std::string(argv[i]));
     actors.push_back(actor);
     actorsNames.push_back(std::string(argv[i]));
     }

   Manipulator::GetInstance().SetActors(actors,actorsNames);
   Manipulator::GetInstance().SetView(glView);

   otb::ViewSettings::PointType center;
   center[0] = 0.5*(ulx+lrx);
   center[1] = 0.5*(uly+lry);
   
   glView->GetSettings()->Center(center);

   while (!glfwWindowShouldClose(window))
     {
     int width, height;
     glfwGetFramebufferSize(window, &width, &height);

     glView->BeforeRendering();

     if(Manipulator::GetInstance().IsDragging() || Manipulator::GetInstance().IsFastRendering())
       {
       glView->LightRender();
       }
     else
       {
       glView->HeavyRender();
       }

     glView->AfterRendering();

    // Swap buffers
    glfwSwapBuffers(window);
    
    glfwWaitEvents();
    }
  
  glfwDestroyWindow(window);
  glfwTerminate();
  
  return EXIT_SUCCESS;
}
