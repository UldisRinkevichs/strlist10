/*
 *  File        : strlist10.c
 *  Version     : 1.0
 *  Description : Simple and dumb multithread-safe string list / implementation
 *  Created on  : Jan 3, 2021
 *  Author      : Uldis Rinkevichs
 */

#include "strlist10.h"

BOOL UStrListCreate(PUSTRLIST List)
{
    if (List == NULL)
        return FALSE;

    List->Lock = CreateMutex(NULL, FALSE, NULL);
    if (List->Lock == NULL)
        return FALSE;

    List->ListHeap = HeapCreate(0, 0, 0);
    if (List->ListHeap == NULL)
    {
        CloseHandle(List->Lock);
        return FALSE;
    }

    List->StringsCount = 0;
    List->Strings = NULL;

    return TRUE;
}

ULONG_PTR UStrListAdd(PUSTRLIST List, const PWCHAR Str)
{
    PUSTRLIST_ENTRY* e;
    SIZE_T          slen;
    ULONG_PTR       result = (ULONG_PTR)-1;

    if ((List == NULL) || (Str == NULL))
        return result;

    while (WAIT_OBJECT_0 == WaitForSingleObjectEx(List->Lock, INFINITE, FALSE))
    {
        slen = lstrlenW(Str);

        for (e = &List->Strings; *e != NULL; e = &(*e)->Next) {};

        *e = (PUSTRLIST_ENTRY)HeapAlloc(List->ListHeap, 0, sizeof(USTRLIST_ENTRY) + (slen * sizeof(WCHAR)));
        if (*e == NULL)
            break;

        (*e)->Next = NULL;
        lstrcpyW((LPWSTR) & (*e)->Str, Str);
        result = List->StringsCount++;

        ReleaseMutex(List->Lock);
        break;
    }

    return result;
}

ULONG_PTR UStrListInsert(PUSTRLIST List, ULONG_PTR Index, const PWCHAR Str)
{
    PUSTRLIST_ENTRY* e, t = NULL;
    SIZE_T          slen;
    ULONG_PTR       result = (ULONG_PTR)-1;

    if ((List == NULL) || (Str == NULL))
        return result;

    while (WAIT_OBJECT_0 == WaitForSingleObjectEx(List->Lock, INFINITE, FALSE))
    {
        slen = lstrlenW(Str);

        for (e = &List->Strings; *e != NULL; e = &(*e)->Next)
        {
            if (Index == 0)
            {
                t = *e;
                break;
            }
            --Index;
        }

        *e = (PUSTRLIST_ENTRY)HeapAlloc(List->ListHeap, 0, sizeof(USTRLIST_ENTRY) + (slen * sizeof(WCHAR)));
        if (*e == NULL)
            break;

        (*e)->Next = t;
        lstrcpyW((LPWSTR) & (*e)->Str, Str);
        result = List->StringsCount++;

        ReleaseMutex(List->Lock);
        break;
    }

    return result;
}

PWCHAR UStrListGet(PUSTRLIST List, ULONG_PTR Index)
{
    PUSTRLIST_ENTRY* e;
    PWCHAR          result = NULL;

    if (List == NULL)
        return NULL;

    while (WAIT_OBJECT_0 == WaitForSingleObjectEx(List->Lock, INFINITE, FALSE))
    {
        for (e = &List->Strings; *e != NULL; e = &(*e)->Next)
        {
            if (Index == 0)
            {
                result = (*e)->Str;
                break;
            }
            --Index;
        }

        ReleaseMutex(List->Lock);
        break;
    }

    return result;
}

BOOL UStrListDelete(PUSTRLIST List, ULONG_PTR Index)
{
    PUSTRLIST_ENTRY* e, t, * prev = NULL;
    BOOL            result = FALSE;

    if (List == NULL)
        return FALSE;

    while (WAIT_OBJECT_0 == WaitForSingleObjectEx(List->Lock, INFINITE, FALSE))
    {
        for (e = &List->Strings; *e != NULL; e = &(*e)->Next)
        {
            if (Index == 0)
            {
                t = (*e)->Next;
                HeapFree(List->ListHeap, 0, *e);

                if (prev == NULL)
                {
                    List->Strings = t;
                }
                else
                {
                    (*prev)->Next = t;
                }

                --List->StringsCount;
                result = TRUE;
                break;
            }
            prev = e;
            --Index;
        }

        ReleaseMutex(List->Lock);
        break;
    }

    return result;
}

BOOL UStrListDestroy(PUSTRLIST List)
{
    if (List == NULL)
        return FALSE;

    if (WAIT_OBJECT_0 == WaitForSingleObjectEx(List->Lock, INFINITE, FALSE))
    {
        HeapDestroy(List->ListHeap);
        CloseHandle(List->Lock);
        RtlSecureZeroMemory(List, sizeof(USTRLIST));
        return TRUE;
    }

    return FALSE;
}