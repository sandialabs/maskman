/* clang-format off */
/*!
 * \file libKillist.c
 * \brief This contains the top-level libKillist functions.
 *
 * This contains the top-level libKillist functions.
 */
/* clang-format on */

#include "libKillist.h"
#include "libMaskman.h"
#include <float.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* clang-format off */
/*!
 * \def KLLNAME
 * \brief This is a parameter that stores the name metadata for libKillist.
 */
/* clang-format on */
#define KLLNAME "Killist"
/* clang-format off */
/*!
 * \def KLLVERSION
 * \brief This is a parameter that stores the version metadata for libKillist.
 */
/* clang-format on */
#define KLLVERSION "3.1"

/* clang-format off */
/*!
 * \def g_kll_malloc_counter
 * \brief This counter is incremented for each use of malloc and decremented for free.
 */
/* clang-format on */
static size_t g_kll_malloc_counter = 0;

/* clang-format off */
/*! \fn static void *_KLLMalloc(size_t bytes)
 * \brief Wrapper around malloc.
 *
 * This allocates memory and increments the counter for checking.
 */
/* clang-format on */
static void *_KLLMalloc(size_t bytes)
{
    ++g_kll_malloc_counter;
    return malloc(bytes);
}

/* clang-format off */
/*! \fn static void _KLLFree(void *ptr)
 * \brief Wrapper around free.
 *
 * This frees memory and decrements the counter for checking.
 */
/* clang-format on */
static void _KLLFree(void *ptr)
{
    --g_kll_malloc_counter;
    free(ptr);
}

/* clang-format off */
/*! \fn KLLMgr_t * KLLInit(void)
 * \brief Initialize an KLL.
 *
 * This initializes an KLL.
 */
/* clang-format on */
KLLMgr_t *KLLInit(void)
{
    KLLMgr_t *kllmgr = (KLLMgr_t *)_KLLMalloc(sizeof(KLLMgr_t));
    kllmgr->size = 0;
    kllmgr->first = NULL;
    kllmgr->last = NULL;
    return kllmgr;
}

/* clang-format off */
/*! \fn void KLLClear(KLLMgr_t *kllmgr)
 * \brief Clear an KLL.
 *
 * This clears an KLL.
 */
/* clang-format on */
void KLLClear(KLLMgr_t *kllmgr)
{
    while (kllmgr->first != NULL)
    {
        KLLPop(kllmgr);
    }
}

/* clang-format off */
/*! \fn void KLLFinalize(void)
 * \brief Finalize an KLL.
 *
 * This finalizes a KLL.
 */
/* clang-format on */
void KLLFinalize(KLLMgr_t **kllmgr)
{
    KLLClear((*kllmgr));
    _KLLFree((*kllmgr));
    *kllmgr = NULL;
}

/* clang-format off */
/*! \fn KLLNode_t * KLLPush(char *key)
 * \brief Add an entry at the end of an KLL.
 *
 * This adds a zeroed value entry at the end of a KLL.
 */
/* clang-format on */
KLLNode_t *KLLPush(KLLMgr_t *kllmgr, char *key)
{
    /* first, make new node */
    KLLNode_t *m_ptr = (KLLNode_t *)_KLLMalloc(sizeof(KLLNode_t));
    if (m_ptr == NULL)
    {
        fprintf(stderr, "Cannot create new KLLNode!\n");
        return m_ptr;
    }

    /* next, populate new node */
    m_ptr->key = (char *)_KLLMalloc(strlen(key) + 2);
    strcpy(m_ptr->key + 1, key);
    m_ptr->key[0] = 'X';
    m_ptr->ival = 0;
    m_ptr->next = NULL;

    /* finally, put it in place */
    if (kllmgr->size == 0 && kllmgr->first == NULL && kllmgr->last == NULL)
    {
        /* if this is the first entry */
        kllmgr->first = m_ptr;
        kllmgr->last = m_ptr;
    }
    else
    {
        kllmgr->last->next = m_ptr;
        kllmgr->last = m_ptr;
    }
    ++kllmgr->size;
    return m_ptr;
}

/* clang-format off */
/*! \fn KLLNode_t * KLLPushI(KLLMgr_t *kllmgr, char *key, KLLi_t ival)
 * \brief Add an entry at the end of an KLL populated with a signed integer.
 *
 * This adds an integer value entry, and associated key, at the end of a KLL.
 */
