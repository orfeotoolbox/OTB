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
  inline TOutput operator ()(const TInput& inputPixel) const;

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

  itkGetConstMacro(TM1, unsigned int);

  itkGetConstMacro(TM2, unsigned int);

  itkGetConstMacro(TM3, unsigned int);

  itkGetConstMacro(TM4, unsigned int);

  itkGetConstMacro(TM5, unsigned int);

  itkGetConstMacro(TM60, unsigned int);

  itkGetConstMacro(TM61, unsigned int);

  itkGetConstMacro(TM62, unsigned int);

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

  itkGetConstMacro(EpsilonToBeConsideredAsZero, double);

  /** Return the index name */
  virtual std::string GetName() const = 0;

protected:
  // This method must be reimplemented in subclasses to actually
  // compute the index value
  double m_EpsilonToBeConsideredAsZero;

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
 *  and ETM+ Images", IEEE Trans. on Geoscience and Remote Sensing,
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

  Bright() {}
  virtual ~Bright() {}

  inline TOutput operator ()(const TInput& inputPixel) 
  {

    double result = (inputPixel[this->m_TM1]+inputPixel[this->m_TM2]+2*inputPixel[this->m_TM3]+2*inputPixel[this->m_TM4]+inputPixel[this->m_TM5]+inputPixel[this->m_TM7])/8.0;
    return static_cast<TOutput>(result);
  }
  

};

/** \class Vis
 *  Visible index for LandsatTM. Computes a mean of the 3 visible
 *  bands as follows:
 *  \f[
 *    \frac{1}{3}\left( TM1 + TM2 + TM3 \right)}
 *  \f]
 *  This expression is the one used in Baraldi et al. 2006, "Automatic
 *  Spectral Rule-Based Preliminary Mapping of Calibrated Landsat TM
 *  and ETM+ Images", IEEE Trans. on Geoscience and Remote Sensing,
 *  vol 44, no 9.
 * 
 * \ingroup Functor
 * \ingroup Radiometry
 * \ingroup LandsatTMIndices
 */
template <class TInput, class TOutput>
class Vis : public LandsatTMIndexBase<TInput, TOutput>
{
public:
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "Vis";
  }

  Vis() {}
  virtual ~Vis() {}

  inline TOutput operator ()(const TInput& inputPixel) 
  {

    double result = (inputPixel[this->m_TM1]+inputPixel[this->m_TM2]+inputPixel[this->m_TM3])/3.0;
    return static_cast<TOutput>(result);
  }
  

};


/** \class NIR
 *  NIR index for LandsatTM. Alias for the TM4 band.
 * 
 * \ingroup Functor
 * \ingroup Radiometry
 * \ingroup LandsatTMIndices
 */
template <class TInput, class TOutput>
class NIR : public LandsatTMIndexBase<TInput, TOutput>
{
public:
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "NIR";
  }

  NIR() {}
  virtual ~NIR() {}

  inline TOutput operator ()(const TInput& inputPixel) 
  {

    double result = this->TM4;
    return static_cast<TOutput>(result);
  }
  

};

/** \class MIR1
 *  MIR1 index for LandsatTM. Alias for the TM5 band.
 * 
 * \ingroup Functor
 * \ingroup Radiometry
 * \ingroup LandsatTMIndices
 */
template <class TInput, class TOutput>
class MIR1 : public LandsatTMIndexBase<TInput, TOutput>
{
public:
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "MIR1";
  }

  MIR1() {}
  virtual ~MIR1() {}

  inline TOutput operator ()(const TInput& inputPixel) 
  {

    double result = this->TM5;
    return static_cast<TOutput>(result);
  }
  

};

/** \class MIR2
 *  MIR2 index for LandsatTM. Alias for the TM7 band.
 * 
 * \ingroup Functor
 * \ingroup Radiometry
 * \ingroup LandsatTMIndices
 */
template <class TInput, class TOutput>
class MIR2 : public LandsatTMIndexBase<TInput, TOutput>
{
public:
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "MIR2";
  }

  MIR2() {}
  virtual ~MIR2() {}

  inline TOutput operator ()(const TInput& inputPixel) 
  {

    double result = this->TM7;
    return static_cast<TOutput>(result);
  }
  

};

/** \class TIR
 *  TIR index for LandsatTM. This is TM60 for Landsat 5 and TM62 for Landsat 7.
 * 
 * \ingroup Functor
 * \ingroup Radiometry
 * \ingroup LandsatTMIndices
 */
