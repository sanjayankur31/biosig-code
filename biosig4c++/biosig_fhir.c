/*

% Copyright (C) 2016 Alois Schloegl <alois.schloegl@ist.ac.at>
% This file is part of the "BioSig for C/C++" repository
% (biosig4c++) at http://biosig.sf.net/


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

#include <stdlib.h>
#include <biosig2.h>
#include <biosig-dev.h>
#include <b64/cencode.h>


/*
  https://www.hl7.org/fhir/binary.html
  https://catalyze.io/learn/the-fhir-resource-object-the-core-building-block
*/

/*
   biosig_hdr2gdf_base64(..)
     converts biosig data into GDF-stream encoded in base64

   hdr: biosig header structure, 
        file must be open in order to read data section
   fid: stream for data output
   
*/
int biosig_hdr2gdf_base64(HDRTYPE *hdr, FILE *fid) {

	hdr->TYPE=GDF;
	hdr->VERSION=3.0;

	struct2gdfbin(hdr);
	size_t len3 = hdrEVT2rawEVT(hdr);
	size_t len0 = hdr->HeadLen + hdr->NRec*hdr->AS.bpb + len3;
	
	size_t buflen = max(max(len3,hdr->HeadLen),hdr->AS.bpb);
	char *buf     = malloc(buflen*2);
	char *mem     = malloc(buflen);

	base64_encodestate B64STATE;
	base64_init_encodestate(&B64STATE);
	// write header	
	int c = base64_encode_block(hdr->AS.Header, (int)hdr->HeadLen, buf, &B64STATE);
	fwrite(buf,1,c,fid);

	// write data block	
	size_t count = 0;
	size_t bpb = bpb8_collapsed_rawdata(hdr);
	size_t blks= buflen/bpb;
	for (size_t nrec=0; nrec < hdr->NRec; nrec+=blks) {
		count = sread_raw(nrec, min(blks, hdr->NRec-nrec), hdr, 1, mem, blks*hdr->AS.bpb);
		c = base64_encode_block(mem, bpb, buf, &B64STATE);
		fwrite(buf,1,c,fid);
	}

	// write event table
	c = base64_encode_block(hdr->AS.rawEventData, (int)len3, buf, &B64STATE);
	fwrite(buf,1,c,fid);

	c = base64_encode_blockend(buf, &B64STATE);
	fwrite(buf,1,c,fid);

	free(buf);
	free(mem);
	
	return(0);
}

/*
   biosig_fhir_binary_json_template:
      opens a biosig file, and generates binary json template according to fhir
*/
char* biosig_fhir_binary_json_template(const char *filename, FILE *fid) {

	/* generate json template
	{
	  "resourceType" : "Binary",
	  "id" : filename,	
	  "meta" : { asprintf_hdr2json(&str, filename),
	   // from Resource: id, meta, implicitRules, and language
	  "contentType" : "<code>", // R!  MimeType of the binary content 
	  "content" : "<base64Binary>" // R!  The actual content
	}
	*/

	HDRTYPE *hdr = NULL;
	hdr = sopen(filename, "r", hdr);

	fprintf(fid,"{\n"	
		  "    \"resourceType\" : \"Binary\",\n"
		  "    \"id\" : \"%s\",\n"
		  "    \"meta\" : ", hdr->FileName);

	fprintf_hdr2json(fid, hdr);
		  
	fprintf(fid, "    \"contentType\" : \"biosig/gdf\",\n"
		     "    \"content\" : \"");

	biosig_hdr2gdf_base64(hdr, fid);

	fprintf(fid, "\"\n}");

	destructHDR(hdr);
	return(0);
}



extern int VERBOSE_LEVEL; 

int main(int argc, char **argv) {
    
    	VERBOSE_LEVEL = 0;
	const char *filename = argv[1]; 

	biosig_fhir_binary_json_template(filename, stdout);
}

