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
#ifndef __otbRadiometricAttributesLabelMapFilter_txx
#define __otbRadiometricAttributesLabelMapFilter_txx

#include "otbRadiometricAttributesLabelMapFilter.h"
#include "itkProgressReporter.h"

namespace otb
{
namespace Functor
{
/** Constructor */
template <class TLabelObject, class TFeatureImage>
MultiStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::MultiStatsAttributesLabelObjectFunctor() : m_ReducedAttributeSet(true),
  m_StatsFunctorsMap()
{}

/** Destructor */
template <class TLabelObject, class TFeatureImage>
MultiStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::~MultiStatsAttributesLabelObjectFunctor(){}

/** The comparators */
template <class TLabelObject, class TFeatureImage>
bool
MultiStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::operator != (const Self &self)
  {
  bool resp = true;
  resp = resp && (m_ReducedAttributeSet != self.m_ReducedAttributeSet);
  resp = resp && (m_StatsFunctorsMap != self.m_StatsFunctorsMap);
  return resp;
  }

template <class TLabelObject, class TFeatureImage>
bool
MultiStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::operator == (const Self &self)
  {
  return !(this != self);
  }

/** This is the functor implementation
 *  Calling the functor on a label object
 *  will update its statistics attributes */
template <class TLabelObject, class TFeatureImage>
void
MultiStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::operator() (LabelObjectType * lo) const
{
  // Walk every registered functors
  for (typename StatsFunctorsMapType::const_iterator it = m_StatsFunctorsMap.begin();
       it != m_StatsFunctorsMap.end(); ++it)
    {
    (it->second)(lo);
    }
}

/** Add a feature with the given name */
template <class TLabelObject, class TFeatureImage>
void
MultiStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::AddFeature(const std::string& name, const TFeatureImage * img)
{
  // Create a new functor
  StatsFunctorType newFunctor;

  // Set the reduced attribute set option
  newFunctor.SetReducedAttributeSet(m_ReducedAttributeSet);

  // Set the feature and its name
  newFunctor.SetFeatureName(name);

  // Set the feature image
  newFunctor.SetFeatureImage(img);

  // Add it to the map
  m_StatsFunctorsMap[name] = newFunctor;
}

/** Remove the feature with this name if it exists */
template <class TLabelObject, class TFeatureImage>
bool
MultiStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::RemoveFeature(const std::string& name)
{
  return (m_StatsFunctorsMap.erase(name) == 1);
}

/** Get the feature image with this name */
template <class TLabelObject, class TFeatureImage>
const TFeatureImage *
MultiStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::GetFeature(const std::string& name) const
{
  if (m_StatsFunctorsMap.count(name) == 0)
    {
    itkGenericExceptionMacro(<< "No feature named " << name << " in map.");
    }
  return m_StatsFunctorsMap[name].GetFeatureImage();
}

/** Clear all the features */
template <class TLabelObject, class TFeatureImage>
void
MultiStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::ClearAllFeatures()
{
  m_StatsFunctorsMap.clear();
}

/** Get the number of features */
template <class TLabelObject, class TFeatureImage>
unsigned int
MultiStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::GetNumberOfFeatures() const
{
  return m_StatsFunctorsMap.size();
}

/** Set the reduced attribute set */
template <class TLabelObject, class TFeatureImage>
void
MultiStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::SetReducedAttributeSet(bool flag)
{
  // Set the flag
  m_ReducedAttributeSet = flag;

  // Set the flag to all the already existing functors
  for (typename StatsFunctorsMapType::iterator it = m_StatsFunctorsMap.begin();
       it != m_StatsFunctorsMap.end(); ++it)
    {
    it->second.SetReducedAttributeSet(m_ReducedAttributeSet);
    }
}
/** Get the reduced attribute set */
template <class TLabelObject, class TFeatureImage>
bool
MultiStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::GetReducedAttributeSet() const
{
  return m_ReducedAttributeSet;
}
} // End namespace Functor

template <class TImage, class TFeatureImage>
RadiometricAttributesLabelMapFilter<TImage, TFeatureImage>
::RadiometricAttributesLabelMapFilter() : m_RedChannelIndex(2),
  m_GreenChannelIndex(1),
  m_BlueChannelIndex(0),
  m_NIRChannelIndex(3)
{
  this->SetNumberOfRequiredInputs(2);
}

/** Set the feature image */
template <class TImage, class TFeatureImage>
void
RadiometricAttributesLabelMapFilter<TImage, TFeatureImage>
::SetFeatureImage(const TFeatureImage *input)
{
  // Set the Nth input
  this->SetNthInput(1, const_cast<TFeatureImage*>(input));
}

/** Get the feature image */
template <class TImage, class TFeatureImage>
const typename RadiometricAttributesLabelMapFilter<TImage, TFeatureImage>
::FeatureImageType *
RadiometricAttributesLabelMapFilter<TImage, TFeatureImage>
::GetFeatureImage() const
{
  return static_cast<const TFeatureImage *>(this->itk::ProcessObject::GetInput(1));
}

/** Set Input1 (for backward compatibility) */
template <class TImage, class TFeatureImage>
void
RadiometricAttributesLabelMapFilter<TImage, TFeatureImage>
::SetInput1(const TImage * input)
{
  this->SetInput(input);
}

/** Get Input1 (for backward compatibility) */
template <class TImage, class TFeatureImage>
const TImage *
RadiometricAttributesLabelMapFilter<TImage, TFeatureImage>
::GetInput1() const
{
  return this->GetInput();
}

/** Set Input2 (for backward compatibility) */
template <class TImage, class TFeatureImage>
void
RadiometricAttributesLabelMapFilter<TImage, TFeatureImage>
::SetInput2(const TFeatureImage * input)
{
  this->SetFeatureImage(input);
}

/** Get Input2 (for backward compatibility) */
template <class TImage, class TFeatureImage>
const TFeatureImage *
RadiometricAttributesLabelMapFilter<TImage, TFeatureImage>
::GetInput2() const
{
  return this->GetFeatureImage();
}

/** Set the reduced attribute set */
template <class TImage, class TFeatureImage>
void
RadiometricAttributesLabelMapFilter<TImage, TFeatureImage>
::SetReducedAttributeSet(bool flag)
{
  if (this->GetFunctor().GetReducedAttributeSet() != flag)
    {
    this->GetFunctor().SetReducedAttributeSet(flag);
    this->Modified();
    }
}

/** Get the reduced attribute set */
template <class TImage, class TFeatureImage>
bool
RadiometricAttributesLabelMapFilter<TImage, TFeatureImage>
::GetReducedAttributeSet() const
{
  return this->GetFunctor().GetReducedAttributeSet();
}

template <class TImage, class TFeatureImage>
void
RadiometricAttributesLabelMapFilter<TImage, TFeatureImage>
::BeforeThreadedGenerateData()
{
  // First call superclass implementation
  Superclass::BeforeThreadedGenerateData();

  // Clear any previous feature
  this->GetFunctor().ClearAllFeatures();

  // Gemi
  GEMIFilterPointerType gemi = GEMIFilterType::New();
  gemi->SetRedIndex(m_RedChannelIndex + 1);
  gemi->SetNIRIndex(m_NIRChannelIndex + 1);
  gemi->SetInput(this->GetFeatureImage());
  gemi->GetOutput()->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
  gemi->Update();
  this->GetFunctor().AddFeature("Gemi", gemi->GetOutput());

  // Ndvi
  NDVIFilterPointerType ndvi = NDVIFilterType::New();
  ndvi->SetRedIndex(m_RedChannelIndex + 1);
  ndvi->SetNIRIndex(m_NIRChannelIndex + 1);
  ndvi->SetInput(this->GetFeatureImage());
  ndvi->GetOutput()->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
  ndvi->Update();
  this->GetFunctor().AddFeature("Ndvi", ndvi->GetOutput());

  // IR
  IRFilterPointerType ir = IRFilterType::New();
  ir->SetGreenIndex(m_GreenChannelIndex + 1);
  ir->SetRedIndex(m_RedChannelIndex + 1);
  ir->SetInput(this->GetFeatureImage());
  ir->GetOutput()->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
  ir->Update();
  this->GetFunctor().AddFeature("Redness", ir->GetOutput());

  // IC
  ICFilterPointerType ic = ICFilterType::New();
  ic->SetGreenIndex(m_GreenChannelIndex + 1);
  ic->SetRedIndex(m_RedChannelIndex + 1);
  ic->SetInput(this->GetFeatureImage());
  ic->GetOutput()->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
  ic->Update();
  this->GetFunctor().AddFeature("Color", ic->GetOutput());

  // IB
  IBFilterPointerType ib = IBFilterType::New();
  ib->SetGreenIndex(m_GreenChannelIndex + 1);
  ib->SetRedIndex(m_RedChannelIndex + 1);
  ib->SetInput(this->GetFeatureImage());
  ib->GetOutput()->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
  ib->Update();
  this->GetFunctor().AddFeature("Brightness", ib->GetOutput());

  // NDWI2
  NDWI2FilterPointerType ndwi2 = NDWI2FilterType::New();
  ndwi2->GetFunctor().SetGIndex(m_GreenChannelIndex + 1);
  ndwi2->GetFunctor().SetNIRIndex(m_NIRChannelIndex + 1);
  ndwi2->SetInput(this->GetFeatureImage());
  ndwi2->GetOutput()->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
  ndwi2->Update();
  this->GetFunctor().AddFeature("Ndwi2", ndwi2->GetOutput());

  // Red
  ChannelFilterPointerType red = ChannelFilterType::New();
  red->SetChannel(m_RedChannelIndex + 1);
  red->SetInput(this->GetFeatureImage());
  red->GetOutput()->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
  red->Update();
  this->GetFunctor().AddFeature("Red", red->GetOutput());

  // Green
  ChannelFilterPointerType green = ChannelFilterType::New();
  green->SetChannel(m_GreenChannelIndex + 1);
  green->SetInput(this->GetFeatureImage());
  green->GetOutput()->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
  green->Update();
  this->GetFunctor().AddFeature("Green", green->GetOutput());

  // Blue
  ChannelFilterPointerType blue = ChannelFilterType::New();
  blue->SetChannel(m_BlueChannelIndex + 1);
  blue->SetInput(this->GetFeatureImage());
  blue->GetOutput()->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
  blue->Update();
  this->GetFunctor().AddFeature("Blue", blue->GetOutput());

  // Nir
  ChannelFilterPointerType nir = ChannelFilterType::New();
  nir->SetChannel(m_NIRChannelIndex + 1);
  nir->SetInput(this->GetFeatureImage());
  nir->GetOutput()->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
  nir->Update();
  this->GetFunctor().AddFeature("NIR", nir->GetOutput());
}

template <class TImage, class TFeatureImage>
void
RadiometricAttributesLabelMapFilter<TImage, TFeatureImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace itk
#endif
