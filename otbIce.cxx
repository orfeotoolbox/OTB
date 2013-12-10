#include <GLFW/glfw3.h>
#include "otbGlView.h"
#include "otbGlImageActor.h"

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

   otb::GlView::Pointer glView = otb::GlView::New();
   glView->Initialize(640,480);

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
     glView->HeavyRender();
     glView->AfterRendering();

    // Swap buffers
    glfwSwapBuffers(window);
    
    glfwWaitEvents();
    }
  
  glfwDestroyWindow(window);
  glfwTerminate();
  
  return EXIT_SUCCESS;
}
