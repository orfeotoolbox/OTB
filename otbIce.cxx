#include <GLFW/glfw3.h>
#include "otbGlView.h"
#include "otbGlImageActor.h"
#include "otbViewSettings.h"

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

  void SetActors(const std::vector<otb::GlImageActor::Pointer> & actors)
  {
    m_Actors = actors;
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

    otb::ViewSettings::PointType zoomCenter;
    zoomCenter[0] = vpx;
    zoomCenter[1] = vpy;

    if(glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
      {
      double delta = yoffset > 0 ? 1/1.1 : 1.1;
      
      m_Actors[m_SelectedActor]->SetMinRed(m_Actors[m_SelectedActor]->GetMinRed()*delta);
      m_Actors[m_SelectedActor]->SetMinGreen(m_Actors[m_SelectedActor]->GetMinGreen()*delta);
      m_Actors[m_SelectedActor]->SetMinBlue(m_Actors[m_SelectedActor]->GetMinBlue()*delta);
      m_Actors[m_SelectedActor]->SetMaxRed(m_Actors[m_SelectedActor]->GetMaxRed()*delta);
      m_Actors[m_SelectedActor]->SetMaxGreen(m_Actors[m_SelectedActor]->GetMaxGreen()*delta);
      m_Actors[m_SelectedActor]->SetMaxBlue(m_Actors[m_SelectedActor]->GetMaxBlue()*delta);
    }
    else if(glfwGetKey(window,GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
      {
      double delta = yoffset > 0 ? 1/1.1 : 1.1;
      
      m_Actors[m_SelectedActor]->SetMinRed(m_Actors[m_SelectedActor]->GetMinRed()*delta);
      m_Actors[m_SelectedActor]->SetMinGreen(m_Actors[m_SelectedActor]->GetMinGreen()*delta);
      m_Actors[m_SelectedActor]->SetMinBlue(m_Actors[m_SelectedActor]->GetMinBlue()*delta);
      m_Actors[m_SelectedActor]->SetMaxRed(m_Actors[m_SelectedActor]->GetMaxRed()/delta);
      m_Actors[m_SelectedActor]->SetMaxGreen(m_Actors[m_SelectedActor]->GetMaxGreen()/delta);
      m_Actors[m_SelectedActor]->SetMaxBlue(m_Actors[m_SelectedActor]->GetMaxBlue()/delta);
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
        origin[1]=m_OriginDragY+m_DeltaDragY*m_ViewSettings->GetSpacing()[1];

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

    if(m_Drag)
      {
      m_DeltaDragX = posx - m_StartDragX;
      m_DeltaDragY = posy - m_StartDragY;

      otb::ViewSettings::PointType origin;
      origin[0]=m_OriginDragX-m_DeltaDragX*m_ViewSettings->GetSpacing()[0];
      origin[1]=m_OriginDragY+m_DeltaDragY*m_ViewSettings->GetSpacing()[1];
      
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

    if(key == GLFW_KEY_1 && action == GLFW_PRESS)
      {
      if(m_Actors.size() > 0)
        {
        m_SelectedActor = 0;
        std::cout<<"Actor 1 selected"<<std::endl;
        }
      }
    if(key == GLFW_KEY_2 && action == GLFW_PRESS)
      {
      
      if(m_Actors.size() > 1)
        {
        m_SelectedActor = 0;
        std::cout<<"Actor 2 selected"<<std::endl;
        }
      }
    if(key == GLFW_KEY_3 && action == GLFW_PRESS)
      {
      
      if(m_Actors.size() > 2)
        {
        m_SelectedActor = 0;
        std::cout<<"Actor 3 selected"<<std::endl;
        }
      }
    if(key == GLFW_KEY_4 && action == GLFW_PRESS)
      {
      
      if(m_Actors.size() > 3)
        {
        m_SelectedActor = 0;
        std::cout<<"Actor 4 selected"<<std::endl;
        }
      }
    if(key == GLFW_KEY_4 && action == GLFW_PRESS)
      {
      
      if(m_Actors.size() > 4)
        {
        m_SelectedActor = 0;
        std::cout<<"Actor 5 selected"<<std::endl;
        }
      }

    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
      {
      if(m_Actors.size()>m_SelectedActor)
        {
        std::cout<<"Actor visibility "<<(m_Actors[m_SelectedActor]->GetVisible()?" off":"on")<<std::endl;
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

     if(key == GLFW_KEY_S && action == GLFW_PRESS)
       {
       std::cout<<"Actor shader rendering "<<(m_Actors[m_SelectedActor]->GetUseShader()?" off":"on")<<std::endl;
       m_Actors[m_SelectedActor]->SetUseShader(!m_Actors[m_SelectedActor]->GetUseShader());
       }
  }


  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    otb::ViewSettings::SizeType size;
    size[0] = width;
    size[1] = height;

    m_ViewSettings->SetViewportSize(size);
  }

private:
  unsigned int m_KeyFilter;

  Manipulator()
  {
    m_KeyFilter=0;
    m_FastRendering = false;
  }

  Manipulator(Manipulator const&);
  void operator=(Manipulator const&);

  otb::ViewSettings::Pointer m_ViewSettings;

  std::vector<otb::GlImageActor::Pointer> m_Actors;

  otb::GlView::Pointer m_View;
  
  bool   m_Drag;
  bool   m_FastRendering;
  double m_StartDragX;
  double m_StartDragY;
  double m_DeltaDragX;
  double m_DeltaDragY;
  double m_OriginDragX;
  double m_OriginDragY;
  double m_ZoomCenterX;
  double m_ZoomCenterY;
  unsigned int m_SelectedActor;
};



void error_callback(int error, const char* description)
{
  std::cerr<<description<<std::endl;
}

int main(int argc, char * argv[])
{
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

   otb::GlImageActor::Pointer mainActor = otb::GlImageActor::New();
   mainActor->Initialize(argv[3]);
   mainActor->SetVisible(true);
   mainActor->UseShaderOn();
   mainActor->SetMinRed(min);
   mainActor->SetMinGreen(min);
   mainActor->SetMinBlue(min);
   mainActor->SetMaxRed(max);
   mainActor->SetMaxGreen(max);
   mainActor->SetMaxBlue(max);
   glView->AddActor(mainActor);

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
     actor->UseShaderOn();
     actor->SetMinRed(min);
     actor->SetMinGreen(min);
     actor->SetMinBlue(min);
     actor->SetMaxRed(max);
     actor->SetMaxGreen(max);
     actor->SetMaxBlue(max);
     glView->AddActor(actor);
     actors.push_back(actor);
     }

   Manipulator::GetInstance().SetActors(actors);

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
