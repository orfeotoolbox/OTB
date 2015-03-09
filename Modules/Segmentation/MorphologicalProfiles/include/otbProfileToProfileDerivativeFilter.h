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
#ifndef __otbProfileToProfileDerivativeFilter_h
#define __otbProfileToProfileDerivativeFilter_h

#include "otbImageListToImageListFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkAbsImageFilter.h"

namespace otb
{
/** \class ProfileToProfileDerivativeFilter
 *  \brief This filter computes the derivative of a given profile.
 *
 * For the profile \f$ \Pi_{\phi}(f)= \{\phi_{n}(f), n \in \{n_{1},\ldots, n_{N}\}\}\f$, the
 * profile derivative is defined by:
 *
 * \f[
 * \Delta\Pi_{\phi}(f)= \{\Delta\phi_{n}(f), n \in \{n_{1},\ldots, n_{N}\}\}
 * \f]
 *
 * with \f$ \Delta\phi_{n}(f) = \mid \Pi_{\phi_{n2}}(f)-\Pi_{\phi_{n1}}(f) \mid \f$
 *
 * \ingroup Streamed
 *
 * \ingroup OTBMorphologicalProfiles
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ProfileToProfileDerivativeFilter
  : public ImageListToImageListFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef ProfileToProfileDerivativeFilter                      Self;
  typedef ImageListToImageListFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                               Pointer;
  typedef itk::SmartPointer<const Self>                         ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ProfileToProfileDerivativeFilter, ImageListToImageListFilter);

  /** Template parameters typedefs */
  typedef typename Superclass::InputImageListType  InputImageListType;
  typedef typename InputImageListType::Pointer     InputImageListPointerType;
  typedef typename Superclass::OutputImageListType OutputImageListType;
  typedef typename OutputImageListType::Pointer    OutputImageListPointerType;
  typedef typename OutputImageListType::ImageType  OutputImageType;
  typedef typename InputImageListType::ImageType   InputImageType;
  typedef typename InputImageType::Pointer         InputImagePointerType;

  /** typedefs of the filters used for the derivative */
  typedef itk::SubtractImageFilter<InputImageType, InputImageType, InputImageType> SubtractFilterType;
  typedef itk::AbsImageFilter<InputImageType, InputImageType>                      AbsFilterType;
  typedef typename SubtractFilterType::Pointer                                     SubtractFilterPointerType;
  typedef typename AbsFilterType::Pointer                                          AbsFilterPointerType;

  /** Generate output information for the ImageList and for each image
      in the list. */
  virtual void GenerateOutputInformation(void);

  /** Generate input requested region for each image in the list. */
  virtual void GenerateInputRequestedRegion(void);

protected:
  /** Main computation method */
  virtual void GenerateData(void);
  /** Constructor */
  ProfileToProfileDerivativeFilter();
  /** Destructor */
  virtual ~ProfileToProfileDerivativeFilter() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ProfileToProfileDerivativeFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Subtract filter for the dervivative */
  SubtractFilterPointerType m_SubtractFilter;
  /** Abs filter for the derivative */
  AbsFilterPointerType m_AbsFilter;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbProfileToProfileDerivativeFilter.txx"
#endif

#endif
