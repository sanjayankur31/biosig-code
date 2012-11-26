/*

    sandbox is used for development and under constraction work
    The functions here are either under construction or experimental.
    The functions will be either fixed, then they are moved to another place;
    or the functions are discarded. Do not rely on the interface in this function

    $Id$
    Copyright (C) 2008,2009,2010,2011,2012 Alois Schloegl <alois.schloegl@gmail.com>
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

#define WITH_TIMESTAMP

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../biosig-dev.h"

// these functios are stubs

#ifdef WITH_DCMTK
#undef WITH_DICOM
#undef WITH_GDCM


extern "C" int sopen_dicom_read(HDRTYPE* hdr) {
	fprintf(stdout,"DCMTK is used to read dicom files.\n");

}

#endif

#ifdef WITH_HDF
#include <hdf5.h>
#endif 
#ifdef WITH_MATIO
#include <matio.h>
#endif 


#ifdef WITH_GDCM
#undef WITH_DICOM

#include "gdcmReader.h"
//#include "gdcmImageReader.h"
//#include "gdcmWriter.h"
#include "gdcmDataSet.h"
#include "gdcmAttribute.h"

//#include "gdcmCommon.h"
#include "gdcmPreamble.h"
#include "gdcmFile.h"
#include "gdcmFileMetaInformation.h"
#include "gdcmReader.h"
#include "gdcmImageReader.h"
#include "gdcmWriter.h"
#include "gdcmDataSet.h"
#include <gdcmAttribute.h>
#include <gdcmWaveform.h>

extern "C" int sopen_dicom_read(HDRTYPE* hdr) {

	fprintf(stdout,"GDCM is used to read dicom files.\n");

	gdcm::Reader r;
        const gdcm::DataElement *de;
	r.SetFileName( hdr->FileName );
	if( !r.Read() )
		return 1;

	gdcm::File &file = r.GetFile();
	gdcm::FileMetaInformation &header = file.GetHeader();
	if ( header.FindDataElement( gdcm::Tag(0x0002, 0x0013 ) ) )
		const gdcm::DataElement &de = header.GetDataElement( gdcm::Tag(0x0002, 0x0013) );

	gdcm::DataSet &ds = file.GetDataSet();

	if ( header.FindDataElement( gdcm::Tag(0x0002, 0x0010 ) ) )
		de = &header.GetDataElement( gdcm::Tag(0x0002, 0x0010) );


fprintf(stdout,"attr <0x0002,0x0010> len=%i\n",de->GetByteValue() );


/*
	{
		gdcm::Attribute<0x28,0x100> at;
		at.SetFromDataElement( ds.GetDataElement( at.GetTag() ) );
		if( at.GetValue() != 8 )
			return 1;
		//at.SetValue( 32 );
		//ds.Replace( at.GetAsDataElement() );
	}
	{

fprintf(stdout,"attr <0x0008,0x002a>\n");
		gdcm::Attribute<0x0008,0x002a> at;
fprintf(stdout,"attr <0x0008,0x002a>\n");
 		ds.GetDataElement( at.GetTag() );
fprintf(stdout,"attr <0x0008,0x002a>\n");
		at.SetFromDataElement( ds.GetDataElement( at.GetTag() ) );

		fprintf(stdout,"DCM: [0008,002a]: %i %p\n",at.GetNumberOfValues(), at.GetValue());
	}
*/
	{

fprintf(stdout,"attr <0x0008,0x0023>\n");
		gdcm::Attribute<0x0008,0x0023> at;
fprintf(stdout,"attr <0x0008,0x0023>\n");
 		ds.GetDataElement( at.GetTag() );
fprintf(stdout,"attr <0x0008,0x0023>\n");
//		at.SetFromDataElement( ds.GetDataElement( at.GetTag() ) );

//		fprintf(stdout,"DCM: [0008,0023]: %i %p\n",at.GetNumberOfValues(), at.GetValue());
	}


/*
				{
				struct tm t0;
				hdr->AS.Header[pos+14]=0;
				t0.tm_sec = atoi((char*)hdr->AS.Header+pos+12);
				hdr->AS.Header[pos+12]=0;
				t0.tm_min = atoi((char*)hdr->AS.Header+pos+10);
				hdr->AS.Header[pos+10]=0;
				t0.tm_hour = atoi((char*)hdr->AS.Header+pos+8);
				hdr->AS.Header[pos+8]=0;
				t0.tm_mday = atoi((char*)hdr->AS.Header+pos+6);
				hdr->AS.Header[pos+6]=0;
				t0.tm_mon = atoi((char*)hdr->AS.Header+pos+4)-1;
				hdr->AS.Header[pos+4]=0;
				t0.tm_year = atoi((char*)hdr->AS.Header+pos)-1900;

				hdr->T0 = tm_time2gdf_time(&t0);
				break;
				}

*/
}
#endif



