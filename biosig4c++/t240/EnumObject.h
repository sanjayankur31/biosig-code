/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "FEF-IntermediateDraft"
 * 	found in "../annexb-snacc-122001.asn1"
 */

#ifndef	_EnumObject_H_
#define	_EnumObject_H_


#include <asn_application.h>

/* Including external dependencies */
#include "MetricsCode.h"
#include <ANY.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* EnumObject */
typedef struct EnumObject {
	MetricsCode_t	 enum_obj_id;
	ANY_t	*enumadditionaldata	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} EnumObject_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_EnumObject;

#ifdef __cplusplus
}
#endif

#endif	/* _EnumObject_H_ */