/* clang-format on */
KLLNode_t *KLLPushI(KLLMgr_t *kllmgr, char *key, KLLi_t ival)
{
    /* first, push new zeroed node */
    KLLNode_t *m_ptr = KLLPush(kllmgr, key);
    if (m_ptr == NULL)
    {
        return m_ptr;
    }
    m_ptr->key[0] = 'I';
    m_ptr->ival = ival;
    return m_ptr;
}

/* clang-format off */
/*! \fn KLLNode_t * KLLPushU(KLLMgr_t *kllmgr, char *key, KLLu_t uval)
 * \brief Add an entry at the end of an KLL populated with an unsigned integer.
 *
 * This adds an unsigned integer value entry, and associated key, at the end of
 * a KLL.
 */
/* clang-format on */
KLLNode_t *KLLPushU(KLLMgr_t *kllmgr, char *key, KLLu_t uval)
{
    /* first, push new zeroed node */
    KLLNode_t *m_ptr = KLLPush(kllmgr, key);
    if (m_ptr == NULL)
    {
        return m_ptr;
    }
    m_ptr->key[0] = 'U';
    m_ptr->uval = uval;
    return m_ptr;
}

/* clang-format off */
/*! \fn KLLNode_t * KLLPushD(KLLMgr_t *kllmgr, char *key, KLLd_t dval)
 * \brief Add an entry at the end of an KLL populated with a double
 * precision float.
 *
 * This adds a double precision float value entry, and associated key, at the end of
 * a KLL.
 */
/* clang-format on */
KLLNode_t *KLLPushD(KLLMgr_t *kllmgr, char *key, KLLd_t dval)
{
    /* first, push new zeroed node */
    KLLNode_t *m_ptr = KLLPush(kllmgr, key);
    if (m_ptr == NULL)
    {
        return m_ptr;
    }
    m_ptr->key[0] = 'D';
    m_ptr->dval = dval;
    return m_ptr;
}

/* clang-format off */
/*! \fn KLLNode_t * KLLPushS(KLLMgr_t *kllmgr, char *key, KLLs_t sval)
 * \brief Add an entry at the end of an KLL populated with a string.
 *
 * This adds a string value entry, and associated key, at the end of a KLL.
 */
/* clang-format on */
KLLNode_t *KLLPushS(KLLMgr_t *kllmgr, char *key, KLLs_t sval)
{
    /* first, push new zeroed node */
    KLLNode_t *m_ptr = KLLPush(kllmgr, key);
    if (m_ptr == NULL)
    {
        return m_ptr;
    }
    m_ptr->key[0] = 'S';
    /* create out own copy of string */
    m_ptr->sval = (char *)_KLLMalloc(strlen(sval) + 1);
    strcpy(m_ptr->sval, sval);
    return m_ptr;
}

/* clang-format off */
/*! \fn KLLNode_t * KLLPushV(KLLMgr_t *kllmgr, char *key, KLLv_t vval)
 * \brief Add an entry at the end of an KLL populated with a string.
 *
 * This adds a string value entry, and associated key, at the end of a KLL.
 */
/* clang-format on */
KLLNode_t *KLLPushV(KLLMgr_t *kllmgr, char *key, KLLv_t vval)
{
    /* first, push new zeroed node */
    KLLNode_t *m_ptr = KLLPush(kllmgr, key);
    if (m_ptr == NULL)
    {
        return m_ptr;
    }
    m_ptr->key[0] = 'V';
    m_ptr->vval = vval;
    return m_ptr;
}

/* clang-format off */
/*! \fn KLLNode_t *KLLPushM(KLLMgr_t *kllmgr, char *key)
 * \brief Add an entry at the end of an KLL populated with a fresh Maskman.
 *
 * This adds an initialized Maskman entry, and associated key, at the end of a KLL.
 */
/* clang-format on */
KLLNode_t *KLLPushM(KLLMgr_t *kllmgr, char *key, size_t bits)
{
    /* first, push new zeroed node */
    KLLNode_t *m_ptr = KLLPush(kllmgr, key);
    if (m_ptr == NULL)
    {
        return m_ptr;
    }
    m_ptr->key[0] = 'M';
    /* create initialized Maskman */
    m_ptr->mval = (KLLm_t)_KLLMalloc(sizeof(mskmn_t));
    mskmn_init(m_ptr->mval, bits);
    return m_ptr;
}

/* clang-format off */
/*! \fn KLLNode_t * KLLPushK(KLLMgr_t *kllmgr, char *key, KLLMgr_t *kval)
 * \brief Add an entry at the end of an KLL populated with another KLL.
 *
 * This adds a string value entry, and associated key, at the end of a KLL.
 */