template <class TInput, class TOutput>
class TIR : public LandsatTMIndexBase<TInput, TOutput>
{
public:
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "TIR";
  }

  TIR() {}
  virtual ~TIR() {}

  inline TOutput operator ()(const TInput& inputPixel) 
  {

    double result = this->TM62;

    if( this->SAT == L5 )
      result = this->TM60;
    
    return static_cast<TOutput>(result);
  }
  

};

/** \class MIRTIR
 *
 *  MIRTIR index for LandsatTM.
 *
 *  In general, clouds tend
 *  to be colder and feature a higher reflectance at the 1700 nm
 *  wavelength (TM5) than cold and highly reflective barren land. To
 *  enhance this difference, a well known TM band 5/6 composite is
 *  specifically designed for cloud detection in :
 *
 *  R. Irish, "Landsat 7 automatic cloud cover assessment (ACCA)",
 *  Poc. of SPIE, vol 4049, pp. 348-355, 2000.
 * 
 * \ingroup Functor
 * \ingroup Radiometry
 * \ingroup LandsatTMIndices
 */
template <class TInput, class TOutput>
class MIRTIR : public LandsatTMIndexBase<TInput, TOutput>
{
public:
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "MIRTIR";
  }

  MIRTIR() {}
  virtual ~MIRTIR() {}

  inline TOutput operator ()(const TInput& inputPixel) 
  {

    double tir = this->TM62;
    double mir1 = this->TM5;

    if( this->SAT == L5 )
      tir = this->TM60;

    double result = (1 - mir1)*tir;
    
    return static_cast<TOutput>(result);
  }
  

};

/** \class NDVI
 *
 *  NDVI index for LandsatTM.
 * 
 * \ingroup Functor
 * \ingroup Radiometry
 * \ingroup LandsatTMIndices
 */
template <class TInput, class TOutput>
class NDVI : public LandsatTMIndexBase<TInput, TOutput>
{
public:
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "NDVI";
  }

  NDVI() {}
  virtual ~NDVI() {}

  inline TOutput operator ()(const TInput& inputPixel) 
  {

    double result = (this->TM4 - this->TM3)/(this->TM4 + this->TM3 + this->m_EpsilonToBeConsideredAsZero);
    
    return static_cast<TOutput>(result);
  }
  

};

/** \class NDBSI
 *
 *  Normalized Difference Bare Soil index for LandsatTM.
 *
 *  \f[
 *    \frac{\left( TM5 - TM4 \right)}{\left( TM5 + TM4 + \epsilon \right)}
 *  \f]
 *
 *  This expression is the one used in Baraldi et al. 2006, "Automatic
 *  Spectral Rule-Based Preliminary Mapping of Calibrated Landsat TM
 *  and ETM+ Images", IEEE Trans. on Geoscience and Remote Sensing,
 *  vol 44, no 9.
 *
 *  NDBSI > -0.2 is a strong (necesary, but not sufficient)
 *  indication of the presence of bare soil areas.
 * 
 * \ingroup Functor
 * \ingroup Radiometry
 * \ingroup LandsatTMIndices
 */
template <class TInput, class TOutput>
class NDBSI : public LandsatTMIndexBase<TInput, TOutput>
{
public:
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "NDBSI";
  }

  NDBSI() {}
  virtual ~NDBSI() {}

  inline TOutput operator ()(const TInput& inputPixel) 
  {

    double result = (this->TM4 - this->TM3)/(this->TM4 + this->TM3 + this->m_EpsilonToBeConsideredAsZero);
    
    return static_cast<TOutput>(result);
  }
  

};

/** \class BIO
 *
 *  Another Bare Soil index for LandsatTM.
 *
 *  \f[
 *    \frac{\left( (TM5 + TM3) - (TM4 + TM1)  \right)}{\left( (TM5 + TM3) + (TM4 + TM1) \right)}
 *  \f]
 *
 *  This expression is the one used in: Roy PS, Miyatake S and Rikimaru
 *  A (1997) Biophysical Spectral Response Modelling Approach for
 *  Forest Density Stratification. FCD Mapper User’s Guide.
 * 
 * \ingroup Functor
 * \ingroup Radiometry
 * \ingroup LandsatTMIndices
 */
