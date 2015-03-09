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
#ifndef __otbHoughTransform2DLinesImageFilter_txx
#define __otbHoughTransform2DLinesImageFilter_txx

#include "otbHoughTransform2DLinesImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkCastImageFilter.h"

namespace otb
{

/** Constructor */
template<typename TInputPixelType, typename TOutputPixelType>
HoughTransform2DLinesImageFilter<TInputPixelType, TOutputPixelType>
::HoughTransform2DLinesImageFilter()
{
  const double nPI = 4.0 * vcl_atan(1.0);

  m_Threshold = 0; // by default

  m_AngleAxisSize = 500;
  m_AngleAxisMinimum = -nPI;
  m_AngleAxisMaximum = nPI;

  m_DistanceAxisSize = -1;
  m_DistanceAxisMinimum = -100;
  m_DistanceAxisMaximum = 100;

  m_NumberOfLines = 1;
  m_DiscRadius = 10;
  m_Variance = 5;
  m_OldModifiedTime = 0;
  m_OldNumberOfLines = 0;
  m_SimplifyAccumulator = NULL;
}

template<typename TInputPixelType, typename TOutputPixelType>
void
HoughTransform2DLinesImageFilter<TInputPixelType, TOutputPixelType>
::EnlargeOutputRequestedRegion(itk::DataObject *output)
{
  // call the superclass' implementation of this method
  Superclass::EnlargeOutputRequestedRegion(output);

  output->SetRequestedRegionToLargestPossibleRegion();
}

template<typename TInputPixelType, typename TOutputPixelType>
void
HoughTransform2DLinesImageFilter<TInputPixelType, TOutputPixelType>
::GenerateOutputInformation()
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  // get pointers to the input and output
  InputImageConstPointer input  = this->GetInput();
  OutputImagePointer     output = this->GetOutput();

  if (!input || !output)
    {
    return;
    }

  // Compute the size of the output image
  if (m_DistanceAxisSize + 1 == 0)
    {
    double inputSize0 = input->GetLargestPossibleRegion().GetSize()[0];
    double inputSize1 = input->GetLargestPossibleRegion().GetSize()[1];
    double inputDiagonalSize = sqrt(inputSize0 * inputSize0 + inputSize1 * inputSize1);
    m_DistanceAxisSize = (long unsigned int) inputDiagonalSize;
    }

  itk::Size<2> size;
  size[0] = (long unsigned int) m_DistanceAxisSize;
  size[1] = (long unsigned int) m_AngleAxisSize;

  typename InputImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(input->GetLargestPossibleRegion().GetIndex());

  output->SetLargestPossibleRegion(region);
}

template<typename TInputPixelType, typename TOutputPixelType>
void
HoughTransform2DLinesImageFilter<TInputPixelType, TOutputPixelType>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
  if (this->GetInput())
    {
    InputImagePointer image = const_cast<InputImageType *>(this->GetInput());
    image->SetRequestedRegionToLargestPossibleRegion();
    }
}

