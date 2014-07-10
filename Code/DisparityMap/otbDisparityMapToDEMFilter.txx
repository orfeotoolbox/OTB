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
#ifndef __otbDisparityMapToDEMFilter_txx
#define __otbDisparityMapToDEMFilter_txx

#include "otbDisparityMapToDEMFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIterator.h"

namespace otb
{

template <class TDisparityImage, class TInputImage, class TOutputDEMImage,
class TEpipolarGridImage, class TMaskImage>
DisparityMapToDEMFilter<TDisparityImage,TInputImage,TOutputDEMImage,TEpipolarGridImage,TMaskImage>
::DisparityMapToDEMFilter()
{
  // Set the number of inputs
  this->SetNumberOfRequiredInputs(7);
  this->SetNumberOfRequiredInputs(1);

  // Set the outputs
  this->SetNumberOfRequiredOutputs(1);
  this->SetNthOutput(0,TOutputDEMImage::New());

  // Default DEM reconstruction parameters
  m_ElevationMin = 0.0;
  m_ElevationMax = 100.0;
  m_DEMGridStep = 10.0;

  m_InputSplitter = SplitterType::New();
  m_UsedInputSplits = 1;
}

template <class TDisparityImage, class TInputImage, class TOutputDEMImage,
class TEpipolarGridImage, class TMaskImage>
DisparityMapToDEMFilter<TDisparityImage,TInputImage,TOutputDEMImage,TEpipolarGridImage,TMaskImage>
::~DisparityMapToDEMFilter()
{}

template <class TDisparityImage, class TInputImage, class TOutputDEMImage,
class TEpipolarGridImage, class TMaskImage>
void
DisparityMapToDEMFilter<TDisparityImage,TInputImage,TOutputDEMImage,TEpipolarGridImage,TMaskImage>
::SetHorizontalDisparityMapInput( const TDisparityImage * hmap)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(0, const_cast<TDisparityImage *>( hmap ));
}

template <class TDisparityImage, class TInputImage, class TOutputDEMImage,
class TEpipolarGridImage, class TMaskImage>
void
DisparityMapToDEMFilter<TDisparityImage,TInputImage,TOutputDEMImage,TEpipolarGridImage,TMaskImage>
::SetVerticalDisparityMapInput( const TDisparityImage * vmap)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(1, const_cast<TDisparityImage *>( vmap ));
}

template <class TDisparityImage, class TInputImage, class TOutputDEMImage,
class TEpipolarGridImage, class TMaskImage>
void
DisparityMapToDEMFilter<TDisparityImage,TInputImage,TOutputDEMImage,TEpipolarGridImage,TMaskImage>
::SetLeftInput( const TInputImage * image)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(2, const_cast<TInputImage *>( image ));
}

template <class TDisparityImage, class TInputImage, class TOutputDEMImage,
class TEpipolarGridImage, class TMaskImage>
void
DisparityMapToDEMFilter<TDisparityImage,TInputImage,TOutputDEMImage,TEpipolarGridImage,TMaskImage>
::SetRightInput( const TInputImage * image)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(3, const_cast<TInputImage *>( image ));
}

template <class TDisparityImage, class TInputImage, class TOutputDEMImage,
class TEpipolarGridImage, class TMaskImage>
void
DisparityMapToDEMFilter<TDisparityImage,TInputImage,TOutputDEMImage,TEpipolarGridImage,TMaskImage>
::SetLeftEpipolarGridInput( const TEpipolarGridImage * grid)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(4, const_cast<TEpipolarGridImage *>( grid ));
}

template <class TDisparityImage, class TInputImage, class TOutputDEMImage,
class TEpipolarGridImage, class TMaskImage>
void
DisparityMapToDEMFilter<TDisparityImage,TInputImage,TOutputDEMImage,TEpipolarGridImage,TMaskImage>
::SetRightEpipolarGridInput( const TEpipolarGridImage * grid)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(5, const_cast<TEpipolarGridImage *>( grid ));
}

template <class TDisparityImage, class TInputImage, class TOutputDEMImage,
class TEpipolarGridImage, class TMaskImage>
void
DisparityMapToDEMFilter<TDisparityImage,TInputImage,TOutputDEMImage,TEpipolarGridImage,TMaskImage>
::SetDisparityMaskInput( const TMaskImage * mask)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(6, const_cast<TMaskImage *>( mask ));
}

template <class TDisparityImage, class TInputImage, class TOutputDEMImage,
class TEpipolarGridImage, class TMaskImage>
const TDisparityImage *
DisparityMapToDEMFilter<TDisparityImage,TInputImage,TOutputDEMImage,TEpipolarGridImage,TMaskImage>
::GetHorizontalDisparityMapInput() const
{
  if(this->GetNumberOfInputs()<1)
    {
    return 0;
    }
  return static_cast<const TDisparityImage *>(this->itk::ProcessObject::GetInput(0));
}

template <class TDisparityImage, class TInputImage, class TOutputDEMImage,
class TEpipolarGridImage, class TMaskImage>
const TDisparityImage *
DisparityMapToDEMFilter<TDisparityImage,TInputImage,TOutputDEMImage,TEpipolarGridImage,TMaskImage>
::GetVerticalDisparityMapInput() const
{
  if(this->GetNumberOfInputs()<2)
    {
    return 0;
    }
  return static_cast<const TDisparityImage *>(this->itk::ProcessObject::GetInput(1));
}

template <class TDisparityImage, class TInputImage, class TOutputDEMImage,
class TEpipolarGridImage, class TMaskImage>
const TInputImage *
DisparityMapToDEMFilter<TDisparityImage,TInputImage,TOutputDEMImage,TEpipolarGridImage,TMaskImage>
::GetLeftInput() const
{
  if(this->GetNumberOfInputs()<3)
    {
    return 0;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(2));
}