/* clang-format on */
KLLNode_t *KLLPushK(KLLMgr_t *kllmgr, char *key, KLLMgr_t *kval)
{
    /* first, push new zeroed node */
    KLLNode_t *m_ptr = KLLPush(kllmgr, key);
    if (m_ptr == NULL)
    {
        return m_ptr;
    }
    m_ptr->key[0] = 'K';
    m_ptr->vval = kval;
    return m_ptr;
}

void _KLLNodeDealloc(KLLNode_t *kllnode)
{
    KLLMgr_t *tmp = NULL;
    if (kllnode == NULL)
    {
        return;
    }
    if (kllnode->key[0] == 'S')
    {
        _KLLFree(kllnode->sval);
        kllnode->sval = NULL;
    }
    else if (kllnode->key[0] == 'V')
    {
        _KLLFree(kllnode->vval);
        kllnode->vval = NULL;
    }
    else if (kllnode->key[0] == 'K')
    {
        tmp = (KLLMgr_t *)kllnode->vval;
        KLLFinalize(&tmp);
    }
    else if (kllnode->key[0] == 'M')
    {
        mskmn_finalize(kllnode->mval);
        _KLLFree(kllnode->mval);
        kllnode->mval = NULL;
    }
    _KLLFree(kllnode->key);
    kllnode->key = NULL;
    _KLLFree(kllnode);
    kllnode = NULL;
}

/* clang-format off */
/*! \fn void KLLPop(KLLMgr_t *kllmgr)
 * \brief Pop first item from list.
 *
 * Pop first item from list.
 */
/* clang-format on */
void KLLPop(KLLMgr_t *kllmgr)
{
    KLLNode_t *kllnode = NULL;
    if (kllmgr->first == NULL)
    {
        return;
    }
    kllnode = kllmgr->first;
    kllmgr->first = kllmgr->first->next;
    _KLLNodeDealloc(kllnode);
    if (kllmgr->first == NULL)
    {
        kllmgr->last = NULL;
    }
    --kllmgr->size;
}

/* clang-format off */
/*! \fn char * KLLName(void)
 * \brief Return the name of the KLL library.
 *
 * This returns the name of the KLL library.
 */
/* clang-format on */
char *KLLName(void)
{
    return KLLNAME;
}

/* clang-format off */
/*! \fn char * KLLVersion(void)
 * \brief Return the version of the KLL library.
 *
 * This returns the version of the KLL library.
 */
/* clang-format on */
char *KLLVersion(void)
{
    return KLLVERSION;
}

void KLLPrint(KLLMgr_t *kllmgr)
{
    KLLNode_t *kllnode = NULL;
    KLLMgr_t *kval = NULL;

    fprintf(stdout, "[KLL Contents]\n");
    fprintf(stdout, "No. Items = %ld\n", kllmgr->size);
    if (kllmgr->first == NULL)
    {
        return;
    }

    kllnode = kllmgr->first;
    while (kllnode != NULL)
    {
        fprintf(stdout, "%s = ", kllnode->key + 1);
        switch (kllnode->key[0])
        {
        case 'I':
            fprintf(stdout, "%" PRIi64 "\n", kllnode->ival);
            break;
        case 'U':
            fprintf(stdout, "%" PRIu64 "\n", kllnode->uval);
            break;
        case 'D':
            fprintf(stdout, "%.16e\n", kllnode->dval);
            break;
        case 'S':
            fprintf(stdout, "%s\n", kllnode->sval);
            break;
        case 'V':
            fprintf(stdout, "Indeterminate Memory Block\n");
            break;
        case 'M':
            fprintf(stdout, "Maskman\n");
            break;
        case 'X':
            fprintf(stdout, "No Value\n");
            break;
        case 'K':
            kval = (KLLMgr_t *)kllnode->vval;
            fprintf(stdout, "KLL with %lu entries\n", kval->size);
            break;
        default:
            fprintf(stderr, "Did not recognize type specifier (%c)!\n", kllnode->key[0]);
        }

        kllnode = kllnode->next;
    }
}

void KLLSelectInit(KLLMgr_t *kllmgr, KLLSelect_t *kllselect)
{
    kllselect->index = 0;
    kllselect->pre = NULL;
    kllselect->at = kllmgr->first;
}

void KLLSelectNext(KLLSelect_t *kllselect)
{
    if (kllselect->at == NULL)
    {
        return;
    }
    ++kllselect->index;
    kllselect->pre = kllselect->at;
    kllselect->at = kllselect->pre->next;
}

