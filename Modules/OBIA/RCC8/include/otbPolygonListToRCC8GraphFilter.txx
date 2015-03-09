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
#ifndef __otbPolygonListToRCC8GraphFilter_txx
#define __otbPolygonListToRCC8GraphFilter_txx

#include "otbPolygonListToRCC8GraphFilter.h"
#include "itkProgressReporter.h"

namespace otb
{
/**
 * Constructor.
 */
template <class TPolygonList, class TOutputGraph>
PolygonListToRCC8GraphFilter<TPolygonList, TOutputGraph>
::PolygonListToRCC8GraphFilter()
{
  this->SetNumberOfRequiredInputs(1);
  m_Optimisation = false;
  m_UseInverted = false;
}
/**
 * Destructor.
 */
template <class TPolygonList, class TOutputGraph>
PolygonListToRCC8GraphFilter<TPolygonList, TOutputGraph>
::~PolygonListToRCC8GraphFilter()
{}

template <class TPolygonList, class TOutputGraph>
void
PolygonListToRCC8GraphFilter<TPolygonList, TOutputGraph>
::SetInput(const PolygonListType *input)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<PolygonListType *>(input));
}

template <class TPolygonList, class TOutputGraph>
const typename PolygonListToRCC8GraphFilter<TPolygonList, TOutputGraph>::PolygonListType *
PolygonListToRCC8GraphFilter<TPolygonList, TOutputGraph>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const TPolygonList *>
           (this->itk::ProcessObject::GetInput(0));
}

template <class TPolygonList, class TOutputGraph>
unsigned int
PolygonListToRCC8GraphFilter<TPolygonList, TOutputGraph>
::GetRelationsCount(RCC8ValueType val)
{
  return m_Accumulator[val];
}

template <class TPolygonList, class TOutputGraph>
unsigned int
PolygonListToRCC8GraphFilter<TPolygonList, TOutputGraph>
::GetNumberOfRelations()
{
  unsigned int result = 0;
  for (unsigned int i = 0; i < 8; ++i)
    {
    result += m_Accumulator[i];

    }
  return result;
}
template <class TPolygonList, class TOutputGraph>
typename PolygonListToRCC8GraphFilter<TPolygonList, TOutputGraph>
::KnowledgeStateType
PolygonListToRCC8GraphFilter<TPolygonList, TOutputGraph>
::GetKnowledge(RCC8ValueType r1, RCC8ValueType r2)
{
  m_Accumulator[0] = 0;
  m_Accumulator[1] = 0;
  m_Accumulator[2] = 0;
  m_Accumulator[3] = 0;
  m_Accumulator[4] = 0;
  m_Accumulator[5] = 0;
  m_Accumulator[6] = 0;
  m_Accumulator[7] = 0;

  // otbMsgDebugMacro(<<"RCC8GraphFilter: entering GetKnowledge method.");
  // This is the RCC8 composition table
  const int knowledge[8][8]
    =
    {
    /**         DC  EC  PO TPP TPPi NTPP i  EQ */
    /** DC */ {-3, -2, -2, -2,  0, -2,  0,  0},
    /** EC  */ {-1, -3, -2, -3, -1, -3,  0,  1},
    /** PO  */ {-1, -1, -3, -3, -1, -3, -1,  2},
    /** TPP */ { 0, -1, -2, -3, -3,  5, -1,  3},
    /** TPPi*/ {-1, -1, -1, -3, -1, -3,  6,  4},
    /** NTPP*/ { 0,  0, -2,  5, -2,  5, -3,  5},
    /** NTPPi*/ {-1, -1, -1, -1,  6, -3,  6,  6},
    /** EQ  */ { 0,  1,  2,  3,  4,  5,  6,  7}
    };

  int value = knowledge[r1][r2];
  // Each negative case correspond to a level of knowledge
  if (value >= 0)
    {
    // otbMsgDebugMacro(<<"RCC8GraphFilter: leaving GetKnowledge method: FULL");
    return KnowledgeStateType(FULL, static_cast<RCC8ValueType>(value));
    }
  else if (value == -1)
    {
    // otbMsgDebugMacro(<<"RCC8GraphFilter: leaving GetKnowledge method: LEVEL_1");
    return KnowledgeStateType(LEVEL_1, OTB_RCC8_DC);
    }
  else if (value == -2)
    {
    // otbMsgDebugMacro(<<"RCC8GraphFilter: leaving GetKnowledge method.LEVEL_3");
    return KnowledgeStateType(LEVEL_3, OTB_RCC8_DC);
    }
  else
    {
    // otbMsgDebugMacro(<<"RCC8GraphFilter: leaving GetKnowledge method.NO_INFO");
    return KnowledgeStateType(NO_INFO, OTB_RCC8_DC);
    }
}
/**
 * Main computation method.
 */
