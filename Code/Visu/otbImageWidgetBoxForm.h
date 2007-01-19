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
#ifndef _otbImageWidgetBoxForm_h
#define _otbImageWidgetBoxForm_h

#include "itkSize.h"
#include "itkIndex.h"
#include "otbImageWidgetFormBase.h"

namespace otb
{
/** \class ImageWidgetBoxForm
 * \brief 
 *
 */
class ImageWidgetBoxForm
  : public ImageWidgetFormBase
{
 public:
  /** Standard class typedefs */
  typedef ImageWidgetBoxForm Self;
  typedef ImageWidgetFormBase Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  
  /** Method for creation through the object factory */
  itkNewMacro(Self);
  
  /** Runtime information */
  itkTypeMacro(ImageWidgetBoxForm, ImageWidgetFormBase);

  typedef itk::Size<2> SizeType;
  typedef itk::Index<2> IndexType;

  itkSetMacro(Size,SizeType);
  itkGetMacro(Size,SizeType);

  itkSetMacro(Index,IndexType);
  itkGetMacro(Index,IndexType);

  void Draw(double openGlZoom, unsigned int originx, unsigned int originy, unsigned int windowh)
    {
      if(this->GetVisible())
	{
	  unsigned int minX, minY, maxX, maxY;
	  minX = static_cast<unsigned int>((m_Index[0]-originx)*openGlZoom);
	  minY = static_cast<unsigned int>(windowh-(m_Index[1]-originy+m_Size[1])*openGlZoom);
	  maxX = static_cast<unsigned int>((m_Index[0]-originx+m_Size[0])*openGlZoom);
	  maxY = static_cast<unsigned int>(windowh-(m_Index[1]-originy)*openGlZoom);


	  glEnable(GL_BLEND);
	  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	  glColor4f(m_Color[0],m_Color[1],m_Color[2],m_Color[3]);
	  glBegin(GL_LINE_LOOP);
	  glVertex2f(minX,minY);
	  glVertex2f(minX,maxY);
	  glVertex2f(maxX,maxY);
	  glVertex2f(maxX,minY);
	  glEnd();
	  glDisable(GL_BLEND);
	}
    };
 protected: 
  /** Constructor. */
  ImageWidgetBoxForm()
      {
	m_Index.Fill(0);
	m_Size.Fill(0);
      };
    /** Destructor. */
    ~ImageWidgetBoxForm(){};
    
 private:
    ImageWidgetBoxForm(const Self&);// purposely not implemented
    void operator=(const Self&);// purposely not implemented
    
    IndexType m_Index;
    SizeType m_Size;

};
} // end namespace otb
#endif

