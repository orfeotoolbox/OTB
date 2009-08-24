/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryImageToLabelMapFilter.txx,v $
  Language:  C++
  Date:      $Date: 2007/02/20 15:28:29 $
  Version:   $Revision: 1.21 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBinaryImageToLabelMapFilter_txx
#define __itkBinaryImageToLabelMapFilter_txx

#include "itkBinaryImageToLabelMapFilter.h"
#include "itkNumericTraits.h"

// don't think we need the indexed version as we only compute the
// index at the start of each run, but there isn't a choice
#include "itkImageLinearConstIteratorWithIndex.h"  
#include "itkConstShapedNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkConnectedComponentAlgorithm.h"
#include "otbPolyLineImageIterator.h"

namespace otb
{
  
template <class TVectorData, class TLabelMap >
VectorDataToLabelMapFilter<TVectorData, TLabelMap >
  ::VectorDataToLabelMapFilter()
  {
    m_BackgroundValue = itk::NumericTraits<OutputLabelMapPixelType>::max();
    //m_lab = 0;
    m_lab = itk::NumericTraits<LabelType>::Zero;
    m_StartIndex.Fill( 0 );
    m_Size.Fill( 0 );
  }
  
/** 
   * overloaded because pb during copyinformation
 */
  template <class TVectorData, class TLabelMap >
   void   VectorDataToLabelMapFilter<TVectorData, TLabelMap >
  ::GenerateOutputInformation()
  {
    // we can't call the superclass method here.
  
  // get pointers to the input and output
    const InputVectorDataType * inputPtr  = this->GetInput();
    OutputLabelMapType  *    outputPtr = this->GetOutput();
    

    if ( !inputPtr || !outputPtr )
    {
      return;
    }
    
    // Set the size of the output region
    typename OutputLabelMapType::RegionType outputLargestPossibleRegion;
    outputLargestPossibleRegion.SetSize( m_Size );
    outputLargestPossibleRegion.SetIndex( m_StartIndex );
    outputPtr->SetLargestPossibleRegion( outputLargestPossibleRegion );
    
    //Set spacing and origin
    outputPtr->SetSpacing(inputPtr->GetSpacing());
    outputPtr->SetOrigin(inputPtr->GetOrigin());


  }
    
template <class TVectorData, class TLabelMap >
void
    VectorDataToLabelMapFilter<TVectorData, TLabelMap >
::GenerateInputRequestedRegion()
{
  /*
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  if( !input )
    {
    return;
    }
  input->SetRequestedRegion( input->GetLargestPossibleRegion() );
  */
}
/*
template <class TVectorData, class TLabelMap >
void 
VectorDataToLabelMapFilter<TVectorData, TLabelMap >
::EnlargeOutputRequestedRegion(itk::DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}
*/

template<class TVectorData , class TLabelMap>
    void
        VectorDataToLabelMapFilter< TVectorData, TLabelMap >
  ::SetInput(const InputVectorDataType *input)
{
// Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast< InputVectorDataType * >( input ) );
}

template<class TVectorData , class TLabelMap>
    void
        VectorDataToLabelMapFilter< TVectorData, TLabelMap >
  ::SetInput(unsigned int idx, const InputVectorDataType *input)
{
    // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(idx,
                                        const_cast< InputVectorDataType * >( input ) );
}

template<class TVectorData , class TLabelMap>
    const typename VectorDataToLabelMapFilter< TVectorData, TLabelMap >::InputVectorDataType *
        VectorDataToLabelMapFilter< TVectorData, TLabelMap >
  ::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
  {
    return 0;
  }

  return static_cast<const TVectorData * >
      (this->itk::ProcessObject::GetInput(0) );
}

template<class TVectorData , class TLabelMap>
    const typename VectorDataToLabelMapFilter< TVectorData, TLabelMap >::InputVectorDataType *
        VectorDataToLabelMapFilter< TVectorData, TLabelMap >
  ::GetInput(unsigned int idx)
{
  return static_cast<const TVectorData * >
      (this->itk::ProcessObject::GetInput(idx) );
}

template <class TVectorData, class TLabelMap >
void
    VectorDataToLabelMapFilter<TVectorData, TLabelMap >
::GenerateData()
{
  // Allocate the output
  //std::cout << "Allocates outputs..."<<std::endl;
  this->AllocateOutputs();
  //std::cout << "before GetOutput" << std::endl;
  OutputLabelMapType * output = this->GetOutput();
  //std::cout << "after GetOutput" << std::endl;
  const InputVectorDataType * input = this->GetInput();
  
  //Process filter for all inputs
  //std::cout << "Process inputs..."<<std::endl;
  for (unsigned int idx = 0; idx < this->GetNumberOfInputs(); ++idx)
  {
    if (this->GetInput(idx))
    {
      std::cout << "Process input num "<< idx <<std::endl;
      InputVectorDataConstPointer input = this->GetInput(idx);
      InternalTreeNodeType * inputRoot = const_cast<InternalTreeNodeType *>(input->GetDataTree()->GetRoot());
      //std::cout << "set background..."<<std::endl;    
      //output->SetBackgroundValue( m_BackgroundValue );
      //Use our own value for the background
      output->SetBackgroundValue( itk::NumericTraits<LabelType>::max() );
      m_lab = 0;
      ProcessNode(inputRoot);

    }
  }
}


