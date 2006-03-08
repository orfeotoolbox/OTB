/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CNES - J.Inglada Rodriguez
  Language  :   C++
  Date      :   07 mars 2006
  Version   :   
  Role      :   Classe de suivi des commandes
  $Id: $

=========================================================================*/
#ifndef __ProgressObserver_h
#define __ProgressObserver_h

#include "itkCommand.h"
#include <sstream>
#include <iostream>

namespace otb
{

template <class TFilter>
class CommandProgressUpdate : public itk::Command 
{
public:
  typedef  CommandProgressUpdate   Self;
  typedef  itk::Command             Superclass;
  typedef  itk::SmartPointer<Self>  Pointer;
  itkNewMacro( Self );
protected:
  CommandProgressUpdate() {};
public:
  typedef   const TFilter   *           FilterPointer;

  void Execute(itk::Object *caller, const itk::EventObject & event);

  void Execute(const itk::Object * object, const itk::EventObject & event);
};


} // end namespace otb


#ifndef ITK_MANUAL_INSTANTIATION
#include "otbCommandProgressUpdate.txx"
#endif

#endif
