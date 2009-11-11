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
#ifndef __otbCoordinateToName_h
#define __otbCoordinateToName_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkMultiThreader.h"

namespace otb
{

/**
 * \class CoordinateToName
 * \brief Retrieve geographical information for longitude and latitude coordinates
 *
 * This class can work in asynchronous mode using \code  MultithreadOn() \endcode. In this
 * case, the web request does not block the rest of the program.
 *
 */


class ITK_EXPORT CoordinateToName : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef CoordinateToName                 Self;
  typedef itk::SmartPointer<Self>          Pointer;
  typedef itk::SmartPointer<const Self>    ConstPointer;


  typedef itk::Object Superclass;

  itkTypeMacro(CoordinateToName, Object);
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  itkGetMacro( Lon, double );
  itkGetMacro( Lat, double );

  itkSetMacro( Lon, double );
  itkSetMacro( Lat, double );

  std::string GetPlaceName() const
  {
    if (m_IsValid)
    {
      return m_PlaceName;
    }
    else
    {
      return "";
    }
  }

  std::string GetCountryName() const
  {
    if (m_IsValid)
    {
      return m_CountryName;
    }
    else
    {
      return "";
    }
  }

  itkGetMacro(Multithread, bool);
  itkSetMacro(Multithread, bool);
  itkBooleanMacro(Multithread);

  virtual bool Evaluate();

protected:
  CoordinateToName();
  virtual ~CoordinateToName() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  void RetrieveXML(std::ostringstream& urlStream) const;
  void ParseXMLGeonames(std::string& placeName, std::string& countryName) const;

  virtual void DoEvaluate();

  static ITK_THREAD_RETURN_TYPE ThreadFunction(void*);

private:
  CoordinateToName( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  double m_Lon;
  double m_Lat;
  bool m_Multithread;
  bool m_IsValid;

  std::string m_PlaceName;
  std::string m_CountryName;
  std::string m_TempFileName;

  itk::MultiThreader::Pointer m_Threader;
};

} // namespace otb


#endif
