/*
 * Copyright © 2024 Nathan Coulter.  All rights reserved.
 *
 */

/*
 * You may distribute and/or modify this program under the terms of the GNU
 * Affero General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.

 * See the file "COPYING" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
*/

/*
 *----------------------------------------------------------------------
 * Tcl_NewObjInterface
 *----------------------------------------------------------------------
 */

#include "tcl.h"
#include "tclInt.h"


Tcl_ObjInterface *
Tcl_NewObjInterface() {
	ObjInterface * ifacePtr;
	ifacePtr = (ObjInterface *)Tcl_Alloc(sizeof(ObjInterface));
	memset(ifacePtr ,0 ,sizeof(ObjInterface));
	return (Tcl_ObjInterface *)ifacePtr;
}

Tcl_ObjType *
Tcl_NewObjType(
) {
	ObjectType *objTypePtr;
	objTypePtr = (ObjectType *)Tcl_Alloc(sizeof(ObjectType));
	return (Tcl_ObjType *)objTypePtr;
}


int
Tcl_ObjInterfaceSetFnListAll(
	Tcl_ObjInterface *objInterfacePtr
	,int (fnPtr)(tclObjTypeInterfaceArgsListAll)
) {
	ObjInterface *oiPtr = (ObjInterface *)objInterfacePtr;
	oiPtr->list.all = fnPtr;
	return TCL_OK;
}


int
Tcl_ObjInterfaceSetFnListAppend(
	Tcl_ObjInterface *objInterfacePtr
	,int (fnPtr)(tclObjTypeInterfaceArgsListAppend)
) {
	ObjInterface *oiPtr = (ObjInterface *)objInterfacePtr;
	oiPtr->list.append = fnPtr;
	return TCL_OK;
}


int
Tcl_ObjInterfaceSetFnListAppendList(
	Tcl_ObjInterface *objInterfacePtr
	,int (fnPtr)(tclObjTypeInterfaceArgsListAppendList)
) {
	ObjInterface *oiPtr = (ObjInterface *)objInterfacePtr;
	oiPtr->list.appendlist = fnPtr;
	return TCL_OK;
}


int
Tcl_ObjInterfaceSetFnListContains(
	Tcl_ObjInterface *objInterfacePtr
	,int (fnPtr)(tclObjTypeInterfaceArgsListContains)
) {
	ObjInterface *oiPtr = (ObjInterface *)objInterfacePtr;
	oiPtr->list.contains = fnPtr;
	return TCL_OK;
}


int
Tcl_ObjInterfaceSetFnListIndex(
	Tcl_ObjInterface *objInterfacePtr
	,int (fnPtr)(tclObjTypeInterfaceArgsListIndex)
) {
	ObjInterface *oiPtr = (ObjInterface *)objInterfacePtr;
	oiPtr->list.index = fnPtr;
	return TCL_OK;
}


int
Tcl_ObjInterfaceSetFnListIndexEnd(
	Tcl_ObjInterface *objInterfacePtr
	,int (fnPtr)(tclObjTypeInterfaceArgsListIndexEnd)
) {
	ObjInterface *oiPtr = (ObjInterface *)objInterfacePtr;
	oiPtr->list.indexEnd = fnPtr;
	return TCL_OK;
}


int
Tcl_ObjInterfaceSetFnListIsSorted(
	Tcl_ObjInterface *objInterfacePtr
	,int (fnPtr)(tclObjTypeInterfaceArgsListIsSorted)
) {
	ObjInterface *oiPtr = (ObjInterface *)objInterfacePtr;
	oiPtr->list.isSorted = fnPtr;
	return TCL_OK;
}


int
Tcl_ObjInterfaceSetFnListLength(
	Tcl_ObjInterface *objInterfacePtr
	,int (fnPtr)(tclObjTypeInterfaceArgsListLength)
) {
	ObjInterface *oiPtr = (ObjInterface *)objInterfacePtr;
	oiPtr->list.length = fnPtr;
	return TCL_OK;
}


int
Tcl_ObjInterfaceSetFnListRange(
	Tcl_ObjInterface *objInterfacePtr
	,Tcl_Obj *(fnPtr)(tclObjTypeInterfaceArgsListRange)
) {
	ObjInterface *oiPtr = (ObjInterface *)objInterfacePtr;
	oiPtr->list.range = fnPtr;
	return TCL_OK;
}


int
Tcl_ObjInterfaceSetFnListRangeEnd(
	Tcl_ObjInterface *objInterfacePtr
	,Tcl_Obj *(fnPtr)(tclObjTypeInterfaceArgsListRangeEnd)
) {
	ObjInterface *oiPtr = (ObjInterface *)objInterfacePtr;
	oiPtr->list.rangeEnd = fnPtr;
	return TCL_OK;
}


int
Tcl_ObjInterfaceSetFnListReplace(
	Tcl_ObjInterface *objInterfacePtr
	,int (fnPtr)(tclObjTypeInterfaceArgsListReplace)
) {
	ObjInterface *oiPtr = (ObjInterface *)objInterfacePtr;
	oiPtr->list.replace = fnPtr;
	return TCL_OK;
}

int Tcl_ObjInterfaceSetFnListReplaceList(
	Tcl_ObjInterface *objInterfacePtr
	,int (fnPtr)(tclObjTypeInterfaceArgsListReplaceList)
) {
	ObjInterface *oiPtr = (ObjInterface *)objInterfacePtr;
	oiPtr->list.replaceList = fnPtr;
	return TCL_OK;
}


