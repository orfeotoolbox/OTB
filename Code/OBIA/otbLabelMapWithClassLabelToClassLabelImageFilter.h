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
#ifndef __otbLabelMapWithClassLabelToClassLabelImageFilter_h
#define __otbLabelMapWithClassLabelToClassLabelImageFilter_h

#include "itkLabelMapFilter.h"

namespace otb {

/** \class LabelMapWithClassLabelToClassLabelImageFilter
 * \brief Converts a LabelMap<LabelObjectWithClassLabel> to an image
 *        of class labels
 */
template<class TInputImage, class TOutputImage>
class ITK_EXPORT LabelMapWithClassLabelToClassLabelImageFilter :
    public itk::LabelMapFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef LabelMapWithClassLabelToClassLabelImageFilter Self;
  typedef itk::LabelMapFilter<TInputImage, TOutputImage>
  Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::RegionType      InputImageRegionType;
  typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef typename InputImageType::LabelObjectType LabelObjectType;
  
  typedef typename LabelObjectType::ConstLineIterator  ConstLineIteratorType;
  
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::ConstPointer   OutputImageConstPointer;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;
  typedef typename OutputImageType::PixelType      OutputImagePixelType;
  typedef typename OutputImageType::IndexType      IndexType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(LabelMapWithClassLabelToClassLabelImageFilter,
               ImageToImageFilter);

protected:
  LabelMapWithClassLabelToClassLabelImageFilter();
  ~LabelMapWithClassLabelToClassLabelImageFilter() {};

  virtual void BeforeThreadedGenerateData();

  virtual void ThreadedProcessLabelObject( LabelObjectType * labelObject );

private:
  LabelMapWithClassLabelToClassLabelImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

}; // end of class

} // end namespace otb
  
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelMapWithClassLabelToClassLabelImageFilter.txx"
#endif

#endif


