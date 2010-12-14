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
#include "itkFixedArray.h"
#include "otbBandName.h"
#include "otbFuzzyVariable.h"
#include <vector>
#include <algorithm>

namespace otb
{

namespace Functor
{

namespace LandsatTM
{

/// Landsat 5 or 7
enum SATType {L5, L7};
/// Thermal band in Kelvin or Celsius
enum DegreeType {Kelvin, HundredsKelvin, Celsius};
/// Reflectances in thousands or in (0-1)
enum ReflectanceType {Thousands, Normalized};
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

  /// Constructor
  LandsatTMIndexBase() :  m_EpsilonToBeConsideredAsZero(0.0000001), m_TM1(0), m_TM2(1), m_TM3(2), m_TM4(3), m_TM5(4), m_TM60(5), m_TM61(5), m_TM62(6), m_TM7{7}, m_SAT(L7), m_Degree(Celsius), m_Reflectance(Normalized) {}
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

    this->m_SAT = sat;
    
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

  void SetDegree(DegreeType deg)
  {
    this->m_Degree = deg;
  }

  itkGetConstMacro(Degree, DegreeType);

  void SetReflectance(ReflectanceType ref)
  {
    this->m_Reflectance = ref;
  }

  itkGetConstMacro(Reflectance, ReflectanceType);

  itkGetConstMacro(EpsilonToBeConsideredAsZero, double);


protected:
  /// Prepare the values so they are normalized and in °C

  TInput PrepareValues(const TInput& inputPixel)
  {

    TInput newPix( inputPixel );
    if( this->m_Degree == Kelvin )
      {
      if(this->m_SAT == L5)
        {
        newPix[this->m_TM60] = newPix[this->m_TM60]-273.15;
        }
      else
        {
        newPix[this->m_TM61] = newPix[this->m_TM61]-273.15;
        newPix[this->m_TM62] = newPix[this->m_TM62]-273.15;
        }
      }
    else
      if( this->m_Degree == HundredsKelvin )
      {
            if(this->m_SAT == L5)
        {
        newPix[this->m_TM60] = newPix[this->m_TM60]/100.-273.15;
        }
      else
        {
        newPix[this->m_TM61] = newPix[this->m_TM61]/100.-273.15;
        newPix[this->m_TM62] = newPix[this->m_TM62]/100.-273.15;
        }
      }
    if( this->m_Reflectance == Thousands )
      {
      newPix[this->m_TM1] = newPix[this->m_TM1]/1000.;
      newPix[this->m_TM2] = newPix[this->m_TM2]/1000.;
      newPix[this->m_TM3] = newPix[this->m_TM3]/1000.;
      newPix[this->m_TM4] = newPix[this->m_TM4]/1000.;
      newPix[this->m_TM5] = newPix[this->m_TM5]/1000.;
      newPix[this->m_TM7] = newPix[this->m_TM7]/1000.;
      }

    return newPix;
  }

  
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
  DegreeType m_Degree;
  ReflectanceType m_Reflectance;

};


/** \class LandsatTMIndex
 * 
 * \ingroup Functor
 * \ingroup Radiometry
 * \ingroup LandsatTMIndices
 */
template <class TInput, class TOutput>
class LandsatTMIndex : public LandsatTMIndexBase<TInput, TOutput>
{
public:

  // Operator performing the work
  inline TOutput operator ()(const TInput& inputPixel) const;

  /** Return the index name */
  virtual std::string GetName() const = 0;

  LandsatTMIndex() {}
  virtual ~LandsatTMIndex() {}
  

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
class Bright : public LandsatTMIndex<TInput, TOutput>
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

