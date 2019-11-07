/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