#ifdef __cplusplus
extern "C" {
#endif


#define ITX_MAXLINELENGTH 400

char *IgorChanLabel(char *inLabel, HDRTYPE *hdr, size_t *ngroup, size_t *nseries, size_t *nsweep, size_t *ns) {
	/*
		extract Channel Label of IGOR ITX data format
	*/

	*ns = 0;
	// static char Label[ITX_MAXLINELENGTH+1];
	int k, s = 0, pos4=0, pos1=0;
	for (k = strlen(inLabel); inLabel[k] < ' '; k--);
	inLabel[k+1] = 0;

	while (inLabel[k] >= ' ') {
		while ( inLabel[k] >= '0' && inLabel[k] <= '9' )
			k--;
		if (inLabel[k]=='_') {
			s++;
			if (s==1) pos4 = k;
			if (s==4) pos1 = k;
			k--;
		}
		if ( inLabel[k] < '0' || inLabel[k] > '9' )
			break;
	}

	if (3 < s) {
		char nvar = 0;
		for (k = strlen(inLabel); 0 < k && nvar < 4; k--) {
			if (inLabel[k] == '_') {
				inLabel[k] = 0;
				char  *v = inLabel+k+1;
				size_t n = atol(v);

				switch (nvar) {
				case 0: *ns = n;
					nvar++;
					break;
				case 1: *nsweep = n;
					nvar++;
					break;
				case 2: *nseries = n;
					nvar++;
					break;
				case 3: *ngroup = n;
					nvar++;
					break;
				}
				inLabel[k] = 0;
			}
		}
		for (k=1; inLabel[pos4+k-1]; k++) {
			inLabel[pos1+k] = inLabel[pos4+k];
		}
	}

	for (k=0; k<hdr->NS; k++) {

	}

	if ((*ns)+1 > hdr->NS) {	// another channel
		hdr->NS = (*ns)+1;
		hdr->CHANNEL = (CHANNEL_TYPE*)realloc(hdr->CHANNEL, hdr->NS * sizeof(CHANNEL_TYPE));
	}

	return(inLabel);
}

#ifdef WITH_HDF
int sopen_hdf5(HDRTYPE* hdr) {
        /*
                file hdr->FileName is already opened and hdr->HeadLen bytes are read
                These are available from hdr->AS.Header. 

                ToDo: populate hdr 
        */
	size_t count = hdr->HeadLen;
        fprintf(stdout,"Trying to read HDF data using \"%s\"\n",H5_VERS_INFO);  

	ifclose(hdr);

        return(-1); 
}
#endif 

#ifdef WITH_MATIO
int sopen_matlab(HDRTYPE* hdr) {
        /*
                file hdr->FileName is already opened and hdr->HeadLen bytes are read
                These are available from hdr->AS.Header. 

                ToDo: populate hdr 
			sanity checks 
			memory leaks
        */
	ifclose(hdr);
	//size_t count = hdr->HeadLen;

        fprintf(stdout, "Trying to read Matlab data using MATIO v%i.%i.%i\n", MATIO_MAJOR_VERSION, MATIO_MINOR_VERSION, MATIO_RELEASE_LEVEL);  

	mat_t *matfile = Mat_Open(hdr->FileName, MAT_ACC_RDONLY);
	matvar_t *EEG=NULL, *pnts=NULL, *nbchan=NULL, *trials=NULL, *srate=NULL, *data=NULL, *chanlocs=NULL, *event=NULL;
	if (matfile != NULL) {
		EEG    = Mat_VarRead(matfile, "EEG" );
		if (EEG != NULL) {
			Mat_VarReadDataAll(matfile, EEG );
			pnts   = Mat_VarGetStructField(EEG, "pnts", BY_NAME, 0);
			nbchan = Mat_VarGetStructField(EEG, "nbchan", BY_NAME, 0);
			trials = Mat_VarGetStructField(EEG, "trials", BY_NAME, 0);
			srate  = Mat_VarGetStructField(EEG, "srate", BY_NAME, 0);
			data   = Mat_VarGetStructField(EEG, "data", BY_NAME, 0);
			chanlocs = Mat_VarGetStructField(EEG, "chanlocs", BY_NAME, 0);
			event    = Mat_VarGetStructField(EEG, "event", BY_NAME, 0);
			
			hdr->NS  = *(double*)(nbchan->data);
			hdr->SPR = *(double*)(pnts->data);
			hdr->NRec= *(double*)(trials->data);
			hdr->SampleRate = *(double*)(srate->data);
			
/* TODO CB
			hdr->NRec 	 = ;
			hdr->SPR  	 = ;
			hdr->T0 	 = 0;        // Unknown;
			uint16_t gdftyp  = ;	16: float; 17: double
*/
			hdr->CHANNEL = (CHANNEL_TYPE*) realloc(hdr->CHANNEL, hdr->NS * sizeof(CHANNEL_TYPE));
			size_t k;
			for (k=0; k<hdr->NS; k++) {
				CHANNEL_TYPE *hc = hdr->CHANNEL+k;
				sprintf(hc->Label,"#%2d",k+1);
				hc->SPR = hdr->SPR;
/* TODO CB
				hc->GDFTYP = gdftyp; 
				hc->Transducer[0] = '\0';
			    	hc->LowPass	= ;
			    	hc->HighPass = ;
			    	hc->Notch	= ;  // unknown
			    	hc->PhysMax	= ;
			    	hc->DigMax	= ;
			    	hc->PhysMin	= ;
			    	hc->DigMin	= ;
			    	hc->Cal	 	= 1.0;
			    	hc->Off	 	= 0.0;
				hc->OnOff    	= 1;
			    	hc->PhysDimCode = 4275; // uV
			    	hc->LeadIdCode  = 0;
			    	hc->bi      	= k*GDFTYP_BITS[gdftyp]>>3;	// TODO AS
*/
			}

			size_t sz = hdr->NS*hdr->SPR*hdr->NRec*GDFTYP_BITS[gdftyp]>>3;
			hdr->AS.rawdata = realloc(hdr->AS.rawdata, sz); 
/* TODO CB
			memcpy(hdr->AS.rawdata,...,sz); 
*/
			hdr->EVENT.N = 0; 	// TODO CB
			hdr->EVENT.POS = (uint32_t*) realloc(hdr->EVENT.POS, hdr->EVENT.N*sizeof(*hdr->EVENT.POS));
			hdr->EVENT.TYP = (uint16_t*) realloc(hdr->EVENT.TYP, hdr->EVENT.N*sizeof(*hdr->EVENT.TYP));
			hdr->EVENT.DUR = (uint32_t*) realloc(hdr->EVENT.DUR, hdr->EVENT.N*sizeof(*hdr->EVENT.DUR));
			hdr->EVENT.CHN = (uint16_t*) realloc(hdr->EVENT.CHN, hdr->EVENT.N*sizeof(*hdr->EVENT.CHN));
			for (k=0; k<hdr->EVENT.N; k++) {
/* TODO CB
				hdr->EVENT.TYP[k] = 			
				FreeTextEvent(hdr, k, annotation)
				hdr->EVENT.POS[k] = 			
				hdr->EVENT.CHN[k] = 0; 			
				hdr->EVENT.DUR[k] = 0;			
*/
			}

		hdr->AS.bpb = hdr->NS*2;
		hdr->FLAG.OVERFLOWDETECTION = 0; 	// BKR does not support automated overflow and saturation detection


			Mat_VarPrint(pnts,   1);
			Mat_VarPrint(nbchan, 1);
			Mat_VarPrint(trials, 1);
			Mat_VarPrint(srate,  1);
			//Mat_VarPrint(data,   1);
			//Mat_VarPrint(chanlocs, 1);
			//Mat_VarPrint(event,  1);

			
			Mat_VarFree(EEG);
		}

		Mat_Close(matfile);
	}
	
        return (0); 
}
#endif 

/****************************************************************************
   heka2gdftime 
     converts heka time format into gdftime 
 ****************************************************************************/
gdf_time heka2gdftime(double t) {		
	t -= 1580970496; if (t<0) t += 4294967296; t += 9561652096;
	return (uint64_t)ldexp(t/(24.0*60*60) + 584755, 32); // +datenum(1601,1,1));
}		

/****************************************************************************
   sopen_heka
     reads heka format 

     if itx is not null, the file is converted into an ITX formated file 
     and streamed to itx, too. 	
 ****************************************************************************/
void sopen_heka(HDRTYPE* hdr, FILE *itx) {
	size_t count = hdr->HeadLen;

	if (hdr->TYPE==HEKA && hdr->VERSION > 0) {

/* TODO: HEKA support of eventtable 
	+ support for resampling (needed by mexSLOAD, SigViewer, save2gdf)
	+ support for selection of experiment,series,sweep,trace (needed by MMA)
	+ make event-table aware of Sweep Selection
	- event-table -> MMA
*/

		int32_t Levels=0;
		uint16_t k;
		//int32_t *Sizes=NULL;
		int32_t Counts[5], counts[5]; //, Sizes[5];
		memset(Counts,0,20);
		memset(counts,0,20);
		//memset(Sizes,0,20);
		uint32_t StartOfData=0,StartOfPulse=0;

		union {
			struct {
				int32_t Root;
				int32_t Group;
				int32_t Series;
				int32_t Sweep;
				int32_t Trace;
			} Rec;
			int32_t all[5];
		} Sizes;


    		// HEKA PatchMaster file format

		count = hdr->HeadLen;
		ifseek(hdr,0,SEEK_SET);
		hdr->AS.Header = (uint8_t*)realloc(hdr->AS.Header, 1024);
		count = ifread(hdr->AS.Header, 1, 1024, hdr);
		while (!ifeof(hdr)) {
			hdr->AS.Header = (uint8_t*)realloc(hdr->AS.Header, 2*count);
			count += ifread(hdr->AS.Header+count, 1, count, hdr);
		}
		hdr->HeadLen = count;

if (VERBOSE_LEVEL>7) fprintf(stdout,"HEKA 114\n");

		hdr->FILE.LittleEndian = *(uint8_t*)(hdr->AS.Header+52) > 0;
		char SWAP = (hdr->FILE.LittleEndian && (__BYTE_ORDER == __BIG_ENDIAN)) ||	  \
			    (!hdr->FILE.LittleEndian && (__BYTE_ORDER == __LITTLE_ENDIAN));

		double oTime;
		uint32_t nItems;
		if (hdr->FILE.LittleEndian) {
			oTime  = lef64p(hdr->AS.Header+40);
			nItems = leu32p(hdr->AS.Header+48);
		}
		else {
			oTime  = bef64p(hdr->AS.Header+40);
			nItems = beu32p(hdr->AS.Header+48);
		}

if (VERBOSE_LEVEL>7) fprintf(stdout,"HEKA 121 nItems=%i\n",nItems);

		if (hdr->VERSION == 1) {
			Sizes.Rec.Root   = 544;
			Sizes.Rec.Group  = 128;
			Sizes.Rec.Series = 1120;
			Sizes.Rec.Sweep  = 160;
			Sizes.Rec.Root   = 296;
		}
		else if (hdr->VERSION == 2)
		for (k=0; k < min(12,nItems); k++) {

if (VERBOSE_LEVEL>7) fprintf(stdout,"HEKA 131 nItems=%i\n",k);

			uint32_t start  = *(uint32_t*)(hdr->AS.Header+k*16+64);
			uint32_t length = *(uint32_t*)(hdr->AS.Header+k*16+64+4);
			if (SWAP) {
				start  = bswap_32(start);
				length = bswap_32(length);
			}
			uint8_t *ext = hdr->AS.Header + k*16 + 64 + 8;

if (VERBOSE_LEVEL>7) fprintf(stdout,"HEKA #%i: <%s> [%i:+%i]\n",k,ext,start,length);

			if (!start) break;

			if (!memcmp(ext,".pul\0\0\0\0",8)) {
				// find pulse data
				ifseek(hdr, start, SEEK_SET);

				//magic  = *(int32_t*)(hdr->AS.Header+start);
				Levels = *(int32_t*)(hdr->AS.Header+start+4);
				if (SWAP) Levels = bswap_32(Levels);
				if (Levels>5) {
					biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "Heka/Patchmaster format with more than 5 levels not supported");
					return;
				}

if (VERBOSE_LEVEL>7) fprintf(stdout,"HEKA124 #%i    Levels=%i\n",k,Levels);

				memcpy(Sizes.all,hdr->AS.Header+start+8,sizeof(int32_t)*Levels);
				if (SWAP) {
					int l;
					for (l=0; l < Levels; l++) Sizes.all[l] = bswap_32(Sizes.all[l]);
				}

if (VERBOSE_LEVEL>7) {int l; for (l=0; l < Levels; l++) fprintf(stdout,"HEKA #%i       %i\n",l, Sizes.all[l]); }

				StartOfPulse = start + 8 + 4 * Levels;
			}

			else if (!memcmp(ext,".dat\0\0\0\0",8)) {
				StartOfData = start;
			}
		}

if (VERBOSE_LEVEL>7) fprintf(stdout,"HEKA 989: \n");

		// if (!Sizes) free(Sizes); Sizes=NULL;

/* TODO: HEKA, check channel number and label 
	pass 1:
		+ get number of sweeps
		+ get number of channels
		+ check whether all traces of a single sweep have the same SPR, and Fs
		- check whether channelnumber (TrAdcChannel), scaling (DataScaler) and Label fit among all sweeps
		+ extract the total number of samples
		+ physical units
		+ level 4 may have no children
		+ count event descriptions Level2/SeLabel
	pass 2:
		+ initialize data to NAN
		+ skip sweeps if selected channel is not in it
		+ Y scale, physical scale
		+ Event.CodeDescription, Events,
		resampling
*/

		uint32_t k1=0, k2=0, k3=0, k4=0;
		uint32_t K1=0, K2=0, K3=0, K4=0, K5=0;
		char flagModifiedTraceHeaders = 0;
		double t;
		size_t pos;

if (VERBOSE_LEVEL>7) fprintf(stdout,"HEKA 995 %i %i \n",StartOfPulse, Sizes.Rec.Root);

		// read K1
		if (SWAP) {
			K1 		= bswap_32(*(uint32_t*)(hdr->AS.Header + StartOfPulse + Sizes.Rec.Root));
			hdr->VERSION 	= bswap_32(*(uint32_t*)(hdr->AS.Header + StartOfPulse));
			union {
				double f64;
				uint64_t u64;
			} c;	
			c.u64 = bswap_64(*(uint64_t*)(hdr->AS.Header + StartOfPulse + 520));
			t = c.f64;
		} else {
			K1 		= (*(uint32_t*)(hdr->AS.Header + StartOfPulse + Sizes.Rec.Root));
			hdr->VERSION 	= (*(uint32_t*)(hdr->AS.Header + StartOfPulse));
			t  		= (*(double*)(hdr->AS.Header + StartOfPulse + 520));
		}

if (VERBOSE_LEVEL>7) fprintf(stdout,"HEKA 997\n");
		
		hdr->T0 = heka2gdftime(t);
		hdr->SampleRate = 0.0;
		double *DT = NULL; 	// list of sampling intervals per channel
		hdr->SPR = 0;

if (VERBOSE_LEVEL>7) fprintf(stdout,"HEKA 999 %p\n",hdr->EVENT.CodeDesc);

		/**************************************************************************
			HEKA: read structural information
 		 **************************************************************************/

		pos = StartOfPulse + Sizes.Rec.Root + 4;
		size_t EventN=0;

		for (k1=0; k1<K1; k1++)	{
		// read group

if (VERBOSE_LEVEL>7) fprintf(stdout,"HEKA L1 @%i=\t%i/%i \n",(int)(pos+StartOfData),k1,K1);

			pos += Sizes.Rec.Group+4;
			// read number of children
			K2 = (*(uint32_t*)(hdr->AS.Header+pos-4));

			hdr->AS.auxBUF = (uint8_t*)realloc(hdr->AS.auxBUF,K2*33);	// used to store name of series
			for (k2=0; k2<K2; k2++)	{
				// read series
				union {
					double   f64;
					uint64_t u64;
				} Delay;
				char *SeLabel =  (char*)(hdr->AS.Header+pos+4);		// max 32 bytes
				strncpy((char*)hdr->AS.auxBUF + 33*k2, (char*)hdr->AS.Header+pos+4, 32); hdr->AS.auxBUF[33*k2+32] = 0;
				SeLabel = (char*)hdr->AS.auxBUF + 33*k2;
				double t  = *(double*)(hdr->AS.Header+pos+136);		// time of series. TODO: this time should be taken into account 
				Delay.u64 = bswap_64(*(uint64_t*)(hdr->AS.Header+pos+472+176));
	
				struct tm tm;
				gdf_time2tm_time_r(heka2gdftime(t),&tm); 
if (VERBOSE_LEVEL>7) fprintf(stdout,"HEKA L2 @%i=%s %f\t%i/%i %i/%i     t=%.17g %s\n",(int)(pos+StartOfData),SeLabel,Delay.f64,k1,K1,k2,K2,t,asctime(&tm));

				pos += Sizes.Rec.Series + 4;
				// read number of children
				K3 = (*(uint32_t*)(hdr->AS.Header+pos-4));

				if (EventN <= hdr->EVENT.N + K3 + 2) {
					EventN = max(max(16,EventN),hdr->EVENT.N+K3+2) * 2;
					hdr->EVENT.TYP = (typeof(hdr->EVENT.TYP)) realloc(hdr->EVENT.TYP,EventN*sizeof(*hdr->EVENT.TYP));
					hdr->EVENT.POS = (typeof(hdr->EVENT.POS)) realloc(hdr->EVENT.POS,EventN*sizeof(*hdr->EVENT.POS));
					if (hdr->EVENT.CHN != NULL && hdr->EVENT.DUR != NULL) {
						hdr->EVENT.CHN = (typeof(hdr->EVENT.CHN)) realloc(hdr->EVENT.CHN,EventN*sizeof(*hdr->EVENT.CHN));
						hdr->EVENT.DUR = (typeof(hdr->EVENT.DUR)) realloc(hdr->EVENT.DUR,EventN*sizeof(*hdr->EVENT.DUR));
					}
				}

				if (!hdr->AS.SegSel[0] && !hdr->AS.SegSel[1] && !hdr->AS.SegSel[2]) {
					// in case of reading the whole file (no sweep selection), include marker for start of series
					FreeTextEvent(hdr, hdr->EVENT.N, SeLabel);
					hdr->EVENT.POS[hdr->EVENT.N] = hdr->SPR;	// within reading the structure, hdr->SPR is used as a intermediate variable counting the number of samples
					hdr->EVENT.N++;
				}

				for (k3=0; k3<K3; k3++)	{
					// read sweep
					hdr->NRec++; 	// increase number of sweeps
					uint32_t SPR = 0, spr = 0;
					double t  = *(double*)(hdr->AS.Header+pos+48);		// time of sweep. TODO: this should be taken into account 

					gdf_time2tm_time_r(heka2gdftime(t),&tm); 
if (VERBOSE_LEVEL>7) fprintf(stdout,"HEKA L3 @%i= %fHz\t%i/%i %i/%i %i/%i t=%.17g %s\n",(int)(pos+StartOfData),hdr->SampleRate,k1,K1,k2,K2,k3,K3,t,asctime(&tm));

					char flagSweepSelected = (hdr->AS.SegSel[0]==0 || k1+1==hdr->AS.SegSel[0])
						              && (hdr->AS.SegSel[1]==0 || k2+1==hdr->AS.SegSel[1])
							      && (hdr->AS.SegSel[2]==0 || k3+1==hdr->AS.SegSel[2]);

					if (flagSweepSelected && hdr->SPR > 0) {
						// marker for start of sweep
						hdr->EVENT.POS[hdr->EVENT.N] = hdr->SPR;	// within reading the structure, hdr->SPR is used as a intermediate variable counting the number of samples
						hdr->EVENT.TYP[hdr->EVENT.N] = 0x7ffe;
						hdr->EVENT.N++;
					}

					pos += Sizes.Rec.Sweep + 4;
					// read number of children
					K4 = (*(uint32_t*)(hdr->AS.Header+pos-4));
					for (k4=0; k4<K4; k4++)	{
						// read trace
						double DigMin, DigMax;
						uint16_t gdftyp  = 0;
						uint32_t ns      = (*(uint32_t*)(hdr->AS.Header+pos+36));
						uint32_t DataPos = (*(uint32_t*)(hdr->AS.Header+pos+40));
						spr              = (*(uint32_t*)(hdr->AS.Header+pos+44));
						double DataScaler= (*(double*)(hdr->AS.Header+pos+72));
						double Toffset   = (*(double*)(hdr->AS.Header+pos+80));		// time offset of 
						uint16_t pdc     = PhysDimCode((char*)(hdr->AS.Header + pos + 96));
						double dT        = (*(double*)(hdr->AS.Header+pos+104));
						double XStart    = (*(double*)(hdr->AS.Header+pos+112));
						uint16_t XUnits  = PhysDimCode((char*)(hdr->AS.Header+pos+120));
						double YRange    = (*(double*)(hdr->AS.Header+pos+128));
						double YOffset   = (*(double*)(hdr->AS.Header+pos+136));
						double Bandwidth   = (*(double*)(hdr->AS.Header+pos+144));
						double PipetteResistance  = (*(double*)(hdr->AS.Header+pos+152));
						double RsValue  = (*(double*)(hdr->AS.Header+pos+192));
						
						uint16_t AdcChan = (*(uint16_t*)(hdr->AS.Header+pos+222));
						double PhysMin   = (*(double*)(hdr->AS.Header+pos+224));
						double PhysMax   = (*(double*)(hdr->AS.Header+pos+232));


						switch (hdr->AS.Header[pos+70]) {
						case 0: gdftyp = 3; 		//int16
							/*
								It seems that the range is 1.024*(2^15-1)/2^15 nA or V
								and symetric around zero. i.e. YOffset is zero
							*/
							DigMax =  ldexp(1.0,15)-1.0;
							DigMin = -DigMax;
							break;
						case 1: gdftyp = 5; 		//int32
							DigMax =  ldexp(1.0, 31) - 1.0;
							DigMin = -DigMax;
							break;
						case 2: gdftyp = 16; 		//float32
							DigMax =  1e9;
							DigMin = -1e9;
							break;
						case 3: gdftyp = 17; 		//float64
							DigMax =  1e9;
							DigMin = -1e9;
							break;
						default:
							DigMax =  NAN;
							DigMin =  NAN;
							biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "Heka/Patchmaster: data type not supported");
						};

						if (SWAP) {
 							AdcChan = bswap_16(AdcChan);
 							ns  = bswap_32(ns);
 							DataPos = bswap_32(DataPos);
							spr = bswap_32(spr);
							// avoid breaking strict-aliasing rules
							union {
								double f64;
								uint64_t u64;
							} c;	
							c.f64 = dT;      c.u64 = bswap_64(c.u64); dT      = c.f64;
							c.f64 = YRange;  c.u64 = bswap_64(c.u64); YRange  = c.f64;
							c.f64 = YOffset; c.u64 = bswap_64(c.u64); YOffset = c.f64;
							c.f64 = PhysMax; c.u64 = bswap_64(c.u64); PhysMax = c.f64;
							c.f64 = PhysMin; c.u64 = bswap_64(c.u64); PhysMin = c.f64;
							c.f64 = Toffset; c.u64 = bswap_64(c.u64); Toffset = c.f64;
 						}

						if (YOffset != 0.0) 
							fprintf(stderr,"!!! WARNING !!!  HEKA: the offset is not zero - "
							"this case is not tested and might result in incorrect scaling of "
							"the data,\n!!! YOU ARE WARNED !!!\n"); 

						double Cal = DataScaler;
						double Off = YOffset;
						double Fs = round(1.0 / ( dT  * PhysDimScale(XUnits) ) );

						if (flagSweepSelected) {
							if (hdr->SampleRate <= 0.0) hdr->SampleRate = Fs;
        	                                        if (fabs(hdr->SampleRate - Fs) > 1e-9) {
								unsigned long DIV1 = 1, DIV2 = 1;
								unsigned long F0 = lcm(round(hdr->SampleRate), Fs);
								DIV1 = F0 / hdr->SampleRate;
								if (DIV1 > 1) {
									hdr->SPR *= DIV1; 
									size_t n = 0;
									while (n < hdr->EVENT.N) 
										hdr->EVENT.POS[n++] *= DIV1;
								}	
								DIV2 = F0 / Fs;
								if (DIV2 > 1) spr *= DIV2; 
								hdr->SampleRate = F0; 
							}

							// samples per sweep
							if (k4==0) SPR = spr;
							else if (SPR != spr) {
								biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "Heka/Patchmaster: number of samples among channels within a single sweep do not match.");
								return;
							}
						}

						char *Label = (char*)hdr->AS.Header+pos+4;
						for (ns=0; ns < hdr->NS; ns++) {
							if (!strcmp(hdr->CHANNEL[ns].Label,Label)) break;
						}

if (VERBOSE_LEVEL>7) fprintf(stdout,"HEKA L4 @%i= #%i,%i, %s %f-%fHz\t%i/%i %i/%i %i/%i %i/%i \n",(int)(pos+StartOfData),ns,AdcChan,Label,hdr->SampleRate,Fs,k1,K1,k2,K2,k3,K3,k4,K4);

						if (ns >= hdr->NS) {
							hdr->NS = ns + 1;
#ifdef WITH_TIMESTAMP
							// allocate memory for an extra time stamp channel, which is define only after the end of the channel loop - see below
							hdr->CHANNEL = (CHANNEL_TYPE*) realloc(hdr->CHANNEL, (hdr->NS+1) * sizeof(CHANNEL_TYPE));  
#else
							hdr->CHANNEL = (CHANNEL_TYPE*) realloc(hdr->CHANNEL, hdr->NS * sizeof(CHANNEL_TYPE));  
#endif
							CHANNEL_TYPE *hc = hdr->CHANNEL + ns;
							strncpy(hc->Label, Label, max(32, MAX_LENGTH_LABEL));
							hc->Label[max(32,MAX_LENGTH_LABEL)] = 0;
							hc->Transducer[0] = 0;
                                                        hc->SPR     = 1;
							hc->PhysDimCode = pdc;
                                                        hc->OnOff   = 1;
							hc->GDFTYP  = gdftyp;
							hc->LeadIdCode = 0;
							hc->DigMin  = DigMin;
							hc->DigMax  = DigMax;

							// TODO: case of non-zero YOffset is not tested //
							hc->PhysMax = hc->DigMax * Cal + Off;
							hc->PhysMin = hc->DigMin * Cal + Off;

							hc->Cal = Cal;
							hc->Off = Off;
							hc->TOffset = Toffset;

#ifndef NDEBUG
							double Cal2 = (hc->PhysMax-hc->PhysMin)/(hc->DigMax-hc->DigMin);
							double Off2 = hc->PhysMin - Cal2*hc->DigMin;
							double Off3 = hc->PhysMax - Cal2*hc->DigMax;

if (VERBOSE_LEVEL>6) fprintf(stdout,"HEKA L4 @%i= #%i,%i, %s %g/%g %g/%g \n",(int)(pos+StartOfData),ns,AdcChan,Label,Cal,Cal2,Off,Off2);

							assert(Cal==Cal2);
							assert(Off==Off2);
							assert(Off==Off3);

#endif


							/* TODO: fix remaining channel header  */
							/* LowPass, HighPass, Notch, Impedance, */
							hc->LowPass = NAN; 
							hc->HighPass = hc->HighPass = (Bandwidth > 0) ? Bandwidth : NAN;
							hc->Notch = NAN;
							hc->Impedance = (RsValue > 0) ? RsValue : NAN;	

							DT = (double*) realloc(DT, hdr->NS*sizeof(double));
							DT[ns] = dT;
						}
						else {
							if (hdr->CHANNEL[ns].PhysMax < PhysMax) hdr->CHANNEL[ns].PhysMax = PhysMax;
							if (hdr->CHANNEL[ns].PhysMin > PhysMin) hdr->CHANNEL[ns].PhysMin = PhysMin;

							if (fabs(hdr->CHANNEL[ns].Cal - Cal) > 1e-9*Cal) flagModifiedTraceHeaders = 1;
							if (fabs(hdr->CHANNEL[ns].Off - Off) > 1e-9*Off) flagModifiedTraceHeaders = 1;
							if (hdr->CHANNEL[ns].GDFTYP < gdftyp) {
								hdr->CHANNEL[ns].GDFTYP = gdftyp;
// OBSOLETE 								flagModifiedTraceHeaders = 1;
							}
						}

						if (YOffset) {
                                                        biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "Heka/Patchmaster: Yoffset is not zero");
						}
						if (hdr->AS.Header[pos+220] != 1) {
                                                        biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "Heka/Patchmaster: ValidYRange not set");
						}
/* OBSOLETE
						if (hdr->CHANNEL[ns].GDFTYP != gdftyp) {
                                                        biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "Heka/Patchmaster: data types do not match");
						}
*/ 
						if ((pdc & 0xFFE0) != (hdr->CHANNEL[ns].PhysDimCode & 0xFFE0)) {
                                                        fprintf(stdout, "Warning: Yunits do not match #%i,%f-%fHz\n",ns, DT[ns],dT);
                                                        biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "Heka/Patchmaster: Yunits do not match");
						}
                                                if ( abs( DT[ns] - dT) > 1e-9 * dT) {
							fprintf(stdout, "Warning sampling intervals do not match #%i,%f-%fHz\n",ns, DT[ns],dT);
                                                        biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "Heka/Patchmaster: sampling intervals do not match");
                                                }

