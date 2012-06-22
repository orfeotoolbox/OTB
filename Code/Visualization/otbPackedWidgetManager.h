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
#ifndef __otbPackedWidgetManager_h
#define __otbPackedWidgetManager_h

#include "otbWidgetManager.h"
#include "otbImageWidgetPackedManager.h"

#include <FL/Fl_Double_Window.H>

namespace otb
{
/** \class PackedWidgetManager
 *   \brief todo
 *
 *  \ingroup Visualization
 */
class PackedWidgetManager
  : public WidgetManager
{
public:
  /** Standard class typedefs */
  typedef PackedWidgetManager           Self;
  typedef WidgetManager                 Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(PackedWidgetManager, WidgetManager);

  /** typedef of the fltk file defining all the widget needed*/
  typedef ImageWidgetPackedManager ImageWidgetPackedManagerType;

  /** Register Wigets*/
  virtual void RegisterFullWidget(WidgetType* fullWidget);
  virtual void RegisterScrollWidget(WidgetType* scrollWidget);
  virtual void RegisterZoomWidget(WidgetType* zoomWidget);
  virtual void RegisterHistogramWidget(WidgetType* curveWidget);
  virtual void RegisterPixelDescriptionWidget(Fl_Output * pixelWidget);
  virtual void Show();
  virtual void Hide();
  virtual void Refresh();
  virtual void SetLabel(const char * label);
  virtual void UnRegisterAll();
  virtual void SetLabelScrollWidget(const char * label);
  virtual void SetLabelFullWidget(const char * label);
  virtual void SetLabelZoomWidget(const char * label);
  virtual int Shown();

protected:
  /** Constructor */
  PackedWidgetManager();

  /** Destructor */
  virtual ~PackedWidgetManager();
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  PackedWidgetManager(const Self&);     // purposely not implemented
  void operator =(const Self&); // purposely not implemented

  ImageWidgetPackedManagerType* m_PackedWidget;

};
}
#endif
