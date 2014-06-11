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
#ifndef __otbVectorDataToLabelMapWithAttributesFilter_txx
#define __otbVectorDataToLabelMapWithAttributesFilter_txx

#include "itkBinaryImageToLabelMapFilter.h"
#include "itkNumericTraits.h"

#include "otbVectorDataProperties.h"

namespace otb
{

template <class TVectorData, class TLabelMap>
VectorDataToLabelMapWithAttributesFilter<TVectorData, TLabelMap>
::VectorDataToLabelMapWithAttributesFilter()
{
  m_BackgroundValue = itk::NumericTraits<OutputLabelMapPixelType>::max();
  this->SetNumberOfRequiredInputs(1);
  m_Spacing.Fill(1.0);
  m_Origin.Fill(0.0);
  m_Direction.SetIdentity();
  m_Size.Fill(0);
  m_StartIndex.Fill(0);
  m_InitialLabel = itk::NumericTraits<LabelType>::Zero;

  m_AutomaticSizeComputation=true;
  m_VectorDataProperties = VectorDataPropertiesType::New();
}

//----------------------------------------------------------------------------
template <class TVectorData, class TLabelMap>
void
VectorDataToLabelMapWithAttributesFilter<TVectorData, TLabelMap>
::SetSpacing(const SpacingType& spacing)
{
  if (this->m_Spacing != spacing)
    {
    this->m_Spacing = spacing;
    this->Modified();
    }
}

//----------------------------------------------------------------------------
template <class TVectorData, class TLabelMap>
void
VectorDataToLabelMapWithAttributesFilter<TVectorData, TLabelMap>
::SetSpacing(const double spacing[2])
{
  SpacingType s(spacing);
  this->SetSpacing(s);
}

//----------------------------------------------------------------------------
template <class TVectorData, class TLabelMap>
void
VectorDataToLabelMapWithAttributesFilter<TVectorData, TLabelMap>
::SetSpacing(const float spacing[2])
{
  itk::Vector<float, 2> sf(spacing);
  SpacingType s;
  s.CastFrom(sf);
  this->SetSpacing(s);
}

//----------------------------------------------------------------------------
template <class TVectorData, class TLabelMap>
void
VectorDataToLabelMapWithAttributesFilter<TVectorData, TLabelMap>
::SetOrigin(const double origin[2])
{
  OriginType p(origin);
  this->SetOrigin(p);
}

//----------------------------------------------------------------------------
template <class TVectorData, class TLabelMap>
void
VectorDataToLabelMapWithAttributesFilter<TVectorData, TLabelMap>
::SetOrigin(const float origin[2])
{
  itk::Point<float, 2> of(origin);
  OriginType p;
  p.CastFrom(of);
  this->SetOrigin(p);
}


/**
   * overloaded because pb during copyinformation
 */

template <class TVectorData, class TLabelMap>
void VectorDataToLabelMapWithAttributesFilter<TVectorData, TLabelMap>
::GenerateOutputInformation()
{
  // we can't call the superclass method here.

  // get pointers to the input and output
  OutputLabelMapType * outputPtr = this->GetOutput();

  if (!outputPtr)
    {
    return;
    }

  RegionType outputLargestPossibleRegion;
  if(m_AutomaticSizeComputation==false)
  {
     // Set the size of the output region
     outputLargestPossibleRegion.SetSize(m_Size);
     outputLargestPossibleRegion.SetIndex(m_StartIndex);
     outputPtr->SetLargestPossibleRegion(outputLargestPossibleRegion);

     otbGenericMsgDebugMacro(<< "LargestPossibleRegion " << outputPtr->GetLargestPossibleRegion());

     // Set spacing and origin
     outputPtr->SetSpacing(m_Spacing);
     outputPtr->SetOrigin(m_Origin);
     outputPtr->SetDirection(m_Direction);
  }
  else
    {
    //typename PolygonType::RegionType  region;
    m_VectorDataProperties->SetVectorDataObject(this->GetInput());
    //m_VectorDataProperties->SetBoundingRegion(region);
    //Compute the global bounding box of the vectordata
    m_VectorDataProperties->ComputeBoundingRegion();
    
    // Compute origin and size
    SizeType size;
    SpacingType spacing = this->GetInput()->GetSpacing();
    OriginType origin = m_VectorDataProperties->GetBoundingRegion().GetOrigin();
    for (unsigned int i=0; i<2; ++i)
      {
      if (spacing[i] < 0.0)
        {
        origin[i] += m_VectorDataProperties->GetBoundingRegion().GetSize(i);
        }
      origin[i] += (0.5 - m_StartIndex[i]) * spacing[i];
      size[i] = static_cast<unsigned long>(vcl_ceil(vcl_abs(
        m_VectorDataProperties->GetBoundingRegion().GetSize(i)/spacing[i])));
      }
    
    outputLargestPossibleRegion.SetSize(size);
    outputLargestPossibleRegion.SetIndex(m_StartIndex);

    outputPtr->SetLargestPossibleRegion(outputLargestPossibleRegion);
    outputPtr->SetSpacing(spacing);
    outputPtr->SetOrigin(origin);
    outputPtr->SetDirection(m_Direction);
    }
  return;
}
/*
template <class TVectorData, class TLabelMap >
void
    VectorDataToLabelMapWithAttributesFilter<TVectorData, TLabelMap >
::GenerateInputRequestedRegion()
{

  //call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // We need all the input.
  InputVectorDataPointer input = const_cast<InputVectorDataType *>(this->GetInput());
  if( !input )
  {
    return;
  }
  input->SetRequestedRegionToLargestPossibleRegion ();
}


template <class TVectorData, class TLabelMap >
void
VectorDataToLabelMapWithAttributesFilter<TVectorData, TLabelMap >
::EnlargeOutputRequestedRegion(itk::DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}
*/

template<class TVectorData, class TLabelMap>
void
VectorDataToLabelMapWithAttributesFilter<TVectorData, TLabelMap>
::SetInput(const InputVectorDataType *input)
{
// Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<InputVectorDataType *>(input));
}

template<class TVectorData, class TLabelMap>
void
VectorDataToLabelMapWithAttributesFilter<TVectorData, TLabelMap>
::SetInput(unsigned int idx, const InputVectorDataType *input)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(idx,
                                        const_cast<InputVectorDataType *>(input));
}

