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
#ifndef __otbBonjour_h
#define __otbBonjour_h

#include <string>

namespace otb
{

/** \class Bonjour
 * \brief Cette classe permet simplement de stocker un message.
 * Elle est utilisee pour valider l'installation de l'OTB.
 *
 */
class Bonjour
{
public:
  /** Constructeur de base */
  Bonjour()
  {
  	m_message = "Hello OTB user !!!";
  }
  /** Destructeur */
  virtual ~Bonjour() throw() {}
    
  /** Methode pour recuperer le message stocke. */
  virtual std::string getMessage() const
  {
  	return m_message;	
  }  
private:
  /** Variable permettant de stocker le message */
  std::string m_message;

};

} // end namespace otb

#endif
