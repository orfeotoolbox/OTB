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

namespace otb
{

/** \class ThresholdImageToPointSetFilter
 *
 */

template <class TInputImage, 
//          class TOutputPointSet>
         class TOutputPointSet = itk::PointSet<ITK_TYPENAME TInputImage::PixelType,2> >
class ThresholdImageToPointSetFilter :  
           public ImageToPointSetFilter< TInputImage,TOutputPointSet >
{
public:

  itkStaticConstMacro(		InputImageDimension,
  				unsigned int,
                      		TInputImage::ImageDimension);

  typedef TInputImage     InputImageType;
  //typedef TOutputPointSet OutputPointSetType;
  

  typedef ThresholdImageToPointSetFilter                             Self;
  typedef ImageToPointSetFilter< InputImageType, TOutputPointSet> Superclass;
  typedef   typename Superclass::OutputPointSetType             OutputPointSetType;
  typedef itk::SmartPointer<Self>                                    Pointer;
  typedef itk::SmartPointer<const Self>                              ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(ThresholdImageToPointSetFilter, ImageToPointSetFilter);
  
  typedef typename Superclass::InputImagePixelType        InputPixelType;
  typedef typename Superclass::InputImagePointer          InputImagePointer;
  typedef typename Superclass::InputImageConstPointer     InputImageConstPointer;
  typedef typename Superclass::InputImageType::SizeType   SizeType;
  typedef typename Superclass::InputImageType::IndexType  IndexType;

  //typedef   typename Superclass::OutputPointSetType             OutputPointSetType;
  typedef   typename Superclass::OutputPointSetPointer          OutputPointSetPointer;
  typedef   typename Superclass::OutputPointSetType::PixelType  OutputPointSetPixelType;

  itkSetMacro(Threshold,InputPixelType);
  itkGetConstReferenceMacro(Threshold, InputPixelType);

protected:
  ThresholdImageToPointSetFilter();
  virtual ~ThresholdImageToPointSetFilter() {};

  virtual void GenerateData();
  
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ThresholdImageToPointSetFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  InputPixelType m_Threshold;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbThresholdImageToPointSetFilter.txx"
#endif

  
#endif