    TInput newPixel(this->PrepareValues( inputPixel ));
    double result = (newPixel[this->m_TM1]+newPixel[this->m_TM2]+2*newPixel[this->m_TM3]+2*newPixel[this->m_TM4]+newPixel[this->m_TM5]+newPixel[this->m_TM7])/8.0;
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
class Vis : public LandsatTMIndex<TInput, TOutput>
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
    TInput newPixel(this->PrepareValues( inputPixel ));
    double result = (newPixel[this->m_TM1]+newPixel[this->m_TM2]+newPixel[this->m_TM3])/3.0;
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
class NIR : public LandsatTMIndex<TInput, TOutput>
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
    TInput newPixel(this->PrepareValues( inputPixel ));
    double result = newPixel[this->m_TM4];
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
class MIR1 : public LandsatTMIndex<TInput, TOutput>
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
    TInput newPixel(this->PrepareValues( inputPixel ));
    double result = newPixel[this->m_TM5];
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
class MIR2 : public LandsatTMIndex<TInput, TOutput>
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
    TInput newPixel(this->PrepareValues( inputPixel ));
    double result = newPixel[this->m_TM7];
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
class TIR : public LandsatTMIndex<TInput, TOutput>
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
    TInput newPixel(this->PrepareValues( inputPixel ));
    double result = newPixel[this->m_TM62];

    if( this->m_SAT == L5 )
      result = newPixel[this->m_TM60];
    
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
class MIRTIR : public LandsatTMIndex<TInput, TOutput>
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
    TInput newPixel(this->PrepareValues( inputPixel ));
    double tir = newPixel[this->m_TM62];
    double mir1 = newPixel[this->m_TM5];

    if( this->m_SAT == L5 )
      tir = newPixel[this->m_TM60];

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
class NDVI : public LandsatTMIndex<TInput, TOutput>
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
    TInput newPixel(this->PrepareValues( inputPixel ));
    double result = (newPixel[this->m_TM4] - newPixel[this->m_TM3])/
      (newPixel[this->m_TM4] + newPixel[this->m_TM3] + this->m_EpsilonToBeConsideredAsZero);
    
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
class NDBSI : public LandsatTMIndex<TInput, TOutput>
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
    TInput newPixel(this->PrepareValues( inputPixel ));
    double result = (newPixel[this->m_TM5] - newPixel[this->m_TM4])/
      (newPixel[this->m_TM5] + newPixel[this->m_TM4] + this->m_EpsilonToBeConsideredAsZero);
    
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
class BIO : public LandsatTMIndex<TInput, TOutput>
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
    TInput newPixel(this->PrepareValues( inputPixel ));
    double result = ((newPixel[this->m_TM5] + newPixel[this->m_TM3])
                     - (newPixel[this->m_TM4] + newPixel[this->m_TM1]))
      /((newPixel[this->m_TM5] + newPixel[this->m_TM3])
        + (newPixel[this->m_TM4] + newPixel[this->m_TM1]));
    
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
class NDSI : public LandsatTMIndex<TInput, TOutput>
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
    TInput newPixel(this->PrepareValues( inputPixel ));
    double result = (newPixel[this->m_TM2] - newPixel[this->m_TM5])
      /(newPixel[this->m_TM2] + newPixel[this->m_TM5] + this->m_EpsilonToBeConsideredAsZero);
    
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
class NDSIVis : public LandsatTMIndex<TInput, TOutput>
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
    TInput newPixel(this->PrepareValues( inputPixel ));
    double vis = (newPixel[this->m_TM1]+newPixel[this->m_TM2]+newPixel[this->m_TM3])/3.0;
    double result = (vis - newPixel[this->m_TM5])/(vis + newPixel[this->m_TM5] + this->m_EpsilonToBeConsideredAsZero);
    
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
class NDBBBI : public LandsatTMIndex<TInput, TOutput>
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
    TInput newPixel(this->PrepareValues( inputPixel ));
    double result = (newPixel[this->m_TM1] - newPixel[this->m_TM5])
      /(newPixel[this->m_TM1] + newPixel[this->m_TM5] + this->m_EpsilonToBeConsideredAsZero);
    return static_cast<TOutput>(result);
  }
  

};

/** \class LandsatTMLinguisticVariables
 *
 * Implementation of the linguistic labels assigned to fuzzy sets
 *  providing a complete partition of a feature space consisting of
 *  scalar variables representing specific Landsat TM indices as
 *  described in Baraldi et al. 2006, "Automatic Spectral Rule-Based
 *  Preliminary Mapping of Calibrated Landsat TM and ETM+ Images",
 *  IEEE Trans. on Geoscience and Remote Sensing, vol 44, no 9.
 *
 * The hard coded thresholds are taken from table III of the paper.
 * The implementation proposed here is not at all optimal in the sense
 * that otb::FuzzyVariables are used where simple thresholds would be
 * enough. This is done like this so this class can also provide (TODO)
 * membership values instead of crisp ones.
 *
 * \ingroup Functor
 * \ingroup Radiometry
 * \ingroup LandsatTMIndices
 */
template <class TInput>
class LinguisticVariables : public LandsatTMIndexBase<TInput, itk::FixedArray<unsigned int, 11> >
{
public:

