
#include "itkMacro.h"

#include <iostream>
#include <cstdlib>
#include "svm.h"

namespace otb
{

class LinearKernelFunctor : public GenericKernelFunctorBase
{
public:
  LinearKernelFunctor(): GenericKernelFunctorBase() {};
  virtual ~LinearKernelFunctor() {};

  virtual double operator()(const svm_node *x, const svm_node *y, const svm_parameter& param)const
    {
      return this->dot(x,y);
    }
};

}



/**
 * In this test, we approximate a 2-D scalar field.
 * The scattered data is derived from a segmented
 * image.  We write the output to an image for
 * comparison.
 */
int svmGenericKernelFunctor( int argc, char *argv[] )
{

  try
    {

        otb::LinearKernelFunctor lFunctor;
        double d(4587.36);
        int i(145);
        std::string s("chaine");

        lFunctor.SetValue<double>("DOUBLE",d);
        lFunctor.SetValue<int>("INT",i);
        lFunctor.SetValue<std::string>("STRING",s);

        lFunctor.print_parameters();

        if( (   lFunctor.GetValue<double>("DOUBLE") != d) ||
            (   lFunctor.GetValue<int>("INT") != i) ||
            (   lFunctor.GetValue<std::string>("STRING") != s) )
        {
                return EXIT_FAILURE;
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















