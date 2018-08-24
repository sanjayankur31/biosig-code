/*
    sandbox is used for development and under constraction work
    The functions here are either under construction or experimental.
    The functions will be either fixed, then they are moved to another place;
    or the functions are discarded. Do not rely on the interface in this function

    Copyright (C) 2018 Alois Schloegl <alois.schloegl@gmail.com>
    This file is part of the "BioSig for C/C++" repository
    (biosig4c++) at http://biosig.sf.net/

    BioSig is free software; you can redistribute it and/or
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

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../biosig.h"

// these functions are stubs

#ifdef WITH_DCMTK
#undef WITH_DICOM
#undef WITH_GDCM

	/*
	DCMTK read files
	https://support.dcmtk.org/redmine/projects/dcmtk/wiki/Howto_LoadMetaHeader
	*/
	#include "dcmtk/config/osconfig.h"
	#include "dcmtk/dcmdata/dctk.h"
	#include "dcmtk/dcmdata/dcistrmf.h"

#define INCLUDE_CSTRING
#define INCLUDE_CSTDLIB
#define INCLUDE_CSTDIO

	#include "dcmtk/ofstd/ofstdinc.h"

	#include "dcmtk/dcmdata/cmdlnarg.h"
	#include "dcmtk/ofstd/ofconapp.h"
	#include "dcmtk/dcmdata/dcuid.h"       /* for dcmtk version name */
	#include "dcmtk/dcmdata/dcostrmz.h"    /* for dcmZlibCompressionLevel */
	#include "dcmtk/dcmdata/dcistrmz.h"    /* for dcmZlibExpectRFC1950Encoding */
	#include "dcmtk/dcmdata/dcfilefo.h"	/* for convertToUTF8 */

	#ifdef WITH_ZLIB
	#include <zlib.h>                      /* for zlibVersion() */
	#endif
	#ifdef DCMTK_ENABLE_CHARSET_CONVERSION
	#include "dcmtk/ofstd/ofchrenc.h"      /* for OFCharacterEncoding */
	#endif


extern "C" int sopen_dcmtk_read(HDRTYPE* hdr) {

	if (PACKAGE_VERSION_NUMBER < 361)
		fprintf(stderr,"WARNING: DCMTK is not 3.6.1 but %s - this is not tested .\n",PACKAGE_VERSION);

	if (VERBOSE_LEVEL > 7) fprintf(stdout,"# %s (line %d): DCMTK %s is used to read dicom files.\n",__func__,__LINE__, PACKAGE_VERSION);

        OFCondition status;

	/* approach 1 */
	DcmMetaInfo metainfo;
	status = metainfo.loadFile(hdr->FileName);
	if (status.good()) {
		OFString sopClassUID, xferUID;
		if (metainfo.findAndGetOFString(DCM_MediaStorageSOPClassUID, sopClassUID).good())
			COUT << "SOP Class UID: " << sopClassUID << OFendl;
		if (metainfo.findAndGetOFString(DCM_TransferSyntaxUID, xferUID).good())
			COUT << "Transfer Syntax UID: " << xferUID << OFendl;
		metainfo.print(COUT);
	}
	else {
		biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "cannot read dicom file");
	}

	if (VERBOSE_LEVEL > 7) fprintf(stdout,"# %s (line %d): DCMTK is used to read dicom files.\n",__func__,__LINE__);

	const char *opt_ifname = hdr->FileName;

	E_FileReadMode opt_readMode = ERM_autoDetect;
	E_TransferSyntax opt_ixfer = EXS_Unknown;
	E_TransferSyntax opt_oxfer = EXS_Unknown;
	E_GrpLenEncoding opt_oglenc = EGL_recalcGL;
	E_EncodingType opt_oenctype = EET_ExplicitLength;
	E_PaddingEncoding opt_opadenc = EPD_noChange;
	OFCmdUnsignedInt opt_filepad = 0;
	OFCmdUnsignedInt opt_itempad = 0;

	#ifdef WITH_ZLIB
		OFCmdUnsignedInt opt_compressionLevel = 0;
	#endif
	#ifdef DCMTK_ENABLE_CHARSET_CONVERSION
		const char *opt_convertToCharset = NULL;
		size_t opt_conversionFlags = 0;
	#endif
	OFBool opt_noInvalidGroups = OFFalse;

	OFCondition cond;
	DcmFileFormat dfile;

        size_t printFlags = DCMTypes::PF_shortenLongTagValues;
	const char* pixelFileName = NULL;
        size_t pixelCounter = 0;
        int out=1; // stdout

        status = dfile.loadFile(hdr->FileName, opt_ixfer, EGL_noChange, DCM_MaxReadLength, opt_readMode);
        if (status.good()) {

		if (VERBOSE_LEVEL > 7) fprintf(stdout,"# %s (line %d): loadFile successful.\n",__func__,__LINE__);

		OFString csetString;
                DcmDataset *dset = dfile.getDataset();
		if (dset->findAndGetOFStringArray(DCM_SpecificCharacterSet, csetString).good()) {

			if (VERBOSE_LEVEL > 7) fprintf(stdout,"# %s (line %d): findAndGetOFStringArray successful.\n",__func__,__LINE__);

	                if (csetString.compare("ISO_IR 192") != 0 && csetString.compare("ISO_IR 6") != 0) {
#ifdef DCMTK_ENABLE_CHARSET_CONVERSION
		                    /* convert all DICOM strings to UTF-8 */
		                    if (VERBOSE_LEVEL>7) fprintf(stdout,"# %s (line %d): converting all element values that are affected by SpecificCharacterSet (0008,0005) to UTF-8.\n",__func__,__LINE__);

		                    status = dset->convertToUTF8();
		                    if (status.bad()) biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "converting file to UTF-8 failed");
#else
				    biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "DICOM read error (character set conversion not available)");
#endif
			}
		}

		if (VERBOSE_LEVEL > 7) fprintf(stdout,"# %s (line %d): \n",__func__,__LINE__);
