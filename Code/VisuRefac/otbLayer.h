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
#ifndef __otbLayer_h
#define __otbLayer_h

#include "itkObject.h"
#include "otbImage.h"
#include "itkRGBPixel.h"

namespace otb
{
/** \class Layer
*   \brief Base class for all layers objects
*   A layer is something that can be rendered to the screen.
*   
*   \sa ImageViewerModel
*/

template <class TOutputImage = Image<itk::RGBPixel<unsigned char>, 2 > > 
class Layer
  : public itk::Object
{
public:
  /** Standard class typedefs */
  typedef Layer                                Self;
  typedef itk::Object                          Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;
    
  /** Runtime information */
  itkTypeMacro(Layer,Object);

  /** Output image typedef */
  typedef TOutputImage                           OutputImageType;
  typedef typename OutputImageType::Pointer      OutputImagePointerType;

  /** Actually render the layer */
  virtual void Render() = 0;

  itkGetObjectMacro(RenderedQuicklook,     OutputImageType);
  itkGetObjectMacro(RenderedExtract,       OutputImageType);
  itkGetObjectMacro(RenderedScaledExtract, OutputImageType);

  itkSetMacro(HasQuicklook,bool);
  itkBooleanMacro(HasQuicklook);

  itkSetMacro(HasExtract,bool);
  itkBooleanMacro(HasExtract);

  itkSetMacro(HasScaledExtract,bool);
  itkBooleanMacro(HasScaledExtract);

  itkSetMacro(Visible,bool);
  itkBooleanMacro(Visible);

  itkSetStringMacro(Name);
  itkGetStringMacro(Name);

protected:
  /** Constructor */
  Layer() : m_HasQuicklook(false), m_HasExtract(false), m_HasScaledExtract(false), m_Visible(false), m_Name("Default")
  {}
  /** Destructor */
  ~Layer(){}
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
    os<<indent<<"Layer "<<m_Name<<":"<<std::endl;
    os<<indent<<indent<<"Has a quicklook: "     <<(m_HasQuicklook     ? "true" : "false") << std::endl;
    os<<indent<<indent<<"Has an extract: "      <<(m_HasExtract       ? "true" : "false") << std::endl;
    os<<indent<<indent<<"Has a scaled extract: "<<(m_HasScaledExtract ? "true" : "false") << std::endl;
  }
 
  /** Rendered quicklook */
  OutputImagePointerType m_RenderedQuicklook;
  bool                   m_HasQuicklook;
  /** Rendered extract */
  OutputImagePointerType m_RenderedExtract;
  bool                   m_HasExtract;
  /** Rendered scaled extract */
  OutputImagePointerType m_RenderedScaledExtract;
  bool                   m_HasScaledExtract;

private:
  Layer(const Self&);     // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  /** The layer name */
  std::string            m_Name;

  /** Is the layer visible ? */
  bool m_Visible;
  
}; // end class 
} // end namespace otb

#endif


