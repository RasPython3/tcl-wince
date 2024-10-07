/*
 * Copyright © 2021, 2024 Nathan Coulter.  All rights reserved.
 *
 * You may distribute and/or modify this program under the terms of the GNU
 * Affero General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.

 * See the file "COPYING" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
*/

/*
 *	Functions to test abstract lists.
 *
 * tclTestABSList.c --
 */

#undef BUILD_tcl
#undef STATIC_BUILD
#ifndef USE_TCL_STUBS
#   define USE_TCL_STUBS
#endif
#include <string.h>
#include <limits.h>
#include "tclInt.h"

/*
 * Forward references
 */

Tcl_Obj *myNewLStringObj(Tcl_WideInt start,
			 Tcl_WideInt length);
static void freeRep(Tcl_Obj* alObj);
static Tcl_ObjInterfaceListSetDeepProc my_LStringObjSetElemR;
static Tcl_DupInternalRepProc DupLStringRep;
static Tcl_ObjInterfaceListLengthProc my_LStringObjLength;
static Tcl_ObjInterfaceListIndexProc my_LStringObjIndex;
static Tcl_ObjInterfaceListRangeProc my_LStringObjRange;
static Tcl_ObjInterfaceListReverseProc my_LStringObjReverse;
static Tcl_ObjInterfaceListReplaceProc my_LStringReplace;
static Tcl_ObjInterfaceListAllProc my_LStringGetElements;
static void lstringFreeElements(Tcl_Obj* lstringObj);
static Tcl_UpdateStringProc UpdateStringOfLString;

/*
 * Internal Representation of an lstring type value
 */

typedef struct LString {
    char *string;	// NULL terminated utf-8 string
    Tcl_Size strlen;	// num bytes in string
    Tcl_Size allocated; // num bytes allocated
    Tcl_Obj**elements;	// elements array, allocated when GetElements is
			// called
} LString;


/*
 * AbstractList definition of an lstring type
 */


static ObjectType lstringTypes[11] = {
    {/*0*/
	"lstring",
	freeRep,
	DupLStringRep,
	UpdateStringOfLString,
	NULL,
	2,
	NULL
    },
    {/*1*/
	"lstring",
	freeRep,
	DupLStringRep,
	UpdateStringOfLString,
	NULL,
	2,
	NULL
    },
    {/*2*/
	"lstring",
	freeRep,
	DupLStringRep,
	UpdateStringOfLString,
	NULL,
	2,
	NULL
    },
    {/*3*/
	"lstring",
	freeRep,
	DupLStringRep,
	UpdateStringOfLString,
	NULL,
	2,
	NULL
    },
    {/*4*/
	"lstring",
	freeRep,
	DupLStringRep,
	UpdateStringOfLString,
	NULL,
	2,
	NULL
    },
    {/*5*/
	"lstring",
	freeRep,
	DupLStringRep,
	UpdateStringOfLString,
	NULL,
	2,
	NULL
    },
    {/*6*/
	"lstring",
	freeRep,
	DupLStringRep,
	UpdateStringOfLString,
	NULL,
	2,
	NULL
    },
    {/*7*/
	"lstring",
	freeRep,
	DupLStringRep,
	UpdateStringOfLString,
	NULL,
	2,
	NULL
    },
    {/*8*/
	"lstring",
	freeRep,
	DupLStringRep,
	UpdateStringOfLString,
	NULL,
	2,
	NULL
    },
    {/*9*/
	"lstring",
	freeRep,
	DupLStringRep,
	UpdateStringOfLString,
	NULL,
	2,
	NULL
    },
    {/*10*/
	"lstring",
	freeRep,
	DupLStringRep,
	UpdateStringOfLString,
	NULL,
	2,
	NULL
    }
};


/*
 *----------------------------------------------------------------------
 *
 * my_LStringObjIndex --
 *
 *	Implements the AbstractList Index function for the lstring type.  The
 *	Index function returns the value at the index position given. Caller
 *	is resposible for freeing the Obj.
 *
 * Results:
 *	TCL_OK on success. Returns a new Obj, with a 0 refcount in the
 *	supplied charObjPtr location. Call has ownership of the Obj.
 *
 * Side effects:
 *	Obj allocated.
 *
 *----------------------------------------------------------------------
 */

static int
my_LStringObjIndex(
    Tcl_Interp *interp,
    Tcl_Obj *lstringObj,
    Tcl_Size index,
    Tcl_Obj **charObjPtr)
{
    LString *lstringRepPtr = (LString*)lstringObj->internalRep.twoPtrValue.ptr1;

  (void)interp;

  if (index < lstringRepPtr->strlen) {
      char cchar[2];
      cchar[0] = lstringRepPtr->string[index];
      cchar[1] = 0;
      *charObjPtr = Tcl_NewStringObj(cchar,1);
  } else {
      *charObjPtr = NULL;
  }

  return TCL_OK;
}


/*
 *----------------------------------------------------------------------
 *
 * my_LStringObjLength --
 *
 *	Implements the AbstractList Length function for the lstring type.
 *	The Length function returns the number of elements in the list.
 *
 * Results:
 *	WideInt number of elements in the list.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static int
my_LStringObjLength(TCL_UNUSED(Tcl_Interp *), Tcl_Obj *lstringObjPtr, Tcl_Size *lenPtr )
{
    LString *lstringRepPtr = (LString *)lstringObjPtr->internalRep.twoPtrValue.ptr1;
    *lenPtr = lstringRepPtr->strlen;
    return TCL_OK; 
}


/*
 *----------------------------------------------------------------------
 *
 * DupLStringRep --
 *
 *	Replicates the internal representation of the src value, and storing
 *	it in the copy
 *
 * Results:
 *	void
 *
 * Side effects:
 *	Modifies the rep of the copyObj.
 *
 *----------------------------------------------------------------------
 */

