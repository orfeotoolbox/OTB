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
#ifndef __otbVectorDataExtractROI_txx
#define __otbVectorDataExtractROI_txx

#include "otbVectorDataExtractROI.h"
#include "itkImageRegion.h"
#include "itkIndex.h"
#include "itkSize.h"

namespace otb
{

/**
 *
 */
template <class TVectorData>
VectorDataExtractROI<TVectorData>
::VectorDataExtractROI()
{
  /** Global Variables initialisation*/
  m_SizeX = 1;
  m_SizeY = 1;
  m_StartX = 0;
  m_StartY = 0;

  
  
}

/**
 *
 */
template <class TVectorData>
void
VectorDataExtractROI<TVectorData>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}


/**
 * VectorDataExtractROI can produce an output vector data which is different depth
 * than the input vectorData
 * As such, VectorDataExtractROI have to provide an implementation for GenerateOutputInformation
 * in order to inform the pipeline execution model. 
 * \sa ProcessObject::GenerateOutputInformaton()
 */
// template <class TVectorData>
// void
// VectorDataExtractROI<TVectorData>
// ::GenerateOutputInformation()
// {
//   // Call to the base class method
//   Superclass::GenerateOutputInformation();
// }

template <class TVectorData>
void
VectorDataExtractROI<TVectorData>
::GenerateData(void)
{
  /** Get The input and the outptut*/
  
  
  
  /** Create a region with the right size*/
  const   unsigned int                Dimension = 2;
  typedef itk::ImageRegion<Dimension>         ImageRegionType;
  typedef itk::Index<Dimension>               IndexType;
  typedef itk::Size<Dimension>                SizeType;
    
  ImageRegionType                     roi;
  IndexType                           index;
  SizeType                            size;

  /** Update the region information*/
  index[0] = m_StartX;
  index[1] = m_StartY;
  size[0]  = m_SizeX;
  size[1]  = m_SizeY;

  /** Create the region*/
  roi.SetSize(size);
  roi.SetIndex(index);

  std::cout << "ROI " << roi<<std::endl;
  
  /***/
  
  
  

}


} // end namespace otb

#endif