template <class TDisparityImage, class TInputImage, class TOutputDEMImage,
class TEpipolarGridImage, class TMaskImage>
const TInputImage *
DisparityMapToDEMFilter<TDisparityImage,TInputImage,TOutputDEMImage,TEpipolarGridImage,TMaskImage>
::GetRightInput() const
{
  if(this->GetNumberOfInputs()<4)
    {
    return 0;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(3));
}

template <class TDisparityImage, class TInputImage, class TOutputDEMImage,
class TEpipolarGridImage, class TMaskImage>
const TEpipolarGridImage *
DisparityMapToDEMFilter<TDisparityImage,TInputImage,TOutputDEMImage,TEpipolarGridImage,TMaskImage>
::GetLeftEpipolarGridInput() const
{
  if(this->GetNumberOfInputs()<5)
    {
    return 0;
    }
  return static_cast<const TEpipolarGridImage *>(this->itk::ProcessObject::GetInput(4));
}

template <class TDisparityImage, class TInputImage, class TOutputDEMImage,
class TEpipolarGridImage, class TMaskImage>
const TEpipolarGridImage *
DisparityMapToDEMFilter<TDisparityImage,TInputImage,TOutputDEMImage,TEpipolarGridImage,TMaskImage>
::GetRightEpipolarGridInput() const
{
  if(this->GetNumberOfInputs()<6)
    {
    return 0;
    }
  return static_cast<const TEpipolarGridImage *>(this->itk::ProcessObject::GetInput(5));
}

template <class TDisparityImage, class TInputImage, class TOutputDEMImage,
class TEpipolarGridImage, class TMaskImage>
const TMaskImage *
DisparityMapToDEMFilter<TDisparityImage,TInputImage,TOutputDEMImage,TEpipolarGridImage,TMaskImage>
::GetDisparityMaskInput() const
{
  if(this->GetNumberOfInputs()<7)
    {
    return 0;
    }
  return static_cast<const TMaskImage *>(this->itk::ProcessObject::GetInput(6));
}

template <class TDisparityImage, class TInputImage, class TOutputDEMImage,
class TEpipolarGridImage, class TMaskImage>
const TOutputDEMImage *
DisparityMapToDEMFilter<TDisparityImage,TInputImage,TOutputDEMImage,TEpipolarGridImage,TMaskImage>
::GetDEMOutput() const
{
  if(this->GetNumberOfOutputs()<1)
    {
    return 0;
    }
  return static_cast<const TOutputDEMImage *>(this->itk::ProcessObject::GetOutput(0));
}

template <class TDisparityImage, class TInputImage, class TOutputDEMImage,
class TEpipolarGridImage, class TMaskImage>
TOutputDEMImage *
DisparityMapToDEMFilter<TDisparityImage,TInputImage,TOutputDEMImage,TEpipolarGridImage,TMaskImage>
::GetDEMOutput()
{
  if(this->GetNumberOfOutputs()<1)
    {
    return 0;
    }
  return static_cast<TOutputDEMImage *>(this->itk::ProcessObject::GetOutput(0));
}