/*
		// this fails for example file
		cond = dfile.convertToUTF8();
		if (cond.bad())
			biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "converting file to UTF-8 failed");

		if (VERBOSE_LEVEL > 7) fprintf(stdout,"# %s (line %d): convertToUTF8 successful.\n",__func__,__LINE__);
*/

		/***** extract header1 fields *****/
	        DcmTagKey key;
		const char *str;

		if (VERBOSE_LEVEL > 7) fprintf(stdout,"# %s (line %d): \n",__func__,__LINE__);

		// Patient Name
	        key.set(OFstatic_cast(Uint16, 0x0010), OFstatic_cast(Uint16, 0x0010));
		if (dset->findAndGetString(key, str).good())
			strncpy(hdr->Patient.Name, str, MAX_LENGTH_NAME+1);

		if (VERBOSE_LEVEL > 7) fprintf(stdout,"# %s (line %d): \n",__func__,__LINE__);

		// Patient ID
	        key.set(OFstatic_cast(Uint16, 0x0010), OFstatic_cast(Uint16, 0x0020));
		if (dset->findAndGetString(key, str).good())
			strncpy(hdr->Patient.Id, str, MAX_LENGTH_PID+1);

		if (VERBOSE_LEVEL > 7) fprintf(stdout,"# %s (line %d): \n",__func__,__LINE__);

		// (0010, 0040) Patient's Birthday                       CS: ''
	        key.set(OFstatic_cast(Uint16, 0x0010), OFstatic_cast(Uint16, 0x0030));
		if (dset->findAndGetString(key, str).good()) {
		        if ( (str != NULL) && (strlen(str) == 8)) {
				char *datestr = strdup(str);
				struct tm t;
				t.tm_mday=atoi(datestr+6);
				datestr[6]=0;
				t.tm_mon=atoi(datestr+4)-1;
				datestr[4]=0;
				t.tm_year=atoi(datestr);
				free(datestr);
				hdr->T0 = tm_time2gdf_time(&t);
			}
			else {
				if (VERBOSE_LEVEL > 7) fprintf(stdout,"# %s (line %d): invalid birthday string <%s>\n",__func__,__LINE__,str ? str : "");
			}
		}

		// (0010, 0040) Patient's Sex                       CS: ''
	        key.set(OFstatic_cast(Uint16, 0x0010), OFstatic_cast(Uint16, 0x0040));
		if (dset->findAndGetString(key, str).good() && str!=NULL) {
			if (strlen(str)==0)
				hdr->Patient.Sex = 0; // unknown
			else if (str[0]=='m' || str[0]=='M')
				hdr->Patient.Sex = 1; // male
			else if (str[0]=='f' || str[0]=='F')
				hdr->Patient.Sex = 2; // female
		}

		if (VERBOSE_LEVEL > 7) fprintf(stdout,"# %s (line %d): \n",__func__,__LINE__);

		// AcquisitionDateTime
	        key.set(OFstatic_cast(Uint16, 0x0008), OFstatic_cast(Uint16, 0x002a));
		if (dset->findAndGetString(key, str).good() && str!=NULL) {
		        if ( (str != NULL) && (strlen(str) == 8)) {
				char *datestr = strdup(str);
				struct tm t0;
				datestr[14]=0;
				t0.tm_sec = atoi((char*)datestr+12);
				datestr[12]=0;
				t0.tm_min = atoi((char*)datestr+10);
				datestr[10]=0;
				t0.tm_hour = atoi((char*)datestr+8);
				datestr[8]=0;
				t0.tm_mday = atoi((char*)datestr+6);
				datestr[6]=0;
				t0.tm_mon = atoi((char*)datestr+4)-1;
				datestr[4]=0;
				t0.tm_year = atoi((char*)datestr)-1900;

				hdr->T0 = tm_time2gdf_time(&t0);
			}
			else {
				if (VERBOSE_LEVEL > 7) fprintf(stdout,"# %s (line %d): invalid birthday string <%s>\n",__func__,__LINE__,str ? str : "");
			}
		}

		if (VERBOSE_LEVEL > 7) fprintf(stdout,"# %s (line %d): \n",__func__,__LINE__);
		// Manufacturer
	        key.set(OFstatic_cast(Uint16, 0x0008), OFstatic_cast(Uint16, 0x0070));
		if (dset->findAndGetString(key, str).good())
			if (str) strncpy(hdr->ID.Manufacturer._field, str, MAX_LENGTH_MANUF+1);

		if (VERBOSE_LEVEL > 7) fprintf(stdout,"# %s (line %d): read H1 successful.\n",__func__,__LINE__);

	        DcmStack stack;
	        key.set(OFstatic_cast(Uint16, 0x5400), OFstatic_cast(Uint16, 0x0100));	// Waveform sequence
	        if (dfile.search(key, stack, ESM_fromHere, OFTrue) == EC_Normal) {
			if (stack.card() > 1)
				fprintf(stderr,"File %s contains more than one (%ld) waveform sequences - only first one will be read \n", hdr->FileName, stack.card());

		if (VERBOSE_LEVEL > 7) fprintf(stdout,"# %s (line %d): %ld wav seqs\n",__func__,__LINE__,stack.card());

			// FIXME: read meta information

//	        	DcmObject *waveformSequence = stack.top();
			DcmItem *waveformSequence = (DcmItem*)stack.elem(0);
//			DcmObject *waveformSequence = stack.elem(0);
			uint16_t US;
			uint32_t UL;
			//  (003a, 0005) Number of Waveform Channels         US: 23
		        key.set(OFstatic_cast(Uint16, 0x003a), OFstatic_cast(Uint16, 0x0005));
			if (waveformSequence->findAndGetUint16(key, US).good())
				hdr->NS=US;

		if (VERBOSE_LEVEL > 7) fprintf(stdout,"# %s (line %d): %d \n",__func__,__LINE__,US);

			//  (003a, 0010) Number of Waveform Samples
		        key.set(OFstatic_cast(Uint16, 0x003a), OFstatic_cast(Uint16, 0x0010));
			if (waveformSequence->findAndGetUint32(key, UL).good())
				hdr->SPR=UL;

		if (VERBOSE_LEVEL > 7) fprintf(stdout,"# %s (line %d): %d \n",__func__,__LINE__,UL);

			//   (003a, 001a) Sampling Frequency                  DS: "256"
		        key.set(OFstatic_cast(Uint16, 0x003a), OFstatic_cast(Uint16, 0x001a));
			if (waveformSequence->findAndGetString(key, str).good())
				hdr->SampleRate = strtod(str, NULL);

		if (VERBOSE_LEVEL > 7) fprintf(stdout,"# %s (line %d): %s \n",__func__,__LINE__,str);
#if 0
		        if (waveformSequence->search(key, stack, ESM_fromHere, OFTrue) == EC_Normal) {
				if (VERBOSE_LEVEL>7) fprintf(stdout,"# %s (line %d): #%ld/%ld  (%04x,%04x) %d\n",__func__,__LINE__,-1, stack.card(),dobj->getGTag(),dobj->getETag(),dobj->isaString());
			}
#endif
	        }

		/***** TODO: extract header2 and header 3 fields *****/

		hdr->CHANNEL = (CHANNEL_TYPE*)realloc(hdr->CHANNEL, hdr->NS * sizeof(CHANNEL_TYPE));
		for (uint16_t k=0; k < hdr->NS; k++) {
			CHANNEL_TYPE* hc = hdr->CHANNEL+k;
			// FIXME: extract corresponding fields from DICOM
			hc->OnOff    = 1;
			hc->GDFTYP   = 3;
			hc->SPR      = hdr->SPR;
			hc->Cal      = 0.1;
			hc->Off      = 0.0;
			hc->Transducer[0] = '\0';
			hc->LowPass  = NAN;
			hc->HighPass = NAN;
			hc->PhysMax  =  3276.7;
			hc->PhysMin  = -3276.8;
			hc->DigMax   =  32767;
			hc->DigMin   = -32768;
			hc->LeadIdCode  = 0;
			hc->PhysDimCode = 4275;	//uV
			hc->bi   = k*hdr->SPR*2;

			char *label = (char*)(hdr->AS.Header+1034+k*512);
			if ( (hdr->AS.Header[1025+k*512]=='E') && strlen(label)<13) {
				strcpy(hc->Label, "EEG ");
				strcat(hc->Label, label);
			}
			else {
				size_t len = MAX_LENGTH_LABEL;
				strncpy(hc->Label, label, len);
				hc->Label[len]=0;
			}
		}

		/***** TODO: extract event information  *****/
		/*
			hdr->EVENT.SampleRate =
			hdr->EVENT.POS =
			hdr->EVENT.TYP =
			hdr->EVENT.CHN =
			hdr->EVENT.DUR =
			hdr->EVENT.CodeDesc =
			hdr->EVENT.LenCodeDesc =
		*/

		/***** TODO: extract data  *****/
		/*
			hdr->data.block =
			hdr->data.size =
		*/
	}
	else {
		fprintf(stderr,"# %s (line %d): DCMTK failed to read dicom file %s.\n",__func__,__LINE__, hdr->FileName);
		biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "DICOM support not completed");
	}

	if (VERBOSE_LEVEL>7) fprintf(stdout,"# %s (line %d): DCMTK is used to read dicom files.\n",__func__,__LINE__);


