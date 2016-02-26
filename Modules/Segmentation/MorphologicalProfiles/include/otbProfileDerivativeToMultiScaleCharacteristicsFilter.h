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
#ifndef __otbProfileDerivativeToMultiScaleCharacteristicsFilter_h
#define __otbProfileDerivativeToMultiScaleCharacteristicsFilter_h

#include "otbImageListToImageFilter.h"

namespace otb
{
/** \class ProfileDerivativeToMultiScaleCharacteristicsFilter
 *  \brief This filter extracts the maximum derivative value of the input profile.
 *
 *  This filter extracts the maximum derivative value of the input profile as well
 *  as the parameter (called characteristic) for which this maxima occurs.
 *
 *  The maxima can be retrieved by the GetOutput() image, while the characteristics
 *  can be retreived by the GetOutputCharacteristics().
 *
 *  Please note that this characteristics can be scaled using the InitialValue and Step
 *  parameter corresponding to the input profile.
 *
 * \sa ProfileToProfileDerivativeImageFilter
 *
 * \ingroup OTBMorphologicalProfiles
 */
template <class TInputImage, class TOutputImage, class TLabeledImage>
class ITK_EXPORT ProfileDerivativeToMultiScaleCharacteristicsFilter
  : public ImageListToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef ProfileDerivativeToMultiScaleCharacteristicsFilter Self;
  typedef ImageListToImageFilter<TInputImage, TOutputImage>  Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ProfileDerivativeToMultiScaleCharacteristicsFilter, ImageToImageListFilter);

  /** Template parameters typedefs */
  typedef TInputImage                             InputImageType;
  typedef TOutputImage                            OutputImageType;
  typedef TLabeledImage                           LabeledImageType;
  typedef typename InputImageType::Pointer        InputImagePointerType;
  typedef typename OutputImageType::Pointer       OutputImagePointerType;
  typedef typename LabeledImageType::Pointer      LabeledImagePointerType;
  typedef typename Superclass::InputImageListType InputImageListType;
  typedef typename InputImageListType::Pointer    InputImageListPointerType;
  typedef typename OutputImageType::RegionType    RegionType;
  typedef typename LabeledImageType::PixelType    LabeledPixelType;
  typedef typename OutputImageType::PixelType     OutputPixelType;

  /**
   * Returns the characteristics image.
   * \return The Characteristics image.
   */
  virtual TLabeledImage * GetOutputCharacteristics(void);

  /** Set/Get the initial profile value */
  itkSetMacro(InitialValue, LabeledPixelType);
  itkGetMacro(InitialValue, LabeledPixelType);
  /** Set/Get the profile step */
  itkSetMacro(Step, LabeledPixelType);
  itkGetMacro(Step, LabeledPixelType);

protected:
  /** Main computation method */
  virtual void GenerateData(void);

  /** GenerateOutputInformation
   * Set the number of bands of the output.
   * Copy information from the first image of the list if existing.
   **/
  virtual void GenerateOutputInformation(void);

  /**
   * GenerateInputRequestedRegion
   * Set the requested region of each image in the list.
   */
  virtual void GenerateInputRequestedRegion(void);

  /** Constructor */
  ProfileDerivativeToMultiScaleCharacteristicsFilter();
  /** Destructor */
  virtual ~ProfileDerivativeToMultiScaleCharacteristicsFilter() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ProfileDerivativeToMultiScaleCharacteristicsFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** The initial value of the profile */
  LabeledPixelType m_InitialValue;
  /** The step of the profile */
  LabeledPixelType m_Step;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbProfileDerivativeToMultiScaleCharacteristicsFilter.txx"
#endif

#endif
