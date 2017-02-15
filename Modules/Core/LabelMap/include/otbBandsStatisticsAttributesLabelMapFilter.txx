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
#ifndef otbBandsStatisticsAttributesLabelMapFilter_txx
#define otbBandsStatisticsAttributesLabelMapFilter_txx

#include "otbBandsStatisticsAttributesLabelMapFilter.h"
#include "itkUnaryFunctorImageFilter.h"
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
::AllocateOutputs()
 {
 // if told to run in place and the types support it,
  if( this->GetInPlace() && this->CanRunInPlace() )
    {
    // Graft this first input to the output.  Later, we'll need to
    // remove the input's hold on the bulk data.
    //
   ImagePointer inputAsOutput = dynamic_cast<TImage *>(const_cast<TImage *>(this->GetInput()));

    if( inputAsOutput )
      {

      this->GraftOutput( inputAsOutput );
      this->GetOutput()->SetLargestPossibleRegion(this->GetOutput()->GetLargestPossibleRegion());
      this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
      this->GetOutput()->SetBufferedRegion(this->GetOutput()->GetBufferedRegion());

      }

    // If there are more than one outputs, allocate the remaining outputs
    for (unsigned int i=1; i < this->GetNumberOfOutputs(); ++i)
      {
      ImagePointer outputPtr;

      outputPtr = this->GetOutput(i);
      outputPtr->SetBufferedRegion(outputPtr->GetRequestedRegion());
      outputPtr->Allocate();
      }
    }
  else
    {
    //

    Superclass::AllocateOutputs();
    // copy the content of the input image to the output image (will be done by ImageSource AllocateOutputs Method)
    // would never occur : inputasoutput condition is always true, since output and input type is TImage for
    // BandsStatisticsAttributesLabelMapFilter class
    }
}


template <class TImage, class TFeatureImage>
void
BandsStatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::GenerateInputRequestedRegion()
{
  itk::ImageToImageFilter<TImage, TImage>::GenerateInputRequestedRegion();
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

} // end namespace otb
#endif
