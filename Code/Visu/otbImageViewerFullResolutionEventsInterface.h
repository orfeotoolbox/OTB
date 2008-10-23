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
#ifndef __otbImageViewerFullResolutionEventsInterface_h
#define __otbImageViewerFullResolutionEventsInterface_h

#include "itkObject.h"
#include "itkImageRegion.h"
#include "otbMacro.h"

namespace otb
{
class ImageViewerFullResolutionEventsInterface
: public itk::Object
{
 public:
  /** Standard class typedefs */
  typedef ImageViewerFullResolutionEventsInterface Self;
  typedef itk::Object                      Superclass;
  typedef itk::SmartPointer<Self>          Pointer;
  typedef itk::SmartPointer<const Self>    ConstPointer;
 
  /** Standard type macros */
  itkTypeMacro(ImageViewerFullResolutionEventsInterface,Superclass);

  typedef itk::ImageRegion<2> RegionType;
  typedef RegionType::IndexType IndexType;

  itkSetMacro(ForwardEvents,bool);
  itkGetMacro(ForwardEvents,bool);

  /** Users actions */
  virtual void RegionSelected(const RegionType & region){};
  virtual void ViewedRegionChanged(){};
  virtual void PixelClicked(const IndexType& index){};

 protected:
  /** Constructor */
  ImageViewerFullResolutionEventsInterface()
    {
      m_ForwardEvents = false;
    }
  /** Destructor */
  ~ImageViewerFullResolutionEventsInterface(){}

 private:
  ImageViewerFullResolutionEventsInterface(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool m_ForwardEvents;
};
} // end namespace otb

#endif
