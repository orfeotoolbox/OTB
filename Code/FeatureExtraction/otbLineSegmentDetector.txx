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
#ifndef __otbLineSegmentDetector_txx
#define __otbLineSegmentDetector_txx

#include "otbLineSegmentDetector.h"
#include "itkImageRegionIterator.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkImageConstIterator.h"
#include "itkNeighborhoodIterator.h"
#include "otbPolygon.h"
#include "itkCastImageFilter.h"

#include "otbRectangle.h"
#include "otbRemoteSensingRegion.h"

#include "otbMath.h"

#include "itkMatrix.h"
#include "itkSymmetricEigenAnalysis.h"


extern "C" double dlngam_(double *x);
extern "C" double dbetai_(double *x, double *a, double *b);

namespace otb
{

template <class TInputImage, class TPrecision>
LineSegmentDetector<TInputImage, TPrecision>
::LineSegmentDetector()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);

  m_DirectionsAllowed = 1. / 8.;
  m_Prec = CONST_PI * m_DirectionsAllowed;
  m_Threshold = 5.2;

  /** Compute the modulus and the orientation gradient images */
  m_GradientFilter = GradientFilterType::New();
  m_MagnitudeFilter = MagnitudeFilterType::New();
  m_OrientationFilter = OrientationFilterType::New();

  /** Image to store the pixels used 0:NOTUSED 127:NOTINIT  255:USED*/
  m_UsedPointImage  = LabelImageType::New();
}

template <class TInputImage, class TPrecision>
void
LineSegmentDetector<TInputImage, TPrecision>
::SetInput(const InputImageType *input)
{
  this->Superclass::SetNthInput(0, const_cast<InputImageType *>(input));
}

template <class TInputImage, class TPrecision>
const typename LineSegmentDetector<TInputImage, TPrecision>
::InputImageType *
LineSegmentDetector<TInputImage, TPrecision>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const InputImageType *>(this->Superclass::GetInput(0));
}

template <class TInputImage, class TPrecision>
void
LineSegmentDetector<TInputImage, TPrecision>
::GenerateInputRequestedRegion(void)
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the inputs
  typename InputImageType::Pointer input  =
    const_cast<InputImageType *> (this->GetInput());

  if ( !input )
    {
    return;
    }

  // The input is necessarily the largest possible region.
  // For a streamed implementation, use the StreamingLineSegmentDetector filter
  input->SetRequestedRegionToLargestPossibleRegion();
}

template <class TInputImage, class TPrecision>
void
LineSegmentDetector<TInputImage, TPrecision>
::GenerateData()
{
  if (this->GetInput()->GetRequestedRegion() != this->GetInput()->GetLargestPossibleRegion())
    {
    itkExceptionMacro(<< "Not streamed filter. ERROR : requested region is not the largest possible region.");
    }

  /** Allocate memory for the temporary label Image*/
  m_UsedPointImage->SetRegions(this->GetInput()->GetLargestPossibleRegion());
  m_UsedPointImage->Allocate();
  m_UsedPointImage->FillBuffer(0);

  /** Cast the MagnitudeOutput Image in */
  typedef itk::CastImageFilter<InputImageType, OutputImageType> castFilerType;
  typename castFilerType::Pointer castFilter =  castFilerType::New();
  castFilter->SetInput(this->GetInput());

  /** Compute the modulus and the orientation gradient image */
  m_GradientFilter->SetInput(castFilter->GetOutput());
  m_GradientFilter->SetSigma(0.6);
  m_MagnitudeFilter->SetInput(m_GradientFilter->GetOutput());
  m_OrientationFilter->SetInput(m_GradientFilter->GetOutput());

  m_MagnitudeFilter->Update();
  m_OrientationFilter->Update();

  /** Compute the seed histogram to begin the search*/
  CoordinateHistogramType CoordinateHistogram;
  CoordinateHistogram = this->SortImageByModulusValue(m_MagnitudeFilter->GetOutput());

  /** Search the segments on the image by growing a region from a seed */
  this->LineSegmentDetection(CoordinateHistogram);

  /** Transfert the detected segment to the output vector data */
  this->ComputeRectangles();
}

