#include "ff.h"


WCHAR ff_convert (WCHAR wch, UINT dir)
{
    if (wch < 0x80)
    {
        /* ASCII Char */
        return wch;
    }

    /* I don't support unicode it is too big! */
    return 0;
}

WCHAR ff_wtoupper (WCHAR wch)
{
    if (wch < 0x80)
    {
        /* ASCII Char */
        if (wch >= 'a' && wch <= 'z')
        {
            wch &= ~0x20;
        }

        return wch;
    }

    /* I don't support unicode it is too big! */
    return 0;
}
