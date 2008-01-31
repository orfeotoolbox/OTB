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
#ifndef _otbImageWidgetPolylineForm_h
#define _otbImageWidgetPolylineForm_h

#include "itkSize.h"
#include "itkIndex.h"
#include "otbImageWidgetFormBase.h"
#include "otbPolylineParametricPathWithValue.h"

namespace otb
{
/** \class ImageWidgetPolylineForm
 * \brief 
 *
 */
template<class TValue = double>
class ITK_EXPORT ImageWidgetPolylineForm
  : public ImageWidgetFormBase
{
  public:
  /** Standard class typedefs */
  typedef ImageWidgetPolylineForm        Self;
  typedef ImageWidgetFormBase           Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef TValue                        ValueType;
  
  /** Method for creation through the object factory */
  itkNewMacro(Self);
  
  /** Runtime information */
  itkTypeMacro(ImageWidgetPolylineForm, ImageWidgetFormBase);
  
  /** Usefull typedef. */
  typedef itk::Size<2>                               SizeType;
  typedef itk::Index<2>                              IndexType;
  typedef PolylineParametricPathWithValue<ValueType,2> PolylineType;
  typedef typename PolylineType::Pointer              PolylinePointerType;       
  typedef typename PolylineType::VertexType           VertexType;
  typedef typename PolylineType::VertexListType       VertexListType;
  typedef typename VertexListType::ConstIterator     VertexListIteratorType;
  typedef typename PolylineType::ContinuousIndexType  ContinuousIndexType;
  
  /** Accessors */
  itkSetMacro(Polyline, PolylinePointerType);
  itkGetMacro(Polyline, PolylinePointerType);
  itkSetMacro(InternalValueToAlphaChannel,bool);
  itkGetMacro(InternalValueToAlphaChannel,bool);
  /** Actually draw the polyline */
  void Draw(double openGlZoom, unsigned int originx, unsigned int originy, unsigned int windowh, unsigned int ss_rate);
  
  
  protected: 
  /** Constructor. */
  ImageWidgetPolylineForm();
  /** Destructor. */
  ~ImageWidgetPolylineForm();
  
  private:
  ImageWidgetPolylineForm(const Self&);// purposely not implemented
    void operator=(const Self&);// purposely not implemented
  
  /** Internal pointer to the otb::Polyline data structure */
  PolylinePointerType m_Polyline;
  /** True if polygon internal value affects the alpha color value
   * (can be used to denote membership values )
   */
  bool m_InternalValueToAlphaChannel;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageWidgetPolylineForm.txx"
#endif
#endif