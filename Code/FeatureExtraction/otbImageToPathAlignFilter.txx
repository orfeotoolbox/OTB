/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   08 fevrier 2006
  Version   :   
  Role      :   
  $Id: $

=========================================================================*/
#ifndef __otbImageToPathAlignFilter_txx
#define __otbImageToPathAlignFilter_txx

#include "otbImageToPathFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkPathIterator.h"
#include "itkNumericTraits.h"

namespace otb
{

/** Constructor */
template <class TInputImage, class TOutputPath>
PathToImageAlignFilter<TInputImage,TOutputPath>
::PathToImageAlignFilter()
{
  this->SetNumberOfRequiredInputs(1);
  m_Size.Fill(0);
  
  for (unsigned int i = 0; i < InputImageDimension; i++)
    {
    // Set an image spacing for the user
    m_Spacing[i] = 1.0;
    m_Origin[i] = 0;
    }

  m_PathValue = itk::NumericTraits<ValueType>::One;
  m_BackgroundValue = itk::NumericTraits<ValueType>::Zero;
}

/** Destructor */
template <class TInputImage, class TOutputPath>
PathToImageAlignFilter<TInputImage,TOutputPath>
::~PathToImageAlignFilter()
{
}
  

//----------------------------------------------------------------------------
template <class TInputImage, class TOutputPath>
void
PathToImageAlignFilter<TInputImage,TOutputPath>
::SetSpacing(const double* spacing)
{
  unsigned int i; 
  for (i=0; i<InputImageDimension; i++)
    {
    if ( spacing[i] != m_Spacing[i] )
      {
      break;
      }
    } 
  if ( i < InputImageDimension ) 
    { 
    for (i=0; i<InputImageDimension; i++)
      {
      m_Spacing[i] = spacing[i];
      }
    }
}

template <class TInputImage, class TOutputPath>
void
PathToImageAlignFilter<TInputImage,TOutputPath>
::SetSpacing(const float* spacing)
{
  unsigned int i; 
  for (i=0; i<InputImageDimension; i++)
    {
    if ( (double)spacing[i] != m_Spacing[i] )
      {
      break;
      }
    } 
  if ( i < InputImageDimension ) 
    { 
    for (i=0; i<InputImageDimension; i++)
      {
      m_Spacing[i] = spacing[i];
      }
    }
}

template <class TInputImage, class TOutputPath>
const double * 
PathToImageAlignFilter<TInputImage,TOutputPath>
::GetSpacing() const
{
  return m_Spacing;
}

//----------------------------------------------------------------------------
template <class TInputImage, class TOutputPath>
void
PathToImageAlignFilter<TInputImage,TOutputPath>
::SetOrigin(const double* origin)
{
  unsigned int i; 
  for (i=0; i<InputImageDimension; i++)
    {
    if ( origin[i] != m_Origin[i] )
      {
      break;
      }
    } 
  if ( i < InputImageDimension ) 
    { 
    for (i=0; i<InputImageDimension; i++)
      {
      m_Origin[i] = origin[i];
      }
    }
}

template <class TInputImage, class TOutputPath>
void
PathToImageAlignFilter<TInputImage,TOutputPath>
::SetOrigin(const float* origin)
{
  unsigned int i; 
  for (i=0; i<InputImageDimension; i++)
    {
    if ( (double)origin[i] != m_Origin[i] )
      {
      break;
      }
    } 
  if ( i < InputImageDimension ) 
    { 
    for (i=0; i<InputImageDimension; i++)
      {
      m_Origin[i] = origin[i];
      }
    }
}

template <class TInputImage, class TOutputPath>
const double * 
PathToImageAlignFilter<TInputImage,TOutputPath>
::GetOrigin() const
{
  return m_Origin;
}

//----------------------------------------------------------------------------

template <class TInputImage, class TOutputPath>
void
PathToImageAlignFilter<TInputImage,TOutputPath>
::GenerateData(void)
{
  unsigned int i;
  itkDebugMacro(<< "ImageToPathAlignFilter::GenerateData() called");

  // Get the input and output pointers 
  const InputImageType * InputImage   = this->GetInput();
  OutputPathPointer    OutputPath = this->GetOutput();

  // Generate the image
  double origin[InputImageDimension];
  SizeType size;
  
  for(i=0;i<InputImageDimension;i++)
    {
    // Set Image size to the size of the path's bounding box
    //size[i] = (long unsigned int)(InputObject->GetBoundingBox()->GetMaximum()[i]
    //                              - InputObject->GetBoundingBox()->GetMinimum()[i]);
    size[i]=0;
    origin[i]=0;
    }
  
  typename InputImageType::IndexType index;
  index.Fill(0);
  typename InputImageType::RegionType region;
  
  // If the size of the output has been explicitly specified, the filter
  // will set the output size to the explicit size, otherwise the size from the spatial
  // paths's bounding box will be used as default.
  
  bool specified = false;
  for (i = 0; i < InputImageDimension; i++)
    {
    if (m_Size[i] != 0)
      {
      specified = true;
      break;
      }
    }
  
  if (specified)
    {
    region.SetSize( m_Size );
    }
  else
    {
    itkExceptionMacro( << "Currently, the user MUST specify an image size" )
    //region.SetSize( size );
    }
  region.SetIndex( index );
  
  InputImage->SetLargestPossibleRegion( region);     // 
  InputImage->SetBufferedRegion( region );           // set the region 
  InputImage->SetRequestedRegion( region );          //                                                                       
  
  // If the spacing has been explicitly specified, the filter
  // will set the output spacing to that explicit spacing, otherwise the spacing from
  // the spatial object is used as default.
  
  specified = false;
  for (i = 0; i < InputImageDimension; i++)
    {
    if (m_Spacing[i] != 0)
      {
      specified = true;
      break;
      }
    }
  
  if (specified)
    {
    InputImage->SetSpacing(this->m_Spacing);         // set spacing
    }
  else
    {
    itkExceptionMacro( << "Currently, the user MUST specify an image spacing" )
    //OutputImage->SetSpacing(InputObject->GetIndexToObjectTransform()->GetScaleComponent());   // set spacing
    }
  InputImage->SetOrigin(origin);   //   and origin
  InputImage->Allocate();   // allocate the image                            
  
  itk::ImageRegionIteratorWithIndex<InputImageType> imageIt(InputImage,region);
  for( imageIt.GoToBegin(); !imageIt.IsAtEnd(); ++imageIt )
    {
    imageIt.Set(m_BackgroundValue);
    }
  
  itk::PathIterator<InputImageType,OutputPathType> pathIt(InputImage,OutputPath);
  for( pathIt.GoToBegin(); !pathIt.IsAtEnd(); ++pathIt )
    {
    pathIt.Set( m_PathValue );
    }
  
  itkDebugMacro(<< "ImageToPathAlignFilter::GenerateData() finished");

} // end update function  


template <class TInputImage, class TOutputPath>
void
PathToImageAlignFilter<TInputImage,TOutputPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Size : " << m_Size << std::endl;
  os << indent << "Path Value : " << m_PathValue << std::endl;
  os << indent << "Background Value : " << m_BackgroundValue << std::endl;
}



} // end namespace otb

#endif
