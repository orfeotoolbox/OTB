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
#ifndef __otbDBOverlapDataNodeFeatureFunction_h
#define __otbDBOverlapDataNodeFeatureFunction_h

#include "otbDataNodeVectorDataFunction.h"

#include "otbDataNode.h"
#include "itkPreOrderTreeIterator.h"

namespace otb
{
/** \class DBOverlapDataNodeFeatureFunction
  * \brief
  *
  *
  * \ingroup Functions
  * \sa DataNodeVectorDataFunction
  */

template <
class TCoordRep = double,
class TPrecision = double
>
class ITK_EXPORT DBOverlapDataNodeFeatureFunction :
    public DataNodeVectorDataFunction<std::vector<TPrecision>, TCoordRep, TPrecision>
{
public:
  /** Standard class typedefs. */
  typedef DBOverlapDataNodeFeatureFunction                    Self;
  typedef DataNodeVectorDataFunction<std::vector<TPrecision>, TCoordRep, TPrecision>
                                                              Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DBOverlapDataNodeFeatureFunction, DataNodeVectorDataFunction);

  /** Some typedefs. */
  typedef typename Superclass::DataNodeType           DataNodeType;
  typedef typename DataNodeType::PointType            PointType;
  typedef typename DataNodeType::LineType             LineType;
  typedef typename LineType::VertexType               VertexType;
  typedef typename DataNodeType::PolygonType          PolygonType;

  typedef typename Superclass::VectorDataType         VectorDataType;
  typedef typename VectorDataType::DataTreeType       DataTreeType;
  typedef itk::PreOrderTreeIterator<typename VectorDataType::DataTreeType>
                                                      TreeIteratorType;

  typedef TCoordRep                                   CoordRepType;

  typedef TPrecision                                  PrecisionType;

  typedef std::vector<PrecisionType>                  OutputType;

  virtual OutputType Evaluate( const DataNodeType& node ) const;

  /** Set/Get methods */
  itkGetConstMacro(DistanceThreshold, PrecisionType);
  itkSetMacro(DistanceThreshold, PrecisionType);

  /** Method to compute the distance of a point to a segment */
  double ComputeEuclideanDistanceMetricToSegment(VertexType q1, VertexType q2, VertexType p) const;

protected:
  DBOverlapDataNodeFeatureFunction();
  ~DBOverlapDataNodeFeatureFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  DBOverlapDataNodeFeatureFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Road / Building distance Threshold */
  PrecisionType               m_DistanceThreshold;
};

}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDBOverlapDataNodeFeatureFunction.txx"
#endif

#endif