  typedef typename TInput::ValueType PrecisionType;
  typedef typename itk::FixedArray<unsigned int, 11> OutputPixelType;
  typedef otb::FuzzyVariable<11, PrecisionType> FuzzyVarType;
  
  enum LinguisticValues {MINlv=0, Low=MINlv, Medium, MAXlv=2, High=MAXlv};
  enum Indices { MINid=0, bright=MINid, vis, nir, mir1, mir2, tir, mirtir, ndsivis, ndbbbi, ndvi, MAXid=10, ndbsi=MAXid };
  
    /** Return the index name */
  virtual std::string GetName() const
  {
    return "LandsatTM Linguistic Variables";
  }
  
  LinguisticVariables()
    {
    m_FvBright = FuzzyVarType::New();
    m_FvVis = FuzzyVarType::New();
    m_FvNIR = FuzzyVarType::New();
    m_FvMIR1 = FuzzyVarType::New();
    m_FvMIR2 = FuzzyVarType::New();
    m_FvTIR = FuzzyVarType::New();
    m_FvMIRTIR = FuzzyVarType::New();
    m_FvNDSIVis = FuzzyVarType::New();
    m_FvNDBBBI = FuzzyVarType::New();
    m_FvNDVI = FuzzyVarType::New();
    m_FvNDBSI = FuzzyVarType::New();

    PrecisionType maximumValue = itk::NumericTraits<PrecisionType>::max();
    PrecisionType minimumValue = itk::NumericTraits<PrecisionType>::NonpositiveMin();
    
    m_FvBright->SetMembership(Low, minimumValue, minimumValue, 40, 40);
    m_FvBright->SetMembership(Medium, 40, 40, 60, 60);
    m_FvBright->SetMembership(High, 60, 60, maximumValue, maximumValue);

    m_FvVis->SetMembership(Low, minimumValue, minimumValue, 30, 30);
    m_FvVis->SetMembership(Medium, 30, 30, 50, 50);
    m_FvVis->SetMembership(High, 50, 50, maximumValue, maximumValue);

    m_FvNIR->SetMembership(Low, minimumValue, minimumValue, 40, 40);
    m_FvNIR->SetMembership(Medium, 40, 40, 60, 60);
    m_FvNIR->SetMembership(High, 60, 60, maximumValue, maximumValue);

    m_FvMIR1->SetMembership(Low, minimumValue, minimumValue, 40, 40);
    m_FvMIR1->SetMembership(Medium, 40, 40, 60, 60);
    m_FvMIR1->SetMembership(High, 60, 60, maximumValue, maximumValue);

    m_FvMIR2->SetMembership(Low, minimumValue, minimumValue, 30, 30);
    m_FvMIR2->SetMembership(Medium, 30, 30, 50, 50);
    m_FvMIR2->SetMembership(High, 50, 50, maximumValue, maximumValue);

    m_FvTIR->SetMembership(Low, minimumValue, minimumValue, 0, 0);
    m_FvTIR->SetMembership(Medium, 0, 0, 28, 28);
    m_FvTIR->SetMembership(High, 28, 28, maximumValue, maximumValue);

    m_FvMIRTIR->SetMembership(Low, minimumValue, minimumValue, 18000, 18000);
    m_FvMIRTIR->SetMembership(Medium, 18000, 18000, 22000, 22000);
    m_FvMIRTIR->SetMembership(High, 22000, 22000, maximumValue, maximumValue);

    m_FvNDSIVis->SetMembership(Low, minimumValue, minimumValue, 0, 0);
    m_FvNDSIVis->SetMembership(Medium, 0, 0, 0.5, 0.5);
    m_FvNDSIVis->SetMembership(High, 0.5, 0.5, maximumValue, maximumValue);

    m_FvNDBBBI ->SetMembership(Low, minimumValue, minimumValue, -0.20, -0.20);
    m_FvNDBBBI->SetMembership(Medium, -0.20, -0.20, 0.10, 0.10);
    m_FvNDBBBI->SetMembership(High, 0.10, 0.10, maximumValue, maximumValue);

    m_FvNDVI->SetMembership(Low, minimumValue, minimumValue, 0.36, 0.36);
    m_FvNDVI->SetMembership(Medium, 0.36, 0.36, 0.7, 0.7);
    m_FvNDVI->SetMembership(High, 0.7, 0.7, maximumValue, maximumValue);

    m_FvNDBSI->SetMembership(Low, minimumValue, minimumValue, -0.20, -0.20);
    m_FvNDBSI->SetMembership(Medium, -0.20, -0.20, 0.10, 0.10);
    m_FvNDBSI->SetMembership(High, 0.10, 0.10, maximumValue, maximumValue);
    }
  virtual ~LinguisticVariables() {}

