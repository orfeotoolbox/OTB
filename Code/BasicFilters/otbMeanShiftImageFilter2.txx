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

#ifndef __otbMeanShiftImageFilter2_txx
#define __otbMeanShiftImageFilter2_txx

#include "otbMeanShiftImageFilter2.h"

#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "otbMacro.h"

#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
//#include "itkOffset.h"
#include "itkProgressReporter.h"

#include "itkConstNeighborhoodIterator.h"

//#include "msImageProcessor.h"

namespace otb
{
template <class TInputImage, class TOutputMetricImage,class TOutputImage,class TKernel>
MeanShiftImageFilter2<TInputImage,TOutputMetricImage, TOutputImage, TKernel>
::MeanShiftImageFilter2()
{
  m_SpatialRadius.Fill(3);
  m_RangeRadius.Fill(9);
  m_ImageLatticeInitialized = false;
  m_HasConverged = false;
  m_IterationCount = 0;
  m_MaxIterationNumber = 4;
  //this->SetRadius();
  m_SpatialKernel=NULL;
  m_RangeKernel=NULL;
  m_Kernel=NULL;

  m_SpectralBandwidth=16.;
  m_Threshold=1e-3;
  m_NumberOfSpatialComponents=2; //image lattice
  //CreateKernel by default
 // m_Kernel.CreateStructuringElement();


  m_NeighborhoodHasTobeUpdated = true;
  this->SetNumberOfOutputs(3);
  this->SetNthOutput(0, OutputImageType::New());
  this->SetNthOutput(1, OutputImageType::New());
  this->SetNthOutput(2, OutputMetricImageType::New());

}


template <class TInputImage, class TOutputMetricImage, class TOutputImage, class TKernel>
MeanShiftImageFilter2<TInputImage,TOutputMetricImage, TOutputImage, TKernel>
::~ MeanShiftImageFilter2()
{
  // if (m_Neighborhood)
//     {

 //    }
   delete[] m_Kernel;
   delete[] m_SpatialKernel;
   delete[] m_RangeKernel;
}


// to be replaced with new and generic method
template <class TInputImage, class TOutputMetricImage, class TOutputImage, class TKernel>
void MeanShiftImageFilter2<TInputImage,TOutputMetricImage, TOutputImage, TKernel>
::CreateUniformKernel()
{
  if(m_SpatialKernel)
    {
    delete[] m_SpatialKernel;
    }
  if(m_Kernel)
    {
     delete[] m_Kernel;
    }
  if(m_RangeKernel)
    {
    delete[] m_RangeKernel;
    }
  InputSizeType kernelSize = this->GetLargestRadius();

  unsigned int numberOfPixels = kernelSize[0] * kernelSize[1];
  //std::cout<<"number of pix "<<numberOfPixels<<std::endl;
  unsigned int spatialNumberOfComponents =  this->GetSpatialOutput()->GetNumberOfComponentsPerPixel();
  unsigned int rangeNumberOfComponents =this->GetRangeOutput()->GetNumberOfComponentsPerPixel();
  unsigned int numberOfComponents = spatialNumberOfComponents + rangeNumberOfComponents;
  // Case Uniform

  // memory allocation
  m_Kernel = new OutputPixelType[numberOfPixels];
  m_SpatialKernel = new OutputPixelType[numberOfPixels];
  m_RangeKernel = new OutputPixelType[numberOfPixels];

  // TODO JGT : to be replaced with generic kernel
  // no kernel defined

  unsigned int kernelCenterX = (kernelSize[0] - 1) / 2;
  unsigned int kernelCenterY = (kernelSize[1] - 1) / 2;

  // test odd/even
  unsigned int spatialMinX = (kernelSize[0] - m_SpatialRadius[0])/ 2;
  unsigned int spatialMinY = (kernelSize[1] - m_SpatialRadius[1])/ 2;
  unsigned int spatialMaxX = spatialMinX+m_SpatialRadius[0];
  unsigned int spatialMaxY = spatialMinY+m_SpatialRadius[1];

  OutputPixelType *spatialIt = m_SpatialKernel;
  OutputPixelType *it = m_Kernel;
  double spatialWeighting = 1. / (m_SpatialRadius[0] * m_SpatialRadius[1]);

  for (int y = 0; y < kernelSize[1]; y++)
    {
    for (int x = 0; x < kernelSize[0]; x++)
      {
      it->SetSize(numberOfComponents);
      it->Fill(0.);
      spatialIt->SetSize(spatialNumberOfComponents);
      spatialIt->Fill(0.);

     if ((x >= spatialMinX) && (x < spatialMaxX) && (y >= spatialMinY) && (y < spatialMaxY))
        {
        spatialIt->Fill(spatialWeighting);
        for (int comp = 0; comp < spatialNumberOfComponents; comp++)
          {
          it->SetElement(comp, spatialWeighting);
          }
        }
      ++spatialIt;
      ++it;
      }

    }

  // test odd/even
    unsigned int rangeMinX = (kernelSize[0] - m_RangeRadius[0])/ 2;
    unsigned int rangeMinY = (kernelSize[1] - m_RangeRadius[1])/ 2;
    unsigned int rangeMaxX = rangeMinX+m_RangeRadius[0];
    unsigned int rangeMaxY = rangeMinY+m_RangeRadius[1];

    OutputPixelType *rangeIt = m_RangeKernel;
    it = m_Kernel;
    double rangeWeighting = 1. / (m_RangeRadius[0] * m_RangeRadius[1]);

    for (int y = 0; y < kernelSize[1]; y++)
      {
      for (int x = 0; x < kernelSize[0]; x++)
        {
        rangeIt->SetSize(rangeNumberOfComponents);
        rangeIt->Fill(0.);

        if ((x >= rangeMinX) && (x < rangeMaxX) && (y >= rangeMinY) && (y < rangeMaxY))
          {
          rangeIt->Fill(rangeWeighting);
          for (int comp = 0; comp < rangeNumberOfComponents; comp++)
            {
            it->SetElement(comp+spatialNumberOfComponents, rangeWeighting);
            }
          }
        ++rangeIt;
        ++it;
        }

      }
}


template <class TInputImage, class TOutputMetricImage, class TOutputImage, class TKernel>
const typename MeanShiftImageFilter2<TInputImage,TOutputMetricImage, TOutputImage, TKernel>::OutputImageType *
MeanShiftImageFilter2<TInputImage,TOutputMetricImage, TOutputImage, TKernel>
::GetSpatialOutput() const
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<const OutputImageType *>(this->itk::ProcessObject::GetOutput(0));
}

