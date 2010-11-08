#include "las2oci.hpp"

#include <liblas/detail/private_utility.hpp>
#include "laskernel.hpp"

typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

namespace po = boost::program_options;

using namespace liblas;
using namespace std;

 void CPL_STDCALL OCIGDALDebugErrorHandler(CPLErr eErrClass, int err_no, const char *msg)
{
    ostringstream oss;
    
    if (eErrClass == CE_Failure || eErrClass == CE_Fatal) {
        oss <<"GDAL Failure number=" << err_no << ": " << msg;
        throw std::runtime_error(oss.str());
    } else if (eErrClass == CE_Debug) {
        std::cout <<"GDAL Debug: " << msg << std::endl;
    } else {
        return;
    }
}

 void CPL_STDCALL OCIGDALErrorHandler(CPLErr eErrClass, int err_no, const char *msg)
{
    ostringstream oss;
    
    if (eErrClass == CE_Failure || eErrClass == CE_Fatal) {
        oss <<"GDAL Failure number=" << err_no << ": " << msg;
        throw std::runtime_error(oss.str());
    } else {
        return;
    }
}


void SetGDALErrorHandler(bool debug)
{
    CPLPopErrorHandler();
    if (debug)
        CPLPushErrorHandler(OCIGDALDebugErrorHandler);
    else
        CPLPushErrorHandler(OCIGDALErrorHandler);

}








void SetElements(   OWStatement* statement,
                    OCIArray* sdo_elem_info, 
                    bool bUseSolidGeometry)
{
    

    statement->AddElement(sdo_elem_info, 1);
    if (bUseSolidGeometry == true) {
        //"(1,1007,3)";
        statement->AddElement(sdo_elem_info, 1007);
    } else {
        //"(1,1003,3)";
        statement->AddElement(sdo_elem_info, 1003);
    }

    statement->AddElement(sdo_elem_info, 3);
 
}

void SetOrdinates(   OWStatement* statement,
                     OCIArray* sdo_ordinates, 
                     liblas::Bounds<double> const& extent)
{
    
    statement->AddElement(sdo_ordinates, extent.min(0));
    statement->AddElement(sdo_ordinates, extent.min(1));
    if (extent.dimension() > 2)
        statement->AddElement(sdo_ordinates, extent.min(2));
    
    statement->AddElement(sdo_ordinates, extent.max(0));
    statement->AddElement(sdo_ordinates, extent.max(1));
    if (extent.dimension() > 2)
        statement->AddElement(sdo_ordinates, extent.max(2));
        

}


// bool FillBlock( OWStatement* statement,
//                 IndexResult& result, 
//                 liblas::Reader* reader,
//                 blocks* b,
//                 long index,
//                 int srid, 
//                 long pc_id,
//                 long gtype,
//                 bool bUseSolidGeometry,
//                 bool bUse3d,
//                 long nDimensions
//               )
// {
// 
// 
//     // list<SpatialIndex::id_type> const& ids = result.GetIDs();
//     IDVector const& ids = result.GetIDs();
//     
//     b->pc_ids[index] = pc_id;
//     b->srids[index] = (long)srid;
//     b->block_ids[index] = result.GetID();
//     b->num_points[index] = (long)ids.size();
//     
//     std::vector<uint8_t>* blob = new std::vector<uint8_t>;
//     result.GetData(reader, *blob);
// 
//     
//     b->blobs[index] = blob;
//     // // FIXME: null srids not supported 
//     b->srids[index] = srid;
//     b->gtypes[index] = gtype;
//     // 
//     OCIArray* sdo_elem_info=0;
//     statement->GetConnection()->CreateType(&sdo_elem_info, statement->GetConnection()->GetElemInfoType());
//     SetElements(statement, sdo_elem_info, bUseSolidGeometry);
//     // 
//     b->element_arrays[index] = sdo_elem_info;
//     
//     OCIArray* sdo_ordinates=0;
//     statement->GetConnection()->CreateType(&sdo_ordinates, statement->GetConnection()->GetOrdinateType());
//     // 
//     // 
//     // 
//     SetOrdinates(statement, sdo_ordinates, result.GetBounds());
//     
//     b->coordinate_arrays[index] = sdo_ordinates;
//     
// 
//     return true;
// }


// blocks* CreateBlock(int size)
// {
//     blocks* b = (blocks*) malloc( sizeof(blocks));
//     
//     b->pc_ids = (long*) malloc( size * sizeof(long));
//     b->block_ids = (long*) malloc ( size * sizeof(long));
//     b->num_points = (long*) malloc ( size * sizeof(long));
//     b->blobs = (std::vector<uint8_t>**) malloc ( size * sizeof(std::vector<uint8_t>*));
//     b->locators =(OCILobLocator**) malloc( sizeof(OCILobLocator*) * size );
// 
//     b->srids = (long*) malloc ( size * sizeof(long));
//     b->gtypes = (long*) malloc ( size * sizeof(long));
// 
//     b->element_arrays = (OCIArray**) malloc ( size * sizeof(OCIArray*));
// 
//     b->coordinate_arrays = (OCIArray**) malloc ( size * sizeof(OCIArray*));
// 
//     b->size = size;
//     
//     return b;
// }

