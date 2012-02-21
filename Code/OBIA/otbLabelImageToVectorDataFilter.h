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
#ifndef __otbLabelImageToVectorDataFilter_h
#define __otbLabelImageToVectorDataFilter_h

#include "otbVectorDataSource.h"
#include "otbVectorData.h"

namespace otb
{
/** \class LabelImageToVectorDataFilter
 *  \brief this class uses GDALPolygonize method to transform a Label image into
 *         a VectorData. It is a non-streamed version.
 *
 *
 *  \sa StreamingLabelImageToVectorDataFilter (streamed version)
 *  \ingroup OBIA
 *
 *
 */

template <class TInputImage, class TPrecision = double>
class ITK_EXPORT LabelImageToVectorDataFilter :
    public VectorDataSource< otb::VectorData<TPrecision> >
{
public:

   /** typedef for the classes standards. */
  typedef LabelImageToVectorDataFilter                    Self;
  typedef VectorDataSource< VectorData<TPrecision> >      Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for management of the object factory. */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(LineSegmentDetector, VectorDataSource);

  /** Definition of the input image */
  typedef TInputImage                           InputImageType;
  typedef typename InputImageType::PixelType    InputPixelType;
  typedef typename InputImageType::IndexType    InputIndexType;
  typedef typename InputImageType::SizeType     SizeType;
  typedef typename InputImageType::RegionType   RegionType;
  typedef typename InputImageType::SpacingType  SpacingType;
  typedef typename InputImageType::PointType    OriginType;

  /** Definition of the output vector data. */
  typedef VectorData<TPrecision>                 VectorDataType;
  typedef typename VectorDataType::Pointer       VectorDataPointerType;
  typedef typename VectorDataType::DataTreeType  DataTreeType;
  typedef typename DataTreeType::Pointer         DataTreePointerType;
  typedef typename DataTreeType::TreeNodeType    InternalTreeNodeType;
  typedef typename VectorDataType::DataNodeType  DataNodeType;
  typedef typename DataNodeType::Pointer         DataNodePointerType;
  typedef typename VectorDataType::LineType      LineType;
  typedef typename VectorDataType::PointType     PointType;
  typedef typename LineType::VertexType          VertexType;
  
  
  /** Set/Get the input image of this process object.  */
  virtual void SetInput(const InputImageType *input);
  virtual const InputImageType * GetInput(void);
  
protected:
  LabelImageToVectorDataFilter();
  virtual ~LabelImageToVectorDataFilter() {}

  virtual void GenerateInputRequestedRegion();

  /** Generate Data method*/
  virtual void GenerateData();
  
private:
  LabelImageToVectorDataFilter(const Self &);  //purposely not implemented
  void operator =(const Self&);      //purposely not implemented

};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelImageToVectorDataFilter.txx"
#endif

#endif