template <class TInputImage, class TOutputMetricImage, class TOutputImage, class TKernel>
typename MeanShiftImageFilter2<TInputImage,TOutputMetricImage,TOutputImage, TKernel>::OutputImageType *
MeanShiftImageFilter2<TInputImage, TOutputMetricImage,TOutputImage, TKernel>
::GetSpatialOutput()
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->itk::ProcessObject::GetOutput(0));
}


template <class TInputImage,  class TOutputMetricImage,class TOutputImage, class TKernel>
const typename MeanShiftImageFilter2<TInputImage,TOutputMetricImage, TOutputImage, TKernel>::OutputImageType *
MeanShiftImageFilter2<TInputImage,TOutputMetricImage, TOutputImage, TKernel>
::GetRangeOutput() const
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return 0;
    }
  return static_cast<const OutputImageType *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputImage, class TOutputMetricImage, class TOutputImage, class TKernel>
typename MeanShiftImageFilter2<TInputImage, TOutputMetricImage,TOutputImage, TKernel>::OutputImageType *
MeanShiftImageFilter2<TInputImage,TOutputMetricImage, TOutputImage, TKernel>
::GetRangeOutput()
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->itk::ProcessObject::GetOutput(1));
}


template <class TInputImage,  class TOutputMetricImage,class TOutputImage, class TKernel>
const typename MeanShiftImageFilter2<TInputImage,TOutputMetricImage, TOutputImage, TKernel>::OutputMetricImageType *
MeanShiftImageFilter2<TInputImage,TOutputMetricImage, TOutputImage, TKernel>
::GetMetricOutput() const
{
  if (this->GetNumberOfOutputs() < 3)
    {
    return 0;
    }
  return static_cast<const OutputMetricImageType *>(this->itk::ProcessObject::GetOutput(2));
}

