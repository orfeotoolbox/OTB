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
#ifndef __otbImageToLabelMapWithAttributesFilter_h
#define __otbImageToLabelMapWithAttributesFilter_h

#include "itkShapeLabelMapFilter.h"
#include "otbAttributesMapLabelObjectWithClassLabel.h"
#include "otbLabelImageToLabelMapWithAdjacencyFilter.h"
#include "otbShapeAttributesLabelMapFilter.h"
#include "otbBandsStatisticsAttributesLabelMapFilter.h"
#include "otbLabelMapWithClassLabelToClassLabelImageFilter.h"


namespace otb
{

/** \class ImageToLabelMapWithAttributesFilter
 * \brief Convert a labeled image to a label map with adjacency information
 *        and computes feature attributes for each LabelObject
 *
 * Convert a labeled image and its associated vector image
 * to a label map with adjacency information, and computes shape and
 * radiometric attributes for each LabelObject.
 *
 * It is suitable to be used with otb::LabelMapWithAdjacency<otb::AttributesMapLabelObject>
 */
template <class TInputImage, class TLabeledImage, class TLabel, class TLabelObject >
class ITK_EXPORT ImageToLabelMapWithAttributesFilter
  : public itk::ImageToImageFilter< TInputImage, LabelMapWithAdjacency<TLabelObject> >
{

public:
  /** Standard class typedefs */
  typedef ImageToLabelMapWithAttributesFilter      Self;
  typedef itk::SmartPointer<Self>                  Pointer;
  typedef itk::SmartPointer<const Self>            ConstPointer;
  typedef itk::ImageToImageFilter< TInputImage, LabelMapWithAdjacency<TLabelObject> > Superclass;

  /** Standard type macro */
  itkTypeMacro(ImageToLabelMapWithAttributesFilter, itk::ImageToImageFilter);

  /** New macro*/
  itkNewMacro(Self);

  typedef TInputImage                 InputImageType;
  typedef TLabeledImage               LabeledImageType;
  typedef TLabelObject                LabelObjectType;

  typedef typename LabelObjectType::LabelType                                  LabelType;
  typedef LabelMapWithAdjacency<LabelObjectType>                               LabelMapType;
  typedef typename LabelMapType::AdjacentLabelsContainerType                   AdjacentLabelsContainerType;

  typedef LabelImageToLabelMapWithAdjacencyFilter<LabeledImageType, LabelMapType> LabelMapFilterType;
  typedef ShapeAttributesLabelMapFilter<LabelMapType>                             ShapeLabelMapFilterType;
  typedef BandsStatisticsAttributesLabelMapFilter<LabelMapType, InputImageType>   BandStatisticsLabelMapFilterType;

  virtual void SetInput( const InputImageType *image);
  virtual void SetLabeledImage( const LabeledImageType * image);
  const InputImageType * GetInput(void);
  const LabeledImageType * GetLabeledImage();
  virtual LabelMapType* GetOutput();

  virtual void GenerateData();

protected:
  /** Constructor */
  ImageToLabelMapWithAttributesFilter();
  /** Destructor */
  virtual ~ImageToLabelMapWithAttributesFilter(){};

  void GenerateInputRequestedRegion();


private:
  ImageToLabelMapWithAttributesFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  typename LabelMapType::Pointer   m_Output;

};
}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToLabelMapWithAttributesFilter.txx"
#endif
#endif

