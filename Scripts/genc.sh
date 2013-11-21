#!/bin/sh

####
## Author Stéphane ALBERT (stephane.albert@c-s.fr)
## Version 0.1 (November 14th, 2013)
########

####
## Global variables
##############################################################################
DIALECT="C++"
SUFFIX=""

####
## Display usage.
##############################################################################
usage() {
    echo "Usage: `basename $0` [-h] [-s EXT] FILES..."
    echo "  -C      Restrict for C dialect instead of C++."
    echo "  -h      Display this help message."
    echo "  -s EXT  Remove suffix EXT from file basename."
}

####
## Check command-line arguments number is 1 or more.
##############################################################################
if [ $# -lt 1 ]
then
    usage >& 2
    exit 1
fi

####
## Parse command-line arguments.
##############################################################################
# parse_args() {
#     while getopts 'Chs:' opt
#     do
#         case $opt in
#             C)
#                 DIALECT="C"
#                 ;;

#             h)
#                 echo "Help"
#                 usage
#                 exit 0
#                 ;;

#             s)
#                 SUFFIX="$OPTARG"
#                 ;;

#             *) ;;
#         esac  
#     done

#     shift `echo $OPTIND-1 | bc`

#     return $*
# }

####
## Translate $1 input file to C++ source code.
##############################################################################
translate_file() {
    local NAME="`basename $1 $SUFFIX | tr -d '[:blank:]' | tr '[:punct:]' '_' | tr '[:lower:]' '[:upper:]'`"

    echo ""
    echo "/****************************************************************************/"
    if [ $DIALECT="C++" ]
    then
        echo "//"
        echo "// '$NAME' Generated from file '`basename $1`'"
    else
        echo "/* '$NAME' Generated from file '`basename $1`'."
    fi
    #echo "const char* $NAME ="
    #echo "`sed 's/^.*$/\"&\"/g' $1`;"
    awk  -v str_name=$NAME -v int_name=${NAME}_COUNT 'BEGIN {
    RS=";"
    FS="\n"

    print "const char* " str_name "[] = {"

    count = 0
}
{
    line = ""

    for( i=1; i<NF; ++i ) {
        if( $i!="" ) {
            line = line "\"" $i "\\n\"\n"
        }
    }

    if( i>2 ) {
        line = line "\"" $i ";\","

        print line

        count ++
    }
}
END {
    print "};\n"
    print "const int " int_name " = " count ";"
}' $1
}

####
## Translate all input files into
##############################################################################
translate() {
    if [ $DIALECT="C++" ]
    then
        echo "namespace"
        echo "{"
    fi

    local result=0

    for f in $*
    do
        if [ -f $f ]
        then
            translate_file $f
        else
            echo "Failed to translate '$f'." >& 2
            result=`echo $result+1 | bc`
        fi
    done

    if [ $DIALECT="C++" ]
    then
        echo "} // End of anonymous namespace."
    fi

    return $result
}

##
## MAIN
##############################################################################

#
# Parse command-line arguments.
while getopts 'hs:' opt
do
    case $opt in
        C)
            DIALECT="C"
            ;;

        h)
            usage
            exit 0
            ;;

        s)
            SUFFIX="$OPTARG"
            ;;

        *) ;;
    esac  
done

#
# Remove options.
shift `echo $OPTIND-1 | bc`

#
# Translate input files into C/C++
translate $*

# if [ $? -gt 0 ]
# then
#     echo "$? error(s) occured." >& 2
# fi

#
# Exit with result status.
exit 0