void KLLSelectPrev(KLLMgr_t *kllmgr, KLLSelect_t *kllselect)
{
    KLLSelect_t iks, current, given = *kllselect;
    if (kllselect->at == NULL)
    {
        return;
    }
    KLLSelectInit(kllmgr, kllselect);
    KLLSelectInit(kllmgr, &current);
    for (KLLSelectInit(kllmgr, &iks); current.at != given.at && iks.at != NULL; KLLSelectNext(&iks))
    {
        *kllselect = current;
        current = iks;
    }
}

KLLs_t KLLSelectGetKey(KLLSelect_t *kllselect)
{
    char *m_key = NULL;
    if (kllselect->at == NULL)
    {
        return m_key;
    }
    m_key = kllselect->at->key + 1;
    return m_key;
}

size_t KLLSelectGetIndex(KLLSelect_t *kllselect)
{
    size_t m_index = SIZE_MAX;
    if (kllselect->at == NULL)
    {
        return m_index;
    }
    m_index = kllselect->index;
    return m_index;
}

KLLi_t KLLSelectGetI(KLLSelect_t *kllselect)
{
    if (kllselect->at == NULL)
    {
        return INT64_MAX;
    }
    return kllselect->at->ival;
}

KLLu_t KLLSelectGetU(KLLSelect_t *kllselect)
{
    if (kllselect->at == NULL)
    {
        return UINT64_MAX;
    }
    return kllselect->at->uval;
}

KLLd_t KLLSelectGetD(KLLSelect_t *kllselect)
{
    if (kllselect->at == NULL)
    {
        return DBL_MAX;
    }
    return kllselect->at->dval;
}

KLLs_t KLLSelectGetS(KLLSelect_t *kllselect)
{
    if (kllselect->at == NULL)
    {
        return NULL;
    }
    return kllselect->at->sval;
}

KLLv_t KLLSelectGetV(KLLSelect_t *kllselect)
{
    if (kllselect->at == NULL)
    {
        return NULL;
    }
    return kllselect->at->vval;
}

KLLm_t KLLSelectGetM(KLLSelect_t *kllselect)
{
    if (kllselect->at == NULL)
    {
        return NULL;
    }
    return kllselect->at->mval;
}

KLLMgr_t *KLLSelectGetK(KLLSelect_t *kllselect)
{
    KLLMgr_t *kval = NULL;
    if (kllselect->at == NULL)
    {
        return NULL;
    }
    kval = (KLLMgr_t *)kllselect->at->vval;
    return kval;
}

void _KLLSelectRmFirst(KLLMgr_t *kllmgr, KLLSelect_t *kllselect)
{
    KLLPop(kllmgr);
    KLLSelectInit(kllmgr, kllselect);
}

void _KLLSelectRmLast(KLLMgr_t *kllmgr, KLLSelect_t *kllselect)
{
    KLLSelect_t prior, current;

    KLLSelectInit(kllmgr, &prior);
    KLLSelectInit(kllmgr, &current);
    for (KLLSelectInit(kllmgr, kllselect); kllselect->at != NULL; KLLSelectNext(kllselect))
    {
        prior = current;
        current = *kllselect;
    }

    if (kllmgr->first == kllmgr->last)
    {
        /* if the list is only 1 long */
        _KLLSelectRmFirst(kllmgr, kllselect);
    }
    else
    {
        prior.at->next = NULL;
        kllmgr->last = prior.at;
        --kllmgr->size;
        *kllselect = prior;
        _KLLNodeDealloc(current.at);
    }
}

void KLLSelectRm(KLLMgr_t *kllmgr, KLLSelect_t *kllselect)
{
    KLLNode_t *kllnode = kllselect->at; /* temporary storage */

    /* if point doesn't exist, don't do anything */
    if (kllselect->at == NULL)
    {
        return;
    }
    if (kllmgr->first == NULL || kllmgr->last == NULL)
    {
        return;
    }

    /* edge cases */
    if (kllselect->at == kllmgr->first)
    {
        _KLLSelectRmFirst(kllmgr, kllselect);
        return;
    }
    else if (kllselect->at == kllmgr->last)
    {
        _KLLSelectRmLast(kllmgr, kllselect);
        return;
    }

    /* list is at least 3 long and the one specified is not on the ends! */
    kllselect->pre->next = kllselect->at->next;
    kllselect->at = kllselect->pre->next;
    --kllmgr->size;

    /* nuke */
    _KLLNodeDealloc(kllnode);
    kllnode = NULL;
}

