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
#ifndef __otbSpatialObjectToImageDrawingFilter_txx
#define __otbSpatialObjectToImageDrawingFilter_txx

#include "otbSpatialObjectToImageDrawingFilter.h"
#include "itkImageRegionIteratorWithIndex.h"

namespace otb
{

/** Constructor */
template <class TInputSpatialObject, class TOutputImage>
SpatialObjectToImageDrawingFilter<TInputSpatialObject, TOutputImage>
::SpatialObjectToImageDrawingFilter()
{
  this->SetNumberOfRequiredInputs(1);
  m_ChildrenDepth = 1;
  m_Size.Fill(0);

  for (unsigned int i = 0; i < OutputImageDimension; ++i)
    {
    m_Spacing[i] = 1.0;
    m_Origin[i] = 0.;
    }

  m_InsideValue = 0;
  m_OutsideValue = 0;
  m_UseObjectValue = false;
}

/** Destructor */
template <class TInputSpatialObject, class TOutputImage>
SpatialObjectToImageDrawingFilter<TInputSpatialObject, TOutputImage>
::~SpatialObjectToImageDrawingFilter()
{
}

/** Set the Input SpatialObject */
template <class TInputSpatialObject, class TOutputImage>
void
SpatialObjectToImageDrawingFilter<TInputSpatialObject, TOutputImage>
::SetInput(const InputSpatialObjectType *input)
{

  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<InputSpatialObjectType *>(input));
}

/** Connect one of the operands  */
template <class TInputSpatialObject, class TOutputImage>
void
SpatialObjectToImageDrawingFilter<TInputSpatialObject, TOutputImage>
::SetInput(unsigned int index, const TInputSpatialObject * object)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(index,
                                        const_cast<TInputSpatialObject *>(object));
}

/** Get the input Spatial Object */
template <class TInputSpatialObject, class TOutputImage>
const typename SpatialObjectToImageDrawingFilter<TInputSpatialObject, TOutputImage>::InputSpatialObjectType *
SpatialObjectToImageDrawingFilter<TInputSpatialObject, TOutputImage>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const TInputSpatialObject *>
           (this->itk::ProcessObject::GetInput(0));
}

/** Get the input Spatial Object */
template <class TInputSpatialObject, class TOutputImage>
const typename SpatialObjectToImageDrawingFilter<TInputSpatialObject, TOutputImage>::InputSpatialObjectType *
SpatialObjectToImageDrawingFilter<TInputSpatialObject, TOutputImage>
::GetInput(unsigned int idx)
{
  return static_cast<const TInputSpatialObject *>
           (this->itk::ProcessObject::GetInput(idx));
}

//----------------------------------------------------------------------------
template <class TInputSpatialObject, class TOutputImage>
void
SpatialObjectToImageDrawingFilter<TInputSpatialObject, TOutputImage>
::SetSpacing(const SpacingType& spacing)
{
  unsigned int i;
  for (i = 0; i < TOutputImage::ImageDimension; ++i)
    {
    if ((double) spacing[i] != m_Spacing[i])
      {
      break;
      }
    }
  if (i < TOutputImage::ImageDimension)
    {
    for (i = 0; i < TOutputImage::ImageDimension; ++i)
      {
      m_Spacing[i] = spacing[i];
      }
    this->Modified();
    }
}

//----------------------------------------------------------------------------
template <class TInputSpatialObject, class TOutputImage>
void
SpatialObjectToImageDrawingFilter<TInputSpatialObject, TOutputImage>
::SetSpacing(const double* spacing)
{
  unsigned int i;
  for (i = 0; i < OutputImageDimension; ++i)
    {
    if (spacing[i] != m_Spacing[i])
      {
      break;
      }
    }
  if (i < OutputImageDimension)
    {
    for (i = 0; i < OutputImageDimension; ++i)
      {
      m_Spacing[i] = spacing[i];
      }
    }
}

template <class TInputSpatialObject, class TOutputImage>
void
SpatialObjectToImageDrawingFilter<TInputSpatialObject, TOutputImage>
::SetSpacing(const float* spacing)
{
  unsigned int i;
  for (i = 0; i < OutputImageDimension; ++i)
    {
    if ((double) spacing[i] != m_Spacing[i])
      {
      break;
      }
    }
  if (i < OutputImageDimension)
    {
    for (i = 0; i < OutputImageDimension; ++i)
      {
      m_Spacing[i] = spacing[i];
      }
    }
}

