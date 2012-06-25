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
#ifndef __otbWidgetManager_h
#define __otbWidgetManager_h

#include "itkObject.h"
#include "otbMacro.h"
#include "otbGlWidget.h"
#include <FL/Fl_Output.H>

namespace otb
{
/** \class WidgetManager
*   \brief  Base class for displays
*
*  \ingroup Visualization
 */

class WidgetManager
  : public itk::Object
{
public:
  /** Standard class typedefs */
  typedef WidgetManager                 Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Runtime information */
  itkTypeMacro(WidgetManager, Object);

  /** Image widget */
  typedef otb::GlWidget       WidgetType;
  typedef WidgetType::Pointer WidgetPointerType;

  /** Register Wigets*/
  virtual void RegisterFullWidget(WidgetType* fullWidget) = 0;
  virtual void RegisterScrollWidget(WidgetType* scrollWidget) = 0;
  virtual void RegisterZoomWidget(WidgetType* zoomWidget) = 0;
  virtual void RegisterHistogramWidget(WidgetType* curveWidget) = 0;
  virtual void RegisterPixelDescriptionWidget(Fl_Output * pixelWidget) = 0;
  virtual void Show() = 0;
  virtual void Hide() = 0;
  virtual void Refresh() = 0;
  virtual void SetLabel(const char * label) = 0;
  virtual void UnRegisterAll() = 0;
  virtual void SetLabelScrollWidget(const char * label) = 0;
  virtual void SetLabelFullWidget(const char * label) = 0;
  virtual void SetLabelZoomWidget(const char * label) = 0;
  virtual void SetResizable(bool resize) = 0;

protected:
  /** Constructor */
  WidgetManager() {}
  /** Destructor */
  virtual ~WidgetManager(){}
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

  WidgetType* m_InternalFullWidget;
  WidgetType* m_InternalScrollWidget;
  WidgetType* m_InternalZoomWidget;
  WidgetType* m_InternalCurveWidget;
  Fl_Output * m_IntenalPixelWidget;

private:
  WidgetManager(const Self&);     // purposely not implemented
  void operator =(const Self&); // purposely not implemented

}; // end class
} // end namespace otb

#endif