KLLSelect_t KLLSelectFindKey(KLLMgr_t *kllmgr, KLLs_t key)
{
    KLLSelect_t kllselect;
    for (KLLSelectInit(kllmgr, &kllselect); kllselect.at != NULL; KLLSelectNext(&kllselect))
    {
        if (strcmp(KLLSelectGetKey(&kllselect), key) == 0)
        {
            break;
        }
    }
    return kllselect;
}

KLLSelect_t KLLSelectFindKeyNext(KLLSelect_t *start, KLLs_t key)
{
    KLLSelect_t kllselect;
    for (kllselect = *start; kllselect.at != NULL; KLLSelectNext(&kllselect))
    {
        if (strcmp(KLLSelectGetKey(&kllselect), key) == 0)
        {
            break;
        }
    }
    return kllselect;
}

KLLSelect_t KLLSelectFindIndex(KLLMgr_t *kllmgr, size_t index)
{
    KLLSelect_t kllselect;
    for (KLLSelectInit(kllmgr, &kllselect); kllselect.at != NULL; KLLSelectNext(&kllselect))
    {
        if (kllselect.index == index)
        {
            break;
        }
    }
    return kllselect;
}

KLLSelect_t KLLSelectFindIndexNext(KLLSelect_t *start, size_t index)
{
    KLLSelect_t kllselect;
    for (kllselect = *start; kllselect.at != NULL; KLLSelectNext(&kllselect))
    {
        if (kllselect.index == index)
        {
            break;
        }
    }
    return kllselect;
}

KLLu_t KLLCountKey(KLLMgr_t *kllmgr, KLLs_t key)
{
    KLLu_t ucnt = 0;
    KLLNode_t *kllnode = NULL;

    if (kllmgr->first == NULL)
    {
        return ucnt;
    }

    kllnode = kllmgr->first;
    while (kllnode != NULL)
    {
        if (strcmp(kllnode->key + 1, key) == 0)
        {
            ++ucnt;
        }
        kllnode = kllnode->next;
    }
    return ucnt;
}

KLLu_t KLLCountI(KLLMgr_t *kllmgr, KLLi_t ival)
{
    KLLu_t ucnt = 0;
    KLLNode_t *kllnode = NULL;

    if (kllmgr->first == NULL)
    {
        return ucnt;
    }

    kllnode = kllmgr->first;
    while (kllnode != NULL)
    {
        if (kllnode->key[0] == 'I')
        {
            if (kllnode->ival == ival)
            {
                ++ucnt;
            }
        }
        kllnode = kllnode->next;
    }
    return ucnt;
}

KLLu_t KLLCountU(KLLMgr_t *kllmgr, KLLu_t uval)
{
    KLLu_t ucnt = 0;
    KLLNode_t *kllnode = NULL;

    if (kllmgr->first == NULL)
    {
        return ucnt;
    }

    kllnode = kllmgr->first;
    while (kllnode != NULL)
    {
        if (kllnode->key[0] == 'U')
        {
            if (kllnode->uval == uval)
            {
                ++ucnt;
            }
        }
        kllnode = kllnode->next;
    }
    return ucnt;
}

KLLu_t KLLCountD(KLLMgr_t *kllmgr, KLLd_t dval)
{
    KLLu_t ucnt = 0;
    KLLNode_t *kllnode = NULL;

    if (kllmgr->first == NULL)
    {
        return ucnt;
    }

    kllnode = kllmgr->first;
    while (kllnode != NULL)
    {
        if (kllnode->key[0] == 'D')
        {
            if (fabs(kllnode->dval - dval) < DBL_EPSILON)
            {
                ++ucnt;
            }
        }
        kllnode = kllnode->next;
    }
    return ucnt;
}

KLLu_t KLLCountS(KLLMgr_t *kllmgr, KLLs_t sval)
{
    KLLu_t ucnt = 0;
    KLLNode_t *kllnode = NULL;

    if (kllmgr->first == NULL)
    {
        return ucnt;
    }

    kllnode = kllmgr->first;
    while (kllnode != NULL)
    {
        if (kllnode->key[0] == 'S')
        {
            if (strcmp(kllnode->sval, sval) == 0)
            {
                ++ucnt;
            }
        }
        kllnode = kllnode->next;
    }
    return ucnt;
}

void KLLPrintMemory(void)
{
    fprintf(stderr, "msg=\"Killist Dangling Malloc\" level=info tag=\"g_kll_malloc_counter\" val=%ld\n",
            g_kll_malloc_counter);
}
