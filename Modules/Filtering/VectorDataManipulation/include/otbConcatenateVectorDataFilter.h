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
#ifndef __otbConcatenateVectorDataFilter_h
#define __otbConcatenateVectorDataFilter_h

#include "otbVectorDataToVectorDataFilter.h"
#include "otbVectorData.h"
#include "itkPreOrderTreeIterator.h"

namespace otb
{
/** \class ConcatenateVectorDataFilter
 *  \brief Produces a VectorData from the concatenation
 *  of several input vectordatas.
 *
 *  This generator produces a unique vector by concatening several
 *  inputs set via the method AddInput(VectorData *).
 *
 * Note that the input vectordatas must have the same node type,
 * this is due that vectordata creation does not support multiple geometries
 * in a single vectordata.
 * Warning : this filter does not create a deep copy of each node.
 *
 */
template <class TVectorData>
class ITK_EXPORT ConcatenateVectorDataFilter :
    public VectorDataSource<TVectorData>
{
public:
  /** Standard class typedefs */
  typedef ConcatenateVectorDataFilter                  Self;
  typedef VectorDataSource<TVectorData >               Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ConcatenateVectorDataFilter,
               VectorDataSource);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef TVectorData                                           VectorDataType;
  typedef typename VectorDataType::Pointer                      VectorDataPointerType;
  typedef typename VectorDataType::DataNodeType                 DataNodeType;
  typedef typename DataNodeType::Pointer                        DataNodePointerType;
  typedef typename DataNodeType::PolygonType::RegionType        RegionType;
  typedef typename DataNodeType::PointType                      PointType;
  typedef typename DataNodeType::PolygonType::VertexType        VertexType;
  typedef typename DataNodeType::LineType                       LineType;
  typedef typename VectorDataType::DataTreeType                 DataTreeType;
  typedef typename DataTreeType::TreeNodeType                   TreeNodeType;
  typedef typename TreeNodeType::ChildrenListType               ChildrenListType;

  typedef typename std::vector<PointType>                       PointVectorType;

  /** Connects the VectorDatas from which the localizations are going to be extracted. */
  void AddInput(const VectorDataType *);

  const VectorDataType * GetInput(unsigned int idx) const;

protected:
  ConcatenateVectorDataFilter();
  virtual ~ConcatenateVectorDataFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Triggers the Computation of the sample list */
  void GenerateData(void);

  /** Recursive method to visit efficiently the vectordata*/
  void ProcessNode(TreeNodeType * source, DataNodeType * outputDocument);

private:
  ConcatenateVectorDataFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  DataNodePointerType       m_Folder;
  DataNodePointerType       m_Document;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbConcatenateVectorDataFilter.txx"
#endif

#endif