template <class TInputSpatialObject, class TOutputImage>
const double *
SpatialObjectToImageDrawingFilter<TInputSpatialObject, TOutputImage>
::GetSpacing() const
{
  return m_Spacing;
}

//----------------------------------------------------------------------------
template <class TInputSpatialObject, class TOutputImage>
void
SpatialObjectToImageDrawingFilter<TInputSpatialObject, TOutputImage>
::SetOrigin(const PointType& origin)
{
  unsigned int i;
  for (i = 0; i < TOutputImage::ImageDimension; ++i)
    {
    if ((double) origin[i] != m_Origin[i])
      {
      break;
      }
    }
  if (i < TOutputImage::ImageDimension)
    {
    for (i = 0; i < TOutputImage::ImageDimension; ++i)
      {
      m_Origin[i] = origin[i];
      }
    this->Modified();
    }
}

//----------------------------------------------------------------------------
template <class TInputSpatialObject, class TOutputImage>
void
SpatialObjectToImageDrawingFilter<TInputSpatialObject, TOutputImage>
::SetOrigin(const double* origin)
{
  unsigned int i;
  for (i = 0; i < OutputImageDimension; ++i)
    {
    if (origin[i] != m_Origin[i])
      {
      break;
      }
    }
  if (i < OutputImageDimension)
    {
    for (i = 0; i < OutputImageDimension; ++i)
      {
      m_Origin[i] = origin[i];
      }
    this->Modified();
    }
}

template <class TInputSpatialObject, class TOutputImage>
void
SpatialObjectToImageDrawingFilter<TInputSpatialObject, TOutputImage>
::SetOrigin(const float* origin)
{
  unsigned int i;
  for (i = 0; i < OutputImageDimension; ++i)
    {
    if ((double) origin[i] != m_Origin[i])
      {
      break;
      }
    }
  if (i < OutputImageDimension)
    {
    for (i = 0; i < OutputImageDimension; ++i)
      {
      m_Origin[i] = origin[i];
      }
    this->Modified();
    }
}

template <class TInputSpatialObject, class TOutputImage>
const double *
SpatialObjectToImageDrawingFilter<TInputSpatialObject, TOutputImage>
::GetOrigin() const
{
  return m_Origin;
}