template <class TInputImage, class TPrecision>
typename LineSegmentDetector<TInputImage, TPrecision>
::CoordinateHistogramType
LineSegmentDetector<TInputImage, TPrecision>
::SortImageByModulusValue(MagnitudeImagePointerType modulusImage)
{
  RegionType largestRegion = this->GetInput()->GetLargestPossibleRegion();

  // Compute the minimum region size
  double logNT = 5. * vcl_log10( static_cast<double>(largestRegion.GetNumberOfPixels()) ) / 2.;
  double log1_p = vcl_log10(m_DirectionsAllowed);
  double rapport = logNT / log1_p;
  m_MinimumRegionSize = static_cast<unsigned int>(-rapport);

  // Computing the min & max of the image
  typedef  itk::MinimumMaximumImageCalculator<OutputImageType> MinMaxCalculatorFilter;
  typename MinMaxCalculatorFilter::Pointer minmaxCalculator =   MinMaxCalculatorFilter::New();

  minmaxCalculator->SetImage(modulusImage);
  minmaxCalculator->ComputeMinimum();
  OutputPixelType min = minmaxCalculator->GetMinimum();
  minmaxCalculator->ComputeMaximum();
  OutputPixelType max = minmaxCalculator->GetMaximum();

  /** Compute the threshold on the gradient*/
  m_Threshold = m_Threshold * ((max - min) / 255.);     // threshold normalized with min & max of the values

  /** Computing the length of the bins*/
  unsigned int NbBin = 1024;
  double       lengthBin = static_cast<double>((max - min)) / static_cast<double>(NbBin-1);
  CoordinateHistogramType  tempHisto(NbBin);  /** Initializing the histogram */

  // New region : without boundaries
  RegionType     region;
  SizeType       size = modulusImage->GetRequestedRegion().GetSize();
  InputIndexType id = modulusImage->GetRequestedRegion().GetIndex();

  // Don't take in carre the boudary of the image.
  // Special cases for streamed call
  if (modulusImage->GetRequestedRegion().GetIndex()[0] == 0)
    {
    id[0]++;
    size[0]--;
    if (modulusImage->GetRequestedRegion().GetSize()[0] + modulusImage->GetRequestedRegion().GetIndex()[0] ==
        largestRegion.GetSize(0))
      size[0]--;
    }
  else if (modulusImage->GetRequestedRegion().GetSize()[0] + modulusImage->GetRequestedRegion().GetIndex()[0] ==
      largestRegion.GetSize(0))
    {
    size[0]--;
    }

  if (modulusImage->GetRequestedRegion().GetIndex()[1] == 0)
    {
    id[1]++;
    size[1]--;
    if (modulusImage->GetRequestedRegion().GetSize()[1] + modulusImage->GetRequestedRegion().GetIndex()[1] ==
        largestRegion.GetSize(1)) size[1]--;
    }
  else if (modulusImage->GetRequestedRegion().GetSize()[1] + modulusImage->GetRequestedRegion().GetIndex()[1] ==
      largestRegion.GetSize(1))
    {
    size[1]--;
    }

  region.SetIndex(id);
  region.SetSize(size);

  itk::ImageRegionIterator<OutputImageType> it(modulusImage, region);

  it.GoToBegin();
  while (!it.IsAtEnd())
    {
    unsigned int    bin = static_cast<unsigned int> (static_cast<double>(it.Value()-min) / lengthBin);

    // Highlights bug 498
    assert(bin<NbBin);

    if (it.Value() - m_Threshold > 1e-10) tempHisto[NbBin - bin - 1].push_back(it.GetIndex());
    else SetPixelToUsed(it.GetIndex());

    ++it;
    }

  return tempHisto;
}

/**************************************************************************************************************/
/**
 * Method used to search the segments
 */
template <class TInputImage, class TPrecision>
void
LineSegmentDetector<TInputImage, TPrecision>
::LineSegmentDetection(CoordinateHistogramType& CoordinateHistogram)
{
  /** Begin the search of the segments*/
  CoordinateHistogramIteratorType ItCoordinateList = CoordinateHistogram.begin();

  while (ItCoordinateList != CoordinateHistogram.end())
    {
    typename IndexVectorType::iterator ItIndexVector = (*ItCoordinateList).begin();
    while (ItIndexVector != (*ItCoordinateList).end())
      {
      InputIndexType index = *(ItIndexVector);

      /** If the point is not yet computed */
      if (this->IsNotUsed(index))
        {
        IndexVectorType region;
        double          regionAngle = 0.;

        bool fail = GrowRegion(index, region, regionAngle);

        if (!fail)
          {
          //region -> rectangle
          RectangleType rectangle = Region2Rectangle(region, regionAngle);

          //ImprovRectangle(&rectangle)
          double nfa = ImproveRectangle(rectangle);

          double density =  (double)region.size() /
            ( vcl_sqrt((rectangle[2]-rectangle[0])*(rectangle[2]-rectangle[0])
                       +(rectangle[3]-rectangle[1])*(rectangle[3]-rectangle[1])) * rectangle[4] );
          if (density < 0.7)
            {
            nfa = -1;
            //std::cout << "Density = " << density << std::endl;
            }

          //if NFA(ImprovRect(rec)) > 0.
          if (nfa > 0.)
            {
            m_RectangleList.push_back(rectangle);
            }
          else
            {
            SetRegionToNotIni(region);
            }
          }
        }
      ++ItIndexVector;
      }
    ++ItCoordinateList;
    }
}

