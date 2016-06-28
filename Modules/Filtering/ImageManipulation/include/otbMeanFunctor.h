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
#ifndef otbMeanFunctor_h
#define otbMeanFunctor_h

namespace otb
{
namespace Functor
{
/** \class MeanFunctor
 *  \brief This functor generate the mean value of a component pixels
 *
 * \ingroup OTBImageManipulation
 */
template<class TInput, class TOutputValue>
class MeanFunctor
{
public:
  typedef MeanFunctor<TInput, TOutputValue> MeanFunctorType;

  MeanFunctor()
  {
  }

  ~MeanFunctor() {}
  inline TOutputValue operator ()(const TInput& inPix)
  {
    double value(0.);
    if (inPix.Size() <= 0) return static_cast<TOutputValue> (0.);
    for (unsigned int i = 0; i < inPix.Size(); ++i)
      {
      value += static_cast<double>(inPix[i]);
      }
    return static_cast<TOutputValue> (value / static_cast<double>(inPix.Size()));
  }

};

} // end namespace functor
} // end namespace otb

#endif