int
Tcl_ObjInterfaceSetFnListReverse(
	Tcl_ObjInterface *objInterfacePtr
	,int (fnPtr)(tclObjTypeInterfaceArgsListReverse)
) {
	ObjInterface *oiPtr = (ObjInterface *)objInterfacePtr;
	oiPtr->list.reverse = fnPtr;
	return TCL_OK;
}

int
Tcl_ObjInterfaceSetFnListSet(
	Tcl_ObjInterface *objInterfacePtr
	,Tcl_Obj *(fnPtr)(tclObjTypeInterfaceArgsListSetList)
) {
	ObjInterface *oiPtr = (ObjInterface *)objInterfacePtr;
	oiPtr->list.setlist = fnPtr;
	return TCL_OK;
}


int
Tcl_ObjInterfaceSetFnListSetFlat(
	Tcl_ObjInterface *objInterfacePtr
	,int (fnPtr)(tclObjTypeInterfaceArgsListSet)
) {
	ObjInterface *oiPtr = (ObjInterface *)objInterfacePtr;
	oiPtr->list.set = fnPtr;
	return TCL_OK;
}


int
Tcl_ObjInterfaceSetFnStringIndex(
	Tcl_ObjInterface *objInterfacePtr
	,int (fnPtr) (tclObjTypeInterfaceArgsStringIndex)
) {
	ObjInterface *oiPtr = (ObjInterface *)objInterfacePtr;
	oiPtr->string.index = fnPtr;
	return TCL_OK;
}


int
Tcl_ObjInterfaceSetFnStringIndexEnd(
	Tcl_ObjInterface *objInterfacePtr
	,int (fnPtr) (tclObjTypeInterfaceArgsListIndexEnd)
) {
	ObjInterface *oiPtr = (ObjInterface *)objInterfacePtr;
	oiPtr->string.indexEnd = fnPtr;
	return TCL_OK;
}


int
Tcl_ObjInterfaceSetFnStringIsEmpty(
    Tcl_ObjInterface *objInterfacePtr
    ,int (fnPtr) (tclObjTypeInterfaceArgsStringIsEmpty)
) {
    ObjInterface *oiPtr = (ObjInterface *)objInterfacePtr;
    oiPtr->string.isEmpty = fnPtr;
    return TCL_OK;
}


int
Tcl_ObjInterfaceSetFnStringLength(
	Tcl_ObjInterface *objInterfacePtr
	,Tcl_Size (fnPtr) (tclObjTypeInterfaceArgsStringLength)
) {
	ObjInterface *oiPtr = (ObjInterface *)objInterfacePtr;
	oiPtr->string.length = fnPtr;
	return TCL_OK;
}


int
Tcl_ObjInterfaceSetFnStringRange(
	Tcl_ObjInterface *objInterfacePtr
	,Tcl_Obj *(fnPtr) (tclObjTypeInterfaceArgsStringRange)
) {
	ObjInterface *oiPtr = (ObjInterface *)objInterfacePtr;
	oiPtr->string.range = fnPtr;
	return TCL_OK;
}


int
Tcl_ObjInterfaceSetFnStringRangeEnd(
	Tcl_ObjInterface *objInterfacePtr
	,Tcl_Obj *(fnPtr) (tclObjTypeInterfaceArgsStringRangeEnd)
) {
	ObjInterface *oiPtr = (ObjInterface *)objInterfacePtr;
	oiPtr->string.rangeEnd = fnPtr;
	return TCL_OK;
}


int
Tcl_ObjInterfaceSetVersion(
	Tcl_ObjInterface *objInterfacePtr
	,int version
) {
	ObjInterface *oiPtr = (ObjInterface *)objInterfacePtr;
	oiPtr->version = version;
	return TCL_OK;
}


int
Tcl_ObjTypeSetFreeInternalRepProc(
	Tcl_ObjType *otPtr
    ,Tcl_FreeInternalRepProc *freeIntRepProc
) {
    otPtr->freeIntRepProc = freeIntRepProc;
	return TCL_OK;
}


int
Tcl_ObjTypeSetDupInternalRepProc(
	Tcl_ObjType *otPtr
    ,Tcl_DupInternalRepProc *dupIntRepProc
) {
    otPtr->dupIntRepProc = dupIntRepProc;
	return TCL_OK;
}


int
Tcl_ObjTypeSetInterface(
	Tcl_ObjType *objTypePtr
	,Tcl_ObjInterface * objInterfacePtr
) {

	ObjectType *otPtr = (ObjectType *)objTypePtr;
	otPtr->ifPtr = objInterfacePtr;
	return TCL_OK;
}


int
Tcl_ObjTypeSetUpdateStringProc(
	Tcl_ObjType *otPtr
    ,Tcl_UpdateStringProc *updateStringProc
) {
    otPtr->updateStringProc = updateStringProc;
	return TCL_OK;
}


int
Tcl_ObjTypeSetSetFromAnyProc(
	Tcl_ObjType *otPtr
    ,Tcl_SetFromAnyProc *setFromAnyProc
) {
    otPtr->setFromAnyProc = setFromAnyProc;
	return TCL_OK;
}


int
Tcl_ObjTypeSetName(
	Tcl_ObjType *otPtr
	,char *name
) {
    otPtr->name = name;
	return TCL_OK;
}


int
Tcl_ObjTypeSetVersion(
	Tcl_ObjType *otPtr
    ,int version
) {
    otPtr->version = version;
	return TCL_OK;
}
