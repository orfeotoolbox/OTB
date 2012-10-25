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
#ifndef __otbMorphologicalProfilesSegmentationFilter_h
#define __otbMorphologicalProfilesSegmentationFilter_h

#include "otbMorphologicalOpeningProfileFilter.h"
#include "otbMorphologicalClosingProfileFilter.h"
#include "otbProfileToProfileDerivativeFilter.h"
#include "otbProfileDerivativeToMultiScaleCharacteristicsFilter.h"
#include "otbMultiScaleConvexOrConcaveClassificationFilter.h"
#include "otbImage.h"
#include "itkScalarConnectedComponentImageFilter.h"
#include "itkBinaryBallStructuringElement.h"

namespace otb
{
/** \class MorphologicalProfilesSegmentationFilter
*
*  
*/

template <class TInputImage,  class TOuputImage, class TInternalPrecision = float, class TStructuringElement = itk::BinaryBallStructuringElement<typename TInputImage::PixelType, TInputImage::ImageDimension> >
class ITK_EXPORT MorphologicalProfilesSegmentationFilter 
  : public itk::ImageToImageFilter<TInputImage, TOuputImage>
{
public:
/** Standard Self typedef */
typedef MorphologicalProfilesSegmentationFilter Self;
typedef itk::ImageToImageFilter<TInputImage, TOuputImage> Superclass;

typedef itk::SmartPointer<Self>       Pointer;
typedef itk::SmartPointer<const Self> ConstPointer;

/** Some convenient typedefs. */
typedef TInputImage                        InputImageType;
typedef typename InputImageType::PixelType InputPixelType;
typedef TOuputImage                        OutputImageType;
typedef otb::Image<TInternalPrecision>     InternalImageType;

typedef TStructuringElement                StructuringElementType;

typedef otb::MorphologicalOpeningProfileFilter<InputImageType, InternalImageType, StructuringElementType>
OpeningProfileFilterType;
typedef otb::MorphologicalClosingProfileFilter<InputImageType, InternalImageType, StructuringElementType>
ClosingProfileFilterType;
typedef otb::ProfileToProfileDerivativeFilter<InternalImageType, InternalImageType> DerivativeFilterType;
typedef otb::ProfileDerivativeToMultiScaleCharacteristicsFilter<InternalImageType, InternalImageType, OutputImageType>
MultiScaleCharacteristicsFilterType;
typedef otb::MultiScaleConvexOrConcaveClassificationFilter<InternalImageType,
                                                           OutputImageType> MultiScaleClassificationFilterType;
typedef itk::ScalarConnectedComponentImageFilter<OutputImageType,OutputImageType> ConnectedComponentsFilterType;

/** Method for creation through the object factory. */
itkNewMacro(Self);

/** Runtime information support. */
itkTypeMacro(MorphologicalProfilesSegmentationFilter, itk::ImageToImageFilter);

itkSetMacro(ProfileSize,unsigned int);
itkGetConstReferenceMacro(ProfileSize,unsigned int);
itkSetMacro(ProfileStart,unsigned int);
itkGetConstReferenceMacro(ProfileStart,unsigned int);
itkSetMacro(ProfileStep,unsigned int);
itkGetConstReferenceMacro(ProfileStep,unsigned int);
itkSetMacro(Sigma,double);
itkGetConstReferenceMacro(Sigma,double);

protected:
MorphologicalProfilesSegmentationFilter();

virtual ~MorphologicalProfilesSegmentationFilter();

virtual void GenerateData();

private:
typename OpeningProfileFilterType::Pointer m_OpeningProfile;
typename ClosingProfileFilterType::Pointer m_ClosingProfile;
typename DerivativeFilterType::Pointer m_OpeningDerivativeProfile;
typename DerivativeFilterType::Pointer m_ClosingDerivativeProfile;
typename MultiScaleCharacteristicsFilterType::Pointer m_OpeningCharacteristicsFilter;
typename MultiScaleCharacteristicsFilterType::Pointer m_ClosingCharacteristicsFilter;
typename MultiScaleClassificationFilterType::Pointer m_ClassificationFilter;
typename ConnectedComponentsFilterType::Pointer m_ConnectedComponentsFilter;

unsigned int m_ProfileSize;
unsigned int m_ProfileStart;
unsigned int m_ProfileStep;
double       m_Sigma;
};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMorphologicalProfilesSegmentationFilter.txx"
#endif

#endif


