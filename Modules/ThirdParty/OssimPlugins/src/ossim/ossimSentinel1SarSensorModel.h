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


#ifndef ossimSentinel1SarSensorModel_HEADER
#define ossimSentinel1SarSensorModel_HEADER

#include <boost/config.hpp>
#include "ossim/ossimSarSensorModel.h"
#include <string>

class ossimXmlDocument;
class ossimString;

namespace ossimplugins
{

class OSSIM_PLUGINS_DLL ossimSentinel1SarSensorModel : public ossimSarSensorModel
{
public:

#if ! (defined(BOOST_NO_DEFAULTED_FUNCTIONS) || defined(BOOST_NO_CXX1_DEFAULTED_FUNCTIONS))
    /** Constructor */
    ossimSentinel1SarSensorModel()=default;

    /** Copy constructor */
    ossimSentinel1SarSensorModel(ossimSentinel1SarSensorModel const& m)=default;
    /** Move constructor */
    ossimSentinel1SarSensorModel(ossimSentinel1SarSensorModel && m)=default;

    /** Destructor */
    virtual ~ossimSentinel1SarSensorModel()=default;
#endif

    void readAnnotationFile(const std::string & annotationXml);

private:
    void readCoordinates(
            ossimXmlDocument const& xmlDoc, ossimString const& xpath,
            ossimString const& rg0_xpath, ossimString const& coeffs_xpath,
            std::vector<CoordinateConversionRecordType> & outputRecords
            );


protected:
        /*
           std::string theProductType;
           std::string theMode;
           std::string theSwath;
           std::string thePolarisation;
         */
};

} // end namespace

#endif
