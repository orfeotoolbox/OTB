/**
 * Copyright 0000 <Nobody>
 * @file
 * @author David Matthew Mattli <dmattli@usgs.gov>
 *
 * @section LICENSE
 *
 * This software is in the public domain, furnished "as is", without
 * technical support, and with no warranty, express or implied, as to
 * its usefulness for any purpose.
 *
 * @section DESCRIPTION
 *
 * Helper utilities and definitions
 *
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <cmath>

namespace sptw {
    /** An enum of possible return values */
    enum PRB_ERROR {
        PRB_NOERROR, /*!< No error occurred */
        PRB_IOERROR, /*!< Error communicating or performing file I/O */
        PRB_BADARG,  /*!< Bad argument provided */
        PRB_PROJERROR, /*!< Error with projection specification */
    };

    enum ProjUnit {
        UNDEF = -1,
        RADIAN = 0,  //  Radians
        FEET,        //  Feet
        METER,       //  Meters
        SECOND,      //  Seconds
        DEGREE,      //  Decimal Degrees
        INT_FEET     //  International Feet
    };

    /*! Coordinate struct
      This class provides a more readable way of storing and passing coordinate
      parameters for the Transformer class. It stores x and y as doubles and units
      corresponds to the GCTP enumeration as defined in constants.h
      */
    struct Coordinate {
        /*! Default Constructor
          0's all attributes.
          */
        Coordinate():x(0.0), y(0.0), units(UNDEF) {}
        /*! Full Constructor
          Sets all attributes in the Coordinate according to the parameters.
          */
        Coordinate(double xx, double yy, ProjUnit uunits)
            : x(xx), y(yy), units(uunits) {}

        /*! Copy Constructor
          Sets all attributes to equal those in Coordinate c.
          */
        Coordinate(const Coordinate &c): x(c.x), y(c.y), units(c.units) {}

        double x;
        double y;
        ProjUnit units;
    };

    /** 
     * @class 
     */
    struct Area {
        /// A constructor
        /** 
         * @brief This constructor initializes points to zero.
         */
        Area():ul(), lr(), units(UNDEF) {}
        /// A constructor
        /**
         * @brief This constructor allows both coordinates to be given initial values.
         * @param ulx Value for the upper-left x 
         * @param uly Value for the upper-left y
         * @param ulx Value for the lower-right x 
         * @param ulx Value for the lower-right y
         */
        Area(double ulx,
                double uly,
                double lrx,
                double lry) : ul(ulx, uly, UNDEF), lr(lrx, lry, UNDEF), units(UNDEF) {}
        /// Upper-left coordinate of area
        Coordinate ul;
        /// Lower-right coordinate of area
        Coordinate lr;
        /// Units used in ul and lr coordinates
        ProjUnit units;
    };
}

#endif// _UTILS_H_
