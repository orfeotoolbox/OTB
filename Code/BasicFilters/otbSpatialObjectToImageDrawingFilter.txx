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
#ifndef _otbSpatialObjectToImageDrawingFilter_txx
#define _otbSpatialObjectToImageDrawingFilter_txx

#include "otbSpatialObjectToImageDrawingFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "otbMacro.h"

namespace otb
{
  /** Constructor */
  template <class TInputSpatialObject, class TOutputImage>
  SpatialObjectToImageDrawingFilter<TInputSpatialObject,TOutputImage>
  ::SpatialObjectToImageDrawingFilter()
  {
    this->SetNumberOfRequiredInputs(1);
    m_ChildrenDepth = 1;
    m_Size.Fill(0);
    m_Spacing.Fill(1);
    m_Origin.Fill(0);
    m_InsideValue = 0;
    m_OutsideValue = 0;
    m_UseObjectValue = false;
    m_OriginSpecified = false;
    m_SizeSpecified = false;
    m_SpacingSpecified = false;
  }

  /** Destructor */
  template <class TInputSpatialObject, class TOutputImage>
  SpatialObjectToImageDrawingFilter<TInputSpatialObject,TOutputImage>
  ::~SpatialObjectToImageDrawingFilter()
  {
  }
  /** Set the Input SpatialObject */
  template <class TInputSpatialObject, class TOutputImage>
  void
  SpatialObjectToImageDrawingFilter<TInputSpatialObject,TOutputImage>
  ::SetInput(const InputSpatialObjectType *input)
  {
    // Process object is not const-correct so the const_cast is required here
    this->itk::ProcessObject::SetNthInput(0,
					  const_cast< InputSpatialObjectType * >( input ) );
  }


  /** Connect one of the operands  */
  template <class TInputSpatialObject, class TOutputImage>
  void
  SpatialObjectToImageDrawingFilter<TInputSpatialObject,TOutputImage>
  ::SetInput( unsigned int index, const TInputSpatialObject * object )
  {
    // Process object is not const-correct so the const_cast is required here
    this->itk::ProcessObject::SetNthInput(index,
					  const_cast< TInputSpatialObject *>( object ) );
  }



  /** Get the input Spatial Object */
  template <class TInputSpatialObject, class TOutputImage>
  const typename SpatialObjectToImageDrawingFilter<TInputSpatialObject,TOutputImage>::InputSpatialObjectType *
  SpatialObjectToImageDrawingFilter<TInputSpatialObject,TOutputImage>
  ::GetInput(void)
  {
    if (this->GetNumberOfInputs() < 1)
      {
	return 0;
      }
    return static_cast<const TInputSpatialObject * >
      (this->itk::ProcessObject::GetInput(0) );
  }

  /** Get the input Spatial Object */
  template <class TInputSpatialObject, class TOutputImage>
  const typename SpatialObjectToImageDrawingFilter<TInputSpatialObject,TOutputImage>::InputSpatialObjectType *
  SpatialObjectToImageDrawingFilter<TInputSpatialObject,TOutputImage>
  ::GetInput(unsigned int idx)
  {
    return static_cast< const TInputSpatialObject * >
      (this->itk::ProcessObject::GetInput(idx));
  }

  //----------------------------------------------------------------------------
  template <class TInputSpatialObject, class TOutputImage>
  void
  SpatialObjectToImageDrawingFilter<TInputSpatialObject,TOutputImage>
  ::SetSize(const SizeType& size)
  {
    m_Size = size;
    m_SizeSpecified = true;
    this->Modified();
  }
  //-----------------------------------------------------------------------------
  template <class TInputSpatialObject, class TOutputImage>
  void
  SpatialObjectToImageDrawingFilter<TInputSpatialObject,TOutputImage>
  ::SetSpacing(const SpacingType& spacing)
  {
    m_Spacing = spacing;
    m_SpacingSpecified = true;
    this->Modified();
  }
  template <class TInputSpatialObject, class TOutputImage>
  void
  SpatialObjectToImageDrawingFilter<TInputSpatialObject,TOutputImage>
  ::SetSpacing(const double* spacing)
  {
    for (unsigned int i=0; i<OutputImageDimension; i++)
      {
	m_Spacing[i] = spacing[i];
      }
    m_SpacingSpecified = true;
    this->Modified();
  }

  template <class TInputSpatialObject, class TOutputImage>
  void
  SpatialObjectToImageDrawingFilter<TInputSpatialObject,TOutputImage>
  ::SetSpacing(const float* spacing)
  {
    for (unsigned int i=0; i<OutputImageDimension; i++)
      {
	m_Spacing[i] = spacing[i];
      }
    m_SpacingSpecified = true;
    this->Modified();
  }
  //----------------------------------------------------------------------------
  template <class TInputSpatialObject, class TOutputImage>
  void
  SpatialObjectToImageDrawingFilter<TInputSpatialObject,TOutputImage>
  ::SetOrigin(const PointType& origin)
  {
    m_Origin=origin;
    m_OriginSpecified = true;
    this->Modified();
  }

