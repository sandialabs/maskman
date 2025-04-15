#include "libLUIS.h"
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

/* only put numbers in here... be weary of double evaluation */
#define LUIS_MAX(a, b) ((a) > (b) ? (a) : (b))
#define LUIS_MIN(a, b) ((a) < (b) ? (a) : (b))

#ifndef LUIS_BUFFER_SIZE
#define LUIS_BUFFER_SIZE 2048
#endif

/* local types */
struct luis_buffer_s
{
    mskmn_t mskmn;
    size_t min;
    size_t max;
};
typedef struct luis_buffer_s luis_buffer_t;

struct luis_node_s
{
    size_t entry;
    size_t valfrom;
    size_t valto;
    size_t valincrement;
    struct luis_node_s *next;
};
typedef struct luis_node_s luis_node_t;

/* local-global vars */
static intmax_t g_dangling_malloc = 0;
static luis_buffer_t g_buffer;
static luis_node_t *g_node_begin = NULL;
static luis_node_t *g_node_end = NULL;
static bool g_verbose = false;
static KLLMgr_t *g_kllmgr = NULL;
/* local declarations */
static void *_luis_malloc(const size_t size);
static void _luis_free(void *ptr);
static void _luis_print_memory(void);
static void _luis_ll_print(void);
static void _luis_buffer_init(const size_t bits);
static void _luis_buffer_reset(void);
static void _luis_buffer_finalize(void);
static void _luis_buffer_set(const size_t bit);
/* static void _luis_buffer_print(void); */
static void _luis_interface(int argc, char *argv[]);
static void _luis_entry_add(void);
static void _luis_manual(char *man);

/* local definitions */

static void *_luis_malloc(const size_t size)
{
    void *ptr = malloc(size);
    ++g_dangling_malloc;
    return ptr;
}

static void _luis_free(void *ptr)
{
    free(ptr);
    --g_dangling_malloc;
}

static void _luis_print_memory(void)
{
    if (g_verbose)
        fprintf(stderr, "msg=\"Method LUIS Dangling Malloc\" level=info tag=\"luis_g_dangling_malloc\" val=%ld\n",
                g_dangling_malloc);
}

static void _luis_buffer_init(const size_t bits)
{
    g_buffer.min = SIZE_MAX;
    g_buffer.max = 0;
    mskmn_init(&g_buffer.mskmn, bits);
}

static void _luis_buffer_reset(void)
{
    g_buffer.min = SIZE_MAX;
    g_buffer.max = 0;
    mskmn_reset(&g_buffer.mskmn);
}

static void _luis_buffer_finalize(void)
{
    g_buffer.min = SIZE_MAX;
    g_buffer.max = 0;
    mskmn_finalize(&g_buffer.mskmn);
}

static void _luis_buffer_set(const size_t bit)
{
    if (bit > g_buffer.mskmn.bits)
    {
        return;
    }
    g_buffer.min = LUIS_MIN(g_buffer.min, bit);
    g_buffer.max = LUIS_MAX(g_buffer.max, bit);
    mskmn_set(&g_buffer.mskmn, bit);
}

/* static void _luis_buffer_print(void) */
/* { */
/*     if(g_verbose) fprintf(stderr, "msg=\"Method LUIS Buffer\" level=info tag=\"min:max\" val_min=%ld, val_max=%ld\n",
 * g_buffer.min, g_buffer.max); */
/* } */

static void _luis_interface(int argc, char *argv[])
{
    /* double scan (set state vars) */
    for (int i0 = 1; i0 < argc; ++i0)
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
    for (int i0 = 1; i0 < argc; ++i0)
    {
        if (strcmp(argv[i0], "--") == 0)
        {
            break;
        }
        if (g_verbose)
            fprintf(stderr, "msg=\"Method LUIS arguments\" level=info tag=\"argv\" index=%d val=\"%s\"\n", i0,
                    argv[i0]);
        _luis_manual(argv[i0]);
    }
    _luis_ll_print();
}