static void
DupLStringRep(Tcl_Obj *srcPtr, Tcl_Obj *copyPtr)
{
  LString *srcLString = (LString*)srcPtr->internalRep.twoPtrValue.ptr1;
  LString *copyLString = (LString*)Tcl_Alloc(sizeof(LString));

  memcpy(copyLString, srcLString, sizeof(LString));
  copyLString->string = (char*)Tcl_Alloc(srcLString->allocated);
  strncpy(copyLString->string, srcLString->string, srcLString->strlen);
  copyLString->string[srcLString->strlen] = '\0';
  copyLString->elements = NULL;
  Tcl_ObjInternalRep itr;
  itr.twoPtrValue.ptr1 = copyLString;
  itr.twoPtrValue.ptr2 = NULL;
  Tcl_StoreInternalRep(copyPtr, srcPtr->typePtr, &itr);

  return;
}

/*
 *----------------------------------------------------------------------
 *
 * my_LStringObjSetElemR --
 *
 *	Replace the element value at the given (nested) index with the
 *	valueObj provided.  If the lstring obj is shared, a new list is
 *	created conntaining the modifed element.
 *
 * Results:
 *	The modifed lstring is returned, either new or original. If the
 *	index is invalid, NULL is returned, and an error is added to the
 *	interp, if provided.
 *
 * Side effects:
 *	A new obj may be created.
 *
 *----------------------------------------------------------------------
 */

