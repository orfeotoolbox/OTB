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
#ifndef __otbChangeLabelImageFilter_h
#define __otbChangeLabelImageFilter_h

#include "itkChangeLabelImageFilter.h"

namespace otb
{
/** \class ChangeLabelImageFilter
 *  \brief VectorImage compliant version of the itk::ChangeLabelImageFilter.
 *
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ChangeLabelImageFilter
      : public itk::ChangeLabelImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard typedefs */
  typedef ChangeLabelImageFilter            Self;
  typedef itk::ChangeLabelImageFilter<TInputImage,TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ChangeLabelImageFilter,ChangeLabelImageFilter);

  /** Set/Get the number of components per pixel */
  itkSetMacro(NumberOfComponentsPerPixel,unsigned int);
  itkGetMacro(NumberOfComponentsPerPixel,unsigned int);


protected:
  /** Constructor */
  ChangeLabelImageFilter()
  {
    m_NumberOfComponentsPerPixel = 1;
  }
  /** Generate the output information missing */
  void GenerateOutputInformation()
  {
    Superclass::GenerateOutputInformation();

    this->GetOutput()->SetNumberOfComponentsPerPixel(m_NumberOfComponentsPerPixel);
  }
  /** Destructor */
  virtual ~ChangeLabelImageFilter() {};
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
    os<<indent<<"Number of components per pixel: "<<m_NumberOfComponentsPerPixel;
  }

private:
  ChangeLabelImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /// Number of components per pixel
  unsigned int m_NumberOfComponentsPerPixel;
};
}// End namespace otb

#endif