  inline itk::FixedArray<unsigned int, 11> operator ()(const TInput& inputPixel) 
  {
    TInput newPixel(this->PrepareValues( inputPixel ));
    itk::FixedArray<unsigned int, 11> result;
    
    m_FvBright->SetValue( Bright<TInput, PrecisionType>()( newPixel ) );
    result[ bright ] = m_FvBright->GetMaxVar();

    m_FvVis->SetValue( Vis<TInput, PrecisionType>()( newPixel ) );
    result[ vis ] = m_FvVis->GetMaxVar();

    m_FvNIR->SetValue( NIR<TInput, PrecisionType>()( newPixel ) );
    result[ nir ] = m_FvNIR->GetMaxVar();

    m_FvMIR1->SetValue( MIR1<TInput, PrecisionType>()( newPixel ) );
    result[ mir1 ] = m_FvMIR1->GetMaxVar();

    m_FvMIR2->SetValue( MIR2<TInput, PrecisionType>()( newPixel ) );
    result[ mir2 ] = m_FvMIR2->GetMaxVar();

    m_FvTIR->SetValue( TIR<TInput, PrecisionType>()( newPixel ) );
    result[ tir ] = m_FvTIR->GetMaxVar();

    m_FvMIRTIR->SetValue( MIRTIR<TInput, PrecisionType>()( newPixel ) );
    result[ mirtir ] = m_FvMIRTIR->GetMaxVar();

    m_FvNDSIVis->SetValue( NDSIVis<TInput, PrecisionType>()( newPixel ) );
    result[ ndsivis ] = m_FvNDSIVis->GetMaxVar();

    m_FvNDBBBI->SetValue( NDBBBI<TInput, PrecisionType>()( newPixel ) );
    result[ ndbbbi ] = m_FvNDBBBI->GetMaxVar();

    m_FvNDVI->SetValue( NDVI<TInput, PrecisionType>()( newPixel ) );
    result[ ndvi ] = m_FvNDVI->GetMaxVar();

    m_FvNDBSI->SetValue( NDBSI<TInput, PrecisionType>()( newPixel ) );
    result[ ndbsi ] = m_FvNDBSI->GetMaxVar();

    
    return result;
  }

protected:
  typename FuzzyVarType::Pointer m_FvBright;
  typename FuzzyVarType::Pointer m_FvVis;
  typename FuzzyVarType::Pointer m_FvNIR;
  typename FuzzyVarType::Pointer m_FvMIR1;
  typename FuzzyVarType::Pointer m_FvMIR2;
  typename FuzzyVarType::Pointer m_FvTIR;
  typename FuzzyVarType::Pointer m_FvMIRTIR;
  typename FuzzyVarType::Pointer m_FvNDSIVis;
  typename FuzzyVarType::Pointer m_FvNDBBBI;
  typename FuzzyVarType::Pointer m_FvNDVI;
  typename FuzzyVarType::Pointer m_FvNDBSI;
  

};


/** \class KernelSpectralRule
 *
 * Implementation of the Kernel Spectral rules for Landsat TM image
 *  land cover classification as described in table IV of Baraldi et
 *  al. 2006, "Automatic Spectral Rule-Based Preliminary Mapping of
 *  Calibrated Landsat TM and ETM+ Images", IEEE Trans. on Geoscience
 *  and Remote Sensing, vol 44, no 9.
 *
 * This is a virtual class defining the common accessors to the 14 spectral rules
 *
 *
 * \ingroup Functor
 * \ingroup Radiometry
 * \ingroup LandsatTMIndices
 */
template <class TInput>
class KernelSpectralRule : public LandsatTMIndexBase<TInput, bool >
{
public:

  typedef typename TInput::ValueType PrecisionType;
  typedef bool OutputPixelType;
  
    /** Return the index name */
  virtual std::string GetName() const
  {
    return "LandsatTM KernelSpectralRule";
  }
  
  KernelSpectralRule() : m_TV1(0.7), m_TV2(0.5) { }
  virtual ~KernelSpectralRule() {}

  void SetTV1(PrecisionType tv1)
  {
    this->m_TV1 = tv1;
  }

  void SetTV2(PrecisionType tv2)
  {
    this->m_TV2 = tv2;
  }

  itkGetConstMacro(TV1, PrecisionType);
  itkGetConstMacro(TV2, PrecisionType);
protected:
  /** Tolerance value 1*/
  PrecisionType m_TV1;
  /** Tolerance value 2*/
  PrecisionType m_TV2;

  PrecisionType m_Max123;
  PrecisionType m_Min123;

  void SetMinMax(const TInput& inputPixel)
  {
  std::vector< PrecisionType > v123;
  v123.push_back(inputPixel[this->m_TM1]);
  v123.push_back(inputPixel[this->m_TM2]);
  v123.push_back(inputPixel[this->m_TM3]);

  this->m_Max123 = *(max_element ( v123.begin(), v123.end() ));
  this->m_Min123 = *(min_element ( v123.begin(), v123.end() ));
  }
};

/** \class ThickCloudsSpectralRule
 *
 * Implementation of the ThickCloudsSpectralRule for Landsat TM image
 *  land cover classification as described in table IV of Baraldi et
 *  al. 2006, "Automatic Spectral Rule-Based Preliminary Mapping of
 *  Calibrated Landsat TM and ETM+ Images", IEEE Trans. on Geoscience
 *  and Remote Sensing, vol 44, no 9.
 *
 *
 * \ingroup Functor
 * \ingroup Radiometry
 * \ingroup LandsatTMIndices
 */
template <class TInput>
class ThickCloudsSpectralRule : public KernelSpectralRule<TInput>
{
public:

  typedef typename TInput::ValueType PrecisionType;
  typedef bool OutputPixelType;
  
    /** Return the index name */
  virtual std::string GetName() const
  {
    return "LandsatTM ThickCloudsSpectralRule";
  }
  
  ThickCloudsSpectralRule() { }
  virtual ~ThickCloudsSpectralRule() {}

