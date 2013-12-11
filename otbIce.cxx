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

    if(yoffset>0)
      {
      m_ViewSettings->Zoom(zoomCenter,1/1.1);
      }
    else if(yoffset < 0)
      {
      m_ViewSettings->Zoom(zoomCenter,1.1);
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
    if(m_KeyFilter % 2 == 1)
      {
      ++m_KeyFilter;
      return;
      }
    ++m_KeyFilter;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
    
    double ulx,uly,lrx,lry;
    m_ViewSettings->GetViewportExtent(ulx,uly,lrx,lry);

    otb::ViewSettings::PointType origin = m_ViewSettings->GetOrigin();

    double deltax = (lrx-ulx)/4;
    double deltay = (lry-uly)/4;
    
    if(key == GLFW_KEY_LEFT)
      {
      origin[0]-=deltax; 
      m_ViewSettings->SetOrigin(origin);
      }

    if(key == GLFW_KEY_RIGHT)
      {
      origin[0]+=deltax; 
      m_ViewSettings->SetOrigin(origin);
      }

    if(key == GLFW_KEY_UP)
      {
      origin[1]+=deltay; 
      m_ViewSettings->SetOrigin(origin);
      }

    if(key == GLFW_KEY_DOWN)
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

    if(key == GLFW_KEY_1)
      {
      std::cout<<"Key 1 actors size: "<<m_Actors.size()<<std::endl;
      std::cout<<"Visible: "<<m_Actors[0]->GetVisible()<<std::endl;
      if(m_Actors.size() > 0)
        {
        m_Actors[0]->SetVisible(!m_Actors[0]->GetVisible());
        }
      }
    if(key == GLFW_KEY_2)
      {
      std::cout<<"Key 2 actors size: "<<m_Actors.size()<<std::endl;
      std::cout<<"Visible: "<<m_Actors[1]->GetVisible()<<std::endl;
      if(m_Actors.size() > 1)
        {
        m_Actors[1]->SetVisible(!m_Actors[1]->GetVisible());
        }

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
  }

  Manipulator(Manipulator const&);
  void operator=(Manipulator const&);

  otb::ViewSettings::Pointer m_ViewSettings;

  std::vector<otb::GlImageActor::Pointer> m_Actors;

  otb::GlView::Pointer m_View;
  
  bool   m_Drag;
  double m_StartDragX;
  double m_StartDragY;
  double m_DeltaDragX;
  double m_DeltaDragY;
  double m_OriginDragX;
  double m_OriginDragY;
  double m_ZoomCenterX;
  double m_ZoomCenterY;
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

     if(Manipulator::GetInstance().IsDragging())
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
