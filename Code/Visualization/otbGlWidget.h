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
#ifndef __otbGlWidget_h
#define __otbGlWidget_h

// FLTK includes
#include <FL/gl.h>
#include <FL/Fl.H>
#include "FL/Fl_Gl_Window.H"

#include "itkFixedArray.h"
#include "itkPoint.h"

#include "otbImageWidgetController.h"

namespace otb
{
/** \class GlWidget
*   \brief Base class for widgets using OpenGl
*
*   The SetUseGlAcceleration() allows you to disable Gl
*   acceleration. If OTB_USE_GL_ACCEL is OFF, enabling Gl acceleration
*   will generate an exception.
*   Using Gl acceleration allows you to have a better rendering when
*   zooming.
*
*  \sa ImageWidget
*  \ingroup Visualization
 */

class GlWidget
  : public Fl_Gl_Window, public itk::Object
{
public:
  /** Standard class typedefs */
  typedef GlWidget                      Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(GlWidget, Object);

  /** Controller typedef */
  typedef otb::ImageWidgetController ControllerType;
  typedef ControllerType::Pointer    ControllerPointerType;

  /** Index typedef */
  typedef itk::Point<double, 2> PointType;

  /** Color typedef (used to draw the rectangle, 4th channel is alpha) */
  typedef itk::FixedArray<float, 4> ColorType;

  /** Set/Get the Controller */
  itkSetObjectMacro(Controller, ControllerType);
  itkGetObjectMacro(Controller, ControllerType);

  /** Handle resizing event. This method is used by FLTK routines and
   *  should not be called on its own.
   */
  virtual void resize(int x, int y, int w, int h);

  /** Enable/disable Gl acceleration */
  itkSetMacro(UseGlAcceleration, bool);
  itkGetMacro(UseGlAcceleration, bool);
  itkBooleanMacro(UseGlAcceleration);

  /** Set/Get the identifier */
  itkSetStringMacro(Identifier);
  itkGetStringMacro(Identifier);

  /** Set/Get the background color */
  itkSetMacro(BackgroundColor, ColorType);
  itkGetMacro(BackgroundColor, ColorType);

  /** Fltk y axis is flipped, therefore we use this function to get
   *  the cursor position using gl axis */
  PointType GetMousePosition();

protected:
  /** Constructor */
  GlWidget();
  /** Destructor */
  virtual ~GlWidget();
  /** Printself method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Actually render the buffer to the screen. This method is
    * used by FLTK routines and should not be called on its own.
    */
  virtual void draw(void);
  /** Handle the event from the users.  This method is used by FLTK
    * routines and should not be called on its own.
    */
  virtual int  handle(int event);

private:
  GlWidget(const Self&);    // purposely not implemented
  void operator =(const Self&); // purposely not implemented

  /** Widget identifier */
  std::string m_Identifier;

  /** Controller */
  ControllerPointerType m_Controller;

  /** Flag for GlAcceleration */
  bool m_UseGlAcceleration;

  /** The background color */
  ColorType m_BackgroundColor;

}; // end class
} // end namespace otb

#endif