if (VERBOSE_LEVEL>7) fprintf(stdout,"HEKA L4 @%i= #%i,%i, %s %f-%fHz\t%i/%i %i/%i %i/%i %i/%i \n",(int)(pos+StartOfData),ns,AdcChan,Label,hdr->SampleRate,Fs,k1,K1,k2,K2,k3,K3,k4,K4);

						pos += Sizes.Rec.Trace+4;
						// read number of children -- this should be 0 - ALWAYS;
						K5 = (*(uint32_t*)(hdr->AS.Header+pos-4));
						if (K5) {
							biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "Heka/Patchmaster: Level 4 has some children");
						}
					}	// end loop k4

					// if sweep is selected, add number of samples to counter 
					if (flagSweepSelected) hdr->SPR += SPR;
				}		// end loop k3
			}			// end loop k2
		}				// end loop k1
                hdr->EVENT.SampleRate = hdr->SampleRate; 

#ifndef NO_BI
		if (DT) free(DT);
#else
		size_t *BI = (size_t*) DT;      // DT is not used anymore, use space for BI
#endif
                DT = NULL;

#ifdef WITH_TIMESTAMP
		{
			/*
				define time stamp channel, memory is already allocated above
			*/		
			CHANNEL_TYPE *hc = hdr->CHANNEL + hdr->NS; 
			hc->GDFTYP  = 7; 	// corresponds to int64_t, gdf_time
			strcpy(hc->Label,"Timestamp");
			hc->Transducer[0]=0;
			hc->PhysDimCode = 2272; // units: days [d]
                        hc->LeadIdCode = 0;
                        hc->SPR     = 1;
			hc->Cal     = ldexp(1.0, -32); 
			hc->Off     = 0.0; 
			hc->OnOff   = 1; 
			hc->DigMax  = ldexp( 1.0, 61); 	
			hc->DigMin  = 0; 	
			hc->PhysMax = hc->DigMax * hc->Cal;  	
			hc->PhysMin = hc->DigMin * hc->Cal;
			hc->TOffset   = 0.0; 
			hc->Impedance = NAN; 
			hc->HighPass = NAN; 
			hc->LowPass  = NAN; 
			hc->Notch    = NAN; 
			hc->XYZ[0] = 0.0; 
			hc->XYZ[1] = 0.0; 
			hc->XYZ[2] = 0.0; 

			hdr->NS++;
		}