template <class TInputImage, class TOutputMetricImage, class TOutputImage, class TKernel>
typename MeanShiftImageFilter2<TInputImage,TOutputMetricImage, TOutputImage, TKernel>::OutputMetricImageType *
MeanShiftImageFilter2<TInputImage,TOutputMetricImage, TOutputImage, TKernel>
::GetMetricOutput()
{
  if (this->GetNumberOfOutputs() < 3)
    {
    return 0;
    }
  return static_cast<OutputMetricImageType *>(this->itk::ProcessObject::GetOutput(2));
}




template <class TInputImage,class TOutputMetricImage, class TOutputImage, class TKernel>
void
MeanShiftImageFilter2<TInputImage, TOutputMetricImage, TOutputImage, TKernel>
::AllocateOutputs()
{

  typename OutputImageType::Pointer   spatialOutputPtr = this->GetSpatialOutput();
  typename OutputImageType::Pointer   rangeOutputPtr = this->GetRangeOutput();
  typename OutputImageType::Pointer   metricOutputPtr = this->GetMetricOutput();


  metricOutputPtr->SetBufferedRegion(metricOutputPtr->GetRequestedRegion());
  metricOutputPtr->Allocate();

  spatialOutputPtr->SetBufferedRegion(spatialOutputPtr->GetRequestedRegion());
  spatialOutputPtr->Allocate();

  rangeOutputPtr->SetBufferedRegion(rangeOutputPtr->GetRequestedRegion());
  rangeOutputPtr->Allocate();

 }



template <class TInputImage,class TOutputMetricImage, class TOutputImage, class TKernel>
void
MeanShiftImageFilter2<TInputImage,TOutputMetricImage, TOutputImage, TKernel>
::Initialize()
 {
  // nothing to do
 }



/* TO DO
 *
 *
 *
 */


template <class TInputImage,class TOutputMetricImage, class TOutputImage, class TKernel>
void
MeanShiftImageFilter2<TInputImage, TOutputMetricImage, TOutputImage, TKernel>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  unsigned int numberOfComponents= this->GetInput()->GetNumberOfComponentsPerPixel();
  if (this->GetSpatialOutput())
      {
  this->GetSpatialOutput()->SetNumberOfComponentsPerPixel(m_NumberOfSpatialComponents); // image lattice
      }
  if (this->GetSpatialOutput())
       {
  this->GetRangeOutput()->SetNumberOfComponentsPerPixel(numberOfComponents);
       }
  if (this->GetMetricOutput())
       {
  this->GetMetricOutput()->SetNumberOfComponentsPerPixel(numberOfComponents+m_NumberOfSpatialComponents); // Spectral Part + lattice
       }
}


