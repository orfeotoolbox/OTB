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
#ifndef __otbPixelDescription_h
#define __otbPixelDescription_h

#include "otbImageWidget.h"
#include "otbImageLayerRenderingModelListener.h"
#include "otbImageWidgetController.h"

#include <FL/Fl_Multiline_Output.H>

namespace otb
{

/** \class PixelDescriptionView
*   \brief 
*   \Todo: Rename PixelDescriptioner when refactoring will be completed.
*/

template <class TModel >
class PixelDescriptionView
  : public PixelDescriptionModelListener, public itk::Object
{
public:
  /** Standard class typedefs */
  typedef PixelDescriptionView                 Self;
  typedef itk::Object                          Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(PixelDescriptionView,itk::Object);

  /** Model typedef */
  typedef TModel                               ModelType;
  typedef typename ModelType::Pointer          ModelPointerType;
  
  /** Pixel description widget typedef */
  typedef Fl_Multiline_Output                  PixelDescriptionWidgetType;

  /**
   * This method unregister with previous model if any, and 
   * register with the new one.
   */
  void SetModel(ModelType * model);
  
  /**
   * Manually trigger a refresh
   */
  void Update();

  /** Get the pixel description widget */
  PixelDescriptionWidgetType * GetPixelDescriptionWidget()
  {
    return m_PixelDescriptionWidget;
  }

 protected:
  /** Constructor */
  PixelDescriptionView();
  /** Destructor */
  ~PixelDescriptionView();
 
  /** Handle notification from the viewer */
  virtual void PixelDescriptionModelNotify();
  
private:
  PixelDescriptionView(const Self&);      // purposely not implemented
  void operator=(const Self&);             // purposely not implemented

  /** The description widget */
  Fl_Multiline_Output * m_PixelDescriptionWidget;

  /** Model pointer */
  ModelPointerType       m_Model;

}; // end class
} // end namespace otb

#ifndef  OTB_MANUAL_INSTANTIATION
#include "otbPixelDescriptionView.txx"
#endif

#endif


