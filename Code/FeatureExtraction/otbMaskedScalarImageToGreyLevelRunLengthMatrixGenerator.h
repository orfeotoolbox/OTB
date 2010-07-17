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
#ifndef __otbMaskedScalarImageToGreyLevelRunLengthMatrixGenerator_h
#define __otbMaskedScalarImageToGreyLevelRunLengthMatrixGenerator_h

#include "itkMaskedScalarImageToGreyLevelRunLengthMatrixGenerator.h"
#include "otbMacro.h"

namespace otb
{
/** \class MaskedScalarImageToGreyLevelRunLengthMatrixGenerator
 *  \brief This class extends the ITK version by providing matrix estimation on a sub-region.
 *
 *  This class compute the run-length matrix over a given region of the image. The region can be set
 *  using the SetRegion()/GetRegion() methods.
 *
 *  \sa itk::Statistics::MaskedScalarImageToGreyLevelRunLengthMatrixFilter
 */
template <class TImage, class THistogramFrequencyContainer = itk::Statistics::DenseFrequencyContainer>
class MaskedScalarImageToGreyLevelRunLengthMatrixGenerator
  : public itk::Statistics::MaskedScalarImageToGreyLevelRunLengthMatrixGenerator<TImage,
      THistogramFrequencyContainer>
{
public:
  /** Standard class typedef */
  typedef MaskedScalarImageToGreyLevelRunLengthMatrixGenerator
                                                     Self;
  typedef itk::Statistics::MaskedScalarImageToGreyLevelRunLengthMatrixGenerator
    <TImage, THistogramFrequencyContainer>           Superclass;
  typedef itk::SmartPointer<Self>                    Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;

  /** Creation through object factory */
  itkNewMacro(Self);

  /** RTTI */
  itkTypeMacro(MaskedScalarImageToGreyLevelRunLengthMatrixGenerator,
               itk::Statistics::MaskedScalarImageToGreyLevelRunLengthMatrixGenerator);

  /** Superclass typedefs */
  typedef TImage                             ImageType;
  typedef typename ImageType::RegionType     RegionType;
  typedef typename ImageType::SizeType       RadiusType;

  /** Set/Get the region on which the run length matrix is computed */
  itkSetMacro(Region, RegionType);
  itkGetMacro(Region, RegionType);

protected:
  /** Constructor */
  MaskedScalarImageToGreyLevelRunLengthMatrixGenerator() : m_Region() {}
  /** Destructor */
  ~MaskedScalarImageToGreyLevelRunLengthMatrixGenerator(){}

  /** Overload this method to restrict run length to the given region */
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
  MaskedScalarImageToGreyLevelRunLengthMatrixGenerator(const Self&);  //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Region on which to compute co-occurence */
  RegionType m_Region;

};
} // End namespace otb
 #endif
