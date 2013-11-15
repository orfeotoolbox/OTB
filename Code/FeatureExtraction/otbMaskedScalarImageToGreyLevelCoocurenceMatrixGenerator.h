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
#ifndef __otbMaskedScalarImageToGreyLevelCoocurrenceMatrixGenerator_h
#define __otbMaskedScalarImageToGreyLevelCoocurrenceMatrixGenerator_h

#include "itkScalarImageToCooccurrenceMatrixFilter.h"
#include "otbMacro.h"

namespace otb
{

//FIXME might be useless now with the new stat framework

/** \class MaskedScalarImageToGreyLevelCooccurrenceMatrixGenerator
 *  \brief This class extends the ITK version by providing matrix estimation on a sub-region.
 *
 *  This class compute the co-occurence matrix over a given region of the image. The region can be set
 *  using the SetRegion()/GetRegion() methods.
 *
 *  \sa itk::Statistics::MaskedScalarImageToGreyLevelCooccurrenceMatrixGenerator
 */
template <class TImage, class THistogramFrequencyContainer = itk::Statistics::DenseFrequencyContainer2>
class MaskedScalarImageToGreyLevelCooccurrenceMatrixGenerator
  : public itk::Statistics::ScalarImageToCooccurrenceMatrixFilter<TImage,
      THistogramFrequencyContainer>//FIXME check templates
{
public:
  /** Standard class typedef */
  typedef MaskedScalarImageToGreyLevelCooccurrenceMatrixGenerator Self;
  typedef itk::Statistics::ScalarImageToCooccurrenceMatrixFilter
  <TImage, THistogramFrequencyContainer>                              Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Creation through object factory */
  itkNewMacro(Self);

  /** RTTI */
  itkTypeMacro(MaskedScalarImageToGreyLevelCooccurrenceMatrixGenerator,
               itk::Statistics::ScalarImageToCooccurrenceMatrixFilter);

  /** Superclass typedefs */
  typedef TImage                          ImageType;
  typedef typename ImageType::RegionType  RegionType;
  typedef typename Superclass::RadiusType RadiusType;

  /** Set/Get the region on which the co-occurence matrix is computed */
  itkSetMacro(Region, RegionType);
  itkGetMacro(Region, RegionType);

protected:
  /** Constructor */
  MaskedScalarImageToGreyLevelCooccurrenceMatrixGenerator() : m_Region() {}
  /** Destructor */
  ~MaskedScalarImageToGreyLevelCooccurrenceMatrixGenerator(){}

  /** Overload this method to restrict co-occurence to the given region */
  virtual void FillHistogram(RadiusType radius, RegionType region)
  {
    // Check that region is inside the requested region
    RegionType subregion = m_Region;

    if (subregion.Crop(region))
      {
      // If so, call superclass FillHistogram implementation
      this->Superclass::FillHistogram(radius, subregion);
      }
  }

private:
  MaskedScalarImageToGreyLevelCooccurrenceMatrixGenerator(const Self&);  //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Region on which to compute co-occurence */
  RegionType m_Region;

};
} // End namespace otb
 #endif