long GetGType(  bool bUse3d,
                bool bUseSolidGeometry)
{
    long gtype = 0;
    if (bUse3d) {
        if (bUseSolidGeometry == true) {
            gtype = 3008;

        } else {
            gtype = 3003;
        }
    } else {
        if (bUseSolidGeometry == true) {
            gtype = 2008;
        } else {
            gtype = 2003;
        }
    }
    
    return gtype;   
}

bool InsertBlock(OWConnection* connection, 
                IndexResult& result, 
                int srid, 
                liblas::Reader* reader, 
                const char* tableName,
                long pc_id,
                bool bUseSolidGeometry,
                bool bUse3d)
{
    ostringstream oss;

    IDVector const& ids = result.GetIDs();
    // const SpatialIndex::Region* b = result.GetBounds();
    boost::uint32_t num_points = ids.size();


    // EnableTracing(connection);
    
    long gtype = GetGType(bUse3d, bUseSolidGeometry);

    oss << "INSERT INTO "<< tableName << 
            "(OBJ_ID, BLK_ID, NUM_POINTS, POINTS,   "
            "PCBLK_MIN_RES, BLK_EXTENT, PCBLK_MAX_RES, NUM_UNSORTED_POINTS, PT_SORT_DIM) "
            "VALUES ( :1, :2, :3, :4, 1, mdsys.sdo_geometry(:5, :6, null,:7, :8)" 
            ", 1, 0, 1)";
          
    OWStatement* statement = 0;
    // TODO: If gotdata == false below, this memory probably leaks --mloskot
    OCILobLocator** locator =(OCILobLocator**) VSIMalloc( sizeof(OCILobLocator*) * 1 );

    statement = connection->CreateStatement(oss.str().c_str());
    

    
    long* p_pc_id = (long*) malloc( 1 * sizeof(long));
    p_pc_id[0] = pc_id;

    long* p_result_id = (long*) malloc( 1 * sizeof(long));
    p_result_id[0] = (long)result.GetID();
    
    long* p_num_points = (long*) malloc (1 * sizeof(long));
    p_num_points[0] = (long)num_points;
    
    
    // :1
    statement->Bind( p_pc_id );
    
    // :2
    statement->Bind( p_result_id );

    // :3
    statement->Bind( p_num_points );
       
    // :4
    statement->Define( locator, 1 ); 

    std::vector<boost::uint8_t> data;
    result.GetData(reader, data);
        
    // std::vector<liblas::uint8_t> data;
    // bool gotdata = GetResultData(result, reader, data, 3);
    // if (! gotdata) throw std::runtime_error("unable to fetch point data byte array");

    statement->Bind((char*)&(data[0]),(long)data.size());

    // :5
    long* p_gtype = (long*) malloc (1 * sizeof(long));
    p_gtype[0] = gtype;

    statement->Bind(p_gtype);
    
    // :6
    long* p_srid  = 0;
    
    
    if (srid != 0) {
        p_srid = (long*) malloc (1 * sizeof(long));
        p_srid[0] = srid;
    }
    statement->Bind(p_srid);
    
    // :7
    OCIArray* sdo_elem_info=0;
    connection->CreateType(&sdo_elem_info, connection->GetElemInfoType());
    SetElements(statement, sdo_elem_info, bUseSolidGeometry);    
    statement->Bind(&sdo_elem_info, connection->GetElemInfoType());
    
    // :8
    OCIArray* sdo_ordinates=0;
    connection->CreateType(&sdo_ordinates, connection->GetOrdinateType());
    
     // x0, x1, y0, y1, z0, z1, bUse3d
    SetOrdinates(statement, sdo_ordinates, result.GetBounds());
    statement->Bind(&sdo_ordinates, connection->GetOrdinateType());
    
    try {
        statement->Execute();
    } catch (std::runtime_error const& e) {
        delete statement;
        ostringstream oss;
        oss << "Failed to insert block # into '" << tableName << "' table. Does the table exist? "  << std::endl << e.what() << std::endl;
        throw std::runtime_error(oss.str());
    }
    
    delete statement; statement = 0;
    oss.str("");
    

    OWStatement::Free(locator, 1);

    delete statement;
    if (p_pc_id != 0) free(p_pc_id);
    if (p_result_id != 0) free(p_result_id);
    if (p_num_points != 0) free(p_num_points);
    if (p_gtype != 0) free(p_gtype);
    if (p_srid != 0) free(p_srid);    

    connection->DestroyType(&sdo_elem_info);
    connection->DestroyType(&sdo_ordinates);
    
    return true;

}


bool InsertBlocks(
                OWConnection* con, 
                KDXIndexSummary* summary,
                liblas::Reader* reader2, 
                const std::string& table_name, 
                int srid, 
                long pc_id,
                bool bUseSolidGeometry,
                bool bUse3d
                )
{
    ResultsVector::iterator i;


    // blocks* b = CreateBlock(nCommitInterval);

    ostringstream oss;
    oss << "INSERT INTO "<< table_name << 
            "(OBJ_ID, BLK_ID, NUM_POINTS, POINTS,   "
            "PCBLK_MIN_RES, BLK_EXTENT, PCBLK_MAX_RES, NUM_UNSORTED_POINTS, PT_SORT_DIM) "
            "VALUES ( :1, :2, :3, :4, 1, mdsys.sdo_geometry(:5, :6, null,:7, :8)" 
            ", 1, 0, 1)";
          
    OWStatement* statement = 0;

    statement = con->CreateStatement(oss.str().c_str());
    long j = 0;
    bool inserted = false;
    ResultsVector& results = summary->GetResults();
        


    for (i=results.begin(); i!=results.end(); i++)
    {        
        inserted = InsertBlock(con, 
                                    *i,
                                    srid, 
                                    reader2, 
                                    table_name.c_str(), 
                                    pc_id, 
                                    bUseSolidGeometry, 
                                    bUse3d);
        j++;
    }
    return inserted;
}

