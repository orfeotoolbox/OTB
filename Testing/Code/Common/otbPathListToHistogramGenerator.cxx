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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#define MAIN


#include "itkExceptionObject.h"
#include "itkPolyLineParametricPath.h"
#include "otbOrientationPathFunction.h"
#include "otbPathListToHistogramGenerator.h"

int otbPathListToHistogramGenerator( int argc, char* argv[] )
{
  try 
    {         
        const   unsigned int        	                       Dimension = 2;
	typedef itk::PolyLineParametricPath< Dimension >       PathType;
        typedef PathType::Pointer                              PathPointer;
        typedef std::vector< PathPointer >   		       PathListType;
	
	typedef otb::OrientationPathFunction<PathType>         FunctionType;

        typedef otb::PathListToHistogramGenerator< PathType,FunctionType >   HistogramGeneratorType;
	
        PathType::ContinuousIndexType cindex;
	int NbAngle = 100;
	
        /* build segments list */
	PathListType*  PathList;
        PathList->clear();
	
	for(int i = 0 ; i <NbAngle ; i++)
	{
	    PathPointer pathElt = PathType::New();        
 	    pathElt->Initialize();
	    cindex[0]=30;
            cindex[1]=30;
            pathElt->AddVertex(cindex);
	    
	    float Theta = 2.0*acos(-1)*i/NbAngle;
            cindex[0]= 30 + cos(Theta);
            cindex[1]= 30 + sin(Theta);
            pathElt->AddVertex(cindex);
         
	    PathList->push_back(pathElt); 
	}

	
	HistogramGeneratorType::Pointer histogramGenerator = HistogramGeneratorType::New();

        typedef HistogramGeneratorType::SizeType   HistogramSizeType;
	HistogramSizeType hsize;
        hsize[0] = 127;  // number of bins for the Red   channel
	
        histogramGenerator->SetInput(  PathList  );
        histogramGenerator->SetNumberOfBins( hsize );
        histogramGenerator->SetMarginalScale( 10.0 );
        histogramGenerator->Compute();

        typedef HistogramGeneratorType::HistogramType  HistogramType;

        const HistogramType * histogram = histogramGenerator->GetOutput();

        const unsigned int histogramSize = histogram->Size();
        std::cout << "Histogram size " << histogramSize << std::endl;


       for( unsigned int bin=0; bin < histogramSize; bin++ )
       {
            std::cout << "bin = " << bin << " frequency = ";
            std::cout << histogram->GetFrequency( bin, 0 ) << std::endl;
       }
	
        
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "itk::ExceptionObject catch !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "unknown Exception catch !" << std::endl; 
    return EXIT_FAILURE;
    } 
  
  return EXIT_SUCCESS;
}
