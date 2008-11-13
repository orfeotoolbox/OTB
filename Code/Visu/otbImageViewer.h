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
#ifndef __otbImageViewer_h
#define __otbImageViewer_h

#include "otbImageViewerBase.h"
#include <FL/Fl_Multiline_Output.H>

namespace otb
{
  /** 
   * \brief Simple image viewer.
   *
   * This class is a simple image viewer using fltk gui and opengl drawing.
   * It is divided into four parts :
   * - The full window that display a part of the image at full resolution,
   * - If necessary, a scroll window to allow the user to navigate through the whole image,
   * - A zoom window whose zoom can be adjusted with mouse scroll,
   * - a Pixel/value window to display information about the pointed pixel.
   *
   * The viewer uses the StreamingShrinkImageFilter to generate a quicklook of possibly very large image
   * without flooding the memory.
   *
   * Channel index and other various parameters can be set.
   * 
   * To use the viewer, setup it with all your parameters, then call the Build(), Show() and the Update() methods.
   * A call to Fl::run() is mandatory to hang the program execution until the viewer is closed.
   *
   * Unkike most OTB filters, ImageViewer is templated over the input pixel type and will take as input an otb::VectorImage<PixelType>.
   * Be aware of that.
   *
   * \sa ImageViewerFullWidget, ImageViewerScrollWidget, ImageViewerZoomWidget
   */
template <class TPixel, class TLabel = double>
class ITK_EXPORT ImageViewer 
  : public ImageViewerBase<TPixel,TLabel>
{
 public:
  /** Standard class typedefs */
  typedef ImageViewer    Self;
  typedef ImageViewerBase<TPixel,TLabel>     Superclass;
  typedef itk::SmartPointer<Self>            Pointer;
  typedef itk::SmartPointer<const Self>      ConstPointer;

 typedef typename Superclass::ViewModelType ViewModelType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageViewer,ImageViewerBase);
  
  /** Show the viewer */
  virtual void Show(void);
   /** Hide all Image View Windows */
  virtual void Hide(void);
  /** Build the interfaces */
  virtual void Build(void);  
  /** Update */
  virtual void Update(void);
 
  protected:
  /**Constructor */
  ImageViewer(); 
  /** Destructor */
  ~ImageViewer();
  
  private:
  ImageViewer(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  /// Fltk surrounding windows
  Fl_Window * m_FullWindow;
  Fl_Window * m_ScrollWindow;
  Fl_Window * m_ZoomWindow;
  Fl_Window * m_PixLocWindow;
  Fl_Window * m_HistogramWindow;
  Fl_Multiline_Output * m_PixLocOutput;
};
} // end namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageViewer.txx"
#endif
#endif
