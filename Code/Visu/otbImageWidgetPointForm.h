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
#ifndef __otbImageWidgetPointForm_h
#define __otbImageWidgetPointForm_h

#include "itkIndex.h"
#include "otbImageWidgetFormBase.h"

namespace otb
{
/** \class ImageWidgetPointForm
 * \brief
 *
 */
class ImageWidgetPointForm
  : public ImageWidgetFormBase
{
 public:
  /** Standard class typedefs */
  typedef ImageWidgetPointForm Self;
  typedef ImageWidgetFormBase Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(ImageWidgetPointForm,ImageWidgetFormBase);

  typedef Superclass::RegionType RegionType;
  typedef RegionType::SizeType SizeType;
  typedef RegionType::IndexType IndexType;


  itkSetMacro(Index,IndexType);
  itkGetMacro(Index,IndexType);

  void Draw(double openGlZoom, unsigned int originx, unsigned int originy, unsigned int windowh, unsigned int ss_rate)
    {
      if(this->GetVisible())
	{
	  unsigned int x,y;
	  x = static_cast<unsigned int>((m_Index[0]-originx)*openGlZoom*ss_rate);
	  y = static_cast<unsigned int>(windowh-(m_Index[1]-originy)*openGlZoom*ss_rate);

	  glEnable(GL_BLEND);
	  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	  glColor4f(m_Color[0],m_Color[1],m_Color[2],m_Color[3]);
	  glBegin(GL_POINTS);
	  glVertex2f(x,y);
	  glEnd();
	}
    }

  RegionType GetRegion(void)
    {
      RegionType resp;
      SizeType size;
      size.Fill(1);
      resp.SetSize(size);
      resp.SetIndex(m_Index);
      return resp;
    }

 protected:
  /** Constructor. */
    ImageWidgetPointForm()
      {
	m_Index.Fill(0);
      }

    /** Destructor. */
    ~ImageWidgetPointForm(){};

 private:
    ImageWidgetPointForm(const Self&);// purposely not implemented
    void operator=(const Self&);// purposely not implemented

    IndexType m_Index;

};
} // end namespace otb
#endif

