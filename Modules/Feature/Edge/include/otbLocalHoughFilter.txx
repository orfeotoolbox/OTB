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
#ifndef __otbLocalHoughFilter_txx
#define __otbLocalHoughFilter_txx

#include "otbLocalHoughFilter.h"
#include "otbImage.h"

namespace otb
{

/**
 *
 */
template <class TInputImage>
LocalHoughFilter<TInputImage>::LocalHoughFilter() : ImageToLineSpatialObjectListFilter<TInputImage>(),
  m_NumberOfLines(1),
  m_Variance(5),
  m_DiscRadius(10),
  m_Threshold(0)
{
  m_Radius.Fill(20);
  m_Overlap.Fill(0);
}

template <class TInputImage>
typename LocalHoughFilter<TInputImage>::LinePointer
LocalHoughFilter<TInputImage>::
LinePointResearch(LineIterator itLines, InputImageType *image, IndexType origin)
{

  // Get the list of points which consists of two points to represent a
  // straight line
  PointListType&                         pointsList = (*itLines)->GetPoints();
  typename PointListType::const_iterator itPoints = pointsList.begin();

  double u[2];
  u[0] = (*itPoints).GetPosition()[0];
  u[1] = (*itPoints).GetPosition()[1];
  ++itPoints;
  double v[2];
  v[0] = u[0] - (*itPoints).GetPosition()[0];
  v[1] = u[1] - (*itPoints).GetPosition()[1];

  double norm = vcl_sqrt(v[0] * v[0] + v[1] * v[1]);
  v[0] /= norm;
  v[1] /= norm;

  typename InputImageType::RegionType region = image->GetLargestPossibleRegion();

  PointListType ptList;
  LinePointType point;

  typename InputImageType::IndexType localIndex;
  typename InputImageType::IndexType previousIndex;
  typename InputImageType::IndexType nextIndex;

  // The diagonal is the largest distance between two edges of image
  itk::Size<2> size = region.GetSize();
  float        diag = vcl_sqrt((float) (size[0] * size[0] + size[1] * size[1]));

  // Loop on the largest distance to be sure to cover all the image
  // whatever the position of the origin u
  for (int i = static_cast<int>(-diag); i < static_cast<int>(diag); ++i)
    {

    previousIndex[0] = static_cast<long>(u[0] + (i - 1) * v[0]);
    previousIndex[1] = static_cast<long>(u[1] + (i - 1) * v[1]);

    localIndex[0] = static_cast<long>(u[0] + i * v[0]);
    localIndex[1] = static_cast<long>(u[1] + i * v[1]);

    nextIndex[0] = static_cast<long>(u[0] + (i + 1) * v[0]);
    nextIndex[1] = static_cast<long>(u[1] + (i + 1) * v[1]);

    // Check if the local index is inside the image and the previous
    // index is outside or if the local index is inside the image and
    // the next index is outside

    if (((region.IsInside(localIndex)) && (!region.IsInside(previousIndex))) ||
        ((region.IsInside(localIndex)) && (!region.IsInside(nextIndex))))
      {

      point.SetPosition(localIndex[0] + origin[0], localIndex[1] + origin[1]);
      ptList.push_back(point);

      }

    }

  // Check if the line is well defined by two points
  LinePointer line = LineType::New();

  if (ptList.size() == 2)
    {
    line->SetId(0);
    line->SetPoints(ptList);
    line->ComputeBoundingBox();
    }

  ptList.clear();

  return (line);

}

template <class TInputImage>
void
LocalHoughFilter<TInputImage>
::GenerateData()
{

  typename InputImageType::ConstPointer input  = this->GetInput();

  typename LinesListType::Pointer list;
  list = this->GetOutput();

  typename ROIFilterType::Pointer ROIfilter = ROIFilterType::New();

  typename HoughFilterType::LinesListType lines;

  typename HoughFilterType::Pointer houghFilter = HoughFilterType::New();

  // Get image size
  itk::Size<2> size = input->GetLargestPossibleRegion().GetSize();

  // Loop on the input image

  // Direction X
  // x and y must be < size in order to avoid 0-sized regions
  for (unsigned long x = 0; x < size[0] - 1; x += (m_Radius[0] - m_Overlap[0]))
    {

    // Initialize the extract ROI filter in the direction X
    ROIfilter->SetStartX(x);

    // Number of pixels of the local region
    if ((x + m_Radius[0]) < size[0]) ROIfilter->SetSizeX(m_Radius[0]);
    else ROIfilter->SetSizeX(size[0] - x - 1);

    // Direction Y
    for (unsigned long y = 0; y < size[1] - 1; y += (m_Radius[1] - m_Overlap[1]))
      {

      // Initialize the extract ROI filter in the direction Y
      ROIfilter->SetStartY(y);

      if ((y + m_Radius[1]) < size[1]) ROIfilter->SetSizeY(m_Radius[1]);
      else ROIfilter->SetSizeY(size[1] - y - 1);

      // Extract the local region of the input image
      ROIfilter->SetInput(this->GetInput());

      // ----------------------------------------------------
      // Create a copy of the extract ROI filter output image
      // ----------------------------------------------------

      typename InputImageType::Pointer localImage = InputImageType::New();
      typename InputImageType::Pointer filterImage = InputImageType::New();

      ROIfilter->UpdateLargestPossibleRegion();
      ROIfilter->Update();

      filterImage = ROIfilter->GetOutput();

      // Create a new image from the extracted region. The starting
      // index is the corner of the newly generated image (0, 0)

      typename InputImageType::RegionType region;

      IndexType index;

      index[0] = 0;
      index[1] = 0;

      region.SetSize(filterImage->GetLargestPossibleRegion().GetSize());
      region.SetIndex(index);
      localImage->SetRegions(region);
      localImage->SetOrigin(filterImage->GetOrigin());
      localImage->SetSpacing(filterImage->GetSpacing());
      localImage->Allocate();

      typedef itk::ImageRegionIteratorWithIndex<InputImageType>      LocalIteratorType;
      typedef itk::ImageRegionConstIteratorWithIndex<InputImageType> FilterIteratorType;

      LocalIteratorType    localIt(localImage, localImage->GetRequestedRegion());
      FilterIteratorType   filterIt(filterImage,  filterImage->GetRequestedRegion());

      localIt.GoToBegin();
      filterIt.GoToBegin();

      // Copy the filter image in the new local image
      for (localIt.GoToBegin(); !localIt.IsAtEnd(); ++localIt, ++filterIt)
        localIt.Set(static_cast<InputPixelType>(filterIt.Get()));

      // -------------------------------
      // Application of Hough filter
      // -------------------------------

      houghFilter->SetInput(localImage);
      houghFilter->SetNumberOfLines(m_NumberOfLines);
      houghFilter->SetVariance(m_Variance);
      houghFilter->SetDiscRadius(m_DiscRadius);
      houghFilter->SetThreshold(m_Threshold);

      houghFilter->Modified();

      houghFilter->Update();

      // ---------------------------------------
      // Get the list of LineSpatialObject lines
      // ---------------------------------------

      lines = houghFilter->GetLines(m_NumberOfLines);

      LineIterator itLines = lines.begin();

      // Loop on the lines of hough filter list
      while (itLines != lines.end())
        {

        LinePointer line = LineType::New();

        IndexType origin;

        origin[0] = x;
        origin[1] = y;

        // Call the private method that researchs the two points
        // used to define a line
        line = LinePointResearch(itLines, localImage, origin);

        if (line->GetNumberOfPoints() != 0) list->push_back(line);

        ++itLines;

        }

      lines.clear();

      } // end of loop in y direction

    } // end of loop in x direction

}

template <class TInputImage>
void
LocalHoughFilter<TInputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
