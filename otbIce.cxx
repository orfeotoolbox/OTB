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
    std::cout<<posx<<" "<<posy<<std::endl;

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
    if(m_KeyEventLock)
      {
      return;
      }
    
    m_KeyEventLock = true;

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

    // if(key == GLFW_KEY_KP_ADD )
    //   {
    //   m_ZoomCenterX = (m_ULX+m_LRX)*0.5;
    //   m_ZoomCenterY = (m_ULY+m_LRY)*0.5;

    //   Zoom(1/1.1);

    //   }

    // if(key == GLFW_KEY_KP_SUBTRACT)
    //   {
    //   m_ZoomCenterX = (m_ULX+m_LRX)*0.5;
    //   m_ZoomCenterY = (m_ULY+m_LRY)*0.5;

    //   Zoom(1.1);
    //   }

    m_KeyEventLock = false;
  }

  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {

  }

private:
  bool m_KeyEventLock;

  Manipulator()
  {}

  Manipulator(Manipulator const&);
  void operator=(Manipulator const&);

  otb::ViewSettings::Pointer m_ViewSettings;

  
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

   otb::GlImageActor::Pointer actor = otb::GlImageActor::New();
   actor->Initialize(argv[1]);
   actor->SetVisible(true);
   std::string label = glView->AddActor(actor);

   // Initialize view with respect to first actor
   glView->GetSettings()->SetOrigin(actor->GetOrigin());
   glView->GetSettings()->SetSpacing(actor->GetSpacing());
   glView->GetSettings()->SetWkt(actor->GetWkt());
   glView->GetSettings()->SetKeywordList(actor->GetKwl());

   while (!glfwWindowShouldClose(window))
     {
     int width, height;
     glfwGetFramebufferSize(window, &width, &height);

     glView->Resize(width,height);

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
