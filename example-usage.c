#include <Windows.h>
#include "strlist10.h"

int main()
{
    USTRLIST    testlist;
    PWCHAR      s;

    if (UStrListCreate(&testlist))
    {
        UStrListAdd(&testlist, L"");
        UStrListAdd(&testlist, L"1 Na zolotom krylce sideli");
        UStrListAdd(&testlist, L"2 Donald Trump");
        UStrListAdd(&testlist, L"3 Shirakami Fubiki");
        UStrListAdd(&testlist, L"4 Inugami Korone");
        UStrListAdd(&testlist, L"5 Nekomata Okayu");
        UStrListAdd(&testlist, L"6 Gawr Gura");

        UStrListInsert(&testlist, 6, L"11 Joe Biden");

        UStrListDelete(&testlist, 0);

        s = UStrListGet(&testlist, 2);
        OutputDebugStringW(s);
        UStrListDestroy(&testlist);
    }

    return 0;
}