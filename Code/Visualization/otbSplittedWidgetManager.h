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
#ifndef __otbSplittedWidgetManager_h
#define __otbSplittedWidgetManager_h

#include "otbWidgetManager.h"
#include "otbImageWidgetSplittedManager.h"

namespace otb
{
/** \class SplittedWidgetManager
*   \brief todo
*
*  \ingroup Visualization
 */
class SplittedWidgetManager
  : public WidgetManager
{
public:
  /** Standard class typedefs */
  typedef SplittedWidgetManager         Self;
  typedef WidgetManager                 Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(SplittedWidgetManager, WidgetManager);

  /** typedef of the fltk file defining all the widget needed*/
  typedef ImageWidgetSplittedManager ImageWidgetSplittedManagerType;

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
  virtual void SetLabelHistogramWidget(const char * label);
  virtual void SetLabelPixelDescriptionWidget(const char * label);
  virtual int  ZoomWidgetShown();
  virtual int  FullWidgetShown();
  virtual void SetResizable(bool resize);

protected:
  /** Constructor */
  SplittedWidgetManager();

  /** Destructor */
  virtual ~SplittedWidgetManager();
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  SplittedWidgetManager(const Self&);     // purposely not implemented
  void operator =(const Self&); // purposely not implemented

  ImageWidgetSplittedManagerType* m_SplittedWidget;
};
}
#endif
