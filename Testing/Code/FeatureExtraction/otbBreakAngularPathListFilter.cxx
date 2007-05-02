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
#include "itkExceptionObject.h"
#include "otbBreakAngularPathListFilter.h"

#include "itkPolyLineParametricPath.h"
#include <fstream>
#include <vector>

int otbBreakAngularPathListFilter(int argc, char * argv[])
{
  try
    {
      const char * outfname = argv[1];
      const double maxAngular(static_cast<double>(::atof(argv[2])));
      
      typedef std::vector<double> PointsVectorType;
      typedef std::vector< PointsVectorType > PointsMatrixType;
      PointsMatrixType MatricePoints;
      PointsVectorType ListPoints;

      int cpt = 3;
      ListPoints.clear();
      
      while ( argv[cpt] != NULL )
      {
        if( argv[cpt][0] == '|' )
        {
                if( (ListPoints.size()%2) != 0 )
                {
                        itkGenericExceptionMacro(<<"Il manque une coordonnée d'un point dans la liste des parametres !");
                }
                MatricePoints.push_back(ListPoints);
                ListPoints.clear();
        }
        else
        {
                ListPoints.push_back(static_cast<double>(::atof(argv[cpt])));
        }
        cpt++;
      }
      MatricePoints.push_back(ListPoints);
      
      
        
      const unsigned int Dimension = 2;
      typedef itk::PolyLineParametricPath<Dimension> PathType;
      typedef otb::BreakAngularPathListFilter<PathType> BreakAngularPathListFilterType;
      typedef BreakAngularPathListFilterType::PathListType PathListType;
      PathType::ContinuousIndexType cindex;
      
      
      PathListType::Pointer InputPathList = PathListType::New();
      
        //Generate PathList
        for(PointsMatrixType::iterator listpos=MatricePoints.begin() ; listpos != MatricePoints.end() ; ++listpos)
        {
                PathType::Pointer path = PathType::New();
                //Generate PathList
                        std::cout << "List "<<std::endl;
                for(PointsVectorType::iterator it=(*listpos).begin() ; it != (*listpos).end() ; ++it)
                {
                        cindex[0] = *it;
                        ++it;
                        cindex[1] = *it;
                        std::cout << "Point Index :"<<cindex[0]<<";"<<cindex[1]<<std::endl;
                        path->AddVertex(cindex);
                }
                InputPathList->PushBack(path);
        }


      // Instantiating object
      BreakAngularPathListFilterType::Pointer breakAngularFilter = BreakAngularPathListFilterType::New();

      breakAngularFilter->SetInput(InputPathList);
      breakAngularFilter->SetMaxAngle(maxAngular*vcl_acos(-1.0)/180.);
      breakAngularFilter->Update();

      PathListType::Pointer OutputPathList = breakAngularFilter->GetOutput();

      typedef PathListType::ConstIterator PathListIteratorType;
      typedef PathType::VertexListType VertexListType;
      typedef VertexListType::ConstIterator VertexIteratorType;

      std::ofstream file;
      file.open(outfname);
      unsigned int counter = 1;
      PathListIteratorType pathListIt = InputPathList->Begin();
        
      file<<"MAX ANGULAR :"<<breakAngularFilter->GetMaxAngle()<< "("<<maxAngular<<" deg.)"<<std::endl;
      file<<"INPUT list of Path "<<": "<<std::endl;
      while(pathListIt!=InputPathList->End())
	{
	  file<<"Path "<<counter<<": ";
	  for(VertexIteratorType vIt = pathListIt.Get()->GetVertexList()->Begin();
	      vIt!=pathListIt.Get()->GetVertexList()->End();
	      ++vIt)
	    {
	      if(vIt!=pathListIt.Get()->GetVertexList()->Begin())
		{
		  file<<", ";
		}
	      file<<vIt.Value();
	    }
	  file<<std::endl;
	  ++pathListIt;
	  ++counter;
	}
        
      counter = 1;
      pathListIt = OutputPathList->Begin();
      file<<"OUTPUT list of Path "<<": "<<std::endl;
      while(pathListIt!=OutputPathList->End())
	{
	  file<<"Path "<<counter<<": ";
	  for(VertexIteratorType vIt = pathListIt.Get()->GetVertexList()->Begin();
	      vIt!=pathListIt.Get()->GetVertexList()->End();
	      ++vIt)
	    {
	      if(vIt!=pathListIt.Get()->GetVertexList()->Begin())
		{
		  file<<", ";
		}
	      file<<vIt.Value();
	    }
	  file<<std::endl;
	  ++pathListIt;
	  ++counter;
	}
      file.close();

    }
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject thrown !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "Unknown exception thrown !" << std::endl; 
    return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;
}
