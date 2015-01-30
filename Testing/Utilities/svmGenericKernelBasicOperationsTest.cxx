
#include "itkMacro.h"

#include <iostream>
#include <fstream>
#include "svm.h"
#include "otbSVMKernels.h"

int svmGenericKernelBasicOperationsTest( int itkNotUsed(argc), char *argv[] )
{
  const char * outputFileName = argv[1];

  struct svm_node* px;
  struct svm_node* py;
  struct svm_node* resSub;
  struct svm_node* resAdd;

  px = new struct svm_node[5];
  py = new struct svm_node[5];
  resSub = NULL;
  resAdd = NULL;

  px[0].index = 1;
  px[1].index = 1;
  px[2].index = 1;
  px[3].index = 1;
  px[4].index = -1;
  py[0].index = 1;
  py[1].index = 1;
  py[2].index = 1;
  py[3].index = 1;
  py[4].index = -1;

  px[0].value = 1.;
  px[1].value = 2.;
  px[2].value = 3.;
  px[3].value = 10.;
  px[4].value = 0.;
  py[0].value = -1.;
  py[1].value = 4.;
  py[2].value = 4.;
  py[3].value = 1.;
  py[4].value = 12.;

  GenericKernelFunctorBase gen;

  resSub=gen.sub(px, py);
  resAdd=gen.add(px, py);

  std::ofstream f;
  f.open(outputFileName);

  f<<"Support Vectors 1: (Index, Value)"<<std::endl;
  for(unsigned int n = 0;n<5;++n)
    {
      f<<"("<<px[n].index<<","<<px[n].value<<")"<<std::endl;
    }
  f<<std::endl;
  f<<"Support Vectors 2: (Index, Value)"<<std::endl;
  for(unsigned int n = 0;n<5;++n)
    {
        f<<"("<<py[n].index<<","<<py[n].value<<")"<<std::endl;
    }
  f<<std::endl;
  f<<"Substraction :"<<std::endl;
  for(unsigned int n = 0;n<5;++n)
    {
      f<<resSub[n].index<<" "<<resSub[n].value<<std::endl;
    }
  f<<std::endl;
  f<<"Addition :"<<std::endl;
  for(unsigned int n = 0;n<5;++n)
    {
      f<<resAdd[n].index<<" "<<resAdd[n].value<<std::endl;
    }

  f.close();

  delete [] px;
  delete [] py;
  delete [] resSub;
  delete [] resAdd;

  return EXIT_SUCCESS;
}
