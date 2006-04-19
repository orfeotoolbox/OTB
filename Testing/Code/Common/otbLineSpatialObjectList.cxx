/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   11 janvier 2005
  Version   :   
  Role      :   Test de la classe LineSpatialObjectList
  $Id$

=========================================================================*/

#include "itkExceptionObject.h"

#include "otbLineSpatialObjectList.h"
#include <list>

int otbLineSpatialObjectList( int argc, char ** argv )
{
  try 
    { 
        typedef otb::LineSpatialObjectList                                      LineSpatialObjectListType;
        typedef LineSpatialObjectListType::LineType                             LineSpatialObjecType;
        typedef LineSpatialObjectListType::LineType::PointListType              PointListType;
        typedef LineSpatialObjectListType::const_iterator                       LineSpatialObjectListConstIterator;

        LineSpatialObjectListType::Pointer listLines = LineSpatialObjectListType::New();
        for( int i = 0 ; i < 10 ; i++ )
        {
                LineSpatialObjecType::Pointer lLine = LineSpatialObjecType::New();
                listLines->push_back( lLine );
        }
        
        LineSpatialObjectListConstIterator lIter;
        lIter = listLines->begin();
        while( lIter != listLines->end() )
        {
                LineSpatialObjecType::Pointer lLine = (*lIter);
                PointListType lPoints = lLine->GetPoints();
                lIter++;
        }
        
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "Exception levee inconnue !" << std::endl; 
    return EXIT_FAILURE;
    } 

  return EXIT_SUCCESS;
}