#endif

		hdr->NRec = 1;
		hdr->AS.bpb = 0;
		for (k = 0; k < hdr->NS; k++) {
			CHANNEL_TYPE *hc = hdr->CHANNEL+k;
#ifndef NO_BI
			hc->bi = hdr->AS.bpb;
#else
			BI[k] = hdr->AS.bpb;
#endif
			hc->SPR = hdr->SPR;
			hdr->AS.bpb += hc->SPR * GDFTYP_BITS[hc->GDFTYP]>>3;
		}

		if (hdr->AS.B4C_ERRNUM) {
#ifdef NO_BI
			if (BI) free(BI);
#endif
 			return;
		}

		hdr->ID.Manufacturer.Name = "HEKA/Patchmaster"; 
		void* tmpptr = realloc(hdr->AS.rawdata, hdr->NRec * hdr->AS.bpb);
		if (tmpptr!=NULL) 
			hdr->AS.rawdata = (uint8_t*) tmpptr;
		else {
                        biosigERROR(hdr, B4C_MEMORY_ALLOCATION_FAILED, "memory allocation failed - not enough memory!");
                        return;
		}	
		memset(hdr->AS.rawdata, 0xff, hdr->NRec * hdr->AS.bpb); 	// initialize with NAN's

#ifdef NO_BI
#define _BI (BI[k])
#else
#define _BI (hc->bi)
#endif
		/* initialize with NAN's */
		for (k=0; k<hdr->NS; k++) {
			size_t k1;
			CHANNEL_TYPE *hc = hdr->CHANNEL+k;
			switch (hc->GDFTYP) {
			case 3:
				for (k1=0; k1<hc->SPR; k1++) *(uint16_t*)(hdr->AS.rawdata + _BI + k1 * 2) = 0x8000;
				break;
			case 5:
				for (k1=0; k1<hc->SPR; k1++) *(uint32_t*)(hdr->AS.rawdata + _BI + k1 * 4) = 0x80000000;
				break;
			case 7:
				for (k1=0; k1<hc->SPR; k1++) *(int64_t*)(hdr->AS.rawdata + _BI + k1 * 4) = 0x8000000000000000LL;
				break;
			case 16:
				for (k1=0; k1<hc->SPR; k1++) *(float*)(hdr->AS.rawdata + _BI + k1 * 4) = NAN;
				break;
			case 17:
				for (k1=0; k1<hc->SPR; k1++) *(double*)(hdr->AS.rawdata + _BI + k1 * 8) = NAN;
				break;
			}
		}
