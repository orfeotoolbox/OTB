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
#ifndef __otbMinMaxAttributesLabelMapFilter_h
#define __otbMinMaxAttributesLabelMapFilter_h

#include "itkLabelMapFilter.h"
#include "itkSimpleDataObjectDecorator.h"

namespace otb {

/** \class MinMaxAttributesLabelMapFilter
 * \brief Computes the min/max of all attributes of a LabelMap
 *        where LabelObject type is compatible with otb::AttributesMapLabelObject
 *
 */
template<class TInputImage>
class ITK_EXPORT MinMaxAttributesLabelMapFilter :
    public itk::LabelMapFilter<TInputImage, TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef MinMaxAttributesLabelMapFilter               Self;
  typedef itk::LabelMapFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage InputImageType;
  typedef typename InputImageType::Pointer             InputImagePointer;
  typedef typename InputImageType::ConstPointer        InputImageConstPointer;
  typedef typename InputImageType::RegionType          InputImageRegionType;
  typedef typename InputImageType::PixelType           InputImagePixelType;
  typedef typename InputImageType::LabelObjectType     LabelObjectType;
  typedef typename InputImageType::ConstIterator       ConstIteratorType;
  
  typedef typename LabelObjectType::AttributesValueType     AttributesValueType;
  typedef typename LabelObjectType::AttributesMapType       AttributesMapType;
  typedef itk::SimpleDataObjectDecorator<AttributesMapType> AttributesMapObjectType;

  typedef itk::DataObject         DataObjectType;
  typedef DataObjectType::Pointer DataObjectPointerType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(MinMaxAttributesLabelMapFilter,
               LabelMapFilter);


  /** Return the computed Minimum. */
  AttributesMapType GetMinimum() const
  {
    return this->GetMinimumOutput()->Get();
  }
  AttributesMapObjectType* GetMinimumOutput();
  const AttributesMapObjectType* GetMinimumOutput() const;

  /** Return the computed Maximum. */
  AttributesMapType GetMaximum() const
  {
    return this->GetMaximumOutput()->Get();
  }
  AttributesMapObjectType* GetMaximumOutput();
  const AttributesMapObjectType* GetMaximumOutput() const;

  virtual DataObjectPointerType MakeOutput(unsigned int idx);

protected:
  MinMaxAttributesLabelMapFilter();
  ~MinMaxAttributesLabelMapFilter() {};

  virtual void GenerateData();

private:
  MinMaxAttributesLabelMapFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  //typedef typename InputImageType::LabelObjectContainerType   LabelObjectContainerType;
  //typedef typename LabelObjectContainerType::const_iterator   LabelObjectContainerConstIterator;


}; // end of class

} // end namespace otb
  
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMinMaxAttributesLabelMapFilter.txx"
#endif

#endif