template <class TDisparityImage, class TInputImage, class TOutputDEMImage,
class TEpipolarGridImage, class TMaskImage>
void
DisparityMapToDEMFilter<TDisparityImage,TInputImage,TOutputDEMImage,TEpipolarGridImage,TMaskImage>
::GenerateOutputInformation()
{
  // Get common area between images
  const TInputImage * leftImgPtr = this->GetLeftInput();
  const TInputImage * rightImgPtr = this->GetRightInput();
  TOutputDEMImage * outputPtr = this->GetDEMOutput();

  // Set-up a transform to use the DEMHandler
  typedef otb::GenericRSTransform<> RSTransform2DType;
  RSTransform2DType::Pointer leftToGroundTransform = RSTransform2DType::New();
  leftToGroundTransform->SetInputKeywordList(leftImgPtr->GetImageKeywordlist());
  leftToGroundTransform->InstanciateTransform();

  RSTransform2DType::Pointer rightToGroundTransform = RSTransform2DType::New();
  rightToGroundTransform->SetInputKeywordList(rightImgPtr->GetImageKeywordlist());
  rightToGroundTransform->InstanciateTransform();

  // left image
  typename SensorImageType::SizeType inputSize = leftImgPtr->GetLargestPossibleRegion().GetSize();
  typename SensorImageType::PointType ulp, urp, llp, lrp;
  itk::ContinuousIndex<double,2> ul(leftImgPtr->GetLargestPossibleRegion().GetIndex());
  ul[0] += -0.5;
  ul[1] += -0.5;

  itk::ContinuousIndex<double,2> ur(ul);
  itk::ContinuousIndex<double,2> lr(ul);
  itk::ContinuousIndex<double,2> ll(ul);
  ur[0] += static_cast<double>(inputSize[0]);
  lr[0] += static_cast<double>(inputSize[0]);
  lr[1] += static_cast<double>(inputSize[1]);
  ll[1] += static_cast<double>(inputSize[1]);

  leftImgPtr->TransformContinuousIndexToPhysicalPoint(ul,ulp);
  leftImgPtr->TransformContinuousIndexToPhysicalPoint(ur,urp);
  leftImgPtr->TransformContinuousIndexToPhysicalPoint(ll,llp);
  leftImgPtr->TransformContinuousIndexToPhysicalPoint(lr,lrp);

  RSTransform2DType::OutputPointType left_ul, left_ur, left_ll, left_lr;
  left_ul = leftToGroundTransform->TransformPoint(ulp);
  left_ur = leftToGroundTransform->TransformPoint(urp);
  left_ll = leftToGroundTransform->TransformPoint(llp);
  left_lr = leftToGroundTransform->TransformPoint(lrp);

  // right image
  inputSize = rightImgPtr->GetLargestPossibleRegion().GetSize();
  ul = rightImgPtr->GetLargestPossibleRegion().GetIndex();
  ul[0] += -0.5;
  ul[1] += -0.5;
  ur = ul;
  lr = ul;
  ll = ul;
  ur[0] += static_cast<double>(inputSize[0]);
  lr[0] += static_cast<double>(inputSize[0]);
  lr[1] += static_cast<double>(inputSize[1]);
  ll[1] += static_cast<double>(inputSize[1]);

  rightImgPtr->TransformContinuousIndexToPhysicalPoint(ul,ulp);
  rightImgPtr->TransformContinuousIndexToPhysicalPoint(ur,urp);
  rightImgPtr->TransformContinuousIndexToPhysicalPoint(ll,llp);
  rightImgPtr->TransformContinuousIndexToPhysicalPoint(lr,lrp);

  RSTransform2DType::OutputPointType right_ul, right_ur, right_lr, right_ll;
  right_ul = rightToGroundTransform->TransformPoint(ulp);
  right_ur = rightToGroundTransform->TransformPoint(urp);
  right_ll = rightToGroundTransform->TransformPoint(llp);
  right_lr = rightToGroundTransform->TransformPoint(lrp);

  double left_xmin = std::min(std::min(std::min(left_ul[0],left_ur[0]),left_lr[0]),left_ll[0]);
  double left_xmax = std::max(std::max(std::max(left_ul[0],left_ur[0]),left_lr[0]),left_ll[0]);
  double left_ymin = std::min(std::min(std::min(left_ul[1],left_ur[1]),left_lr[1]),left_ll[1]);
  double left_ymax = std::max(std::max(std::max(left_ul[1],left_ur[1]),left_lr[1]),left_ll[1]);

  double right_xmin = std::min(std::min(std::min(right_ul[0],right_ur[0]),right_lr[0]),right_ll[0]);
  double right_xmax = std::max(std::max(std::max(right_ul[0],right_ur[0]),right_lr[0]),right_ll[0]);
  double right_ymin = std::min(std::min(std::min(right_ul[1],right_ur[1]),right_lr[1]),right_ll[1]);
  double right_ymax = std::max(std::max(std::max(right_ul[1],right_ur[1]),right_lr[1]),right_ll[1]);

  double box_xmin = std::max(left_xmin,right_xmin);
  double box_xmax = std::min(left_xmax,right_xmax);
  double box_ymin = std::max(left_ymin,right_ymin);
  double box_ymax = std::min(left_ymax,right_ymax);

  if (box_xmin >= box_xmax || box_ymin >= box_ymax)
    {
    itkExceptionMacro(<<"Wrong reconstruction area, images don't overlap, check image corners");
    }

  // Compute step :
  // TODO : use a clean RS transform instead
  typename TOutputDEMImage::SpacingType outSpacing;
  outSpacing[0] = 57.295779513 * m_DEMGridStep / (6378137.0 * vcl_cos((box_ymin + box_ymax) * 0.5 * 0.01745329251));
  outSpacing[1] = -57.295779513 * m_DEMGridStep / 6378137.0;
  outputPtr->SetSpacing(outSpacing);

  // Choose origin
  typename TOutputDEMImage::PointType outOrigin;
  outOrigin[0] = box_xmin + 0.5 * outSpacing[0];
  outOrigin[1] = box_ymax + 0.5 * outSpacing[1];
  outputPtr->SetOrigin(outOrigin);

  // Compute output size
  typename DEMImageType::RegionType outRegion;
  outRegion.SetIndex(0,0);
  outRegion.SetIndex(1,0);
  outRegion.SetSize(0, static_cast<unsigned int>((box_xmax - box_xmin) / vcl_abs(outSpacing[0])));
  outRegion.SetSize(1, static_cast<unsigned int>((box_ymax - box_ymin) / vcl_abs(outSpacing[1])));

  outputPtr->SetLargestPossibleRegion(outRegion);
  outputPtr->SetNumberOfComponentsPerPixel(1);
}

