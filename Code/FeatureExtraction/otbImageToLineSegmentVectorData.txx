/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbImageToLineSegmentVectorData_txx
#define __otbImageToLineSegmentVectorData_txx

#include "otbImageToLineSegmentVectorData.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"


namespace otb
{

template<class TInputImage, class TPrecision>
PersistentLineSegmentDetector<TInputImage, TPrecision>
::PersistentLineSegmentDetector()
{
  //m_LineDetector = LineDetectorType::New();
   m_LineSpatialObjectList = LineSpatialObjectListType::New();
}


template<class TInputImage, class TPrecision>
void
PersistentLineSegmentDetector<TInputImage, TPrecision>
::Reset()
{
  m_LineSpatialObjectList->Clear();
}

template<class TInputImage, class TPrecision>
void
PersistentLineSegmentDetector<TInputImage, TPrecision>
::Synthetize()
{}


template<class TInputImage, class TPrecision>
void
PersistentLineSegmentDetector<TInputImage, TPrecision>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();

  if ( this->GetInput() )
  {
    ImagePointer image = const_cast< ImageType * >( this->GetInput() );
    image->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
  }
}


template<class TInputImage, class TPrecision>
void
PersistentLineSegmentDetector<TInputImage, TPrecision>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  if ( this->GetInput() )
  {
    this->GetOutput()->CopyInformation(this->GetInput());
    this->GetOutput()->SetLargestPossibleRegion(this->GetInput()->GetLargestPossibleRegion());
  }

  if (this->GetOutput()->GetRequestedRegion().GetNumberOfPixels()==0)
  {
    this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetLargestPossibleRegion());
  }
}


template<class TInputImage, class TPrecision>
void
PersistentLineSegmentDetector<TInputImage, TPrecision>
::ThreadedGenerateData (const RegionType& outputRegionForThread,int threadId)
{
  ImagePointer inputPtr = const_cast< TInputImage * >( this->GetInput() );
  ImagePointer outputPtr = const_cast< TInputImage * >( this->GetOutput() );

  inputPtr->SetRequestedRegion(outputRegionForThread);
  inputPtr->PropagateRequestedRegion();
  inputPtr->UpdateOutputData();

  LineDetectorPointerType lineDetector = LineDetectorType::New();
  ExtractorPointerType extractor = ExtractorType::New();
  
  extractor->SetInput(this->GetInput());
  extractor->SetExtractionRegion(this->GetInput()->GetRequestedRegion());
  extractor->UpdateOutputInformation();

  lineDetector->SetInput(extractor->GetOutput());
  lineDetector->Update();


  m_LineSpatialObjectList->PushBack(lineDetector->GetOutput());
}

template<class TInputImage, class TPrecision>
void
PersistentLineSegmentDetector<TInputImage, TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}


// end of class PersistentLineSegmentDetector

/**===========================================================================*/

template<class TInputImage, class TPrecision>
ImageToLineSegmentVectorData<TInputImage, TPrecision>
::ImageToLineSegmentVectorData()
{
}


template<class TInputImage, class TPrecision>
typename ImageToLineSegmentVectorData<TInputImage, TPrecision>::VectorDataPointerType
ImageToLineSegmentVectorData<TInputImage, TPrecision>
::GetOutputVectorData()
{
  unsigned int listSize = this->GetLines()->Size();
  if( listSize == 0)
    {
      itkExceptionMacro(<<"No LineSpatialObject to vectorize");
    }
  
  VectorDataPointerType vlines = VectorDataType::New();
  DataNodePointerType document = DataNodeType::New();
  DataNodePointerType folder   = DataNodeType::New();
  DataNodePointerType root     = vlines->GetDataTree()->GetRoot()->Get();
  
  document->SetNodeType(otb::DOCUMENT);
  folder->SetNodeType(otb::FOLDER);
  vlines->GetDataTree()->Add(document,root);
  vlines->GetDataTree()->Add(folder,document);
  vlines->SetProjectionRef(this->GetInput()->GetProjectionRef());
  
  for(unsigned int i = 0; i<listSize; i++ )
    {  
      typename LineSpatialObjectType::const_iterator  it    = this->GetLines()->GetNthElement(i)->begin();
      typename LineSpatialObjectType::const_iterator  itEnd = this->GetLines()->GetNthElement(i)->end();
  

      while(it != itEnd)
	{
	  VertexType p1,p2;
	  typename LineType::Pointer l = LineType::New();
	  typename LineSpatialObjectType::LineType::PointListType & pointsList = (*it)->GetPoints();
	  typename LineSpatialObjectType::LineType::PointListType::const_iterator itPoints = pointsList.begin();
	  p1[0] =(*itPoints).GetPosition()[0];     //First Vertex
	  p1[1] =(*itPoints).GetPosition()[1];
	  itPoints++;
	  p2[0] =(*itPoints).GetPosition()[0];     //Second Vertex
	  p2[1] =(*itPoints).GetPosition()[1];
	  l->AddVertex(p1);
	  l->AddVertex(p2);
	 
	  DataNodePointerType node = DataNodeType::New();
	  node->SetNodeType(otb::FEATURE_LINE);
	  node->SetLine(l);
	  vlines->GetDataTree()->Add(node,folder);
	  ++it;
	}  /** End lsd To VectorData */
    }
  
  return vlines;
}






}// end namespace otb
#endif