template<class TVectorData , class TLabelMap>
void
VectorDataToLabelMapFilter< TVectorData, TLabelMap >
::ProcessNode(InternalTreeNodeType * source)
{

  
  //std::cout << "process node;num: "<< m_lab <<std::endl;
  // Get the children list from the input node
  ChildrenListType children = source->GetChildrenList();
  
  // For each child
  for(typename ChildrenListType::iterator it = children.begin(); it!=children.end();++it)
  {
    // Copy input DataNode info
    DataNodePointerType dataNode = (*it)->Get();

    switch(dataNode->GetNodeType())
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
        otbGenericMsgDebugMacro(<<"Insert Point from vectorData");
        //IndexPointerType index = IndexType::New();
        //PointType physicalPoint;
        IndexType index;
        this->GetOutput()->TransformPhysicalPointToIndex(dataNode->GetPoint(), index);
        
        
        //index.CopyWithCast(physicalPoint);
        /*
        for (unsigned int i = 0 ; i < TVectorData::Dimension ; i++)
        {
          index[i] = static_cast<typename IndexType::IndexValueType>( (dataNode->GetPoint()[i] - this->GetInput()->GetOrigin()[i] ) /
              this->GetInput()->GetSpacing[i] );
        }
        */
        
        this->GetOutput()->SetPixel( index ,m_lab);
        ++m_lab;
        //this->GetOutput()->InsertPoint( static_cast<typename TLabelMap::PointType> (dataNode->GetPoint()) );
        break;
      }
      case otb::FEATURE_LINE:
      {
//         this->GetOutput()->InsertLineString( static_cast<typename TLabelMap::LinePointerType> (dataNode->GetLine()) );
        //Iterator on the line
        /*
        typedef typename DataNodeType::LineType                          LineType;
        typedef typename LineType::RegionType                   RegionType;
        
        
        //otb::Image <TVectorData>
        typedef otb::Image< InputVectorData::PrecisionType, TVectorData::Dimension > ImageType;     
        typedef PolyLineImageIterator<RegionType,LineType> PolyLineIteratorType;
        
        PolyLineIteratorType LineIt(dataNode->GetLine()->GetBoundingRegion()->GetImageRegion (),dataNode->GetLine());
        /*
        for (LineIt.GoToBegin();!LineIt.IsAtEnd();++LineIt)
        {
          this->GetOutput()->SetPixel( LineIt.GetIndex() ,m_lab);
        }
        */
        //++m_lab;
        itkExceptionMacro(<<"This type (FEATURE_LINE) is not handle (yet) by VectorDataToLabelMapFilter(), please request for it");   
        break;
      }
      case FEATURE_POLYGON:
      {
        //std::cout << "before insert polygon" << std::endl;
//         this->GetOutput()->InsertPolygons( static_cast<typename TLabelMap::PolygonConstPointerType> (dataNode->GetPolygonExteriorRing()), static_cast<typename TLabelMap::PolygonListConstPointerType> (dataNode->GetPolygonInteriorRings()) );
        typedef typename DataNodeType::PolygonType                          PolygonType;
        typedef typename PolygonType::RegionType                   RegionType;
        typedef typename PolygonType::VertexType                    VertexType;
        RegionType polygonExtRingBoundReg = dataNode->GetPolygonExteriorRing()->GetBoundingRegion();
       
        VertexType vertex;
        //get coordinates of the bounding region of the polygon
        vertex[0]=polygonExtRingBoundReg.GetIndex()[0];
        vertex[1]=polygonExtRingBoundReg.GetIndex()[1];
        std::cout << "polygon bounding region: " << vertex << " size " << polygonExtRingBoundReg.GetSize() << std::endl;
        
        VertexType physicalVertex;
        for (unsigned int i=0;i < polygonExtRingBoundReg.GetSize(0) ;++i) 
        {
          vertex[1]=polygonExtRingBoundReg.GetIndex()[1];
          for (unsigned int j=0;j < polygonExtRingBoundReg.GetSize(1);++j) 
          {
            
            //this->GetInput()->TransformPointToPhysicalPoint (vertex, physicalVertex); 
            std::cout << "current vertex: " << vertex <<std::endl;
            if (dataNode->GetPolygonExteriorRing()->IsInside(vertex) || dataNode->GetPolygonExteriorRing()->IsOnEdge (vertex))
            {
              IndexType index;
              
              if ( this->GetOutput()->TransformPhysicalPointToIndex(vertex, index) )
              {
                
                std::cout << "add pixel index: " << index << " to label map label num:" << m_lab <<std::endl;
                this->GetOutput()->SetPixel(index ,m_lab);
              }
               
            }
            ++vertex[1];
          }  
          ++vertex[0];
        }
        //++m_lab;
        m_lab+=255;
        break;
      }
      case FEATURE_MULTIPOINT:
      {
    itkExceptionMacro(<<"This type (FEATURE_MULTIPOINT) is not handle (yet) by VectorDataToLabelMapFilter(), please request for it");   
//         this->GetOutput()->InsertMultiPoints( static_cast<typename TLabelMap::PointType> (dataNode->GetPoint()) );
        break;
      }
      case FEATURE_MULTILINE:
      {
        itkExceptionMacro(<<"This type (FEATURE_MULTILINE) is not handle (yet) by VectorDataToLabelMapFilter(), please request for it");
        break;
      }
      case FEATURE_MULTIPOLYGON:
      {
        itkExceptionMacro(<<"This type (FEATURE_MULTIPOLYGON) is not handle (yet) by VectorDataToLabelMapFilter(), please request for it");
        break;
      }
      case FEATURE_COLLECTION:
      {
        itkExceptionMacro(<<"This type (FEATURE_COLLECTION) is not handle (yet) by VectorDataToLabelMapFilter(), please request for it");
        break;
      }
    }
  }
}

template <class TVectorData, class TLabelMap >
void
VectorDataToLabelMapFilter<TVectorData, TLabelMap >
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "BackgroundValue: "  << static_cast<typename itk::NumericTraits<OutputLabelMapPixelType>::PrintType>(m_BackgroundValue) << std::endl;
}

} // end namespace itk

#endif