#undef _BI

		char *WAVENAME = NULL; 
		if (itx) {
			fprintf(itx, "IGOR\r\nX Silent 1\r\n");
			const char *fn = strrchr(hdr->FileName,'\\');
			if (fn) fn++;
			else fn = strrchr(hdr->FileName,'/');
			if (fn) fn++;
			else fn = hdr->FileName;

			size_t len = strspn(fn,"."); 
			WAVENAME = (char*)malloc(strlen(hdr->FileName)+7);
			if (len) 
				strncpy(WAVENAME, fn, len); 
			else 
				strcpy(WAVENAME, fn); 
		}

if (VERBOSE_LEVEL>7) hdr2ascii(hdr,stdout,4);

		/**************************************************************************
			HEKA: read data blocks
 		 **************************************************************************/
		uint32_t SPR = 0;
		pos = StartOfPulse + Sizes.Rec.Root + 4;
		for (k1=0; k1<K1; k1++)	{
		// read group

if (VERBOSE_LEVEL>7) fprintf(stdout,"HEKA+L1 @%i=\t%i/%i \n",(int)(pos+StartOfData),k1,K1);

			pos += Sizes.Rec.Group+4;
			// read number of children
			K2 = (*(uint32_t*)(hdr->AS.Header+pos-4));

			for (k2=0; k2<K2; k2++)	{
				// read series
				union {
					double   f64;
					uint64_t u64;
				} Delay;
				uint32_t spr = 0;
				char *SeLabel = (char*)(hdr->AS.Header+pos+4);		// max 32 bytes
				Delay.u64 = bswap_64(*(uint64_t*)(hdr->AS.Header+pos+472+176));

if (VERBOSE_LEVEL>7) fprintf(stdout,"HEKA+L2 @%i=%s %f\t%i/%i %i/%i \n",(int)(pos+StartOfData),SeLabel,Delay.f64,k1,K1,k2,K2);

				/* move to reading of data */
				pos += Sizes.Rec.Series+4;
				// read number of children
				K3 = (*(uint32_t*)(hdr->AS.Header+pos-4));
				for (k3=0; k3<K3; k3++)	{
#ifdef WITH_TIMESTAMP
					double t  = *(double*)(hdr->AS.Header+pos+48);		// time of sweep. TODO: this should be taken into account 

if (VERBOSE_LEVEL>7) fprintf(stdout,"   %i %i %i\n",k3,hdr->AS.bpb,SPR);

#ifdef NO_BI
#define _BI (BI[hdr->NS-1])
#else
#define _BI (hdr->CHANNEL[hdr->NS-1].bi)
#endif
					*(int64_t*)(hdr->AS.rawdata + _BI + SPR * 8) = heka2gdftime(t);
#undef _BI
#endif


					// read sweep
					char flagSweepSelected = (hdr->AS.SegSel[0]==0 || k1+1==hdr->AS.SegSel[0])
						              && (hdr->AS.SegSel[1]==0 || k2+1==hdr->AS.SegSel[1])
							      && (hdr->AS.SegSel[2]==0 || k3+1==hdr->AS.SegSel[2]);

if (VERBOSE_LEVEL>7) fprintf(stdout,"HEKA+L3 @%i=\t%i/%i %i/%i %i/%i sel=%i\n",(int)(pos+StartOfData),k1,K1,k2,K2,k3,K3,flagSweepSelected);

					pos += Sizes.Rec.Sweep + 4;
					// read number of children
					K4 = (*(uint32_t*)(hdr->AS.Header+pos-4));
					size_t DIV=1;
					for (k4=0; k4<K4; k4++)	{
						if (!flagSweepSelected) {
							pos += Sizes.Rec.Trace+4;
							continue;
						}

						// read trace
						uint16_t gdftyp  = 0;	
						uint32_t ns      = (*(uint32_t*)(hdr->AS.Header+pos+36));
						uint32_t DataPos = (*(uint32_t*)(hdr->AS.Header+pos+40));
						spr              = (*(uint32_t*)(hdr->AS.Header+pos+44));
						double Toffset   = (*(double*)(hdr->AS.Header+pos+80));		// time offset of 
						// uint16_t pdc     = PhysDimCode((char*)(hdr->AS.Header + pos + 96));
						char *physdim    = (char*)(hdr->AS.Header + pos + 96);
						double dT        = (*(double*)(hdr->AS.Header+pos+104));
						double YRange    = (*(double*)(hdr->AS.Header+pos+128));
						double YOffset   = (*(double*)(hdr->AS.Header+pos+136));
						uint16_t AdcChan = (*(uint16_t*)(hdr->AS.Header+pos+222));
						double PhysMin   = (*(double*)(hdr->AS.Header+pos+224));
						double PhysMax   = (*(double*)(hdr->AS.Header+pos+232));

						switch (hdr->AS.Header[pos+70]) {
						case 0: gdftyp = 3;  break;	// int16
						case 1: gdftyp = 5;  break;	// int32
						case 2: gdftyp = 16; break;	// float32
						case 3: gdftyp = 17; break;	// float64
						default: 
							biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "Heka/Patchmaster unknown data type is used");
						};

						if (SWAP) {
 							AdcChan  = bswap_16(AdcChan);
 							ns       = bswap_32(ns);
 							DataPos  = bswap_32(DataPos);
							spr      = bswap_32(spr);
							// avoid breaking strict-aliasing rules
							union {
								double f64;
								uint64_t u64;
							} c;	
							c.f64 = dT;      c.u64 = bswap_64(c.u64); dT      = c.f64;
							c.f64 = YRange;  c.u64 = bswap_64(c.u64); YRange  = c.f64;
							c.f64 = YOffset; c.u64 = bswap_64(c.u64); YOffset = c.f64;
							c.f64 = PhysMax; c.u64 = bswap_64(c.u64); PhysMax = c.f64;
							c.f64 = PhysMin; c.u64 = bswap_64(c.u64); PhysMin = c.f64;
							c.f64 = Toffset; c.u64 = bswap_64(c.u64); Toffset = c.f64;
 						}
                                                double Fs  = round(1.0 / dT);
						DIV = round(hdr->SampleRate / Fs);

						char *Label = (char*)(hdr->AS.Header+pos+4);
						for (ns=0; ns < hdr->NS; ns++) {
							if (!strcmp(hdr->CHANNEL[ns].Label, Label)) break;
						}
						CHANNEL_TYPE *hc = hdr->CHANNEL+ns;

if (VERBOSE_LEVEL>7) fprintf(stdout,"HEKA+L4 @%i= #%i,%i,%i/%i %s\t%i/%i %i/%i %i/%i %i/%i DIV=%i,%i,%i\n",(int)(pos+StartOfData),ns,AdcChan,spr,SPR,Label,k1,K1,k2,K2,k3,K3,k4,K4,(int)DIV,gdftyp,hc->GDFTYP);

#ifdef NO_BI
#define _BI (BI[ns])
#else
#define _BI (hc->bi)
#endif
						if (itx) {
							uint32_t k5;
							double Cal = hdr->CHANNEL[ns].Cal;
							double Off = hdr->CHANNEL[ns].Off;
							fprintf(itx, "\r\nWAVES %s_%i_%i_%i_%i\r\nBEGIN\r\n", WAVENAME,k1+1,k2+1,k3+1,k4+1);
							switch (hc->GDFTYP) {
							case 3:  
								for (k5 = 0; k5 < spr; ++k5)
									fprintf(itx,"% e\n", (double)*(int16_t*)(hdr->AS.Header + DataPos + k5 * 2) * Cal + Off);
								break;
							case 5:
								for (k5 = 0; k5 < spr; ++k5)
									fprintf(itx,"% e\n", (double)*(int32_t*)(hdr->AS.Header + DataPos + k5 * 4) * Cal + Off);
								break;
							case 16: 
								for (k5 = 0; k5 < spr; ++k5)
									fprintf(itx,"% e\n", (double)*(float*)(hdr->AS.Header + DataPos + k5 * 4) * Cal + Off);
								break;
							case 17: 
								for (k5 = 0; k5 < spr; ++k5)
									fprintf(itx,"% e\n", *(double*)(hdr->AS.Header + DataPos + k5 * 8) * Cal + Off);
								break;
							}
							fprintf(itx, "END\r\nX SetScale/P x, %g, %g, \"s\", %s_%i_%i_%i_%i\r\n", Toffset, dT, WAVENAME, k1+1,k2+1,k3+1,k4+1);
							fprintf(itx, "X SetScale y,0,0,\"%s\", %s_%i_%i_%i_%i\n", physdim, WAVENAME, k1+1,k2+1,k3+1,k4+1);
						}	

						// no need to check byte order because File.Endian is set and endian conversion is done in sread
						if ((DIV==1) && (gdftyp == hc->GDFTYP))	{
							uint16_t sz = GDFTYP_BITS[hc->GDFTYP]>>3;	
							memcpy(hdr->AS.rawdata + _BI + SPR * sz, hdr->AS.Header + DataPos, spr * sz);
						}
						else if (1) {
							uint32_t k5,k6;
							switch (gdftyp) {
							case 3: 
								switch (hc->GDFTYP) {
								case 3: 
									for (k5 = 0; k5 < spr; ++k5) {
										int16_t ival = *(int16_t*)(hdr->AS.Header + DataPos + k5 * 2);
										for (k6 = 0; k6 < DIV; ++k6) 
											*(int16_t*)(hdr->AS.rawdata + _BI + (SPR + k5*DIV + k6) * 2) = ival;
									}
									break;
								case 5: 
									for (k5 = 0; k5 < spr; ++k5) {
										int16_t ival = *(int16_t*)(hdr->AS.Header + DataPos + k5 * 2);
										for (k6 = 0; k6 < DIV; ++k6) 
											*(int32_t*)(hdr->AS.rawdata + _BI + (SPR + k5*DIV + k6) * 4) = (int32_t)ival;
									}
									break;
								case 16: 
									for (k5 = 0; k5 < spr; ++k5) {
										int16_t ival = *(int16_t*)(hdr->AS.Header + DataPos + k5 * 2);
										for (k6 = 0; k6 < DIV; ++k6) 
											*(float*)(hdr->AS.rawdata + _BI + (SPR + k5*DIV + k6) * 4) = (float)ival;
									}
									break;
								case 17: 
									for (k5 = 0; k5 < spr; ++k5) {
										int16_t ival = *(int16_t*)(hdr->AS.Header + DataPos + k5 * 2);
										for (k6 = 0; k6 < DIV; ++k6) 
											*(double*)(hdr->AS.rawdata + _BI + (SPR + k5*DIV + k6) * 8) = (double)ival;
									}
									break;
								}
								break;
							case 5:
								switch (hc->GDFTYP) {
								case 5: 
									for (k5 = 0; k5 < spr; ++k5) {
										int32_t ival = *(int32_t*)(hdr->AS.Header + DataPos + k5 * 4);
										for (k6 = 0; k6 < DIV; ++k6) 
											*(int32_t*)(hdr->AS.rawdata + _BI + (SPR + k5*DIV + k6) * 4) = ival;
									}
									break;
								case 16: 
									for (k5 = 0; k5 < spr; ++k5) {
										int32_t ival = *(int32_t*)(hdr->AS.Header + DataPos + k5 * 4);
										for (k6 = 0; k6 < DIV; ++k6) 
											*(float*)(hdr->AS.rawdata + _BI + (SPR + k5*DIV + k6) * 4) = (float)ival;
									}
									break;
								case 17: 
									for (k5 = 0; k5 < spr; ++k5) {
										int32_t ival = *(int32_t*)(hdr->AS.Header + DataPos + k5 * 4);
										for (k6 = 0; k6 < DIV; ++k6) 
											*(double*)(hdr->AS.rawdata + _BI + (SPR + k5*DIV + k6) * 8) = (double)ival;
									}
									break;
								}
								break;
							case 16:
								switch (hc->GDFTYP) {
								case 16: 
									for (k5 = 0; k5 < spr; ++k5) {
										float ival = *(float*)(hdr->AS.Header + DataPos + k5 * 4);
										for (k6 = 0; k6 < DIV; ++k6) 
											*(float*)(hdr->AS.rawdata + _BI + (SPR + k5*DIV + k6) * 4) = ival;
									}
									break;
								case 17: 
									for (k5 = 0; k5 < spr; ++k5) {
										float ival = *(float*)(hdr->AS.Header + DataPos + k5 * 4);
										for (k6 = 0; k6 < DIV; ++k6) 
											*(double*)(hdr->AS.rawdata + _BI + (SPR + k5*DIV + k6) * 8) = (double)ival;
									}
									break;
								}
								break;
							case 17:
								switch (hc->GDFTYP) {
								case 17: 
									for (k5 = 0; k5 < spr; ++k5) {
										double ival = *(double*)(hdr->AS.Header + DataPos + k5 * 8);
										for (k6 = 0; k6 < DIV; ++k6) 
											*(double*)(hdr->AS.rawdata + _BI + (SPR + k5*DIV + k6) * 8) = ival;
									}
									break;
								}
								break;
							}
						}
#undef _BI
						pos += Sizes.Rec.Trace+4;

					}
					if (flagSweepSelected) SPR += spr * DIV;
				}
			}
		}
