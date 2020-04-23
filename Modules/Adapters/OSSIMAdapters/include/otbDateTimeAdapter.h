/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbDateTimeAdapter_h
#define otbDateTimeAdapter_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "OTBOSSIMAdaptersExport.h"

class ossimLocalTm;
// class ossimDate;
// class ossimTime;

namespace otb
{

/** \class DateTimeAdapter
 *  \brief This is a dummy class to the ossimLocalTm class
 *
 *  This class provide functions to define, compare and measure times and dates.
 *
 *
 * \ingroup OTBOSSIMAdapters
 **/
class OTBOSSIMAdapters_EXPORT DateTimeAdapter : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef DateTimeAdapter               Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DateTimeAdapter, itk::Object);

  /** Year Accessor*/
  int GetYear();

  /** Month Accessor*/
  int GetMonth();

  /** Day Accessor*/
  int GetDay();

  /** Hour Accessor*/
  int GetHour();

  /** Minute Accessor*/
  int GetMinute();

  /** Seconds Accessor*/
  double GetSeconds() const;

  /** Set the date and time from an Iso8601 string
   *  Return true if the date is valid
   */
  bool SetFromIso8601(const std::string& date);

  /** Return the delta with an other date, expressed in seconds */
  double GetDeltaInSeconds(const DateTimeAdapter* pastDate);

  // TODO : add print self function

protected:
  DateTimeAdapter();
  ~DateTimeAdapter() override;

private:
  DateTimeAdapter(const Self&) = delete;
  void operator=(const Self&) = delete;

  ossimLocalTm* m_LocalTm;
};

// TODO add extern '<<' function

} // namespace otb

#endif
