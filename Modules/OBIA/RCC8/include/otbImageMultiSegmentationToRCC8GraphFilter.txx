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
#ifndef __otbImageMultiSegmentationToRCC8GraphFilter_txx
#define __otbImageMultiSegmentationToRCC8GraphFilter_txx

#include "otbImageMultiSegmentationToRCC8GraphFilter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "otbPolygonToPolygonRCC8Calculator.h"
#include "otbRCC8VertexIterator.h"
#include "otbRCC8InEdgeIterator.h"
#include "otbRCC8OutEdgeIterator.h"
#include "itkProgressReporter.h"
#include "otbImageToEdgePathFilter.h"
#include "otbSimplifyPathListFilter.h"

namespace otb
{
/**
 * Constructor.
 */
template <class TInputImage, class TOutputGraph>
ImageMultiSegmentationToRCC8GraphFilter<TInputImage, TOutputGraph>
::ImageMultiSegmentationToRCC8GraphFilter()
{
  m_Optimisation = false;
}
/**
 * Destructor.
 */
template <class TInputImage, class TOutputGraph>
ImageMultiSegmentationToRCC8GraphFilter<TInputImage, TOutputGraph>
::~ImageMultiSegmentationToRCC8GraphFilter()
{}

template <class TInputImage, class TOutputGraph>
unsigned int
ImageMultiSegmentationToRCC8GraphFilter<TInputImage, TOutputGraph>
::GetRelationsCount(RCC8ValueType val)
{
  return m_Accumulator[val];
}

template <class TInputImage, class TOutputGraph>
unsigned int
ImageMultiSegmentationToRCC8GraphFilter<TInputImage, TOutputGraph>
::GetNumberOfRelations()
{
  unsigned int result = 0;
  for (unsigned int i = 0; i < 8; ++i)
    {
    result += m_Accumulator[i];

    }
  return result;
}
template <class TInputImage, class TOutputGraph>
typename ImageMultiSegmentationToRCC8GraphFilter<TInputImage, TOutputGraph>
::KnowledgeStateType
ImageMultiSegmentationToRCC8GraphFilter<TInputImage, TOutputGraph>
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
template <class TInputImage, class TOutputGraph>
void
ImageMultiSegmentationToRCC8GraphFilter<TInputImage, TOutputGraph>
::GenerateData()
{
  // Input image list pointer
  InputImageListPointerType segList = this->GetInput();

  // Ouptut graph pointer
  OutputGraphPointerType graph = this->GetOutput();

  // invert value vector
  RCC8ValueType invert[8] = {OTB_RCC8_DC, OTB_RCC8_EC, OTB_RCC8_PO, OTB_RCC8_TPPI,
                             OTB_RCC8_TPP, OTB_RCC8_NTPPI, OTB_RCC8_NTPP, OTB_RCC8_EQ};

  // Some typedefs
  typedef otb::ImageToEdgePathFilter<InputImageType, PathType> EdgeExtractionFilterType;
  typedef otb::SimplifyPathListFilter<PathType>                SimplifyPathFilterType;
  typedef typename SimplifyPathFilterType::InputListType       PathListType;

  typedef itk::MinimumMaximumImageCalculator<InputImageType> MinMaxCalculatorType;
  typedef PolygonToPolygonRCC8Calculator<PathType>           RCC8CalculatorType;
  typedef RCC8VertexIterator<OutputGraphType>                VertexIteratorType;
  typedef RCC8InEdgeIterator<OutputGraphType>                InEdgeIteratorType;


  // Vector of label
  std::vector<PixelType> maxLabelVector;

  // Vertex indexes
  unsigned int vertexIndex = 0;
  unsigned int segmentationImageIndex = 0;
  unsigned int nbVertices = 0;

  // For each segmentation image
  for (ConstListIteratorType it = segList->Begin(); it != segList->End(); ++it)
    {
    // Compute the maximum label
    typename MinMaxCalculatorType::Pointer minMax = MinMaxCalculatorType::New();
    minMax->SetImage(it.Get());
    minMax->ComputeMaximum();
    maxLabelVector.push_back(minMax->GetMaximum());
    otbMsgDebugMacro(<< "Number of objects in image " << segmentationImageIndex << ": "
                     << minMax->GetMaximum());

    // then for each region of the images
    for (PixelType label = 1; label <= maxLabelVector.back(); ++label)
      {
      typename PathListType::Pointer             region = PathListType::New();
      typename EdgeExtractionFilterType::Pointer extraction = EdgeExtractionFilterType::New();
      extraction->SetInput(it.Get());
      extraction->SetForegroundValue(label);
      extraction->Update();
      region->PushBack(extraction->GetOutput());
      typename SimplifyPathFilterType::Pointer simplifier = SimplifyPathFilterType::New();
      simplifier->SetInput(region);
      simplifier->GetFunctor().SetTolerance(0.1);
      simplifier->Update();

      // Create a new vertex
      VertexPointerType vertex = VertexType::New();
      // Set its properties
      vertex->SetPath(simplifier->GetOutput()->GetNthElement(0));
      vertex->SetSegmentationLevel(segmentationImageIndex / 2);
      vertex->SetSegmentationType(segmentationImageIndex % 2);
      // Put it in the graph
      graph->SetVertex(vertexIndex, vertex);
      ++vertexIndex;
      ++nbVertices;
      }
    ++segmentationImageIndex;
    }

  itk::ProgressReporter progress(this, 0, nbVertices*nbVertices);

  VertexIteratorType vIt1(graph);
  VertexIteratorType vIt2(graph);

  // For each couple of vertices
  for (vIt1.GoToBegin(); !vIt1.IsAtEnd(); ++vIt1)
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
          graph->AddEdge(vIt1.GetIndex(), vIt2.GetIndex(), value);
          }
        }
      progress.CompletedPixel();
      progress.CompletedPixel();
      }
    }
}

template <class TInputImage, class TOutputGraph>
void
ImageMultiSegmentationToRCC8GraphFilter<TInputImage, TOutputGraph>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace otb
#endif