template <class TInput, class TOutput>
class BIO : public LandsatTMIndexBase<TInput, TOutput>
{
public:
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "BIO";
  }

  BIO() {}
  virtual ~BIO() {}

  inline TOutput operator ()(const TInput& inputPixel) 
  {

    double result = ((this->TM5 + this->TM3) + (this->TM4 + this->TM1))/((this->TM5 + this->TM3) - (this->TM4 + this->TM1));
    
    return static_cast<TOutput>(result);
  }
  

};


/** \class NDSI
 *
 *  Normalized Difference Snow index for LandsatTM.
 *
 *  \f[
 *    \frac{\left( TM2 - TM5 \right)}{\left( TM2 + TM5 + \epsilon \right)}
 *  \f]
 *
 *  This expression is the one used in R. Irish, "Landsat 7 automatic
 *  cloud cover assessment (ACCA)", Poc. of SPIE, vol 4049,
 *  pp. 348-355, 2000.
 * 
 * \ingroup Functor
 * \ingroup Radiometry
 * \ingroup LandsatTMIndices
 */
template <class TInput, class TOutput>
class NDSI : public LandsatTMIndexBase<TInput, TOutput>
{
public:
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "NDSI";
  }

  NDSI() {}
  virtual ~NDSI() {}

  inline TOutput operator ()(const TInput& inputPixel) 
  {

    double result = (this->TM2 - this->TM5)/(this->TM2 + this->TM5 + this->m_EpsilonToBeConsideredAsZero);
    
    return static_cast<TOutput>(result);
  }
  

};

/** \class NDSIVis
 *
 *  Baraldi's Normalized Difference Snow index for LandsatTM.
 *
 *  \f[
 *    \frac{\left( Vis - TM5 \right)}{\left( Vis + TM5 + \epsilon \right)}
 *  \f]
 *
 *  where
 *
 *  \f[
 *    Vis = \frac{1}{3}\left( TM1 + TM2 + TM3 \right)}
 *  \f]
 *
 *  This expression is the one used in Baraldi et al. 2006, "Automatic
 *  Spectral Rule-Based Preliminary Mapping of Calibrated Landsat TM
 *  and ETM+ Images", IEEE Trans. on Geoscience and Remote Sensing,
 *  vol 44, no 9.
 *
 *  and is an adaptation of the one given in R. Irish, "Landsat 7
 *  automatic cloud cover assessment (ACCA)", Poc. of SPIE, vol 4049,
 *  pp. 348-355, 2000.
 *
 *  NDSIVis > 0.5 is a strong (necesary, but not sufficient)
 *  indication of the presence of snow areas.
 * 
 * \ingroup Functor
 * \ingroup Radiometry
 * \ingroup LandsatTMIndices
 */
template <class TInput, class TOutput>
class NDSIVis : public LandsatTMIndexBase<TInput, TOutput>
{
public:
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "NDSIVis";
  }

  NDSIVis() {}
  virtual ~NDSIVis() {}

  inline TOutput operator ()(const TInput& inputPixel) 
  {

    double vis = (inputPixel[this->m_TM1]+inputPixel[this->m_TM2]+inputPixel[this->m_TM3])/3.0;
    double result = (vis - this->TM5)/(vis + this->TM5 + this->m_EpsilonToBeConsideredAsZero);
    
    return static_cast<TOutput>(result);
  }
  

};

/** \class NDBBBI
 *
 *  Normalized Difference Blue-band component in Built-up areas and
 *  Barren land index for LandsatTM.
 *
 *  \f[
 *    \frac{\left( TM1 - TM5 \right)}{\left( TM1 + TM5 + \epsilon \right)}
 *  \f]
 *
 *  This expression is the one used in Baraldi et al. 2006, "Automatic
 *  Spectral Rule-Based Preliminary Mapping of Calibrated Landsat TM
 *  and ETM+ Images", IEEE Trans. on Geoscience and Remote Sensing,
 *  vol 44, no 9.
 * 
 * \ingroup Functor
 * \ingroup Radiometry
 * \ingroup LandsatTMIndices
 */
template <class TInput, class TOutput>
class NDBBBI : public LandsatTMIndexBase<TInput, TOutput>
{
public:
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "NDBBBI";
  }

  NDBBBI() {}
  virtual ~NDBBBI() {}

  inline TOutput operator ()(const TInput& inputPixel) 
  {

    double result = (this->TM2 - this->TM5)/(this->TM2 + this->TM5 + this->m_EpsilonToBeConsideredAsZero);
    
    return static_cast<TOutput>(result);
  }
  

};


} // namespace LandsatTM
} // namespace Functor
} // namespace otb

#endif