template <class TInputImage,class TOutputMetricImage, class TOutputImage, class TKernel>
void
MeanShiftImageFilter2<TInputImage, TOutputMetricImage, TOutputImage, TKernel>
::GenerateInputRequestedRegion()
{
  // Call superclass implementation
  Superclass::GenerateInputRequestedRegion();

  // Retrieve input pointers
  TInputImage * inPtr  = const_cast<TInputImage *>(this->GetInput());

  TOutputMetricImage    * outMetricPtr = this->GetMetricOutput();
  TOutputImage * outSpatialPtr = this->GetSpatialOutput();
  TOutputImage * outRangePtr = this->GetRangeOutput();

  // Check pointers before using them
  if(!inPtr || !outMetricPtr || !outSpatialPtr || !outRangePtr)
    {
    return;
    }


  // Retrieve requested region (TODO: check if we need to handle
  // region for outHDispPtr)
  RegionType outputRequestedRegion = outMetricPtr->GetRequestedRegion();

  // spatial and range radius may differ, padding must be done with the largest.
  //unsigned int largestRadius= this->GetLargestRadius();
  InputSizeType largestRadius= this->GetLargestRadius();
  // Pad by the appropriate radius
  RegionType inputRequestedRegion  = outputRequestedRegion;
  inputRequestedRegion.PadByRadius(largestRadius);

  // crop the input requested region at the input's largest possible region
   if ( inputRequestedRegion.Crop(inPtr->GetLargestPossibleRegion()) )
     {
     inPtr->SetRequestedRegion( inputRequestedRegion );
     return;
     }
   else
     {
     // Couldn't crop the region (requested region is outside the largest
     // possible region).  Throw an exception.

     // store what we tried to request (prior to trying to crop)
     inPtr->SetRequestedRegion( inputRequestedRegion );

     // build an exception
     itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
     e.SetLocation(ITK_LOCATION);
     e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
     e.SetDataObject(inPtr);
     throw e;
     }

}


template <class TInputImage,class TOutputMetricImage, class TOutputImage, class TKernel>
void
MeanShiftImageFilter2<TInputImage,TOutputMetricImage, TOutputImage, TKernel>
::BeforeThreadedGenerateData()
{

  TOutputMetricImage    * outMetricPtr = this->GetMetricOutput();
  TOutputImage * outSpatialPtr   = this->GetSpatialOutput();
  TOutputImage * outRangePtr   = this->GetRangeOutput();

  // each value is tested
  m_HasConverged=false;

  //TODO define generic case for the Kernel
  this->CreateUniformKernel();
}