#ifdef NO_BI
		if (BI) free(BI);
#endif
		hdr->AS.first  = 0;
		hdr->AS.length = hdr->NRec;
		free(hdr->AS.Header);
		hdr->AS.Header = NULL; 

		if (VERBOSE_LEVEL>7) fprintf(stdout,"End of SOPEN_HEKA\n");
	}

	else if (hdr->TYPE==HEKA) {
		biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "Heka/Patchmaster format has unsupported version number");
        }
}

int sopen_zzztest(HDRTYPE* hdr) {

	if (hdr->TYPE==ITX) {

		fprintf(stdout,"Warning: support for ITX is very experimental\n");

/*
		while (~ifeof(hdr)) {
			hdr->Header = realloc(hdr->Header,count*2+1);
			count += ifread(hdr->Header+count,1,count,hdr);
		}
		hdr->Header[count]=0;
*/

    		char line[ITX_MAXLINELENGTH+1];
    		char flag = 0;
		double *data = NULL;

	    	size_t ns=0, NS=0, spr = 0, SPR = 0;
		size_t ngroup=0, nseries=0, nsweep=0;
		double deltaT = -INFINITY;
		hdr->SPR = 0;

		ifseek(hdr, 0, SEEK_SET);
		while (!ifeof(hdr)) {
		    	ifgets(line, ITX_MAXLINELENGTH, hdr);
			if (!strlen(line))
				;
			else if (!strncmp(line,"BEGIN",5)) {
				flag = 1;
				spr = 0;
                        }
		    	else if (!strncmp(line,"END",3)) {
				flag = 0;
				hdr->CHANNEL[ns].SPR += spr;
		    	}
		    	else if (!strncmp(line,"X SetScale/P x,",15)) {
				strtok(line,",");
				strtok(NULL,",");
		    		double dur = atof(strtok(NULL,","));
		    		char *p = strchr(line,'"');
				if (p != NULL) {
					p++;
		    			char *p2 = strchr(p,'"');
		    			if (p2!=NULL) *p2=0;
					dur *= PhysDimScale(PhysDimCode(p));
				}
				if (deltaT < 0.0) {
					deltaT = dur;
		    			hdr->SampleRate = 1.0 / dur;
				}
				else if (deltaT != dur) {
					biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "different sampling rates not supported for ITX format");
				}
			}
			else if (!strncmp(line,"X SetScale y,",13)) {
				char *p = strchr(line,'"');
				if (p!=NULL) {
					p++;
					char *p2 = strchr(p,'"');
					if (p2!=NULL) *p2=0;
					hdr->CHANNEL[ns].PhysDimCode = PhysDimCode(p);
				}
				ns++;
			}
			else if (!strncmp(line,"WAVES",5)) {
	    			// get properties of current trace, allocate new channel when needed
				IgorChanLabel(line+6, hdr, &ngroup, &nseries, &nsweep, &ns);
				CHANNEL_TYPE *hc = hdr->CHANNEL+ns;

				hc->OnOff    = 1;
				hc->SPR      = 0;
				hc->GDFTYP   = 17;
				hc->DigMax   = (double)(int16_t)(0x7fff);
				hc->DigMin   = (double)(int16_t)(0x8000);

				hc->Cal      = 1.0;
				hc->Off      = 0.0;
				hc->Transducer[0] = '\0';
				hc->LowPass  = NAN;
				hc->HighPass = NAN;
				hc->PhysMax  = hc->Cal * hc->DigMax;
				hc->PhysMin  = hc->Cal * hc->DigMin;
	    		}
	    		else if (flag)
	    			spr++;
	    	}
		hdr->NS = ns;

		hdr->SPR = 0;
		for (ns=0; ns<NS; ns++) {
#ifndef NO_BI
			hdr->CHANNEL[ns].bi  = SPR*sizeof(double);
#endif
			SPR += hdr->CHANNEL[ns].SPR;
			if (hdr->SPR < hdr->CHANNEL[ns].SPR)
				hdr->SPR = hdr->CHANNEL[ns].SPR;
			hdr->CHANNEL[ns].SPR = 0;	// reset
		}

		data = (double*)realloc(hdr->AS.rawdata, SPR*sizeof(double));
		hdr->FILE.LittleEndian = (__BYTE_ORDER == __LITTLE_ENDIAN);   // no swapping
		hdr->AS.rawdata = (uint8_t*) data;

    		ifseek(hdr, 0, SEEK_SET);
    		while (!ifeof(hdr)) {
	    		ifgets(line, ITX_MAXLINELENGTH, hdr);
	    		if (!strncmp(line,"BEGIN",5)) {
	    			flag = 1;
	    			spr = 0;
	    		}
	    		else if (!strncmp(line,"END",3)) {
	    			flag = 0;
	    			hdr->CHANNEL[ns].SPR += spr;
	    		}
	    		else if (!strncmp(line,"WAVES",5)) {
	    			// get properties of current trace
				IgorChanLabel(line+6, hdr, &ngroup, &nseries, &nsweep, &ns);
			}
	    		else if (flag) {
#ifndef NO_BI
	    			data[hdr->CHANNEL[ns].SPR + hdr->CHANNEL[ns].bi + spr++] = atof(line);
#else
#error .bi not defined yet
#endif
	    		}
	    	}
		hdr->NRec = 1;
		hdr->AS.first  = 0;
		hdr->AS.length = 1;
	}

	return(0);
};

