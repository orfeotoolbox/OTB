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
#ifndef otbBinaryImageMinimalBoundingRegionCalculator_h
#define otbBinaryImageMinimalBoundingRegionCalculator_h

#include "itkImageToImageFilter.h"
#include "itkObjectFactory.h"

namespace otb
{
/**
 * \class BinaryImageMinimalBoundingRegionCalculator
 * \brief This class compute the smallest region of the image
 * containing every pixel with the foreground value.
 *
 * This class is used for instance in the RCC8 calculator filter,
 * where the input region used for computation has to be the smallest possible
 * for costs reasons. The Pad arg allows the user to get a region of pad  pixel larger
 * at each bound in case a security margin has to be kept.
 *
 * \sa ImageToImageRCC8Calculator
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputImage>
class ITK_EXPORT BinaryImageMinimalBoundingRegionCalculator
  : public itk::ImageToImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard typedefs */
  typedef BinaryImageMinimalBoundingRegionCalculator        Self;
  typedef itk::ImageToImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;
  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Type macro */
  itkTypeMacro(BinaryImageMinimalBoundingRegionCalculator, ImageToImageFilter);

  typedef TInputImage                         InputImageType;
  typedef typename InputImageType::PixelType  PixelType;
  typedef typename InputImageType::RegionType RegionType;
  typedef typename InputImageType::Pointer    InputImagePointerType;
  /** Toogle the pad option */
  itkGetMacro(Region, RegionType);
  itkSetMacro(InsideValue, PixelType);
  itkGetMacro(InsideValue, PixelType);
  itkSetMacro(Pad, unsigned int);
  itkGetMacro(Pad, unsigned int);

protected:
  /** Constructor */
  BinaryImageMinimalBoundingRegionCalculator();
  /** Destructor */
  ~BinaryImageMinimalBoundingRegionCalculator() ITK_OVERRIDE {}
  /** Main computation method */
  void GenerateData(void) ITK_OVERRIDE;
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;
private:
  BinaryImageMinimalBoundingRegionCalculator(const Self &); // purposely not implemented
  void operator =(const Self&); // purposely not implemented
  /** The computed region */
  RegionType m_Region;
  /** Toogle if pad wanted */
  unsigned int m_Pad;
  /** Inside value */
  PixelType m_InsideValue;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBinaryImageMinimalBoundingRegionCalculator.txx"
#endif

#endif
