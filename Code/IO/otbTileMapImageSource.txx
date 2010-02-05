/*=========================================================================
 
 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$
 
 
 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.
 
 Some parts of this code are derived from ITK. See ITKCopyright.txt
 for details.
 
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.
 
 =========================================================================*/
#ifndef __otbTileMapImageSource_txx
#define __otbTileMapImageSource_txx

#include "otbTileMapImageSource.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"
#include "itkObjectFactory.h"

namespace otb
{
  
  template <class TOutputImage>
  TileMapImageSource<TOutputImage>
  ::TileMapImageSource()
  {
    //Initial image is 256 wide in each direction.
    for (unsigned int i=0; i<TOutputImage::GetImageDimension(); i++)
    {
      m_Size[i] = 256;
      m_Spacing[i] = 1.0;
      m_Origin[i] = 0.0;
    }
    m_Direction.SetIdentity();
    
    // Initialize Tile Map
    m_Depth = m_DepthMin;
    m_Latitude = 0.;
    m_Longitude = 0.;
    m_ServerName = "";
    
    // Tile Management
    m_CacheDirectory = "./TileMapCache";
    m_TileMapModel = new ossimTileMapModel;
  }
  
  template <class TOutputImage>
  void 
  TileMapImageSource<TOutputImage>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
    
    unsigned int i;
    
    os << indent << "Size: [";
    for (i=0; i < TOutputImage::GetImageDimension(); i++)
    {
      os << m_Size[i] << ", ";
    }
    os << "]" << std::endl;
    
    os << indent << "Origin: [";
    for (i=0; i < TOutputImage::GetImageDimension(); i++)
    {
      os << m_Origin[i] << ", ";
    }
    os << "]" << std::endl;
    
    os << indent << "Spacing: " << m_Spacing << std::endl;
    
    os << indent << "Direction:" << std::endl;
    os << m_Direction << std::endl;
    
    os << indent << "Server name:" << std::endl;
    os << m_ServerName << std::endl;
    
    os << indent << "Depth:" << std::endl;
    os << m_Depth << std::endl;
    
    os << indent << "Latitude:" << std::endl;
    os << m_Latitude << std::endl;
    
    os << indent << "Longitude:" << std::endl;
    os << m_Longitude << std::endl;
  }
  
  //----------------------------------------------------------------------------
  template <typename TOutputImage>
  void 
  TileMapImageSource<TOutputImage>
  ::GenerateOutputInformation()
  {
    TOutputImage *output;
    typename TOutputImage::IndexType index = {{0}};
    typename TOutputImage::SizeType size = {{0}};
    size[0] = m_Size[0];
    size[1] = m_Size[1];

    output = this->GetOutput(0);

    typename TOutputImage::RegionType largestPossibleRegion;
    largestPossibleRegion.SetSize( size );
    largestPossibleRegion.SetIndex( index );
    output->SetLargestPossibleRegion( largestPossibleRegion );

    output->SetSpacing(m_Spacing);
    output->SetOrigin(m_Origin);
    output->SetDirection(m_Direction);
  }
  
  template <typename TOutputImage>
  void 
  TileMapImageSource<TOutputImage>
  ::GenerateData()
  {
//    typename TOutputImage::Pointer outputPtr = this->GetOutput();
    
    // allocate the output buffer
//    outputPtr->SetBufferedRegion( outputPtr->GetRequestedRegion() );
//    outputPtr->Allocate();
    
    // Create and initialize a new gaussian function
    /*  typedef itk::GaussianSpatialFunction<double, NDimensions> FunctionType;
     typedef typename FunctionType::InputType FunctionPositionType;
     typename FunctionType::Pointer pGaussian = FunctionType::New();
     
     pGaussian->SetSigma(m_Sigma);
     pGaussian->SetMean(m_Mean);
     pGaussian->SetScale(m_Scale);
     pGaussian->SetNormalized(m_Normalized);
     */
    // Create an iterator that will walk the output region
    /* typedef ImageRegionIterator<TOutputImage> OutputIterator;
    OutputIterator outIt = OutputIterator(outputPtr,
                                          outputPtr->GetRequestedRegion());

    // The value get from net OSM
    double value;

    // The position at which the function is evaluated
    Point<double, TOutputImage::ImageDimension> evalPoint;

    ProgressReporter progress(this, 0,
                              outputPtr->GetRequestedRegion()
                              .GetNumberOfPixels());

    // Walk the output image, evaluating the spatial function at each pixel
    for (; !outIt.IsAtEnd(); ++outIt)
    {
      typename TOutputImage::IndexType index = outIt.GetIndex();
      outputPtr->TransformIndexToPhysicalPoint(index, evalPoint );
      value = pGaussian->Evaluate(evalPoint);
      
      // Set the pixel value to the function value
      outIt.Set( (typename TOutputImage::PixelType) value);
      progress.CompletedPixel();
    }
    } */
  }
  
} // end namespace otb

#endif
