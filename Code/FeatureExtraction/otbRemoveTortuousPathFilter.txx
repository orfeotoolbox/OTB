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
#ifndef _otbRemoveTortuousPathFilter_txx
#define _otbRemoveTortuousPathFilter_txx

#include "otbRemoveTortuousPathFilter.h"
#include "otbMacro.h"
#include "vnl/vnl_math.h"

namespace otb
{
  /**
   * Constructor
   */
  template <class TPath>
  RemoveTortuousPathFilter<TPath>
  ::RemoveTortuousPathFilter()
  {
    m_MeanDistanceThreshold = 1.0;
  }
  template <class TPath>
  double 
  RemoveTortuousPathFilter<TPath>
  ::ComputeMeanDistanceBetweenVertices(PathPointerType path)
  {
    double resp = 0.0;
    VertexListConstIteratorType beginIt = path->GetVertexList()->Begin();
    VertexListConstIteratorType beforeTheEndIt = path->GetVertexList()->End();
    --beforeTheEndIt;
    while (beginIt != beforeTheEndIt)
      {
	VertexType v1 = beginIt.Value();
	++beginIt;
	VertexType v2 = beginIt.Value();
	resp+=vcl_sqrt(vcl_pow(v1[0]-v2[0],2)+vcl_pow(v1[1]-v2[1],2));
      }

    double nbVertices = static_cast<double>(path->GetVertexList()->Size());
    resp = resp / (nbVertices-1);
    otbMsgDevMacro(<<"Mean distance: "<<resp);
    return resp;

  }
  template <class TPath>
  void
  RemoveTortuousPathFilter<TPath>
  ::GenerateData(void)
  {
   // input/output wiring
    PathListConstPointerType inputPtr = this->GetInput();
    PathListPointerType outputPtr = this->GetOutput();

    // path iterator typedef
    typedef typename PathListType::ConstIterator PathIteratorType;

    // iterator definition 
    PathIteratorType inputIt = inputPtr->Begin();
    while(inputIt != inputPtr->End())
      {
	if(this->ComputeMeanDistanceBetweenVertices(inputIt.Get())>=static_cast<double>(m_MeanDistanceThreshold))
	{
	  outputPtr->PushBack(inputIt.Get());
	}
	++inputIt;
      }
  }
  /**
   * PrintSelf Method
   */
  template <class TPath>
  void
  RemoveTortuousPathFilter<TPath>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
    os << indent << "MeanDistanceThreshold value: "<<m_MeanDistanceThreshold<<std::endl;
  }
} // End namespace otb
#endif