  inline bool operator ()(const TInput& inputPixel)
  {
    TInput newPixel(this->PrepareValues( inputPixel ));
    this->SetMinMax(newPixel);

    bool result = (this->m_Min123 >= (this->m_TV1 * this->m_Max123))
      and (this->m_Max123 <= this->m_TV1 * newPixel[this->m_TM4])
      and (newPixel[this->m_TM5] <= this->m_TV1 * newPixel[this->m_TM4])
      and (newPixel[this->m_TM5] >= this->m_TV1 * this->m_Max123)
      and (newPixel[this->m_TM7] <= this->m_TV1 * newPixel[this->m_TM4]);

    return result;
  }

};

/** \class ThinCloudsSpectralRule
 *
 * Implementation of the ThinCloudsSpectralRule for Landsat TM image
 *  land cover classification as described in table IV of Baraldi et
 *  al. 2006, "Automatic Spectral Rule-Based Preliminary Mapping of
 *  Calibrated Landsat TM and ETM+ Images", IEEE Trans. on Geoscience
 *  and Remote Sensing, vol 44, no 9.
 *
 *
 * \ingroup Functor
 * \ingroup Radiometry
 * \ingroup LandsatTMIndices
 */
template <class TInput>
class ThinCloudsSpectralRule : public KernelSpectralRule<TInput>
{
public:

  typedef typename TInput::ValueType PrecisionType;
  typedef bool OutputPixelType;
  
    /** Return the index name */
  virtual std::string GetName() const
  {
    return "LandsatTM ThinCloudsSpectralRule";
  }
  
  ThinCloudsSpectralRule() { }
  virtual ~ThinCloudsSpectralRule() {}

  inline bool operator ()(const TInput& inputPixel)
  {
    TInput newPixel(this->PrepareValues( inputPixel ));
    this->SetMinMax(newPixel);

    bool result = (this->m_Min123 >= (this->m_TV1 * this->m_Max123))
      and (newPixel[this->m_TM4] >= this->m_Max123)
      and !((newPixel[this->m_TM1] <= newPixel[this->m_TM2]
             and newPixel[this->m_TM2] <= newPixel[this->m_TM3]
              and newPixel[this->m_TM3] <= newPixel[this->m_TM4])
            and (newPixel[this->m_TM3] >= this->m_TV1 * newPixel[this->m_TM4]))
      and (newPixel[this->m_TM4] >= this->m_TV1 * newPixel[this->m_TM5])
      and (newPixel[this->m_TM5] >= this->m_TV1 * newPixel[this->m_TM4])
      and (newPixel[this->m_TM5] >= this->m_TV1 * this->m_Max123)
      and (newPixel[this->m_TM5] >= this->m_TV1 * newPixel[this->m_TM7]);

    return result;
  }

};

/** \class SnowOrIceSpectralRule
 *
 * Implementation of the SnowOrIceSpectralRule for Landsat TM image
 *  land cover classification as described in table IV of Baraldi et
 *  al. 2006, "Automatic Spectral Rule-Based Preliminary Mapping of
 *  Calibrated Landsat TM and ETM+ Images", IEEE Trans. on Geoscience
 *  and Remote Sensing, vol 44, no 9.
 *
 *
 * \ingroup Functor
 * \ingroup Radiometry
 * \ingroup LandsatTMIndices
 */
template <class TInput>
class SnowOrIceSpectralRule : public KernelSpectralRule<TInput>
{
public:

  typedef typename TInput::ValueType PrecisionType;
  typedef bool OutputPixelType;
  
    /** Return the index name */
  virtual std::string GetName() const
  {
    return "LandsatTM SnowOrIceSpectralRule";
  }
  
  SnowOrIceSpectralRule() { }
  virtual ~SnowOrIceSpectralRule() {}