template <class TDisparityImage, class TInputImage, class TOutputDEMImage,
class TEpipolarGridImage, class TMaskImage>
void
DisparityMapToDEMFilter<TDisparityImage,TInputImage,TOutputDEMImage,TEpipolarGridImage,TMaskImage>
::GenerateInputRequestedRegion()
{
  // For the epi grid : generate full buffer here !
  TEpipolarGridImage * leftGrid = const_cast<TEpipolarGridImage*>(this->GetLeftEpipolarGridInput());
  TEpipolarGridImage * rightGrid = const_cast<TEpipolarGridImage*>(this->GetRightEpipolarGridInput());

  leftGrid->SetRequestedRegionToLargestPossibleRegion();
  rightGrid->SetRequestedRegionToLargestPossibleRegion();

  leftGrid->UpdateOutputData();
  rightGrid->UpdateOutputData();

  // For the input left-right images
  //  -> No need, only use metadata and keywordlist
  const TInputImage * leftSensor = this->GetLeftInput();
  TOutputDEMImage * outputDEM = this->GetDEMOutput();

  typename DEMImageType::RegionType outRegion = outputDEM->GetRequestedRegion();
  typename DEMImageType::PointType outOrigin = outputDEM->GetOrigin();
  typename DEMImageType::SpacingType outSpacing = outputDEM->GetSpacing();

  RSTransformType::Pointer groundToLeftTransform = RSTransformType::New();
  groundToLeftTransform->SetOutputKeywordList(leftSensor->GetImageKeywordlist());
  groundToLeftTransform->InstanciateTransform();

  // For the disparity maps and mask
  // Iterate over OutputRequestedRegion corners for elevation min and max
  TDisparityImage * horizDisp = const_cast<TDisparityImage*>(this->GetHorizontalDisparityMapInput());
  TDisparityImage * vertiDisp = const_cast<TDisparityImage*>(this->GetVerticalDisparityMapInput());
  TMaskImage * maskDisp = const_cast<TMaskImage*>(this->GetDisparityMaskInput());

  // We impose that both disparity map inputs have the same size
  if(horizDisp->GetLargestPossibleRegion()
     != vertiDisp->GetLargestPossibleRegion())
    {
    itkExceptionMacro(<<"Horizontal and vertical disparity maps do not have the same size ! Horizontal largest region: "
      <<horizDisp->GetLargestPossibleRegion()<<", vertical largest region: "<<vertiDisp->GetLargestPossibleRegion());
    }

  if (maskDisp && horizDisp->GetLargestPossibleRegion() != maskDisp->GetLargestPossibleRegion())
    {
    itkExceptionMacro(<<"Disparity map and mask do not have the same size ! Map region : "
      <<horizDisp->GetLargestPossibleRegion()<<", mask region : "<<maskDisp->GetLargestPossibleRegion());
    }

  // up left at elevation min
  TDPointType corners[8];
  corners[0][0]= outOrigin[0] + outSpacing[0] * (-0.5 + static_cast<double>(outRegion.GetIndex(0)));
  corners[0][1]= outOrigin[1] + outSpacing[1] * (-0.5 + static_cast<double>(outRegion.GetIndex(1)));
  corners[0][2]= m_ElevationMin;
  // up left at elevation max
  corners[1][0]= corners[0][0];
  corners[1][1]= corners[0][1];
  corners[1][2]= m_ElevationMax;
  // up right at elevation min
  corners[2][0]= corners[0][0] + outSpacing[0] * static_cast<double>(outRegion.GetSize(0));
  corners[2][1]= corners[0][1];
  corners[2][2]= m_ElevationMin;
  // up right at elevation max
  corners[3][0]= corners[2][0];
  corners[3][1]= corners[2][1];
  corners[3][2]= m_ElevationMax;
  // low right at elevation min
  corners[4][0]= corners[0][0] + outSpacing[0] * static_cast<double>(outRegion.GetSize(0));
  corners[4][1]= corners[0][1] + outSpacing[1] * static_cast<double>(outRegion.GetSize(1));
  corners[4][2]= m_ElevationMin;
  // low right at elevation max
  corners[5][0]= corners[4][0];
  corners[5][1]= corners[4][1];
  corners[5][2]= m_ElevationMax;
  // low left at elevation min
  corners[6][0]= corners[0][0];
  corners[6][1]= corners[0][1] + outSpacing[1] * static_cast<double>(outRegion.GetSize(1));
  corners[6][2]= m_ElevationMin;
  // low left at elevation max
  corners[7][0]= corners[6][0];
  corners[7][1]= corners[6][1];
  corners[7][2]= m_ElevationMax;

  double x_loc,y_loc;
  double a_grid,b_grid;
  double u_loc[2];
  double v_loc[2];
  double det;
  typename GridImageType::IndexType gridIndex;
  typename GridImageType::IndexType gridIndexU;
  typename GridImageType::IndexType gridIndexV;
  typename GridImageType::PointType gridPoint;
  typename GridImageType::PointType gridPointU;
  typename GridImageType::PointType gridPointV;
  typename GridImageType::PixelType origLoc(2);
  typename GridImageType::PixelType uUnitLoc(2);
  typename GridImageType::PixelType vUnitLoc(2);

  typename GridImageType::RegionType gridRegion = leftGrid->GetLargestPossibleRegion();
  itk::ContinuousIndex<double,2> gridContiIndex;
  typename DisparityMapType::PointType epiPosition;
  itk::ContinuousIndex<double,2> epiContiIndex;
  double epiIndexMin[2];
  double epiIndexMax[2];
  int maxGridIndex[2];
  int minGridIndex[2];
  maxGridIndex[0] = static_cast<int>(gridRegion.GetIndex(0) + gridRegion.GetSize(0)) - 2;
  maxGridIndex[1] = static_cast<int>(gridRegion.GetIndex(1) + gridRegion.GetSize(1)) - 2;
  minGridIndex[0] = static_cast<int>(gridRegion.GetIndex(0));
  minGridIndex[1] = static_cast<int>(gridRegion.GetIndex(1));

  for (unsigned int k=0; k<8; k++)
    {
    // compute left image coordinate
    TDPointType tmpSensor = groundToLeftTransform->TransformPoint(corners[k]);

    // compute epipolar position
    gridIndex[0] = minGridIndex[0];
    gridIndex[1] = minGridIndex[1];

    //we assume 3 iterations are enough to find the 4 surrounding pixels
    for (unsigned int s=0; s<3; s++)
      {
      gridIndexU[0] = gridIndex[0] + 1;
      gridIndexU[1] = gridIndex[1];

      gridIndexV[0] = gridIndex[0];
      gridIndexV[1] = gridIndex[1] + 1;

      leftGrid->TransformIndexToPhysicalPoint(gridIndex, gridPoint);
      leftGrid->TransformIndexToPhysicalPoint(gridIndexU, gridPointU);
      leftGrid->TransformIndexToPhysicalPoint(gridIndexV, gridPointV);

      origLoc[0] = (leftGrid->GetPixel(gridIndex))[0] + gridPoint[0];
      origLoc[1] = (leftGrid->GetPixel(gridIndex))[1] + gridPoint[1];
      uUnitLoc[0] = (leftGrid->GetPixel(gridIndexU))[0] + gridPointU[0];
      uUnitLoc[1] = (leftGrid->GetPixel(gridIndexU))[1] + gridPointU[1];
      vUnitLoc[0] = (leftGrid->GetPixel(gridIndexV))[0] + gridPointV[0];
      vUnitLoc[1] = (leftGrid->GetPixel(gridIndexV))[1] + gridPointV[1];

      u_loc[0] = static_cast<double>(uUnitLoc[0] - origLoc[0]);
      u_loc[1] = static_cast<double>(uUnitLoc[1] - origLoc[1]);

      v_loc[0] = static_cast<double>(vUnitLoc[0] - origLoc[0]);
      v_loc[1] = static_cast<double>(vUnitLoc[1] - origLoc[1]);

      det = u_loc[0] * v_loc[1] - v_loc[0] * u_loc[1];

      x_loc = static_cast<double>(tmpSensor[0]) - static_cast<double>(origLoc[0]);
      y_loc = static_cast<double>(tmpSensor[1]) - static_cast<double>(origLoc[1]);

      a_grid = (x_loc * v_loc[1] - y_loc * v_loc[0]) / det;
      b_grid = (y_loc * u_loc[0] - x_loc * u_loc[1]) / det;

      gridContiIndex[0] = static_cast<double>(gridIndex[0]) + a_grid;
      gridContiIndex[1] = static_cast<double>(gridIndex[1]) + b_grid;

      leftGrid->TransformContinuousIndexToPhysicalPoint(gridContiIndex, epiPosition);

      horizDisp->TransformPhysicalPointToContinuousIndex(epiPosition,epiContiIndex);

      if (0.0 < a_grid && a_grid < 1.0 && 0.0 < b_grid && b_grid < 1.0)
        {
        // The four nearest positions in epipolar grid have been found : stop search
        break;
        }
      else
        {
        // Shift the gridIndex
        int a_grid_int = static_cast<int>(gridIndex[0]) + static_cast<int>(vcl_floor(a_grid));
        int b_grid_int = static_cast<int>(gridIndex[1]) + static_cast<int>(vcl_floor(b_grid));

        if (a_grid_int < minGridIndex[0]) a_grid_int = minGridIndex[0];
        if (a_grid_int > maxGridIndex[0]) a_grid_int = maxGridIndex[0];
        if (b_grid_int < minGridIndex[1]) b_grid_int = minGridIndex[1];
        if (b_grid_int > maxGridIndex[1]) b_grid_int = maxGridIndex[1];

        gridIndex[0] = a_grid_int;
        gridIndex[1] = b_grid_int;
        }
      }

    if (k==0)
      {
      epiIndexMin[0] = epiContiIndex[0];
      epiIndexMin[1] = epiContiIndex[1];
      epiIndexMax[0] = epiContiIndex[0];
      epiIndexMax[1] = epiContiIndex[1];
      }
    else
      {
      if (epiContiIndex[0] < epiIndexMin[0]) epiIndexMin[0] = epiContiIndex[0];
      if (epiContiIndex[1] < epiIndexMin[1]) epiIndexMin[1] = epiContiIndex[1];
      if (epiIndexMax[0] < epiContiIndex[0]) epiIndexMax[0] = epiContiIndex[0];
      if (epiIndexMax[1] < epiContiIndex[1]) epiIndexMax[1] = epiContiIndex[1];
      }

    }

  typename DisparityMapType::RegionType inputDisparityRegion;
  inputDisparityRegion.SetIndex(0, static_cast<int>(vcl_floor(epiIndexMin[0] + 0.5)));
  inputDisparityRegion.SetIndex(1, static_cast<int>(vcl_floor(epiIndexMin[1] + 0.5)));
  inputDisparityRegion.SetSize(0, 1 + static_cast<unsigned int>(vcl_floor(epiIndexMax[0] - epiIndexMin[0] + 0.5)));
  inputDisparityRegion.SetSize(1, 1 + static_cast<unsigned int>(vcl_floor(epiIndexMax[1] - epiIndexMin[1] + 0.5)));

  // crop the disparity region at the largest possible region
  if ( inputDisparityRegion.Crop(horizDisp->GetLargestPossibleRegion()))
    {
    horizDisp->SetRequestedRegion( inputDisparityRegion );
    vertiDisp->SetRequestedRegion( inputDisparityRegion );

    if (maskDisp)
      {
      maskDisp->SetRequestedRegion( inputDisparityRegion );
      }
    }
  else
    {
    // use empty region
    typename DisparityMapType::RegionType emptyRegion;

    emptyRegion.SetIndex(horizDisp->GetLargestPossibleRegion().GetIndex());
    emptyRegion.SetSize(0,0);
    emptyRegion.SetSize(1,0);

    horizDisp->SetRequestedRegion( emptyRegion );
    vertiDisp->SetRequestedRegion( emptyRegion );
    if (maskDisp)
      {
      maskDisp->SetRequestedRegion( emptyRegion );
      }

    // no exception : it can happen in this filter
    if (0)
      {
      // Couldn't crop the region (requested region is outside the largest
      // possible region).  Throw an exception.
      // store what we tried to request (prior to trying to crop)
      horizDisp->SetRequestedRegion( inputDisparityRegion );
      vertiDisp->SetRequestedRegion( inputDisparityRegion );

      // build an exception
      itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
      std::ostringstream msg;
      msg << this->GetNameOfClass()
                  << "::GenerateInputRequestedRegion()";
      e.SetLocation(msg.str().c_str());
      e.SetDescription("Requested region is (at least partially) outside the largest possible region of disparity map.");
      e.SetDataObject(horizDisp);
      throw e;
      }
    }
}

