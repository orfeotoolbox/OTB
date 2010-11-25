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
#ifndef __otbLandsatTMIndices_h
#define __otbLandsatTMIndices_h

#include "otbMath.h"
#include "itkVariableLengthVector.h"
#include "otbBandName.h"

namespace otb
{

namespace Functor
{

namespace LandsatTM
{

enum SATType {L5, L7};
/**
   * \class LandsatTMIndexBase
   * \brief Base class for Landsat-TM indices
   *
   *  Implement operators for UnaryFunctorImageFilter templated with a
   *  vector pixel having 8 components. The order of the components
   *  defaults to the one of Lansat 7, but the Landsat 5 order is also
   *  available. Any other order of bands can be manually selected.
   *  Subclasses should overload operator().
   *
   * \ingroup Radiometry
 */
template<class TInput, class TOutput>
class LandsatTMIndexBase
{
public:

  //operators !=
  bool operator !=(const LandsatTMIndexBase&) const
  {
    return false;
  }
  //operator ==
  bool operator ==(const LandsatTMIndexBase& other) const
  {
    return !(*this != other);
  }
  // Operator performing the work
  inline TOutput operator ()(const TInput& inputPixel) const = 0;

  /// Constructor
  LandsatTMIndexBase() :  m_EpsilonToBeConsideredAsZero(0.0000001), m_TM1(0), m_TM2(1), m_TM3(2), m_TM4(3), m_TM5(4), m_TM60(5), m_TM61(5), m_TM62(6), m_TM7{7}, m_SAT(L7) {}
  /// Desctructor
  virtual ~LandsatTMIndexBase() {}

  /** Set index, generic method */
  void SetIndex(BandName::LandsatTMBandNames band, unsigned int channel)
  {
    switch( band )
      {
      case BandName::TM1:
        m_TM1 = channel;
        break;
      case BandName::TM2:
        m_TM2 = channel;
        break;
      case BandName::TM3:
        m_TM3 = channel;
        break;
      case BandName::TM4:
        m_TM4 = channel;
        break;
      case BandName::TM5:
        m_TM5 = channel;
        break;
      case BandName::TM60:
        m_TM60 = channel;
        break;
      case BandName::TM61:
        m_TM61 = channel;
        break;
      case BandName::TM62:
        m_TM62 = channel;
        break;
      case BandName::TM7:
        m_TM7 = channel;
        break;
      }
  }
  /** Get index, generic method */
  unsigned int GetIndex(BandName::LandsatTMBandNames band) const
  {
    switch( band )
      {
      case BandName::TM1:
        return m_TM1;
        break;
      case BandName::TM2:
        return m_TM2;
        break;
      case BandName::TM3:
        return m_TM3;
        break;
      case BandName::TM4:
        return m_TM4;
        break;
      case BandName::TM5:
        return m_TM5;
        break;
      case BandName::TM60:
        return m_TM60;
        break;
      case BandName::TM61:
        return m_TM61;
        break;
      case BandName::TM62:
        return m_TM62;
        break;
      case BandName::TM7:
        return m_TM7;
        break;
      }
    return m_TM1;
  }

  itkSetMacro(TM1, unsigned int);
  itkGetConstMacro(TM1, unsigned int);

  itkSetMacro(TM2, unsigned int);
  itkGetConstMacro(TM2, unsigned int);

  itkSetMacro(TM3, unsigned int);
  itkGetConstMacro(TM3, unsigned int);

  itkSetMacro(TM4, unsigned int);
  itkGetConstMacro(TM4, unsigned int);

  itkSetMacro(TM5, unsigned int);
  itkGetConstMacro(TM5, unsigned int);

  itkSetMacro(TM60, unsigned int);
  itkGetConstMacro(TM60, unsigned int);

  itkSetMacro(TM61, unsigned int);
  itkGetConstMacro(TM61, unsigned int);

  itkSetMacro(TM62, unsigned int);
  itkGetConstMacro(TM62, unsigned int);

  itkSetMacro(TM7, unsigned int);
  itkGetConstMacro(TM7, unsigned int);

  void SetSAT(SATType sat)
  {
    if( sat == L5 )
      {
      m_TM1 = 0;
      m_TM2 = 1; 
      m_TM3 = 2;
      m_TM4 = 3; 
      m_TM5 = 4; 
      m_TM60 = 5;
      m_TM7 = 6;
      m_SAT = L5;
      }
    else
      {
      m_TM1 = 0;
      m_TM2 = 1; 
      m_TM3 = 2;
      m_TM4 = 3; 
      m_TM5 = 4; 
      m_TM61 = 5;
      m_TM62 = 6;
      m_TM7 = 7;
      m_SAT = L7;
      }
  }
  itkGetConstMacro(SAT, SATType);

  /** Return the index name */
  virtual std::string GetName() const = 0;

protected:
  // This method must be reimplemented in subclasses to actually
  // compute the index value
  double m_EpsilonToBeConsideredAsZero;

private:
  unsigned int m_TM1;
  unsigned int m_TM2;
  unsigned int m_TM3;
  unsigned int m_TM4;
  unsigned int m_TM5;
  unsigned int m_TM60;
  unsigned int m_TM61;
  unsigned int m_TM62;
  unsigned int m_TM7;

  SATType m_SAT;

};


/** \class Bright
 *  Brightness index for LandsatTM. Computes a weighted mean of the
 *  non thermal bands as follows :
 *  \f[
 *    \frac{1}{8}\left( TM1 + TM2 + 2\times TM3 + 2\times TM4 + TM5 + TM7\right)
 *  \f]
 *  This expression is the one used in Baraldi et al. 2006, "Automatic
 *  Spectral Rule-Based Preliminary Mapping of Calibrated Landsat TM
 *  and ETMµ+ Images", IEEE Trans. on Geoscience and Remote Sensing,
 *  vol 44, no 9.
 * 
 * \ingroup Functor
 * \ingroup Radiometry
 * \ingroup LandsatTMIndices
 */
template <class TInput, class TOutput>
class Bright : public LandsatTMIndexBase<TInput, TOutput>
{
public:
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "Bright";
  }

  typedef NDVI<TInput, TOutput> NDVIFunctorType;
  Bright() {}
  virtual ~Bright() {}

  inline TOutput operator ()(const TInput& inputPixel) 
  {

    double result = inputPixel[TM1]+inputPixel[TM2]+2*inputPixel[TM3]+2*inputPixel[TM4]+inputPixel[TM5]+inputPixel[TM7];
    return static_cast<TOutput>(result);
  }
  

};

} // namespace LandsatTM
} // namespace Functor
} // namespace otb

#endif
