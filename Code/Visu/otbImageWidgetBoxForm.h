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

  void Draw(double openGlZoom, unsigned int originx, unsigned int originy, unsigned int windowh, unsigned int ss_rate)
    {
      if(this->GetVisible())
	{
	  double xul = static_cast<double>((static_cast<double>(m_Index[0])-static_cast<double>(originx))*openGlZoom*(1/static_cast<double>(ss_rate)));
          double xlr = static_cast<double>((static_cast<double>(m_Index[0]+m_Size[0])-static_cast<double>(originx))*openGlZoom*(1/static_cast<double>(ss_rate)));
          
          double yul = static_cast<double>(static_cast<double>(windowh)+(static_cast<double>(originy)-static_cast<double>(m_Index[1]))*openGlZoom*(1/static_cast<double>(ss_rate))); 
          
          double ylr = static_cast<double>(static_cast<double>(windowh)+(static_cast<double>(originy)-static_cast<double>(m_Size[1]+m_Index[1]))*openGlZoom*(1/static_cast<double>(ss_rate)));


	  glEnable(GL_BLEND);
	  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	  glColor4f(m_Color[0],m_Color[1],m_Color[2],m_Color[3]);
	  glBegin(GL_LINE_LOOP);
	  glVertex2d(xul,yul);
	  glVertex2d(xul,ylr);
	  glVertex2d(xlr,ylr);
	  glVertex2d(xlr,yul);
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