template <class TDisparityImage, class TInputImage, class TOutputDEMImage,
class TEpipolarGridImage, class TMaskImage>
void
DisparityMapToDEMFilter<TDisparityImage,TInputImage,TOutputDEMImage,TEpipolarGridImage,TMaskImage>
::BeforeThreadedGenerateData()
{
  const TDisparityImage * horizDisp = this->GetHorizontalDisparityMapInput();
  const TInputImage * leftSensor = this->GetLeftInput();
  const TInputImage * rightSensor = this->GetRightInput();

  const TOutputDEMImage * outputDEM = this->GetDEMOutput();

  typename DisparityMapType::RegionType requestedRegion = horizDisp->GetRequestedRegion();

  m_UsedInputSplits = m_InputSplitter->GetNumberOfSplits(requestedRegion, this->GetNumberOfThreads());

  m_LeftToGroundTransform = RSTransformType::New();
  m_RightToGroundTransform = RSTransformType::New();

  m_LeftToGroundTransform->SetInputKeywordList(leftSensor->GetImageKeywordlist());
  m_RightToGroundTransform->SetInputKeywordList(rightSensor->GetImageKeywordlist());

  m_LeftToGroundTransform->InstanciateTransform();
  m_RightToGroundTransform->InstanciateTransform();

  // ensure empty regions are not processed
  if (requestedRegion.GetSize(0) == 0 && requestedRegion.GetSize(1) == 0)
    {
    m_UsedInputSplits = 0;
    }

  if (m_UsedInputSplits <= static_cast<unsigned int>(this->GetNumberOfThreads()))
    {
    m_TempDEMRegions.clear();

    for (unsigned int i=0; i<m_UsedInputSplits; i++)
      {
      typename DEMImageType::Pointer tmpImg = TOutputDEMImage::New();
      tmpImg->SetNumberOfComponentsPerPixel(1);
      tmpImg->SetRegions(outputDEM->GetRequestedRegion());
      tmpImg->Allocate();

      typename DEMImageType::PixelType minElevation = static_cast<typename DEMImageType::PixelType>(m_ElevationMin);
      tmpImg->FillBuffer(minElevation);

      m_TempDEMRegions.push_back(tmpImg);
      }
    }
  else
    {
    itkExceptionMacro(<<"Wrong number of splits for input multithreading : "<<m_UsedInputSplits);
    }
}

