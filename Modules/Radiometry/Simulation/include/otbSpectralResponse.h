/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbSpectralResponse_h
#define otbSpectralResponse_h

#include "itkDataObject.h"
#include <itkObjectFactory.h>
#include <vector>
#include <utility>
#include <fstream>
#include <limits>

#include "otbVectorImage.h"
#include "itkImageRegionIterator.h"

#include "otbAtmosphericCorrectionParameters.h"

#include <boost/shared_ptr.hpp>

namespace otb
{
/** \class SpectralResponse
 * \brief This class represents the spectral response of an object (or a satellite band).
 *
 * This class contains a SORTED std::vector of std::pair values.
 * The first argument of the pair is a wavelength lambda (in micro meters).
 * The second argument of the pair is the reflectance response associated to lambda.
 *
 * The two templates indicate:
 * - the precision of the wavelength (X) (by default: double),
 * - the precision of the reflectance associated to the wavelength (Y) (by default: double).
 *
 * This Spectral response can be loaded from ASCII file (JPL spectral library Version 1.2)
 * http://speclib.jpl.nasa.gov.
 * \sa itk::DataObject
 *
 * \ingroup OTBSimulation
 */
template<class TPrecision = double, class TValuePrecision = double>
class SpectralResponse: public itk::DataObject
{
public:
  /** Standard class typedefs */
  typedef SpectralResponse Self;
  typedef itk::DataObject Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Template parameters typedef */
  typedef TPrecision PrecisionType;
  typedef TValuePrecision ValuePrecisionType;

  typedef std::pair<TPrecision, TValuePrecision> PairType;
  //typedef boost::shared_ptr<PairType> PairPointerType;
  typedef typename std::vector<PairType> VectorPairType;

  /** Standard image typedef */
  typedef otb::VectorImage<TValuePrecision, 2> ImageType;
  typedef typename ImageType::Pointer ImagePointerType;
  typedef itk::ImageRegionIterator<ImageType> IteratorType;

  /** Standard filter functions values typedef */
  typedef otb::FilterFunctionValues FilterFunctionValuesType;
  typedef itk::SmartPointer<FilterFunctionValuesType> FilterFunctionValuesPointerType;

  typedef std::pair<TPrecision, TPrecision> IntervalType;
  /** Standard macros */
  itkNewMacro(Self);
  itkTypeMacro(SpectralResponse, DataObject);


  itkSetMacro(SensitivityThreshold, TPrecision);
  itkGetConstMacro(SensitivityThreshold, TPrecision);

  itkSetMacro(UsePosGuess, bool);
  itkGetConstMacro(UsePosGuess, bool);


  /** Clear the vector of data pairs  */
  virtual bool Clear();

  /** Return the number of element in the vector (number of pairs) */
  virtual unsigned int Size() const;

  /** Fill from a ASCII file (CD-ROM ASTER Spectral library Version 1.2)*/
  void Load(const std::string & filename, ValuePrecisionType coefNormalization = 1.0);

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** get vector of spectral responses */
  VectorPairType & GetResponse()
  {
    return m_Response;
  }

  void SetResponse(const VectorPairType& resp)
  {
    m_Response = resp;
  }

  /**
   * \param PrecisionType
   * \return The interpolate value of the SR (for now a simple linear interpolation is used).
   */
  inline ValuePrecisionType operator()(const PrecisionType & lambda);


  /** Operator for comparing Pair Lambda/Response
   * Pairs are ordered by wavelength
   */
  struct sort_pair
  {
    bool operator()(PairType a, PairType b)
    {
      return a.first < b.first;
    }
  };

  /** Drop null value of reflectance in the Vector */
  //void DropNullReflectance() {}

  /** Transform the SR in a image to compute the TOA reflectance*/
  ImagePointerType GetImage(ImagePointerType image) const;
  /** Transform the image to an SR structure*/
  void SetFromImage(ImagePointerType image);

  /** Transform the SR in a Filter Function (6S parameters)*/
  FilterFunctionValuesPointerType GetFilterFunctionValues(double step = 0.0025);

  /** Get the wavelength of the first and last not null reflectance  */
  IntervalType GetInterval()
  {
    if (!m_IntervalComputed) this->ComputeInterval();
    return m_Interval;
  }

  /** set index in m_Response vector to accelerate () operator **/
  void SetPosGuessMin(const PrecisionType & lambda);


protected:
  /** Constructor */
  SpectralResponse();
  /** Constructor from a ASCII file */
  //SpectralResponse( const std::string & filename );
  /** Destructor */
  ~SpectralResponse() override
  {
  }
;
  /** PrintSelf method */
  //void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Spectral response vector of pairs (first=lambda; second=response)*/
  VectorPairType m_Response;

  /** Minimum value to consider that the spectral response is not null  */
  TPrecision m_SensitivityThreshold;
  IntervalType m_Interval;
  unsigned long m_PosGuess;
  bool m_IntervalComputed;
  bool m_UsePosGuess;
  void ComputeInterval();

private:
  SpectralResponse(const Self&) = delete;
  void operator=(const Self&) = delete;

};

}// end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSpectralResponse.hxx"
#endif

#endif
