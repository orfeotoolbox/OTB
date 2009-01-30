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
#ifndef __otbImageWidgetFormBase_h
#define __otbImageWidgetFormBase_h

#include "itkDataObject.h"
#include "itkObjectFactory.h"
#include "itkFixedArray.h"
#include "itkImageRegion.h"
#include "itkMacro.h"
#include <FL/gl.h>

namespace otb
{
/** \class ImageWidgetFormBase
 * \brief
 *
 */
class ITK_EXPORT ImageWidgetFormBase
      : public itk::DataObject
{
public:
  /** Standard class typedefs */
  typedef ImageWidgetFormBase           Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Runtime information */
  itkTypeMacro(ImageWidgetFormBase,DataObject);

  /** Method for creation through the object factory */
  itkNewMacro(Self);



  typedef itk::FixedArray<float,4> ColorType;
  typedef itk::ImageRegion<2> RegionType;

  itkSetMacro(Visible,bool);
  itkGetMacro(Visible,bool);
  itkSetMacro(Color,ColorType);
  itkGetMacro(Color,ColorType);

  /**
   * Set the form color.
   * \param r The red component.
   * \param g The green component.
   * \param b The blue component.
   * \param a The alpha component.
   */
  virtual void SetColor(float r, float g, float b, float a)
  {
    m_Color[0]=r;
    m_Color[1]=g;
    m_Color[2]=b;
    m_Color[3]=a;
  }
  /**
   * Draw the form in opengl context.
   * \param openGlZoom the openGl zoom factor,
   * \param originx The x axis origin,
   * \param originy The y axis origin,
   * \param windowh The window height,
   *  \param ss_rate The subsampling rate.
   */
  virtual void Draw(double openGlZoom, unsigned int originx, unsigned int originy, unsigned int windowh, unsigned int ss_rate) {};

  virtual RegionType GetRegion(void)
  {
    itkExceptionMacro("Method must be implemented in subclasses!");
  };

protected:
  /** Constructor. */
  ImageWidgetFormBase()
  {
    m_Visible=true;
    m_Color[0]=1;
    m_Color[1]=0;
    m_Color[2]=0;
    m_Color[3]=1;
  };

  /** Destructor. */
  ~ImageWidgetFormBase()
  {};

  ColorType m_Color;

private:
  ImageWidgetFormBase(const Self&);// purposely not implemented
  void operator=(const Self&);// purposely not implemented


  bool m_Visible;

};
} // end namespace otb
#endif

