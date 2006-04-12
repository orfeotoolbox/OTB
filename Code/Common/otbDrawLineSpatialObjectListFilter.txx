/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   06 avril 2006
  Role      :   Draw Line Spatial Object List
  $Id : $ 

=========================================================================*/
#ifndef __otbDrawLineSpatialObjectListFilter_txx
#define __otbDrawLineSpatialObjectListFilter_txx

#include "otbDrawLineSpatialObjectListFilter.h"

#include "itkDataObject.h"
#include "itkExceptionObject.h"
#include "itkImageRegionIterator.h"

#include <math.h>


namespace otb
{

/** 
 *
 */
template <class TInputImage, class TOutputImage>
DrawLineSpatialObjectListFilter<TInputImage, TOutputImage>::DrawLineSpatialObjectListFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1); 
  
  m_DrawLineFilter = DrawLineType::New();
  m_InputList.clear();

}

    
template <class TInputImage, class TOutputImage>
void
DrawLineSpatialObjectListFilter<TInputImage, TOutputImage>
::SetInputLineSpatialObjectList(const LinesListType & list)
{
  m_InputList = list;
  this->Modified();
}


template <class TInputImage, class TOutputImage>
typename DrawLineSpatialObjectListFilter<TInputImage, TOutputImage>::LinesListType & 
DrawLineSpatialObjectListFilter<TInputImage, TOutputImage>
::GetInputLineSpatialObjectList(void)
{
  return m_InputList;   
}

template <class TInputImage, class TOutputImage>
void
DrawLineSpatialObjectListFilter<TInputImage, TOutputImage>
::GenerateData(void)
{

  typename InputImageType::ConstPointer input  = this->GetInput();
  typename OutputImageType::Pointer  	output = this->GetOutput();

  typename OutputImageType::RegionType region;
  region.SetSize(input->GetLargestPossibleRegion().GetSize());
  region.SetIndex(input->GetLargestPossibleRegion().GetIndex());
  output->SetRegions( region );
  output->SetOrigin(input->GetOrigin());
  output->SetSpacing(input->GetSpacing());
  output->Allocate();
  output->FillBuffer(0);

std::cout<<"list = "<<m_InputList.size()<<std::endl;
 
  LineListIterator  itList = m_InputList.begin();
  m_DrawLineFilter->SetInputImage( output );
  m_DrawLineFilter->SetInputLine( *itList );
  m_DrawLineFilter->GraftOutput( this->GetOutput() );
  m_DrawLineFilter->Update();
  ++itList;
  
int cpt=0;
  
  // Draw each line of the list
  while( itList != m_InputList.end() )
     {
     	
std::cout<<"cpt = "<<cpt<<" "<<std::endl;
cpt++;
    
     m_DrawLineFilter->SetInputImage( this->GetOutput() ); 
     m_DrawLineFilter->SetInputLine( *itList );
     m_DrawLineFilter->Update();
     

     ++itList;

	if (  itList == m_InputList.end() ) 
	   std::cout<<"Idem"<<std::endl;  
  
     }

  this->GraftOutput( m_DrawLineFilter->GetOutput() );

	
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput>
void 
DrawLineSpatialObjectListFilter<TInputImage, TOutput>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


} // end namespace otb


#endif
