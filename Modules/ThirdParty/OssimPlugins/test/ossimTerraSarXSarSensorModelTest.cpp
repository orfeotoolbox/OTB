/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#if defined(__GNUC__) || defined(__clang__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Woverloaded-virtual"
#include "ossimTerraSarXSarSensorModel.h"
# pragma GCC diagnostic pop
#else
#include "ossimTerraSarXSarSensorModel.h"
#endif

int main(int argc, char * argv[])
{
  std::cout.precision(9);

  if(argc != 4)
    return EXIT_FAILURE;

  std::string annotationXml = argv[2];
  std::string geoXml = argv[3];
  bool inverse = atoi(argv[1]);

  ossimplugins::ossimTerraSarXSarSensorModel * sensor = new ossimplugins::ossimTerraSarXSarSensorModel();

  sensor->readAnnotationFile(annotationXml, geoXml);

  bool validate(false);

  if(inverse)
    {
    validate = sensor->autovalidateInverseModelFromGCPs();
    }
  else
    {
    validate = sensor->autovalidateForwardModelFromGCPs();
    }

  delete sensor;

  if(validate)
    return EXIT_SUCCESS;

  return EXIT_FAILURE;
}