// returns input spatial neighborhood, range, and binary map for boundaries
template <class TInputImage,class TOutputMetricImage, class TOutputImage, class TKernel>
typename MeanShiftImageFilter2<TInputImage,TOutputMetricImage, TOutputImage, TKernel>::OutputMetricPixelType  MeanShiftImageFilter2<TInputImage,TOutputMetricImage, TOutputImage, TKernel>
::CalculateMeanShiftVector(OutputPixelType *neighborhood,OutputPixelType spatialPixel,OutputPixelType rangePixel)
 {
  //std::cout<<"calculate mean shift vector"<<std::endl;
  OutputMetricPixelType meanShiftVector;
  OutputMetricPixelType weightingMeanShiftVector;
  // Kernel*Input //
  InputSizeType kernelSize = this->GetLargestRadius();

  unsigned int numberOfPixels= kernelSize[0]*kernelSize[1];
  //std::cout<<"number of pix "<<numberOfPixels<<std::endl;
  unsigned int spatialNumberOfComponents = spatialPixel.Size();
  unsigned int rangeNumberOfComponents   = rangePixel.Size();
  unsigned int numberOfComponents   = spatialNumberOfComponents +rangeNumberOfComponents;
  double sum=0;

  meanShiftVector.SetSize(numberOfComponents);
  meanShiftVector.Fill(0.);
  weightingMeanShiftVector.SetSize(numberOfComponents);
  weightingMeanShiftVector.Fill(0.);

  // use only m_Kernel : need to define a concatenate output not spatial and range
  OutputPixelType *it = neighborhood;
  OutputPixelType *spatialIt = m_SpatialKernel;
  OutputPixelType *rangeIt = m_RangeKernel;

  //std::cout<<"start processing"<<std::endl;
  double neighborhoodValue;
  double value;
  unsigned int boundaryWeightIndex=numberOfComponents;
  bool isInside;
  for( int y=0;y<kernelSize[1];y++)
    {
      for (int x = 0; x < kernelSize[0]; x++)
        {
       // std::cout<<"it.Size "<<it->Size()<<std::endl;
        isInside=true;
        for(unsigned int comp=0;comp<rangeNumberOfComponents;comp++)
                   {

                     neighborhoodValue=it->GetElement(comp+spatialNumberOfComponents);
                     if(std::abs(neighborhoodValue-rangePixel[comp])>m_SpectralBandwidth)
                       isInside=false;
                   }

          if(it->GetElement(boundaryWeightIndex) && isInside)
           {

            for(unsigned int comp=0;comp<spatialNumberOfComponents;comp++)
            {
             neighborhoodValue=it->GetElement(comp);
             //value=spatialIt->GetElement(comp);
             value=1;
             meanShiftVector[comp]+=(neighborhoodValue);
             weightingMeanShiftVector[comp]+=value;

            }
           for(unsigned int comp=0;comp<rangeNumberOfComponents;comp++)
           {
             neighborhoodValue=it->GetElement(comp+spatialNumberOfComponents);
             //value=rangeIt->GetElement(comp);
             value=1;
            //  meanShiftVector[spatialNumberOfComponents+comp]+=(neighborhoodValue-rangePixel[comp])*(neighborhoodValue-rangePixel[comp])*neighborhoodValue*value;
            //  weightingMeanShiftVector[spatialNumberOfComponents+comp]+=(neighborhoodValue-rangePixel[comp])*(neighborhoodValue-rangePixel[comp])*value;
             meanShiftVector[spatialNumberOfComponents+comp]+=(neighborhoodValue);
             weightingMeanShiftVector[spatialNumberOfComponents+comp]+=value;
           }

           }
           ++it;
           ++rangeIt;
           ++spatialIt;
        }
    }

  for(unsigned int comp=0;comp<spatialNumberOfComponents;comp++)
              {
               if( weightingMeanShiftVector[comp]>0)
                meanShiftVector[comp]=meanShiftVector[comp]/weightingMeanShiftVector[comp]-spatialPixel[comp];
               else
                meanShiftVector[comp]=0;
              }
  for(unsigned int comp=0;comp<rangeNumberOfComponents;comp++)
    {
    if( weightingMeanShiftVector[spatialNumberOfComponents+comp]>0)
                   meanShiftVector[spatialNumberOfComponents+comp]=meanShiftVector[spatialNumberOfComponents+comp]/weightingMeanShiftVector[spatialNumberOfComponents+comp]-rangePixel[comp];
                  else
                   meanShiftVector[spatialNumberOfComponents+comp]=0;
   }

  return meanShiftVector;
 }


