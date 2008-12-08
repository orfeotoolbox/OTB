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
#ifndef __otbImageWidgetRectangleForm_h
#define __otbImageWidgetRectangleForm_h

#include "otbImageWidgetFormBase.h"
#include "otbMacro.h"

namespace otb
{
/** \class ImageWidgetRectangleForm
 * \brief
 *
 */
class ImageWidgetRectangleForm
  : public ImageWidgetFormBase
{
 public:
  /** Standard class typedefs */
  typedef ImageWidgetRectangleForm Self;
  typedef ImageWidgetFormBase Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(ImageWidgetRectangleForm,ImageWidgetFormBase);

  typedef Superclass::RegionType RegionType;
  typedef RegionType::SizeType SizeType;
  typedef RegionType::IndexType IndexType;

  itkSetMacro(Size,SizeType);
  itkGetMacro(Size,SizeType);

  itkSetMacro(Index,IndexType);
  itkGetMacro(Index,IndexType);

  void Draw(double openGlZoom, unsigned int originx, unsigned int originy, unsigned int windowh,unsigned int ss_rate)
    {
      if(this->GetVisible())
	{
	  //otbMsgDebugMacro(<<"Drawing a rectangle.");
	  glEnable(GL_BLEND);
	  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	  glColor4f(m_Color[0],m_Color[1],m_Color[2],m_Color[3]);
	  glBegin(GL_QUADS);

          int xul = static_cast<int>((static_cast<int>(m_Index[0])-static_cast<int>(originx))*openGlZoom*(1/static_cast<double>(ss_rate)));
          int xlr = static_cast<int>((static_cast<int>(m_Index[0]+m_Size[0])-static_cast<int>(originx))*openGlZoom*(1/static_cast<double>(ss_rate)));

          int yul = static_cast<int>(static_cast<int>(windowh)+(static_cast<int>(originy)-static_cast<int>(m_Index[1]))*openGlZoom*(1/static_cast<double>(ss_rate)));

          int ylr = static_cast<int>(static_cast<int>(windowh)+(static_cast<int>(originy)-static_cast<int>(m_Size[1]+m_Index[1]))*openGlZoom*(1/static_cast<double>(ss_rate)));

	  gl_rect(xul,yul,xlr,ylr);
	  glEnd();
	  glDisable(GL_BLEND);
	}
    }

  RegionType GetRegion(void)
    {
      RegionType resp;
      resp.SetSize(m_Size);
      resp.SetIndex(m_Index);
      return resp;
    }

 protected:
  /** Constructor. */
    ImageWidgetRectangleForm()
      {
	m_Index.Fill(0);
	m_Size.Fill(0);
      }

    /** Destructor. */
    ~ImageWidgetRectangleForm(){}

 private:
    ImageWidgetRectangleForm(const Self&);// purposely not implemented
    void operator=(const Self&);// purposely not implemented

    IndexType m_Index;
    SizeType m_Size;

};
} // end namespace otb
#endif

