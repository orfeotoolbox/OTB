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

#include "itkLabelMap.h"
#include "itkLabelObject.h"
#include "itkLabelMapFilter.h"
#include "otbAttributesMapLabelObjectWithClassLabel.h"
#include "otbLabelImageToLabelMapWithAdjacencyFilter.h"
#include "otbLabelMapWithAdjacency.h"
#include "otbShapeAttributesLabelMapFilter.h"
#include "otbRadiometricAttributesLabelMapFilter.h"
#include "otbLabelMapWithClassLabelToClassLabelImageFilter.h"


namespace otb
{

template <class TInputImage, class TLabeledImage, class TLabel, class TObjectLabel >
class ITK_EXPORT ImageToLabelMapWithAttributesFilter
  : public itk::LabelMapFilter< LabelMapWithAdjacency<TObjectLabel  >,
                                LabelMapWithAdjacency< TObjectLabel > >
{

public:
  /** Standard class typedefs */
  typedef ImageToLabelMapWithAttributesFilter      Self;
  typedef itk::SmartPointer<Self>                  Pointer;
  typedef itk::SmartPointer<const Self>            ConstPointer;
  typedef itk::LabelMapFilter< LabelMapWithAdjacency<TObjectLabel  >,
                               LabelMapWithAdjacency< TObjectLabel > > Superclass;
  /** Standard type macro */
  itkTypeMacro(ImageToLabelMapWithAttributesFilter,itk::LabelMapFilter);

  /** New macro*/
  itkNewMacro(Self);
  
  typedef TInputImage                 InputImageType;
  typedef TLabeledImage               LabeledImageType;
  typedef TLabel                      LabelType;

  /** Algorithms typedef */
  typedef TObjectLabel      LabelObjectType;
  //typedef AttributesMapLabelObjectWithClassLabel<LabelType,2,double,LabelType> LabelObjectType;
  typedef LabelMapWithAdjacency<LabelObjectType>                               LabelMapType;
  typedef typename LabelMapType::AdjacentLabelsContainerType                   AdjacentLabelsContainerType;
  
  typedef LabelImageToLabelMapWithAdjacencyFilter<LabeledImageType,LabelMapType> LabelMapFilterType;
  typedef ShapeAttributesLabelMapFilter<LabelMapType>                            ShapeLabelMapFilterType;
  typedef RadiometricAttributesLabelMapFilter<LabelMapType,InputImageType>       RadiometricLabelMapFilterType;

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