template<class TVectorData, class TLabelMap>
const typename VectorDataToLabelMapWithAttributesFilter<TVectorData, TLabelMap>::InputVectorDataType *
VectorDataToLabelMapWithAttributesFilter<TVectorData, TLabelMap>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const TVectorData *>
           (this->itk::ProcessObject::GetInput(0));
}

template<class TVectorData, class TLabelMap>
const typename VectorDataToLabelMapWithAttributesFilter<TVectorData, TLabelMap>::InputVectorDataType *
VectorDataToLabelMapWithAttributesFilter<TVectorData, TLabelMap>
::GetInput(unsigned int idx)
{
  return static_cast<const TVectorData *>
           (this->itk::ProcessObject::GetInput(idx));
}

template <class TVectorData, class TLabelMap>
void
VectorDataToLabelMapWithAttributesFilter<TVectorData, TLabelMap>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  OutputLabelMapType * output = this->GetOutput();

  //For each input
  for (unsigned int idx = 0; idx < this->GetNumberOfInputs(); ++idx)
    {
    if (this->GetInput(idx))
      {

      InputVectorDataConstPointer input = this->GetInput(idx);
      InternalTreeNodeType *      inputRoot = const_cast<InternalTreeNodeType *>(input->GetDataTree()->GetRoot());
      //Use our own value for the background
      output->SetBackgroundValue(m_BackgroundValue);
      //Set the value of the first label
      m_lab = m_InitialLabel;
//       otbGenericMsgDebugMacro(<<"input " <<  idx);

      //The projection information
      output->SetMetaDataDictionary(input->GetMetaDataDictionary());
      ProcessNode(inputRoot);

      }
    }
}

