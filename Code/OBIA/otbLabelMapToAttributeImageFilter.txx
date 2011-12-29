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

#ifndef __otbLabelMapToAttributeImageFilter_txx
#define __otbLabelMapToAttributeImageFilter_txx

#include "otbLabelMapToAttributeImageFilter.h"
#include "otbMacro.h"

namespace otb
{

/** Constructor */
template <class TInputImage, class TOutputImage , class TAttributeAccessor>
LabelMapToAttributeImageFilter<TInputImage, TOutputImage, TAttributeAccessor>
::LabelMapToAttributeImageFilter()
{
m_ChosenAttributes.resize(0);
m_BackgroundValue = static_cast<OutputInternalPixelType>(0);
}

template <class TInputImage, class TOutputImage , class TAttributeAccessor>
void LabelMapToAttributeImageFilter<TInputImage, TOutputImage, TAttributeAccessor>
::SetChosenAttributes(AttributeListType & newAttributes)
{
  m_ChosenAttributes = newAttributes;
}

template <class TInputImage, class TOutputImage , class TAttributeAccessor>
unsigned int LabelMapToAttributeImageFilter<TInputImage, TOutputImage, TAttributeAccessor>
::GetNumberOfComponentsPerPixel()
{
  return m_ChosenAttributes.size();
}

template <class TInputImage, class TOutputImage , class TAttributeAccessor>
bool LabelMapToAttributeImageFilter<TInputImage, TOutputImage, TAttributeAccessor>
::SetAttributeForNthChannel(unsigned int channel, const char *attribute)
{
  std::string strAttribute(attribute);
  if (strAttribute.size() == 0)
    {
    otbWarningMacro("Attribute name is empty");
    return false;
    }
  
  if( channel<GetNumberOfComponentsPerPixel() )
    {
    m_ChosenAttributes[channel] = strAttribute;
    }
  else if( channel==GetNumberOfComponentsPerPixel() )
    {
    m_ChosenAttributes.push_back(strAttribute);
    }
  else
    {
    return false;
    }
  return true;
}

template <class TInputImage, class TOutputImage , class TAttributeAccessor>
void LabelMapToAttributeImageFilter<TInputImage, TOutputImage, TAttributeAccessor>
::GenerateOutputInformation()
{
  if (this->GetOutput() && this->GetInput())
    {
    this->GetOutput()->CopyInformation(this->GetInput());
    this->GetOutput()->SetNumberOfComponentsPerPixel(GetNumberOfComponentsPerPixel());
    }
}

template <class TInputImage, class TOutputImage , class TAttributeAccessor>
void LabelMapToAttributeImageFilter<TInputImage, TOutputImage, TAttributeAccessor>
::BeforeThreadedGenerateData()
{
  //first call superclass method to initialize label object container and progress
  Superclass::BeforeThreadedGenerateData();
  
  unsigned int nbChannels = GetNumberOfComponentsPerPixel();
  if (nbChannels == 0)
    {
    itkExceptionMacro("No attribute given");
    }
  
  OutputImageType *output = this->GetOutput();
  OutputPixelType backgroundPixel;
  backgroundPixel.SetSize(nbChannels);
  for (unsigned int k=0; k<nbChannels; k++)
    {
    backgroundPixel[k] = m_BackgroundValue;
    }
  output->FillBuffer( backgroundPixel );
}

template <class TInputImage, class TOutputImage , class TAttributeAccessor>
void LabelMapToAttributeImageFilter<TInputImage, TOutputImage, TAttributeAccessor>
::ThreadedProcessLabelObject( LabelObjectType * labelObject )
{
  unsigned int nbChannels = GetNumberOfComponentsPerPixel();
  OutputImageType *output = this->GetOutput();
  AttributeAccessorType accessor;
  OutputPixelType outPixel;
  outPixel.SetSize(nbChannels);
  for (unsigned int k=0; k<nbChannels; k++)
    {
    accessor.SetAttributeName(m_ChosenAttributes[k].c_str());
    const AttributeValueType & attribute = accessor( labelObject );
    outPixel[k] = static_cast<OutputInternalPixelType>(attribute);
    }

  LineContainerConstIterator lit;
  const LineContainerType & lineContainer = labelObject->GetLineContainer();

  for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
    {
    IndexType idx = lit->GetIndex();
    unsigned long length = lit->GetLength();
    for( unsigned int i=0; i<length; i++)
      {
      output->SetPixel( idx, outPixel );
      idx[0]++;
      }
    }
}

}

#endif
