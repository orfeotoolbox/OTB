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
#ifndef _otbImageWidgetPolygonForm_h
#define _otbImageWidgetPolygonForm_h

#include "itkSize.h"
#include "itkIndex.h"
#include "otbImageWidgetFormBase.h"
#include "otbPolygon.h"

namespace otb
{
/** \class ImageWidgetPolygonForm
 * \brief 
 *
 */
template<class TValue = double>
class ITK_EXPORT ImageWidgetPolygonForm
  : public ImageWidgetFormBase
{
  public:
  /** Standard class typedefs */
  typedef ImageWidgetPolygonForm        Self;
  typedef ImageWidgetFormBase           Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef TValue                        ValueType;
  
  /** Method for creation through the object factory */
  itkNewMacro(Self);
  
  /** Runtime information */
  itkTypeMacro(ImageWidgetPolygonForm, ImageWidgetFormBase);
  
  /** Usefull typedef. */
  typedef itk::Size<2>                               SizeType;
  typedef itk::Index<2>                              IndexType;
  typedef Polygon<ValueType>                         PolygonType;
  typedef typename PolygonType::Pointer              PolygonPointerType;       
  typedef typename PolygonType::VertexType           VertexType;
  typedef typename PolygonType::VertexListType       VertexListType;
  typedef typename VertexListType::ConstIterator     VertexListIteratorType;
  typedef typename PolygonType::ContinuousIndexType  ContinuousIndexType;
  
 
  itkSetMacro(Polygon, PolygonPointerType);
  itkGetMacro(Polygon, PolygonPointerType);
  
  void Draw(double openGlZoom, unsigned int originx, unsigned int originy, unsigned int windowh);
  
  
  protected: 
  /** Constructor. */
  ImageWidgetPolygonForm();
  /** Destructor. */
  ~ImageWidgetPolygonForm();
  
  private:
  ImageWidgetPolygonForm(const Self&);// purposely not implemented
    void operator=(const Self&);// purposely not implemented
  
  PolygonPointerType m_Polygon;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageWidgetPolygonForm.txx"
#endif
#endif
