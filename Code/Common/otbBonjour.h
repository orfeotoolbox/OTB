/**
=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   21 decembre 2005
  Version   :   
  Role      :  
  $Id$ 

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
