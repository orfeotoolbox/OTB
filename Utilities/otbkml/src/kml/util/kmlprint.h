// Copyright 2008, Google Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//  3. Neither the name of Google Inc. nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Some KML printing utility functions.

#ifndef KMLPRINT_H__
#define KMLPRINT_H__

#include <string>

#include <fstream>
class ofstream_kmlprint
{
public:
        std::ofstream flux;
};
ofstream_kmlprint G_kmlprint;
#undef assert
#define assert(condition) \
        { \
        G_kmlprint.flux << "Condition in line " << __LINE__  <<" -> "<<(bool)(condition)<<std::endl; \
        if (!condition) \
        { \
                G_kmlprint.flux << "Error condition assert in " __FILE__ ", line " << __LINE__  <<std::endl; \
        } \
        }


namespace kmldom {
class Container;
class Feature;
}

// All PrintXXX() functions print to stdout.

// Print the given string indented according to depth.
void PrintIndented(std::string item, int depth);

// Print each Feature in the container recursively.
void PrintContainer(const kmldom::Container* container, int depth);

// Print the name of the Feature.  This recurses on Container.
void PrintFeature(const kmldom::Feature* feature, int depth);

#endif  // KMLPRINT_H__
