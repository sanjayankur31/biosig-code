#include <python2.7/Python.h>
#define BIOSIG_MODULE
#include "biosigmodule.h"

#include <string.h>
#include <biosig.h>

static PyObject *BiosigError;

static int PyBiosig_Header(const char *filename, char **jsonstr) {
	// open file 
	HDRTYPE *hdr = NULL;
	hdr = sopen(filename, "r", hdr);

	if (serror2(hdr)) {
	        PyErr_SetString(BiosigError, "could not open file");
		destructHDR(hdr);
	        return -1;
	}

	// convert to json-string
	char *str = NULL;
	asprintf_hdr2json(&str, hdr);

	destructHDR(hdr);

	*jsonstr = strdup(str);
	return 0;
}

static PyObject *biosig_json_header(PyObject *self, PyObject *args) {
	// get input arguments 

	const char *filename = NULL;
	char *str = NULL;

	if (!PyArg_ParseTuple(args, "s", &filename)) return NULL;

	if (PyBiosig_Header(filename, &str)) return NULL;

	return Py_BuildValue("s", str);
}


static PyMethodDef BiosigMethods[] = {
    {"header",  biosig_json_header, METH_VARARGS, "load biosig header and export as JSON ."},
/*
    {"data",    biosig_json_header, METH_VARARGS, "load biosig header and export as JSON ."},
    {"base64",  biosig_json_header, METH_VARARGS, "load biosig header and export as JSON ."},
    {"fhir_json_binary_template",  biosig_json_header, METH_VARARGS, "load biosig header and export as JSON ."},
    {"fhir_xml_binary_template",  biosig_json_header, METH_VARARGS, "load biosig header and export as JSON ."},
*/
    {NULL, NULL, 0, NULL}        /* Sentinel */
};


PyMODINIT_FUNC initbiosig(void) {
    PyObject *m;

    m = Py_InitModule("biosig", BiosigMethods);

    if (m == NULL) return;

    BiosigError = PyErr_NewException("biosig.error", NULL, NULL);
    Py_INCREF(BiosigError);
    PyModule_AddObject(m, "error", BiosigError);

     /* additional initialization can happen here */
}


