/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) GET / ENST Bretagne. All rights reserved.
  See GETCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbROIdataConversion_h
#define __otbROIdataConversion_h

#include <vector>
#include "otbImage.h"
#include "itkImageToImageFilter.h"

namespace otb
{

/** \class ROIdataConversion
 * \brief image data to vector conversion.
 *
 * A class that extract pixel value from an input image according to a ROI
 * image and outputs it on a vector of PixelType.
 *
 * \ingroup OTBProjection
 */
template <class TInputImage, class TInputROIImage>
class ROIdataConversion
  : public itk::ImageToImageFilter<TInputImage, otb::Image<typename TInputImage::PixelType, 1> >
{
public:
  typedef ROIdataConversion                                                                     Self;
  typedef itk::ImageToImageFilter<TInputImage, otb::Image<typename TInputImage::PixelType, 1> > Superclass;
  typedef itk::SmartPointer<Self>                                                               Pointer;
  typedef itk::SmartPointer<const Self>                                                         ConstPointer;

  itkNewMacro(Self);
  itkTypeMacro(ROIdataConversion, itk::ImageToImageFilter);

  typedef TInputImage                          InputImageType;
  typedef typename TInputImage::PixelType      InputImagePixelType;
  typedef typename TInputImage::Pointer        InputImagePointerType;
  typedef TInputROIImage                       InputROIImageType;
  typedef typename TInputROIImage::Pointer     InputROIImagePointerType;
  typedef typename Superclass::OutputImageType OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;

  typedef typename OutputImageType::SizeType::SizeValueType SizeValueType;

  /** Gets/Sets the input image */
  const InputImageType * GetInputImage()
  {
    return this->Superclass::GetInput();
  }
  void SetInputImage(const InputImageType * img)
  {
    this->Superclass::SetInput(img);
  }

  /** Gets/Sets the ROI image */
  InputROIImageType * GetROIImage()
  {
    return static_cast<InputROIImageType *> (this->itk::ProcessObject::GetInput(1));
  }
  void SetROIImage(const InputROIImageType * img)
  {
    this->itk::ProcessObject::SetNthInput(1, const_cast<InputROIImageType *>(img));
  }

protected:
  ROIdataConversion();
  virtual ~ROIdataConversion() {}
  virtual void GenerateOutputInformation();
  virtual void GenerateInputRequestedRegion();
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

  /** Performs its job! */
  virtual void GenerateData();

  /** Count the number for sample in the training area */
  SizeValueType GetNumberOfSample();
};

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbROIdataConversion.txx"
#endif

#endif
