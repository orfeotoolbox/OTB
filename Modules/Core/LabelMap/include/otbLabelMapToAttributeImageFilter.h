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

#ifndef __otbLabelMapToAttributeImageFilter_h
#define __otbLabelMapToAttributeImageFilter_h

#include "itkLabelMapFilter.h"
#include "otbAttributesMapLabelObject.h"
#include "itkVariableLengthVector.h"

namespace otb
{
/** \class LabelMapToAttributeImageFilter
 *
 * \brief This class produces an image from attributes in a label map
 *
 * This filter is intended to work on AttributesMapLabelObject. The user chooses
 * the attributes to display for each channel. The output image is expected to be
 * a vector image.
 */

template< class TInputImage, class TOutputImage , class TAttributeAccessor=
    typename Functor::AttributesMapLabelObjectAccessor< typename TInputImage::LabelObjectType > >
class ITK_EXPORT LabelMapToAttributeImageFilter :
    public itk::LabelMapFilter< TInputImage , TOutputImage >
{
public:
  /** Standard class typedefs. */
  typedef LabelMapToAttributeImageFilter   Self;
  typedef itk::LabelMapFilter
    < TInputImage , TOutputImage >      Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard New method. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(LabelMapToAttributeImageFilter, LabelMapFilter);

  /** Some convenient typedefs. */
  typedef TInputImage                                       InputImageType;
  typedef typename InputImageType::LabelObjectType          LabelObjectType;
  typedef typename LabelObjectType::ConstLineIterator       ConstLineIteratorType;
  typedef typename LabelObjectType::AttributesMapType       AttributesMapType;

  typedef TOutputImage                                OutputImageType;
  typedef typename OutputImageType::PixelType         OutputPixelType;
  typedef typename OutputImageType::InternalPixelType OutputInternalPixelType;
  typedef typename OutputImageType::IndexType         IndexType;
  typedef itk::VariableLengthVector<OutputInternalPixelType> VectorPixelType;

  typedef std::vector< std::string >  AttributeListType;
  typedef TAttributeAccessor          AttributeAccessorType;
  typedef typename AttributeAccessorType::AttributeValueType AttributeValueType;

  /** Get macro for background value */
  itkGetConstMacro(BackgroundValue, OutputInternalPixelType);

  /** Set macro for background value */
  itkSetMacro(BackgroundValue, OutputInternalPixelType);

  /** Get the current chosen attributes */
  const AttributeListType & GetChosenAttributes()
  {
    return m_ChosenAttributes;
  }

  /** Set the chosen attributes */
  void SetChosenAttributes(AttributeListType & newAttributes);

  /** Get the number of components of the output image */
  unsigned int GetNumberOfComponentsPerPixel();

  /** Fill the given channel with value from the given attribute */
  bool SetAttributeForNthChannel(unsigned int channel, const char *attribute);

  /** Fill the given channel with value from the given attribute */
  bool SetAttributeForNthChannel(unsigned int channel, const std::string& attribute)
  {
    return this->SetAttributeForNthChannel(channel, attribute.c_str());
  }

protected:
  LabelMapToAttributeImageFilter();
  ~LabelMapToAttributeImageFilter() {};

  virtual void GenerateOutputInformation();

  virtual void BeforeThreadedGenerateData();

  virtual void ThreadedProcessLabelObject( LabelObjectType * labelObject );

private:
  /** Background pixel value */
  OutputInternalPixelType m_BackgroundValue;

  /** List of chosen attributes */
  AttributeListType m_ChosenAttributes;
};

}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelMapToAttributeImageFilter.txx"
#endif

#endif
