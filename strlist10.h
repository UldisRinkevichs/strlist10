/*
 *  File        : strlist10.h
 *  Version     : 1.0
 *  Description : Simple and dumb multithread-safe string list / header file
 *  Created on  : Jan 3, 2021
 *  Author      : Uldis Rinkevichs
 */

#pragma once

#ifndef _STRLIST10H_
#define _STRLIST10H_

#include <Windows.h>

typedef struct _USTRLIST_ENTRY {
    struct  _USTRLIST_ENTRY* Next;
    WCHAR  Str[1];
} USTRLIST_ENTRY, * PUSTRLIST_ENTRY;

typedef struct _USTRLIST {
    HANDLE              ListHeap;
    HANDLE              Lock;
    ULONG_PTR           StringsCount;
    PUSTRLIST_ENTRY     Strings;
} USTRLIST, * PUSTRLIST;

BOOL UStrListCreate(PUSTRLIST List);
ULONG_PTR UStrListAdd(PUSTRLIST List, const PWCHAR Str);
ULONG_PTR UStrListInsert(PUSTRLIST List, ULONG_PTR Index, const PWCHAR Str);
PWCHAR UStrListGet(PUSTRLIST List, ULONG_PTR Index);
BOOL UStrListDelete(PUSTRLIST List, ULONG_PTR Index);
BOOL UStrListDestroy(PUSTRLIST List);

#endif // _STRLIST10H_