int sopen_unipro_read(HDRTYPE* hdr) {
		hdr->FILE.LittleEndian = (__BYTE_ORDER == __LITTLE_ENDIAN);
		char *Header1 = (char*)hdr->AS.Header;
		struct tm t0;
		char tmp[5];
		memset(tmp,0,5);
		strncpy(tmp,Header1+0x9c,2);
		t0.tm_mon = atoi(tmp)-1;
		strncpy(tmp,Header1+0x9e,2);
		t0.tm_mday = atoi(tmp);
		strncpy(tmp,Header1+0xa1,2);
		t0.tm_hour = atoi(tmp);
		strncpy(tmp,Header1+0xa3,2);
		t0.tm_min = atoi(tmp);
		strncpy(tmp,Header1+0xa5,2);
		t0.tm_sec = atoi(tmp);
		strncpy(tmp,Header1+0x98,4);
		t0.tm_year = atoi(tmp)-1900;
		hdr->T0 = tm_time2gdf_time(&t0);

		memset(tmp,0,5);
		strncpy(tmp,Header1+0x85,2);
		t0.tm_mday = atoi(tmp);
		strncpy(tmp,Header1+0x83,2);
		t0.tm_mon = atoi(tmp)-1;
		strncpy(tmp,Header1+0x7f,4);
		t0.tm_year = atoi(tmp)-1900;
		hdr->Patient.Birthday = tm_time2gdf_time(&t0);

		// filesize = leu32p(hdr->AS.Header + 0x24);
		biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "UNIPRO not supported");
		return(0);
}