template <class TPolygonList, class TOutputGraph>
void
PolygonListToRCC8GraphFilter<TPolygonList, TOutputGraph>
::GenerateData()
{
  // Call a method that can be overridden by a subclass to perform
  // some calculations prior to splitting the main computations into
  // separate threads
  this->BeforeThreadedGenerateData();

  // Set up the multithreaded processing
  ThreadStruct str;
  str.Filter = this;

  // // Initializing edges vectors per thread
  EdgeMapType defaultEdgeMap;
  m_EdgesPerThread = EdgeMapVectorType(this->GetNumberOfThreads(), defaultEdgeMap);

  // Setting up multithreader
  this->GetMultiThreader()->SetNumberOfThreads(this->GetNumberOfThreads());
  this->GetMultiThreader()->SetSingleMethod(this->ThreaderCallback, &str);

  // multithread the execution
  this->GetMultiThreader()->SingleMethodExecute();

  // Call a method that can be overridden by a subclass to perform
  // some calculations after all the threads have completed
  this->AfterThreadedGenerateData();
}

template <class TPolygonList, class TOutputGraph>
void
PolygonListToRCC8GraphFilter<TPolygonList, TOutputGraph>
::BeforeThreadedGenerateData()
{
  // Ouptut graph pointer
  OutputGraphPointerType      graph = this->GetOutput();
  PolygonListConstPointerType inputPtr = this->GetInput();

// Initializing output graph
  graph->SetNumberOfVertices(inputPtr->Size());
  graph->Build();

  // Vertex indexes
  unsigned int nbVertices = 0;

  typedef typename PolygonListType::ConstIterator PolygonListConstIteratorType;

  // Loads the polygons list to graph nodes
  for (PolygonListConstIteratorType it = inputPtr->Begin();
       it != inputPtr->End(); ++it)
    {
    // Create a new vertex
    VertexPointerType vertex = VertexType::New();
    // Set its properties
    vertex->SetPath(it.Get());

    // look for the appropriate segmentation index

    unsigned int segIndex = 1;

    while (segIndex < m_SegmentationRanges.size() &&
           (nbVertices < m_SegmentationRanges[segIndex - 1] || nbVertices >= m_SegmentationRanges[segIndex]))
      {
      ++segIndex;
      }

    vertex->SetSegmentationLevel(segIndex - 1);

    // Put it in the graph
    graph->SetVertex(nbVertices, vertex);
    otbMsgDevMacro(<< "Adding vertex: " << nbVertices);
    ++nbVertices;
    }
}