/** Generate the accumulator image */
template<typename TInputPixelType, typename TOutputPixelType>
void
HoughTransform2DLinesImageFilter<TInputPixelType, TOutputPixelType>
::GenerateData()
{
  itkDebugMacro(<< "HoughTransform2DLinesImageFilter called");

  m_AngleAxisIncrement = (m_AngleAxisMaximum - m_AngleAxisMinimum) / m_AngleAxisSize;
  m_DistanceAxisIncrement = (m_DistanceAxisMaximum - m_DistanceAxisMinimum) / m_DistanceAxisSize;

  // Get the input and output pointers
  InputImageConstPointer inputImage  = this->GetInput(0);
  OutputImagePointer     outputImage = this->GetOutput(0);

  // Allocate the output
  this->AllocateOutputs();
  outputImage->FillBuffer(0);

  itk::ImageRegionConstIteratorWithIndex<InputImageType>  image_it(inputImage,  inputImage->GetRequestedRegion());
  image_it.GoToBegin();

  itk::Index<2> index;

// Pre-computation of trigonometric lines
  unsigned int nbAngles = static_cast<unsigned long int>(m_AngleAxisSize);
  std::vector<double> cosAngle(nbAngles, 0.);
  std::vector<double> sinAngle(nbAngles, 0.);

  //const double nPI = 4.0 * vcl_atan( 1.0 ); Unused in this method
  for (unsigned int indexAngle = 0; indexAngle < nbAngles; indexAngle++)
    {
    double angle = this->GetAngleValue(indexAngle);
    cosAngle[indexAngle] = vcl_cos(angle);
    sinAngle[indexAngle] = vcl_sin(angle);
    }

  while (!image_it.IsAtEnd())
    {
    if (image_it.Get() > m_Threshold)
      {
      for (unsigned int indexAngle = 0; indexAngle < nbAngles; indexAngle++)
        {
        double       r = image_it.GetIndex()[0] * cosAngle[indexAngle] + image_it.GetIndex()[1] * sinAngle[indexAngle];
        unsigned int indexR = this->GetDistanceIndex(r);
        index[0] = indexR; // m_R
        index[1] = indexAngle; // m_Theta
        if (outputImage->GetBufferedRegion().IsInside(index))
          {
          outputImage->SetPixel(index, outputImage->GetPixel(index) + image_it.Get());
          }
        }
      }
    ++image_it;
    }
}

/** Simplify the accumulator
 * Do the same iteration process as the Update() method but find the maximum
 * along the curve and then remove the curve */
template<typename TInputPixelType, typename TOutputPixelType>
void
HoughTransform2DLinesImageFilter<TInputPixelType, TOutputPixelType>
::Simplify(void)
{
  // Get the input and output pointers
  InputImageConstPointer inputImage = this->GetInput(0);
  OutputImagePointer     outputImage = this->GetOutput(0);

  if (!inputImage || !outputImage)
    {
    itkExceptionMacro("Update() must be called before Simplify().");
    }

  /** Allocate the simplify accumulator */
  m_SimplifyAccumulator = OutputImageType::New();
  m_SimplifyAccumulator->SetRegions(outputImage->GetLargestPossibleRegion());
  m_SimplifyAccumulator->SetOrigin(inputImage->GetOrigin());
  m_SimplifyAccumulator->SetSpacing(inputImage->GetSpacing());
  m_SimplifyAccumulator->Allocate();
  m_SimplifyAccumulator->FillBuffer(0);

  itk::Index<2>                       index;
  itk::Index<2>                       maxIndex;
  typename OutputImageType::PixelType value;
  typename OutputImageType::PixelType valuemax;

  itk::ImageRegionConstIteratorWithIndex<InputImageType>  image_it(inputImage,  inputImage->GetRequestedRegion());
  image_it.GoToBegin();

  while (!image_it.IsAtEnd())
    {
    if (image_it.Get() > m_Threshold)
      {
      // Look for maximum along the curve and remove the curve at the same time
      valuemax = -1;
      maxIndex[0] = 0;
      maxIndex[1] = 0;
      for (double angle = m_AngleAxisMinimum; angle < m_AngleAxisMaximum; angle += m_AngleAxisIncrement)
        {
        index[0] = (long int) (image_it.GetIndex()[0] * vcl_cos(angle) + image_it.GetIndex()[1] * vcl_sin(angle)); // m_R
        index[1] = (long int) (this->GetAngleIndex(angle)); // m_Theta

        if (outputImage->GetBufferedRegion().IsInside(index))
          {
          value = outputImage->GetPixel(index);
          if (value > valuemax)
            {
            valuemax = value;
            maxIndex = index;
            }
          }
        }
      m_SimplifyAccumulator->SetPixel(maxIndex, m_SimplifyAccumulator->GetPixel(maxIndex) + 1);
      }
    ++image_it;
    }

  itk::ImageRegionConstIteratorWithIndex<OutputImageType>  accusimple_it(m_SimplifyAccumulator,
                                                                         m_SimplifyAccumulator->GetRequestedRegion());
  itk::ImageRegionIteratorWithIndex<OutputImageType>       accu_it(outputImage,  outputImage->GetRequestedRegion());

  accusimple_it.GoToBegin();
  accu_it.GoToBegin();

  while (!accusimple_it.IsAtEnd())
    {
    accu_it.Set(accusimple_it.Get());
    ++accu_it;
    ++accusimple_it;
    }

}