template <class TInputSpatialObject, class TOutputImage>
void
SpatialObjectToImageDrawingFilter<TInputSpatialObject, TOutputImage>
::GenerateOutputInformation()
{
  const InputSpatialObjectType * InputObject  = this->GetInput();
  OutputImagePointer             OutputImage = this->GetOutput();

  unsigned int i;
  // Generate the image
  SizeType size;
  double   origine[ObjectDimension];
  InputObject->ComputeBoundingBox();
  bool originspecified = false;
  if (!strcmp(InputObject->GetNameOfClass(), "GroupSpatialObject"))
    {
    ChildrenType children;
    children = InputObject->GetChildren(0);
    IteratorType iter = children->begin();
    IteratorType end = children->end();
    double       minimum[ObjectDimension];

    (*iter)->ComputeBoundingBox();
    for (i = 0; i < ObjectDimension; ++i)
      {
      minimum[i] = (*iter)->GetBoundingBox()->GetMinimum()[i];
      }

    while (iter != end)
      {
      (*iter)->ComputeBoundingBox();
      for (i = 0; i < ObjectDimension; ++i)
        {
        if ((*iter)->GetBoundingBox()->GetMinimum()[i] < minimum[i])
          {
          minimum[i] = (*iter)->GetBoundingBox()->GetMinimum()[i];
          }
        }
      ++iter;
      }

    for (i = 0; i < ObjectDimension; ++i)
      {
      size[i] = (long unsigned int) (InputObject->GetBoundingBox()->GetMaximum()[i] - minimum[i]) + 1;
      origine[i] = (long int) minimum[i];
      originspecified = true;
      }

    itkDebugMacro(
      << "minx= " << minimum[0] << ", miny= " << minimum[0] << ", maxx= " <<
      InputObject->GetBoundingBox()->GetMaximum()[0] << ", maxy= " << InputObject->GetBoundingBox()->GetMaximum()[1]);
    }
  else
    {

    for (i = 0; i < ObjectDimension; ++i)
      {
      size[i] = (long int) (InputObject->GetBoundingBox()->GetMaximum()[i]
                            - InputObject->GetBoundingBox()->GetMinimum()[i]);
      }
    }

  typename OutputImageType::IndexType index;
  index.Fill(0);
  typename OutputImageType::RegionType region;

  // If the size of the output has been explicitly specified, the filter
  // will set the output size to the explicit size, otherwise the size from the spatial
  // object's bounding box will be used as default.

  bool specified = false;
  for (i = 0; i < OutputImageDimension; ++i)
    {
    if (m_Size[i] != 0)
      {
      specified = true;
      break;
      }
    }

  if (specified)
    {
    region.SetSize(m_Size);
    }
  else
    {
    region.SetSize (size);
    m_Size = size;
    }
  region.SetIndex(index);

  OutputImage->SetLargestPossibleRegion(region);
  specified = false;
  for (i = 0; i < OutputImageDimension; ++i)
    {
    if (m_Spacing[i] != 0)
      {
      specified = true;
      break;
      }
    }

  if (specified)
    {
    OutputImage->SetSpacing(this->m_Spacing);           // set spacing
    }
  else
    {
    OutputImage->SetSpacing(InputObject->GetIndexToObjectTransform()->GetScaleComponent());     // set spacing
    m_Spacing[0] = InputObject->GetIndexToObjectTransform()->GetScaleComponent()[0];
    m_Spacing[1] = InputObject->GetIndexToObjectTransform()->GetScaleComponent()[1];
    }

  if (originspecified)
    {
    origine[0] += 0.5 * m_Spacing[0];
    origine[1] += 0.5 * m_Spacing[1];
    OutputImage->SetOrigin(origine);     //   and origin
    m_Origin[0] = OutputImage->GetOrigin()[0];
    m_Origin[1] = OutputImage->GetOrigin()[1];

    }
  else OutputImage->SetOrigin(m_Origin);
}

//----------------------------------------------------------------------------

/** Update */
template <class TInputSpatialObject, class TOutputImage>
void
SpatialObjectToImageDrawingFilter<TInputSpatialObject, TOutputImage>
::GenerateData(void)
{
  itkDebugMacro(<< "SpatialObjectToImageDrawingFilter::Update() called");

  // Get the input and output pointers
  const InputSpatialObjectType * InputObject  = this->GetInput();
  OutputImagePointer             OutputImage = this->GetOutput();

  typename OutputImageType::RegionType region = OutputImage->GetLargestPossibleRegion();
  OutputImage->SetBufferedRegion(region);             // set the region
  OutputImage->SetRequestedRegion(region);            //

  OutputImage->Allocate();   // allocate the image

  typedef itk::ImageRegionIteratorWithIndex<OutputImageType> myIteratorType;

  myIteratorType it(OutputImage, region);

  itk::Point<double, ObjectDimension> point;

  while (!it.IsAtEnd())
    {

    // ValueAt requires the point to be in physical coordinate i.e
    for (unsigned int i = 0; i < ObjectDimension; ++i)
      {
      point[i] = (int) (it.GetIndex()[i] * m_Spacing[i]) + m_Origin[i];
      }
    double val = 0;

    InputObject->ValueAt(point, val, 99999);
    if (m_InsideValue != 0 ||  m_OutsideValue != 0)
      {
      if (val)
        {
        if (m_UseObjectValue)
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
      }
    else
      {
      it.Set(static_cast<ValueType>(val));
      }
    ++it;
    }

  itkDebugMacro(<< "SpatialObjectToImageDrawingFilter::Update() finished");

} // end update function

template<class TInputSpatialObject, class TOutputImage>
void
SpatialObjectToImageDrawingFilter<TInputSpatialObject, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Size : " << m_Size << std::endl;
  os << indent << "Children depth : " << m_ChildrenDepth << std::endl;
  os << indent << "Inside Value : " << m_InsideValue << std::endl;
  os << indent << "Outside Value : " << m_OutsideValue << std::endl;
  if (m_UseObjectValue)
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