#if 0
        searchKey.set(OFstatic_cast(Uint16, 0x0010), OFstatic_cast(Uint16, 0x0010));	// Patient Name
        if (dset->search(searchKey, stack, ESM_fromHere, OFTrue) == EC_Normal) {
		DcmObject *dobj = stack.top();
		if (VERBOSE_LEVEL>7) fprintf(stdout,"# %s (line %d): #%ld/%ld  len=%ld (%04x,%04x) %d\n",__func__,__LINE__,-1, stack.card(),dobj->getLengthField(),dobj->getGTag(),dobj->getETag(),dobj->isaString());
/*
		if (dobj->isaString())
			strncpy(hdr->AS.PID,,MAX_LENGTH_PID); dobj->getLengthField)=
 */
        }

	/* walk through whole stack */
        while (dset->nextObject(stack, 1) == EC_Normal) {
		DcmObject *dobj = stack.top();
		if (VERBOSE_LEVEL>7) fprintf(stdout,"# %s (line %d): #%d/%d  len=%ld (%04x,%04x)\n",__func__,__LINE__,-1, stack.card(),dobj->getLengthField(),dobj->getGTag(),dobj->getETag());

		for (int k=0; k<stack.card(); k++) {
			DcmObject *dobj = stack.elem(k);
		if (VERBOSE_LEVEL>7) fprintf(stdout,"# %s (line %d): 		#%d/%d  len=%ld (%04x,%04x)\n",__func__,__LINE__,k, stack.card(),dobj->getLengthField(),dobj->getGTag(),dobj->getETag());
		}
        }

        if (dset->search(searchKey, stack, ESM_fromHere, OFTrue) == EC_Normal) {

	if (VERBOSE_LEVEL>7) fprintf(stdout,"# %s (line %d): DCMTK was used to read dicom files. size of stack is %d\n",__func__,__LINE__,stack.card());

//                printResult(out, stack, printFlags, pixelFileName, &pixelCounter);
                if (0) // (printAllInstances)
                {
                    //while (dset->search(searchKey, stack, ESM_afterStackTop, OFTrue) == EC_Normal) {
                    while (dset->search(searchKey, stack) == EC_Normal) {
                      //printResult(out, stack, printFlags, pixelFileName, &pixelCounter);
	if (VERBOSE_LEVEL>7) fprintf(stdout,"# %s (line %d): DCMTK was used to read dicom files. size of stack is %d\n",__func__,__LINE__,stack.card());

                    }
                }
        }
#endif

	hdr2ascii(hdr,stdout,4);
	biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, ( "DICOM support not completed (see TOOD's and FIXME's in function SOPEN_DCMTK_READ) " ));

}
#endif  // DCMTK


