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
#ifndef __otbImageToModulusAndDirectionImageFilter_h
#define __otbImageToModulusAndDirectionImageFilter_h

#include "itkImageToImageFilter.h"

namespace otb
{

/** \class ImageToModulusAndDirectionImageFilter
 *
 * \brief Base class for modulus and direction image filters.
 *
 * This is the base class for alls class' generate modulus and
 * directions outputs.
 * GetOutput() method return the output image modulus and
 * GetOutputDirection() return the output image direction
 *
 *
 * \ingroup OTBCommon
 */
template <class TInputImage,
    class TOutputImage,
    class TOutputImageDirection = TOutputImage>
class ITK_EXPORT ImageToModulusAndDirectionImageFilter :  public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /**   Extract dimensions as well of the images of entry of exit. */
  itkStaticConstMacro(InputImageDimension,
                      unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension,
                      unsigned int,
                      TOutputImage::ImageDimension);

  /** typedef for the classes standards. */
  typedef ImageToModulusAndDirectionImageFilter              Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Method for management of the object factory. */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(ImageToModulusAndDirectionImageFilter, itk::ImageToImageFilter);

  typedef typename Superclass::InputImageType  InputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;
  typedef TOutputImageDirection                OutputImageDirectionType;

  typedef typename InputImageType::Pointer       InputImagePointer;
  typedef typename InputImageType::ConstPointer  InputImageConstPointer;
  typedef typename InputImageType::RegionType    InputImageRegionType;
  typedef typename OutputImageType::Pointer      OutputImagePointer;
  typedef typename OutputImageType::ConstPointer OutputImageConstPointer;
  typedef typename OutputImageType::RegionType   OutputImageRegionType;

  /** Return the const output image modulus */
  const OutputImageType * GetOutput() const;

  /** Return the output image modulus */
  OutputImageType * GetOutput();

  /** Return the const output image direction */
  const OutputImageDirectionType * GetOutputDirection() const;
  /** Return the output image direction */
  OutputImageDirectionType * GetOutputDirection();

  virtual void GenerateInputRequestedRegion();

protected:
  ImageToModulusAndDirectionImageFilter();
  virtual ~ImageToModulusAndDirectionImageFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ImageToModulusAndDirectionImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToModulusAndDirectionImageFilter.txx"
#endif

#endif
