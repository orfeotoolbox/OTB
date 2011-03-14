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
#ifndef __otbQuickLookImageGenerator_h
#define __otbQuickLookImageGenerator_h

// This class is deprecated. Use StreamingShrinkImageFilter instead
#include "vcl_deprecated_header.h"

#include "otbStreamingShrinkImageFilter.h"

namespace otb
{
/** \class QuickLookImageGenerator
 *  \brief <b>DEPRECATED:</b> Use StreamingShrinkImageFilter instead
 *
 *  \deprecated Use StreamingShrinkImageFilter instead
 *
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT QuickLookImageGenerator :
  public otb::StreamingShrinkImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef QuickLookImageGenerator                                     Self;
  typedef TInputImage                                                 InputImageType;
  typedef TOutputImage                                                OutputImageType;

  typedef typename otb::StreamingShrinkImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                                     Pointer;
  typedef itk::SmartPointer<const Self>                               ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  void SetVariance(double var)
  {
    itkWarningMacro(<< "DEPRECATED function")
  }
  itkGetMacro(Variance, double);

  void UseImageSpacing(bool boo)
  {
    itkWarningMacro(<< "DEPRECATED function")
  }
  itkGetMacro(UseImageSpacing, bool);

  void SetMaximumKernelWidth(unsigned int width)
  {
    itkWarningMacro(<< "DEPRECATED function")
  }
  itkGetMacro(MaximumKernelWidth, unsigned int);

  void SetMaximumError(double error)
  {
    itkWarningMacro(<< "DEPRECATED function")
  }
  itkGetMacro(MaximumError, double);


  void SetSampleRatio(unsigned int sr)
  {
    this->SetShrinkFactor(sr);
  }

  unsigned int GetSampleRatio(unsigned int sr)
  {
    return this->GetShrinkFactor(sr);
  }

protected:
  QuickLookImageGenerator() {}
  virtual ~QuickLookImageGenerator(){}

private:
  QuickLookImageGenerator(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // End namespace otb

#endif
