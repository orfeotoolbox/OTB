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
#ifndef __otbDateTimeAdapter_h
#define __otbDateTimeAdapter_h

#include "itkObject.h"
#include "itkObjectFactory.h"

class ossimLocalTm;
//class ossimDate;
//class ossimTime;

namespace otb
{

/** \class DateTimeAdapter
 *  \brief This is a dummy class to the ossimLocalTm class
 *
 *  This class provide functions to define, compare and measure times and dates.
 *
 **/
class DateTimeAdapter : public itk::Object
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
  double GetSeconds();
  
  /** Set the date and time from an Iso8601 string
   *  Return true if the date is valid
   */
  bool SetFromIso8601(const std::string &date);
  
  /** Return the delta with an other date, expressed in seconds */
  double GetDeltaInSeconds(const DateTimeAdapter *pastDate);
  
  // TODO : add print self function
  
protected:
  DateTimeAdapter();
  virtual ~DateTimeAdapter();

private:
  DateTimeAdapter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  ossimLocalTm *m_LocalTm;

};

// TODO add extern '<<' function

} // namespace otb

#endif
