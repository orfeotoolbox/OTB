
#include "itkMacro.h"

#include <iostream>
#include <cstdlib>
#include "svm.h"

namespace otb
{

class LinearKernelFunctor : public GenericKernelFunctorBase
{
public:
  typedef LinearKernelFunctor                 Self;
  typedef GenericKernelFunctorBase            Superclass;

  LinearKernelFunctor() : GenericKernelFunctorBase() {}
  virtual ~LinearKernelFunctor() {}

  // Deep copy operator
  virtual GenericKernelFunctorBase* Clone() const
  {
    return new Self(*this);
  }

  virtual double operator ()(const svm_node *x, const svm_node *y, const svm_parameter&) const
  {
    return this->dot(x, y);
  }

protected:
  LinearKernelFunctor(const Self& copy)
  : Superclass(copy)
  {
    *this = copy;
  }

  LinearKernelFunctor& operator=(const Self& copy)
  {
    Superclass::operator =(copy);
    return *this;
  }
};

}



/**
 * In this test, we approximate a 2-D scalar field.
 * The scattered data is derived from a segmented
 * image.  We write the output to an image for
 * comparison.
 */
int svmTest( int itkNotUsed(argc), char *argv[] )
{

  try
    {
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];

//        otb::LinearKernelFunctor lFunctor;

        struct svm_model* model;
        model = svm_load_model(inputFilename,NULL);
        if(model == 0)
        {
                itkGenericExceptionMacro( << "Problem while loading SVM model "
			 << std::string(inputFilename) );
        }

        if(svm_save_model(outputFilename, model)!=0)
        {
                itkGenericExceptionMacro( << "Problem while saving SVM model "
			 << std::string(outputFilename) );
        }

    }
  catch( itk::ExceptionObject & err )
    {
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl;
    std::cout << err << std::endl;
    return EXIT_FAILURE;
    }
  catch (...)
    {
    std::cerr << "svmTest exception thrown" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
};
