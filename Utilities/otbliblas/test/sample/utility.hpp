// $Id$
//
// (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#ifndef LIBLAS_SAMPLE_UTILITY_HPP_INCLUDED
#define LIBLAS_SAMPLE_UTILITY_HPP_INCLUDED
// std
#include <iosfwd>
#include <utility>
// Forward declarations
namespace liblas {
    class LASPoint;
}

template <typename T>
inline std::ostream& operator<<(std::ostream& os, typename std::pair<T, T> const& p)
{
    os << p.first << "   " << p.second;
    return os;
}

inline std::ostream& operator<<(std::ostream& os, liblas::LASPoint const& p)
{
    os << std::fixed << std::setprecision(6)
       << "\nx: " << p.GetX()
       << "\ny: " << p.GetY()
       << "\nz: " << p.GetZ()
       << "\nint: " << p.GetIntensity()
       << "\nrn: " << p.GetReturnNumber()
       << "\nnor: " << p.GetNumberOfReturns()
       << "\nsd: " << p.GetScanDirection()
       << "\neofl: " << p.GetFlightLineEdge()
       << "\ntime: " << p.GetTime()
       << std::endl;

    return os;
}

inline void print_point(std::ostream& os, liblas::LASPoint const& p, liblas::uint8_t const& minor)
{
    os << std::fixed << std::setprecision(6)
       << "\nx: " << p.GetX()
       << "\ny: " << p.GetY()
       << "\nz: " << p.GetZ()
       << "\nint: " << p.GetIntensity()
       << "\nrn: " << p.GetReturnNumber()
       << "\nnor: " << p.GetNumberOfReturns()
       << "\nsd: " << p.GetScanDirection()
       << "\neofl: " << p.GetFlightLineEdge()
       << "\ntime: " << p.GetTime();

    if (1 == minor)
    {
        const int len = 13;
        static std::string meaning[len] =
        {
            "Created, never classified",
            "Unclassified",
            "Ground",
            "Low Vegetation",
            "Medium Vegetation",
            "High Vegetation",
            "Building",
            "Low Point (noise)",
            "Model Key-point (mass point)",
            "Water",
            "Reserved for ASPRS Definition",
            "Reserved for ASPRS Definition",
            "Overlap Points",
        };

        // bit 0:4
        liblas::uint8_t chClass = p.GetClassification();
        unsigned int idx = (unsigned int)((chClass >> 3) & 0x1F); 
        std::cout << "\nClassification: "
            << ((idx >= 0 && idx < len) ? meaning[idx] : "Reserved for ASPRS Definition");
        std::cout << "\n- Synthetic: " << ((1 == (short)((chClass >> 2) & 0x1)) ? -1 : 0);
        std::cout << "\n- Model_Key_Point: " << ((1 == (short)((chClass >> 1) & 0x1)) ? -1 : 0);
        std::cout << "\n- Withheld: " << ((1 == (short)(chClass & 0x1)) ? -1 : 0);
    }
    else
    {
       std::cout << "\ncls: " << (int)p.GetClassification();
    }
    std::cout << std::endl;
}

#endif // LIBLAS_SAMPLE_UTILITY_HPP_INCLUDED
