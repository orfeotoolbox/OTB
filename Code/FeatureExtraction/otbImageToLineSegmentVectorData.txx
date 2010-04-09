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
  m_LineSpatialObjectList = LineSpatialObjectListType::New();
  m_Extractor = ExtractorListType::New();
  m_LineDetector = LineDetectorListType::New();
  m_RegionList.clear();
}

template<class TInputImage, class TPrecision>
void
PersistentLineSegmentDetector<TInputImage, TPrecision>
::Reset()
{
  m_LineSpatialObjectList->Clear();
  int nbThread = this->GetNumberOfThreads();
  m_LineSpatialObjectList->Clear();
  m_LineSpatialObjectList->Resize(nbThread);
  m_RegionList = RegionListType(nbThread);
  m_Extractor->Clear();
  m_LineDetector->Clear();

  for (int p = 0; p < nbThread; ++p)
    {
    m_Extractor->PushBack(ExtractorType::New());
    m_LineDetector->PushBack(LineDetectorType::New());
    m_LineDetector->GetNthElement(p)->SetImageSize(this->GetInput()->GetLargestPossibleRegion().GetSize());
    }

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

  if (this->GetInput())
    {
    ImagePointer image = const_cast<ImageType *>(this->GetInput());
    image->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
    }
}

template<class TInputImage, class TPrecision>
void
PersistentLineSegmentDetector<TInputImage, TPrecision>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  if (this->GetInput())
    {
    this->GetOutput()->CopyInformation(this->GetInput());
    this->GetOutput()->SetLargestPossibleRegion(this->GetInput()->GetLargestPossibleRegion());
    }

  if (this->GetOutput()->GetRequestedRegion().GetNumberOfPixels() == 0)
    {
    this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetLargestPossibleRegion());
    }
}

template<class TInputImage, class TPrecision>
void
PersistentLineSegmentDetector<TInputImage, TPrecision>
::ThreadedGenerateData(const RegionType& outputRegionForThread, int threadId)
{
  ImagePointer inputPtr = const_cast<TInputImage *>(this->GetInput());
  ImagePointer outputPtr = const_cast<TInputImage *>(this->GetOutput());

  inputPtr->SetRequestedRegion(outputRegionForThread);
  inputPtr->PropagateRequestedRegion();
  inputPtr->UpdateOutputData();

  m_Extractor->GetNthElement(threadId)->SetInput(this->GetInput());
  m_Extractor->GetNthElement(threadId)->SetExtractionRegion(outputRegionForThread);
  m_Extractor->GetNthElement(threadId)->UpdateOutputInformation();

  m_LineDetector->GetNthElement(threadId)->SetInput(m_Extractor->GetNthElement(threadId)->GetOutput());
  m_LineDetector->GetNthElement(threadId)->Update();

  m_RegionList[threadId] = outputRegionForThread;
  m_LineSpatialObjectList->SetNthElement(threadId,  m_LineDetector->GetNthElement(threadId)->GetOutput());
}

template<class TInputImage, class TPrecision>
void
PersistentLineSegmentDetector<TInputImage, TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

// end of class PersistentLineSegmentDetector

/**===========================================================================*/

template<class TInputImage, class TPrecision>
ImageToLineSegmentVectorData<TInputImage, TPrecision>
::ImageToLineSegmentVectorData()
{
  m_ThreadDistanceThreshold = 10.;
}

/**
 * The aim here is to gathered each thread output into a vector data.
 * For that we have to fuse different thread output, in particular for line that throw thread.
 * For that, for each thread and each detected line, we look if one of its extrema is over a thread lower limit (RegionList.GetSize()[1]).
 * If yes, we store the line. If no, we add it to the vector data.
 * At the next loop (ie. next thread), for each line, we check that it doesn't have a point in common with lines that end at the end of
 * the previous thread.
 * If yes we compute the extrema (one point ineach thread).
 * If not, we add it to the vector data, else we store it.
 * And so on.
 * m_ThreadDistanceThreshold allows a tolerance over the commmon point spatial distance when 2 lines are fused.
 */