/**************************************************************************************************************/
/**
 * Method used to compute rectangles from region
 * Here you can access the NFA for each region
 */
template <class TInputImage, class TPrecision>
int
LineSegmentDetector<TInputImage, TPrecision>
::ComputeRectangles()
{
  // Output
  this->GetOutput(0)->SetMetaDataDictionary(this->GetInput()->GetMetaDataDictionary());
  // Retrieving root node
  typename DataNodeType::Pointer root = this->GetOutput(0)->GetDataTree()->GetRoot()->Get();
  // Create the document node
  typename DataNodeType::Pointer document = DataNodeType::New();
  document->SetNodeType(otb::DOCUMENT);
  // Adding the layer to the data tree
  this->GetOutput(0)->GetDataTree()->Add(document, root);
  // Create the folder node
  typename DataNodeType::Pointer folder = DataNodeType::New();
  folder->SetNodeType(otb::FOLDER);
  // Adding the layer to the data tree
  this->GetOutput(0)->GetDataTree()->Add(folder, document);
  this->GetOutput(0)->SetProjectionRef(this->GetInput()->GetProjectionRef());

  SpacingType spacing = this->GetInput()->GetSpacing();
  OriginType  origin  = this->GetInput()->GetOrigin();

  /** store the lines*/
  RectangleListTypeIterator itRec = m_RectangleList.begin();
  while (itRec != m_RectangleList.end())
    {
    VertexType start, end;

    start[0] = origin[0]
      + static_cast<TPrecision>((*itRec)[0]) * spacing[0];
    start[1] = origin[1]
      + static_cast<TPrecision>((*itRec)[1]) * spacing[1];

    end[0] = origin[0]
      + static_cast<TPrecision>((*itRec)[2]) * spacing[0];
    end[1] = origin[1]
      + static_cast<TPrecision>((*itRec)[3]) * spacing[1];

    typename DataNodeType::Pointer CurrentGeometry = DataNodeType::New();
    CurrentGeometry->SetNodeId("FEATURE_LINE");
    CurrentGeometry->SetNodeType(otb::FEATURE_LINE);
    typename LineType::Pointer line = LineType::New();
    CurrentGeometry->SetLine(line);
    this->GetOutput(0)->GetDataTree()->Add(CurrentGeometry, folder);
    CurrentGeometry->GetLine()->AddVertex(start);
    CurrentGeometry->GetLine()->AddVertex(end);

    ++itRec;
    }

  return EXIT_SUCCESS;
}

/**************************************************************************************************************/
/**
 *  Copy a rectangle rSrc in a rectangle rDst
 */
template <class TInputImage, class TPrecision>
void
LineSegmentDetector<TInputImage, TPrecision>
::CopyRectangle(RectangleType& rDst, RectangleType& rSrc) const
{
  RectangleIteratorType itSrc = rSrc.begin();

  while (itSrc != rSrc.end())
    {
    rDst.push_back(*(itSrc));
    ++itSrc;
    }
}

/**************************************************************************************************************/
/**
 * Method used to compute improve the NFA of The rectangle by changing
 * the components of the rectangle
 */