  inline bool operator ()(const TInput& inputPixel)
  {
    TInput newPixel(this->PrepareValues( inputPixel ));
    this->SetMinMax(newPixel);

    bool result = (this->m_Min123 >= (this->m_TV1 * this->m_Max123))
      and (newPixel[this->m_TM4] >= (this->m_TV1 * this->m_Max123))
      and (newPixel[this->m_TM5] <= this->m_TV2 * newPixel[this->m_TM4])
      and (newPixel[this->m_TM5] <= this->m_TV1 * this->m_Min123)
      and (newPixel[this->m_TM5] <= this->m_TV1 * this->m_Max123)
      and (newPixel[this->m_TM7] <= this->m_TV2 * newPixel[this->m_TM4])
      and (newPixel[this->m_TM7] <= this->m_TV1 * this->m_Min123);

    return result;
  }

};


/** \class WaterOrShadowSpectralRule
 *
 * Implementation of the WaterOrShadowSpectralRule for Landsat TM image
 *  land cover classification as described in table IV of Baraldi et
 *  al. 2006, "Automatic Spectral Rule-Based Preliminary Mapping of
 *  Calibrated Landsat TM and ETM+ Images", IEEE Trans. on Geoscience
 *  and Remote Sensing, vol 44, no 9.
 *
 *
 * \ingroup Functor
 * \ingroup Radiometry
 * \ingroup LandsatTMIndices
 */
template <class TInput>
class WaterOrShadowSpectralRule : public KernelSpectralRule<TInput>
{
public:

  typedef typename TInput::ValueType PrecisionType;
  typedef bool OutputPixelType;
  
    /** Return the index name */
  virtual std::string GetName() const
  {
    return "LandsatTM WaterOrShadowSpectralRule";
  }
  
  WaterOrShadowSpectralRule() { }
  virtual ~WaterOrShadowSpectralRule() {}

  inline bool operator ()(const TInput& inputPixel)
  {
    TInput newPixel(this->PrepareValues( inputPixel ));
    this->SetMinMax(newPixel);

    bool result = (newPixel[this->m_TM1] >= newPixel[this->m_TM2])
      and (newPixel[this->m_TM2] >= newPixel[this->m_TM3])
      and (newPixel[this->m_TM3] >= newPixel[this->m_TM4])
      and (newPixel[this->m_TM4] >= newPixel[this->m_TM5])
      and (newPixel[this->m_TM4] >= newPixel[this->m_TM7]);

    return result;
  }

};


/** \class PitbogOrGreenhouseSpectralRule
 *
 * Implementation of the PitbogOrGreenhouseSpectralRule for Landsat TM image
 *  land cover classification as described in table IV of Baraldi et
 *  al. 2006, "Automatic Spectral Rule-Based Preliminary Mapping of
 *  Calibrated Landsat TM and ETM+ Images", IEEE Trans. on Geoscience
 *  and Remote Sensing, vol 44, no 9.
 *
 *
 * \ingroup Functor
 * \ingroup Radiometry
 * \ingroup LandsatTMIndices
 */
template <class TInput>
class PitbogOrGreenhouseSpectralRule : public KernelSpectralRule<TInput>
{
public:

  typedef typename TInput::ValueType PrecisionType;
  typedef bool OutputPixelType;
  
    /** Return the index name */
  virtual std::string GetName() const
  {
    return "LandsatTM PitbogOrGreenhouseSpectralRule";
  }
  
  PitbogOrGreenhouseSpectralRule() { }
  virtual ~PitbogOrGreenhouseSpectralRule() {}

  inline bool operator ()(const TInput& inputPixel)
  {
    TInput newPixel(this->PrepareValues( inputPixel ));
    this->SetMinMax(newPixel);

    bool result = (newPixel[this->m_TM3] >= this->m_TV1 * newPixel[this->m_TM1])
      and (newPixel[this->m_TM1] >= this->m_TV1 * newPixel[this->m_TM3])
      and (this->m_Max123 <= this->m_TV1 * newPixel[this->m_TM4])
      and (newPixel[this->m_TM5] <= this->m_TV1 * newPixel[this->m_TM4])
      and (newPixel[this->m_TM3] >= this->m_TV2 * newPixel[this->m_TM5])
      and (this->m_Min123 >= this->m_TV1 * newPixel[this->m_TM7]);


    return result;
  }

};

} // namespace LandsatTM
} // namespace Functor
} // namespace otb

#endif
