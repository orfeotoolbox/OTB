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
#ifndef __otbImageWidgetCircleForm_h
#define __otbImageWidgetCircleForm_h

#include "itkSize.h"
#include "itkIndex.h"
#include "otbImageWidgetFormBase.h"

namespace otb
{
/** \class ImageWidgetCircleForm
 * \brief 
 *
 */
class ITK_EXPORT ImageWidgetCircleForm
  : public ImageWidgetFormBase
{
  public:
  /** Standard class typedefs */
  typedef ImageWidgetCircleForm        Self;
  typedef ImageWidgetFormBase           Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  
  /** Method for creation through the object factory */
  itkNewMacro(Self);
  
  /** Runtime information */
  itkTypeMacro(ImageWidgetCircleForm, ImageWidgetFormBase);
  
  /** Usefull typedef. */
  typedef itk::ContinuousIndex<double,2> ContinuousIndexType;
  
  /** Accessors */
  itkSetMacro(Solid,bool);
  itkGetMacro(Solid,bool);
  itkSetMacro(Radius,double);
  itkGetMacro(Radius,double);
  itkSetMacro(Center,ContinuousIndexType);
  itkGetMacro(Center,ContinuousIndexType);

  
  /** Actually draw the polygon */
  void Draw(double openGlZoom, unsigned int originx, unsigned int originy, unsigned int windowh, unsigned int ss_rate);
  
  
  protected: 
  /** Constructor. */
  ImageWidgetCircleForm();
  /** Destructor. */
  ~ImageWidgetCircleForm();
  
  private:
  ImageWidgetCircleForm(const Self&);// purposely not implemented
    void operator=(const Self&);// purposely not implemented
  
  /// true if needed to draw a solid polygon
  bool m_Solid;
  /// Radius of the circle
  double m_Radius;
  /// Center of the circle
  ContinuousIndexType m_Center;

};
} // end namespace otb
#endif