/** Get the list of lines. This recomputes the lines */
template<typename TInputPixelType, typename TOutputPixelType>
typename HoughTransform2DLinesImageFilter<TInputPixelType, TOutputPixelType>::LinesListType&
HoughTransform2DLinesImageFilter<TInputPixelType, TOutputPixelType>
::GetLines(unsigned int n)
{
  if ((this->GetMTime() == m_OldModifiedTime) && (n == m_OldNumberOfLines)) // if the filter has not been updated
    {
    return m_LinesList;
    }

  m_LinesList.clear();

  /** Blur the accumulator in order to find the maximum */
  typedef float                                 InternalImagePixelType;
  typedef itk::Image<InternalImagePixelType, 2> InternalImageType;

  OutputImagePointer outputImage = this->GetOutput(0);

  if (!outputImage)
    {
    itkExceptionMacro("Update() must be called before GetLines().");
    }

  /** Convert the accumulator output image type to internal image type*/
  typedef itk::CastImageFilter<OutputImageType, InternalImageType> CastImageFilterType;

  typename CastImageFilterType::Pointer castImageFilter = CastImageFilterType::New();
  castImageFilter->SetInput(outputImage);

  typedef itk::DiscreteGaussianImageFilter<InternalImageType, InternalImageType> GaussianFilterType;
  typename GaussianFilterType::Pointer gaussianFilter = GaussianFilterType::New();

  gaussianFilter->SetInput(castImageFilter->GetOutput()); // the output is the accumulator image
  double variance[2];
  variance[0] = m_Variance;
  variance[1] = m_Variance;
  gaussianFilter->SetVariance(variance);
  gaussianFilter->Update();
  InternalImageType::Pointer postProcessImage = gaussianFilter->GetOutput();

  typedef itk::MinimumMaximumImageCalculator<InternalImageType> MinMaxCalculatorType;
  typename MinMaxCalculatorType::Pointer minMaxCalculator = MinMaxCalculatorType::New();
  itk::ImageRegionIterator<InternalImageType>
  it_input(postProcessImage, postProcessImage->GetLargestPossibleRegion());

  const double nPI = 4.0 * vcl_atan(1.0);

  itk::Index<2> index;

  unsigned int lines = 0;
  bool         found;

  // Find maxima
  do
    {
    minMaxCalculator->SetImage(postProcessImage);
    minMaxCalculator->ComputeMaximum();
    InternalImageType::PixelType max = minMaxCalculator->GetMaximum();

    found = false;
    for (it_input.GoToBegin(); !it_input.IsAtEnd(); ++it_input)
      {
      if (it_input.Get() == max)
        {
        // Create the line
        LineType::PointListType list; // insert two points per line

        double radius = this->GetDistanceValue(it_input.GetIndex()[0]);
        double teta   = this->GetAngleValue(it_input.GetIndex()[1]);
        double Vx = radius * vcl_cos(teta);
        double Vy = radius * vcl_sin(teta);
        double norm = vcl_sqrt(Vx * Vx + Vy * Vy);
        double VxNorm = Vx / norm;
        double VyNorm = Vy / norm;

        if ((teta <= 0) || (teta >= nPI / 2))
          {
          if (teta >= nPI / 2)
            {
            VyNorm = -VyNorm;
            VxNorm = -VxNorm;
            }

          LinePointType p;
          p.SetPosition(Vx, Vy);
          list.push_back(p);
          p.SetPosition(Vx - VyNorm * 5, Vy + VxNorm * 5);
          list.push_back(p);
          }
        else // if teta>0
          {
          LinePointType p;
          p.SetPosition(Vx, Vy);
          list.push_back(p);
          p.SetPosition(Vx - VyNorm * 5, Vy + VxNorm * 5);
          list.push_back(p);
          } // end if(teta>0)

        // Create a Line Spatial Object
        LinePointer Line = LineType::New();
        Line->SetId(lines);
        Line->SetPoints(list);
        Line->ComputeBoundingBox();

        m_LinesList.push_back(Line);

        // Remove a black disc from the hough space domain
        for (double angle = m_AngleAxisMinimum; angle <= m_AngleAxisMaximum; angle += m_AngleAxisIncrement)
          {
          for (double length = 0; length < m_DiscRadius; length += 1)
            {
            index[0] = (long int) (it_input.GetIndex()[0] + length * vcl_cos(angle));
            index[1] = (long int) (it_input.GetIndex()[1] + length * vcl_sin(angle));
            if (postProcessImage->GetBufferedRegion().IsInside(index))
              {
              postProcessImage->SetPixel(index, 0);
              }
            }
          }
        minMaxCalculator->SetImage(postProcessImage);
        minMaxCalculator->ComputeMaximum();
        max = minMaxCalculator->GetMaximum();

        ++lines;
        found = true;
        if (lines == m_NumberOfLines) break;
        }
      }
    }
  while ((lines < m_NumberOfLines) && (found));

  m_OldModifiedTime = this->GetMTime();
  m_OldNumberOfLines = m_LinesList.size();
  return m_LinesList;
}