  //----------------------------------------------------------------------------
  template <class TInputSpatialObject, class TOutputImage>
  void
  SpatialObjectToImageDrawingFilter<TInputSpatialObject,TOutputImage>
  ::SetOrigin(const double* origin)
  {
    for (unsigned int i=0; i<OutputImageDimension; i++)
      {
	m_Origin[i] = origin[i];
      }
    m_OriginSpecified = true;
    this->Modified();
  }

template <class TInputSpatialObject, class TOutputImage>
void
SpatialObjectToImageDrawingFilter<TInputSpatialObject,TOutputImage>
::SetOrigin(const float* origin)
{
  for (unsigned int i=0; i<OutputImageDimension; i++)
    {
      m_Origin[i] = origin[i];
    }
  m_OriginSpecified = true;
  this->Modified();
}

//----------------------------------------------------------------------------

/** Main computation method */
template <class TInputSpatialObject, class TOutputImage>
void
SpatialObjectToImageDrawingFilter<TInputSpatialObject,TOutputImage>
::GenerateData(void)
{
  unsigned int i;
  otbMsgDebugMacro(<< "SpatialObjectToImageDrawingFilter::Update() called");

  // Get the input and output pointers
  const InputSpatialObjectType * InputObject  = this->GetInput();
  OutputImagePointer   OutputImage = this->GetOutput();

    // Generate the image
    if((!m_OriginSpecified)||(!m_SizeSpecified))
      {
	InputObject->ComputeBoundingBox();
	if (!strcmp(InputObject->GetNameOfClass(),"GroupSpatialObject"))
	{
	  ChildrenType children;
	  children=InputObject->GetChildren(0);
	  IteratorType iter = children->begin();
	  IteratorType end = children->end();
	  PointType minimum;
	  (*iter)->ComputeBoundingBox();
	  for(i=0;i<ObjectDimension;i++)
	    {
	      minimum[i]=(*iter)->GetBoundingBox()->GetMinimum()[i];
	    }
	  while(iter != end)
	    {
	      (*iter)->ComputeBoundingBox();
	      for(i=0;i<ObjectDimension;i++)
		{
		  if ((*iter)->GetBoundingBox()->GetMinimum()[i] < minimum[i])
		    {
		      minimum[i]=(*iter)->GetBoundingBox()->GetMinimum()[i];
		    }
		}
	      iter++;
	    }
	  for(i=0;i<ObjectDimension;i++)
	    {
	      if(!m_SizeSpecified)
		m_Size[i] = static_cast<typename SizeType::SizeValueType>((InputObject->GetBoundingBox()->GetMaximum()[i] - minimum[i])+1);
	      if(!m_OriginSpecified)
		m_Origin[i]= static_cast<typename PointType::ValueType>( minimum[i]);
	    }
	  otbMsgDebugMacro("minimum: "<<minimum);
	  otbMsgDebugMacro("maximum: "<<InputObject->GetBoundingBox()->GetMaximum());
	}
	else
	{
	  for(i=0;i<ObjectDimension;i++)
	  {
	    if(!m_SizeSpecified)
	      m_Size[i] = static_cast<typename SizeType::SizeValueType>(InputObject->GetBoundingBox()->GetMaximum()[i]- InputObject->GetBoundingBox()->GetMinimum()[i]);
	    if(!m_OriginSpecified)
	      m_Origin[i]=static_cast<typename PointType::ValueType>(InputObject->GetBoundingBox()->GetMinimum()[i]);
	  }
	}
      }
  if(!m_SpacingSpecified)
    m_Spacing=InputObject->GetIndexToObjectTransform()->GetScaleComponent();
  
  typename OutputImageType::IndexType index;
  index.Fill(0);
  typename OutputImageType::RegionType region;
  
  // If the size of the output has been explicitly specified, the filter
  // will set the output size to the explicit size, otherwise the size from the spatial
  // object's bounding box will be used as default
  region.SetSize( m_Size );
  region.SetIndex( index );
  
  OutputImage->SetLargestPossibleRegion( region);     //
  OutputImage->SetBufferedRegion( region );           // set the region
  OutputImage->SetRequestedRegion( region );          //
  // If the spacing has been explicitly specified, the filter
  // will set the output spacing to that explicit spacing, otherwise the spacing from
  // the spatial object is used as default.
  OutputImage->SetSpacing(this->m_Spacing);         // set spacing
  OutputImage->SetOrigin(m_Origin);  //   and origin
  OutputImage->Allocate();   // allocate the image
  typedef itk::ImageRegionIteratorWithIndex<OutputImageType> myIteratorType;
  myIteratorType it(OutputImage,region);
  PointType point;

  otbMsgDebugMacro(<<"origin: "<<m_Origin<<" size: "<<m_Size<<" spacing: "<<m_Spacing);


  while(!it.IsAtEnd())
    {
      // ValueAt requires the point to be in physical coordinate i.e
      for(unsigned int i=0;i<ObjectDimension;i++)
	{
	  point[i]=(int) (it.GetIndex()[i]*m_Spacing[i])+m_Origin[i];
	}
      double val =0;
      
      InputObject->ValueAt(point,val,99999);
      if( val)
	{
	  if(m_UseObjectValue)
	    {
	      it.Set(static_cast<ValueType>(val));
	    }
	  else
	    {
	      it.Set(m_InsideValue);
	    }
	}
      else
	{
	  it.Set(m_OutsideValue);
	}
      ++it;
    }
  
  itkDebugMacro(<< "SpatialObjectToImageDrawingFilter::Update() finished");
} // end update function


  template<class TInputSpatialObject, class TOutputImage>
    void
    SpatialObjectToImageDrawingFilter<TInputSpatialObject,TOutputImage>
    ::PrintSelf(std::ostream& os, itk::Indent indent) const
    {
      Superclass::PrintSelf(os, indent);
      os << indent << "Size : " << m_Size << std::endl;
      os << indent << "Children depth : " << m_ChildrenDepth << std::endl;
      os << indent << "Inside Value : " << m_InsideValue << std::endl;
      os << indent << "Outside Value : " << m_OutsideValue << std::endl;
      if(m_UseObjectValue)
	{
	  os << indent << "Using Object Value : ON" << std::endl;
	}
      else
	{
	  os << indent << "Using Object Value : OFF" << std::endl;
	}
    }



} // end namespace otb

#endif
