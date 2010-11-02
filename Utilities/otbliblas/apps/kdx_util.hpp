#ifndef KDX_UTIL_HPP_INCLUDED
#define KDX_UTIL_HPP_INCLUDED

#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include <boost/concept_check.hpp>

#include <liblas/liblas.hpp>
#include <liblas/detail/fwd.hpp>
#include "chipper.hpp"

#include <fstream>
#include <sstream>
#include <exception>
#include <vector>
#include <sys/stat.h>




typedef std::vector<boost::uint32_t> IDVector;
typedef boost::shared_ptr< IDVector > IDVectorPtr;

class IndexResult 
{
public:
    IndexResult(boost::uint32_t id) : bounds(), m_id(id) {}

    IDVector const& GetIDs() const { return ids; }
    void SetIDs(IDVector& v) {ids = v;}
    const liblas::Bounds<double> GetBounds() const { return bounds; }
    void SetBounds(const liblas::Bounds<double> b) {bounds = b;}
    boost::uint32_t GetID() const {return m_id;}
    void SetID(boost::uint32_t v) {m_id = v;}
    
    void GetData(liblas::Reader* reader, std::vector<boost::uint8_t>& data);

private:

    // /// Copy constructor.
    // IndexResult(IndexResult const& other);
    // 
    // /// Assignment operator.
    // IndexResult& operator=(IndexResult const& rhs);
    
    IDVector ids;
    liblas::Bounds<double> bounds;
    boost::uint32_t m_id;

};

typedef std::vector<IndexResult> ResultsVector;
class KDXIndexSummary
{
public:
    KDXIndexSummary(std::istream& input);
    KDXIndexSummary(liblas::Reader& reader, boost::uint32_t capacity, bool verbose);
    boost::shared_ptr<liblas::Bounds<double > > bounds;
    ResultsVector& GetResults() { return m_results; }
private:
    ResultsVector m_results;
    bool m_first;    
};

bool KDTreeIndexExists(std::string& filename);


#endif // KDX_UTIL_HPP_INCLUDED