template <class TInputImage, class TPrecision>
double
LineSegmentDetector<TInputImage, TPrecision>
::ImproveRectangle(RectangleType &rec) const
{
  int           n = 0;
  double        nfa_new;
  double        delta = 0.5;
  double        delta_2 = delta / 2.0;
  RectangleType r;

  double NFA = this->ComputeRectNFA(rec);

  if (NFA > 0.) return NFA;

  /*Try to improve the precison of the oriented */
  CopyRectangle(r, rec);
  for (n = 0; n < 5; ++n)
    {
    r[7] /= 2.0;
    r[6] = CONST_PI * r[7];                // prec = rec[6]
    nfa_new = this->ComputeRectNFA(r);
    if (nfa_new > NFA)
      {
      NFA = nfa_new;
      CopyRectangle(rec, r);
      }
    }

  if (NFA > 0.) return NFA;

  /*Try to improve the width of the rectangle*/
  CopyRectangle(r, rec);
  for (n = 0; n < 5; ++n)
    {
    r[4] -= delta;     //r[4] is stored as the width
    nfa_new = this->ComputeRectNFA(r);
    if (nfa_new > NFA)
      {
      NFA = nfa_new;
      CopyRectangle(rec, r);
      }
    }
  if (NFA > 0.) return NFA;

  /*Try to improve the extremity of the segments*/
  CopyRectangle(r, rec);
  for (n = 0; n < 5; ++n)
    {
    if ((r[4] - delta) >= 0.5)
      {
      r[0] += -vcl_sin(r[5]) * delta_2;
      r[1] +=  vcl_cos(r[5]) * delta_2;
      r[2] += -vcl_sin(r[5]) * delta_2;
      r[3] +=  vcl_cos(r[5]) * delta_2;
      r[4] -= delta;

      nfa_new = this->ComputeRectNFA(r);
      if (nfa_new > NFA)
        {
        NFA = nfa_new;
        CopyRectangle(rec, r);
        }
      }
    }
  if (NFA > 0.) return NFA;

  CopyRectangle(r, rec);
  for (n = 0; n < 5; ++n)
    {
    if ((r[4] - delta) >= 0.5)
      {
      r[0] -= -vcl_sin(r[5]) * delta_2;
      r[1] -=  vcl_cos(r[5]) * delta_2;
      r[2] -= -vcl_sin(r[5]) * delta_2;
      r[3] -=  vcl_cos(r[5]) * delta_2;
      r[4] -= delta;

      nfa_new = this->ComputeRectNFA(r);
      if (nfa_new > NFA)
        {
        NFA = nfa_new;
        CopyRectangle(rec, r);
        }
      }
    }
  if (NFA > 0.) return NFA;

  /*Try to improve the precision again */
  CopyRectangle(r, rec);
  for (n = 0; n < 5; ++n)
    {
    r[7] /= 2.0;
    r[6] = CONST_PI * r[7];                // prec = rec[]
    nfa_new = this->ComputeRectNFA(r);
    if (nfa_new > NFA)
      {
      NFA = nfa_new;
      CopyRectangle(rec, r);
      }
    }

  return NFA;

}

/**************************************************************************************************************/
/**
 * Method IsNotUsed : Determine if a point was NOTUSED or not. search in the m_LabelImage
 */
template <class TInputImage, class TPrecision>
bool
LineSegmentDetector<TInputImage, TPrecision>
::IsNotUsed(InputIndexType& index) const
{
  bool isNotUsed = false;

  typedef itk::ImageConstIterator<LabelImageType> ImageIteratorType;
  RegionType     region = m_UsedPointImage->GetLargestPossibleRegion();
  InputIndexType indexRef = region.GetIndex();
  ImageIteratorType itLabel(m_UsedPointImage, region);
  itLabel.GoToBegin();

  if (m_UsedPointImage->GetLargestPossibleRegion().IsInside(index))
    {
    itLabel.SetIndex(index);
    if (itLabel.Get() == 0) isNotUsed = true;
    }
  else
    {
    itkExceptionMacro(<< "Can't access to index " << index << ", outside the image largest region (" << indexRef
                      << ", " << region.GetSize() << ")");
    }

  return isNotUsed;
}
/**************************************************************************************************************/
/**
 * Method IsUsed : Determine if a point was USED or not. search in the m_LabelImage
 */
template <class TInputImage, class TPrecision>
bool
LineSegmentDetector<TInputImage, TPrecision>
::IsUsed(InputIndexType& index) const
{
  bool isUsed = false;

  typedef itk::ImageConstIterator<LabelImageType> ImageIteratorType;
  RegionType     region = m_UsedPointImage->GetLargestPossibleRegion();
  InputIndexType indexRef = region.GetIndex();
  ImageIteratorType itLabel(m_UsedPointImage, region);
  itLabel.GoToBegin();

  if (m_UsedPointImage->GetLargestPossibleRegion().IsInside(index))
    {
    itLabel.SetIndex(index);
    if (itLabel.Get() == 255) isUsed = true;
    }
  else
    {
    itkExceptionMacro(<< "Can't access to index " << index << ", outside the image largest region (" <<indexRef
                        << ", " << region.GetSize() << ")");
    }

  return isUsed;
}

/**************************************************************************************************************/
/**
 * Method IsNotIni : Determine if a point was NOTINI or not. search in the m_LabelImage
 */
