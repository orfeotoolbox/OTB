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
#ifndef __otbThresholdImageToPointSetFilter_h
#define __otbThresholdImageToPointSetFilter_h

#include "otbImageToPointSetFilter.h"
#include "itkPointSet.h"
#include "itkConceptChecking.h"

namespace otb
{

/** \class ThresholdImageToPointSetFilter
 * \brief Produce a PointSet according to filtering conditions
 *
 */

template <class TInputImage,
    class TOutputPointSet = itk::PointSet<typename TInputImage::PixelType, 2> >
class ITK_EXPORT ThresholdImageToPointSetFilter :
  public ImageToPointSetFilter<TInputImage, TOutputPointSet>
{
public:

  itkStaticConstMacro(InputImageDimension,
                      unsigned int,
                      TInputImage::ImageDimension);

  typedef TInputImage InputImageType;

  typedef ThresholdImageToPointSetFilter                         Self;
  typedef ImageToPointSetFilter<InputImageType, TOutputPointSet> Superclass;
  typedef typename Superclass::OutputPointSetType                OutputPointSetType;
  typedef itk::SmartPointer<Self>                                Pointer;
  typedef itk::SmartPointer<const Self>                          ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(ThresholdImageToPointSetFilter, ImageToPointSetFilter);

  typedef typename Superclass::InputImagePixelType       InputPixelType;
  typedef typename Superclass::InputImagePointer         InputImagePointer;
  typedef typename Superclass::InputImageRegionType      InputImageRegionType;
  typedef typename Superclass::InputImageConstPointer    InputImageConstPointer;
  typedef typename Superclass::InputImageType::SizeType  SizeType;
  typedef typename Superclass::InputImageType::IndexType IndexType;

  typedef typename Superclass::OutputPointSetPointer         OutputPointSetPointer;
  typedef typename Superclass::OutputPointSetType::PixelType OutputPointSetPixelType;
  typedef typename Superclass::PointsContainerType           PointsContainerType;
  typedef typename Superclass::PointDataContainerType        PointDataContainerType;

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(PixelConvertibleToPointSetDataType,
                  (itk::Concept::Convertible<InputPixelType, typename PointDataContainerType::Element>));
#endif


  itkSetMacro(LowerThreshold, InputPixelType);
  itkGetConstReferenceMacro(LowerThreshold, InputPixelType);
  itkSetMacro(UpperThreshold, InputPixelType);
  itkGetConstReferenceMacro(UpperThreshold, InputPixelType);

protected:
  ThresholdImageToPointSetFilter();
  virtual ~ThresholdImageToPointSetFilter() {}

  virtual void ThreadedGenerateData(const InputImageRegionType& inputRegionForThread, itk::ThreadIdType threadId);

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ThresholdImageToPointSetFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  InputPixelType m_LowerThreshold;
  InputPixelType m_UpperThreshold;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbThresholdImageToPointSetFilter.txx"
#endif

#endif
