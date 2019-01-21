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


#ifndef EnvisatAsarRecordFactory_h
#define EnvisatAsarRecordFactory_h

#include <EnvisatAsar/EnvisatAsarRecord.h>
#include <map>
#include <string>

namespace ossimplugins
{

/**
 * @ingroup EnvisatAsar
 * @brief This class is a factory able to construct Record base classes given the id of the record wanted
 */
class EnvisatAsarRecordFactory
{
public:
  /**
   * @brief Constructor
   */
  EnvisatAsarRecordFactory();
  /**
   * @brief Destructor
   */
  ~EnvisatAsarRecordFactory();

  /**
   * @brief Adds a new Record type available in this factory
   * @param record Record to add in the factory
   * @param id Id of the new avalaible Record
   */
  void RegisterRecord(std::string id, EnvisatAsarRecord * record);

  /**
   * @brief Instanciatse a new Record
   * @param id Id of the Record we want to instantiate
   */
  EnvisatAsarRecord* Instantiate(std::string id);

protected:
  /**
   * @brief Contains all the available Records for the factory
   */
  std::map<std::string, EnvisatAsarRecord*> _availableRecords;

private:
};
}
#endif