template <class TInputImage, class TPrecision>
bool
LineSegmentDetector<TInputImage, TPrecision>
::IsNotIni(InputIndexType& index) const
{
  bool isNotIni = false;

  typedef itk::ImageConstIterator<LabelImageType> ImageIteratorType;
  RegionType     region = m_UsedPointImage->GetLargestPossibleRegion();
  InputIndexType indexRef = region.GetIndex();
  ImageIteratorType itLabel(m_UsedPointImage, region);
  itLabel.GoToBegin();

  if (m_UsedPointImage->GetLargestPossibleRegion().IsInside(index))
    {
    itLabel.SetIndex(index);
    if (itLabel.Get() == 127) isNotIni = true;
    }
  else
    {
    itkExceptionMacro(<< "Can't access to index " << index << ", outside the image largest region (" << indexRef
                      << ", " << region.GetSize() << ")");
    }

  return isNotIni;
}

/**************************************************************************************************************/

/**
 * Method SetPixelToUsed : Set a pixel to USED
 */
template <class TInputImage, class TPrecision>
void
LineSegmentDetector<TInputImage, TPrecision>
::SetPixelToUsed(InputIndexType index)
{
  typedef itk::NeighborhoodIterator<LabelImageType> NeighborhoodLabelIteratorType;
  typename NeighborhoodLabelIteratorType::SizeType radiusLabel;
  radiusLabel.Fill(0);
  NeighborhoodLabelIteratorType                       itLabel(radiusLabel, m_UsedPointImage,
                                                              m_UsedPointImage->GetRequestedRegion());
  itLabel.SetLocation(index);
  itLabel.SetCenterPixel(255);     // 255 : Set the point status to : Used Point
}

/**************************************************************************************************************/

/**
 * Method SetPixelToNotIni : Set a pixel to NOTINI
 */
template <class TInputImage, class TPrecision>
void
LineSegmentDetector<TInputImage, TPrecision>
::SetPixelToNotIni(InputIndexType index)
{
  typedef itk::NeighborhoodIterator<LabelImageType> NeighborhoodLabelIteratorType;
  typename NeighborhoodLabelIteratorType::SizeType radiusLabel;
  radiusLabel.Fill(0);
  NeighborhoodLabelIteratorType                       itLabel(radiusLabel, m_UsedPointImage,
                                                              m_UsedPointImage->GetRequestedRegion());
  itLabel.SetLocation(index);
  itLabel.SetCenterPixel(127);     // 127 : Set the point status to : Not Ini Point
}

/**************************************************************************************************************/
/**
 * Method SetRegionToNotIni : Set a region pixels to NOTINI
 */
template <class TInputImage, class TPrecision>
void
LineSegmentDetector<TInputImage, TPrecision>
::SetRegionToNotIni(IndexVectorType region)
{
  IndexVectorIteratorType it = region.begin();
  while (it != region.end())
    {
    this->SetPixelToNotIni(*it);
    it ++;
    }
}

/**************************************************************************************************************/
/**
 * Method GrowRegion : From a seed grow the region to find a connected region with the same orientation
 * within a precision (m_Prec)
 */
template <class TInputImage, class TPrecision>
bool
LineSegmentDetector<TInputImage, TPrecision>
::GrowRegion(InputIndexType index, IndexVectorType& region, double& regionAngle)
{
  /** Add the point to the used list point*/
  this->SetPixelToUsed(index);

  /** Neighborhooding */
  typedef itk::ConstNeighborhoodIterator<OutputImageType> NeighborhoodIteratorType;
  typename NeighborhoodIteratorType::SizeType radius;
  radius.Fill(1);
  NeighborhoodIteratorType itNeigh(radius, m_MagnitudeFilter->GetOutput(),
                                   m_MagnitudeFilter->GetOutput()->GetRequestedRegion());
  NeighborhoodIteratorType itNeighDir(radius, m_OrientationFilter->GetOutput(),
                                      m_OrientationFilter->GetOutput()-> GetRequestedRegion());

  /** Vector where to store the point belonging to the current region*/
  unsigned int    neighSize  = itNeigh.GetSize()[0] * itNeigh.GetSize()[1];

  /** Add the first point to the region */
  region.push_back(index);
  double sumX = 0.;
  double sumY = 0.;

  /**
   * Loop for searching regions
   */
  for (unsigned int cpt = 0; cpt < region.size(); ++cpt)
    {
    itNeigh.SetLocation(region[cpt]);
    itNeighDir.SetLocation(region[cpt]);
    sumX += vcl_cos(*(itNeighDir.GetCenterValue()));
    sumY += vcl_sin(*(itNeighDir.GetCenterValue()));
    regionAngle = vcl_atan2(sumY, sumX);

    unsigned int s = 0;
    while (s < neighSize)
      {
      InputIndexType NeighIndex = itNeigh.GetIndex(s);
      double         angleComp  = itNeighDir.GetPixel(s);

      if (this->GetInput()->GetLargestPossibleRegion().IsInside(NeighIndex))    /** Check if the index is inside the image*/
        {
        if ((this->IsNotUsed(NeighIndex) || this->IsNotIni(NeighIndex)) && this->IsAligned(angleComp, regionAngle, m_Prec))
          {
          this->SetPixelToUsed(NeighIndex);
          region.push_back(NeighIndex);
          }
        }
      ++s;
      }
    } /** End Searching loop*/

  unsigned int nbPixels = this->GetInput()->GetLargestPossibleRegion().GetNumberOfPixels();
  if (region.size() > m_MinimumRegionSize && region.size() < nbPixels / 4)
    {
    return EXIT_SUCCESS;
    }
  else
    {
    return EXIT_FAILURE;
    }
}