/** Print Self information */
template<typename TInputPixelType, typename TOutputPixelType>
void
HoughTransform2DLinesImageFilter<TInputPixelType, TOutputPixelType>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << "Threshold: " << m_Threshold << std::endl;
  os << "Angle axis size: " << m_AngleAxisSize << std::endl;
  os << "Angle axis minimum: " << m_AngleAxisMinimum << std::endl;
  os << "Angle axis maximum: " << m_AngleAxisMaximum << std::endl;
  os << "Number Of Lines: " << m_NumberOfLines << std::endl;
  os << "Disc Radius: " << m_DiscRadius << std::endl;
  os << "Accumulator blur variance: " << m_Variance << std::endl;
  os << "Simplify Accumulator" << m_SimplifyAccumulator << std::endl;
}

/** Computes angle index from angle value */
template<typename TInputPixelType, typename TOutputPixelType>
int
HoughTransform2DLinesImageFilter<TInputPixelType, TOutputPixelType>
::GetAngleIndex(double angle)
{
  int indexAngle = static_cast<int>((angle - m_AngleAxisMinimum) / m_AngleAxisIncrement);
  return indexAngle;
}

/** Computes angle value from angle index */
template<typename TInputPixelType, typename TOutputPixelType>
double
HoughTransform2DLinesImageFilter<TInputPixelType, TOutputPixelType>
::GetAngleValue(int indexAngle)
{
  double angle = m_AngleAxisMinimum + indexAngle * m_AngleAxisIncrement;
  return angle;
}

/** Computes distance index from distance value */
template<typename TInputPixelType, typename TOutputPixelType>
int
HoughTransform2DLinesImageFilter<TInputPixelType, TOutputPixelType>
::GetDistanceIndex(double distance)
{
  int indexDistance = static_cast<int>((distance - m_DistanceAxisMinimum) / m_DistanceAxisIncrement);
  return indexDistance;
}

/** Computes distance value from distance index */
template<typename TInputPixelType, typename TOutputPixelType>
double
HoughTransform2DLinesImageFilter<TInputPixelType, TOutputPixelType>
::GetDistanceValue(int indexDistance)
{
  double distance = m_DistanceAxisMinimum + indexDistance * m_DistanceAxisIncrement;
  return distance;
}

} // end namespace

#endif
