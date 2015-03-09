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
#ifndef __otbVariableLengthVectorConverter_txx
#define __otbVariableLengthVectorConverter_txx

#include "otbVariableLengthVectorConverter.h"
#include <complex>


namespace otb
{

// Real Matrix
template< class TInternalInputType, class TPrecisionType >
typename VariableLengthVectorConverter< std::vector<std::vector<TInternalInputType> >, TPrecisionType>
::OutputType
VariableLengthVectorConverter< std::vector<std::vector<TInternalInputType> >, TPrecisionType>
::Convert(InputType input)
{
  unsigned int p, q;
  unsigned int count = 0;
  unsigned int rsltIdx = 0;

  OutputType result;

  p = input.size();

  for (unsigned int l=0; l<p; ++l)
    {
      count+=input.at(l).size();
    }

  result.SetSize(count);

  for (unsigned int i=0; i<p; ++i)
    {
      q = input.at(i).size();
      for (unsigned int j=0; j<q; ++j)
        {
          result[rsltIdx] = static_cast<OutputPrecisionType>(input.at(i).at(j));
          rsltIdx ++;
        }
    }

  return result;
}

// Complex Matrix
template< class TInternalInputType, class TPrecisionType >
typename VariableLengthVectorConverter< std::vector<std::vector<std::complex<TInternalInputType> > >,
                                        TPrecisionType>
::OutputType
VariableLengthVectorConverter< std::vector<std::vector<std::complex<TInternalInputType> > >,
                               TPrecisionType>
::Convert(InputType input)
{
  unsigned int p, q;
  unsigned int count = 0;
  unsigned int rsltIdx = 0;
  OutputType result;

  p = input.size();

  for (unsigned int l=0; l<p; ++l)
    {
      count+=input.at(l).size();
    }

  result.SetSize(count*2);

  for (unsigned int i=0; i<p; ++i)
    {
      q = input.at(i).size();
    for (unsigned int j=0; j<q; ++j)
      {
      result[rsltIdx] = static_cast<OutputPrecisionType>(input.at(i).at(j).real());
      rsltIdx ++;
      result[rsltIdx] = static_cast<OutputPrecisionType>(input.at(i).at(j).imag());
      rsltIdx ++;
      }
    }
  return result;
}

// Fixed Array
template< class TInternalInputType, unsigned int VArrayDimension, class TPrecisionType >
typename VariableLengthVectorConverter< itk::FixedArray<TInternalInputType, VArrayDimension>, TPrecisionType>
::OutputType
VariableLengthVectorConverter< itk::FixedArray<TInternalInputType, VArrayDimension>, TPrecisionType>
::Convert(InputType input)
{
  unsigned int rsltIdx = 0;
  OutputType result;

  result.SetSize(VArrayDimension);

  for (unsigned int i=0; i<VArrayDimension; ++i)
    {
    result[rsltIdx] = static_cast<OutputPrecisionType>(input[i]);
    rsltIdx ++;
    }
  return result;
}

// Histogram
template< class TPixel, class TPrecisionType >
typename VariableLengthVectorConverter< itk::SmartPointer< itk::Statistics::Histogram<TPixel> >,
                                        TPrecisionType>
::OutputType
VariableLengthVectorConverter< itk::SmartPointer< itk::Statistics::Histogram<TPixel> >,
                               TPrecisionType>
::Convert(InputType input)
{
  unsigned int rsltIdx = 0;
  itk::Size<1> nbBins;
  OutputType result;

  nbBins[0] = input->GetSize()[0];

  result.SetSize(nbBins[0]);

  for (unsigned int i=0; i<nbBins[0]; ++i)
    {
    result[rsltIdx] = static_cast<OutputPrecisionType>(input->GetFrequency(i));
    rsltIdx ++;
    }
  return result;
}

} // namespace otb

#endif
