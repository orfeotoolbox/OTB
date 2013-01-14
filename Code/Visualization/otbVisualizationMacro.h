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
#ifndef __otbVisualizationMacro_h
#define __otbVisualizationMacro_h

/**
 * otbVisualizationMacro.h defines visualization related macros.
 */
#ifdef OTB_USE_VISU_GUI
#include <FL/Fl.H>

// Replace th Fl::run method
#define otbFlRunMacro(condition) { while (condition) { Fl::wait(1e20); } }

// Check if application running or will be stopped
#define otbRunningMacro() \
private: \
  virtual void StartRunning() {this->m_Running = true;  } \
  virtual void StopRunning()  {this->m_Running = false; } \
  bool m_Running; \
public: \
  virtual bool IsRunning() { return this->m_Running; }

#define otbTestCallbackWithValue(view, button, button_value) \
    { \
    std::cout << "Line " << __LINE__ << ", testing widget " << # button << " of view " << view->GetNameOfClass() << \
    " with value " << # button_value << ": "; \
    if (view->button->active() && (view->button->callback())) \
      { \
      std::cout << " active, triggering callback." << std::endl; \
      view->button->value(button_value); \
      view->button->do_callback(view->button); \
      Fl::check(); \
      } \
    else \
      { \
      std::cout << "inactive." << std::endl; \
      } \
    }

#define otbTestCallback(view, button) \
    { \
    std::cout << "Line " << __LINE__ << ", testing widget " << # button << " of view " << view->GetNameOfClass() << \
    ": "; \
    if (view->button->active() && (view->button->callback())) \
      { \
      std::cout << " active, triggering callback." << std::endl; \
      view->button->do_callback(view->button); \
      Fl::check(); \
      } \
    else \
      { \
      std::cout << "inactive." << std::endl; \
      } \
    }

#define otbTestMenuItemCallback(view, menubar, button) \
    { \
    std::cout << "Line " << __LINE__ << ", testing menu item " << # button << " from menu bar " << # menubar << \
    " of view " << view->GetNameOfClass() << ": "; \
    if (view->button->active() && (view->button->callback())) \
      { \
      std::cout << " active, triggering callback." << std::endl; \
      view->button->do_callback(view->menubar); \
      Fl::check(); \
      } \
    else \
      { \
      std::cout << "inactive." << std::endl; \
      } \
    }

#else

#define otbFlRunMacro(condition)
#define otbRunningMacro()
#define otbTestCallbackWithValue(view, button, button_value)
#define otbTestCallback(view, button)
#define otbTestMenuItemCallback(view, menubar, button)

#endif

#endif
