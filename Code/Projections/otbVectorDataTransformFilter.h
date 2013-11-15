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
#ifndef __otbVectorDataTransformFilter_h
#define __otbVectorDataTransformFilter_h

#include "otbVectorDataToVectorDataFilter.h"
#include "itkTransform.h"
#include "otbImageKeywordlist.h"
#include "otbVectorData.h"

namespace otb
{

/** \class VectorDataTransformFilter
  * \brief Apply a Transform To a VectorData
  *
  * This class is used to reproject vector data using a itk::Transform
  * the input and output coordinate system are the same
  *
  * \ingroup VectorDataFilter
  * \ingroup Projection
  */

template <class TInputVectorData, class TOutputVectorData>
class ITK_EXPORT VectorDataTransformFilter :
      public otb::VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData>
{

public:
  /** Standard class typedefs. */
  typedef VectorDataTransformFilter         Self;
  typedef otb::VectorDataToVectorDataFilter
  <TInputVectorData, TOutputVectorData>      Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  typedef TInputVectorData                        InputVectorDataType;
  typedef TOutputVectorData                       OutputVectorDataType;
  typedef typename TInputVectorData::ConstPointer InputVectorDataPointer;
  typedef typename TOutputVectorData::Pointer     OutputVectorDataPointer;

  /** Some typedefs. */
  typedef itk::Transform<double, 2, 2>           GenericTransformType;
  typedef typename GenericTransformType::Pointer GenericTransformPointerType;

  typedef itk::Vector<double, 2> SpacingType;
  typedef itk::Point<double, 2>  OriginType;

  typedef typename InputVectorDataType::DataNodePointerType  InputDataNodePointerType;
  typedef typename OutputVectorDataType::DataNodeType        OutputDataNodeType;
  typedef typename OutputVectorDataType::DataNodePointerType OutputDataNodePointerType;
  typedef typename OutputVectorDataType::DataTreePointerType OutputDataTreePointerType;

  typedef typename InputVectorDataType::DataTreeType::TreeNodeType    InputInternalTreeNodeType;
  typedef typename OutputVectorDataType::DataTreeType::TreeNodeType   OutputInternalTreeNodeType;
  typedef typename InputInternalTreeNodeType::ChildrenListType        InputChildrenListType;


  typedef typename OutputDataNodeType::PointType            PointType;
  typedef typename OutputDataNodeType::LineType             LineType;
  typedef typename OutputDataNodeType::LineConstPointerType LineConstPointerType;
  typedef typename OutputDataNodeType::LinePointerType      LinePointerType;

  typedef typename OutputDataNodeType::PolygonType             PolygonType;
  typedef typename OutputDataNodeType::PolygonConstPointerType PolygonConstPointerType;
  typedef typename OutputDataNodeType::PolygonPointerType      PolygonPointerType;

  typedef typename OutputDataNodeType::PolygonListType             PolygonListType;
  typedef typename OutputDataNodeType::PolygonListConstPointerType PolygonListConstPointerType;
  typedef typename OutputDataNodeType::PolygonListPointerType      PolygonListPointerType;


  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataTransformFilter, VectorDataToVectorDataFilter);

  /** Set The Transform applied to VectorData*/
  itkSetObjectMacro(Transform, GenericTransformType);
  itkGetObjectMacro(Transform, GenericTransformType);

protected:
  VectorDataTransformFilter();
  virtual ~VectorDataTransformFilter() {};

  PointType ProcessPoint(PointType point) const;
  LinePointerType ProcessLine(LinePointerType line) const;
  PolygonPointerType ProcessPolygon(PolygonPointerType polygon) const;
  PolygonListPointerType ProcessPolygonList(PolygonListPointerType polygonList) const;

  void GenerateData(void);

private:
  VectorDataTransformFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  GenericTransformPointerType m_Transform;
};

} // end namespace otb

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbVectorDataTransformFilter.txx"
#endif

#endif