template <class TPolygonList, class TOutputGraph>
void
PolygonListToRCC8GraphFilter<TPolygonList, TOutputGraph>
::ThreadedGenerateData(unsigned int startIndex, unsigned int stopIndex, itk::ThreadIdType threadId)
{
  //std::cout<<"Starting thread "<<threadId <<" to work on range ["<<startIndex<<", "<<stopIndex<<"]"<<std::endl;

  // Ouptut graph pointer
  OutputGraphPointerType      graph = this->GetOutput();
  PolygonListConstPointerType inputPtr = this->GetInput();

  // invert value vector
  RCC8ValueType invert[8] = {OTB_RCC8_DC, OTB_RCC8_EC, OTB_RCC8_PO, OTB_RCC8_TPPI,
                             OTB_RCC8_TPP, OTB_RCC8_NTPPI, OTB_RCC8_NTPP, OTB_RCC8_EQ};

  unsigned int nbVertices = graph->GetNumberOfVertices();
  itk::ProgressReporter progress(this, threadId, ((int) stopIndex - (int) startIndex)*nbVertices);

  otbMsgDevMacro(<< "Adjacency matrix size: " << nbVertices * nbVertices);

  VertexIteratorType vIt1(graph);
  VertexIteratorType vIt2(graph);

  unsigned int count = 0;

  // For each couple of vertices
  for (vIt1.GoToBegin(); !vIt1.IsAtEnd(); ++vIt1)
    {
    // TODO: this is not correct and should be replaced by a
    // vIt1(graph, index1, index2), which does not exist for the moment
    if ((count >= startIndex) && (count < stopIndex))
      {
      for (vIt2.GoToBegin(); !vIt2.IsAtEnd(); ++vIt2)
        {
        //We do not examine each couple because of the RCC8 symmetry
        if (vIt1.GetIndex() < vIt2.GetIndex())
          {

          // Compute the RCC8 relation
          typename RCC8CalculatorType::Pointer calc = RCC8CalculatorType::New();
          calc->SetPolygon1(vIt1.Get()->GetPath());
          calc->SetPolygon2(vIt2.Get()->GetPath());
          RCC8ValueType value = OTB_RCC8_DC;

          // if the optimisations are activated
          if (m_Optimisation)
            {
            //  otbMsgDebugMacro(<<"RCC8GraphFilter: Entering optimisation loop");
            InEdgeIteratorType inIt1(vIt1.GetIndex(), graph);
            InEdgeIteratorType inIt2(vIt2.GetIndex(), graph);
            // otbMsgDebugMacro(<<"Optimisation loop: iterators initialised");
            VertexDescriptorType betweenIndex;
            KnowledgeStateType know(NO_INFO, OTB_RCC8_DC);
            inIt1.GoToBegin();

            // Iterate through the edges going to the first vertex
            while (!inIt1.IsAtEnd() && (know.first != FULL))
              {
              betweenIndex = inIt1.GetSourceIndex();
              inIt2.GoToBegin();
              bool edgeFound = false;
              while (!inIt2.IsAtEnd() && (know.first != FULL))
                {
                // try to find an intermediate vertex between the two ones which
                // we vant to compute the relationship
                if (inIt2.GetSourceIndex() == betweenIndex)
                  {
                  // if an intermediate vertex is found
                  edgeFound = true;
                  // otbMsgDebugMacro(<<"Optimisation loop: found an intermediary vertex:" <<betweenIndex);
                  // See if it brings some info on the RCCC8 value
                  know = GetKnowledge(invert[inIt1.GetValue()], inIt2.GetValue());
                  calc->SetLevel1APrioriKnowledge(know.first == LEVEL_1);
                  calc->SetLevel3APrioriKnowledge(know.first == LEVEL_3);
                  //  otbMsgDebugMacro(<<"Optimisation loop: knowledge: "<<know.first<<","<<know.second);
                  }
                ++inIt2;
                }
              // If no intermediate was found
              if (!edgeFound)
                {
                //  otbMsgDebugMacro(<<"Optimisation loop: found an intermediary vertex:" <<betweenIndex);
                // Try using a DC relationship
                know = GetKnowledge(invert[inIt1.GetValue()], OTB_RCC8_DC);
                calc->SetLevel1APrioriKnowledge(know.first == LEVEL_1);
                calc->SetLevel3APrioriKnowledge(know.first == LEVEL_3);
                // otbMsgDebugMacro(<<"Optimisation loop: knowledge: "<<know.first<<","<<know.second);
                }
              ++inIt1;
              }
            // If the search has fully determined the RCC8
            if (know.first == FULL)
              {
              // Get the value
              value = know.second;
              }
            else
              {
              // Else trigger the computation
              // (which will take the optimisation phase info into account)
              calc->Compute();
              value = calc->GetValue();
              }
            // otbMsgDebugMacro(<<"RCC8GraphFilter: Leaving optimisation loop");
            }
          // If the optimisations are not activated
          else
            {
            calc->Compute();
            value = calc->GetValue();
            }
          m_Accumulator[value] += 1;
          m_Accumulator[invert[value]] += 1;
          // If the vertices are connected
          if (value > OTB_RCC8_DC)
            {
            // Add the edge to the graph.
            otbMsgDevMacro(<< "Adding edge: " << vIt1.GetIndex() << " -> " << vIt2.GetIndex() << ": " << value);
            if (value == OTB_RCC8_NTPPI)
              {
              m_EdgesPerThread[threadId][EdgePairType(vIt2.GetIndex(), vIt1.GetIndex())] = OTB_RCC8_NTPP;
              }
            else if (value == OTB_RCC8_TPPI)
              {
              m_EdgesPerThread[threadId][EdgePairType(vIt2.GetIndex(), vIt1.GetIndex())] = OTB_RCC8_TPP;
              }
            else
              {
              m_EdgesPerThread[threadId][EdgePairType(vIt1.GetIndex(), vIt2.GetIndex())] = value;
              }
            if (m_UseInverted)
              {
              m_EdgesPerThread[threadId][EdgePairType(vIt2.GetIndex(), vIt1.GetIndex())] = invert[value];
              }
            }
          }
        progress.CompletedPixel();
        progress.CompletedPixel();
        }
      }
    ++count;
    }
  otbMsgDebugMacro(<< "End thread " << threadId);
}

