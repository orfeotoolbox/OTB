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
#ifndef __otbVectorDataReProjectionFilter_h
#define __otbVectorDataReProjectionFilter_h

#include "otbVectorDataToVectorDataFilter.h"

#include "otbImage.h"

#include "otbVectorDataProjectionFilter.h"
#include "otbVectorDataExtractROI.h"
/*
#include "itkTransform.h"
#include "otbGenericRSTransform.h"
#include "otbImageKeywordlist.h"*/

namespace otb
{

/** \class VectorDataReProjectionFilter
  * \brief Reproject vector data in the coordinate system of an image
  *
  *
  * \ingroup VectorDataFilter
  * \ingroup Projection
  *
  *
  */

template <class TInputVectorData, class TInputImage>
class ITK_EXPORT VectorDataReProjectionFilter :
  public otb::VectorDataToVectorDataFilter<TInputVectorData, TInputVectorData>
{

public:
  /** Standard class typedefs. */
  typedef VectorDataReProjectionFilter                                           Self;
  typedef otb::VectorDataToVectorDataFilter<TInputVectorData, TInputVectorData>  Superclass;
  typedef itk::SmartPointer<Self>                                                Pointer;
  typedef itk::SmartPointer<const Self>                                          ConstPointer;

  /** VectorData typedefs. */
  typedef TInputVectorData                        InputVectorDataType;
  typedef typename TInputVectorData::ConstPointer InputVectorDataPointer;

  /** Image typedefs. */
  typedef TInputImage                                          ImageType;
  typedef typename TInputImage::Pointer                        ImagePointerType;



  // use to display vectordata
  typedef itk::PreOrderTreeIterator<typename InputVectorDataType::DataTreeType> TreeIteratorType;


  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataReProjectionFilter, VectorDataToVectorDataFilter);

  itkSetStringMacro(DEMDirectory);
  itkGetStringMacro(DEMDirectory);

  void SetInputVectorData(InputVectorDataPointer input)
  {
    this->SetInput(input);
  }

  void SetInputImage(ImagePointerType input)
  {
    m_InputImage = input;
  }

protected:
  VectorDataReProjectionFilter();

  virtual ~VectorDataReProjectionFilter() {}

  void GenerateData(void);

  // Projection filter
  typedef VectorDataProjectionFilter<InputVectorDataType, InputVectorDataType>
                                                                VectorDataProjectionFilterType;
  typedef VectorDataExtractROI<InputVectorDataType>             VectorDataExtractROIType;
  typedef typename VectorDataExtractROIType::RegionType         RemoteSensingRegionType;

private:
  VectorDataReProjectionFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  std::string m_DEMDirectory;

  ImagePointerType m_InputImage;

  typename VectorDataProjectionFilterType::Pointer m_VdProjFilter;
  typename VectorDataExtractROIType::Pointer       m_VdExtractFilter;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataReProjectionFilter.txx"
#endif

#endif