/**************************************************************************************************************/
/**
 *  The method atan2 gives values of angles modulo PI, put the angle in a rang [0, Pi]
 */
template <class TInputImage, class TPrecision>
bool
LineSegmentDetector<TInputImage, TPrecision>
::IsAligned(double Angle, double regionAngle, double prec) const
{
  double diff = Angle - regionAngle;

  if (diff < 0.0) diff = -diff;
  if (diff > 1.5 * CONST_PI)
    {
    diff -= CONST_2PI;
    if (diff < 0.0) diff = -diff;
    }

  return diff < prec;
}

/**************************************************************************************************************/
/**
 *  compute the best rectangle possible that
 */
template <class TInputImage, class TPrecision>
typename LineSegmentDetector<TInputImage, TPrecision>
::RectangleType
LineSegmentDetector<TInputImage, TPrecision>
::Region2Rectangle(IndexVectorType region, double regionAngle)
{
  /** Local Variables*/
  double weight = 0., sumWeight = 0.;
  double x = 0., y = 0.;
  double l_min = 0., l_max = 0., l = 0., w = 0., w_min = 0., w_max = 0.;

  /** Neighborhooding again*/
  typedef itk::ConstNeighborhoodIterator<OutputImageType> NeighborhoodIteratorType;
  typename NeighborhoodIteratorType::SizeType radius;
  radius.Fill(0);
  NeighborhoodIteratorType                                itNeigh(radius, m_MagnitudeFilter->GetOutput(),
                                                                  m_MagnitudeFilter->GetOutput()->GetRequestedRegion());

  /** Computing the center of the rectangle*/
  IndexVectorIteratorType it = region.begin();
  while (it != region.end())
    {
    itNeigh.SetLocation(*it);
    weight = *itNeigh.GetCenterValue();
    x += static_cast<double>((*it)[0]) * weight;
    y += static_cast<double>((*it)[1]) * weight;
    sumWeight +=  weight;
    ++it;
    }
  if (sumWeight < 1e-10)
    {
    x = 0.; y = 0.;
    }
  else
    {
    x /= sumWeight;
    y /= sumWeight;
    }

  /** Compute the orientation of the region*/
  double theta = this->ComputeRegionOrientation(region, x, y, regionAngle);

  /* Length & Width of the rectangle **/
  typedef std::vector<MagnitudePixelType> MagnitudeVector;

  RegionType largestRegion = this->GetInput()->GetLargestPossibleRegion();
  unsigned int Diagonal =
    static_cast<unsigned int>(vnl_math_hypot(static_cast<double>(largestRegion.GetSize(1)), static_cast<double>(
        largestRegion.GetSize(0))) + 2);

  MagnitudeVector sum_l(2*Diagonal, itk::NumericTraits<MagnitudePixelType>::Zero);
  MagnitudeVector sum_w(2*Diagonal, itk::NumericTraits<MagnitudePixelType>::Zero);

  double dx = vcl_cos(theta);
  double dy = vcl_sin(theta);

  it = region.begin();
  while (it != region.end())
    {
    itNeigh.SetLocation(*it);
    weight = *itNeigh.GetCenterValue();
    l =  (static_cast<double>((*it)[0]) - x) * dx + (static_cast<double>((*it)[1]) - y) * dy;
    w = -(static_cast<double>((*it)[0]) - x) * dy + (static_cast<double>((*it)[1]) - y) * dx;

    if (l < l_min) l_min = l; if (l > l_max) l_max = l;
    if (w < w_min) w_min = w; if (w > w_max) w_max = w;

    sum_l[static_cast < int > (vcl_floor(l) + 0.5) + Diagonal] +=  static_cast<MagnitudePixelType>(weight);
    sum_w[static_cast < int > (vcl_floor(w) + 0.5) + Diagonal] +=  static_cast<MagnitudePixelType>(weight);

    ++it;
    }

  /** Thresholdinq the width and the length*/
  double sum_th = 0.01 * sumWeight; /* weight threshold for selecting region */
  double s = 0.;
  int    i = 0;

  for (s = 0.0, i = static_cast<int>(l_min); s < sum_th && i <= static_cast<int>(l_max); ++i)
    s += sum_l[Diagonal + i];

  double lb = (static_cast<double>(i - 1) - 0.5);

  for (s = 0.0, i = static_cast<int>(l_max); s < sum_th && i >= static_cast<int>(l_min); i--)
    s += sum_l[Diagonal  + i];
  double lf =  (static_cast<double>(i + 1) + 0.5);

  for (s = 0.0, i = static_cast<int>(w_min); s < sum_th && i <= static_cast<int>(w_max); ++i)
    s += sum_w[Diagonal + i];

  double wr = (static_cast<double>(i - 1) - 0.5);

  for (s = 0.0, i = static_cast<int>(w_max); s < sum_th && i >= static_cast<int>(w_min); i--)
    s += sum_w[Diagonal  + i];

  double wl = (static_cast<double>(i + 1) + 0.5);

  /** Finally store the rectangle in vector this way :
   *  vec[0] = x1
   *  vec[1] = y1
   *  vec[2] = x2
   *  vec[3] = y2
   *  vec[4] = width
   *  vec[5] = theta
   *  vec[6] = prec = Pi/8
   *  vec[7] = p =  1/8
   */
  RectangleType          rec(8, 0.);       // Definition of a
                                           // rectangle : 8 components

  if (vcl_abs(wl - wr)
      - vcl_sqrt( static_cast<double> (largestRegion.GetSize(0) * largestRegion.GetSize(0) +
                                       largestRegion.GetSize(1) * largestRegion.GetSize(1)))
      < 1e-10 )
    {
    rec[0] = (x + lb * dx > 0) ? x + lb * dx : 0.;
    rec[1] = (y + lb * dy > 0) ? y + lb * dy : 0.;
    rec[2] = (x + lf * dx > 0) ? x + lf * dx : 0.;
    rec[3] = (y + lf * dy > 0) ? y + lf * dy : 0;
    rec[4] = wl - wr;
    rec[5] = theta;
    rec[6] = m_Prec;
    rec[7] = m_DirectionsAllowed;

    if (rec[4] - 1. < 1e-10) rec[4] = 1.;

    }
  return rec;
}

