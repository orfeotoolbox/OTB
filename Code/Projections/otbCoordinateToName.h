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

namespace otb
{

/**
 * \class CoordinateToName
 * \brief Retrieve Geographical information for Longitude and Latitude coordinates
 *
 */


class ITK_EXPORT CoordinateToName : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef CoordinateToName                                      Self;
  typedef itk::SmartPointer<Self>                                Pointer;
  typedef itk::SmartPointer<const Self>                          ConstPointer;


  typedef itk::Object Superclass;

  itkTypeMacro(CoordinateToName, Object);
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  itkGetMacro( Lon, double );
  itkGetMacro( Lat, double );
  itkGetMacro( PlaceName, std::string );
  itkGetMacro( CountryName, std::string );

  itkSetMacro( Lon, double );
  itkSetMacro( Lat, double );

  virtual bool Evaluate();

protected:
  CoordinateToName();
  virtual ~CoordinateToName() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  void RetrieveXML(std::ostringstream& urlStream);
  void ParseXMLGeonames();

private:
  CoordinateToName( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  double m_Lon;
  double m_Lat;
  std::string m_PlaceName;
  std::string m_CountryName;
  std::string m_TempFileName;
};

} // namespace otb


#endif