static void _luis_manual(char *man)
{
    /*                          Past                                           Present */
    /*                             |                                           |       */
    size_t m_entry[] = {g_node_end == NULL ? 0 : g_node_end->entry, g_node_end == NULL ? 0 : g_node_end->entry};
    enum parsed_e
    {
        Null = 0,
        Removed,
        Value,
        Range,
        Nextentry
    };
    typedef enum parsed_e parsed_t;

    /*                 Past________________       Present */
    /*                    |    \     \     \      | */
    /*                    0     1     2     3     4 */
    parsed_t found[] = {Null, Null, Null, Null, Null};

    /* https://stackoverflow.com/questions/13399594/how-to-extract-numbers-from-string-in-c */
    char *p = man;
    /*                  Past  Present */
    /*                     |  |       */
    unsigned long val[] = {0, 0};
    while (*p)
    {
        if (isdigit(*p))
        {
            val[0] = val[1];
            val[1] = strtoul(p, &p, 10); /* read a number */
            if (g_verbose)
                fprintf(stderr, "msg=\"Method LUIS value\" level=info tag=\"value\" val=%lu\n", val[1]);
            found[4] = Value;
        }
        else if (*p == ':' && found[3] != Range)
        {
            ++p;
            found[4] = Range;
            if (g_verbose)
                fprintf(stderr, "msg=\"Method LUIS range\" level=info tag=\"range\"\n");
        }
        else if (*p == ';' && found[3] != Nextentry)
        {
            ++p;
            found[4] = Nextentry;
            if (g_verbose)
                fprintf(stderr, "msg=\"Method LUIS nextentry\" level=info tag=\"nextentry\"\n");
        }
        else
        {
            ++p;
            continue;
        }

        /* think about what was received */
        /* luis_node_t *m_node = g_node_end == NULL ? NULL : g_node_end; */
        luis_node_t *m_node = g_node_end;
        switch (found[4])
        {
        case Value:
            if (found[3] != Range)
            {
                /* if you found a value and it's not part of a range, make a new node */
                m_node = (luis_node_t *)_luis_malloc(sizeof(luis_node_t));

                /* attach it to the start or tack it on */
                if (g_node_begin == NULL || g_node_end == NULL)
                {
                    g_node_begin = m_node;
                    g_node_end = m_node;
                }
                else
                {
                    g_node_end->next = m_node;
                    g_node_end = m_node;
                }

                /* populate it */
                m_node->entry = m_entry[1];
                m_node->valfrom = (size_t)val[1];
                m_node->valto = (size_t)val[1];
                m_node->valincrement = 1;
                m_node->next = NULL;
            }
            else if (found[3] == Range && found[2] == Value && found[1] == Range && found[0] == Value)
            {
                /* add the increment */
                m_node->valincrement = (size_t)val[1];
                /* clear for awkwardness */
                found[0] = Removed;
                found[1] = Removed;
                found[2] = Removed;
                found[3] = Removed;
                found[4] = Removed;
            }
            else if (found[3] == Range && found[2] == Value)
            {
                /* add the to */
                m_node->valto = (size_t)val[1];
                size_t m_val_min = LUIS_MIN(m_node->valfrom, m_node->valto);
                size_t m_val_max = LUIS_MAX(m_node->valfrom, m_node->valto);
                m_node->valfrom = m_val_min;
                m_node->valto = m_val_max;
            }
            break;
        case Range:
            if (found[3] == Null || found[3] == Nextentry)
            {
                found[4] = Null;
            }
            break;
        case Nextentry:
            if (m_entry[1] == m_entry[0] && found[3] != Null)
            {
                ++m_entry[1];
            }
            found[0] = Removed;
            found[1] = Removed;
            found[2] = Removed;
            found[3] = Removed;
            break;
        case Null:
            if (g_verbose)
                fprintf(
                    stderr,
                    "msg=\"Method LUIS manual value parsed item that shouldn't occur\" level=error tag=\"value\"\n");
            break;
        case Removed:
            if (g_verbose)
                fprintf(
                    stderr,
                    "msg=\"Method LUIS manual value parsed item that shouldn't occur\" level=error tag=\"value\"\n");
            break;
        }

        /* shift history at end */
        found[0] = found[1];
        found[1] = found[2];
        found[2] = found[3];
        found[3] = found[4];
        m_entry[0] = m_entry[1];
    }
}

static void _luis_ll_print(void)
{
    luis_node_t *ll = g_node_begin;
    luis_node_t *m_node = ll;
    while (m_node != NULL)
    {
        if (g_verbose)
            fprintf(stderr,
                    "msg=\"Method LUIS Node\" level=info tag=\"cli_node\" val_entry=%ld val_from=%ld val_to=%ld "
                    "val_increment=%ld\n",
                    m_node->entry, m_node->valfrom, m_node->valto, m_node->valincrement);
        m_node = m_node->next;
    }
}

static void _luis_entry_add(void)
{
    KLLNode_t *m_killist_node = NULL;
    size_t bytes_copy = (g_buffer.max + 1) % 8 == 0 ? (g_buffer.max + 1) / 8 : ((g_buffer.max + 1) / 8) + 1;
    bytes_copy *= sizeof(mskmnslot_t);
    m_killist_node = KLLPushM(g_kllmgr, "msk", g_buffer.max + 1);
    memcpy(m_killist_node->mval->bitarray, g_buffer.mskmn.bitarray, bytes_copy);
    _luis_buffer_reset();
    /* mskmn_print_binary(m_killist_node->mval); */
    /* mskmn_print_delim(":"); */
    /* mskmn_print_hex(m_killist_node->mval); */
    /* mskmn_print_delim(":"); */
}

/* main */
int luis_main(int argc, char *argv[], KLLMgr_t *kllmgr)
{
    g_kllmgr = kllmgr;           /* put Killist Manager into local-global var */
    _luis_interface(argc, argv); /* parse cli args */

    luis_node_t *m_luis_node = g_node_begin;
    luis_node_t *m_luis_node_next = NULL;
    luis_node_t m_luis_node_history;
    m_luis_node_history.entry = 0;
    m_luis_node_history.valfrom = 0;
    m_luis_node_history.valto = 0;
    m_luis_node_history.valincrement = 0;
    m_luis_node_history.next = NULL;

    _luis_buffer_init(LUIS_BUFFER_SIZE);

    while (m_luis_node != NULL)
    {
        /* check if a new entry needs to be made */
        if (m_luis_node->entry != m_luis_node_history.entry)
        {
            _luis_entry_add();
        }
        for (size_t i0 = m_luis_node->valfrom; i0 <= m_luis_node->valto; i0 += m_luis_node->valincrement)
        {
            _luis_buffer_set(i0);
        }

        /* cycle history */
        m_luis_node_history.entry = m_luis_node->entry;
        m_luis_node_history.valfrom = m_luis_node->valfrom;
        m_luis_node_history.valto = m_luis_node->valto;
        m_luis_node_history.valincrement = m_luis_node->valincrement;

        /* remove node and set next */
        m_luis_node_next = m_luis_node->next;
        _luis_free(m_luis_node);
        m_luis_node = m_luis_node_next;
    }
    _luis_entry_add();

    _luis_buffer_finalize();
    if (g_verbose)
        _luis_print_memory();

    return 0;
}

const char *luis_name(void)
{
    return "luiscli";
}
