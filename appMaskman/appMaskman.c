#include "libKillist.h" /* Killist */
#include "libLUIS.h"    /* LUIS & luis_* */
#include "libMaskman.h" /* libMaskman */
#include "libMaskman.h"
#include "libSimpleSTDOUT.h" /* SimpleSTDOUT & smpout_* */
#include <stdbool.h>         /* bool */
#include <stddef.h>          /* size_t */
#include <string.h>          /* memset, strcmp */

/* BEGIN MANAGE Input & Output METHODS */
/*   First, Add entry to enumeration */
/*     NOTE: 1st value must start at 0, entries must be sequential, and "imethod_size" must be last */
enum imethod_e
{
    luiscli = 0,
    imethod_size
};
typedef enum imethod_e imethod_t;

enum omethod_e
{
    simplestdout = 0,
    omethod_size
};
typedef enum omethod_e omethod_t;

typedef int (*mskmncli_funcptrs_t)(int, char **, KLLMgr_t *);
typedef const char *(*mskmncli_nameptrs_t)(void);
void _mskmncli_imethod_init(bool *is_imethod, mskmncli_funcptrs_t *funcptrs, mskmncli_nameptrs_t *nameptrs);
void _mskmncli_omethod_init(bool *is_imethod, mskmncli_funcptrs_t *funcptrs, mskmncli_nameptrs_t *nameptrs);

/*   Second, assign appropriate function pointers for execution and the name */
void _mskmncli_imethod_init(bool *is_imethod, mskmncli_funcptrs_t *funcptrs, mskmncli_nameptrs_t *nameptrs)
{
    memset(is_imethod, 0, sizeof(bool) * imethod_size); /* make all false initially */
    funcptrs[luiscli] = luis_main;                      /* set and ensure this ties back to enum */
    nameptrs[luiscli] = luis_name;                      /* set and ensure this ties back to enum */
}

void _mskmncli_omethod_init(bool *is_imethod, mskmncli_funcptrs_t *funcptrs, mskmncli_nameptrs_t *nameptrs)
{
    memset(is_imethod, 0, sizeof(bool) * omethod_size); /* make all false initially */
    funcptrs[simplestdout] = smplout_main;              /* set and ensure this ties back to enum */
    nameptrs[simplestdout] = smplout_name;              /* set and ensure this ties back to enum */
}
/* END MANAGE Input & Output METHODS */

static bool g_verbose = false;

int main(int argc, char *argv[])
{
    int code_return = 0;          /* accumulation return code */
    KLLMgr_t *kllmgr = KLLInit(); /* put Maskman entries herein */
    bool imethod_bools[imethod_size];
    bool omethod_bools[omethod_size];
    mskmncli_funcptrs_t imethod_funcptrs[imethod_size];
    mskmncli_nameptrs_t imethod_nameptrs[imethod_size];
    mskmncli_funcptrs_t omethod_funcptrs[omethod_size];
    mskmncli_nameptrs_t omethod_nameptrs[omethod_size];

    /* initialize method containers */
    _mskmncli_imethod_init(imethod_bools, imethod_funcptrs, imethod_nameptrs);
    _mskmncli_omethod_init(omethod_bools, omethod_funcptrs, omethod_nameptrs);

    /* LIGHTLY PARSE PRELIMINARILY RELEVANT ARGS WITH INITIAL PASS */
    /*   input methods */
    bool is_imethod_selected = false;
    for (int i0 = 1; i0 < argc; ++i0)
    {
        if (strcmp(argv[i0], "--") == 0)
        {
            break;
        }
        for (int i1 = 0; i1 < imethod_size; ++i1)
        {
            if (strcmp(argv[i0], imethod_nameptrs[i1]()) == 0)
            {
                imethod_bools[i1] = true;
                is_imethod_selected = true;
                break;
            }
        }
    }
    if (!is_imethod_selected)
    {
        imethod_bools[luiscli] = true;
    }

    /*   output methods */
    bool is_omethod_selected = false;
    for (int i0 = 1; i0 < argc; ++i0)
    {
        if (strcmp(argv[i0], "--") == 0)
        {
            break;
        }
        for (int i1 = 0; i1 < omethod_size; ++i1)
        {
            if (strcmp(argv[i0], omethod_nameptrs[i1]()) == 0)
            {
                omethod_bools[i1] = true;
                is_omethod_selected = true;
                break;
            }
        }
    }
    if (!is_omethod_selected)
    {
        omethod_bools[simplestdout] = true;
    }

    /*   CLI-relevant options */
    for (int i0 = 1; i0 < argc; ++i0)
    {
        if (strcmp(argv[i0], "--") == 0)
        {
            break;
        }
        else if (strcmp(argv[i0], "-v") == 0 || strcmp(argv[i0], "--verbose") == 0)
        {
            g_verbose = true;
        }
    }

    /*   help */
    for (int i0 = 1; i0 < argc; ++i0)
    {
        if (strcmp(argv[i0], "--") == 0)
        {
            break;
        }
        else if (strcmp(argv[i0], "-h") == 0 || strcmp(argv[i0], "--help") == 0)
        {
            fprintf(stdout, "Please refer to the Maskman documentation.\n"
                            "\n"
                            "- Unix Manual :: =man maskman=\n"
                            "- GNU Info :: =info maskman=\n"
                            "\n\n"
                            "* Current Settings for Methods\n"
                            "\n");

            fprintf(stdout, "** Input Methods\n");
            for (int i1 = 0; i1 < imethod_size; ++i1)
            {
                fprintf(stdout, "- %s :: %s\n", imethod_nameptrs[i1](), imethod_bools[i1] ? "On" : "Off");
            }

            fprintf(stdout, "\n"
                            "** Output Methods\n");
            for (int i1 = 0; i1 < omethod_size; ++i1)
            {
                fprintf(stdout, "- %s :: %s\n", omethod_nameptrs[i1](), omethod_bools[i1] ? "On" : "Off");
            }

            goto graceful_exit;
        }
    }

    /* EXECUTE */
    /*   input methods */
    for (size_t i0 = 0; i0 < imethod_size; ++i0)
    {
        code_return += imethod_bools[i0] ? imethod_funcptrs[i0](argc, argv, kllmgr) : 0;
    }

    /* PRINT OUT RESULTS */
    /*   output methods */
    for (size_t i0 = 0; i0 < omethod_size; ++i0)
    {
        code_return += omethod_bools[i0] ? omethod_funcptrs[i0](argc, argv, kllmgr) : 0;
    }

    /* EXIT GRACEFULLY */
graceful_exit:
    KLLFinalize(&kllmgr); /* free all memory */
    if (g_verbose)
        KLLPrintMemory(); /* Killist */
    if (g_verbose)
        mskmn_print_memory(); /* Maskman */
    return code_return;
}
