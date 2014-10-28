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

// Adapted from otbImageToPathListAlignFilter.txx

#ifndef __otbRegionImageToRectangularPathListFilter_txx
#define __otbRegionImageToRectangularPathListFilter_txx

#include <iostream>
#include <exception>
#include "otbRegionImageToRectangularPathListFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkPathIterator.h"
#include "itkImageLinearIteratorWithIndex.h"
#include "otbMath.h"
#include "itkNeighborhoodBinaryThresholdImageFunction.h"
#include "itkFloodFilledImageFunctionConditionalIterator.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkConstantBoundaryCondition.h"

namespace otb
{

/** Constructor */
template <class TInputImage, class TOutputPath>
RegionImageToRectangularPathListFilter<TInputImage, TOutputPath>
::RegionImageToRectangularPathListFilter()
{
  this->SetNumberOfRequiredInputs(1);
  m_MinimumFit = 0.0;
  m_MinimumSize = 0.0;
}

/** Destructor */
template <class TInputImage, class TOutputPath>
RegionImageToRectangularPathListFilter<TInputImage, TOutputPath>
::~RegionImageToRectangularPathListFilter()
{
}

/* Algorithm */

template <class TInputImage, class TOutputPath>
void
RegionImageToRectangularPathListFilter<TInputImage, TOutputPath>
::GenerateData(void)
{
  typename InputImageType::SizeType Taille;
  //this->DebugOn();
  //itkDebugMacro(<< "RegionImageToRectangularPathListFilter::GenerateData() called");

  // Get the input and output pointers
  const InputImageType * InputImage   = this->GetInput();
  OutputPathListType *   OutputPath   = this->GetOutput();
  // Generate the image

/* Filter algorithm */

  Taille = InputImage->GetLargestPossibleRegion().GetSize();
  itkDebugMacro(<< "Input image size : " << Taille);

  typename InputImageType::PointType   origin;
  typename InputImageType::SpacingType spacing;
  origin = InputImage->GetOrigin();
  spacing = InputImage->GetSpacing();
  std::cout << "Image origin : " << origin << std::endl;
  std::cout << "Image spacing : " << spacing << std::endl;

  typedef itk::ImageRegionConstIterator<InputImageType> IteratorType;
  IteratorType it(InputImage, InputImage->GetLargestPossibleRegion());
  int pixelCount = 0;
  int regionCount = 0;
  int selectedRegionCount = 0;
  int pixelDebugNumber = 0; //20;
  int regionDebugNumber = 0; //5000; //20;

  typedef itk::ConstNeighborhoodIterator<InputImageType> NeighborhoodIteratorType;
  typename NeighborhoodIteratorType::RadiusType radius;
  radius.Fill(1); // square one-pixel neighborhood
  //2 std::cout << "radius : " << radius << std::endl;
  NeighborhoodIteratorType nit(radius, InputImage, InputImage->GetLargestPossibleRegion());
  NeighborhoodIteratorType nit2(radius, InputImage, InputImage->GetLargestPossibleRegion());
  // Set up the boundary condition to be zero padded (used on input image)
  itk::ConstantBoundaryCondition<TInputImage> BC;
  //BC.SetConstant(itk::NumericTraits<PixelType>::Zero);
  BC.SetConstant(itk::NumericTraits<PixelType>::max());
  nit.OverrideBoundaryCondition(&BC); // assign the boundary condition
  nit2.OverrideBoundaryCondition(&BC); // assign the boundary condition

  // Build a temporary image of chars for use in the flood algorithm
  markerImage = MarkerImageType::New();
  typename MarkerImageType::RegionType markerRegion = InputImage->GetLargestPossibleRegion();

  markerImage->SetLargestPossibleRegion(markerRegion);
  markerImage->SetBufferedRegion(markerRegion);
  markerImage->SetRequestedRegion(markerRegion);
  markerImage->Allocate();
  unsigned char maxValue = itk::NumericTraits<typename MarkerImageType::PixelType>::max();
  markerImage->FillBuffer(maxValue);
  unsigned char zeroValue = itk::NumericTraits<typename MarkerImageType::PixelType>::Zero;
  unsigned char regionValue;
  int           neighbor;
  //markerImage->FillBuffer(itk::NumericTraits<typename MarkerImageType::PixelType>::max());

  //typedef itk::ImageRegionIterator<MarkerImageType> MarkerIteratorType;
  typedef itk::ImageRegionIteratorWithIndex<MarkerImageType> MarkerIteratorType;
  MarkerIteratorType mit(markerImage, markerRegion);

  typedef typename TInputImage::IndexType IndexType;
  std::vector<IndexType>                    regionContainer; // Pb for growing from within loop
  typename std::vector<IndexType>::iterator regionIterator;
  typename std::vector<IndexType>::iterator regionIterator2;
  regionContainer.reserve(Taille[0] * Taille[1]); // to avoid growth problems
  IndexType explorerIndex; // position whose neighbors are to be checked for inclusion in current region

  try {
    OutputPath->Clear();

    for (nit.GoToBegin(), pixelCount = 0; !nit.IsAtEnd(); ++nit)
      {
      pixelCount++;
      if (pixelCount <= pixelDebugNumber)
        {
        std::cout << "Pixel #" << pixelCount << " : " << nit.GetCenterPixel() << std::endl;
        for (neighbor = 1; neighbor <= 7; neighbor += 2) // 4 neighbors : above, left, right, below
          std::cout << "Neighbor #" << neighbor << " : " << nit.GetPixel(neighbor) << std::endl;
        }
      mit.SetIndex(nit.GetIndex());
      if (mit.Get() == maxValue) // pixel not yet processed
        {
        mit.Set(zeroValue);
        regionContainer.clear();
        regionContainer.push_back(nit.GetIndex());
        regionValue = nit.GetCenterPixel();
        regionCount++;
        if (pixelCount <= pixelDebugNumber)
          {
          std::cout << "Starting new region at " << nit.GetIndex() << " with value " << (unsigned short) regionValue <<
          std::endl;
          }
        // Collect all pixels in same region (same value), reachable in 4-connectivity
        for (regionIterator = regionContainer.begin(); regionIterator != /*<*/ regionContainer.end(); ++regionIterator /* regionContainer grows within loop */)
          {
          // add neighbors not yet processed, until whole region has been collected (no "new" neighbor)
          explorerIndex = *regionIterator;
          nit2.SetLocation(explorerIndex);
          if (pixelCount <= pixelDebugNumber) std::cout << "Exploring neighbors of " << explorerIndex << std::endl;

          for (neighbor = 1; neighbor <= 7; neighbor += 2)
            if (nit2.GetPixel(neighbor) == regionValue) // ZZ and not yet processed...
              {
              mit.SetIndex(nit2.GetIndex(neighbor));
              if (mit.Get() == maxValue) // pixel not yet processed
                {
                mit.Set(zeroValue);
                regionContainer.push_back(nit2.GetIndex(neighbor));
                if (pixelCount <= pixelDebugNumber)
                  {
                  std::cout << "Adding " << nit2.GetIndex(neighbor) << std::endl;
                  std::cout << "Added " << regionContainer.back() << std::endl;
                  }
                }
              }
          }
        if (pixelCount <= pixelDebugNumber)
          {
          std::cout << "Region queue (" << regionContainer.size() << " elements) : " << std::endl;
          for (regionIterator = regionContainer.begin(); regionIterator != regionContainer.end(); ++regionIterator)
            std::cout << *regionIterator;
          std::cout << std::endl;
          }

        // Compute variance-covariance matrix of x-y coordinates of region pixels
        double sumX = 0, sumY = 0, sumX2 = 0, sumY2 = 0, sumXY = 0;
        double avgX, avgY, varX, varY, covarXY;
        int    n = regionContainer.size();
        for (regionIterator = regionContainer.begin(); regionIterator != regionContainer.end(); ++regionIterator)
          {
          explorerIndex = *regionIterator;
          sumX += explorerIndex[0];
          sumY += explorerIndex[1];
          sumX2 += explorerIndex[0] * explorerIndex[0];
          sumY2 += explorerIndex[1] * explorerIndex[1];
          sumXY += explorerIndex[0] * explorerIndex[1];
          }
        avgX = (double) sumX / n;
        avgY = (double) sumY / n;
        varX = (double) sumX2 / n - avgX * avgX;
        varY = (double) sumY2 / n - avgY * avgY;
        covarXY = (double) sumXY / n - avgX * avgY;

        // Compute average deviation (or mean absolute deviation) matrix instead, because rectangles resulting from above statistics are too elongated
        double sumAX = 0, sumAY = 0, crossTermAXY = 0;
        double adevX, adevY, adevXY;
        double ax, ay;
        for (regionIterator = regionContainer.begin(); regionIterator != regionContainer.end(); ++regionIterator)
          {
          explorerIndex = *regionIterator;
          ax = vcl_abs(explorerIndex[0] - avgX);
          ay = vcl_abs(explorerIndex[1] - avgY);
          sumAX += ax;
          sumAY += ay;
          crossTermAXY += ax * ay;
          }
        adevX = sumAX / n;
        adevY = sumAY / n;
        adevXY = vcl_sqrt(crossTermAXY) / n;

        // Compute eigenvalues and eigenvectors of variance-covariance matrix (for DIRECTION)
        double delta,
               l1, l2, /* eigenvalues */
               y1, y2, /* eigenvectors y coordinate, for x = 1*/
               x1 = 1, /* first eigenvector x coordinate */
               x2 = 1, /* second eigenvector x coordinate, 1 except in special case when covarXY == 0 */
               alpha /* main direction */;
        delta = (varX - varY) * (varX - varY) + 4 * covarXY * covarXY;
        l1 = (varX + varY + vcl_sqrt(delta)) / 2;
        l2 = (varX + varY - vcl_sqrt(delta)) / 2;
        if (covarXY != 0.0)        // ZZ or larger than a small epsilon ? (eg 10^(-15)...)
          {
          y1 = (l1 - varX) / covarXY; // for x1 = 1
          y2 = (l2 - varX) / covarXY; // for x2 = 1
          }
        else   // matrix was already diagonal
          {
          y1 = 0;
          x2 = 0;
          y2 = 1;
          }

        // Compute eigenvalues and eigenvectors of absolute mean deviation matrix (for PROPORTIONS)
        double adelta,
               al1, al2, /* eigenvalues */
               ay1, ay2, /* eigenvectors y coordinate, for x = 1*/
               ax1 = 1, /* first eigenvector x coordinate */
               ax2 = 1; /* second eigenvector x coordinate, 1 except in special case when covarXY == 0 */
        adelta = (adevX - adevY) * (adevX - adevY) + 4 * adevXY * adevXY;
        al1 = (adevX + adevY + vcl_sqrt(adelta)) / 2;
        al2 = (adevX + adevY - vcl_sqrt(adelta)) / 2;
        if (adevXY != 0.0)        // ZZ or larger than a small epsilon ? (eg 10^(-15)...)
          {
          ay1 = (al1 - adevX) / adevXY; // for x1 = 1
          ay2 = (al2 - adevX) / adevXY; // for x2 = 1
          }
        else   // matrix was already diagonal
          {
          ay1 = 0;
          ax2 = 0;
          ay2 = 1;
          }

        if (y1 != 0) alpha = vcl_atan(1 / y1) * 180 / vnl_math::pi;
        else alpha = 90;
        if (alpha < 0) alpha += 180;  // Conventionnaly given as a value between 0 and 180 degrees

        // Compute equivalent length and width (based on equal area criterion)
        double length, width;
        if (al2 != 0)
          {
          length = vcl_sqrt(vcl_abs(al1 / al2) * n);
          //length = vcl_sqrt(l1 / l2 * n);
          if (al1 != 0) width = vcl_abs(al2 / al1) * length;
          else // l1 == 0 and l2 == 0
            {
            length = width = vcl_sqrt(static_cast<double>(n)); // should happen only when n == 1 anyway
            }
          }
        else
          {
          length = n; // Arbitrary representation for degenerate case
          width = 1;
          }

        // Normalize eigenvectors (for following tests)
        double norm;
        norm = vcl_sqrt(x1 * x1 + y1 * y1);
        assert (norm != 0); //(by construction of eigenvectors)
        if (norm != 0)
          {
          x1 /= norm;
          y1 /= norm;
          }
        norm = vcl_sqrt(x2 * x2 + y2 * y2);
        assert (norm != 0); //(by construction of eigenvectors)
        if (norm != 0)
          {
          x2 /= norm;
          y2 /= norm;
          }

        // Normalize eigenvectors (for following tests) (No : used only for printing values for debugging)
        norm = vcl_sqrt(ax1 * ax1 + ay1 * ay1);
        assert (norm != 0); //(by construction of eigenvectors)
        if (norm != 0)
          {
          ax1 /= norm;
          ay1 /= norm;
          }
        norm = vcl_sqrt(ax2 * ax2 + ay2 * ay2);
        assert (norm != 0); //(by construction of eigenvectors)
        if (norm != 0)
          {
          ax2 /= norm;
          ay2 /= norm;
          }

        // Count the proportion of region pixels contained within rectangle model, to evaluate rectangular fit, or "rectangularity"
        // Rectangle model uses [x1, y1] and [x2, y2] for direction (angle), and size derived from adev matrix
        double vx, vy; // x-y coordinates relative to rectangle center
        double halfLength = length / 2, halfWidth = width / 2; // to write formulas more easily
        int    countWithin = 0; // number of pixels contained within rectangle
        for (regionIterator = regionContainer.begin(); regionIterator != regionContainer.end(); ++regionIterator)
          {
          explorerIndex = *regionIterator;
          vx = explorerIndex[0] - avgX;
          vy = explorerIndex[1] - avgY;
          if (vcl_abs(vx * x1 + vy * y1) <= halfLength
              && vcl_abs(vx * x2 + vy * y2) <= halfWidth) countWithin++;
          }

        if (regionCount <= regionDebugNumber)
          {
          std::cout << std::endl << "Region " << regionCount << " (area = " << n << " pixels)" << std::endl;
          std::cout << "sumX = " << sumX << "; sumY = " << sumY << "; sumX2 = " << sumX2 << "; sumY2 = " << sumY2 <<
          "; sumXY = " << sumXY << std::endl;
          std::cout << "avgX = " << avgX << "; avgY = " << avgY << std::endl;
          std::cout << "varX = " << varX << "; varY = " << varY << "; covarXY = " << covarXY << std::endl;
          std::cout << "adevX = " << adevX << "; adevY = " << adevY << "; adevXY = " << adevXY << std::endl;
          std::cout << "crossTermAXY = " << crossTermAXY << std::endl;
          std::cout << "eigenvalue 1 = " << l1 << "; eigenvalue 2 = " << l2 << std::endl;
          std::cout << "eigenvector 1 = [" << x1 << ", " << y1 << "]; eigenvector 2 = [" << x2 << ", " << y2 << "]" <<
          std::endl;
          std::cout << "A-eigenvalue 1 = " << al1 << "; A-eigenvalue 2 = " << al2 << std::endl;
          std::cout << "A-eigenvector 1 = [" << ax1 << ", " << ay1 << "]; A-eigenvector 2 = [" << ax2 << ", " << ay2 <<
          "]" << std::endl;
          std::cout << "length = " << length << "; width = " << width << std::endl;
          std::cout << "main direction = " << alpha << " degree" << std::endl;
          std::cout << "rectangular fit = " << (float) countWithin / n << std::endl;
          }

        // Build rectangle list, converting image coordinates into physical coordinates
        typedef typename OutputPathType::ContinuousIndexType ContinuousIndexType;

        ContinuousIndexType point;

        OutputPathPointerType path = OutputPathType::New();

        path->Initialize();
        point[0] = (avgX + x1 * halfLength + x2 * halfWidth) * spacing[0] + origin[0];
        point[1] = (avgY + y1 * halfLength + y2 * halfWidth) * spacing[1] + origin[1];
        path->AddVertex(point);
        if (regionCount <= regionDebugNumber)
          std::cout << "corner 1 : [" << point[0] << ", " << point[1] << "]" <<
          std::endl;
        point[0] = (avgX - x1 * halfLength + x2 * halfWidth) * spacing[0] + origin[0];
        point[1] = (avgY - y1 * halfLength + y2 * halfWidth) * spacing[1] + origin[1];
        path->AddVertex(point);
        if (regionCount <= regionDebugNumber)
          std::cout << "corner 2 : [" << point[0] << ", " << point[1] << "]" <<
          std::endl;
        point[0] = (avgX - x1 * halfLength - x2 * halfWidth) * spacing[0] + origin[0];
        point[1] = (avgY - y1 * halfLength - y2 * halfWidth) * spacing[1] + origin[1];
        path->AddVertex(point);
        if (regionCount <= regionDebugNumber)
          std::cout << "corner 3 : [" << point[0] << ", " << point[1] << "]" <<
          std::endl;
        point[0] = (avgX + x1 * halfLength - x2 * halfWidth) * spacing[0] + origin[0];
        point[1] = (avgY + y1 * halfLength - y2 * halfWidth) * spacing[1] + origin[1];
        path->AddVertex(point);
        if (regionCount <= regionDebugNumber)
          std::cout << "corner 4 : [" << point[0] << ", " << point[1] << "]" <<
          std::endl;
        point[0] = (avgX + x1 * halfLength + x2 * halfWidth) * spacing[0] + origin[0];
        point[1] = (avgY + y1 * halfLength + y2 * halfWidth) * spacing[1] + origin[1];
        path->AddVertex(point);

        path->SetValue((double) countWithin / n);

        if ((float) countWithin / n >= m_MinimumFit // keep only rectangles with fit larger than minimumFit
            && n >= m_MinimumSize)                  // and size larger than minimumSize
          {
          OutputPath->PushBack(path);
          selectedRegionCount++;
          }
        }
      }
    } catch (std::exception & e)
    {
    std::cerr << "Caught exception " << e.what() << std::endl;
    }
  std::cout << pixelCount << " pixels seen." << std::endl;
  std::cout << regionCount << " regions processed, " << selectedRegionCount << " regions selected." << std::endl;

  itkDebugMacro(<< "ImageToPathListAlignFilter::GenerateData() finished");

} // end update function

template <class TInputImage, class TOutputPath>
void
RegionImageToRectangularPathListFilter<TInputImage, TOutputPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
