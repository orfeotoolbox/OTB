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
#ifndef _otbImageMultiSegmentationToRCC8GraphFilter_txx
#define _otbImageMultiSegmentationToRCC8GraphFilter_txx

#include "otbImageMultiSegmentationToRCC8GraphFilter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "otbImageToImageRCC8Calculator.h"
#include "otbRCC8VertexIterator.h"

namespace otb
{
/** 
 * Constructor.
 */
template <class TInputImage, class TOutputGraph>
ImageMultiSegmentationToRCC8GraphFilter<TInputImage, TOutputGraph>
::ImageMultiSegmentationToRCC8GraphFilter()
{}
/**
 * Destructor.
 */
template <class TInputImage, class TOutputGraph>
ImageMultiSegmentationToRCC8GraphFilter<TInputImage, TOutputGraph>
::~ImageMultiSegmentationToRCC8GraphFilter()
{}
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

  // Some typedefs
  typedef itk::MinimumMaximumImageCalculator<InputImageType> MinMaxCalculatorType;
  typedef ImageToImageRCC8Calculator<InputImageType> RCC8CalculatorType;
  typedef RCC8VertexIterator<OutputGraphType> VertexIteratorType; 

    // Vector of label
  std::vector<PixelType> maxLabelVector;
  
  // Vertex indexes
  unsigned int vertexIndex = 0;
  unsigned int segmentationImageIndex = 0;

  // For each segmentation image
  for(ConstListIteratorType it = segList->Begin();it!=segList->End();++it)
    {
      // Compute the maximum label
      typename MinMaxCalculatorType::Pointer minMax = MinMaxCalculatorType::New();
      minMax->SetImage(it.Get());
      minMax->ComputeMaximum();
      maxLabelVector.push_back(minMax->GetMaximum());
      otbMsgDebugMacro(<<"Number of objects in image "<<segmentationImageIndex<<": "
	       <<minMax->GetMaximum());
      
      // then for each region of the images
      for(PixelType label=1; label<=maxLabelVector.back();++label)
	{
	  // Create a new vertex
	  VertexPointerType vertex = VertexType::New();
	  // Set its properties
	  vertex->SetSegmentationImageIndex(segmentationImageIndex);
	  vertex->SetObjectLabelInImage(label);
	  // Put it in the graph
	  graph->SetVertex(vertexIndex,vertex);
	  vertexIndex++;
	}
      segmentationImageIndex++;
    }

  VertexIteratorType vIt1(graph);
  VertexIteratorType vIt2(graph);

  // For each couple of vertices
  for(vIt1.GoToBegin();!vIt1.IsAtEnd();++vIt1)
    {
      for(vIt2.GoToBegin();!vIt2.IsAtEnd();++vIt2)
	{
	  // Get the segmentation images indexes 
	  unsigned int source = vIt1.Get()->GetSegmentationImageIndex();
	  unsigned int target = vIt2.Get()->GetSegmentationImageIndex();
	 
	  // We do not examine each couple because of the RCC8 simetry
	  if(source<target)
	    {
	      // Get the labels of source and target
	      PixelType label1 = vIt1.Get()->GetObjectLabelInImage();	 
	      PixelType label2 = vIt2.Get()->GetObjectLabelInImage();

	      // Compute the RCC8 relation
	      typename RCC8CalculatorType::Pointer calc = RCC8CalculatorType::New();
	      calc->SetInput1(segList->GetNthElement(source));
	      calc->SetInsideValue1(label1);
	      calc->SetInput2(segList->GetNthElement(target));
	      calc->SetInsideValue2(label2);
	      calc->Update();

	      // If the vertices are connected
	      if(calc->GetValue()>OTB_RCC8_DC)
		{
		  // Add the edge to the graph.
		  otbMsgDebugMacro(<<"Adding edge: "<<source<<" -> "<<target<<": "<<calc->GetValue());
		  graph->AddEdge(vIt1.GetIndex(),vIt2.GetIndex(),calc->GetValue());
		}
	    }
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