template <class TPolygonList, class TOutputGraph>
void
PolygonListToRCC8GraphFilter<TPolygonList, TOutputGraph>
::AfterThreadedGenerateData()
{
  // in order to have the same output graph whatever the number of
  // thread is, we use a map to sort the edges in lexicographical
  // order

  OutputGraphPointerType graph = this->GetOutput();
  EdgeMapType            globalEdgeMap;

  // merge all edges
  for (typename EdgeMapVectorType::iterator vIt = m_EdgesPerThread.begin();
       vIt != m_EdgesPerThread.end(); ++vIt)
    {
    for (typename EdgeMapType::iterator mIt = (*vIt).begin();
         mIt != (*vIt).end(); ++mIt)
      {
      globalEdgeMap[mIt->first] = mIt->second;
      }
    }

  // Report edges to the graph
  for (typename EdgeMapType::iterator mIt = globalEdgeMap.begin();
       mIt != globalEdgeMap.end(); ++mIt)
    {
    graph->AddEdge(mIt->first.first, mIt->first.second, mIt->second);
    }
}

// Callback routine used by the threading library. This routine just calls
// the ThreadedGenerateData method after setting the correct region for this
// thread.

template <class TPolygonList, class TOutputGraph>
ITK_THREAD_RETURN_TYPE
PolygonListToRCC8GraphFilter<TPolygonList, TOutputGraph>
::ThreaderCallback(void *arg)
{
  ThreadStruct *str;
  int           threadId, threadCount;
  unsigned int  total, start, stop;

  threadId = ((itk::MultiThreader::ThreadInfoStruct *) (arg))->ThreadID;
  threadCount = ((itk::MultiThreader::ThreadInfoStruct *) (arg))->NumberOfThreads;
  str = (ThreadStruct *) (((itk::MultiThreader::ThreadInfoStruct *) (arg))->UserData);

  total = str->Filter->GetOutput()->GetNumberOfVertices();

  if (threadId < static_cast<int>(total))
    {

    // Split the adjacency matrix in strip of equal dimension
    start =
      static_cast<unsigned int>(vcl_floor(total *
                                          vcl_sqrt(static_cast<double>(threadId) /
                                                   static_cast<double>(threadCount)) + 0.5));
    stop =
      static_cast<unsigned int>(vcl_floor(total *
                                          vcl_sqrt(static_cast<double>(threadId +
                                                                       1) / static_cast<double>(threadCount)) + 0.5));
    if (stop > total) stop = total;

    // For very small graphs it might occur that start = stop. In this
    // case the vertex at that index will be processed in the next strip.
    if (start != stop)
      {
      str->Filter->ThreadedGenerateData(start, stop, threadId);
      }
    }
  // else
  //   {
  //   otherwise don't use this thread. Sometimes the threads dont
  //   break up very well and it is just as efficient to leave a
  //   few threads idle.
  //   }

  return ITK_THREAD_RETURN_VALUE;
}

template <class TPolygonList, class TOutputGraph>
void
PolygonListToRCC8GraphFilter<TPolygonList, TOutputGraph>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace otb
#endif