// returns input spatial neighborhood, range, and binarry map for boundaries
template <class TInputImage,class TOutputMetricImage, class TOutputImage, class TKernel>
void MeanShiftImageFilter2<TInputImage,TOutputMetricImage, TOutputImage, TKernel>
::GetNeighborhood(OutputPixelType **neighborhood,PointType latticePosition)
 {

  typename InputImageType::ConstPointer input = this->GetInput();
  InputSizeType kernelSize = this->GetLargestRadius();

  unsigned int numberOfPixels = kernelSize[0] * kernelSize[1];
  //std::cout<<"number of pix "<<numberOfPixels<<std::endl;
  unsigned int numberOfComponents = input->GetNumberOfComponentsPerPixel();
  RegionType largestPossibleRegion = input->GetLargestPossibleRegion();
  InputSizeType inputSize = largestPossibleRegion.GetSize();

  unsigned int width = inputSize[0];
  unsigned int height = inputSize[1];

  // define region
  itk::ImageRegion<2> imageRegion;
  IndexType index;
  index[0] = std::floor(latticePosition[0] + 0.5);
  index[1] = std::floor(latticePosition[1] + 0.5);

  int xMin = index[0] - std::floor(kernelSize[0] / 2);
  int xMax = xMin + kernelSize[0];
  int yMin = index[1] - std::floor(kernelSize[1] / 2);
  int yMax = yMin + kernelSize[1];

  IndexType minIndex;
  minIndex[0] = std::max(xMin, 0);
  minIndex[1] = std::max(yMin, 0);
  IndexType maxIndex;
  maxIndex[0] = std::min(xMax, static_cast<int> (width));
  maxIndex[1] = std::min(yMax, static_cast<int> (height));

  imageRegion.SetIndex(index);
  SizeType size;
  size[0] = maxIndex[0] - minIndex[0];
  size[1] = maxIndex[1] - minIndex[1];
  imageRegion.SetSize(size);

  OutputPixelType *it = *neighborhood;
  PointType pixelPos;
  IndexType pixelIndex;
  InputPixelType inputPixel;
  inputPixel.SetSize(numberOfComponents);

  // fill m_Neighborhood
  unsigned int indextype = 0;
  for (int y = 0; y < kernelSize[1]; y++)
    {
    for (int x = 0; x < kernelSize[0]; x++)
      {
      it->SetSize(numberOfComponents + 3);
      pixelIndex[0] = xMin + x;
      pixelIndex[1] = yMin + y;
      pixelPos[0] = xMin + x;
      pixelPos[1] = yMin + y;

      if ((pixelPos[0] >= minIndex[0]) && (pixelPos[0] <= maxIndex[0]) && (pixelPos[1] >= minIndex[1]) && (pixelPos[1]
          <= maxIndex[1]))
        {

        inputPixel = input->GetPixel(pixelIndex);

        it->SetElement(0, pixelPos[0]);
        it->SetElement(1, pixelPos[1]);
        for (unsigned int comp = 0; comp < numberOfComponents; comp++)
          {
          it->SetElement(comp + m_NumberOfSpatialComponents, inputPixel[comp]);
          }

        it->SetElement(numberOfComponents + m_NumberOfSpatialComponents, 1.);
        }
      else
        {
        for (unsigned int comp = 0; comp < numberOfComponents; comp++)
          {
          it->SetElement(comp + m_NumberOfSpatialComponents, 0.);
          }
        it->SetElement(0, pixelPos[0]);
        it->SetElement(1, pixelPos[1]);
        it->SetElement(numberOfComponents + m_NumberOfSpatialComponents, 0.);
        }

      ++it;
      indextype++;
      }
    }

}