static int
my_LStringObjSetElemR(
    Tcl_Interp *interp,
    Tcl_Obj *lstringObj,
    Tcl_Size numIndicies,
    Tcl_Obj *const indices[],
    Tcl_Obj *valueObj,
    Tcl_Obj **resPtrPtr)
{
    LString *lstringRepPtr = (LString*)lstringObj->internalRep.twoPtrValue.ptr1;
    Tcl_Size index;
    int status;
    Tcl_Obj *resPtr;

    if (numIndicies > 1) {
	Tcl_SetObjResult(interp,
	    Tcl_ObjPrintf("Multiple indices not supported by lstring."));
	*resPtrPtr = NULL;
	return TCL_ERROR;
    }

    status = Tcl_GetIntForIndex(interp, indices[0], lstringRepPtr->strlen, &index);
    if (status != TCL_OK) {
	resPtrPtr = NULL;
	return TCL_ERROR;
    }

    resPtr = Tcl_IsShared(lstringObj) ? Tcl_DuplicateObj(lstringObj) : lstringObj;
    lstringRepPtr = (LString*)resPtr->internalRep.twoPtrValue.ptr1;

    if (index >= lstringRepPtr->strlen) {
	index = lstringRepPtr->strlen;
	lstringRepPtr->strlen++;
	lstringRepPtr->string = (char*)Tcl_Realloc(lstringRepPtr->string, lstringRepPtr->strlen+1);
    }

    if (valueObj) {
	const char newvalue = Tcl_GetString(valueObj)[0];
	lstringRepPtr->string[index] = newvalue;
    } else if (index < lstringRepPtr->strlen) {
	/* Remove the char by sliding the tail of the string down */
	char *sptr = &lstringRepPtr->string[index];
	/* This is an overlapping copy, by definition */
	lstringRepPtr->strlen--;
	memmove(sptr, (sptr+1), (lstringRepPtr->strlen - index));
    }
    // else do nothing

    Tcl_InvalidateStringRep(resPtr);

    *resPtrPtr = resPtr;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * my_LStringObjRange --
 *
 *	Creates a new Obj with a slice of the src listPtr.
 *
 * Results:
 *	A new Obj is assigned to newObjPtr. Returns TCL_OK
 *
 * Side effects:
 *	A new Obj is created.
 *
 *----------------------------------------------------------------------
 */

int my_LStringObjRange(
    Tcl_Interp *interp,
    Tcl_Obj *lstringObj,
    Tcl_Size fromIdx,
    Tcl_Size toIdx,
    Tcl_Obj **resPtrPtr)
{
    Tcl_Obj *rangeObj, *newObjPtr;
    LString *lstringRepPtr = (LString*)lstringObj->internalRep.twoPtrValue.ptr1;
    LString *rangeRep;
    Tcl_WideInt len = toIdx - fromIdx + 1;

    if (lstringRepPtr->strlen < fromIdx ||
	lstringRepPtr->strlen < toIdx) {
	Tcl_SetObjResult(interp,
	    Tcl_ObjPrintf("Range out of bounds "));
	*resPtrPtr = NULL;
	return TCL_ERROR;
    }

    if (len <= 0) {
	// Return empty value;
	newObjPtr = Tcl_NewObj();
    } else {
	rangeRep = (LString*)Tcl_Alloc(sizeof(LString));
	rangeRep->allocated = len+1;
	rangeRep->strlen = len;
	rangeRep->string = (char*)Tcl_Alloc(rangeRep->allocated);
	strncpy(rangeRep->string,&lstringRepPtr->string[fromIdx],len);
	rangeRep->string[len] = 0;
	rangeRep->elements = NULL;
	rangeObj = Tcl_NewObj();
	Tcl_ObjInternalRep itr;
	itr.twoPtrValue.ptr1 = rangeRep;
	itr.twoPtrValue.ptr2 = NULL;
	Tcl_StoreInternalRep(rangeObj, lstringObj->typePtr, &itr);
	if (rangeRep->strlen > 0) {
	    Tcl_InvalidateStringRep(rangeObj);
	} else {
	    Tcl_InitStringRep(rangeObj, NULL, 0);
	}
	newObjPtr = rangeObj;
    }
    *resPtrPtr = newObjPtr;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * my_LStringObjReverse --
 *
 *	Creates a new Obj with the order of the elements in the lstring
 *	value reversed, where first is last and last is first, etc.
 *
 * Results:
 *	A new Obj is assigned to newObjPtr. Returns TCL_OK
 *
 * Side effects:
 *	A new Obj is created.
 *
 *----------------------------------------------------------------------
 */

static int
my_LStringObjReverse(Tcl_Interp *interp, Tcl_Obj *srcObj)
{
    LString *srcRep = (LString*)srcObj->internalRep.twoPtrValue.ptr1;
    Tcl_Size len;
    char *srcp, *endp;
    char temp;
    (void)interp;  
    if (Tcl_IsShared(srcObj)) {
	Tcl_Panic("%s called with shared object", "my_LStringObjReverse");
    }

    len = srcRep->strlen;
    srcp = srcRep->string;
    endp = &srcRep->string[len];
    endp--;
    while (srcp < endp) {
	temp = *endp;
	*endp-- = *srcp;
	*srcp++ = temp;
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * my_LStringReplace --
 *
 *	Delete and/or Insert elements in the list, starting at index first.
 *	See more details in the comments below. This should not be called with
 *	a Shared Obj.
 *
 * Results:
 *	The value of the listObj is modified.
 *
 * Side effects:
 *	The string rep is invalidated.
 *
 *----------------------------------------------------------------------
 */

static int
my_LStringReplace(
    Tcl_Interp *interp,
    Tcl_Obj *listObj,
    Tcl_Size first,
    Tcl_Size numToDelete,
    Tcl_Size numToInsert,
    Tcl_Obj *const insertObjs[])
{
    LString *lstringRep = (LString*)listObj->internalRep.twoPtrValue.ptr1;
    Tcl_Size newLen;
    Tcl_Size x, ix, kx;
    char *newStr;
    char *oldStr = lstringRep->string;
    (void)interp;

    newLen = lstringRep->strlen - numToDelete + numToInsert;

    if (newLen >= lstringRep->allocated) {
	lstringRep->allocated = newLen+1;
	newStr = (char*)Tcl_Alloc(lstringRep->allocated);
	newStr[newLen] = 0;
    } else {
	newStr = oldStr;
    }

    /* Tcl_ListObjReplace replaces zero or more elements of the list
     * referenced by listPtr with the objc values in the array referenced by
     * objv.
     *
     * If listPtr does not point to a list value, Tcl_ListObjReplace
     * will attempt to convert it to one; if the conversion fails, it returns
     * TCL_ERROR and leaves an error message in the interpreter's result value
     * if interp is not NULL. Otherwise, it returns TCL_OK after replacing the
     * values.
     *
     *    * If objv is NULL, no new elements are added.
     *
     *    * If the argument first is zero or negative, it refers to the first
     *      element.
     *
     *    * If first is greater than or equal to the number of elements in the
     *      list, then no elements are deleted; the new elements are appended
     *      to the list. count gives the number of elements to replace.
     *
     *    * If count is zero or negative then no elements are deleted; the new
     *      elements are simply inserted before the one designated by first.
     *      Tcl_ListObjReplace invalidates listPtr's old string representation.
     *
     *    * The reference counts of any elements inserted from objv are
     *      incremented since the resulting list now refers to them. Similarly,
     *      the reference counts for any replaced values are decremented.
     */

    // copy 0 to first-1
    if (newStr != oldStr) {
	strncpy(newStr, oldStr, first);
    }

    // move front elements to keep
    for(x=0, kx=0; x<newLen && kx<first; kx++, x++) {
	newStr[x] = oldStr[kx];
    }
    // Insert new elements into new string
    for(x=first, ix=0; ix<numToInsert; x++, ix++) {
	char const *svalue = Tcl_GetString(insertObjs[ix]);
	newStr[x] = svalue[0];
    }
    // Move remaining elements
    if ((first+numToDelete) < newLen) {
	for(/*x,*/ kx=first+numToDelete; (kx <lstringRep->strlen && x<newLen); x++, kx++) {
	    newStr[x] = oldStr[kx];
	}
    }

    // Terminate new string.
    newStr[newLen] = 0;


    if (oldStr != newStr) {
	Tcl_Free(oldStr);
    }
    lstringRep->string = newStr;
    lstringRep->strlen = newLen;

    /* Changes made to value, string rep and elements array no longer valid */
    Tcl_InvalidateStringRep(listObj);
    lstringFreeElements(listObj);

    return TCL_OK;
}

static const Tcl_ObjType *
my_SetAbstractProc(int ptype)
{
    const ObjectType *typePtr = &lstringTypes[0]; /* default value */
    if (4 <= ptype && ptype <= 11) {
	/* Table has no entries for the slots upto setfromany */
	typePtr = &lstringTypes[(ptype-3)];
    }
    return (Tcl_ObjType *)typePtr;
}


/*
 *----------------------------------------------------------------------
 *
 * my_NewLStringObj --
 *
 *	Creates a new lstring Obj using the string value of objv[0]
 *
 * Results:
 *	results
 *
 * Side effects:
 *	side effects
 *
 *----------------------------------------------------------------------
 */

static Tcl_Obj *
my_NewLStringObj(
    Tcl_Interp *interp,
    int objc,
    Tcl_Obj * const objv[])
{
    LString *lstringRepPtr;
    Tcl_ObjInternalRep itr;
    size_t repSize;
    Tcl_Obj *lstringPtr;
    const char *string;
    static const char* procTypeNames[] = {
	"FREEREP", "DUPREP", "UPDATESTRING", "SETFROMANY",
	"LENGTH", "INDEX", "SLICE", "REVERSE", "GETELEMENTS",
	"SETELEMENT", "REPLACE", NULL
    };
    int i = 0;
    int ptype;
    const Tcl_ObjType *lstringTypePtr = (Tcl_ObjType *)&lstringTypes[10];

    repSize = sizeof(LString);
    lstringRepPtr = (LString*)Tcl_Alloc(repSize);

    while (i<objc) {
	const char *s = Tcl_GetString(objv[i]);
	if (strcmp(s, "-not")==0) {
	    i++;
	    if (Tcl_GetIndexFromObj(interp, objv[i], procTypeNames, "proctype", 0, &ptype)==TCL_OK) {
		lstringTypePtr = my_SetAbstractProc(ptype);
	    }
	} else if (strcmp(s, "--") == 0) {
	    // End of options
	    i++;
	    break;
	} else {
	    break;
	}
	i++;
    }
    if (i != objc-1) {
	Tcl_Free((char*)lstringRepPtr);
	Tcl_WrongNumArgs(interp, 0, objv, "lstring string");
	return NULL;
    }
    string = Tcl_GetString(objv[i]);

    lstringRepPtr->strlen = strlen(string);
    lstringRepPtr->allocated = lstringRepPtr->strlen + 1;
    lstringRepPtr->string = (char*)Tcl_Alloc(lstringRepPtr->allocated);
    strcpy(lstringRepPtr->string, string);
    lstringRepPtr->elements = NULL;
    lstringPtr = Tcl_NewObj();
    itr.twoPtrValue.ptr1 = lstringRepPtr;
    itr.twoPtrValue.ptr2 = NULL;
    Tcl_StoreInternalRep(lstringPtr, lstringTypePtr, &itr);
    if (lstringRepPtr->strlen > 0) {
	Tcl_InvalidateStringRep(lstringPtr);
    } else {
	Tcl_InitStringRep(lstringPtr, NULL, 0);
    }
    return lstringPtr;
}

/*
 *----------------------------------------------------------------------
 *
 * freeElements --
 *
 *      Free the element array
 *
 */

static void
lstringFreeElements(Tcl_Obj* lstringObj)
{
    LString *lstringRepPtr = (LString*)lstringObj->internalRep.twoPtrValue.ptr1;
    if (lstringRepPtr->elements) {
	Tcl_Obj **objptr = lstringRepPtr->elements;
	while (objptr < &lstringRepPtr->elements[lstringRepPtr->strlen]) {
	    Tcl_DecrRefCount(*objptr++);
	}
	Tcl_Free((char*)lstringRepPtr->elements);
	lstringRepPtr->elements = NULL;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * freeRep --
 *
 *	Free the value storage of the lstring Obj.
 *
 * Results:
 *	void
 *
 * Side effects:
 *	Memory free'd.
 *
 *----------------------------------------------------------------------
 */

static void
freeRep(Tcl_Obj* lstringObj)
{
    LString *lstringRepPtr = (LString*)lstringObj->internalRep.twoPtrValue.ptr1;
    if (lstringRepPtr->string) {
	Tcl_Free(lstringRepPtr->string);
    }
    lstringFreeElements(lstringObj);
    Tcl_Free((char*)lstringRepPtr);
    lstringObj->internalRep.twoPtrValue.ptr1 = NULL;
}

/*
 *----------------------------------------------------------------------
 *
 * my_LStringGetElements --
 *
 *	Get the elements of the list in an array.
 *
 * Results:
 *	objc, objv return values
 *
 * Side effects:
 *	A Tcl_Obj is stored for every element of the abstract list
 *
 *----------------------------------------------------------------------
 */

static int my_LStringGetElements(Tcl_Interp *interp,
				 Tcl_Obj *lstringObj,
				 Tcl_Size *objcptr,
				 Tcl_Obj ***objvptr)
{
    LString *lstringRepPtr = (LString*)lstringObj->internalRep.twoPtrValue.ptr1;
    Tcl_Obj **objPtr;
    char *cptr = lstringRepPtr->string;
    (void)interp;
    if (lstringRepPtr->strlen == 0) {
	*objcptr = 0;
	*objvptr = NULL;
	return TCL_OK;
    }
    if (lstringRepPtr->elements == NULL) {
	lstringRepPtr->elements = (Tcl_Obj**)Tcl_Alloc(sizeof(Tcl_Obj*) * lstringRepPtr->strlen);
	objPtr=lstringRepPtr->elements;
	while (objPtr < &lstringRepPtr->elements[lstringRepPtr->strlen]) {
	    *objPtr = Tcl_NewStringObj(cptr++,1);
	    Tcl_IncrRefCount(*objPtr++);
	}
    }
    *objvptr = lstringRepPtr->elements;
    *objcptr = lstringRepPtr->strlen;
    return TCL_OK;
}

/*
** UpdateStringRep
*/

static void
UpdateStringOfLString(Tcl_Obj *objPtr)
{
#   define LOCAL_SIZE 64
    int localFlags[LOCAL_SIZE], *flagPtr = NULL;
    char *p;
    int bytesNeeded = 0;
    int status;
    Tcl_Size i ,llen;


    /*
     * Handle empty list case first, so rest of the routine is simpler.
     */
    status = my_LStringObjLength(NULL, objPtr, &llen);
    if ((status != TCL_OK) || llen <= 0) {
	Tcl_InitStringRep(objPtr, NULL, 0);
	return;
    }

    /*
     * Pass 1: estimate space.
     */
    if (llen <= LOCAL_SIZE) {
	flagPtr = localFlags;
    } else {
	/* We know numElems <= LIST_MAX, so this is safe. */
	flagPtr = (int *) Tcl_Alloc(llen*sizeof(int));
    }
    for (bytesNeeded = 0, i = 0; i < llen; i++) {
	Tcl_Obj *elemObj;
	const char *elemStr;
	Tcl_Size elemLen;
	flagPtr[i] = (i ? TCL_DONT_QUOTE_HASH : 0);

	my_LStringObjIndex(NULL, objPtr, i, &elemObj);

	Tcl_IncrRefCount(elemObj);
	elemStr = Tcl_GetStringFromObj(elemObj, &elemLen);
	/* Note TclScanElement updates flagPtr[i] */
	bytesNeeded += Tcl_ScanCountedElement(elemStr, elemLen, &flagPtr[i]);
	if (bytesNeeded < 0) {
	    Tcl_Panic("max size for a Tcl value (%d bytes) exceeded", INT_MAX);
	}
	Tcl_DecrRefCount(elemObj);
    }
    if (bytesNeeded > INT_MAX - llen + 1) {
	Tcl_Panic("max size for a Tcl value (%d bytes) exceeded", INT_MAX);
    }
    bytesNeeded += llen; /* Separating spaces and terminating nul */

    /*
     * Pass 2: generate the string repr.
     */
    objPtr->bytes = (char *) Tcl_Alloc(bytesNeeded);
    p = objPtr->bytes;
    for (i = 0; i < llen; i++) {
	Tcl_Obj *elemObj;
	const char *elemStr;
	Tcl_Size elemLen;
	flagPtr[i] |= (i ? TCL_DONT_QUOTE_HASH : 0);
	my_LStringObjIndex(NULL, objPtr, i, &elemObj);
	Tcl_IncrRefCount(elemObj);
	elemStr = Tcl_GetStringFromObj(elemObj, &elemLen);
	p += Tcl_ConvertCountedElement(elemStr, elemLen, p, flagPtr[i]);
	*p++ = ' ';
	Tcl_DecrRefCount(elemObj);
    }
    p[-1] = '\0'; /* Overwrite last space added */

    /* Length of generated string */
    objPtr->length = p - 1 - objPtr->bytes;

    if (flagPtr != localFlags) {
	Tcl_Free(flagPtr);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * lLStringObjCmd --
 *
 *	Script level command that creats an lstring Obj value.
 *
 * Results:
 *	Returns and lstring Obj value in the interp results.
 *
 * Side effects:
 *	Interp results modified.
 *
 *----------------------------------------------------------------------
 */

static int
lLStringObjCmd(
    void *clientData,
    Tcl_Interp *interp,
    int objc,
    Tcl_Obj * const objv[])
{
  Tcl_Obj *lstringObj;

  (void)clientData;
  if (objc < 2) {
      Tcl_WrongNumArgs(interp, 1, objv, "string");
      return TCL_ERROR;
  }

  lstringObj = my_NewLStringObj(interp, objc-1, &objv[1]);

  if (lstringObj) {
      Tcl_SetObjResult(interp, lstringObj);
      return TCL_OK;
  }
  return TCL_ERROR;
}

/*
** lgen - Derived from TIP 192 - Lazy Lists
** Generate a list using a command provided as argument(s).
** The command computes the value for a given index.
*/

/*
 * Internal rep for the Generate Series
 */
typedef struct LgenSeries {
    Tcl_Interp *interp; // used to evaluate gen script
    Tcl_Size len;       // list length
    Tcl_Size nargs;     // Number of arguments in genFn including "index"
    Tcl_Obj *genFnObj;  // The preformed command as a list. Index is set in
			// the last element (last argument)
} LgenSeries;

/*
 * Evaluate the generation function.
 * The provided funtion computes the value for a give index
 */
static Tcl_Obj*
lgen(
    Tcl_Obj* objPtr,
    Tcl_Size index)
{
    LgenSeries *lgenSeriesPtr = (LgenSeries*)objPtr->internalRep.twoPtrValue.ptr1;
    Tcl_Obj *elemObj = NULL;
    Tcl_Interp *intrp = lgenSeriesPtr->interp;
    Tcl_Obj *genCmd = lgenSeriesPtr->genFnObj;
    Tcl_Size endidx = lgenSeriesPtr->nargs-1;

    if (0 <= index && index < lgenSeriesPtr->len) {
	Tcl_Obj *indexObj = Tcl_NewWideIntObj(index);
	Tcl_ListObjReplace(intrp, genCmd, endidx, 1, 1, &indexObj);
	// EVAL DIRECT to avoid interfering with bytecode compile which may be
	// active on the stack
	int flags = TCL_EVAL_GLOBAL|TCL_EVAL_DIRECT;
	int status = Tcl_EvalObjEx(intrp, genCmd, flags);
	elemObj = Tcl_GetObjResult(intrp);
	if (status != TCL_OK) {
	    Tcl_SetObjResult(intrp, Tcl_ObjPrintf(
		"Error: %s\nwhile executing %s\n",
		elemObj ? Tcl_GetString(elemObj) : "NULL", Tcl_GetString(genCmd)));
	    return NULL;
	}
    }
    return elemObj;
}

/*
 *  Abstract List Length function
 */
static int
lgenSeriesObjLength(TCL_UNUSED(Tcl_Interp *), Tcl_Obj *objPtr, Tcl_Size *lenPtr)
{
    LgenSeries *lgenSeriesRepPtr = (LgenSeries *)objPtr->internalRep.twoPtrValue.ptr1;
    *lenPtr = lgenSeriesRepPtr->len;
    return TCL_OK;
}

/*
 *  Abstract List Index function
 */
static int
lgenSeriesObjIndex(
    Tcl_Interp *interp,
    Tcl_Obj *lgenSeriesObjPtr,
    Tcl_Size index,
    Tcl_Obj **elemPtr)
{
    LgenSeries *lgenSeriesRepPtr;
    Tcl_Obj *element;

    lgenSeriesRepPtr = (LgenSeries*)lgenSeriesObjPtr->internalRep.twoPtrValue.ptr1;

    if (index < 0 || index >= lgenSeriesRepPtr->len) {
	*elemPtr = NULL;
	return TCL_OK;
    }
    if (lgenSeriesRepPtr->interp == NULL && interp == NULL) {
	return TCL_ERROR;
    }

    lgenSeriesRepPtr->interp = interp;

    element = lgen(lgenSeriesObjPtr, index);
    if (element) {
	*elemPtr = element;
    } else {
	return TCL_ERROR;
    }

    return TCL_OK;
}

/*
** UpdateStringRep
*/

static void
UpdateStringOfLgen(Tcl_Obj *objPtr)
{
    LgenSeries *lgenSeriesRepPtr;
    Tcl_Obj *element;
    Tcl_Size i;
    size_t bytlen;
    Tcl_Obj *tmpstr = Tcl_NewObj();

    lgenSeriesRepPtr = (LgenSeries*)objPtr->internalRep.twoPtrValue.ptr1;

    for (i=0, bytlen=0; i<lgenSeriesRepPtr->len; i++) {
	element = lgen(objPtr, i);
	if (element) {
	    if (i) {
		Tcl_AppendToObj(tmpstr," ",1);
	    }
	    Tcl_AppendObjToObj(tmpstr,element);
	}
    }

    bytlen = Tcl_GetCharLength(tmpstr);
    Tcl_InitStringRep(objPtr, Tcl_GetString(tmpstr), bytlen);
    Tcl_DecrRefCount(tmpstr);

    return;
}

/*
 *  ObjType Free Internal Rep function
 */
static void
FreeLgenInternalRep(Tcl_Obj *objPtr)
{
    LgenSeries *lgenSeries = (LgenSeries*)objPtr->internalRep.twoPtrValue.ptr1;
    if (lgenSeries->genFnObj) {
	Tcl_DecrRefCount(lgenSeries->genFnObj);
    }
    lgenSeries->interp = NULL;
    Tcl_Free(lgenSeries);
    objPtr->internalRep.twoPtrValue.ptr1 = 0;
}

static void DupLgenSeriesRep(Tcl_Obj *srcPtr, Tcl_Obj *copyPtr);

/*
 *  Abstract List ObjType definition
 */


static ObjectType lgenObjectType = {
    "lgenseries",
    FreeLgenInternalRep,
    DupLgenSeriesRep,
    UpdateStringOfLgen,
    NULL, /* SetFromAnyProc */
    0,
    NULL
};


static Tcl_ObjType *lgenTypePtr = (Tcl_ObjType *)&lgenObjectType;

/*
 *  ObjType Duplicate Internal Rep Function
 */
static void
DupLgenSeriesRep(
    Tcl_Obj *srcPtr,
    Tcl_Obj *copyPtr)
{
    LgenSeries *srcLgenSeries = (LgenSeries*)srcPtr->internalRep.twoPtrValue.ptr1;
    Tcl_Size repSize = sizeof(LgenSeries);
    LgenSeries *copyLgenSeries = (LgenSeries*)Tcl_Alloc(repSize);

    copyLgenSeries->interp = srcLgenSeries->interp;
    copyLgenSeries->nargs = srcLgenSeries->nargs;
    copyLgenSeries->len = srcLgenSeries->len;
    copyLgenSeries->genFnObj = Tcl_DuplicateObj(srcLgenSeries->genFnObj);
    Tcl_IncrRefCount(copyLgenSeries->genFnObj);
    copyPtr->typePtr = lgenTypePtr;
    copyPtr->internalRep.twoPtrValue.ptr1 = copyLgenSeries;
    copyPtr->internalRep.twoPtrValue.ptr2 = NULL;
    return;
}

/*
 *  Create a new lgen Tcl_Obj
 */
Tcl_Obj *
newLgenObj(
    Tcl_Interp *interp,
    int objc,
    Tcl_Obj * const objv[])
{
    Tcl_WideInt length;
    LgenSeries *lGenSeriesRepPtr;
    Tcl_Size repSize;
    Tcl_Obj *lGenSeriesObj;

    if (objc < 2) {
	return NULL;
    }

    if (Tcl_GetWideIntFromObj(NULL, objv[0], &length) != TCL_OK
	|| length < 0) {
	return NULL;
    }

    lGenSeriesObj = Tcl_NewObj();
    repSize = sizeof(LgenSeries);
    lGenSeriesRepPtr = (LgenSeries*)Tcl_Alloc(repSize);
    lGenSeriesRepPtr->interp = interp; //Tcl_CreateInterp();
    lGenSeriesRepPtr->len = length;

    // Allocate array of *obj for cmd + index + args
    // objv  length cmd arg1 arg2 arg3 ...
    // argsv         0   1    2    3   ... index

    lGenSeriesRepPtr->nargs = objc;
    lGenSeriesRepPtr->genFnObj = Tcl_NewListObj(objc-1, objv+1);
    // Addd 0 placeholder for index
    Tcl_ListObjAppendElement(interp, lGenSeriesRepPtr->genFnObj, Tcl_NewIntObj(0));
    Tcl_IncrRefCount(lGenSeriesRepPtr->genFnObj);
    lGenSeriesObj->internalRep.twoPtrValue.ptr1 = lGenSeriesRepPtr;
    lGenSeriesObj->internalRep.twoPtrValue.ptr2 = NULL;
    lGenSeriesObj->typePtr = lgenTypePtr;

    if (length > 0) {
	Tcl_InvalidateStringRep(lGenSeriesObj);
    } else {
	Tcl_InitStringRep(lGenSeriesObj, NULL, 0);
    }
    return lGenSeriesObj;
}

/*
 *  The [lgen] command
 */
static int
lGenObjCmd(
    TCL_UNUSED(void *),
    Tcl_Interp *interp,
    int objc,
    Tcl_Obj * const objv[])
{
    Tcl_Obj *genObj = newLgenObj(interp, objc-1, &objv[1]);
    if (genObj) {
	Tcl_SetObjResult(interp, genObj);
	return TCL_OK;
    }
    Tcl_WrongNumArgs(interp, 1, objv, "length cmd ?args?");
    return TCL_ERROR;
}

/*
 *  lgen package init
 */
int Lgen_Init(Tcl_Interp *interp) {
    if (Tcl_InitStubs(interp, "8.7", 0) == NULL) {
	return TCL_ERROR;
    }
    Tcl_CreateObjCommand(interp, "lgen", lGenObjCmd, NULL, NULL);
    Tcl_PkgProvide(interp, "lgen", "1.0");
    return TCL_OK;
}



/*
 *----------------------------------------------------------------------
 *
 * ABSListTest_Init --
 *
 *	Provides Abstract List implemenations via new commands
 *
 * lstring command
 * Usage:
 *      lstring /string/
 *
 * Description:
 *      Creates a list where each character in the string is treated as an
 *      element. The string is kept as a string, not an actual list. Indexing
 *      is done by char.
 *
 * lgen command
 * Usage:
 *      lgen /length/ /cmd/ ?args...?
 *
 *      The /cmd/ should take the last argument as the index value, and return
 *      a value for that element.
 *
 * Results:
 *	The commands listed above are added to the interp.
 *
 * Side effects:
 *	New commands defined.
 *
 *----------------------------------------------------------------------
 */

int Tcl_ABSListTest_Init(Tcl_Interp *interp) {
    if (Tcl_InitStubs(interp, "8.7-", 0) == NULL) {
	return TCL_ERROR;
    }
    Tcl_ObjInterface *lgenIfPtr ,*lstringfullPtr ,*lstringNoLengthPtr ,*lstringNoIndexPtr
	,*lstringNoRangePtr ,*lstringNoGetElementsPtr
	,*lstringNoSetElementRPtr ,*lstringNoReplacePtr
	;
    
    lstringfullPtr = Tcl_NewObjInterface();
    Tcl_ObjInterfaceSetVersion(lstringfullPtr ,1);
    Tcl_ObjInterfaceSetFnListAll(lstringfullPtr , my_LStringGetElements);
    Tcl_ObjInterfaceSetFnListIndex(lstringfullPtr ,my_LStringObjIndex);
    Tcl_ObjInterfaceSetFnListLength(lstringfullPtr ,my_LStringObjLength);
    Tcl_ObjInterfaceSetFnListRange(lstringfullPtr ,my_LStringObjRange);
    Tcl_ObjInterfaceSetFnListReplace(lstringfullPtr ,my_LStringReplace);
    Tcl_ObjInterfaceSetFnListReverse(lstringfullPtr ,my_LStringObjReverse);
    Tcl_ObjInterfaceSetFnListSetDeep(lstringfullPtr ,my_LStringObjSetElemR);
    Tcl_ObjTypeSetInterface((Tcl_ObjType *)&lstringTypes[0], lstringfullPtr);
    Tcl_ObjTypeSetInterface((Tcl_ObjType *)&lstringTypes[4], lstringfullPtr);
    Tcl_ObjTypeSetInterface((Tcl_ObjType *)&lstringTypes[8], lstringfullPtr);
    Tcl_ObjTypeSetInterface((Tcl_ObjType *)&lstringTypes[9], lstringfullPtr);
    Tcl_ObjTypeSetInterface((Tcl_ObjType *)&lstringTypes[10], lstringfullPtr);

    lstringNoLengthPtr = Tcl_NewObjInterface();
    Tcl_ObjInterfaceSetFnListAll(lstringNoLengthPtr , my_LStringGetElements);
    Tcl_ObjInterfaceSetFnListIndex(lstringNoLengthPtr ,my_LStringObjIndex);
    Tcl_ObjInterfaceSetFnListRange(lstringNoLengthPtr ,my_LStringObjRange);
    Tcl_ObjInterfaceSetFnListReplace(lstringNoLengthPtr ,my_LStringReplace);
    Tcl_ObjInterfaceSetFnListReverse(lstringNoLengthPtr ,my_LStringObjReverse);
    Tcl_ObjInterfaceSetFnListSetDeep(lstringNoLengthPtr ,my_LStringObjSetElemR);
    Tcl_ObjTypeSetInterface((Tcl_ObjType *)&lstringTypes[1], lstringNoLengthPtr);

    lstringNoIndexPtr = Tcl_NewObjInterface();
    Tcl_ObjInterfaceSetFnListAll(lstringNoIndexPtr , my_LStringGetElements);
    Tcl_ObjInterfaceSetFnListLength(lstringNoIndexPtr ,my_LStringObjLength);
    Tcl_ObjInterfaceSetFnListRange(lstringNoIndexPtr ,my_LStringObjRange);
    Tcl_ObjInterfaceSetFnListReplace(lstringNoIndexPtr ,my_LStringReplace);
    Tcl_ObjInterfaceSetFnListReverse(lstringNoIndexPtr ,my_LStringObjReverse);
    Tcl_ObjInterfaceSetFnListSetDeep(lstringNoIndexPtr ,my_LStringObjSetElemR);
    Tcl_ObjTypeSetInterface((Tcl_ObjType *)&lstringTypes[2], lstringNoIndexPtr);

    lstringNoRangePtr = Tcl_NewObjInterface();
    Tcl_ObjInterfaceSetFnListAll(lstringNoRangePtr , my_LStringGetElements);
    Tcl_ObjInterfaceSetFnListIndex(lstringNoRangePtr ,my_LStringObjIndex);
    Tcl_ObjInterfaceSetFnListLength(lstringNoRangePtr ,my_LStringObjLength);
    Tcl_ObjInterfaceSetFnListReplace(lstringNoRangePtr ,my_LStringReplace);
    Tcl_ObjInterfaceSetFnListReverse(lstringNoRangePtr ,my_LStringObjReverse);
    Tcl_ObjInterfaceSetFnListSetDeep(lstringNoRangePtr ,my_LStringObjSetElemR);
    Tcl_ObjTypeSetInterface((Tcl_ObjType *)&lstringTypes[3], lstringNoRangePtr);

    lstringNoGetElementsPtr = Tcl_NewObjInterface();
    Tcl_ObjInterfaceSetVersion(lstringNoGetElementsPtr ,1);
    Tcl_ObjInterfaceSetFnListIndex(lstringNoGetElementsPtr ,my_LStringObjIndex);
    Tcl_ObjInterfaceSetFnListLength(lstringNoGetElementsPtr ,my_LStringObjLength);
    Tcl_ObjInterfaceSetFnListRange(lstringNoGetElementsPtr ,my_LStringObjRange);
    Tcl_ObjInterfaceSetFnListReplace(lstringNoGetElementsPtr ,my_LStringReplace);
    Tcl_ObjInterfaceSetFnListReverse(lstringNoGetElementsPtr ,my_LStringObjReverse);
    Tcl_ObjInterfaceSetFnListSetDeep(lstringNoGetElementsPtr ,my_LStringObjSetElemR);
    Tcl_ObjTypeSetInterface((Tcl_ObjType *)&lstringTypes[5], lstringNoGetElementsPtr);

    lstringNoSetElementRPtr = Tcl_NewObjInterface();
    Tcl_ObjInterfaceSetVersion(lstringNoSetElementRPtr ,1);
    Tcl_ObjInterfaceSetFnListAll(lstringNoSetElementRPtr , my_LStringGetElements);
    Tcl_ObjInterfaceSetFnListIndex(lstringNoSetElementRPtr ,my_LStringObjIndex);
    Tcl_ObjInterfaceSetFnListLength(lstringNoSetElementRPtr ,my_LStringObjLength);
    Tcl_ObjInterfaceSetFnListRange(lstringNoSetElementRPtr ,my_LStringObjRange);
    Tcl_ObjInterfaceSetFnListReplace(lstringNoSetElementRPtr ,my_LStringReplace);
    Tcl_ObjInterfaceSetFnListReverse(lstringNoSetElementRPtr ,my_LStringObjReverse);
    Tcl_ObjTypeSetInterface((Tcl_ObjType *)&lstringTypes[6], lstringfullPtr);

    lstringNoReplacePtr = Tcl_NewObjInterface();
    Tcl_ObjInterfaceSetVersion(lstringNoReplacePtr ,1);
    Tcl_ObjInterfaceSetFnListAll(lstringNoReplacePtr , my_LStringGetElements);
    Tcl_ObjInterfaceSetFnListIndex(lstringNoReplacePtr ,my_LStringObjIndex);
    Tcl_ObjInterfaceSetFnListLength(lstringNoReplacePtr ,my_LStringObjLength);
    Tcl_ObjInterfaceSetFnListRange(lstringNoReplacePtr ,my_LStringObjRange);
    Tcl_ObjInterfaceSetFnListReverse(lstringNoReplacePtr ,my_LStringObjReverse);
    Tcl_ObjInterfaceSetFnListSetDeep(lstringNoReplacePtr ,my_LStringObjSetElemR);
    Tcl_ObjTypeSetInterface((Tcl_ObjType *)&lstringTypes[7], lstringNoReplacePtr);

    lgenIfPtr = Tcl_NewObjInterface();
    Tcl_ObjInterfaceSetVersion(lgenIfPtr ,1);
    Tcl_ObjInterfaceSetFnListIndex(lgenIfPtr ,lgenSeriesObjIndex);
    Tcl_ObjInterfaceSetFnListLength(lgenIfPtr ,lgenSeriesObjLength);
    Tcl_ObjTypeSetInterface(lgenTypePtr, lgenIfPtr);

    Tcl_CreateObjCommand(interp, "lstring", lLStringObjCmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "lgen", lGenObjCmd, NULL, NULL);
    Tcl_PkgProvide(interp, "abstractlisttest", "1.0.0");

    return TCL_OK;
}
