#include "libSimpleSTDOUT.h"
#include "libKillist.h"
#include "libMaskman.h"
#include <ctype.h>  /* isdigit */
#include <limits.h> /* SIZE_MAX */
#include <stdbool.h>
#include <stddef.h> /* size_t */
#include <stdint.h>
#include <stdio.h>  /* fprintf */
#include <stdlib.h> /* strtoul */
#include <string.h> /* memcpy */

/* local-global vars */
static bool g_verbose = false;
static KLLMgr_t *g_kllmgr = NULL;

/* local definitions */

/* main */
int smplout_main(int argc, char *argv[], KLLMgr_t *kllmgr)
{
    g_kllmgr = kllmgr; /* put Killist Manager into local-global var */
    KLLSelect_t kllselect;
    KLLm_t m_mskmn;

    /* look for relevant command line args */
    for (int i0 = 0; i0 < argc; ++i0)
    {
        if (strcmp(argv[i0], "-v") == 0 || strcmp(argv[i0], "--verbose") == 0)
        {
            g_verbose = true;
        }
        else if (strcmp(argv[i0], "--") == 0)
        {
            break;
        }
    }

    bool after_first = false;
    for (KLLSelectInit(g_kllmgr, &kllselect); kllselect.at != NULL; KLLSelectNext(&kllselect))
    {
        if (after_first)
        {
            mskmn_print_delim(",");
        }
        else
        {
            after_first = true;
        }
        m_mskmn = KLLSelectGetM(&kllselect);
        if (g_verbose)
            mskmn_print_binary(m_mskmn);
        mskmn_print_hex(m_mskmn);
    }

    return 0;
}

const char *smplout_name(void)
{
    return "simplestdout";
}