template <class TInputImage,class TOutputMetricImage, class TOutputImage, class TKernel>
void
MeanShiftImageFilter2<TInputImage,TOutputMetricImage, TOutputImage, TKernel>
::ThreadedGenerateData(const OutputRegionType& outputRegionForThread, int threadId)
{
  // at the first iteration

  // Allocate the output image
  this->AllocateOutputs();

  RegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  // Allocate output
  typename OutputImageType::Pointer spatialOutput = this->GetSpatialOutput();
  typename OutputImageType::Pointer rangeOutput = this->GetRangeOutput();
  typename OutputMetricImageType::Pointer metricOutput = this->GetMetricOutput();

  typename InputImageType::ConstPointer input = this->GetInput();

  // defines input and output iterators
  //ypedef itk::ConstIterator<InputImageType>   InputConstNeighborhoodIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType> OutputIteratorType;
  typedef itk::ImageRegionIterator<OutputMetricImageType> OutputMetricIteratorType;
  typedef itk::ImageRegionConstIteratorWithIndex<InputImageType> InputIteratorWithIndexType;

  typename OutputImageType::PixelType rangePixel;
  typename OutputImageType::PixelType spatialPixel;
  typename OutputMetricImageType::PixelType metricPixel;

  InputIteratorWithIndexType inputIt(input, inputRegionForThread);

  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inputIt.GoToBegin();

  OutputIteratorType rangeIt(rangeOutput, outputRegionForThread);
  OutputIteratorType spatialIt(spatialOutput, outputRegionForThread);
  OutputMetricIteratorType metricIt(metricOutput, outputRegionForThread);

  // fill pixel
  rangeIt.GoToBegin();
  spatialIt.GoToBegin();
  metricIt.GoToBegin();

  unsigned int spatialNumberOfComponents = spatialOutput->GetNumberOfComponentsPerPixel();
  unsigned int rangeNumberOfComponents = rangeOutput->GetNumberOfComponentsPerPixel();
  unsigned int numberOfComponents = spatialNumberOfComponents + rangeNumberOfComponents;

  InputSizeType kernelSize = this->GetLargestRadius();
  OutputPixelType *neighborhood = new OutputPixelType[kernelSize[0] * kernelSize[1]];
  unsigned int iteration = 0;

  while (!inputIt.IsAtEnd())
    {
    bool neighborhoodHasTobeUpdated = true;
    rangePixel = rangeIt.Get();
    spatialPixel = spatialIt.Get();
    metricPixel = metricIt.Get();

    InputIndexType index = inputIt.GetIndex();

    spatialPixel.SetElement(0, index[0]);
    spatialPixel.SetElement(1, index[1]);

    rangePixel = inputIt.Get();

    // TODO change the maximum value;

    m_HasConverged = false;
    while ((iteration < m_MaxIterationNumber) || (!m_HasConverged))
      {

      typename OutputMetricImageType::PixelType meanShiftVector;

      PointType position;
      position[0] = spatialPixel[0];
      position[1] = spatialPixel[1];

      // use only when needed
      if (neighborhoodHasTobeUpdated)
        {
        this->GetNeighborhood(&neighborhood, position);
        neighborhoodHasTobeUpdated = false;
        }

      //Calculate meanShiftVector
      meanShiftVector = this->CalculateMeanShiftVector(neighborhood, spatialPixel, rangePixel);

      double sum = 0;

      for (unsigned int comp = 0; comp < spatialNumberOfComponents; comp++)
        {
        neighborhoodHasTobeUpdated = neighborhoodHasTobeUpdated || ((std::floor(
                                                                                spatialPixel[comp]
                                                                                    + meanShiftVector[comp] + 0.5)
            - std::floor(spatialPixel[comp] + 0.5)) != 0);
        spatialPixel[comp] += meanShiftVector[comp];
        metricPixel[comp] = meanShiftVector[comp] * meanShiftVector[comp];
        sum += metricPixel[comp];
        }


      for (unsigned int comp = 0; comp < rangeNumberOfComponents; comp++)
        {

        rangePixel[comp] += meanShiftVector[spatialNumberOfComponents + comp];
        metricPixel[spatialNumberOfComponents + comp] = meanShiftVector[spatialNumberOfComponents + comp]
            * meanShiftVector[spatialNumberOfComponents + comp];
        sum += metricPixel[comp];
        }


      m_HasConverged = sum < m_Threshold;

      iteration++;
      }

    rangeIt.Set(rangePixel);
    spatialIt.Set(spatialPixel);
    metricIt.Set(metricPixel);

    ++inputIt;

    ++rangeIt;
    ++spatialIt;
    ++metricIt;

    progress.CompletedPixel();

    // use iteration map
    iteration = 0;
    }
  if (neighborhood != NULL)
    {
     delete[] neighborhood;
    }

}




/* after threaded convergence test */
template <class TInputImage,class TOutputMetricImage, class TOutputImage, class TKernel>
void
MeanShiftImageFilter2<TInputImage,TOutputMetricImage, TOutputImage, TKernel>
::AfterThreadedGenerateData()
{


}




template <class TInputImage,class TOutputMetricImage, class TOutputImage, class TKernel>
void
MeanShiftImageFilter2<TInputImage,TOutputMetricImage, TOutputImage, TKernel>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Spatial radius: "                << m_SpatialRadius               << std::endl;
  os << indent << "Range radius: "                  << m_RangeRadius                 << std::endl;
 }

} // end namespace otb

#endif