template<class TInputImage, class TPrecision>
typename ImageToLineSegmentVectorData<TInputImage, TPrecision>::VectorDataPointerType
ImageToLineSegmentVectorData<TInputImage, TPrecision>
::GetOutputVectorData()
{
  unsigned int listSize = this->GetLines()->Size();
  if (listSize == 0)
    {
    itkExceptionMacro(<< "No LineSpatialObject to vectorize");
    }

  VectorDataPointerType vlines = VectorDataType::New();
  DataNodePointerType   document = DataNodeType::New();
  DataNodePointerType   folder   = DataNodeType::New();
  DataNodePointerType   root     = vlines->GetDataTree()->GetRoot()->Get();

  document->SetNodeType(otb::DOCUMENT);
  folder->SetNodeType(otb::FOLDER);
  vlines->GetDataTree()->Add(document, root);
  vlines->GetDataTree()->Add(folder, document);
  vlines->SetProjectionRef(this->GetInput()->GetProjectionRef());

  RegionListType regionList = this->GetRegionList();
  SizeType       whereAmI;
  whereAmI.Fill(0);

  typedef std::vector<VertexType> DoubleVertexType;
  std::vector<DoubleVertexType> vertexList, vertexNewList;

  for (unsigned int i = 0; i < listSize; ++i)
    {
    typename LineSpatialObjectType::const_iterator it    = this->GetLines()->GetNthElement(i)->begin();
    typename LineSpatialObjectType::const_iterator itEnd = this->GetLines()->GetNthElement(i)->end();

    while (it != itEnd)
      {
      VertexType                                                              p1, p2;
      typename LineType::Pointer                                              l = LineType::New();
      typename LineSpatialObjectType::LineType::PointListType&                pointsList = (*it)->GetPoints();
      typename LineSpatialObjectType::LineType::PointListType::const_iterator itPoints = pointsList.begin();
      p1[0] = (*itPoints).GetPosition()[0];       //First Vertex
      p1[1] = (*itPoints).GetPosition()[1];
      ++itPoints;
      p2[0] = (*itPoints).GetPosition()[0];       //Second Vertex
      p2[1] = (*itPoints).GetPosition()[1];

      bool go = false;
      // horizontal line
      if (vcl_abs(p1[1] - whereAmI[1]) < m_ThreadDistanceThreshold && vcl_abs(p2[1] - whereAmI[1]) <
          m_ThreadDistanceThreshold)
        {
        go = true;
        }
      // point over the border
      else if (vcl_abs(p1[1] - whereAmI[1]) < m_ThreadDistanceThreshold || vcl_abs(p2[1] - whereAmI[1]) <
               m_ThreadDistanceThreshold)
        {
        // have to fuse?
        unsigned int j = 0;
        bool         done = false;
        VertexType   p1Old;
        VertexType   p2Old;
        while (j < vertexList.size() && done == false)
          {
          p1Old = vertexList[j][0];
          p2Old = vertexList[j][1];

          // if p1=p1old and p2=p2Old or p1=p2old and p2=p1Old mod 1
          if ((vcl_abs(p1[0] - p1Old[0]) < m_ThreadDistanceThreshold && vcl_abs(p1[1] - p1Old[1]) <
               m_ThreadDistanceThreshold) ||
              (vcl_abs(p2[0] - p2Old[0]) < m_ThreadDistanceThreshold && vcl_abs(p2[1] - p2Old[1]) <
               m_ThreadDistanceThreshold) ||
              (vcl_abs(p1[0] - p2Old[0]) < m_ThreadDistanceThreshold && vcl_abs(p1[1] - p2Old[1]) <
               m_ThreadDistanceThreshold) ||
              (vcl_abs(p2[0] - p1Old[0]) < m_ThreadDistanceThreshold && vcl_abs(p2[1] - p1Old[1]) <
               m_ThreadDistanceThreshold))
            {
            done = true;
            }
          ++j;
          }
        if (done == true && j != 0)
          {
          DoubleVertexType vert;
          // keep the 2 lines extramities
          // p1 is the common point -> keep p2
          if (vcl_abs(p1[1] - whereAmI[1]) < m_ThreadDistanceThreshold)
            {
            vert.push_back(p2);
            // p1 is the same as p1Old -> keep p2Old
            if (vcl_abs(p1[0] - p1Old[0]) < m_ThreadDistanceThreshold && vcl_abs(p1[1] - p1Old[1]) <
                m_ThreadDistanceThreshold) vert.push_back(p2Old);
            else vert.push_back(p1Old);
            }
          // p2 is the common point -> keep p1
          else
            {
            vert.push_back(p1);
            // p2 is the same as p1Old -> keep p1Old
            if (vcl_abs(p2[0] - p2Old[0]) < m_ThreadDistanceThreshold &&
                vcl_abs(p2[1] - p2Old[1] < m_ThreadDistanceThreshold)) vert.push_back(p1Old);
            else vert.push_back(p2Old);
            }
          vertexNewList.push_back(vert);
          // j-1 because of the ++j at the end of the while
          vertexList.erase(vertexList.begin() + (j - 1));
          }
        else if (j == 0)
          {
          DoubleVertexType vert;
          vert.push_back(p1);
          vert.push_back(p2);
          vertexNewList.push_back(vert);
          }
        // no point to fuse
        else go = true;
        }   // else if( vcl_abs(p1[1]-whereAmI[1])<m_ThreadDistanceThreshold || vcl_abs(p2[1]-whereAmI[1])<m_ThreadDistanceThreshold )
      else
        {
        go = true;
        }

      if (go == true)
        {
        l->AddVertex(p1);
        l->AddVertex(p2);

        DataNodePointerType node = DataNodeType::New();
        node->SetNodeType(otb::FEATURE_LINE);
        node->SetLine(l);
        vlines->GetDataTree()->Add(node, folder);
        }

      ++it;
      }   // End while(it != itEnd) loop

    // write the false alarm in vertexList (those that don't have a continuation in the next thread)
    for (unsigned int k = 0; k < vertexList.size(); ++k)
      {
      typename LineType::Pointer l = LineType::New();
      l->AddVertex(vertexList[k][0]);
      l->AddVertex(vertexList[k][1]);

      DataNodePointerType node = DataNodeType::New();
      node->SetNodeType(otb::FEATURE_LINE);
      node->SetLine(l);
      vlines->GetDataTree()->Add(node, folder);
      }
    vertexList.clear();
    vertexList = vertexNewList;
    vertexNewList.clear();
    whereAmI[0] += regionList[i].GetSize()[0];
    whereAmI[1] += regionList[i].GetSize()[1];

    } // End for loop

  return vlines;
}

} // end namespace otb
#endif
