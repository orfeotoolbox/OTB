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
#ifndef otbVectorDataToLabelMapFilter_txx
#define otbVectorDataToLabelMapFilter_txx

#include "itkBinaryImageToLabelMapFilter.h"
#include "itkNumericTraits.h"

#include "otbVectorDataProperties.h"

namespace otb
{

template <class TVectorData, class TLabelMap>
VectorDataToLabelMapFilter<TVectorData, TLabelMap>
::VectorDataToLabelMapFilter()
{
  m_BackgroundValue = itk::NumericTraits<OutputLabelMapPixelType>::max();
  this->SetNumberOfRequiredInputs(1);
  m_Spacing.Fill(1.0);
  m_Origin.Fill(0.0);
  m_Direction.SetIdentity();
  m_Size.Fill(0);
  m_StartIndex.Fill(0);
  m_lab = itk::NumericTraits<LabelType>::Zero;
}

//----------------------------------------------------------------------------
template <class TVectorData, class TLabelMap>
void
VectorDataToLabelMapFilter<TVectorData, TLabelMap>
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
VectorDataToLabelMapFilter<TVectorData, TLabelMap>
::SetSpacing(const double spacing[2])
{
  SpacingType s(spacing);
  this->SetSpacing(s);
}

//----------------------------------------------------------------------------
template <class TVectorData, class TLabelMap>
void
VectorDataToLabelMapFilter<TVectorData, TLabelMap>
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
VectorDataToLabelMapFilter<TVectorData, TLabelMap>
::SetOrigin(const double origin[2])
{
  OriginType p(origin);
  this->SetOrigin(p);
}

//----------------------------------------------------------------------------
template <class TVectorData, class TLabelMap>
void
VectorDataToLabelMapFilter<TVectorData, TLabelMap>
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
void VectorDataToLabelMapFilter<TVectorData, TLabelMap>
::GenerateOutputInformation()
{
  // we can't call the superclass method here.

  // get pointers to the input and output
  OutputLabelMapType * outputPtr = this->GetOutput();

  if (!outputPtr)
    {
    return;
    }

  // Set the size of the output region
  typename OutputLabelMapType::RegionType outputLargestPossibleRegion;
  outputLargestPossibleRegion.SetSize(m_Size);
  outputLargestPossibleRegion.SetIndex(m_StartIndex);
  outputPtr->SetLargestPossibleRegion(outputLargestPossibleRegion);

  otbGenericMsgDebugMacro(<< "LargestPossibleRegion " << outputPtr->GetLargestPossibleRegion());

  // Set spacing and origin
  outputPtr->SetSpacing(m_Spacing);
  outputPtr->SetOrigin(m_Origin);
  outputPtr->SetDirection(m_Direction);

  return;
}
/*
template <class TVectorData, class TLabelMap >
void
    VectorDataToLabelMapFilter<TVectorData, TLabelMap >
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
VectorDataToLabelMapFilter<TVectorData, TLabelMap >
::EnlargeOutputRequestedRegion(itk::DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}
*/

template<class TVectorData, class TLabelMap>
void
VectorDataToLabelMapFilter<TVectorData, TLabelMap>
::SetInput(const InputVectorDataType *input)
{
// Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<InputVectorDataType *>(input));
}

template<class TVectorData, class TLabelMap>
void
VectorDataToLabelMapFilter<TVectorData, TLabelMap>
::SetInput(unsigned int idx, const InputVectorDataType *input)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(idx,
                                        const_cast<InputVectorDataType *>(input));
}

template<class TVectorData, class TLabelMap>
const typename VectorDataToLabelMapFilter<TVectorData, TLabelMap>::InputVectorDataType *
VectorDataToLabelMapFilter<TVectorData, TLabelMap>
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
const typename VectorDataToLabelMapFilter<TVectorData, TLabelMap>::InputVectorDataType *
VectorDataToLabelMapFilter<TVectorData, TLabelMap>
::GetInput(unsigned int idx)
{
  return static_cast<const TVectorData *>
           (this->itk::ProcessObject::GetInput(idx));
}

template <class TVectorData, class TLabelMap>
void
VectorDataToLabelMapFilter<TVectorData, TLabelMap>
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
      output->SetBackgroundValue(itk::NumericTraits<OutputLabelMapPixelType>::max());
      //Set the value of the first label
      m_lab = itk::NumericTraits<LabelType>::Zero;
//       otbGenericMsgDebugMacro(<<"input " <<  idx);

      //The projection information
      output->SetMetaDataDictionary(input->GetMetaDataDictionary());
      ProcessNode(inputRoot);

      }
    }
}

