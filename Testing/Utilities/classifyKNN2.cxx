/*=========================================
 Copyright (c) Ruben Cardenes Almeida. August 2006
 This work is licensed under a
 Creative Commons Atribution 2.5 License
 http://creativecommons.org/licenses/by/2.5
 ==========================================*/

#include "otbImage.h"
#include "itkVector.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "classifyKNNCore.h"
#include "voronoiFilter.h"
#include <stdio.h>
#include <stdlib.h>
#include "itkMinimumMaximumImageCalculator.h"


#ifdef WIN32
#include <winsock.h>
#else
#include <sys/time.h>
#endif 

#ifndef UCHAR
#define UCHAR(c) ((unsigned char)(c))
#endif
#ifndef MAXDIM
#define MAXDIM 20
#endif
#ifndef MAXPATTERNS
#define MAXPATTERNS (16384*4)
#endif

#ifdef WIN32
void print_timing_Win(FILE *fp, double start, double end) 
{
  fprintf(fp,"Elapsed time: %g\n", (end - start) ); \
}
#else
void print_timing(FILE *fp, struct timeval start, struct timeval end) 
{
  double tuend = 1e-06*(double)end.tv_usec; \
  double tustart = 1e-06*(double)start.tv_usec; \
  double tend = end.tv_sec + tuend;\
  double tstart = start.tv_sec + tustart;\
  fprintf(fp,"Elapsed time: %g\n", (tend - tstart) ); \
}
#endif 

int countFloatsInString(const char *fltString)
/* char *flts - character array of floats 
 Return -1 on a malformed string
 Return the count of the number of floating point numbers
*/
{
  char *end;
  const char *start = fltString;
  double d;
  int count = 0;
  while ((UCHAR(*start) != '\0') && isspace(UCHAR(*start))) { start++; }
  if (UCHAR(*start) == '\0') return -1; /* Don't ask to convert empty strings */
  do {
    d = strtod(start, (char **)&end);
    if (end == start) { 
      /* I want to parse strings of numbers with comments at the end */
      /* This return is executed when the next thing along can't be parsed */
      return count; 
    } 
    count++;   /* Count the number of floats */
    start = end;  /* Keep converting from the returned position. */
    while ((UCHAR(*start) != '\0') && isspace(UCHAR(*start))) { start++; }
  } while (UCHAR(*start) != '\0');
  return count; /* Success */
}

int getFloatString(int numFloats, const char *flts, float *tgts)
/* char *flts - character array of floats */
/* float *tgts - array to save floats */
{
  char *end;
  const char *start = flts;
  double d;
  int count = 0;
  if (countFloatsInString(flts) != numFloats) return 1;
  while ((UCHAR(*start) != '\0') && isspace(UCHAR(*start))) { start++; }
  do {
    d = strtod(start, (char **)&end);
    if (end == start) { 
      /* Can't do any more conversions on this line */
      return (count == numFloats) ? 0 : 1;
    }
    tgts[count++] = d;   /* Convert from double to float */
    start = end;  /* Keep converting from the returned position. */
    while ((UCHAR(*start) != '\0') && isspace(UCHAR(*start))) { start++; }
    if (count == numFloats) return 0; /* I don't care if there are more on
			the line as long as I got what I wanted */
  } while (UCHAR(*start) != '\0');
  return 0; /* Success */
}

