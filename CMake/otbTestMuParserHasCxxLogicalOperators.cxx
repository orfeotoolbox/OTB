#include <cstdlib>
#include "muParser.h"

int main(int argc, char *argv[])
{
  // Test "&&" operator
  // This will throw an exception if muParser version is < 2.0.0,
  // which have "and" instead

  mu::Parser parser;
  parser.SetExpr("0 && 1");

  try
    {
    double result = parser.Eval();
    }
  catch( const mu::Parser::exception_type& e )
    {
    std::cerr << "Message:     "   << e.GetMsg()   << std::endl
              << "Formula:     "   << e.GetExpr()  << std::endl
              << "Token:       "   << e.GetToken() << std::endl
              << "Position:    "   << e.GetPos()   << std::endl;
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