#ifdef WITH_DICOM
int sopen_dicom_read(HDRTYPE* hdr) {

		fprintf(stdout,"home-made parser is used to read dicom files.\n");

		char FLAG_implicite_VR = 0;
		int EndOfGroup2=-1;

		if (hdr->HeadLen<132) {
			hdr->AS.Header = (uint8_t*)realloc(hdr->AS.Header, 132);
		    	hdr->HeadLen += ifread(hdr->AS.Header+hdr->HeadLen, 1, 132-hdr->HeadLen, hdr);
		}
		size_t count = hdr->HeadLen;
		size_t pos = 128;
		while (!hdr->AS.Header[pos] && (pos<128)) pos++;
		if ((pos==128) && !memcmp(hdr->AS.Header+128,"DICM",4)) {
//			FLAG_implicite_VR = 0;
			pos = 132;
		}
		else
			pos = 0;

		size_t bufsiz = 16384;
		while (!ifeof(hdr)) {
			hdr->AS.Header = (uint8_t*)realloc(hdr->AS.Header, count+bufsiz+1);
		    	count += ifread(hdr->AS.Header+count, 1, bufsiz, hdr);
		    	bufsiz *= 2;
		}
	    	ifclose(hdr);
	    	hdr->AS.Header[count] = 0;

		uint16_t nextTag[2];

		struct tm T0;
		char flag_t0=0;
		char flag_ignored;
		uint32_t Tag;
		uint32_t Len;
		nextTag[0] = *(uint16_t*)(hdr->AS.Header+pos);
		nextTag[1] = *(uint16_t*)(hdr->AS.Header+pos+2);
		while (pos < count) {

			if ((__BYTE_ORDER == __BIG_ENDIAN) ^ !hdr->FILE.LittleEndian) {
				// swapping required
				Tag  = (((uint32_t)bswap_16(nextTag[0])) << 16) + bswap_16(nextTag[1]);
				pos += 4;
				if (FLAG_implicite_VR) {
					Len = bswap_32(*(uint32_t*)(hdr->AS.Header+pos));
					pos += 4;
				}
				else {
					// explicite_VR
					if (pos+4 > count) break;

					if (memcmp(hdr->AS.Header+pos,"OB",2)
					 && memcmp(hdr->AS.Header+pos,"OW",2)
					 && memcmp(hdr->AS.Header+pos,"OF",2)
					 && memcmp(hdr->AS.Header+pos,"SQ",2)
					 && memcmp(hdr->AS.Header+pos,"UT",2)
					 && memcmp(hdr->AS.Header+pos,"UN",2) ) {
						Len = bswap_16(*(uint16_t*)(hdr->AS.Header+pos+2));
						pos += 4;
					}
					else {
						Len = bswap_32(*(uint32_t*)(hdr->AS.Header+pos+4));
						pos += 8;
					}
				}
			}
			else {
				// no swapping
				Tag  = (((uint32_t)nextTag[0]) << 16) + nextTag[1];
				pos += 4;
				if (FLAG_implicite_VR) {
					Len = *(uint32_t*)(hdr->AS.Header+pos);
					pos += 4;
				}
				else {
					// explicite_VR
					if (pos+4 > count) break;

					if (memcmp(hdr->AS.Header+pos,"OB",2)
					 && memcmp(hdr->AS.Header+pos,"OW",2)
					 && memcmp(hdr->AS.Header+pos,"OF",2)
					 && memcmp(hdr->AS.Header+pos,"SQ",2)
					 && memcmp(hdr->AS.Header+pos,"UT",2)
					 && memcmp(hdr->AS.Header+pos,"UN",2) ) {
						Len = *(uint16_t*)(hdr->AS.Header+pos+2);
						pos += 4;
					}
					else {
						Len = *(uint32_t*)(hdr->AS.Header+pos+4);
						pos += 8;
					}
				}
			}

			/*
			    backup next tag, this allows setting of terminating 0
			*/
			if (pos+Len < count) {
				nextTag[0] = *(uint16_t*)(hdr->AS.Header+pos+Len);
				nextTag[1] = *(uint16_t*)(hdr->AS.Header+pos+Len+2);
				hdr->AS.Header[pos+Len] = 0;
	    		}


			flag_ignored = 0;
			if (VERBOSE_LEVEL>8)
				fprintf(stdout,"        %6x:   (%04x,%04x) %8d\t%s\n",pos,Tag>>16,Tag&0x0ffff,Len,(char*)hdr->AS.Header+pos);

			switch (Tag) {


			/* elements of group 0x0002 use always
				Explicite VR little Endian encoding
			*/
			case 0x00020000: {
				int c = 0;
				if (!memcmp(hdr->AS.Header+pos-8,"UL",2))
					c = leu32p(hdr->AS.Header+pos);
				else if (!memcmp(hdr->AS.Header+pos-8,"SL",2))
					c = lei32p(hdr->AS.Header+pos);
				else if (!memcmp(hdr->AS.Header+pos-8,"US",2))
					c = leu16p(hdr->AS.Header+pos);
				else if (!memcmp(hdr->AS.Header+pos-8,"SS",2))
					c = lei16p(hdr->AS.Header+pos);
				else  {
					biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "DICOM (0002,0000): unsupported");
				}
				EndOfGroup2 = c + pos;
				break;
				}
			case 0x00020001:
				break;

			case 0x00020002: {
				hdr->NS = 1;
				char *t = (char*)hdr->AS.Header+pos;
				while (isspace(*t)) t++;	// deblank
				char *ct[] = {	"1.2.840.10008.5.1.4.1.1.9.1.1",
						"1.2.840.10008.5.1.4.1.1.9.1.2",
						"1.2.840.10008.5.1.4.1.1.9.1.3",
						"1.2.840.10008.5.1.4.1.1.9.2.1",
						"1.2.840.10008.5.1.4.1.1.9.3.1",
						"1.2.840.10008.5.1.4.1.1.9.4.1"
						};
				if (!strcmp(t,*ct)) hdr->NS = 12;
				break;
				}

			case 0x00020003:
				break;

			case 0x00020010: {
				char *t = (char*)hdr->AS.Header+pos;
				while (isspace(*t)) t++;	// deblank
				char *ct[] = {	"1.2.840.10008.1.2",
						"1.2.840.10008.1.2.1",
						"1.2.840.10008.1.2.1.99",
						"1.2.840.10008.1.2.2"
						};

				if      (!strcmp(t,*ct))   FLAG_implicite_VR = 1;
				else if (!strcmp(t,*ct+1)) FLAG_implicite_VR = 0;
				else if (!strcmp(t,*ct+3)) FLAG_implicite_VR = 1;
				break;
				}

			case 0x00080020:  // StudyDate
			case 0x00080023:  // ContentDate
				{
				hdr->AS.Header[pos+8]=0;
				T0.tm_mday = atoi((char*)hdr->AS.Header+pos+6);
				hdr->AS.Header[pos+6]=0;
				T0.tm_mon = atoi((char*)hdr->AS.Header+pos+4)-1;
				hdr->AS.Header[pos+4]=0;
				T0.tm_year = atoi((char*)hdr->AS.Header+pos)-1900;
				flag_t0 |= 1;
				break;
				}
			case 0x0008002a:  // AcquisitionDateTime
				{
				struct tm t0;
				hdr->AS.Header[pos+14]=0;
				t0.tm_sec = atoi((char*)hdr->AS.Header+pos+12);
				hdr->AS.Header[pos+12]=0;
				t0.tm_min = atoi((char*)hdr->AS.Header+pos+10);
				hdr->AS.Header[pos+10]=0;
				t0.tm_hour = atoi((char*)hdr->AS.Header+pos+8);
				hdr->AS.Header[pos+8]=0;
				t0.tm_mday = atoi((char*)hdr->AS.Header+pos+6);
				hdr->AS.Header[pos+6]=0;
				t0.tm_mon = atoi((char*)hdr->AS.Header+pos+4)-1;
				hdr->AS.Header[pos+4]=0;
				t0.tm_year = atoi((char*)hdr->AS.Header+pos)-1900;

				hdr->T0 = tm_time2gdf_time(&t0);
				break;
				}
			case 0x00080030:  // StudyTime
			case 0x00080033:  // ContentTime
				{
				hdr->AS.Header[pos+6]=0;
				T0.tm_sec = atoi((char*)hdr->AS.Header+pos+4);
				hdr->AS.Header[pos+4]=0;
				T0.tm_min = atoi((char*)hdr->AS.Header+pos+2);
				hdr->AS.Header[pos+2]=0;
				T0.tm_hour = atoi((char*)hdr->AS.Header+pos);
				flag_t0 |= 2;
				break;
				}
			case 0x00080070:  // Manufacturer
				{
				strncpy(hdr->ID.Manufacturer._field,(char*)hdr->AS.Header+pos,MAX_LENGTH_MANUF);
				hdr->ID.Manufacturer.Name = hdr->ID.Manufacturer._field;
				break;
				}
			case 0x00081050:  // Performing Physician
				{
				strncpy(hdr->ID.Technician,(char*)hdr->AS.Header+pos,MAX_LENGTH_TECHNICIAN);
				break;
				}
			case 0x00081090: // Manufacturer Model
				{
				const size_t nn = strlen(hdr->ID.Manufacturer.Name)+1;
				strncpy(hdr->ID.Manufacturer._field+nn,(char*)hdr->AS.Header+pos,MAX_LENGTH_MANUF-nn-1);
				hdr->ID.Manufacturer.Model = hdr->ID.Manufacturer._field+nn;
				break;
				}

			case 0x00100010: // Name
				if (!hdr->FLAG.ANONYMOUS) {
					strncpy(hdr->Patient.Name,(char*)hdr->AS.Header+pos,MAX_LENGTH_NAME);
					hdr->Patient.Name[MAX_LENGTH_NAME]=0;
				}
				break;
			case 0x00100020: // ID
				strncpy(hdr->Patient.Id,(char*)hdr->AS.Header+pos,MAX_LENGTH_NAME);
				hdr->Patient.Id[MAX_LENGTH_PID]=0;
				break;

			case 0x00100030: // Birthday
				{
				struct tm t0;
				t0.tm_sec = 0;
				t0.tm_min = 0;
				t0.tm_hour = 12;

				hdr->AS.Header[pos+8]=0;
				t0.tm_mday = atoi((char*)hdr->AS.Header+pos+6);
				hdr->AS.Header[pos+6]=0;
				t0.tm_mon = atoi((char*)hdr->AS.Header+pos+4)-1;
				hdr->AS.Header[pos+4]=0;
				t0.tm_year = atoi((char*)hdr->AS.Header+pos)-1900;

				hdr->Patient.Birthday = tm_time2gdf_time(&t0);
				break;
				}
			case 0x00100040:
				hdr->Patient.Sex = (toupper(hdr->AS.Header[pos])=='M') + 2*(toupper(hdr->AS.Header[pos])=='F');
				break;

			case 0x00101010: //Age
				break;
			case 0x00101020:
				hdr->Patient.Height = (uint8_t)(atof((char*)hdr->AS.Header+pos)*100.0);
				break;
			case 0x00101030:
				hdr->Patient.Weight = (uint8_t)atoi((char*)hdr->AS.Header+pos);
				break;

			default:
				flag_ignored = 1;
				if (VERBOSE_LEVEL<7)
					fprintf(stdout,"ignored %6x:   (%04x,%04x) %8d\t%s\n",pos,Tag>>16,Tag&0x0ffff,Len,(char*)hdr->AS.Header+pos);

			}

			if (VERBOSE_LEVEL>6) {
			if (!FLAG_implicite_VR || (Tag < 0x00030000))
				fprintf(stdout,"%s %6x:   (%04x,%04x) %8d %c%c \t%s\n",(flag_ignored?"ignored":"       "),pos,Tag>>16,Tag&0x0ffff,Len,hdr->AS.Header[pos-8],hdr->AS.Header[pos-7],(char*)hdr->AS.Header+pos);
			else
				fprintf(stdout,"%s %6x:   (%04x,%04x) %8d\t%s\n",(flag_ignored?"ignored":"       "),pos,Tag>>16,Tag&0x0ffff,Len,(char*)hdr->AS.Header+pos);
			}
			pos += Len + (Len & 0x01 ? 1 : 0); // even number of bytes

		}
		if (flag_t0 == 3) hdr->T0 = tm_time2gdf_time(&T0);
	return(0);
}
#endif

#ifdef WITH_PDP
#include "../NONFREE/sopen_pdp_read.c"
#endif

#ifdef WITH_TRC
#include "../NONFREE/sopen_trc_read.c"
#endif

#ifdef __cplusplus
}
#endif

