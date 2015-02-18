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
#ifndef __otbImageToSIFTKeyPointSetFilter_txx
#define __otbImageToSIFTKeyPointSetFilter_txx

#include "otbImageToSIFTKeyPointSetFilter.h"

#include "itkMatrix.h"
#include "itkProcessObject.h"

namespace otb
{

template <class TInputImage, class TOutputPointSet>
const double
ImageToSIFTKeyPointSetFilter<TInputImage, TOutputPointSet>
::m_HistogramGaussianWeights[73] =
  {
  2.3771112282795414e-07, 3.8860734758633732e-07, 6.2655544995978937e-07, 9.9631120821413786e-07,
  1.5624909838697011e-06, 2.4167238265599128e-06, 3.6865788528530121e-06,
  5.5463469229192623e-06, 8.2295774080263437e-06, 1.2043009749602365e-05, 1.738119136656513e-05,
  2.4740646513897326e-05, 3.4731980398846277e-05, 4.808781565748272e-05,
  6.5664032975164266e-05, 8.8431512984476723e-05, 0.00011745555408931643, 0.00015386047198026335,
  0.00019877765486783745, 0.00025327659834301937, 0.00031828015928190065,
  0.00039446735551235698, 0.00048216931692246382, 0.00058126620279441276, 0.00069109471776775144,
  0.00081037694122312908, 0.00093718121775182789, 0.0010689246133776746,
  0.0012024238404411182,  0.0013339976954896103,  0.0014596192424447215,  0.0015751106965100009,  0.0016763688464699555,
  0.0017596045720966803,  0.0018215772013714365,
  0.0018598035923515156,  0.0018727231637146351,  0.0018598035923515156,  0.0018215772013714365,  0.0017596045720966803,
  0.0016763688464699555,  0.0015751106965100009,
  0.0014596192424447215,  0.0013339976954896103,  0.0012024238404411182,  0.0010689246133776746,
  0.00093718121775182789, 0.00081037694122312908, 0.00069109471776775144,
  0.00058126620279441276, 0.00048216931692246382, 0.00039446735551235698, 0.00031828015928190065,
  0.00025327659834301937, 0.00019877765486783745, 0.00015386047198026335,
  0.00011745555408931643, 8.8431512984476723e-05, 6.5664032975164266e-05, 4.808781565748272e-05,
  3.4731980398846277e-05, 2.4740646513897326e-05, 1.738119136656513e-05,
  1.2043009749602365e-05, 8.2295774080263437e-06, 5.5463469229192623e-06, 3.6865788528530121e-06,
  2.4167238265599128e-06, 1.5624909838697011e-06, 9.9631120821413786e-07,
  6.2655544995978937e-07, 3.8860734758633732e-07, 2.3771112282795414e-07
  };

template <class TInputImage, class TOutputPointSet>
const typename ImageToSIFTKeyPointSetFilter<TInputImage, TOutputPointSet>::OffsetType
ImageToSIFTKeyPointSetFilter<TInputImage, TOutputPointSet>::m_Offsets[8] =
  {
       {{-1, -1}}, //0
       {{-1, 0}}, //1
       {{-1, 1}}, //2
       {{ 0, -1}}, //3
       {{ 0, 1}}, //4
       {{ 1, -1}}, //5
       {{ 1, 0}}, //6
       {{ 1, 1}}, //7
  };

/**
 * Constructor
 */
template <class TInputImage, class TOutputPointSet>
ImageToSIFTKeyPointSetFilter<TInputImage, TOutputPointSet>
::ImageToSIFTKeyPointSetFilter()
{
  m_OctavesNumber = 1;
  m_ScalesNumber = 3;
  m_ExpandFactors = 2;
  m_ShrinkFactors = 2;
  m_Sigma0 = 1.6;
  m_Sigmak = 0;

  m_DoGThreshold = 0.03;
  m_EdgeThreshold = 10;
  m_RatioEdgeThreshold = 0;

  m_ValidatedKeyPoints = 0;
  m_DifferentSamplePoints = 0;
  m_DiscardedKeyPoints = 0;
  m_ChangeSamplePointsMax = 2;

  m_SigmaFactorOrientation = 3;
  m_SigmaFactorDescriptor = 1.5;

  m_GradientMagnitudeThreshold = 0.2;

  m_ExpandFilter = ExpandFilterType::New();
}

template <class TInputImage, class TOutputPointSet>
void
ImageToSIFTKeyPointSetFilter<TInputImage, TOutputPointSet>
::GenerateData()
{
  // First, subsample the input image
  InitializeInputImage();

  InputImagePointerType              input = m_ExpandFilter->GetOutput();
  typename InputImageType::PointType point;
  typename InputImageType::IndexType index;
  index[0] = 0;
  index[1] = 0;

  m_ExpandFilter->GetOutput()->TransformIndexToPhysicalPoint(index, point);

  // for each octave, compute the difference of gaussian
  unsigned int lOctave = 0;
  m_Sigmak = vcl_pow(2, static_cast<double>(1 / (double) (m_ScalesNumber + 1)));
  m_RatioEdgeThreshold = (m_EdgeThreshold + 1) * (m_EdgeThreshold + 1) / m_EdgeThreshold;

  for (lOctave = 0; lOctave != m_OctavesNumber; lOctave++)
    {
    m_DifferentSamplePoints = 0;
    m_DiscardedKeyPoints = 0;

    typename InputImageType::PointType origin0 = input->GetOrigin();

    ComputeDifferenceOfGaussian(input);
    DetectKeyPoint(lOctave);

    // Get the last gaussian for subsample and
    // repeat the process
    m_ShrinkFilter = ShrinkFilterType::New();
    m_ShrinkFilter->SetInput(m_LastGaussian);
    m_ShrinkFilter->SetShrinkFactors(m_ShrinkFactors);
    m_ShrinkFilter->Update();

    input = m_ShrinkFilter->GetOutput();

    typename InputImageType::PointType   origin1;
    typename InputImageType::SpacingType spacing = input->GetSpacing();

    origin1[0] = origin0[0] + spacing[0] * 0.25;
    origin1[1] = origin0[1] + spacing[1] * 0.25;

    input->SetOrigin(origin1);

    otbGenericMsgDebugMacro(<< "ImageToSIFTKeyPointSetFilter:: Number total key points : " \
                            << m_ValidatedKeyPoints);
    otbGenericMsgDebugMacro(<< "ImageToSIFTKeyPointSetFilter:: Number different sample key points per octave : " \
                            << m_DifferentSamplePoints);
    otbGenericMsgDebugMacro(<< "ImageToSIFTKeyPointSetFilter:: Number discarded key points per octave : " \
                            << m_DiscardedKeyPoints);
    otbGenericMsgDebugMacro(<< "ImageToSIFTKeyPointSetFilter:: Resample image factor : " \
                            << m_ShrinkFactors);

    }

  otbGenericMsgDebugMacro(<< "ImageToSIFTKeyPointSetFilter:: Total number key points : " \
                          << this->GetOutput()->GetNumberOfPoints());

}

/**
 * Initialize the input image
 */
template <class TInputImage, class TOutputPointSet>
void
ImageToSIFTKeyPointSetFilter<TInputImage, TOutputPointSet>
::InitializeInputImage()
{
  m_ExpandFilter->SetInput(this->GetInput());
  m_ExpandFilter->SetExpandFactors(m_ExpandFactors);
  m_ExpandFilter->Update();

  typename InputImageType::PointType   origin0 = this->GetInput()->GetOrigin();
  typename InputImageType::PointType   origin1;
  typename InputImageType::SpacingType spacing = m_ExpandFilter->GetOutput()->GetSpacing();

  origin1[0] = origin0[0] - spacing[0] * 0.5;
  origin1[1] = origin0[1] - spacing[1] * 0.5;

  m_ExpandFilter->GetOutput()->SetOrigin(origin1);
}

/**
 * Compute the difference of gaussian
 */
template <class TInputImage, class TOutputPointSet>
void
ImageToSIFTKeyPointSetFilter<TInputImage, TOutputPointSet>
::ComputeDifferenceOfGaussian(InputImagePointerType input)
{
  unsigned int          lScale = 0;
  InputImagePointerType previousGaussian;

  m_DoGList = ImageListType::New();

  m_MagnitudeList = ImageListType::New();
  m_OrientationList = ImageListType::New();
//     m_GaussianWeightOrientationList = ImageListType::New();
//     m_GaussianWeightDescriptorList = ImageListType::New();

  // itkRecursiveGaussian use spacing to compute
  // length sigma gaussian (in mm)
  // sigma = sigma/spacing
  //
  // with multiply by spacing before filtering, length sigma gaussian
  // is compute in pixel
  double xsigman = vcl_abs(input->GetSpacing()[0]) * m_Sigma0;
  double ysigman = vcl_abs(input->GetSpacing()[1]) * m_Sigma0;

  for (lScale = 0; lScale != m_ScalesNumber + 2; lScale++)
    {
    m_XGaussianFilter = GaussianFilterType::New();
    m_YGaussianFilter = GaussianFilterType::New();

    m_XGaussianFilter->SetSigma(xsigman);
    m_XGaussianFilter->SetDirection(0);
    m_XGaussianFilter->SetInput(input);

    m_YGaussianFilter->SetSigma(ysigman);
    m_YGaussianFilter->SetDirection(1);
    m_YGaussianFilter->SetInput(m_XGaussianFilter->GetOutput());

    m_YGaussianFilter->Update();

    m_GradientFilter = GradientFilterType::New();
    m_MagnitudeFilter = MagnitudeFilterType::New();
    m_OrientationFilter = OrientationFilterType::New();

    m_GradientFilter->SetInput(m_YGaussianFilter->GetOutput());
    m_MagnitudeFilter->SetInput(m_GradientFilter->GetOutput());
    m_OrientationFilter->SetInput(m_GradientFilter->GetOutput());

    m_MagnitudeFilter->Update();
    m_OrientationFilter->Update();

    m_MagnitudeList->PushBack(m_MagnitudeFilter->GetOutput());
    m_OrientationList->PushBack(m_OrientationFilter->GetOutput());

    if (lScale > 0)
      {
      m_SubtractFilter = SubtractFilterType::New();
      m_SubtractFilter->SetInput1(m_YGaussianFilter->GetOutput());
      m_SubtractFilter->SetInput2(previousGaussian);
      m_SubtractFilter->Update();
      m_DoGList->PushBack(m_SubtractFilter->GetOutput());
      }

    previousGaussian = m_YGaussianFilter->GetOutput();
    xsigman = xsigman * m_Sigmak;
    ysigman = ysigman * m_Sigmak;
    }
  m_LastGaussian = previousGaussian;
  otbGenericMsgDebugMacro(<< "ImageToSIFTKeyPointSetFilter:: Number of DoG " << m_DoGList->Size());
}

/**
 * Localize key point
 */
template <class TInputImage, class TOutputPointSet>
void
ImageToSIFTKeyPointSetFilter<TInputImage, TOutputPointSet>
::DetectKeyPoint(const unsigned int octave)
{
  // need at least 3 DoG, ie 2 scales
  if (m_ScalesNumber > 1)
    {
    typename ImageListType::Iterator     lIterDoG = m_DoGList->Begin() + 1;
    unsigned int                         lScale = 1;
    OutputPointSetPointerType            outputPointSet = this->GetOutput();
    typename InputImageType::SpacingType spacing = lIterDoG.Get()->GetSpacing();

    while ((lIterDoG + 1) != m_DoGList->End())
      {
      otbGenericMsgDebugMacro(<< "ImageToSIFTKeyPointSetFilter:: octave: " << octave << " scale: " << lScale);
      otbUnusedMacro(octave);
      // Compute max of DoG
      MinimumMaximumCalculatorPointerType lMaximumCalculator = MinimumMaximumCalculatorType::New();
      lMaximumCalculator->SetImage(lIterDoG.Get());
      lMaximumCalculator->Compute();

      typename InputImageType::SizeType lRadius;
      lRadius.Fill(1);
      typename ImageListType::Iterator lIterNext = lIterDoG + 1;
      typename ImageListType::Iterator lIterPrev = lIterDoG - 1;

      NeighborhoodIteratorType lIterCurrent(lRadius,
                                            lIterDoG.Get(),
                                            lIterDoG.Get()->GetLargestPossibleRegion());
      NeighborhoodIteratorType lIterLowerAdjacent(lRadius,
                                                  lIterPrev.Get(),
                                                  lIterPrev.Get()->GetLargestPossibleRegion());
      NeighborhoodIteratorType lIterUpperAdjacent(lRadius,
                                                  lIterNext.Get(),
                                                  lIterNext.Get()->GetLargestPossibleRegion());

      while (!lIterCurrent.IsAtEnd() &&
             !lIterLowerAdjacent.IsAtEnd() &&
             !lIterUpperAdjacent.IsAtEnd())
        {
        // check local min/max
        if (IsLocalExtremum(lIterCurrent,
                            lIterLowerAdjacent,
                            lIterUpperAdjacent))
          {
          VectorPointType lTranslation(PixelType(0));
          OffsetType lOffsetZero = {{0, 0}};

          unsigned int lChangeSamplePoints = 0;
          NeighborhoodIteratorType neighborCurrentScale(lIterCurrent);
          NeighborhoodIteratorType neighborPreviousScale(lIterLowerAdjacent);
          NeighborhoodIteratorType neighborNextScale(lIterUpperAdjacent);

          bool accepted = false;
          bool changed = true;
          while (lChangeSamplePoints < m_ChangeSamplePointsMax &&
                 changed)
            {
            accepted = RefineLocationKeyPoint(neighborCurrentScale,
                                              neighborPreviousScale,
                                              neighborNextScale,
                                              lTranslation);

            OffsetType lTranslateOffset = {{0, 0}};

            lTranslateOffset[0] += static_cast<int>(lTranslation[0] > 0.5);
            lTranslateOffset[0] += -static_cast<int>(lTranslation[0] < -0.5);

            lTranslateOffset[1] += static_cast<int>(lTranslation[1] > 0.5);
            lTranslateOffset[1] += -static_cast<int>(lTranslation[1] < -0.5);

            NeighborhoodIteratorType moveIterator = neighborCurrentScale + lTranslateOffset;

            if (moveIterator.InBounds())
              {
              changed = lTranslateOffset != lOffsetZero;

              // move iterator
              neighborCurrentScale += lTranslateOffset;
              neighborPreviousScale += lTranslateOffset;
              neighborNextScale += lTranslateOffset;
              }
            else
              {
              changed = false;
              }
            ++lChangeSamplePoints;
            }
          if (changed)
            {
            ++m_DifferentSamplePoints;
            }

          // add key point
          if (accepted)
            {
            std::vector<PixelType> lOrientations = ComputeKeyPointOrientations(neighborCurrentScale,
                                                                               lScale,
                                                                               lTranslation[2]);

            // for each main orientation
            for (typename std::vector<PixelType>::iterator orientationIt = lOrientations.begin();
                 orientationIt != lOrientations.end();
                 ++orientationIt)
              {

              std::vector<PixelType> lDescriptors = ComputeKeyPointDescriptor(neighborCurrentScale,
                                                                              lScale,
                                                                              *orientationIt);

              OutputPointType keyPoint;

              lIterDoG.Get()->TransformIndexToPhysicalPoint(neighborCurrentScale.GetIndex(),
                                                            keyPoint);
              keyPoint[0] += spacing[0] * lTranslation[0];
              keyPoint[1] += spacing[1] * lTranslation[1];

              outputPointSet->SetPoint(m_ValidatedKeyPoints, keyPoint);

              OutputPixelType data;
              data.SetSize(128);
              // check this, compute scale
              // real scale = octave*scale
              typename std::vector<PixelType>::const_iterator lIterDescriptor =
                lDescriptors.begin();

              unsigned int lIndDesc = 0;
              while (lIterDescriptor != lDescriptors.end())
                {
                data.SetElement(lIndDesc, *lIterDescriptor);
                ++lIndDesc;
                ++lIterDescriptor;
                }
              outputPointSet->SetPointData(m_ValidatedKeyPoints, data);

              ++m_ValidatedKeyPoints;
              }
            }
          }

        ++lIterCurrent;
        ++lIterLowerAdjacent;
        ++lIterUpperAdjacent;
        }

      ++lIterDoG;
      ++lScale;
      }
    }
}

/**
 * Check local extremum for 26 neighbors
 */
template <class TInputImage, class TOutputPointSet>
bool
ImageToSIFTKeyPointSetFilter<TInputImage, TOutputPointSet>
::IsLocalExtremum(const NeighborhoodIteratorType& currentScale,
                  const NeighborhoodIteratorType& previousScale,
                  const NeighborhoodIteratorType& nextScale) const
{
  bool         isMin = currentScale.GetCenterPixel() < currentScale.GetPixel(m_Offsets[0]);
  bool         isMax = currentScale.GetCenterPixel() > currentScale.GetPixel(m_Offsets[0]);
  bool         isExtremum = isMin || isMax;
  unsigned int lIterOffset = 0;

  while (isExtremum && lIterOffset != 8)
    {
    OffsetType off = m_Offsets[lIterOffset];
    if (isMin)
      {
      isExtremum =
        currentScale.GetCenterPixel() < currentScale.GetPixel(off) &&
        currentScale.GetCenterPixel() < previousScale.GetPixel(off) &&
        currentScale.GetCenterPixel() < nextScale.GetPixel(off);
      }
    else if (isMax)
      {
      isExtremum =
        currentScale.GetCenterPixel() > currentScale.GetPixel(off) &&
        currentScale.GetCenterPixel() > previousScale.GetPixel(off) &&
        currentScale.GetCenterPixel() > nextScale.GetPixel(off);
      }
    lIterOffset++;
    }
  if (isExtremum && isMin)
    {
    isExtremum =
      currentScale.GetCenterPixel() < previousScale.GetCenterPixel() &&
      currentScale.GetCenterPixel() < nextScale.GetCenterPixel();
    }
  else if (isExtremum && isMax)
    {
    isExtremum =
      currentScale.GetCenterPixel() > previousScale.GetCenterPixel() &&
      currentScale.GetCenterPixel() > nextScale.GetCenterPixel();
    }
  return isExtremum;
}

/**
 * Refine location key point
 */
template <class TInputImage, class TOutputPointSet>
bool
ImageToSIFTKeyPointSetFilter<TInputImage, TOutputPointSet>
::RefineLocationKeyPoint(const NeighborhoodIteratorType& currentScale,
                         const NeighborhoodIteratorType& previousScale,
                         const NeighborhoodIteratorType& nextScale,
                         VectorPointType& solution)
{
  bool accepted = true;
  solution = VectorPointType(PixelType(0));

  PixelType dx = 0.5 * (currentScale.GetPixel(m_Offsets[6])
                        - currentScale.GetPixel(m_Offsets[1]));

  PixelType dy = 0.5 * (currentScale.GetPixel(m_Offsets[4])
                        - currentScale.GetPixel(m_Offsets[3]));

  PixelType ds = 0.5 * (nextScale.GetCenterPixel() -
                        previousScale.GetCenterPixel());

  PixelType dxx = currentScale.GetPixel(m_Offsets[6])
                  - 2 * currentScale.GetCenterPixel()
                  + currentScale.GetPixel(m_Offsets[1]);

  PixelType dyy = currentScale.GetPixel(m_Offsets[3])
                  - 2 * currentScale.GetCenterPixel()
                  + currentScale.GetPixel(m_Offsets[4]);

  PixelType dss = previousScale.GetCenterPixel()
                  - 2 * currentScale.GetCenterPixel()
                  + nextScale.GetCenterPixel();

  PixelType dxy = 0.25 * (currentScale.GetPixel(m_Offsets[7])
                          + currentScale.GetPixel(m_Offsets[0])
                          - currentScale.GetPixel(m_Offsets[2])
                          - currentScale.GetPixel(m_Offsets[5]));

  PixelType dxs = 0.25 * (nextScale.GetPixel(m_Offsets[6])
                          + previousScale.GetPixel(m_Offsets[1])
                          - nextScale.GetPixel(m_Offsets[1])
                          - previousScale.GetPixel(m_Offsets[6]));

  PixelType dys = 0.25 * (nextScale.GetPixel(m_Offsets[4])
                          + previousScale.GetPixel(m_Offsets[3])
                          - nextScale.GetPixel(m_Offsets[3])
                          - previousScale.GetPixel(m_Offsets[4]));

  // Compute matrice determinant
  double det = dxx * (dyy * dss - dys * dys) - dxy * (dxy * dss - dxs * dys) + dxs * (dxy * dys - dxs * dyy);

  // Solve system, compute key point offset
  solution[0] = -dx * (dyy * dss - dys * dys) - dy * (dxs * dys - dxy * dss) - ds * (dxy * dys - dyy * dxs);
  solution[1] = -dx * (dys * dxs - dss * dxy) - dy * (dxx * dss - dxs * dxs) - ds * (dxs * dxy - dxx * dys);
  solution[2] = -dx * (dxy * dys - dxs * dyy) - dy * (dxy * dxs - dxx * dys) - ds * (dxx * dyy - dxy * dxy);

  // Compute interpolated value DoG for lSolution (determinant factor)
  PixelType lDoGInterpolated = det * currentScale.GetCenterPixel() +
                               0.5 * (dx * solution[0] +
                                      dy * solution[1] +
                                      ds * solution[2]);

  PixelType lHessianTrace2 = (dxx + dyy) * (dxx + dyy);
  PixelType lHessianDet = dxx * dyy - dxy * dxy;
  // DoG threshold

  accepted = fabs(lDoGInterpolated) >= fabs(det * m_DoGThreshold);

  // Eliminating edge response

  accepted = accepted &&
             fabs(lHessianTrace2) < fabs(m_RatioEdgeThreshold * lHessianDet);

  if (!accepted)
    {
    ++m_DiscardedKeyPoints;
    }
  if (det < 1e-10f)
    {
    solution.Fill(0);
    }
  else
    {
    // normalize offset with determinant of derivative matrix
    solution /= det;
    }
  return accepted;
}

/**
 * Compute key point orientation
 */
template <class TInputImage, class TOutputPointSet>
std::vector<typename ImageToSIFTKeyPointSetFilter<TInputImage, TOutputPointSet>::PixelType>
ImageToSIFTKeyPointSetFilter<TInputImage, TOutputPointSet>
::ComputeKeyPointOrientations(const NeighborhoodIteratorType& currentScale,
                              const unsigned int scale,
                              const PixelType itkNotUsed(translation))
{
  // radius of the neighborhood
  unsigned int radius = 4;
  double       lSigma = scale * 3;
  unsigned int nbBins = 36;
  double       binWidth = 360. / nbBins;

  // initialize the histogram
  std::vector<double> lHistogram(nbBins, 0.), lSmoothedHistogram(nbBins, 0.);

  // Build the region to examine
  typename InputImageType::RegionType            region;
  typename InputImageType::RegionType::SizeType  regionSize;
  typename InputImageType::RegionType::IndexType regionIndex;
  regionSize.Fill(2 * radius + 2);
  region.SetSize(regionSize);
  regionIndex[0] = currentScale.GetIndex()[0] - regionSize[0] / 2;
  regionIndex[1] = currentScale.GetIndex()[1] - regionSize[1] / 2;
  region.SetIndex(regionIndex);

  if (!region.Crop(m_OrientationList->GetNthElement(scale)->GetLargestPossibleRegion()))
    {
    itkExceptionMacro(<< "Region " << region << " is strictly outside the largest possible region!");
    }

  // iterators on the orientation and the magnitude
  RegionIteratorType lIterOrientation(m_OrientationList->GetNthElement(scale), region);
  RegionIteratorType lIterMagn(m_MagnitudeList->GetNthElement(scale), region);
  lIterOrientation.GoToBegin();
  lIterMagn.GoToBegin();

  // For each pixel
  while (!lIterOrientation.IsAtEnd() && !lIterMagn.IsAtEnd())
    {

    // check if pixel is inside the circle of radius
    float dx = lIterMagn.GetIndex()[0] - currentScale.GetIndex()[0];
    float dy = lIterMagn.GetIndex()[1] - currentScale.GetIndex()[1];
    float dist = vcl_sqrt(dx * dx + dy * dy);

    // If we are in the circle
    if (dist < radius)
      {
      //Get the values
      PixelType lOrientation = lIterOrientation.Get();
      PixelType lMagnitude = lIterMagn.Get();

      // Compute the gaussian weight
      double lWeightMagnitude = vcl_exp(-dist * dist / (2 * lSigma * lSigma));

      // Compute the histogram bin index
      unsigned int lHistoIndex = static_cast<unsigned int>(vcl_floor(nbBins * lOrientation / (CONST_2PI)));

      // Update the histogram value
      lHistogram[lHistoIndex] += lMagnitude * lWeightMagnitude;
      }
    ++lIterOrientation;
    ++lIterMagn;
    }

  // Computing smoothed histogram and looking for the maximum and a second maximum within 80% of the first
  double max = 0;
  double secondMax = 0;
  double sum = 0;
  int    maxIndex = 0;
  //int    secondMaxIndex = -1;
  int    j = 0;
  int    i = 0;

  // Smoothing histogram
  for (i = 0; i < static_cast<int>(nbBins); ++i)
    {
    sum = 0;
    for (j = i - nbBins; j < i; ++j)
      {
      sum += lHistogram[i - j - 1] * m_HistogramGaussianWeights[j + nbBins];
      }
    lSmoothedHistogram[i] = sum;
    }

  // looking for maximums
  for (i = 0; i < static_cast<int>(nbBins); ++i)
    {
    if (lSmoothedHistogram[i] > max)
      {
      secondMax = max;
      //secondMaxIndex = maxIndex;
      max = lSmoothedHistogram[i];
      maxIndex = i;
      }
    else if (sum > secondMax)
      {
      secondMax = lSmoothedHistogram[i];
      //secondMaxIndex = i;
      }
    }
  // This structure will hold the located maximums
  std::vector<PixelType> orientations;

  //interpolate orientation maximum
  double x1, x2, x3, y1, y2, y3, a, b, num, denom, orientation;
  x1 = (maxIndex - 1) * binWidth + binWidth / 2;
  y1 = lSmoothedHistogram[(maxIndex - 1) < 0 ? maxIndex - 1 + nbBins : maxIndex - 1];
  x2 = (maxIndex) * binWidth + binWidth / 2;
  y2 = lSmoothedHistogram[maxIndex];
  x3 = (maxIndex + 1) * binWidth + binWidth / 2;
  y3 = lSmoothedHistogram[maxIndex + 1 > static_cast < int > (nbBins) - 1 ? maxIndex + 1 - nbBins : maxIndex + 1];

  denom = x1 * x1 * x2 + x2 * x2 * x3 + x3 * x3 * x1 - x1 * x1 * x3 - x2 * x2 * x1 - x3 * x3 * x2;
  num =   y1 * x2  + y2 * x3  + y3 * x1  - y1 * x3  - y2 * x1  - y3 * x2;

  if (denom == 0 || num == 0)
    {
    // no main orientation, return an empty orientation vector
    return orientations;
    }

  a = num / denom;
  b = ((y1 - y2) - a * (x1 * x1 - x2 * x2)) / (x1 - x2);

  orientation = -b / (2 * a);
  if (orientation < 0)
    {
    orientation += 360;
    }
  else if (orientation >= 360)
    {
    orientation -= 360;
    }

//     orientations.push_back( static_cast<PixelType>(maxIndex*binWidth + binWidth/2));
  orientations.push_back(static_cast<PixelType>(orientation));

  // Second peak is disabled, since it seems to confuse the matching procedure.

  //   if(secondMaxIndex>=0 && secondMax > 0.8 * max)
//       {
//   x1 = (secondMaxIndex-1)*binWidth+binWidth/2;
//   y1 = lSmoothedHistogram[(secondMaxIndex-1)<0 ? secondMaxIndex-1+nbBins : secondMaxIndex-1];
//   x2 = (secondMaxIndex)*binWidth+binWidth/2;
//   y2 = lSmoothedHistogram[secondMaxIndex];
//   x3 = (secondMaxIndex+1)*binWidth+binWidth/2;
//   y3 = lSmoothedHistogram[secondMaxIndex+1>static_cast<int>(nbBins)-1 ? secondMaxIndex+1-nbBins : secondMaxIndex+1];

//   denom = x1*x1*x2 + x2*x2*x3 + x3*x3*x1 - x1*x1*x3 - x2*x2*x1 - x3*x3*x2;
//   num = y1*x2 + y2 * x3 + y3*x1 - y1*x3 - y2*x1 - y3*x2;

//   if(denom == 0 || num == 0)
//     {
//       // no main orientation, return an empty orientation vector
//       return orientations;
//     }

//   a = num/denom;
//   b = ((y1-y2)-a*(x1*x1-x2*x2))/(x1-x2);

//   orientation = -b/(2*a);
//   if(orientation<0)
//     {
//       orientation+=360;
//     }
//   else if(orientation>=360)
//     {
//       orientation-=360;
//     }
// //   orientations.push_back( static_cast<PixelType>(secondMaxIndex*binWidth+binWidth/2));
//   orientations.push_back(static_cast<PixelType>(orientation));
//       }

  return orientations;
}

/**
 * Compute key point descriptor
 */
template <class TInputImage, class TOutputPointSet>
std::vector<typename ImageToSIFTKeyPointSetFilter<TInputImage, TOutputPointSet>::PixelType>
ImageToSIFTKeyPointSetFilter<TInputImage, TOutputPointSet>
::ComputeKeyPointDescriptor(const NeighborhoodIteratorType& currentScale,
                            const unsigned int scale,
                            const PixelType& orientation)
{
  std::vector<PixelType> lHistogram(128, 0.);

  typename InputImageType::RegionType            region;
  typename InputImageType::RegionType::SizeType  regionSize;
  typename InputImageType::RegionType::IndexType regionIndex;

  unsigned int nbHistograms = 4;
  unsigned int nbPixelsPerHistogram = 4;
  unsigned int nbBinsPerHistogram = 8;

  float radius = static_cast<float>(nbHistograms / 2 * nbPixelsPerHistogram);

//     std::cout<<"Radius: "<<radius<<std::endl;

  // 4 region of 4 pixels plus 2 pixels of margin
  regionSize[0] = nbHistograms * nbPixelsPerHistogram + 2;
  regionSize[1] = nbHistograms * nbPixelsPerHistogram + 2;

  // sigma set to one half the width of descriptor window
  // TODO check this
  double lSigma = radius;

  // index - regionSize/2
  regionIndex[0] = currentScale.GetIndex()[0] - regionSize[0] / 2;
  regionIndex[1] = currentScale.GetIndex()[1] - regionSize[1] / 2;

  region.SetIndex(regionIndex);
  region.SetSize(regionSize);

  // Crop with largest region
  if (!region.Crop(m_OrientationList->GetNthElement(scale)->GetLargestPossibleRegion()))
    {
    itkExceptionMacro(<< "Region " << region << " is outside of the largest possible region!");
    }
  RegionIteratorType lIterMagnitude(m_MagnitudeList->GetNthElement(scale), region);
  RegionIteratorType lIterOrientation(m_OrientationList->GetNthElement(scale), region);
  lIterMagnitude.GoToBegin();
  lIterOrientation.GoToBegin();

  // For each pixel in the region
  while (!lIterMagnitude.IsAtEnd() && !lIterOrientation.IsAtEnd())
    {
    // check if pixel is inside the circle of radius
    float dx = lIterMagnitude.GetIndex()[0] - currentScale.GetIndex()[0];
    float dy = lIterMagnitude.GetIndex()[1] - currentScale.GetIndex()[1];
    float dist = vcl_sqrt(dx * dx + dy * dy);

    // If we are in the circle
    if (dist < radius)
      {
      // rotate the pixel location to compensate sift orientation
      float angle = orientation * CONST_PI_180;
      float cosangle = vcl_cos(-angle);
      float sinangle = vcl_sin(-angle);
      float rdx = dx * cosangle - dy * sinangle;
      float rdy = dx * sinangle + dy * cosangle;
      // decide to which histogram the pixel contributes
      unsigned int xHistogramIndex =
        static_cast<unsigned int>(vcl_floor((rdx + radius) / static_cast<float>(nbPixelsPerHistogram)));
      unsigned int yHistogramIndex =
        static_cast<unsigned int>(vcl_floor((rdy + radius) / static_cast<float>(nbPixelsPerHistogram)));

      // decide to which bin of the histogram the pixel contributes
      float compensatedOrientation =  lIterOrientation.Get() - angle;
      if (compensatedOrientation < 0)
        {
        compensatedOrientation += CONST_2PI;
        }
      if (compensatedOrientation >= CONST_2PI)
        {
        compensatedOrientation -= CONST_2PI;
        }
      unsigned int histogramBin =
        static_cast<unsigned int>(vcl_floor(compensatedOrientation * nbBinsPerHistogram / (CONST_2PI)));

      // Compute the wheight of the pixel in the histogram
      double lWeightMagnitude = vcl_exp(-(dist * dist) / (2 * lSigma * lSigma));

      // Compute the global descriptor index
      unsigned int descriptorIndex = yHistogramIndex * nbBinsPerHistogram * nbHistograms
                                     + xHistogramIndex * nbBinsPerHistogram + histogramBin;
      lHistogram[descriptorIndex] += lIterMagnitude.Get() * lWeightMagnitude;
      }

    ++lIterOrientation;
    ++lIterMagnitude;
    }

  // normalize histogram to unit length
  typename std::vector<PixelType>::iterator lIterHisto = lHistogram.begin();
  float                                     lNorm = 0.0;

  while (lIterHisto != lHistogram.end())
    {
    lNorm = lNorm + (*lIterHisto) * (*lIterHisto);
    ++lIterHisto;
    }
  lNorm = vcl_sqrt(lNorm);

  lIterHisto = lHistogram.begin();
  while (lIterHisto != lHistogram.end())
    {
    if (lNorm > 0)
      {
      *lIterHisto = (*lIterHisto) / lNorm;
      }
    else
      {
      *lIterHisto = m_GradientMagnitudeThreshold;
      }

    // threshold gradient magnitude
    if (*lIterHisto > m_GradientMagnitudeThreshold)
      {
      *lIterHisto = m_GradientMagnitudeThreshold;
      }
    ++lIterHisto;
    }

  // renormalize histogram to unit length
  lIterHisto = lHistogram.begin();
  lNorm = 0.0;

  while (lIterHisto != lHistogram.end())
    {
    lNorm = lNorm + (*lIterHisto) * (*lIterHisto);
    ++lIterHisto;
    }
  lNorm = vcl_sqrt(lNorm);

  lIterHisto = lHistogram.begin();
  while (lIterHisto != lHistogram.end())
    {
    *lIterHisto = (*lIterHisto) / lNorm;
    ++lIterHisto;
    }

  return lHistogram;
}

/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputPointSet>
void
ImageToSIFTKeyPointSetFilter<TInputImage, TOutputPointSet>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{

  typedef itk::ProcessObject                                     ProcessObjectType;
  const OutputPointSetType* output = dynamic_cast<const OutputPointSetType*>(this->ProcessObjectType::GetOutput(0));

  Superclass::PrintSelf(os, indent);
  os << indent << "Number of octaves: " << m_OctavesNumber << std::endl;
  os << indent << "Number of scales: " << m_ScalesNumber << std::endl;

  os << indent << "Number of SIFT key points: " << output->GetNumberOfPoints() << std::endl;
}

} // End namespace otb

#endif