template <class TDisparityImage, class TInputImage, class TOutputDEMImage,
class TEpipolarGridImage, class TMaskImage>
void
DisparityMapToDEMFilter<TDisparityImage,TInputImage,TOutputDEMImage,TEpipolarGridImage,TMaskImage>
::ThreadedGenerateData(const RegionType & outputRegionForThread, itk::ThreadIdType threadId)
{
  const TDisparityImage * horizDisp = this->GetHorizontalDisparityMapInput();
  const TDisparityImage * vertiDisp = this->GetVerticalDisparityMapInput();

  const TInputImage * leftSensor = this->GetLeftInput();
  const TInputImage * rightSensor = this->GetRightInput();

  const TMaskImage * disparityMask = this->GetDisparityMaskInput();

  const TOutputDEMImage * outputDEM = this->GetDEMOutput();

  const TEpipolarGridImage * leftGrid = this->GetLeftEpipolarGridInput();
  const TEpipolarGridImage * rightGrid = this->GetRightEpipolarGridInput();

  typename TEpipolarGridImage::RegionType gridRegion = leftGrid->GetLargestPossibleRegion();

  TOutputDEMImage * tmpDEM = NULL;
  typename TOutputDEMImage::RegionType outputRequestedRegion = outputDEM->GetRequestedRegion();

  typename TDisparityImage::RegionType disparityRegion;
  if (static_cast<unsigned int>(threadId) < m_UsedInputSplits)
    {
    disparityRegion = m_InputSplitter->GetSplit(threadId,m_UsedInputSplits,horizDisp->GetRequestedRegion());
    tmpDEM = m_TempDEMRegions[threadId];
    }
  else
    {
    return;
    }

  itk::ImageRegionConstIteratorWithIndex<DisparityMapType> horizIt(horizDisp,disparityRegion);
  itk::ImageRegionConstIteratorWithIndex<DisparityMapType> vertiIt(vertiDisp,disparityRegion);

  horizIt.GoToBegin();
  vertiIt.GoToBegin();

  bool useMask = false;
  itk::ImageRegionConstIterator<MaskImageType> maskIt;
  if (disparityMask)
    {
    useMask = true;
    maskIt = itk::ImageRegionConstIterator<MaskImageType>(disparityMask,disparityRegion);
    maskIt.GoToBegin();
    }

  typename TDisparityImage::PointType epiPoint;
  itk::ContinuousIndex<double,2> gridIndexConti;
  double subPixIndex[2];
  typename GridImageType::IndexType ulIndex, urIndex, lrIndex, llIndex;
  typename GridImageType::PixelType ulPixel(2);
  typename GridImageType::PixelType urPixel(2);
  typename GridImageType::PixelType lrPixel(2);
  typename GridImageType::PixelType llPixel(2);
  typename GridImageType::PixelType cPixel(2);

  typename GridImageType::PointType ulPoint;
  typename GridImageType::PointType urPoint;
  typename GridImageType::PointType lrPoint;
  typename GridImageType::PointType llPoint;

  TDPointType sensorPoint;
  TDPointType leftGroundHmin;
  TDPointType leftGroundHmax;
  TDPointType rightGroundHmin;
  TDPointType rightGroundHmax;

  while (!horizIt.IsAtEnd() && !vertiIt.IsAtEnd())
    {
    // check mask value if any
    if (useMask)
      {
      if (!(maskIt.Get() > 0))
        {
        ++horizIt;
        ++vertiIt;
        ++maskIt;
        continue;
        }
      }

    // compute left ray
    horizDisp->TransformIndexToPhysicalPoint(horizIt.GetIndex(),epiPoint);
    leftGrid->TransformPhysicalPointToContinuousIndex(epiPoint,gridIndexConti);

    ulIndex[0] = static_cast<int>(vcl_floor(gridIndexConti[0]));
    ulIndex[1] = static_cast<int>(vcl_floor(gridIndexConti[1]));
    if (ulIndex[0] < gridRegion.GetIndex(0)) ulIndex[0] = gridRegion.GetIndex(0);
    if (ulIndex[1] < gridRegion.GetIndex(1)) ulIndex[1] = gridRegion.GetIndex(1);
    if (ulIndex[0] > (gridRegion.GetIndex(0) + static_cast<int>(gridRegion.GetSize(0)) - 2))
      {
      ulIndex[0] = gridRegion.GetIndex(0) + gridRegion.GetSize(0) - 2;
      }
    if (ulIndex[1] > (gridRegion.GetIndex(1) + static_cast<int>(gridRegion.GetSize(1)) - 2))
      {
      ulIndex[1] = gridRegion.GetIndex(1) + gridRegion.GetSize(1) - 2;
      }
    urIndex[0] = ulIndex[0] + 1;
    urIndex[1] = ulIndex[1];
    lrIndex[0] = ulIndex[0] + 1;
    lrIndex[1] = ulIndex[1] + 1;
    llIndex[0] = ulIndex[0];
    llIndex[1] = ulIndex[1] + 1;
    subPixIndex[0] = gridIndexConti[0] - static_cast<double>(ulIndex[0]);
    subPixIndex[1] = gridIndexConti[1] - static_cast<double>(ulIndex[1]);

    leftGrid->TransformIndexToPhysicalPoint(ulIndex, ulPoint);
    leftGrid->TransformIndexToPhysicalPoint(urIndex, urPoint);
    leftGrid->TransformIndexToPhysicalPoint(lrIndex, lrPoint);
    leftGrid->TransformIndexToPhysicalPoint(llIndex, llPoint);

    ulPixel[0] = (leftGrid->GetPixel(ulIndex))[0] + ulPoint[0];
    ulPixel[1] = (leftGrid->GetPixel(ulIndex))[1] + ulPoint[1];
    urPixel[0] = (leftGrid->GetPixel(urIndex))[0] + urPoint[0];
    urPixel[1] = (leftGrid->GetPixel(urIndex))[1] + urPoint[1];
    lrPixel[0] = (leftGrid->GetPixel(lrIndex))[0] + lrPoint[0];
    lrPixel[1] = (leftGrid->GetPixel(lrIndex))[1] + lrPoint[1];
    llPixel[0] = (leftGrid->GetPixel(llIndex))[0] + llPoint[0];
    llPixel[1] = (leftGrid->GetPixel(llIndex))[1] + llPoint[1];
    cPixel = (ulPixel * (1.0 - subPixIndex[0]) + urPixel * subPixIndex[0]) * (1.0 - subPixIndex[1]) +
             (llPixel * (1.0 - subPixIndex[0]) + lrPixel * subPixIndex[0]) * subPixIndex[1];

    sensorPoint[0] = cPixel[0];
    sensorPoint[1] = cPixel[1];
    sensorPoint[2] = m_ElevationMin;
    leftGroundHmin = m_LeftToGroundTransform->TransformPoint(sensorPoint);

    sensorPoint[2] = m_ElevationMax;
    leftGroundHmax = m_LeftToGroundTransform->TransformPoint(sensorPoint);

    // compute right ray
    itk::ContinuousIndex<double,2> rightIndexEstimate;
    rightIndexEstimate[0] = static_cast<double>((horizIt.GetIndex())[0]) + static_cast<double>(horizIt.Get());
    rightIndexEstimate[1] = static_cast<double>((horizIt.GetIndex())[1]) + static_cast<double>(vertiIt.Get());

    horizDisp->TransformContinuousIndexToPhysicalPoint(rightIndexEstimate,epiPoint);
    rightGrid->TransformPhysicalPointToContinuousIndex(epiPoint,gridIndexConti);

    ulIndex[0] = static_cast<int>(vcl_floor(gridIndexConti[0]));
    ulIndex[1] = static_cast<int>(vcl_floor(gridIndexConti[1]));
    if (ulIndex[0] < gridRegion.GetIndex(0)) ulIndex[0] = gridRegion.GetIndex(0);
    if (ulIndex[1] < gridRegion.GetIndex(1)) ulIndex[1] = gridRegion.GetIndex(1);
    if (ulIndex[0] > (gridRegion.GetIndex(0) + static_cast<int>(gridRegion.GetSize(0)) - 2))
      {
      ulIndex[0] = gridRegion.GetIndex(0) + gridRegion.GetSize(0) - 2;
      }
    if (ulIndex[1] > (gridRegion.GetIndex(1) + static_cast<int>(gridRegion.GetSize(1)) - 2))
      {
      ulIndex[1] = gridRegion.GetIndex(1) + gridRegion.GetSize(1) - 2;
      }
    urIndex[0] = ulIndex[0] + 1;
    urIndex[1] = ulIndex[1];
    lrIndex[0] = ulIndex[0] + 1;
    lrIndex[1] = ulIndex[1] + 1;
    llIndex[0] = ulIndex[0];
    llIndex[1] = ulIndex[1] + 1;
    subPixIndex[0] = gridIndexConti[0] - static_cast<double>(ulIndex[0]);
    subPixIndex[1] = gridIndexConti[1] - static_cast<double>(ulIndex[1]);

    rightGrid->TransformIndexToPhysicalPoint(ulIndex, ulPoint);
    rightGrid->TransformIndexToPhysicalPoint(urIndex, urPoint);
    rightGrid->TransformIndexToPhysicalPoint(lrIndex, lrPoint);
    rightGrid->TransformIndexToPhysicalPoint(llIndex, llPoint);

    ulPixel[0] = (rightGrid->GetPixel(ulIndex))[0] + ulPoint[0];
    ulPixel[1] = (rightGrid->GetPixel(ulIndex))[1] + ulPoint[1];
    urPixel[0] = (rightGrid->GetPixel(urIndex))[0] + urPoint[0];
    urPixel[1] = (rightGrid->GetPixel(urIndex))[1] + urPoint[1];
    lrPixel[0] = (rightGrid->GetPixel(lrIndex))[0] + lrPoint[0];
    lrPixel[1] = (rightGrid->GetPixel(lrIndex))[1] + lrPoint[1];
    llPixel[0] = (rightGrid->GetPixel(llIndex))[0] + llPoint[0];
    llPixel[1] = (rightGrid->GetPixel(llIndex))[1] + llPoint[1];
    cPixel = (ulPixel * (1.0 - subPixIndex[0]) + urPixel * subPixIndex[0]) * (1.0 - subPixIndex[1]) +
             (llPixel * (1.0 - subPixIndex[0]) + lrPixel * subPixIndex[0]) * subPixIndex[1];

    sensorPoint[0] = cPixel[0];
    sensorPoint[1] = cPixel[1];
    sensorPoint[2] = m_ElevationMin;
    rightGroundHmin = m_RightToGroundTransform->TransformPoint(sensorPoint);

    sensorPoint[2] = m_ElevationMax;
    rightGroundHmax = m_RightToGroundTransform->TransformPoint(sensorPoint);

    // Compute ray intersection (mid-point method), TODO : implement non-iterative method from Hartley & Sturm
    double a = (leftGroundHmax[0] - leftGroundHmin[0]) * (leftGroundHmax[0] - leftGroundHmin[0]) +
               (leftGroundHmax[1] - leftGroundHmin[1]) * (leftGroundHmax[1] - leftGroundHmin[1]) +
               (leftGroundHmax[2] - leftGroundHmin[2]) * (leftGroundHmax[2] - leftGroundHmin[2]);
    double b = (rightGroundHmax[0] - rightGroundHmin[0]) * (rightGroundHmax[0] - rightGroundHmin[0]) +
               (rightGroundHmax[1] - rightGroundHmin[1]) * (rightGroundHmax[1] - rightGroundHmin[1]) +
               (rightGroundHmax[2] - rightGroundHmin[2]) * (rightGroundHmax[2] - rightGroundHmin[2]);
    double c = -(leftGroundHmax[0] - leftGroundHmin[0]) * (rightGroundHmax[0] - rightGroundHmin[0])
               -(leftGroundHmax[1] - leftGroundHmin[1]) * (rightGroundHmax[1] - rightGroundHmin[1])
               -(leftGroundHmax[2] - leftGroundHmin[2]) * (rightGroundHmax[2] - rightGroundHmin[2]);
    double g = (leftGroundHmax[0] - leftGroundHmin[0]) * (rightGroundHmin[0] - leftGroundHmin[0]) +
               (leftGroundHmax[1] - leftGroundHmin[1]) * (rightGroundHmin[1] - leftGroundHmin[1]) +
               (leftGroundHmax[2] - leftGroundHmin[2]) * (rightGroundHmin[2] - leftGroundHmin[2]);
    double h = -(rightGroundHmax[0] - rightGroundHmin[0]) * (rightGroundHmin[0] - leftGroundHmin[0])
               -(rightGroundHmax[1] - rightGroundHmin[1]) * (rightGroundHmin[1] - leftGroundHmin[1])
               -(rightGroundHmax[2] - rightGroundHmin[2]) * (rightGroundHmin[2] - leftGroundHmin[2]);

    double rLeft = (b * g - c * h) / (a * b - c * c);
    double rRight = (a * h - c * g) / (a * b - c * c);

    TDPointType leftFoot;
    leftFoot.SetToBarycentricCombination(leftGroundHmax,leftGroundHmin,rLeft );

    TDPointType rightFoot;
    rightFoot.SetToBarycentricCombination(rightGroundHmax,rightGroundHmin,rRight);

    TDPointType midPoint3D;
    midPoint3D.SetToMidPoint(leftFoot,rightFoot);

    // Is point inside DEM area ?
    typename DEMImageType::PointType midPoint2D;
    midPoint2D[0] = midPoint3D[0];
    midPoint2D[1] = midPoint3D[1];
    itk::ContinuousIndex<double,2> midIndex;
    outputDEM->TransformPhysicalPointToContinuousIndex(midPoint2D,midIndex);
    typename DEMImageType::IndexType cellIndex;

    //TODO JGT check if cellIndex should be calculed from the center of the pixel
    // TransformContinuousIndexToPhysicalPoint with index [0,0] returns Origin of image
    // TransformContinuousIndexToPhysicalPoint with index [0.5,0.5] returns a slight difference from Origin of image
    cellIndex[0] = static_cast<int>(vcl_floor(midIndex[0] + 0.5));
    cellIndex[1] = static_cast<int>(vcl_floor(midIndex[1] + 0.5));

    if (outputRequestedRegion.IsInside(cellIndex))
      {
      // Estimate local reference elevation (average, DEM or geoid) => NO NEED, ALREADY HAVE 3D RAYS
      //double localElevation = demHandler->GetHeightAboveEllipsoid(midPoint2D);

      // Add point to its corresponding cell (keep maximum)
      DEMPixelType cellHeight = static_cast<DEMPixelType>(midPoint3D[2]);
      if (cellHeight > tmpDEM->GetPixel(cellIndex) && cellHeight < static_cast<DEMPixelType>(m_ElevationMax))
        {
        tmpDEM->SetPixel(cellIndex,cellHeight);
        }
      }

    ++horizIt;
    ++vertiIt;

    if (useMask) ++maskIt;

    }

}

