/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CNES - J.Inglada Rodriguez
  Language  :   C++
  Date      :   07 mars 2006
  Version   :   
  Role      :   Classe de suivi des commandes
  $Id: $

=========================================================================*/
#ifndef __otbCommandProgressUpdate_txx
#define __otbCommandProgressUpdate_txx


#include "otbCommandProgressUpdate.h"

namespace otb
{


template <class TFilter>
void CommandProgressUpdate<TFilter>::Execute(itk::Object *caller, const itk::EventObject & event)
{
      Execute( (const itk::Object *)caller, event);
    }

template <class TFilter>
void CommandProgressUpdate<TFilter>::Execute(const itk::Object * object, const itk::EventObject & event)
{
      FilterPointer filter = 
        dynamic_cast< FilterPointer >( object );
      if( typeid( event ) != typeid( itk::ProgressEvent ) )
        {
        return;
        }

      int factor = 160;

      int val = int(filter->GetProgress()*factor);
      
      if((val%2) == 0)
	{
	std::cout << "|";
	std::cout.flush();
	}

      if(val == factor)
	{
	std::cout << ">" ;
	std::cout.flush();
	}
    }



} // end namespace otb


#endif
