#!/usr/bin/php 
<?php

/*
   Copyright (C) 2016 Alois Schloegl <alois.schloegl@ist.ac.at>
   This file is part of the BioSig repository at http://biosig.sf.net/

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 3
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */


/*
	biosig_fhir: 
		makes a fhir_json_binary template
*/
function biosig_fhir($filename) {
	$h = popen('/home/as/src/biosig-code/biosig4c++/biosig_fhir "' . $filename.'"', 'rb');
	$jsonhdr = stream_get_contents($h);
	fclose($h);
	return $jsonhdr;
}

/*
        get header (meta) information
 */
function biosig_json_header($filename) {
	$h = popen('save2gdf -JSON "' . $filename.'"', 'rb');
	$jsonhdr = stream_get_contents($h);
	fclose($h);
	return $jsonhdr;
}

/*
        get header (meta) information in php array
*/
function biosig_header($filename) {
	return json_decode( biosig_json_header($filename) );
}


$filename="data/Newtest17-256.bdf";
$BIN = biosig_fhir($filename);
echo $BIN;
print "\n\n";
$HDR = biosig_header($filename);

var_dump($HDR);

?>

