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
#ifndef __otbMorphologicalOpeningProfileFilter_h
#define __otbMorphologicalOpeningProfileFilter_h

#include "otbImageToProfileFilter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkOpeningByReconstructionImageFilter.h"

namespace otb
{
/** \class MorphologicalOpeningProfileFilter
 *  \brief This filter compute the morphological opening profile.
 *
 * This algorithm is based on the following publication:
 * \par
 * Martino Pesaresi and Jon Alti Benediktsson, Member, IEEE:
 * A new approach for the morphological segmentation of high resolution
 * satellite imagery. IEEE Transactions on geoscience and remote sensing, vol. 39,
 * NO. 2, February 2001, p. 309-320.
 * \par
 *
 * The opening profile is a set of images beeing the result of a geodesic morphological
 * opening by reconstruction with an increasing range of structuring element sizes.
 *
 * For more information on profiles please refer to the documentation of the otb::ImageToProfileFilter
 * class.
 *
 * \sa ImageToProfileFilter
 * \sa itk::OpeningByReconstructionImageFilter
 *
 * \ingroup OTBMorphologicalProfiles
 */
template <class TInputImage, class TOutputImage, class TStructuringElement>
class ITK_EXPORT MorphologicalOpeningProfileFilter
  : public ImageToProfileFilter<TInputImage, TOutputImage,
      itk::OpeningByReconstructionImageFilter
      <TInputImage, TOutputImage, TStructuringElement>,
      unsigned int>
{
public:
  /** Standard typedefs */
  typedef MorphologicalOpeningProfileFilter Self;
  typedef ImageToProfileFilter<TInputImage, TOutputImage,
      itk::OpeningByReconstructionImageFilter
      <TInputImage, TOutputImage, TStructuringElement>,
      unsigned int> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(MorphologicalOpeningProfileFilter, ImageToProfileFilter);

  typedef TStructuringElement                StructuringElementType;
  typedef typename Superclass::ParameterType ParameterType;

protected:
  /** Set the profile parameter */
  virtual void SetProfileParameter(ParameterType param)
  {
    StructuringElementType se;
    se.SetRadius(param);
    se.CreateStructuringElement();
    this->GetFilter()->SetKernel(se);
  }
  /** Constructor */
  MorphologicalOpeningProfileFilter() {};
  /** Destructor */
  virtual ~MorphologicalOpeningProfileFilter() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  MorphologicalOpeningProfileFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};
} // End namespace otb
#endif
