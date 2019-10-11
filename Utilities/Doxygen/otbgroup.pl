#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# if regular doxycomment add a @{
# at next empty line add a //@}

$ingroup = 0;
$semicount =0;
$endbracecount = 0;
$endparencount = 0;
while(<>)
{
    chomp;
    $line = $_;
    # if the line is not an empty line
    if( $line =~ /\S+/ )
    {
        if ( /\/\*\*(.*)/ )
        {
            # I guess it was not a group, dump savebuffer
            if($ingroup)
            {
                print "/**" . $savebuffer . "\n";
            }
            # if it is a class or brief then output the line but
            # do not start a group
            if ( /(\\class|\\brief)/ )
            {
                print $line . "\n";
            }
            # must be a group so start saving
            else
            {
                $savebuffer = "$1" . "\n";
                $ingroup = 1;
                $semicount = 0;
                $endbracecount = 0;
                $endparencount = 0;
            }
        }
        else
        {
            # add to save buffer if in group
            if($ingroup)
            {
                $savebuffer = $savebuffer . $_ . "\n";
            }
            else
            {
                # non empty line that is not the start of a doxy comment
                print $_ . "\n";
            }
        }
        if($line =~ /;/ )
        {
            $semicount = $semicount + 1;
        }
        if($line =~ /\}/ )
        {
            $endbracecount = $endbracecount + 1;
        }
        if($line =~ /\)/ )
        {
            $endparencount = $endparencount + 1;
        }
    }
    else
    {
        if($ingroup)
        {
            if($endparencount > 1 && ($semicount > 1 || $endbracecount > 1))
            {
                print "/**@\{" . $savebuffer . "//@}\n\n";
            }
            else
            {
                print "/**" . $savebuffer . "\n";
            }
            $savebuffer = "";
            $ingroup = 0;
        }
        else
        {
            print $line . "\n";
        }
    }
}
