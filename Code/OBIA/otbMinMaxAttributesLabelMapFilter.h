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

#include <vector>
#include "itkLabelMapFilter.h"
#include "itkSimpleDataObjectDecorator.h"

namespace otb {

/** \class MinMaxAttributesLabelMapFilter
 * \brief Computes the min/max of all attributes of a itk::LabelMap<otb::AttributesMapLabelObject>
 */
template<class TInputImage, class TOutputImage>
class ITK_EXPORT MinMaxAttributesLabelMapFilter :
    public itk::LabelMapFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef MinMaxAttributesLabelMapFilter Self;
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
  
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::ConstPointer   OutputImageConstPointer;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;
  typedef typename OutputImageType::PixelType      OutputImagePixelType;
  typedef typename OutputImageType::IndexType      IndexType;
  
  typedef typename LabelObjectType::AttributesValueType             AttributesValueType;
  typedef std::vector<AttributesValueType>                          AttributesValueVectorType;
  typedef itk::SimpleDataObjectDecorator<AttributesValueVectorType> AttributesValueVectorObjectType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(MinMaxAttributesLabelMapFilter,
               LabelMapFilter);


  /** Return the computed Minimum. */
  AttributesValueVectorType GetMinimum() const
  {
    return this->GetMinimumOutput()->Get();
  }
  AttributesValueVectorObjectType* GetMinimumOutput();
  const AttributesValueVectorObjectType* GetMinimumOutput() const;

  /** Return the computed Maximum. */
  AttributesValueVectorType GetMaximum() const
  {
    return this->GetMaximumOutput()->Get();
  }
  AttributesValueVectorObjectType* GetMaximumOutput();
  const AttributesValueVectorObjectType* GetMaximumOutput() const;


protected:
  MinMaxAttributesLabelMapFilter();
  ~MinMaxAttributesLabelMapFilter() {};

  virtual void GenerateData();

private:
  MinMaxAttributesLabelMapFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  typedef typename InputImageType::LabelObjectContainerType   LabelObjectContainerType;
  typedef typename LabelObjectContainerType::const_iterator   LabelObjectContainerConstIterator;


}; // end of class

} // end namespace otb
  
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMinMaxAttributesLabelMapFilter.txx"
#endif

#endif


