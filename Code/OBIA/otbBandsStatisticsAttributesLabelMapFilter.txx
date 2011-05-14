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
#ifndef __otbBandsStatisticsAttributesLabelMapFilter_txx
#define __otbBandsStatisticsAttributesLabelMapFilter_txx

#include "otbBandsStatisticsAttributesLabelMapFilter.h"
#include "itkVectorIndexSelectionCastImageFilter.h"

namespace otb
{
namespace Functor
{
/** Constructor */
template <class TLabelObject, class TFeatureImage>
BandStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::BandStatsAttributesLabelObjectFunctor() : m_ReducedAttributeSet(true),
  m_StatsFunctorsMap()
{}

/** Destructor */
template <class TLabelObject, class TFeatureImage>
BandStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::~BandStatsAttributesLabelObjectFunctor(){}

/** The comparators */
template <class TLabelObject, class TFeatureImage>
bool
BandStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::operator != (const Self &self)
  {
  bool resp = true;
  resp = resp && (m_ReducedAttributeSet != self.m_ReducedAttributeSet);
  resp = resp && (m_StatsFunctorsMap != self.m_StatsFunctorsMap);
  return resp;
  }

template <class TLabelObject, class TFeatureImage>
bool
BandStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::operator == (const Self &self)
  {
  return !(this != self);
  }

/** This is the functor implementation
 *  Calling the functor on a label object
 *  will update its statistics attributes */
template <class TLabelObject, class TFeatureImage>
void
BandStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
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
BandStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
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
BandStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::RemoveFeature(const std::string& name)
{
  return (m_StatsFunctorsMap.erase(name) == 1);
}

/** Get the feature image with this name */
template <class TLabelObject, class TFeatureImage>
const TFeatureImage *
BandStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::GetFeatureImage(const std::string& name) const
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
BandStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::ClearAllFeatures()
{
  m_StatsFunctorsMap.clear();
}

/** Get the number of features */
template <class TLabelObject, class TFeatureImage>
unsigned int
BandStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::GetNumberOfFeatures() const
{
  return m_StatsFunctorsMap.size();
}

/** Set the reduced attribute set */
template <class TLabelObject, class TFeatureImage>
void
BandStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
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
BandStatsAttributesLabelObjectFunctor<TLabelObject, TFeatureImage>
::GetReducedAttributeSet() const
{
  return m_ReducedAttributeSet;
}
} // End namespace Functor

template <class TImage, class TFeatureImage>
BandsStatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::BandsStatisticsAttributesLabelMapFilter()
{
  this->SetNumberOfRequiredInputs(2);
}

/** Set the feature image */
template <class TImage, class TFeatureImage>
void
BandsStatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::SetFeatureImage(const TFeatureImage *input)
{
  // Set the Nth input
  this->SetNthInput(1, const_cast<TFeatureImage*>(input));
}

/** Get the feature image */
template <class TImage, class TFeatureImage>
const typename BandsStatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::FeatureImageType *
BandsStatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::GetFeatureImage() const
{
  return static_cast<const TFeatureImage *>(this->itk::ProcessObject::GetInput(1));
}

/** Set the reduced attribute set */
template <class TImage, class TFeatureImage>
void
BandsStatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
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
BandsStatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::GetReducedAttributeSet() const
{
  return this->GetFunctor().GetReducedAttributeSet();
}

template <class TImage, class TFeatureImage>
void
BandsStatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::GenerateInputRequestedRegion()
{

  for (unsigned int idx = 0; idx < this->GetNumberOfInputs(); ++idx)
      {
       ImagePointer input = const_cast<ImageType *>(this->GetInput(idx));
      if (!input.IsNull())
        {
        input->SetRequestedRegionToLargestPossibleRegion();
        // Check whether the input is an image of the appropriate
        // dimension (use ProcessObject's version of the GetInput()
        // method since it returns the input as a pointer to a
        // DataObject as opposed to the subclass version which
        // static_casts the input to an TInputImage).

        // Use the function object RegionCopier to copy the output region
        // to the input.  The default region copier has default implementations
        // to handle the cases where the input and output are the same
        // dimension, the input a higher dimension than the output, and the
        // input a lower dimension than the output.
        InputImageRegionType inputRegion;
        this->CallCopyOutputRegionToInputRegion(inputRegion, this->GetOutput()->GetRequestedRegion());
        input->SetRequestedRegion( inputRegion );
        }
      }
}


template <class TImage, class TFeatureImage>
void
BandsStatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::BeforeThreadedGenerateData()
{
  // First call superclass implementation
  Superclass::BeforeThreadedGenerateData();

  unsigned long nbComponents = this->GetFeatureImage()->GetNumberOfComponentsPerPixel();

  // Clear any previous feature
  this->GetFunctor().ClearAllFeatures();

  // Add each band of the feature image to the statistics functor
  for (unsigned int i = 0; i < nbComponents; ++i)
    {
    typedef itk::VectorIndexSelectionCastImageFilter<FeatureImageType, InternalImageType>
      VectorIndexSelectionCastImageFilterType;

    typename VectorIndexSelectionCastImageFilterType::Pointer band = VectorIndexSelectionCastImageFilterType::New();
    band->SetInput(this->GetFeatureImage());
    band->SetIndex(i);
    band->UpdateOutputInformation();
    band->GetOutput()->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
    band->Update();
    std::ostringstream oss;
    oss << "Band" << i + 1; // [1..N] convention in feature naming
    this->GetFunctor().AddFeature(oss.str(), band->GetOutput());

    }
}

template <class TImage, class TFeatureImage>
void
BandsStatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace itk
#endif
