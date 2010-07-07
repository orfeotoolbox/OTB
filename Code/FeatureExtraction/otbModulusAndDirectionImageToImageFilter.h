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
#ifndef __otbModulusAndDirectionImageToImageFilter_h
#define __otbModulusAndDirectionImageToImageFilter_h

#include "itkImageToImageFilter.h"

namespace otb
{

/** \class ModulusAndDirectionImageToImageFilter
 *
 * \brief Base class for modulus and direction image filters.
 *
 * This is the base class for alls class' generate an image dulus and
 * by using an modulus and a direction images intputs.
 * SetInput() method set the modulus image input and
 * SetInputDirection() set the image direction input.
 *
 */
template <class TInputImage,
    class TInputImageDirection,
    class TOutputImage>
class ITK_EXPORT ModulusAndDirectionImageToImageFilter :  public itk::ImageToImageFilter<TInputImage, TOutputImage>
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
  typedef ModulusAndDirectionImageToImageFilter              Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Method for management of the object factory. */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(ModulusAndDirectionImageToImageFilter, itk::ImageToImageFilter);

  typedef typename Superclass::InputImageType  InputImageType;
  typedef TInputImageDirection                 InputImageDirectionType;
  typedef typename Superclass::OutputImageType OutputImageType;

  /** Set/Get the image input of this process object.  */
  virtual void SetInput(const InputImageType *input);
  virtual void SetInputDirection(const InputImageDirectionType *direction);
  const InputImageType * GetInput(void);
  const InputImageDirectionType * GetInputDirection(void);

protected:
  ModulusAndDirectionImageToImageFilter();
  virtual ~ModulusAndDirectionImageToImageFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ModulusAndDirectionImageToImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbModulusAndDirectionImageToImageFilter.txx"
#endif

#endif
