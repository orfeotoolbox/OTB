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
#ifndef __otbRegionGlComponent_h
#define __otbRegionGlComponent_h

#include "otbGlComponent.h"
#include "itkImageRegion.h"

namespace otb
{
/** \class RegionGlComponent
*   \brief This Gl Component represents a region.
*/

class RegionGlComponent
  : public GlComponent
{
public:
  /** Standard class typedefs */
  typedef RegionGlComponent                    Self;
  typedef GlComponent                          Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;
  typedef itk::ImageRegion<2>                  RegionType;
 
  // affine transform
  typedef Superclass::AffineTransformType      AffineTransformType;
  typedef AffineTransformType::InputPointType  PointType;
  typedef AffineTransformType::InputVectorType VectorType;
  typedef Superclass::ColorType                ColorType;
    
  /** Runtime information */
  itkTypeMacro(RegionGlComponent,GlComponent);

  /** New macro */
  itkNewMacro(Self);

  /// Render the curve according to display extent and axis characteristics
  virtual void  Render(const RegionType& extent,const AffineTransformType * space2ScreenTransform)
  {
    PointType ip1, ip2, sp1,sp2;

    // Get the input region points
    ip1[0]=m_Region.GetIndex()[0]+1;
    ip1[1]=m_Region.GetIndex()[1]+1;
    ip2[0]=m_Region.GetIndex()[0] + m_Region.GetSize()[0]-1;
    ip2[1]=m_Region.GetIndex()[1] + m_Region.GetSize()[1]-1;

    // Convert to screen points
    sp1 = space2ScreenTransform->TransformPoint(ip1);
    sp2 = space2ScreenTransform->TransformPoint(ip2);

    // draw the box
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4d(m_Color[0],m_Color[1],m_Color[2],m_Color[3]);

    glBegin(GL_LINE_LOOP);
    glVertex2d(sp1[0],sp1[1]);
    glVertex2d(sp1[0],sp2[1]);
    glVertex2d(sp2[0],sp2[1]);
    glVertex2d(sp2[0],sp1[1]);
    glEnd();

    glDisable(GL_BLEND);
  }
  
  /** Set/Get the region to render */
  itkSetMacro(Region,RegionType);
  itkGetConstReferenceMacro(Region,RegionType);

  /** Set/Get the color of the region to render */
  itkSetMacro(Color,ColorType);
  itkGetConstReferenceMacro(Color,ColorType);

protected:
  /** Constructor */
  RegionGlComponent() : m_Region(), m_Color()
  {
    m_Color.Fill(0);
    m_Color[0] = 1.;
    m_Color[3] = 1.;
}
  /** Destructor */
  virtual ~RegionGlComponent(){}
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
  }

private:
  RegionGlComponent(const Self&);     // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  // The region to render
  RegionType m_Region;

  // The color of the region to render
  ColorType  m_Color;

}; // end class 
} // end namespace otb

#endif