bool CreateSDOEntry(    OWConnection* connection, 
                        const char* tableName, 
                        KDXIndexSummary* query, 
                        long srid, 
                        long precision,
                        bool bUse3d,
                        bool bSetExtents,
                        liblas::Bounds<double> const& bounds)
{
    ostringstream oss;
    OWStatement* statement = 0;

    ostringstream oss_geom;
    
    oss.setf(std::ios_base::fixed, std::ios_base::floatfield);
    oss.precision(precision);

    ostringstream s_srid;
    

    if (srid == 0) {
        s_srid << "NULL";
        // bUse3d = true;
    }
    else {
        s_srid << srid;
    }

    double tolerance = 0.05;
    liblas::Bounds<double> e = *query->bounds.get();

    if (IsGeographic(connection, srid)) {
        e.min(0,-180.0); e.max(0,180.0);
        e.min(1,-90.0); e.max(1,90.0);
        e.min(2,0.0); e.max(2,20000.0);

        tolerance = 0.000000005;
    }


    if (bSetExtents){
        e = bounds;
    }     

     
    oss <<  "INSERT INTO user_sdo_geom_metadata VALUES ('" << tableName <<
        "','blk_extent', MDSYS.SDO_DIM_ARRAY(";
    
    oss << "MDSYS.SDO_DIM_ELEMENT('X', " << e.min(0) << "," << e.max(0) <<"," << tolerance << "),"
           "MDSYS.SDO_DIM_ELEMENT('Y', " << e.min(1) << "," << e.max(1) <<"," << tolerance << ")";
           
    if (bUse3d) {
        oss << ",";
        oss <<"MDSYS.SDO_DIM_ELEMENT('Z', "<< e.min(2) << "," << e.max(2) << "," << tolerance << ")";
    }
    oss << ")," << s_srid.str() << ")";
    
    statement = RunSQL(connection, oss);
    if (statement != 0) delete statement; else return false;
    oss.str("");
    
    
    return true;
        
}





long CreatePCEntry( OWConnection* connection, 
                    KDXIndexSummary* query, 
                    std::string blkTableName, 
                    std::string pcTableName, 
                    std::string cloudColumnName,
                    std::string aux_columns,
                    std::string aux_values,
                    int nDimension, 
                    int srid,
                    int blk_capacity,
                    long precision,
                    bool bUseSolidGeometry,
                    bool bUse3d,
                    bool bInsertHeaderBlob,
                    std::string const& header_blob_column,
                    std::vector<boost::uint8_t> const& header_data)
{
    ostringstream oss;

    OWStatement* statement = 0;

    oss.setf(std::ios_base::fixed, std::ios_base::floatfield);
    oss.precision(precision);
    
    std::string blkTableName_u = to_upper(blkTableName);    
    std::string pcTableName_u = to_upper(pcTableName);
    std::string cloudColumnName_u = to_upper(cloudColumnName);
    std::string aux_columns_u = to_upper(aux_columns);
    std::string header_blob_column_u = to_upper(header_blob_column);

    ostringstream columns;
    ostringstream values;
    
    if (!aux_columns_u.empty() ) {
        columns << cloudColumnName_u << "," << aux_columns_u;
    
        values << "pc," << aux_values;
    } else {
        columns << cloudColumnName_u;
        values << "pc";
    }
    
    if (!header_blob_column_u.empty()){
        columns << "," << header_blob_column_u;
        values <<", :2";
    }


    ostringstream s_srid;
    ostringstream s_gtype;
    ostringstream s_eleminfo;
    ostringstream s_geom;


    IsGeographic(connection, srid);

    if (srid == 0)
    {
        s_srid << "NULL";
    }
    else
    {
        s_srid << srid;
    }

    long gtype = GetGType(bUse3d, bUseSolidGeometry);
    s_gtype << gtype;
    if (bUse3d) {
        if (bUseSolidGeometry == true) {
            // s_gtype << "3008";
            s_eleminfo << "(1,1007,3)";

        } else {
            // s_gtype << "3003";
            s_eleminfo  << "(1,1003,3)";

        }
    } else {
        if (bUseSolidGeometry == true) {
            // s_gtype << "2008";
            s_eleminfo << "(1,1007,3)";

        } else {
            // s_gtype << "2003";
            s_eleminfo  << "(1,1003,3)";

        }
    }
    

    // extent* e = GetExtent(  *(query->bounds.get()), bUse3d );
    liblas::Bounds<double> e =  *(query->bounds.get());

    s_geom << "           mdsys.sdo_geometry("<<s_gtype.str() <<", "<<s_srid.str()<<", null,\n"
"              mdsys.sdo_elem_info_array"<< s_eleminfo.str() <<",\n"
"              mdsys.sdo_ordinate_array(\n";

    s_geom << e.min(0) << "," << e.min(1) << ",";

    if (bUse3d) {
        s_geom << e.min(2) << ",";
    }
    
    s_geom << e.max(0) << "," << e.max(1);

    if (bUse3d) {
        s_geom << "," << e.max(2);
    }

    s_geom << "))";

    
    
oss << "declare\n"
"  pc_id NUMBER := :1;\n"
"  pc sdo_pc;\n"

"begin\n"
"  -- Initialize the Point Cloud object.\n"
"  pc := sdo_pc_pkg.init( \n"
"          '"<< pcTableName_u<<"', -- Table that has the SDO_POINT_CLOUD column defined\n"
"          '"<< cloudColumnName_u<<"',   -- Column name of the SDO_POINT_CLOUD object\n"
"          '"<<blkTableName_u<<"', -- Table to store blocks of the point cloud\n"
"           'blk_capacity="<<blk_capacity<<"', -- max # of points per block\n"
<< s_geom.str() <<
",  -- Extent\n"
"     0.5, -- Tolerance for point cloud\n"
"           "<<nDimension<<", -- Total number of dimensions\n"
"           null);\n"
"  :1 := pc.pc_id;\n"

"  -- Insert the Point Cloud object  into the \"base\" table.\n"
"  insert into " << pcTableName_u << " ( ID, "<< columns.str() <<
        ") values ( pc.pc_id, " << values.str() << ");\n"

"  "
"end;\n";


    int pc_id = 0;
    long output = 0;
    statement = connection->CreateStatement(oss.str().c_str());
    statement->Bind(&pc_id);
    if (bInsertHeaderBlob) {
        OCILobLocator** locator =(OCILobLocator**) VSIMalloc( sizeof(OCILobLocator*) * 1 );
        statement->Define( locator, 1 ); 

        statement->Bind((char*)&(header_data[0]),(long)header_data.size());
    
    }
    try {
        statement->Execute();
    } catch (std::runtime_error const& e) {
        ostringstream oss;
        oss << "Failed at creating Point Cloud entry into " << pcTableName << " table. Does the table exist? "  << e.what();
        throw std::runtime_error(oss.str());
    }
    output = pc_id;

    return output;
}

