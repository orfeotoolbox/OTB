/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   06 avril 2006
  Role      :   Draw Line Spatial Object
  $Id : $ 

=========================================================================*/
#ifndef __otbDrawLineSpatialObjectFilter_txx
#define __otbDrawLineSpatialObjectFilter_txx

#include "otbDrawLineSpatialObjectFilter.h"

#include "itkDataObject.h"
#include "itkExceptionObject.h"
#include "itkImageRegionIterator.h"
#include "itkImageIteratorWithIndex.h"
#include "itkImageConstIteratorWithIndex.h"

#include <math.h>


namespace otb
{

/** 
 *
 */
template <class TInputImage, class TOutputImage>
DrawLineSpatialObjectFilter<TInputImage, TOutputImage>::DrawLineSpatialObjectFilter()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(1); 
  
  m_Value = static_cast<OutputPixelType>(255.0);
   
}

template <class TInputImage, class TOutputImage>
void
DrawLineSpatialObjectFilter<TInputImage, TOutputImage>
::SetInputLine(const InputLineType *line)
{  
  this->itk::ProcessObject::SetNthInput(0, 
                                   const_cast< InputLineType * >( line ) );
}


template <class TInputImage, class TOutputImage>
typename DrawLineSpatialObjectFilter<TInputImage, TOutputImage>::InputLineType * 
DrawLineSpatialObjectFilter<TInputImage, TOutputImage>
::GetInput(void)
{  
    return static_cast<InputLineType *>
    (this->itk::ProcessObject::GetInput(0) );
}
    
    
template <class TInputImage, class TOutputImage>
void
DrawLineSpatialObjectFilter<TInputImage, TOutputImage>
::SetInputImage(const InputImageType *image)
{
  this->itk::ProcessObject::SetNthInput(1, 
                                   const_cast< InputImageType * >( image ) );
}


template <class TInputImage, class TOutputImage>
const typename DrawLineSpatialObjectFilter<TInputImage, TOutputImage>::InputImageType *
DrawLineSpatialObjectFilter<TInputImage, TOutputImage>
::GetInputImage(void)
{
  if (this->GetNumberOfInputs() < 2)
    {
    return 0;
    }
  
  return static_cast<const InputImageType *>
    (this->itk::ProcessObject::GetInput(1) );
}

template <class TInputImage, class TOutputImage>
void
DrawLineSpatialObjectFilter<TInputImage, TOutputImage>
::GenerateData(void)
{

  typename InputImageType::ConstPointer 	input  = this->GetInputImage();
  typename OutputImageType::Pointer  		output = this->GetOutput();

    // Get the region
  typename OutputImageType::RegionType region;
  
//  typename OutputImageType::RegionType region =
//                        output->GetLargestPossibleRegion();
  region.SetSize(input->GetLargestPossibleRegion().GetSize());
  region.SetIndex(input->GetLargestPossibleRegion().GetIndex());
  output->SetRegions( region );
  output->SetOrigin(input->GetOrigin());
  output->SetSpacing(input->GetSpacing());
  output->Allocate();

  typedef itk::ImageRegionIteratorWithIndex< OutputImageType > OutputIteratorType; 
  typedef itk::ImageRegionConstIteratorWithIndex< InputImageType >  InputIteratorType; 
    
  OutputIteratorType    outputIt( output, output->GetRequestedRegion() );
  InputIteratorType     inputIt(  input,  input->GetRequestedRegion() );

  outputIt.GoToBegin();
  inputIt.GoToBegin();

  // Copy the input image in the output image
  for ( outputIt.GoToBegin(); !outputIt.IsAtEnd(); ++outputIt,++inputIt)
     outputIt.Set( static_cast<OutputPixelType>(inputIt.Get()) );

  // Get the LineSpatialObject
  InputLineType * inputLine = this->GetInput();

  // Get the list of points which consists of two points to represent a
  // straight line 
  PointListType & pointsList = inputLine->GetPoints();
  typename PointListType::const_iterator   itPoints = pointsList.begin();

  IndexType outputIndex;

  double x1, y1;
  x1 = (*itPoints).GetPosition()[0];
  y1 = (*itPoints).GetPosition()[1];
  
  itPoints++;
  
  double x2, y2;
  x2 = (*itPoints).GetPosition()[0];
  y2 = (*itPoints).GetPosition()[1];

std::cout<<"("<<x1<<","<<y1<<")"<<std::endl;
std::cout<<"("<<x2<<","<<y2<<")"<<std::endl;

  // Get image size
  itk::Size<2> size = output->GetLargestPossibleRegion().GetSize();
                        
  // Distance between two points
  double DeltaX, DeltaY;
    
  DeltaX = fabs(x2-x1);
  DeltaY = fabs(y2-y1);

  // To draw the line, we seek in which direction the number 
  // of pixels between the two points is most important
  
  if ( (DeltaX >= DeltaY) && (DeltaX > 0.) )
     {
     long int Xmin, Xmax;	
     Xmin = static_cast<long int>(std::min(x1,x2));
     Xmax = static_cast<long int>(std::max(x1,x2));
  
     // Slope of the line y=slope*(x-x1)+y1
     double Slope = (y2-y1) / (x2-x1) ;

     // Set a point for each x value between xmin and xmax	 
     for ( long int x = Xmin; x <= Xmax; x++)
        {
	outputIndex[0] = static_cast<long int>( x ) ;
	outputIndex[1] = static_cast<long int>( Slope*(x-x1) + y1 ) ;

	 
	// Set the point if the pixel index belongs to the output image  
	if( region.IsInside( outputIndex ) ) {
std::cout<<"Index out ("<<outputIndex[0]<<","<<outputIndex[1]<<") Value = "<< (int)m_Value<<std::endl;
	   output->SetPixel( outputIndex, m_Value);
	}
	}
	 
      }
   else if ( DeltaX < DeltaY )
      {
      long int Ymin, Ymax;  
      Ymin = static_cast<long int>(std::min(y1,y2));
      Ymax = static_cast<long int>(std::max(y1,y2));
  	 	
      double SlopeInv = (x2-x1) / (y2-y1) ;
	 
      for ( long int y = Ymin; y <= Ymax; y++)
         {
	 outputIndex[0] = static_cast<long int>( SlopeInv * (y-y1) + x1 ) ;
	 outputIndex[1] = static_cast<long int>( y );
	    
	 if( region.IsInside( outputIndex ) )
	    output->SetPixel( outputIndex, m_Value);
	      
	 } 
      } 

   // Exception
   else 
      {
      itkExceptionMacro(<< "otb::DrawLineSpatialObjectFilter::GenerateData : "
                      	<< "the line is defined by one point : deltaX = deltaY = 0.");
      }
	
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput>
void 
DrawLineSpatialObjectFilter<TInputImage, TOutput>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Path Value: " << m_Value << std::endl;
}


} // end namespace otb


#endif
