#include "otbGPUTileLoader.h"
#include <GLFW/glfw3.h>


class ViewSettings
{
public:
  static ViewSettings & GetInstance()
  {
    static ViewSettings instance;
    return instance;
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
    if(yoffset>0)
      {
      Zoom(1/1.1);
      }
    else if(yoffset < 0)
      {
      Zoom(1.1);
      }
  }

  void mouse_button_callback(GLFWwindow * window, int button, int action, int mode)
  {
    if(button == GLFW_MOUSE_BUTTON_1)
      {
      if(action == GLFW_PRESS)
        {
        m_Drag = true;
        glfwGetCursorPos(window,&m_StartDragX,&m_StartDragY);
        }
      else if(action == GLFW_RELEASE)
        {
        m_Drag = false;

        m_ULX-=m_DeltaDragX;
        m_ULY+=m_DeltaDragY;
        m_LRX-=m_DeltaDragX;
        m_LRY+=m_DeltaDragY;

        m_StartDragX = 0;
        m_StartDragY = 0;
        m_DeltaDragX = 0;
        m_DeltaDragY = 0;
        }
      }
  }
  void cursor_pos_callback(GLFWwindow * window, double xpos, double ypos)
  {
    double posx, posy;
    glfwGetCursorPos(window,&posx,&posy);
    
    if(m_Drag)
      {
      m_DeltaDragX = posx - m_StartDragX;
      m_DeltaDragY = posy - m_StartDragY;
      }
    
    m_ZoomCenterX = posx;
    m_ZoomCenterY = posy;
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
    
    double deltax = (m_LRX-m_ULX)/4;
    double deltay = (m_LRY-m_ULY)/4;

    if(key == GLFW_KEY_LEFT)
      {
      m_ULX-=deltax; 
      m_LRX-=deltax; 
      }

    if(key == GLFW_KEY_RIGHT)
      {
      m_ULX+=deltax; 
      m_LRX+=deltax; 
      }

    if(key == GLFW_KEY_UP)
      {
      m_ULY+=deltay; 
      m_LRY+=deltay; 
      }

    if(key == GLFW_KEY_DOWN)
      {
      m_ULY-=deltay; 
      m_LRY-=deltay; 
      }

    if(key == GLFW_KEY_KP_ADD )
      {
      m_ZoomCenterX = (m_ULX+m_LRX)*0.5;
      m_ZoomCenterY = (m_ULY+m_LRY)*0.5;

      Zoom(1/1.1);

      }

    if(key == GLFW_KEY_KP_SUBTRACT)
      {
      m_ZoomCenterX = (m_ULX+m_LRX)*0.5;
      m_ZoomCenterY = (m_ULY+m_LRY)*0.5;

      Zoom(1.1);
      }

    m_KeyEventLock = false;
  }

  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    m_LRX = m_ULX + width;
    m_LRY = m_ULY + height;
  }


  double m_ULX;
  double m_ULY;
  double m_LRX;
  double m_LRY;
  double m_Scale;
  bool   m_Drag;
  double m_StartDragX;
  double m_StartDragY;
  double m_DeltaDragX;
  double m_DeltaDragY;
  double m_ZoomCenterX;
  double m_ZoomCenterY;

private:
  bool m_KeyEventLock;

  ViewSettings()
    : m_ULX(0),
      m_ULY(0),
      m_LRX(0),
      m_LRY(0),
      m_Scale(1.),
      m_Drag(false),
      m_StartDragX(0),
      m_StartDragY(0),
      m_DeltaDragX(0),
      m_DeltaDragY(0)
  {}
  ViewSettings(ViewSettings const&);
  void operator=(ViewSettings const&);

  void Zoom(double scale)
  {
    double w = m_LRX-m_ULX;
    double h = m_LRY-m_ULY;
    
    w*=scale;
    h*=scale;
    
    m_ULX = m_ZoomCenterX-w/2;
    m_LRX = m_ZoomCenterX+w/2;

    m_ULY = m_ZoomCenterY-h/2;
    m_LRY = m_ZoomCenterY+h/2;

    m_Scale = scale;

  }

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


   glfwSetKeyCallback(window, &ViewSettings::static_key_callback);
   glfwSetFramebufferSizeCallback(window,&ViewSettings::static_framebuffer_size_callback);
   glfwSetMouseButtonCallback(window,&ViewSettings::static_mouse_button_callback);
   glfwSetCursorPosCallback(window,&ViewSettings::static_cursor_pos_callback);
   glfwSetScrollCallback(window,&ViewSettings::static_scroll_callback);

 ViewSettings & settings = ViewSettings::GetInstance();

 settings.m_LRX = 640;
 settings.m_LRY = 480;

  otb::GPUTileLoader::Pointer tileLoader = otb::GPUTileLoader::New();

  tileLoader->Initialize(argv[1]);
  
  tileLoader->Update();

  glClearColor(0.5,0.5,0.5,1.0);
  glShadeModel(GL_FLAT);

  while (!glfwWindowShouldClose(window))
    {
     int width, height;
     glfwGetFramebufferSize(window, &width, &height);

     glViewport(0, 0, (GLint)width, (GLint)height);
   

     //std::cout<<"Settings: "<<settings.m_ULX<<" "<<settings.m_ULY<<" "<<settings.m_LRX<<" "<<settings.m_LRY<<std::endl;

     if(!settings.m_Drag)
       {
       tileLoader->UpdateViewport(settings.m_ULX,settings.m_ULY,settings.m_LRX,settings.m_LRY);
       tileLoader->Update();
       }

     // Clear back-buffer(s) before rendering sub-components.
    glClear( GL_COLOR_BUFFER_BIT );
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(settings.m_ULX, settings.m_LRX, settings.m_ULY, settings.m_LRY, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();

    if(settings.m_Drag)
      {
      glTranslatef(settings.m_DeltaDragX,-settings.m_DeltaDragY,0);
      }
    
    glScalef(settings.m_Scale,settings.m_Scale,1);
    // // Render current loader tiles
    tileLoader->Render();
        
    glPopMatrix();
    
    // Swap buffers
    glfwSwapBuffers(window);
    
    glfwWaitEvents();
    }
  
  glfwDestroyWindow(window);
  glfwTerminate();
  
  return EXIT_SUCCESS;
}