/**************************************************************************************************************/
/**
 * Compute the orientation of a region , using an eigen Value analysis.
 */

template <class TInputImage, class TPrecision>
double
LineSegmentDetector<TInputImage, TPrecision>
::ComputeRegionOrientation(IndexVectorType region, double x, double y, double angleRegion) const
{

  double Ixx = 0.0;
  double Iyy = 0.0;
  double Ixy = 0.0;
  double theta = 0.;
  double weight = 0., sum = 0.;

  /** Neighborhooding again*/
  typedef itk::ConstNeighborhoodIterator<OutputImageType> NeighborhoodIteratorType;
  typename NeighborhoodIteratorType::SizeType radius;
  radius.Fill(0);
  NeighborhoodIteratorType itNeigh(radius, m_MagnitudeFilter->GetOutput(),
                                   m_MagnitudeFilter->GetOutput()->GetRequestedRegion());

  /** Computing the center of the rectangle*/
  IndexVectorIteratorType it = region.begin();
  while (it != region.end())
    {
    itNeigh.SetLocation(*it);
    weight = *itNeigh.GetCenterValue();
    double Iyy2 = static_cast<double>((*it)[0]) - x;
    double Ixx2 = static_cast<double>((*it)[1]) - y;

    Ixx += Ixx2 * Ixx2 * weight;
    Iyy += Iyy2 * Iyy2 * weight;
    Ixy -= Ixx2 * Iyy2 * weight;
    sum +=  weight;
    ++it;
    }

  /** using te itk Eigen analysis*/
  typedef itk::Matrix<double, 2, 2> MatrixType;
  typedef std::vector<double>       MatrixEigenType;
  MatrixType Inertie, eigenVector;
  MatrixEigenType  eigenMatrix(2, 0.);
  Inertie[1][1] = Ixx;
  Inertie[0][0] = Iyy;
  Inertie[0][1] = Ixy;
  Inertie[1][0] = Ixy;

  typedef itk::SymmetricEigenAnalysis<MatrixType, MatrixEigenType> EigenAnalysisType;
  EigenAnalysisType eigenFilter(2);
  eigenFilter.ComputeEigenValuesAndVectors(Inertie, eigenMatrix, eigenVector);
  theta = vcl_atan2(eigenVector[1][1], -eigenVector[1][0]);

  /* the previous procedure don't cares orientations,
     so it could be wrong by 180 degrees.
     here is corrected if necessary */

  if (this->angle_diff(theta, angleRegion) > m_Prec) theta += CONST_PI;

  return theta;
}

