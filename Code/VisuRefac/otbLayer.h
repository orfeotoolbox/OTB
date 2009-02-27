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
#include "otbBlendingFunction.h"
#include "otbUniformAlphaBlendingFunction.h"

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
  typedef typename OutputImageType::RegionType   RegionType;
  typedef typename RegionType::SizeType          SizeType;
  typedef typename OutputImageType::PixelType    PixelType;

  /** Blending function typedef */
  typedef Function::BlendingFunction<PixelType>  BlendingFunctionType;
  typedef typename BlendingFunctionType::Pointer BlendingFunctionPointerType;

  /** Actually render the layer */
  virtual void Render() = 0;

  itkGetObjectMacro(RenderedQuicklook,     OutputImageType);
  itkGetObjectMacro(RenderedExtract,       OutputImageType);
  itkGetObjectMacro(RenderedScaledExtract, OutputImageType);

  itkSetMacro(HasQuicklook,bool);
  itkGetMacro(HasQuicklook,bool);
  itkBooleanMacro(HasQuicklook);

  itkSetMacro(HasExtract,bool);
  itkGetMacro(HasExtract,bool);
  itkBooleanMacro(HasExtract);

  itkSetMacro(HasScaledExtract,bool);
  itkGetMacro(HasScaledExtract,bool);
  itkBooleanMacro(HasScaledExtract);

  itkSetMacro(Visible,bool);
  itkGetMacro(Visible,bool);
  itkBooleanMacro(Visible);

  itkSetStringMacro(Name);
  itkGetStringMacro(Name);

  itkSetMacro(QuicklookSize,SizeType);
  itkGetConstReferenceMacro(QuicklookSize,SizeType);

  itkSetMacro(Extent,RegionType);
  itkGetConstReferenceMacro(Extent,RegionType);

  /** This parameters should be reported to the appropriate filters in
  implementations. Therefore this method is marked as virtual. */
  virtual void SetExtractRegion(const RegionType & region)
  {
    m_ExtractRegion = region;
  }
  itkGetConstReferenceMacro(ExtractRegion,RegionType);

  /** This parameters should be reported to the appropriate filters in
  implementations. Therefore this method is marked as virtual. */
  virtual void SetScaledExtractRegion(const RegionType & region)
  {
    m_ScaledExtractRegion = region;
  }
  itkGetConstReferenceMacro(ScaledExtractRegion,RegionType);

  itkSetMacro(QuicklookSubsamplingRate,unsigned int);
  itkGetMacro(QuicklookSubsamplingRate,unsigned int);

  itkSetObjectMacro(BlendingFunction,BlendingFunctionType);
  itkGetObjectMacro(BlendingFunction,BlendingFunctionType);

protected:
  /** Constructor */
  Layer() : m_Name("Default"), m_Visible(false), m_Extent(), 
	    m_RenderedQuicklook(), m_HasQuicklook(false), m_QuicklookSize(),  m_QuicklookSubsamplingRate(1),
	    m_RenderedExtract(),   m_HasExtract(false),   m_ExtractRegion(),
	    m_RenderedScaledExtract(), m_HasScaledExtract(false), m_ScaledExtractRegion(),
	    m_BlendingFunction()
  {
    // Default blending function
    m_BlendingFunction = Function::UniformAlphaBlendingFunction<PixelType>::New();
  }
  /** Destructor */
  virtual ~Layer(){}
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
    os<<indent<<"Layer "<<m_Name<<":"<<std::endl;
    os<<indent<<indent<<"Has a quicklook: "     <<(m_HasQuicklook     ? "true" : "false") << std::endl;
    os<<indent<<indent<<"Has an extract: "      <<(m_HasExtract       ? "true" : "false") << std::endl;
    os<<indent<<indent<<"Has a scaled extract: "<<(m_HasScaledExtract ? "true" : "false") << std::endl;
  }

  // These are protected to prevent from unwanted usage
  itkSetObjectMacro(RenderedQuicklook,     OutputImageType);
  itkSetObjectMacro(RenderedExtract,       OutputImageType);
  itkSetObjectMacro(RenderedScaledExtract, OutputImageType);

private:
  Layer(const Self&);     // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  /** The layer name */
  std::string            m_Name;

  /** Is the layer visible ? */
  bool                   m_Visible;
  
  /** Data extent */
  RegionType             m_Extent;

  /** Rendered quicklook */
  OutputImagePointerType m_RenderedQuicklook;
  bool                   m_HasQuicklook;
  SizeType               m_QuicklookSize;
  unsigned int           m_QuicklookSubsamplingRate;

  /** Rendered extract */
  OutputImagePointerType m_RenderedExtract;
  bool                   m_HasExtract;
  RegionType             m_ExtractRegion;

  /** Rendered scaled extract */
  OutputImagePointerType m_RenderedScaledExtract;
  bool                   m_HasScaledExtract;
  RegionType             m_ScaledExtractRegion;

  /** Pointer to the blending function */
  BlendingFunctionPointerType m_BlendingFunction;

}; // end class 
} // end namespace otb

#endif


