#include "libMaskman.h"
#include <inttypes.h> /* for intmax_t */
#include <limits.h>   /* for CHAR_BIT */
#include <stdio.h>    /* for fprintf() */
#include <stdlib.h>   /* for malloc(), free() */
#include <string.h>   /* for memset() */

/* https://c-faq.com/misc/bitsets.html */
#define BITMASK(b) (1 << ((b) % CHAR_BIT))
#define BITSLOT(b) ((b) / CHAR_BIT)
#define BITSET(a, b) ((a)[BITSLOT(b)] |= BITMASK(b))
/* #define BITCLEAR(a, b) ((a)[BITSLOT(b)] &= ~BITMASK(b)) */
/* #define BITTEST(a, b) ((a)[BITSLOT(b)] & BITMASK(b)) */
#define BITNSLOTS(nb) ((nb + CHAR_BIT - 1) / CHAR_BIT)

#define MSKMNSLOT_T_SIZEOF CHAR_BIT

static intmax_t g_dangling_malloc = 0;

/* local declarations */

static void *_mskmn_malloc(const size_t size);
static void _mskmn_free(void *ptr);

/* definitions */

void mskmn_print_memory(void)
{
    fprintf(stderr, "msg=\"libMaskman Dangling Malloc\" level=info tag=\"lib_g_dangling_malloc\" val=%ld\n",
            g_dangling_malloc);
}

static void *_mskmn_malloc(const size_t size)
{
    void *ptr = malloc(size);
    ++g_dangling_malloc;
    return ptr;
}

static void _mskmn_free(void *ptr)
{
    free(ptr);
    --g_dangling_malloc;
}

void mskmn_init(mskmn_t *obj, const size_t bits)
{
    obj->bits = bits;
    obj->size = BITNSLOTS(bits);
    size_t size_mem = obj->size * MSKMNSLOT_T_SIZEOF;
    obj->bitarray = (mskmnslot_t *)_mskmn_malloc(size_mem);
    memset(obj->bitarray, 0, size_mem);
}

void mskmn_reset(mskmn_t *obj)
{
    size_t size_mem = obj->size * MSKMNSLOT_T_SIZEOF;
    memset(obj->bitarray, 0, size_mem);
}

void mskmn_finalize(mskmn_t *obj)
{
    obj->bits = 0;
    obj->size = 0;
    _mskmn_free((void *)obj->bitarray);
    obj->bitarray = NULL;
}

void mskmn_print_delim(const char *const m_delim)
{
    fprintf(stdout, "%s", m_delim);
}

void mskmn_set(mskmn_t *obj, const size_t bit)
{
    if (bit > obj->bits)
    {
        return;
    }
    BITSET(obj->bitarray, bit);
}

void mskmn_print_binary(const mskmn_t *const obj)
{
    size_t i = obj->size;
    size_t j = MSKMNSLOT_T_SIZEOF;
    for (size_t i0 = obj->size; i0 > 0; --i0)
    {
        --i;
        j = MSKMNSLOT_T_SIZEOF;
        for (size_t j0 = MSKMNSLOT_T_SIZEOF; j0 > 0; --j0)
        {
            --j;
            fprintf(stderr, "%d", (obj->bitarray[i] >> j) & 1);
        }
        fprintf(stderr, " ");
    }
    fprintf(stderr, "\n");
}

void mskmn_print_hex(const mskmn_t *const obj)
{
    const size_t num_char_per_slot = MSKMNSLOT_T_SIZEOF % 4 == 0 ? MSKMNSLOT_T_SIZEOF / 4 : MSKMNSLOT_T_SIZEOF / 4 + 1;

    char fmtn[6];
    memset(fmtn, 0, sizeof(fmtn)); /* null termination paranoia */
    snprintf(fmtn, sizeof(fmtn) / sizeof(char), "%s0%lux", "%", num_char_per_slot);

    fprintf(stdout, "0x");
    size_t i = obj->size;
    for (size_t i0 = obj->size; i0 > 0; --i0)
    {
        --i;
        fprintf(stdout, fmtn, obj->bitarray[i]);
    }
}
