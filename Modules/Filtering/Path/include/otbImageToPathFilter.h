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
#ifndef __otbImageToPathFilter_h
#define __otbImageToPathFilter_h

#include "itkPathSource.h"

namespace otb
{
/**
 * \class ImageToPathFilter
 * \brief Base class for filters taking an image as input, and producing an
 * output of type Path.
 *
 * \ingroup OTBPath
 */
template <class TInputImage, class TOutputPath>
class ITK_EXPORT ImageToPathFilter
  : public itk::PathSource<TOutputPath>
{
public:
  /// Standard class typedefs
  typedef ImageToPathFilter             Self;
  typedef itk::PathSource<TOutputPath>  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef itk::ProcessObject            ProcessObjectType;
  /// Method for creation through the object factory
  itkNewMacro(Self);
  /// Runtime information
  itkTypeMacro(PathToImageFilter, PathSource);
  /// template parameters typedefs
  typedef          TInputImage             InputImageType;
  typedef typename InputImageType::Pointer InputImagePointerType;
  typedef          TOutputPath             OutputPathType;
  typedef typename OutputPathType::Pointer OutputPathPointerType;
  /// Input image dimension
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  /// Input image Setters and getters
  using Superclass::SetInput;
  virtual void SetInput(const InputImageType * Image);
  const InputImageType * GetInput(void);

protected:
  ImageToPathFilter();
  ~ImageToPathFilter() ITK_OVERRIDE {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:
  ImageToPathFilter(const Self &);
  void operator =(const Self&);
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToPathFilter.txx"
#endif

#endif
