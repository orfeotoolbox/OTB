/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. IMBO
  Language  :   C++
  Date      :   7 avril 2006
  Version   :   
  Role      :   Unitary test for the otb::FillGapsFilter class
  $Id$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include "otbFillGapsFilter.h"
#include "otbLineSpatialObjectList.h"


int otbFillGapsFilter( int argc, char ** argv )
{
  try 
    { 
	typedef otb::FillGapsFilter              FillGapsFilterType;
  	typedef otb::LineSpatialObjectList	 LinesListType;
  	typedef LinesListType::LineType	         LineType;


	FillGapsFilterType::Pointer fillgaps = FillGapsFilterType::New();
        
	LinesListType::Pointer   linesListBeforeFillGaps = LinesListType::New();
	const LinesListType *linesListAfterFillGaps ;

  	LineType::PointListType  pointList;
        LineType::LinePointType  point;

        // Definition of the first line
        double Ux, Uy, Vx, Vy;
        Ux = 10.;
        Uy = 10.;
        Vx = 10.;
        Vy = 20.;

        point.SetPosition(Ux,Uy);
        pointList.push_back(point);
        point.SetPosition(Vx,Vy);
        pointList.push_back(point);

        LineType::Pointer line = LineType::New();
        line->SetId(0);
        line->SetPoints( pointList );
        line->ComputeBoundingBox();
       
        linesListBeforeFillGaps->push_back(line);
        
        pointList.clear();

        // Definition of a second line

        Ux = 10.;
        Uy = 30.;
        Vx = 10.;
        Vy = 50.;

        point.SetPosition(Ux,Uy);
        pointList.push_back(point);
        point.SetPosition(Vx,Vy);
        pointList.push_back(point);
        
        LineType::Pointer line2 = LineType::New();
        line2->SetId(0);
        line2->SetPoints( pointList );
        line2->ComputeBoundingBox();
       
        linesListBeforeFillGaps->push_back(line2); 
        
        pointList.clear();

        //  FillGapsFilter parameters
	
	fillgaps->SetRadius(15.);
	fillgaps->SetAngularBeam(1.0);  // Angle in Radian
	fillgaps->SetInput(linesListBeforeFillGaps);
	linesListAfterFillGaps = fillgaps->GetOutput();
//	fillgaps->GenerateData();
	fillgaps->Update();
	        
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

