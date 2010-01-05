/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "FEF-IntermediateDraft"
 * 	found in "../annexb-snacc-122001.asn1"
 */

#include <asn_internal.h>

#include "MeasuredDataSection.h"

static asn_TYPE_member_t asn_MBR_realtimesas_2[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_RealTimeSampleArrayMeasuredDataSection,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		""
		},
};
static ber_tlv_tag_t asn_DEF_realtimesas_tags_2[] = {
	(ASN_TAG_CLASS_APPLICATION | (7025 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_realtimesas_specs_2 = {
	sizeof(struct MeasuredDataSection__realtimesas),
	offsetof(struct MeasuredDataSection__realtimesas, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_realtimesas_2 = {
	"realtimesas",
	"realtimesas",
	SEQUENCE_OF_free,
	SEQUENCE_OF_print,
	SEQUENCE_OF_constraint,
	SEQUENCE_OF_decode_ber,
	SEQUENCE_OF_encode_der,
	SEQUENCE_OF_decode_xer,
	SEQUENCE_OF_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_realtimesas_tags_2,
	sizeof(asn_DEF_realtimesas_tags_2)
		/sizeof(asn_DEF_realtimesas_tags_2[0]) - 1, /* 1 */
	asn_DEF_realtimesas_tags_2,	/* Same as above */
	sizeof(asn_DEF_realtimesas_tags_2)
		/sizeof(asn_DEF_realtimesas_tags_2[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_realtimesas_2,
	1,	/* Single element */
	&asn_SPC_realtimesas_specs_2	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_timesas_4[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_TimeSampleArrayMeasuredDataSection,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		""
		},
};
static ber_tlv_tag_t asn_DEF_timesas_tags_4[] = {
	(ASN_TAG_CLASS_APPLICATION | (7026 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_timesas_specs_4 = {
	sizeof(struct MeasuredDataSection__timesas),
	offsetof(struct MeasuredDataSection__timesas, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_timesas_4 = {
	"timesas",
	"timesas",
	SEQUENCE_OF_free,
	SEQUENCE_OF_print,
	SEQUENCE_OF_constraint,
	SEQUENCE_OF_decode_ber,
	SEQUENCE_OF_encode_der,
	SEQUENCE_OF_decode_xer,
	SEQUENCE_OF_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_timesas_tags_4,
	sizeof(asn_DEF_timesas_tags_4)
		/sizeof(asn_DEF_timesas_tags_4[0]) - 1, /* 1 */
	asn_DEF_timesas_tags_4,	/* Same as above */
	sizeof(asn_DEF_timesas_tags_4)
		/sizeof(asn_DEF_timesas_tags_4[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_timesas_4,
	1,	/* Single element */
	&asn_SPC_timesas_specs_4	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_distributionsas_6[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_DistributionSampleArrayMeasuredDataSection,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		""
		},
};
static ber_tlv_tag_t asn_DEF_distributionsas_tags_6[] = {
	(ASN_TAG_CLASS_APPLICATION | (7027 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_distributionsas_specs_6 = {
	sizeof(struct MeasuredDataSection__distributionsas),
	offsetof(struct MeasuredDataSection__distributionsas, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_distributionsas_6 = {
	"distributionsas",
	"distributionsas",
	SEQUENCE_OF_free,
	SEQUENCE_OF_print,
	SEQUENCE_OF_constraint,
	SEQUENCE_OF_decode_ber,
	SEQUENCE_OF_encode_der,
	SEQUENCE_OF_decode_xer,
	SEQUENCE_OF_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_distributionsas_tags_6,
	sizeof(asn_DEF_distributionsas_tags_6)
		/sizeof(asn_DEF_distributionsas_tags_6[0]) - 1, /* 1 */
	asn_DEF_distributionsas_tags_6,	/* Same as above */
	sizeof(asn_DEF_distributionsas_tags_6)
		/sizeof(asn_DEF_distributionsas_tags_6[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_distributionsas_6,
	1,	/* Single element */
	&asn_SPC_distributionsas_specs_6	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_numerics_8[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_NumericMeasuredDataSection,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		""
		},
};
static ber_tlv_tag_t asn_DEF_numerics_tags_8[] = {
	(ASN_TAG_CLASS_APPLICATION | (7028 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_numerics_specs_8 = {
	sizeof(struct MeasuredDataSection__numerics),
	offsetof(struct MeasuredDataSection__numerics, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_numerics_8 = {
	"numerics",
	"numerics",
	SEQUENCE_OF_free,
	SEQUENCE_OF_print,
	SEQUENCE_OF_constraint,
	SEQUENCE_OF_decode_ber,
	SEQUENCE_OF_encode_der,
	SEQUENCE_OF_decode_xer,
	SEQUENCE_OF_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_numerics_tags_8,
	sizeof(asn_DEF_numerics_tags_8)
		/sizeof(asn_DEF_numerics_tags_8[0]) - 1, /* 1 */
	asn_DEF_numerics_tags_8,	/* Same as above */
	sizeof(asn_DEF_numerics_tags_8)
		/sizeof(asn_DEF_numerics_tags_8[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_numerics_8,
	1,	/* Single element */
	&asn_SPC_numerics_specs_8	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_enumerations_10[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_EnumerationMeasuredDataSection,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		""
		},
};
static ber_tlv_tag_t asn_DEF_enumerations_tags_10[] = {
	(ASN_TAG_CLASS_APPLICATION | (7029 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_enumerations_specs_10 = {
	sizeof(struct MeasuredDataSection__enumerations),
	offsetof(struct MeasuredDataSection__enumerations, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_enumerations_10 = {
	"enumerations",
	"enumerations",
	SEQUENCE_OF_free,
	SEQUENCE_OF_print,
	SEQUENCE_OF_constraint,
	SEQUENCE_OF_decode_ber,
	SEQUENCE_OF_encode_der,
	SEQUENCE_OF_decode_xer,
	SEQUENCE_OF_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_enumerations_tags_10,
	sizeof(asn_DEF_enumerations_tags_10)
		/sizeof(asn_DEF_enumerations_tags_10[0]) - 1, /* 1 */
	asn_DEF_enumerations_tags_10,	/* Same as above */
	sizeof(asn_DEF_enumerations_tags_10)
		/sizeof(asn_DEF_enumerations_tags_10[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_enumerations_10,
	1,	/* Single element */
	&asn_SPC_enumerations_specs_10	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_alerts_12[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_AlertSection,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		""
		},
};
static ber_tlv_tag_t asn_DEF_alerts_tags_12[] = {
	(ASN_TAG_CLASS_APPLICATION | (7007 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_alerts_specs_12 = {
	sizeof(struct MeasuredDataSection__alerts),
	offsetof(struct MeasuredDataSection__alerts, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_alerts_12 = {
	"alerts",
	"alerts",
	SEQUENCE_OF_free,
	SEQUENCE_OF_print,
	SEQUENCE_OF_constraint,
	SEQUENCE_OF_decode_ber,
	SEQUENCE_OF_encode_der,
	SEQUENCE_OF_decode_xer,
	SEQUENCE_OF_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_alerts_tags_12,
	sizeof(asn_DEF_alerts_tags_12)
		/sizeof(asn_DEF_alerts_tags_12[0]) - 1, /* 1 */
	asn_DEF_alerts_tags_12,	/* Same as above */
	sizeof(asn_DEF_alerts_tags_12)
		/sizeof(asn_DEF_alerts_tags_12[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_alerts_12,
	1,	/* Single element */
	&asn_SPC_alerts_specs_12	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_attributechanges_14[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_DynamicAttributeChangeSection,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		""
		},
};
static ber_tlv_tag_t asn_DEF_attributechanges_tags_14[] = {
	(ASN_TAG_CLASS_APPLICATION | (7030 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_attributechanges_specs_14 = {
	sizeof(struct MeasuredDataSection__attributechanges),
	offsetof(struct MeasuredDataSection__attributechanges, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_attributechanges_14 = {
	"attributechanges",
	"attributechanges",
	SEQUENCE_OF_free,
	SEQUENCE_OF_print,
	SEQUENCE_OF_constraint,
	SEQUENCE_OF_decode_ber,
	SEQUENCE_OF_encode_der,
	SEQUENCE_OF_decode_xer,
	SEQUENCE_OF_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_attributechanges_tags_14,
	sizeof(asn_DEF_attributechanges_tags_14)
		/sizeof(asn_DEF_attributechanges_tags_14[0]) - 1, /* 1 */
	asn_DEF_attributechanges_tags_14,	/* Same as above */
	sizeof(asn_DEF_attributechanges_tags_14)
		/sizeof(asn_DEF_attributechanges_tags_14[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_attributechanges_14,
	1,	/* Single element */
	&asn_SPC_attributechanges_specs_14	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_MeasuredDataSection_1[] = {
	{ ATF_POINTER, 7, offsetof(struct MeasuredDataSection, realtimesas),
		(ASN_TAG_CLASS_APPLICATION | (7025 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_realtimesas_2,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"realtimesas"
		},
	{ ATF_POINTER, 6, offsetof(struct MeasuredDataSection, timesas),
		(ASN_TAG_CLASS_APPLICATION | (7026 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_timesas_4,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"timesas"
		},
	{ ATF_POINTER, 5, offsetof(struct MeasuredDataSection, distributionsas),
		(ASN_TAG_CLASS_APPLICATION | (7027 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_distributionsas_6,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"distributionsas"
		},
	{ ATF_POINTER, 4, offsetof(struct MeasuredDataSection, numerics),
		(ASN_TAG_CLASS_APPLICATION | (7028 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_numerics_8,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"numerics"
		},
	{ ATF_POINTER, 3, offsetof(struct MeasuredDataSection, enumerations),
		(ASN_TAG_CLASS_APPLICATION | (7029 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_enumerations_10,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"enumerations"
		},
	{ ATF_POINTER, 2, offsetof(struct MeasuredDataSection, alerts),
		(ASN_TAG_CLASS_APPLICATION | (7007 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_alerts_12,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"alerts"
		},
	{ ATF_POINTER, 1, offsetof(struct MeasuredDataSection, attributechanges),
		(ASN_TAG_CLASS_APPLICATION | (7030 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_attributechanges_14,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"attributechanges"
		},
};
static ber_tlv_tag_t asn_DEF_MeasuredDataSection_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_MeasuredDataSection_tag2el_1[] = {
    { (ASN_TAG_CLASS_APPLICATION | (7007 << 2)), 5, 0, 0 }, /* alerts at 1209 */
    { (ASN_TAG_CLASS_APPLICATION | (7025 << 2)), 0, 0, 0 }, /* realtimesas at 1189 */
    { (ASN_TAG_CLASS_APPLICATION | (7026 << 2)), 1, 0, 0 }, /* timesas at 1193 */
    { (ASN_TAG_CLASS_APPLICATION | (7027 << 2)), 2, 0, 0 }, /* distributionsas at 1197 */
    { (ASN_TAG_CLASS_APPLICATION | (7028 << 2)), 3, 0, 0 }, /* numerics at 1201 */
    { (ASN_TAG_CLASS_APPLICATION | (7029 << 2)), 4, 0, 0 }, /* enumerations at 1205 */
    { (ASN_TAG_CLASS_APPLICATION | (7030 << 2)), 6, 0, 0 } /* attributechanges at 1212 */
};
static asn_SEQUENCE_specifics_t asn_SPC_MeasuredDataSection_specs_1 = {
	sizeof(struct MeasuredDataSection),
	offsetof(struct MeasuredDataSection, _asn_ctx),
	asn_MAP_MeasuredDataSection_tag2el_1,
	7,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_MeasuredDataSection = {
	"MeasuredDataSection",
	"MeasuredDataSection",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_MeasuredDataSection_tags_1,
	sizeof(asn_DEF_MeasuredDataSection_tags_1)
		/sizeof(asn_DEF_MeasuredDataSection_tags_1[0]), /* 1 */
	asn_DEF_MeasuredDataSection_tags_1,	/* Same as above */
	sizeof(asn_DEF_MeasuredDataSection_tags_1)
		/sizeof(asn_DEF_MeasuredDataSection_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_MeasuredDataSection_1,
	7,	/* Elements count */
	&asn_SPC_MeasuredDataSection_specs_1	/* Additional specs */
};