int classifyKNN2( int argc, char *argv[] ){
 struct timeval start;
 struct timeval end;
  if( argc!= 6 ){
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " Prototypes Ch1ImageFile Ch2ImageFile OutputFile K" << std::endl;
    return 1;
  }
  
  const unsigned int K = atoi(argv[5]);
  //Definition of the pixel types for each image
  typedef int InputPixelType;
  typedef unsigned short Ch1PixelType; 
  typedef unsigned short Ch2PixelType;
  typedef unsigned short OutputPixelType;
  typedef float DiagramPixelType;
  // Dimension espacial:
  const unsigned int Dimension=3;
  // Dimension en intensidad:
  const unsigned int Dimension_int = 2;

#ifdef WIN32
  double startWin = GetTickCount()/1000.0f; 
#else
  gettimeofday(&start,NULL);
#endif

  //Definition of the Image types
  typedef otb::Image<InputPixelType, Dimension> InputImageType;
  typedef otb::Image<Ch1PixelType, Dimension> Ch1ImageType;
  typedef otb::Image<Ch2PixelType, Dimension> Ch2ImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  typedef otb::Image<InputPixelType, Dimension_int> VoronoiInputType;
  typedef otb::Image< std::vector<int> , Dimension_int>  DiagramImageType;
  typedef VoronoiInputType::IndexType IndexType;

  //Definition of the Domain and Prototypes readers 
  typedef otb::ImageFileReader< InputImageType  >  InputReaderType;
  typedef otb::ImageFileReader< Ch1ImageType  >  Ch1ReaderType;
  typedef otb::ImageFileReader< Ch2ImageType  >  Ch2ReaderType;
  typedef otb::ImageFileWriter< OutputImageType  >  OutputWriterType;
  
  // En una dimension porque solo hay un canal: 
  typedef itk::voronoiFilter<VoronoiInputType, VoronoiInputType::PixelType, Dimension_int> VoronoiFilterType;
  
  typedef itk::classifyKNNCore<Ch1ImageType, Ch2ImageType, DiagramImageType, OutputImageType, Dimension> classifyKNNCoreType;
  
  VoronoiFilterType::Pointer voronoi = VoronoiFilterType::New();
  classifyKNNCoreType::Pointer KNNCore = classifyKNNCoreType::New();
  DiagramImageType::Pointer diagram= DiagramImageType::New();
  
  Ch1ReaderType::Pointer ch1Reader = Ch1ReaderType::New();
  ch1Reader->SetFileName( argv[2] );
  ch1Reader->Update(); 
  
  Ch2ReaderType::Pointer ch2Reader = Ch2ReaderType::New();
  ch2Reader->SetFileName( argv[3] );
  ch2Reader->Update(); 

  DiagramImageType::IndexType index;
  InputImageType::IndexType index_espacial;
  index.Fill(0);
  index_espacial.Fill(0);

  classifyKNNCoreType::ListType proto;
  classifyKNNCoreType::NodeType auxNode;

  index[0]=0;
  index[1]=0;

  itk::MinimumMaximumImageCalculator< Ch1ImageType >::Pointer Ch1Calculator=itk::MinimumMaximumImageCalculator<  Ch1ImageType  >::New();
  Ch1Calculator->SetImage(ch1Reader->GetOutput());
  itk::MinimumMaximumImageCalculator< Ch2ImageType >::Pointer Ch2Calculator=itk::MinimumMaximumImageCalculator<  Ch2ImageType  >::New();
  Ch2Calculator->SetImage(ch2Reader->GetOutput());
  
  ch1Reader->Update();
  ch2Reader->Update();

//LECTURA DE PROTOTIPOS
struct nodeDataNew {
  int id;
  int pclass;
  float d[MAXDIM];
  int row;
  int col;
  int slice;
};
  int pclass = -1;
  //float prob = 0.0;
  int pdim = 0;
//  int* marker = (int*)malloc(sizeof(int)*input->GetRequestedRegion().GetSize(0)*input->GetRequestedRegion().GetSize(1));
  char *straux,*end2;
  FILE *fp = NULL;
  char buffer[2048];
  struct nodeDataNew prototypeNodeData[MAXPATTERNS];
  int numPrototypes=0;

  fp = fopen(argv[1],"r");
  if (fp == NULL) {
    fprintf(stderr,"Failed reading prototypes %s\n",argv[1]);
    return 1;
  }
  int sum = 0;
//  short id=0;
  while (!feof(fp)) {
    //while (meca) {
      memset(buffer,0,sizeof(buffer));
      if (fgets(buffer, sizeof(buffer),fp) != (char *)NULL) {
        if (sscanf(buffer,"%d", &pclass) != 1) {
          /* Skip blank lines */
          continue;
        }
	memset(buffer,0,sizeof(buffer));

	/* Anadido para obtener la informacion espacial de los prototipos de entrenamiento */	
	if (fgets(buffer, sizeof(buffer),fp) != (char *)NULL) {
	  pdim = countFloatsInString(buffer);
	  if (getFloatString(pdim,buffer,prototypeNodeData[numPrototypes].d) != 0) {
	    fprintf(stderr,"Failed to parse float string\n");
	    return -1; /* failure */
	  }

	  straux = strstr(buffer,"row-");
	  //index_espacial[1]=strtod(straux+4,(char**)&end2);//row
	  index_espacial[1]=strtol(straux+4,(char**)&end2,10);//row
	  
	  straux = strstr(buffer,"col-");
	  //index_espacial[0]=strtod(straux+4,(char**)&end2);//col
	  index_espacial[0]=strtol(straux+4,(char**)&end2,10);//col
       	}

	for (int i=0; i<pdim; i++) {
	  index[i] = (int)prototypeNodeData[numPrototypes].d[i];
	}

	auxNode.index_space=index_espacial;
	auxNode.index_intensity=index;
	auxNode.clase=pclass;
	proto.push_back(auxNode);
      }
      sum++;
  }
  fclose(fp);
  std::cout << "num prototipos: " << sum-1 << std::endl; 

  //END READING PROTOTYPES

  IndexType v_index;
  v_index.Fill(0);
  VoronoiInputType::RegionType diagramRegion;
  diagramRegion.SetIndex(v_index);
  VoronoiInputType::SizeType size;
  Ch1Calculator->ComputeMaximum();
  Ch2Calculator->ComputeMaximum();
  size[0]=Ch1Calculator->GetMaximum()+1;
  size[1]=Ch2Calculator->GetMaximum()+1;
  //if (size[0] < 256) size[0] = 256;
  diagramRegion.SetSize(size);
  std::cout << "inputRegion del Test = " << size << std::endl;
  VoronoiInputType::Pointer inputvoronoi = VoronoiInputType::New();
  inputvoronoi->SetRegions(diagramRegion);
  inputvoronoi->Allocate();   
  inputvoronoi->FillBuffer(-1);   

    for (unsigned int i=0; i<proto.size(); i++){
      inputvoronoi->SetPixel(proto[i].index_intensity,i);
    }

    voronoi->SetBackgroundValue(-1);
    voronoi->SetInput(inputvoronoi);
    voronoi->SetK(K);
    diagram = voronoi->GetOutput();
    voronoi->Update();

    //IndexType featureIndex,mapIndex;
    //classifyKNNCoreType::NodeType auxNodo;

    KNNCore->SetDiagram(diagram);
    KNNCore->SetCh1(ch1Reader->GetOutput());
    KNNCore->SetCh2(ch2Reader->GetOutput());
    KNNCore->SetPrototipos(proto);
    KNNCore->SetK(K);
    // KNNCore->SetMaxClass(maxClass);
    KNNCore->Update();

    OutputWriterType::Pointer outputWriter = OutputWriterType::New();
    outputWriter->SetFileName( argv[4] );
    outputWriter->SetInput( KNNCore->GetOutput() );
    // outputWriter->SetInput( diagram1 );
    
    try
      {
	outputWriter->Update();    
      }
    catch( itk::ExceptionObject & ee )
      {
	std::cerr << "Exception caught " << std::endl;
	std::cerr << ee << std::endl;
	return EXIT_FAILURE;
      }

#ifdef WIN32
   double endWin = GetTickCount()/1000.0f; 
   print_timing_Win(stdout, startWin, endWin);
#else
   gettimeofday(&end,NULL);
   print_timing(stdout, start, end);
#endif
  
   return EXIT_SUCCESS;
}//end main 
