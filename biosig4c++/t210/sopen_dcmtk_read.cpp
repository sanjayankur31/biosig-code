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

	DcmFileFormat dfile;

	if (dfile.loadFile(hdr->FileName, opt_ixfer, EGL_noChange, DCM_MaxReadLength, opt_readMode).bad()) {
		fprintf(stderr,"# %s (line %d): DCMTK failed to read dicom file %s.\n",__func__,__LINE__, hdr->FileName);
		biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "DICOM support not completed");
		return -1;
	}

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

		DcmSequenceOfItems *seq = NULL;
		if (dset->findAndGetSequence(DCM_WaveformSequence, seq).bad() || (seq==NULL)) {
			biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "DICOM read error - no Waveform sequence found");
			return -1;
		}

		Uint32 numGroups = seq->card();
		if (numGroups == 0) {
			biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "DICOM read error - no multiplex group founds");
			return -1;
		}
		for (uint32_t i = 0; i < numGroups; i++) {
			DcmItem *item = seq->getItem(i);
			DcmStack stack;
			DcmElement *el;
			OFCondition result;

			// Number Of Waveform Channels
			Uint16 numChannels;
			if (item->findAndGetUint16(DCM_NumberOfWaveformChannels, numChannels).good())
				hdr->NS=numChannels;

			// Number of samples
			Uint32 numSamples;
			if (item->findAndGetUint32(DCM_NumberOfWaveformSamples, numSamples).good())
				hdr->SPR=numSamples;

			// Sampling Frequency
			if (item->findAndGetString(DCM_SamplingFrequency, str).good())
				hdr->SampleRate = strtod(str, NULL);

			// read the channels
			hdr->CHANNEL = (CHANNEL_TYPE*)realloc(hdr->CHANNEL, hdr->NS * sizeof(CHANNEL_TYPE));

			DcmSequenceOfItems *channelSeq;
			item->findAndGetSequence(DCM_ChannelDefinitionSequence, channelSeq);
			assert(hdr->NS == channelSeq->card()); // should be identical to numChannels !


			/***** extract header2 and header 3 fields *****/

			// Read channel attributes
			size_t bi=0;
			hdr->NRec = 1;
			for (uint32_t k=0; k < hdr->NS; k++) {

				DcmItem *channelItem = channelSeq->getItem(k);

				CHANNEL_TYPE* hc = hdr->CHANNEL + k;

				hc->SPR      = hdr->SPR;
				hc->Cal      = 1.0;
				hc->Off      = 0.0;
				hc->Transducer[0] = '\0';
				hc->LowPass  = 0.0/0.0;
				hc->HighPass = 0.0/0.0;

				hc->DigMax   = (1<<15)-1;
				hc->DigMin   = -1<<15;
				hc->LeadIdCode  = 0;
				hc->PhysDimCode = 0;	// undefined

				// channel label
				if (channelItem->findAndGetString(DCM_ChannelLabel, str).good())
					strncpy(hc->Label, str, MAX_LENGTH_LABEL);

				// channel sensitivity
				if (channelItem->findAndGetString(DCM_ChannelSensitivity, str).good()) {
					hc->Cal = strtod(str, NULL);
				}
				if (channelItem->findAndGetString(DCM_ChannelBaseline, str).good()) {
					hc->Off = strtod(str, NULL);
				}
				if (channelItem->findAndGetString(DCM_FilterLowFrequency, str).good()) {
					hc->HighPass = strtod(str, NULL);
				}
				if (channelItem->findAndGetString(DCM_FilterHighFrequency, str).good()) {
					hc->LowPass = strtod(str, NULL);
				}
				if (channelItem->findAndGetString(DCM_NotchFilterFrequency, str).good()) {
					hc->Notch = strtod(str, NULL);
				}


				Uint16 gdftyp;
				if (channelItem->findAndGetUint16(DCM_WaveformBitsStored, gdftyp).good()) {
					hc->bi   = bi;
					switch (gdftyp) {
					case 32: hc->GDFTYP = 5;
						hc->DigMax   = (1<<31)-1;
						hc->DigMin   = -1<<31;
						break;
					case 24: hc->GDFTYP = 3+256;
						hc->DigMax   = (1<<23)-1;
						hc->DigMin   = -1<<23;
						break;
					case 16: hc->GDFTYP = 3;
						hc->DigMax   = (1<<15)-1;
						hc->DigMin   = -1<<15;
						break;
					case  8: hc->GDFTYP = 1;
						hc->DigMax   = (1<<7)-1;
						hc->DigMin   = -1<<7;
						break;
					default: hc->GDFTYP = 3;
						hc->DigMax   = (1<<15)-1;
						hc->DigMin   = -1<<15;
					}
					hc->PhysMax  = hc->DigMax*hc->Cal+hc->Off;
					hc->PhysMin  = hc->DigMin*hc->Cal+hc->Off;
					bi += hc->SPR * GDFTYP_BITS[hc->GDFTYP] >> 3;
					hdr->AS.bpb = bi;
				}

				// TODO: LeadIdCode
				DcmSequenceOfItems *channelSourceSeq;
				channelItem->findAndGetSequence(DCM_ChannelSourceSequence, channelSourceSeq);

				// TODO: Physical Units
				DcmSequenceOfItems *channelUnitsSeq;
				channelItem->findAndGetSequence(DCM_ChannelSensitivityUnitsSequence, channelUnitsSeq);

		if (VERBOSE_LEVEL > 7) fprintf(stdout,"# %s (line %d): card <%d>\n",__func__,__LINE__,channelSourceSeq->card());
			}

			// TODO: waveform data
			DcmElement *wvfData;
			result = item->findAndGetElement(DCM_WaveformData, wvfData);
			wvfData->getVR(); // OB, OW,ox

		if (VERBOSE_LEVEL > 7) fprintf(stdout,"# %s (line %d)\n",__func__,__LINE__);
			hdr->AS.rawdata = (uint8_t*)realloc(hdr->AS.rawdata, hdr->AS.bpb*hdr->NRec);
			void *ptr=hdr->AS.rawdata;

			wvfData->getUint8Array((Uint8*&)ptr);
			hdr->AS.length = 1;

		if (VERBOSE_LEVEL > 7) fprintf(stdout,"# %s (line %d) %d %d\n",__func__,__LINE__,hdr->AS.bpb,hdr->NRec);

			memcpy(hdr->AS.rawdata,ptr,hdr->AS.bpb*hdr->NRec);

		if (VERBOSE_LEVEL > 7) fprintf(stdout,"# %s (line %d)\n",__func__,__LINE__);

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

	if (VERBOSE_LEVEL>7) fprintf(stdout,"# %s (line %d): DCMTK is used to read dicom files.\n",__func__,__LINE__);

	hdr2ascii(hdr,stdout,4);
	biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, ( "DICOM support not completed (see TOOD's and FIXME's in function SOPEN_DCMTK_READ) " ));

}
#endif  // DCMTK