std::string GetInvocationHeader()
{
    ostringstream oss;
    oss << "--------------------------------------------------------------------\n";
    oss << "    las2oci (" << GetFullVersion() << ")\n";
    oss << "--------------------------------------------------------------------\n";
    return oss.str();    
}

void usage() {
    fprintf(stderr,"----------------------------------------------------------\n");
    fprintf(stderr,"    las2oci (version ) usage:\n");
    fprintf(stderr,"----------------------------------------------------------\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"--block-table-name: (-bk)\n");
    fprintf(stderr,"--base-table-name: (-bn)\n");
    fprintf(stderr,"--cloud-table-name: (-cn) \n");
    fprintf(stderr,"--overwrite: (-d) \n");
    fprintf(stderr,"--srid: (-s) \n");
    fprintf(stderr,"--pre-sql: (-prs) \"CREATE TABLE BASE (id number, cloud mdsys.sdo_pc)\" \n");    
    fprintf(stderr,"--post-sql: (-pos) \"UPDATE TABLE BASE SET ID=1\" \n");    
    fprintf(stderr,"--pre-block-sql: (-pbs) \"UPDATE TABLE PC a set a.geometry = (SELECT b.geometry from boundaries b where a.filename = b.filename))\"");
    fprintf(stderr,"--aux-columns: \"id,description\" \n");    
    fprintf(stderr,"--aux-values: \"0,'A description'\" \n");    
    fprintf(stderr,"--precision: 8\n");    
        
    fprintf(stderr,"las2oci -i output.las lidar/lidar@oraclemachine/instance \n"
                   "--block-table-name  hobu_blocks --base-table-name hobu_base\n"
                   "--cloud-column-name PC --srid 4326 -d\n");
    
    

    fprintf(stderr, "\nFor more information, see the full documentation for las2las at:\n"
                    " http://liblas.org/browser/trunk/doc/las2las.txt\n");
    fprintf(stderr,"----------------------------------------------------------\n");    
}

std::vector<boost::uint8_t> GetHeaderData(std::string const& filename, boost::uint32_t offset)
{
    std::istream* in = OpenInput(filename, false);
    std::vector<boost::uint8_t> data(offset);
    liblas::detail::read_n(data.front(), *in, offset);
    delete in;
    return data;
}

po::options_description GetFileOptions()
{
po::options_description file_options("las2oci options");



file_options.add_options()
    ("help,h", "produce help message")
    ("input,i", po::value< string >(), "input LAS file")
    ("connection,c", po::value< string >(), "OCI connection string")
    ("verbose,v", po::value<bool>()->zero_tokens(), "Verbose message output")
    ("debug", po::value<bool>()->zero_tokens(), "Enable debug messages (SQL calls)")
    ("base-table-name", po::value< string >()->default_value("HOBU"), "The table name in which to put the point cloud object.  This table must have a column of type SDO_PC, with the name to be specified with --cloud-column-name")
    ("block-table-name", po::value< string >(), "The table name in which to put the block data.  This table must be of type SDO_PC.BLK_TABLE.  This table will be created using the filename of the input LAS file if not specified.  Use -d to delete the table if it already exists.")
    ("cloud-column-name", po::value< string >()->default_value("CLOUD"), "The column name that contains the point cloud object in the base table")
    ("header-blob-column", po::value< string >(), "Blob column name in the base table in which to optionally insert the contents of the input file's header.")
    ("overwrite,d", po::value<bool>()->zero_tokens(), "Drop block table before inserting data.")
    ("block-capacity", po::value<boost::uint32_t>()->default_value(3000), "Maximum number of points to be inserted into each block")
    ("precision,p", po::value<boost::uint32_t>()->default_value(8), "Number of decimal points to write into SQL for point coordinate data.  Used in user_sdo_geom_metadata entry and defining the PC_EXTENT for the point cloud object.")
    ("srid,s", po::value<boost::uint32_t>(), "Oracle numerical SRID value to use to define point cloud.")
    ("pre-sql", po::value< string >(), "Quoted SQL or filename location of PL/SQL to run before executing the point cloud creation process.")
    ("pre-block-sql", po::value< string >(), "Quoted SQL or filename location of PL/SQL to run before executing the insertion of block data.")
    ("post-sql", po::value< string >(), "Quoted SQL or filename location of PL/SQL to run after inserting block data.")
    ("base-table-aux-columns", po::value< string >(), "Quoted, comma-separated list of columns to add to the SQL that gets executed as part of the point cloud insertion into the base-table-name")
    ("base-table-aux-values", po::value< string >(), "Quoted, comma-separated list of values to add to the SQL that gets executed as part of the point cloud insertion into the base-table-name")
    ("solid", po::value<bool>()->zero_tokens(), "Define the point cloud's PC_EXTENT geometry gtype as (1,1007,3) instead of the normal (1,1003,3), and use gtype 3008/2008 vs 3003/2003 for BLK_EXTENT geometry values.")
    ("3d", po::value<bool>()->zero_tokens(), "Use Z values for insertion of all extent (PC_EXTENT, BLK_EXTENT, USER_SDO_GEOM_METADATA) entries")
    ("global-extent", po::value< std::vector<double> >(), "Extent window to define for the PC_EXTENT.\nUse a comma-separated or quoted, space-separated list, for example, \n -e minx, miny, maxx, maxy\n or \n -e minx, miny, minz, maxx, maxy, maxz\n -e \"minx miny minz maxx maxy maxz\"")
    ("cached", po::value<bool>()->zero_tokens(), "Cache the entire file on the first read")


;

return file_options;
}

po::options_description GetHiddenOptions()
{
    po::options_description hidden_options("hidden options");
hidden_options.add_options()
    ("xmin", po::value< double >(), "global-extent minx value")
    ("ymin", po::value< double >(), "global-extent miny value")
    ("zmin", po::value< double >(), "global-extent minz value")
    ("xmax", po::value< double >(), "global-extent maxx value")
    ("ymax", po::value< double >(), "global-extent maxy value")
    ("zmax", po::value< double >(), "global-extent maxz value")
    ("aux-columns", po::value< string >(), "Quoted, comma-separated list of columns to add to the SQL that gets executed as part of the point cloud insertion into the base-table-name")
    ("aux-values", po::value< string >(), "Quoted, comma-separated list of values to add to the SQL that gets executed as part of the point cloud insertion into the base-table-name")
;


return hidden_options;
}
// select sdo_pc_pkg.to_geometry(a.points, a.num_points, 3, 4326) from NACHES_BAREEARTH_BLOCK1 a where a.obj_id= 8907
int main(int argc, char* argv[])
{

    std::string input ("");
    std::string connection("");
    std::string username;
    std::string password;
    std::string instance;
    std::string point_cloud_name("CLOUD");
    std::string base_table_name("HOBU");
    std::string block_table_name("");
    std::string header_blob_column("");
    
    std::string pre_sql("");
    std::string post_sql("");
    std::string pre_block_sql("");
    
    std::string aux_columns("");
    std::string aux_values("");
    
    bool bUseExistingBlockTable = false;
    bool bDropTable = false;
    bool bUseSolidGeometry = false;
    bool bUse3d = false;
    bool bInsertHeaderBlob = false;
    bool bCachedReader = false;
    
    boost::uint32_t nCapacity = 10000;

    int srid = 0;
    long precision = 8;
    
    liblas::Bounds<double> global_extent;
    
    bool bSetExtents = false;    
    
    bool verbose = false;
    bool debug = false;
    std::vector<liblas::FilterPtr> filters;
    std::vector<liblas::TransformPtr> transforms;
    
    liblas::Header header;
    

    try {

        po::options_description file_options = GetFileOptions();
        po::options_description header_options = GetHeaderOptions();        
        po::options_description filtering_options = GetFilteringOptions();
        po::options_description transform_options = GetTransformationOptions() ;
        po::options_description hidden_options = GetHiddenOptions();
        po::positional_options_description p;
        p.add("input", 1);
        p.add("connection", 1);
        

        

        po::variables_map vm;
        po::options_description options;
        options.add(file_options).add(header_options).add(transform_options).add(filtering_options).add(hidden_options);
        po::store(po::command_line_parser(argc, argv).
        options(options).positional(p).run(), vm);

        po::notify(vm);

        if (vm.count("help")) 
        {
            std::cout << GetInvocationHeader()<<file_options<<"\n"<<transform_options<<"\n"<<filtering_options<<"\n";
            std::cout <<"\nFor more information, see the full documentation for las2oci at:\n";
            
            std::cout << " http://liblas.org/utilities/las2oci.html\n";
            std::cout << "----------------------------------------------------------\n";
            return 0;
        }

        if (vm.count("verbose")) 
        {
            verbose = vm["verbose"].as< bool >();
        }
        if (vm.count("debug")) 
        {
            debug = vm["debug"].as< bool >();
            verbose = true; // implies verbose too
            const char* gdal_debug = getenv("CPL_DEBUG");
            if (gdal_debug == 0)
            {
                char d[20] = "CPL_DEBUG=ON";
                putenv(d);
            }
            
        }
        if (vm.count("input")) 
        {
            input = vm["input"].as< string >();
            std::ifstream ifs;
            if (verbose)
                std::cout << "Opening " << input << " to fetch header" << std::endl;
            if (!liblas::Open(ifs, input.c_str()))
            {
                std::cerr << "Cannot open " << input << "for read.  Exiting...";
                return 1;
            }
            liblas::Reader reader(ifs);
            header = reader.GetHeader();
        } else {
            std::cerr << "Input LAS file not specified!\n";
            std::cout << GetInvocationHeader()<<file_options<<transform_options<<filtering_options<<"\n";
            return 1;
        }

        if (vm.count("connection")) 
        {
            connection = vm["connection"].as< string >();
            string::size_type slash_pos = connection.find("/",0);
            username = connection.substr(0,slash_pos);
            string::size_type at_pos = connection.find("@",slash_pos);

            password = connection.substr(slash_pos+1, at_pos-slash_pos-1);
            instance = connection.substr(at_pos+1);
        } else {
            std::cerr << "Connection string not specified!\n";
            return 1;
        }
        


        if (vm.count("overwrite")) 
        {
            bDropTable = vm["overwrite"].as< bool >();
        }

        if (vm.count("base-table-name")) 
        {
            base_table_name = vm["base-table-name"].as< string >();
            if (verbose)
                std::cout << "Setting output base table to: " << base_table_name << std::endl;
        }
        if (vm.count("block-table-name")) 
        {
            block_table_name = vm["block-table-name"].as< string >();
            if (verbose)
                std::cout << "Setting output block table to: " << block_table_name << std::endl;
        }
        if (vm.count("cloud-column-name")) 
        {
            point_cloud_name = vm["cloud-column-name"].as< string >();
            if (verbose)
                std::cout << "Setting output point cloud column to: " << point_cloud_name << std::endl;
        }
        if (vm.count("header-blob-column")) 
        {
            header_blob_column = vm["header-blob-column"].as< string >();
            bInsertHeaderBlob=true;
            if (verbose)
                std::cout << "Setting header blob column to: " << header_blob_column << std::endl;
        }    

        if (vm.count("base-table-aux-columns")) 
        {
            aux_columns = vm["base-table-aux-columns"].as< string >();
            if (verbose)
                std::cout << "Setting base-table-aux-columns to: " << aux_columns << std::endl;

        }
        if (vm.count("base-table-aux-values")) 
        {
            aux_values = vm["base-table-aux-values"].as< string >();
            if (verbose)
                std::cout << "Setting base-table-aux-values to: " << aux_values << std::endl;

        }
            
        if (vm.count("aux-columns")) 
        {
            aux_columns = vm["aux-columns"].as< string >();
            if (verbose)
                std::cout << "Setting base-table-aux-columns to: " << aux_columns << std::endl;

        }
        if (vm.count("aux-values")) 
        {
            aux_values = vm["aux-values"].as< string >();
            if (verbose)
                std::cout << "Setting base-table-aux-values to: " << aux_values << std::endl;

        }

        if (vm.count("block-capacity")) 
        {
            nCapacity = vm["block-capacity"].as< boost::uint32_t >();
            if (verbose)
                std::cout << "Setting block capacity to: " << nCapacity << std::endl;
        }
        
        if (vm.count("precision")) 
        {
            precision = vm["precision"].as< boost::uint32_t >();
            if (verbose)
                std::cout << "Setting precision to: " << precision << std::endl;

        }
        
        if (vm.count("srid")) 
        {
            srid = vm["srid"].as< boost::uint32_t >();
            if (verbose)
                std::cout << "Setting output Oracle SRID to: " << srid << std::endl;
        }
        
        if (vm.count("pre-sql")) 
        {
            std::string sql = vm["pre-sql"].as< string >();
            bool used_file = false;
            try {
                pre_sql = ReadSQLData(sql);
                used_file = true;
            } catch (std::runtime_error const& e) {
                boost::ignore_unused_variable_warning(e);
                pre_sql = std::string(sql);
                used_file = false;
            }
            if (verbose)
                if (!used_file)
                    std::cout << "Setting output pre-sql to: " << pre_sql << std::endl;
                else
                    std::cout << "Setting output pre-sql to: " << sql << std::endl; // Tell filename instead
        }
        if (vm.count("post-sql")) 
        {
            std::string sql = vm["post-sql"].as< string >();
            bool used_file = false;
            try {
                post_sql = ReadSQLData(sql);
                used_file = true;
            } catch (std::runtime_error const& e) {
                boost::ignore_unused_variable_warning(e);
                post_sql = std::string(sql);
                used_file = false;
            }
            if (verbose)
                if (!used_file)
                    std::cout << "Setting output post-sql to: " << post_sql << std::endl;
                else
                    std::cout << "Setting output post-sql to: " << sql << std::endl; // Tell filename instead
        }
        if (vm.count("pre-block-sql")) 
        {
            std::string sql = vm["pre-block-sql"].as< string >();
            bool used_file = false;
            try {
                pre_block_sql = ReadSQLData(sql);
                used_file = true;
            } catch (std::runtime_error const& e) {
                boost::ignore_unused_variable_warning(e);
                pre_block_sql = std::string(sql);
                used_file = false;
            }
            if (verbose)
                if (!used_file)
                    std::cout << "Setting output pre-block-sql to: " << pre_sql << std::endl;
                else
                    std::cout << "Setting output pre-block-sql to: " << sql << std::endl; // Tell filename instead
        }

        if (vm.count("solid")) 
        {
            bUseSolidGeometry = vm["solid"].as< bool >();
            if (verbose)
                std::cout << "Storing solid geometries... " << std::endl;
        }
        if (vm.count("3d")) 
        {
            bUse3d = vm["3d"].as< bool >();
            if (verbose)
                std::cout << "Storing 3D geometries... " << std::endl;
        }
        if (vm.count("global-extent")) 
        {
            std::string bounds_string = vm["global-extent"].as< string >();

            boost::char_separator<char> sep(SEPARATORS);

            std::vector<double> vbounds;
            tokenizer tokens(bounds_string, sep);
            for (tokenizer::iterator t = tokens.begin(); t != tokens.end(); ++t) {
                vbounds.push_back(atof((*t).c_str()));
            }
            if (vbounds.size() == 4) 
            {
                global_extent = liblas::Bounds<double>(vbounds[0], 
                                        vbounds[1], 
                                        vbounds[2], 
                                        vbounds[3]);
            } else if (vbounds.size() == 6)
            {
                global_extent = liblas::Bounds<double>(vbounds[0], 
                                        vbounds[1], 
                                        vbounds[2], 
                                        vbounds[3], 
                                        vbounds[4], 
                                        vbounds[5]);
            } else {
                ostringstream oss;
                oss << "Bounds must be specified as a 4-tuple or "
                       "6-tuple, not a "<< vbounds.size()<<"-tuple" << "\n";
                throw std::runtime_error(oss.str());
            }
        }

        if (vm.count("xmin")) 
        {
            double xmin = vm["xmin"].as< double >();
            global_extent.min(0, xmin);
            if (verbose)
                std::cout << "Setting xmin to: " << xmin << std::endl;
        }
        if (vm.count("ymin")) 
        {
            double ymin = vm["ymin"].as< double >();
            global_extent.min(1, ymin);
            if (verbose)
                std::cout << "Setting ymin to: " << ymin << std::endl;
        }
        if (vm.count("zmin")) 
        {
            double zmin = vm["zmin"].as< double >();
            global_extent.min(2, zmin);
            if (verbose)
                std::cout << "Setting zmin to: " << zmin << std::endl;
        }
        if (vm.count("xmax")) 
        {
            double xmax = vm["xmax"].as< double >();
            global_extent.max(0, xmax);
            if (verbose)
                std::cout << "Setting xmax to: " << xmax << std::endl;
        }
        if (vm.count("ymax")) 
        {
            double ymax = vm["ymax"].as< double >();
            global_extent.max(1, ymax);
            if (verbose)
                std::cout << "Setting ymax to: " << ymax << std::endl;
        }
        if (vm.count("zmax")) 
        {
            double zmax = vm["zmax"].as< double >();
            global_extent.max(2, zmax);
            if (verbose)
                std::cout << "Setting zmax to: " << zmax << std::endl;
        }
        if (vm.count("cached")) 
        {
            bCachedReader = vm["cached"].as< bool >();
            if (verbose)
                std::cout << "Caching entire file... " << std::endl;
        }

        SetGDALErrorHandler(debug);

        filters = GetFilters(vm, verbose);
        
        // Transforms alter our header as well.  Setting scales, offsets, etc.
        transforms = GetTransforms(vm, verbose, header);
        
        if (connection.empty() || input.empty()) {
            if (input.empty())
                std::cerr << "Input .las file not specified!" << std::endl;
            if (connection.empty())
                std::cerr << "Oracle connection string empty!" << std::endl;

            std::cout << GetInvocationHeader()<<file_options<<transform_options<<filtering_options<<"\n";
            return 1;

        }

        if (block_table_name.size() == 0) {
            // change filename foo.las -> foo for an appropriate
            // block tablename for oracle... must be less than 30 characters
            // and no extraneous characters.
            string::size_type dot_pos = input.find_first_of(".");
            block_table_name = input.substr(0,dot_pos);
        } else {
            block_table_name = block_table_name;
        }
        string::size_type slash_pos = connection.find("/",0);
        username = connection.substr(0,slash_pos);
        string::size_type at_pos = connection.find("@",slash_pos);
    //    std::cout <<"slash_pos: " << slash_pos << " at_pos: " << at_pos<<std::endl;
        password = connection.substr(slash_pos+1, at_pos-slash_pos-1);
        instance = connection.substr(at_pos+1);
        if (verbose)
            std::cout << "Connecting with username: " << username << " password: "<< password<< " instance: " << instance << std::endl;    
        if (verbose)
        std::cout << "Base table name " << base_table_name << " cloud column: " << point_cloud_name <<" block table: " << block_table_name << std::endl;
        // OCI_SUCCESS_WITH_INFO error, which according to google relates to 
        // a warning related to expired or expiring passwords needs to be 
        // handled in the oracle wrapper.

        // Create the index before connecting to Oracle.  That way we don't heartbeat
        // the server while we're cruising through the file(s).

        // OS X RAMDISK configuration
        // http://www.macosxhints.com/article.php?story=20090222092827145

        // Obj_id is serial for each row in the block table
        // blk_id is the index leaf node id (this is currently being written incorrectly)
        OWConnection* con = new OWConnection(username.c_str(),password.c_str(),instance.c_str());
        if (con->Succeeded()) {
            if (verbose)
                std::cout <<"Oracle connection succeded" << std::endl;
        } else {
            std::cerr <<"Oracle connection failed" << std::endl; return(1);
        }



        // con->StartTransaction();

        if (bDropTable) {
            if (verbose)
                std::cout << "dropping existing tables..." << std::endl;

            WipeBlockTable(con, block_table_name, base_table_name, point_cloud_name);
        }

        if (!pre_sql.empty()) {
            std::cout << "running pre-sql ..." << std::endl;
            ostringstream oss;
            oss << pre_sql;
            OWStatement* statement = 0;
            statement = RunSQL(con, oss);
            if (statement != 0) {
                delete statement; 
            }
            else {
                std::cerr << "pre-sql execution failed.." << std::endl;
                return 1;
            }
            oss.str("");        
        }
        if (!BlockTableExists(con, block_table_name.c_str()))
            CreateBlockTable(con, block_table_name);
        else {
            bUseExistingBlockTable = true;
            if (verbose)
                std::cout << "Using existing block table ... " << std::endl;

        }



        std::istream* istrm2;
        istrm2 = OpenInput(input, false);
        
        liblas::Reader* reader2 = 0;
        if (bCachedReader)
            reader2 = new liblas::Reader(*istrm2,0, header);
        else
            reader2 = new liblas::Reader(*istrm2, header);

        reader2->SetFilters(filters);
        reader2->SetTransforms(transforms);

        std::vector<boost::uint8_t> header_data = GetHeaderData(input, reader2->GetHeader().GetDataOffset());

        KDXIndexSummary* query = 0;
        if (!KDTreeIndexExists(input)) {
            if (verbose)
                std::cout << "Chipping data for loading into Oracle with " << nCapacity<< " block capacity" << std::endl;
            query = new KDXIndexSummary(*reader2, nCapacity, verbose);
        } else {
            std::ostringstream os;
            os << input << ".kdx" ;
            if (verbose)
                std::cout << "Using existing "<<os.str()<<" chip file ... " << std::endl;


            std::istream* kdx = OpenInput(os.str(), false);
            query = new KDXIndexSummary(*kdx);
        }

        ResultsVector& results = query->GetResults();

        ResultsVector::iterator i;

        long pc_id = CreatePCEntry(  con, 
                        query, 
                        block_table_name,
                        base_table_name,
                        point_cloud_name,
                        aux_columns,
                        aux_values,
                        3, // we're assuming 3d for now
                        srid,
                        nCapacity,
                        precision,
                        bUseSolidGeometry,
                        bUse3d,
                        bInsertHeaderBlob,
                        header_blob_column,
                        header_data);


        if (!pre_block_sql.empty()) {
            if (verbose)
                std::cout << "running pre-block-sql ..." << std::endl;

            ostringstream oss;
            oss << pre_block_sql;
            OWStatement* statement = 0;
            statement = RunSQL(con, oss);
            if (statement != 0) {
                delete statement; 
            }
            else {
                std::cerr << " pre-block-sql execution failed.." << std::endl;
                return 1;
            }
            oss.str("");
            con->Commit();     
        }

        if (verbose)
            std::cout << "Inserting " << results.size() << " blocks ..." << std::endl;

        InsertBlocks(con,
                     query,
                     reader2,
                     block_table_name,
                     srid,
                     pc_id,
                     bUseSolidGeometry,
                     bUse3d);


        if (!bUseExistingBlockTable) {
            if (verbose)
                std::cout << "Creating new block table user_sdo_geom_metadata entries and index ..." << std::endl;
            CreateSDOEntry( con, 
                            block_table_name.c_str(), 
                            query, 
                            srid , 
                            precision, 
                            bUse3d,
                            bSetExtents,
                            global_extent);

            CreateBlockIndex(   con, 
                                block_table_name.c_str(), 
                                srid,  
                                bUse3d);
        }




        if (!post_sql.empty()) {
            if (verbose)
                std::cout << "running post-sql ..." << std::endl;

            ostringstream oss;
            oss << post_sql;
            OWStatement* statement = 0;
            statement = RunSQL(con, oss);
            if (statement != 0) {
                delete statement; 
            }
            else {
                std::cerr << "post-sql execution failed.." << std::endl;
                return 1;
            }
            oss.str("");        
        }

        con->Commit();
        
        delete reader2;
        delete istrm2;
        delete con;
    }
    catch(std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...) {
        std::cerr << "Exception of unknown type!\n";
    }
    
    return 0;

}
// 
// select t.x, t.y, t.id from (
// select 
// 
//  sdo_pc_pkg.to_geometry(
//                     a.points,   -- point LOB
//                     a.num_points, -- # of points in the LOB
//                     2,  -- total dimensionality
//                     8265 -- SRID
//                     ) shape from line_27006_reproj a
//                     --)
//      )c ,  TABLE(SDO_UTIL.GETVERTICES(c.shape)) t
// select sdo_pc_pkg.to_geometry(
//                     a.points,   -- point LOB
//                     a.num_points, -- # of points in the LOB
//                     2,  -- total dimensionality
//                     8265 -- SRID
//                     ) shape from line_27006_reproj a
//                     where rownum < 10
                    
