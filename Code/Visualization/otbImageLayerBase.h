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
#ifndef __otbImageLayerBase_h
#define __otbImageLayerBase_h

#include "itkObject.h"
#include "otbImage.h"
#include "itkVariableLengthVector.h"
#include "itkRGBPixel.h"
#include "itkRGBAPixel.h"
#include "otbBlendingFunction.h"
#include "otbUniformAlphaBlendingFunction.h"

namespace otb
{
/** \class ImageLayerBase
*   \brief Base class for all layers objects
*   A layer is something that can be rendered to the screen.
*
*   \sa ImageViewerModel
*  \ingroup Visualization
 */

template <class TOutputImage = Image<itk::RGBAPixel<unsigned char>, 2>, class TLayerValuePrecision = double>
class ImageLayerBase
  : public itk::Object
{
public:
  /** Standard class typedefs */
  typedef ImageLayerBase                Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Runtime information */
  itkTypeMacro(ImageLayerBase, Object);

  /** Output image typedef */
  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;
  typedef typename OutputImageType::RegionType RegionType;
  typedef typename RegionType::SizeType        SizeType;
  typedef typename RegionType::IndexType       IndexType;
  typedef typename OutputImageType::PixelType  OutputPixelType;

  /** Blending function typedef */
  typedef Function::BlendingFunction<OutputPixelType> BlendingFunctionType;
  typedef typename BlendingFunctionType::Pointer      BlendingFunctionPointerType;

  typedef TLayerValuePrecision                               LayerValuePrecisionType;
  typedef itk::VariableLengthVector<LayerValuePrecisionType> LayerValueType;

  /** Actually render the layer */
  virtual void Render() = 0;

  /** Get the pixel description */
  virtual std::string GetPixelDescription(const IndexType& index, bool getPlaceName = true) = 0;

  /** Get the pixel value in TLayerValuePrecision type */
  virtual LayerValueType GetValueAtIndex(const IndexType& index) = 0;

  itkGetObjectMacro(RenderedQuicklook,     OutputImageType);
  itkGetObjectMacro(RenderedExtract,       OutputImageType);
  itkGetObjectMacro(RenderedScaledExtract, OutputImageType);

  itkSetMacro(HasQuicklook, bool);
  itkGetMacro(HasQuicklook, bool);
  itkBooleanMacro(HasQuicklook);

  itkSetMacro(HasExtract, bool);
  itkGetMacro(HasExtract, bool);
  itkBooleanMacro(HasExtract);

  itkSetMacro(HasScaledExtract, bool);
  itkGetMacro(HasScaledExtract, bool);
  itkBooleanMacro(HasScaledExtract);

  itkSetMacro(Visible, bool);
  itkGetMacro(Visible, bool);
  itkBooleanMacro(Visible);

  itkSetStringMacro(Name);
  itkGetStringMacro(Name);

  itkSetMacro(QuicklookSize, SizeType);
  itkGetConstReferenceMacro(QuicklookSize, SizeType);

  itkSetMacro(Extent, RegionType);
  itkGetConstReferenceMacro(Extent, RegionType);

  /** This parameters should be reported to the appropriate filters in
  implementations. Therefore this method is marked as virtual. */
  virtual void SetExtractRegion(const RegionType& region)
  {
    m_ExtractRegion = region;
  }
  itkGetConstReferenceMacro(ExtractRegion, RegionType);

  /** This parameters should be reported to the appropriate filters in
  implementations. Therefore this method is marked as virtual. */
  virtual void SetScaledExtractRegion(const RegionType& region)
  {
    m_ScaledExtractRegion = region;
  }
  itkGetConstReferenceMacro(ScaledExtractRegion, RegionType);

  itkSetMacro(QuicklookSubsamplingRate, unsigned int);
  itkGetMacro(QuicklookSubsamplingRate, unsigned int);

  itkSetObjectMacro(BlendingFunction, BlendingFunctionType);
  itkGetObjectMacro(BlendingFunction, BlendingFunctionType);

  itkSetMacro(MaxValues, LayerValueType);
  itkGetMacro(MaxValues, LayerValueType);

  itkSetMacro(MinValues, LayerValueType);
  itkGetMacro(MinValues, LayerValueType);

protected:
  /** Constructor */
  ImageLayerBase() : m_Name("Default"), m_Visible(false), m_Extent(),
    m_RenderedQuicklook(), m_HasQuicklook(false), m_QuicklookSize(),  m_QuicklookSubsamplingRate(1),
    m_RenderedExtract(),   m_HasExtract(false),   m_ExtractRegion(),
    m_RenderedScaledExtract(), m_HasScaledExtract(false), m_ScaledExtractRegion(),
    m_BlendingFunction()
  {
    // Default blending function
    m_BlendingFunction = Function::UniformAlphaBlendingFunction<OutputPixelType>::New();
  }
  /** Destructor */
  virtual ~ImageLayerBase(){}
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
    os << indent << "ImageLayerBase " << m_Name << ":" << std::endl;
    os << indent << indent << "Has a quicklook: "     << (m_HasQuicklook     ? "true" : "false") << std::endl;
    os << indent << indent << "Has an extract: "      << (m_HasExtract       ? "true" : "false") << std::endl;
    os << indent << indent << "Has a scaled extract: " << (m_HasScaledExtract ? "true" : "false") << std::endl;
  }