template<class TVectorData, class TLabelMap>
void
VectorDataToLabelMapFilter<TVectorData, TLabelMap>
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

        this->GetOutput()->SetPixel(index, m_lab);
        m_lab += 10;
        break;
        }
      case otb::FEATURE_LINE:
        {
        //TODO Bresenham
        itkExceptionMacro(
          << "This type (FEATURE_LINE) is not handle (yet) by VectorDataToLabelMapFilter(), please request for it");
        break;
        }
      case FEATURE_POLYGON:
        {

        /** correct polygon exterior ring (simplify and close the pokygon)*/
        CorrectFunctorType correct;
        PolygonPointerType correctPolygonExtRing = correct(dataNode->GetPolygonExteriorRing());

        typedef typename DataNodeType::PolygonType PolygonType;
        typedef typename PolygonType::RegionType   RegionType;
        typedef typename PolygonType::VertexType   VertexType;
        typedef typename IndexType::IndexValueType IndexValueType;
        typedef typename VertexType::ValueType     VertexValueType;
        RegionType polygonExtRingBoundReg = correctPolygonExtRing->GetBoundingRegion();

        VertexType vertex;
        otbMsgDevMacro( "Polygon bounding region " << polygonExtRingBoundReg);
        otbMsgDevMacro( "output origin " << this->GetOutput()->GetOrigin());
        otbMsgDevMacro( "spacing " << this->GetOutput()->GetSpacing());
        // For each position in the bounding region of the polygon

        for (double i = polygonExtRingBoundReg.GetOrigin(0);
             i < polygonExtRingBoundReg.GetOrigin(0) + polygonExtRingBoundReg.GetSize(0);
             i += this->GetOutput()->GetSpacing()[0])
          {
          vertex[0] = static_cast<VertexValueType>(i);
          for (double j = polygonExtRingBoundReg.GetOrigin(1);
               j < polygonExtRingBoundReg.GetOrigin(1) + polygonExtRingBoundReg.GetSize(1);
               j += this->GetOutput()->GetSpacing()[1])
            {
            vertex[1] = static_cast<VertexValueType>(j);

            if (correctPolygonExtRing->IsInside(vertex) || correctPolygonExtRing->IsOnEdge (vertex))
              {
              IndexType index;
              index[0] = static_cast<IndexValueType>(vertex[0] - polygonExtRingBoundReg.GetOrigin(0));
              index[1] = static_cast<IndexValueType>(vertex[1] - polygonExtRingBoundReg.GetOrigin(1));
//               index[0] += this->GetOutput()->GetOrigin()[0];
//               index[1] += this->GetOutput()->GetOrigin()[1];
//               std::cout << "index " << index << std::endl;
              if (this->GetOutput()->HasLabel(m_lab))
                {
                if (!this->GetOutput()->GetLabelObject(m_lab)->HasIndex(index))
                  { //Add a pixel to the current labelObject
                  this->GetOutput()->SetPixel(index, m_lab);
                  }
                }
              else
                {
                //Add a pixel to the current labelObject
                this->GetOutput()->SetPixel(index, m_lab);
                }
              }
            }
          }
        //Modify the label for the next layer
        m_lab += 10;
        break;
        }
      case FEATURE_MULTIPOINT:
        {
        itkExceptionMacro(
          <<
          "This type (FEATURE_MULTIPOINT) is not handle (yet) by VectorDataToLabelMapFilter(), please request for it");
        break;
        }
      case FEATURE_MULTILINE:
        {
        itkExceptionMacro(
          << "This type (FEATURE_MULTILINE) is not handle (yet) by VectorDataToLabelMapFilter(), please request for it");
        break;
        }
      case FEATURE_MULTIPOLYGON:
        {
        itkExceptionMacro(
          <<
          "This type (FEATURE_MULTIPOLYGON) is not handle (yet) by VectorDataToLabelMapFilter(), please request for it");
        break;
        }
      case FEATURE_COLLECTION:
        {
        itkExceptionMacro(
          <<
          "This type (FEATURE_COLLECTION) is not handle (yet) by VectorDataToLabelMapFilter(), please request for it");
        break;
        }
      }
    }
}

template <class TVectorData, class TLabelMap>
void
VectorDataToLabelMapFilter<TVectorData, TLabelMap>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "BackgroundValue: "  << static_cast<typename itk::NumericTraits<OutputLabelMapPixelType>::PrintType>(
    m_BackgroundValue) << std::endl;
}

} // end namespace otb

#endif