template<class TVectorData, class TLabelMap>
void
VectorDataToLabelMapWithAttributesFilter<TVectorData, TLabelMap>
::ProcessNode(InternalTreeNodeType * source)
{

  // Get the children list from the input node
  ChildrenListType children = source->GetChildrenList();

  // For each child
  for (typename ChildrenListType::iterator it = children.begin(); it != children.end(); ++it)
    {
    // Copy input DataNode info
    DataNodePointerType dataNode = (*it)->Get();
    otbGenericMsgDebugMacro(<< "Type of node " << dataNode->GetNodeType() << " id" << dataNode->GetNodeId());
    switch (dataNode->GetNodeType())
      {
      case otb::ROOT:
        {
        ProcessNode((*it));
        break;
        }
      case otb::DOCUMENT:
        {
        ProcessNode((*it));
        break;
        }
      case otb::FOLDER:
        {
        ProcessNode((*it));
        break;
        }
      case FEATURE_POINT:
        {
        otbGenericMsgDebugMacro(<< "Insert Point from vectorData");
        IndexType index;
        this->GetOutput()->TransformPhysicalPointToIndex(dataNode->GetPoint(), index);
        if (this->GetOutput()->GetLargestPossibleRegion().IsInside(index))
          {
          this->GetOutput()->SetPixel(index, m_lab);
          m_lab += 1;
          }
        break;
        }
      case otb::FEATURE_LINE:
        {
        //TODO Bresenham
        itkExceptionMacro(
          << "This type (FEATURE_LINE) is not handle (yet) by VectorDataToLabelMapWithAttributesFilter(), please request for it");
        break;
        }
      case FEATURE_POLYGON:
        {

        /** correct polygon exterior ring (simplify and close the pokygon)*/
        CorrectFunctorType correct;
        PolygonPointerType correctPolygonExtRing = correct(dataNode->GetPolygonExteriorRing());

        //typedef typename DataNodeType::PolygonType PolygonType;
        typedef typename PolygonType::RegionType   RSRegionType;
        typedef typename PolygonType::VertexType   VertexType;
        typedef typename IndexType::IndexValueType IndexValueType;
        typedef typename VertexType::ValueType     VertexValueType;
        RSRegionType polygonExtRingBoundReg = correctPolygonExtRing->GetBoundingRegion();
        
        IndexType startIdx,endIdx,tmpIdx;
        OriginType physCorners[4];
        physCorners[0][0] = polygonExtRingBoundReg.GetOrigin(0);
        physCorners[0][1] = polygonExtRingBoundReg.GetOrigin(1);
        physCorners[1] = physCorners[0];
        physCorners[2] = physCorners[0];
        physCorners[3] = physCorners[0];
        
        physCorners[1][1] += polygonExtRingBoundReg.GetSize(1);
        physCorners[2][1] += polygonExtRingBoundReg.GetSize(1);
        physCorners[2][0] += polygonExtRingBoundReg.GetSize(0);
        physCorners[3][0] += polygonExtRingBoundReg.GetSize(0);
        
        for (unsigned int k=0; k<4; ++k)
          {
          this->GetOutput()->TransformPhysicalPointToIndex(physCorners[k],tmpIdx);
          if (k == 0)
            {
            startIdx = tmpIdx;
            endIdx = tmpIdx;
            }
          else
            {
            startIdx[0] = std::min(startIdx[0],tmpIdx[0]);
            startIdx[1] = std::min(startIdx[1],tmpIdx[1]);
            endIdx[0] = std::max(endIdx[0],tmpIdx[0]);
            endIdx[1] = std::max(endIdx[1],tmpIdx[1]);
            }
          }
        // Check that the polygon intersects the largest possible region
        RegionType polyRegion;
        polyRegion.SetIndex(startIdx);
        polyRegion.SetSize(0,endIdx[0] - startIdx[0] + 1);
        polyRegion.SetSize(1,endIdx[1] - startIdx[1] + 1);
        if (polyRegion.Crop(this->GetOutput()->GetLargestPossibleRegion()))
          {
          startIdx = polyRegion.GetIndex();
          endIdx[0] = startIdx[0] - 1 + polyRegion.GetSize(0);
          endIdx[1] = startIdx[1] - 1 + polyRegion.GetSize(1);
          }
        else
          {
          // No intersection
          break;
          }
        
        OriginType tmpPoint;
        VertexType vertex;
        for (IndexValueType j=startIdx[1]; j<=endIdx[1]; ++j)
          {
          for (IndexValueType i=startIdx[0]; i<=endIdx[0]; ++i)
            {
            tmpIdx[0] = i;
            tmpIdx[1] = j;
            this->GetOutput()->TransformIndexToPhysicalPoint(tmpIdx,tmpPoint);
            vertex[0] = tmpPoint[0];
            vertex[1] = tmpPoint[1];
            if (correctPolygonExtRing->IsInside(vertex) || correctPolygonExtRing->IsOnEdge (vertex))
              {
              // TODO : should also test interior rings
              if (this->GetOutput()->HasLabel(m_lab))
                {
                if (!this->GetOutput()->GetLabelObject(m_lab)->HasIndex(tmpIdx))
                  { //Add a pixel to the current labelObject
                  this->GetOutput()->SetPixel(tmpIdx, m_lab);
                  }
                }
              else
                {
                //Add a pixel to the current labelObject
                this->GetOutput()->SetPixel(tmpIdx, m_lab);
                //add attributes
                AttributesValueType fieldValue;
                for(unsigned int ii=0; ii<dataNode->GetFieldList().size(); ii++)
                  {
                  fieldValue = static_cast<AttributesValueType>( dataNode->GetFieldAsString(dataNode->GetFieldList()[ii]) );
                  this->GetOutput()->GetLabelObject(m_lab)->SetAttribute( dataNode->GetFieldList()[ii].c_str(), fieldValue );
                  }
                }
              }
            }
          }
        //Modify the label for the next layer
        m_lab += 1;
        break;
        }
      case FEATURE_MULTIPOINT:
        {
        itkExceptionMacro(
          <<
          "This type (FEATURE_MULTIPOINT) is not handle (yet) by VectorDataToLabelMapWithAttributesFilter(), please request for it");
        break;
        }
      case FEATURE_MULTILINE:
        {
        itkExceptionMacro(
          << "This type (FEATURE_MULTILINE) is not handle (yet) by VectorDataToLabelMapWithAttributesFilter(), please request for it");
        break;
        }
      case FEATURE_MULTIPOLYGON:
        {
        itkExceptionMacro(
          <<
          "This type (FEATURE_MULTIPOLYGON) is not handle (yet) by VectorDataToLabelMapWithAttributesFilter(), please request for it");
        break;
        }
      case FEATURE_COLLECTION:
        {
        itkExceptionMacro(
          <<
          "This type (FEATURE_COLLECTION) is not handle (yet) by VectorDataToLabelMapWithAttributesFilter(), please request for it");
        break;
        }
      }
    }
}

template <class TVectorData, class TLabelMap>
void
VectorDataToLabelMapWithAttributesFilter<TVectorData, TLabelMap>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "BackgroundValue: "  << static_cast<typename itk::NumericTraits<OutputLabelMapPixelType>::PrintType>(
    m_BackgroundValue) << std::endl;
}

} // end namespace otb

#endif
