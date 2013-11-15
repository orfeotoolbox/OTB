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
#ifndef __otbSpatialisationFilter_txx
#define __otbSpatialisationFilter_txx

#include "itkNumericTraits.h"

#include "otbVectorDataProperties.h"
#include "otbSpatialisationFilter.h"

namespace otb
{

template <class TLabelMap>
SpatialisationFilter<TLabelMap>
::SpatialisationFilter()
{
  //default value
}


template <class TLabelMap>
void
SpatialisationFilter<TLabelMap>
::GenerateData()
{

  if (m_AreaVector.size() != (m_NumberOfObjects[0] * m_NumberOfObjects[1]))
    {
    itkExceptionMacro( << "Size problem : AreaVector size must be equal to "<< m_NumberOfObjects[0]*m_NumberOfObjects[1] );
    }
  if (m_PathVector.size() != (m_NumberOfObjects[0] * m_NumberOfObjects[1]))
    {
    itkExceptionMacro( << "Size problem : PathVector size must be equal to "<< m_NumberOfObjects[0]*m_NumberOfObjects[1] );
    }
  if (m_Labels.size() != (m_NumberOfObjects[0] * m_NumberOfObjects[1]))
    {
    itkExceptionMacro( << "Size problem : Labels size must be equal to "<< m_NumberOfObjects[0]*m_NumberOfObjects[1] );
    }

  unsigned int numberOfObjects = m_NumberOfObjects[0] * m_NumberOfObjects[1];
  //first object must have label 1. 0 is for background value in a label map.
  for (unsigned int i = 0; i < numberOfObjects; ++i)
    {
    this->ProcessObject(i);
    }
}

template <class TLabelMap>
void
SpatialisationFilter<TLabelMap>
::ProcessObject(unsigned int obj)
{
   IndexType index;
   IndexType startIndex;
   startIndex[1]= static_cast<unsigned int> ( ((obj) / m_NumberOfObjects[0]) * m_ObjectSize[1] );
   startIndex[0]= static_cast<unsigned int> ( ((obj) % m_NumberOfObjects[0]) * m_ObjectSize[0] );
   
   for(unsigned int col=startIndex[0]; col<startIndex[0]+m_ObjectSize[0]; col++)
   {
      for(unsigned int line=startIndex[1]; line<startIndex[1]+m_ObjectSize[1]; line++)
      {
         index[0]=col;
         index[1]=line;
         this->GetOutput()->AddPixel(index, m_Labels[obj]);
      }
   }
   this->GetOutput()->GetLabelObject(m_Labels[obj])->SetAttribute( "area", m_AreaVector[obj] );
   if(m_PathVector[obj]!="") this->GetOutput()->GetLabelObject(m_Labels[obj])->SetAttribute( "path", m_PathVector[obj] );
}

template <class TLabelMap>
void
SpatialisationFilter<TLabelMap>
::GenerateOutputInformation()
{

  Superclass::GenerateOutputInformation();

  RegionType region;
  SizeType size;
  IndexType index;
  index.Fill(0);
  size[0] = m_ObjectSize[0] * m_NumberOfObjects[0];
  size[1] = m_ObjectSize[1] * m_NumberOfObjects[1];
  region.SetSize(size);
  region.SetIndex(index);

  //    RegionType requestedRegion;
  //    size[0]=m_ObjectSize[0];
  //    size[1]=m_ObjectSize[1];
  //    requestedRegion.SetSize(size);
  //    requestedRegion.SetIndex(index);

  this->GetOutput()->SetLargestPossibleRegion(region);
  //    this->GetOutput()->SetRequestedRegion(requestedRegion);
}

template <class TLabelMap>
void
SpatialisationFilter<TLabelMap>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif

