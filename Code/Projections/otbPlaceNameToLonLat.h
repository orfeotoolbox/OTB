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
#ifndef __otbPlaceNameToLonLat_h
#define __otbPlaceNameToLonLat_h

#include "itkObject.h"
#include "itkObjectFactory.h"

namespace otb
{

/**
 * \class PlaceNameToLonLat
 * \brief Retrieve Longitude and Latitude coordinates from a place name
 *

 */


class ITK_EXPORT PlaceNameToLonLat : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef PlaceNameToLonLat                                      Self;
  typedef itk::SmartPointer<Self>                                Pointer;
  typedef itk::SmartPointer<const Self>                          ConstPointer;


  typedef itk::Object Superclass;

  itkTypeMacro(PlaceNameToLonLat, Object);
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  itkGetMacro( Lon, double );
  itkGetMacro( Lat, double );
  itkGetMacro( PlaceName, std::string );

  itkSetMacro( Lon, double );
  itkSetMacro( Lat, double );
  itkSetMacro( PlaceName, std::string );

  typedef enum {ALL, GEONAMES, GOOGLE, YAHOO} SearchMethodEnum;//Not implemented yet TODO

  virtual bool Evaluate();

protected:
  PlaceNameToLonLat();
  ~PlaceNameToLonLat(){};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  void RetrieveXML(std::ostringstream& urlStream);
  void ParseXMLYahoo();
  void ParseXMLGoogle();
  void ParseXMLGeonames();//Not implemented yet TODO

private:
  PlaceNameToLonLat( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  double m_Lon;
  double m_Lat;
  std::string m_PlaceName;
  SearchMethodEnum m_SearchMethod;//Not implemented yet TODO
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPlaceNameToLonLat.txx"
#endif

#endif