template <class TDisparityImage, class TInputImage, class TOutputDEMImage,
class TEpipolarGridImage, class TMaskImage>
void
DisparityMapToDEMFilter<TDisparityImage,TInputImage,TOutputDEMImage,TEpipolarGridImage,TMaskImage>
::AfterThreadedGenerateData()
{
  TOutputDEMImage * outputDEM = this->GetDEMOutput();

  if (m_TempDEMRegions.size() < 1)
    {
    outputDEM->FillBuffer(m_ElevationMin);
    return;
    }

  itk::ImageRegionIterator<DEMImageType> outputDEMIt(outputDEM,outputDEM->GetRequestedRegion());
  itk::ImageRegionIterator<DEMImageType> firstDEMIt(m_TempDEMRegions[0],outputDEM->GetRequestedRegion());

  outputDEMIt.GoToBegin();
  firstDEMIt.GoToBegin();

  // Copy first DEM
  while (!outputDEMIt.IsAtEnd() && !firstDEMIt.IsAtEnd())
    {
    outputDEMIt.Set(firstDEMIt.Get());
    ++outputDEMIt;
    ++firstDEMIt;
    }

  // Check DEMs from other threads and keep the maximum elevation
  for (unsigned int i=1; i<m_TempDEMRegions.size(); i++)
    {
    itk::ImageRegionIterator<DEMImageType> tmpDEMIt(m_TempDEMRegions[i],outputDEM->GetRequestedRegion());

    outputDEMIt.GoToBegin();
    tmpDEMIt.GoToBegin();

    while (!outputDEMIt.IsAtEnd() && !tmpDEMIt.IsAtEnd())
      {
      if (tmpDEMIt.Get() > outputDEMIt.Get())
        {
        outputDEMIt.Set(tmpDEMIt.Get());
        }
      ++outputDEMIt;
      ++tmpDEMIt;
      }
    }
}

}

#endif