  // These are protected to prevent from unwanted usage
  itkSetObjectMacro(RenderedQuicklook,     OutputImageType);
  itkSetObjectMacro(RenderedExtract,       OutputImageType);
  itkSetObjectMacro(RenderedScaledExtract, OutputImageType);

private:
  ImageLayerBase(const Self&);     // purposely not implemented
  void operator =(const Self&); // purposely not implemented

  /** The layer name */
  std::string m_Name;

  /** Is the layer visible ? */
  bool m_Visible;

  /** Data extent */
  RegionType m_Extent;

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

  LayerValueType m_MaxValues;
  LayerValueType m_MinValues;
}; // end class

// default impl, assuming scalar

/**
 * \class LayerValueGenerator
 * \brief TODO
 */

template <class TInputPixelType, class TLayerValueType>
class LayerValueGenerator
{
public:
  typedef TInputPixelType PixelType;
  typedef TLayerValueType LayerValueType;
  typedef typename LayerValueType::ValueType LayerValueInternalType;

  static LayerValueType Convert(const PixelType& pixel)
  {
    LayerValueType layerVal;
    layerVal.SetSize(1);
    layerVal[0] = static_cast<LayerValueInternalType>(pixel);
    return layerVal;
  }
};

// VariableLengthVector<T> partial specialization
template <class TInputInternalPixelType, class TLayerValueType>
class LayerValueGenerator<itk::VariableLengthVector<TInputInternalPixelType>, TLayerValueType>
{
public:
  typedef itk::VariableLengthVector<TInputInternalPixelType> PixelType;
  typedef TLayerValueType                                    LayerValueType;
  typedef typename LayerValueType::ValueType                 LayerValueInternalType;

  static LayerValueType Convert(const PixelType& pixel)
  {
    LayerValueType layerVal;
    layerVal.SetSize(pixel.GetSize());
    for (unsigned int i = 0; i < pixel.GetSize(); ++i)
      {
      layerVal[i] = static_cast<LayerValueInternalType>(pixel[i]);
      }
    return layerVal;
  }
};

// itk::RGBPixel<T> partial specialization
template <class TInputInternalPixelType, class TLayerValueType>
class LayerValueGenerator<itk::RGBPixel<TInputInternalPixelType>, TLayerValueType>
{
public:
  typedef itk::RGBPixel<TInputInternalPixelType>          PixelType;
  typedef TLayerValueType                                 LayerValueType;
  typedef typename LayerValueType::ValueType              LayerValueInternalType;

  static LayerValueType Convert(const PixelType& pixel)
  {
    LayerValueType layerVal;
    layerVal.SetSize(3);
    layerVal[0] = pixel[0];
    layerVal[1] = pixel[1];
    layerVal[2] = pixel[2];
    return layerVal;
  }
};

// itk::RGBAPixel<T> partial specialization
template <class TInputInternalPixelType, class TLayerValueType>
class LayerValueGenerator<itk::RGBAPixel<TInputInternalPixelType>, TLayerValueType>
{
public:
  typedef itk::RGBAPixel<TInputInternalPixelType>         PixelType;
  typedef TLayerValueType                                 LayerValueType;
  typedef typename LayerValueType::ValueType              LayerValueInternalType;

  static LayerValueType Convert(const PixelType& pixel)
  {
    LayerValueType layerVal;
    layerVal.SetSize(4);
    layerVal[0] = pixel[0];
    layerVal[1] = pixel[1];
    layerVal[2] = pixel[2];
    layerVal[3] = pixel[3];
    return layerVal;
  }
};


} // end namespace otb

#endif
