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
#ifndef __otbHarrisImageToPointSetFilter_h
#define __otbHarrisImageToPointSetFilter_h

#include "otbThresholdImageToPointSetFilter.h"
#include "otbHarrisImageFilter.h"

namespace otb
{

/** \class HarrisImageToPointSetFilter
 * \brief TODO
 */

template <class TInputImage,
    class TOutputPointSet = itk::PointSet<typename TInputImage::PixelType, 2> >
class ITK_EXPORT HarrisImageToPointSetFilter :
  public ImageToPointSetFilter<TInputImage, TOutputPointSet>
{
public:

  itkStaticConstMacro(InputImageDimension,
                      unsigned int,
                      TInputImage::ImageDimension);

  typedef TInputImage     InputImageType;
  typedef TOutputPointSet OutputPointSetType;

  typedef HarrisImageToPointSetFilter                               Self;
  typedef ImageToPointSetFilter<InputImageType, OutputPointSetType> Superclass;
  typedef itk::SmartPointer<Self>                                   Pointer;
  typedef itk::SmartPointer<const Self>                             ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(HarrisImageToPointSetFilter, ImageToPointSetFilter);

  typedef typename InputImageType::PixelType InputPixelType;
  typedef typename InputImageType::SizeType  SizeType;

  typedef typename otb::HarrisImageFilter<InputImageType, InputImageType> HarrisImageFilterType;
  typedef typename otb::ThresholdImageToPointSetFilter<InputImageType,
      OutputPointSetType>       ThresholdImageToPointSetType;

  typedef typename OutputPointSetType::PixelType OutputPixelType;

  itkSetMacro(SigmaD, double);
  itkGetConstReferenceMacro(SigmaD, double);
  itkSetMacro(SigmaI, double);
  itkGetConstReferenceMacro(SigmaI, double);
  itkSetMacro(Alpha, double);
  itkGetConstReferenceMacro(Alpha, double);
  itkSetMacro(LowerThreshold, InputPixelType);
  itkGetConstReferenceMacro(LowerThreshold, InputPixelType);
  itkSetMacro(UpperThreshold, InputPixelType);
  itkGetConstReferenceMacro(UpperThreshold, InputPixelType);

protected:
  HarrisImageToPointSetFilter();
  virtual ~HarrisImageToPointSetFilter() {}

  virtual void GenerateData();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  HarrisImageToPointSetFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  InputPixelType m_LowerThreshold;
  InputPixelType m_UpperThreshold;
  double         m_SigmaD;
  double         m_SigmaI;
  double         m_Alpha;

  typename HarrisImageFilterType::Pointer m_HarrisFilter;
  typename ThresholdImageToPointSetType::Pointer m_ThresholdFilter;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbHarrisImageToPointSetFilter.txx"
#endif

#endif
