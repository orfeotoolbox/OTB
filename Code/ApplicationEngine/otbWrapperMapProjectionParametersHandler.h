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
#ifndef __otbWrapperMapProjectionParametersHandler_h
#define __otbWrapperMapProjectionParametersHandler_h

#include "otbWrapperApplication.h"

namespace otb
{
namespace Wrapper
{

// enum used for the map projection choices
enum
{
  Map_Utm,
  Map_Lambert2,
  Map_Lambert93,
  //Map_Transmercator,
  Map_WGS84,
  Map_Epsg
};


/** \class MapProjectionParametersHandler
 *  \brief This class represent a helper class.
 *
 * This class allow creating a Group with several mapProjection
 * paramaters
 */
class MapProjectionParametersHandler
{
public:
  /**
    * Add a Group containing several choices for map projections
    *
    */
  static void AddMapProjectionParameters(Application::Pointer app, const std::string & key);

  /**
    * Helper method : Compute the ProjectionRef knowing the map
    * projection picked up by the user
    *
    */
  static const std::string GetProjectionRefFromChoice(const Application::Pointer app,
                                                      const std::string & key);
    

  /**
    * Helper method : Compute the UTM paramaters relative an image
    * Note: The key of the image must be set to be able to get the image.
    *       The key must be totally if the InputImageParameter belongs
    *       to a ParamaterGroup, ie set io.in
    */
  static void InitializeUTMParameters(Application::Pointer app,
                                      const std::string & imageKey,
                                      const std::string & mapKey );

protected:
  MapProjectionParametersHandler(); // not implemented
  virtual ~MapProjectionParametersHandler(); // not implemented
};

}
}


#endif // __otbWrapperMapProjectionParametersHandler_h_