/**************************************************************************************************************/
/**
 * Compute the difference betwenn 2 angles modulo 2_PI
 */
template <class TInputImage, class TPrecision>
double
LineSegmentDetector<TInputImage, TPrecision>
::angle_diff(double a, double b) const
{
  a -= b;
  while (a <= -CONST_PI)
    a += CONST_2PI;
  while (a >   CONST_PI)
    a -= CONST_2PI;
  if (a < 0.0) a = -a;
  return a;
}

/**************************************************************************************************************/
/**
 * compute the number of false alarm of the rectangle
 */
template <class TInputImage, class TPrecision>
double
LineSegmentDetector<TInputImage, TPrecision>
::ComputeRectNFA(const RectangleType& rec) const
{
  int    NbAligned = 0;
  double nfa_val = 0.;

  /** Compute the NFA of the rectangle
   *  We Need : The number of : Points in the rec  (Area of the rectangle)
   *                            Aligned points  with theta in the rectangle
   */

  /**  Compute the number of points aligned */
  typedef otb::Rectangle<double> RectangleType;
  RectangleType::Pointer rectangle =  RectangleType::New();

  /** Fill the rectangle with the points*/
  for (int i = 0; i < 2; ++i)
    {
    typename RectangleType::VertexType vertex;
    vertex[0] = rec[2 * i];
    vertex[1] = rec[2 * i + 1];
    rectangle->AddVertex(vertex);
    }
  rectangle->SetWidth(rec[4]);
  rectangle->SetOrientation(rec[5]);

  /** Get The Bounding Region*/
  OutputImageDirRegionType region = m_OrientationFilter->GetOutput()->GetLargestPossibleRegion();
  region.Crop(rectangle->GetBoundingRegion());


  itk::ImageRegionIterator<OutputImageDirType> it(m_OrientationFilter->GetOutput(), region);
  it.GoToBegin();

  int pts = 0;

  while (!it.IsAtEnd())
    {
    if (rectangle->IsInside(it.GetIndex()) &&
        m_OrientationFilter->GetOutput()->GetBufferedRegion().IsInside(it.GetIndex()))
      {
      ++pts;

      if (this->IsAligned(it.Get(), rec[5] /*theta*/, rec[6] /*Prec*/)) NbAligned++;
      }
    ++it;
    }

  /** Compute the NFA from the rectangle computed below*/
  RegionType largestRegion = const_cast<Self*>(this)->GetInput()->GetLargestPossibleRegion();
  double logNT = 5. * vcl_log10( static_cast<double>(largestRegion.GetNumberOfPixels()) ) / 2.;

  nfa_val = NFA(pts, NbAligned, m_DirectionsAllowed, logNT);

  return nfa_val;
}

/**************************************************************************************************************/
/*
   compute logarithm of binomial NFA
   NFA = NT.b(n, k, p)
   log10 NFA = log10( NFA )

   n, k, p - binomial parameters.
   logNT - logarithm of Number of Tests
 */
template <class TInputImage, class TPrecision>
double
LineSegmentDetector<TInputImage, TPrecision>
::NFA(int n, int k, double p, double logNT) const
{
  double val;
  double n_d = static_cast<double>(n);
  double k_d = static_cast<double>(k);

  if (k == 0)
    return -logNT;

//  double x = p;
  double a = k_d;
  double b = n_d - k_d + 1.0;
  val = -logNT - log10( dbetai_(&p, &a, &b) );

  if (vnl_math_isinf(val)) /* approximate by the first term of the tail */
  {
    double x1 = n_d + 1.0;
    double x2 = k_d + 1.0;
    double x3 = n_d - k_d + 1.0;

    val = -logNT - (dlngam_(&x1) - dlngam_(&x2) - dlngam_(&x3)) / CONST_LN10
          - k_d * log10(p) - (n_d - k_d) * log10(1.0 - p);
  }
  return val;
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TPrecision>
void
LineSegmentDetector<TInputImage, TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

}

} // end namespace otb

#endif
