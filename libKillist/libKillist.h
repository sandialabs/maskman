/* clang-format off */
/*!
 * \file libKillist.h
 * \brief This contains the external-facing functions for libKillist.
 *
 * This contains the external-facing functions for libKillist.
 */
/* clang-format on */

#ifndef LIBKILLIST
#define LIBKILLIST

#include "libMaskman.h"
#include <stdint.h>
#include <stdio.h>

typedef int64_t KLLi_t;
typedef uint64_t KLLu_t;
typedef double KLLd_t;
typedef char *KLLs_t;
typedef void *KLLv_t;
typedef mskmn_t *KLLm_t;
struct KLLNode_s /* tuning size for x86_64 */
{
    char *key; /* 64 bits */
    union
    {
        KLLi_t ival; /* 64 bits */
        KLLu_t uval; /* 64 bits */
        KLLd_t dval; /* 64 bits */
        KLLs_t sval; /* 64 bits */
        KLLv_t vval; /* 64 bits */
        KLLm_t mval; /* 64 bits */
    };
    struct KLLNode_s *next; /* 64 bits */
};
typedef struct KLLNode_s KLLNode_t; /* 192 bits (24 bytes) */

struct KLLMgr_s
{
    size_t size;
    KLLNode_t *first;
    KLLNode_t *last;
};
typedef struct KLLMgr_s KLLMgr_t;

struct KLLSelect_s
{
    size_t index;
    KLLNode_t *pre;
    KLLNode_t *at;
};
typedef struct KLLSelect_s KLLSelect_t;

KLLMgr_t *KLLInit(void);
void KLLClear(KLLMgr_t *kllmgr);
void KLLFinalize(KLLMgr_t **kllmgr);
void KLLPrintMemory(void);

void KLLPop(KLLMgr_t *kllmgr);
KLLNode_t *KLLPush(KLLMgr_t *kllmgr, char *key);
KLLNode_t *KLLPushI(KLLMgr_t *kllmgr, char *key, KLLi_t ival);
KLLNode_t *KLLPushU(KLLMgr_t *kllmgr, char *key, KLLu_t uval);
KLLNode_t *KLLPushD(KLLMgr_t *kllmgr, char *key, KLLd_t dval);
KLLNode_t *KLLPushS(KLLMgr_t *kllmgr, char *key, KLLs_t sval);
KLLNode_t *KLLPushV(KLLMgr_t *kllmgr, char *key, KLLv_t vval);
KLLNode_t *KLLPushM(KLLMgr_t *kllmgr, char *key, size_t bits);
KLLNode_t *KLLPushK(KLLMgr_t *kllmgr, char *key, KLLMgr_t *kval);

KLLu_t KLLCountKey(KLLMgr_t *kllmgr, KLLs_t key);
KLLu_t KLLCountI(KLLMgr_t *kllmgr, KLLi_t ival);
KLLu_t KLLCountU(KLLMgr_t *kllmgr, KLLu_t uval);
KLLu_t KLLCountD(KLLMgr_t *kllmgr, KLLd_t dval);
KLLu_t KLLCountS(KLLMgr_t *kllmgr, KLLs_t sval);

void KLLSelectInit(KLLMgr_t *kllmgr, KLLSelect_t *kllselect);
void KLLSelectRm(KLLMgr_t *kllmgr, KLLSelect_t *kllselect);
void KLLSelectNext(KLLSelect_t *kllselect);
void KLLSelectPrev(KLLMgr_t *kllmgr, KLLSelect_t *kllselect);
KLLs_t KLLSelectGetKey(KLLSelect_t *kllselect);
size_t KLLSelectGetIndex(KLLSelect_t *kllselect);
KLLi_t KLLSelectGetI(KLLSelect_t *kllselect);
KLLu_t KLLSelectGetU(KLLSelect_t *kllselect);
KLLd_t KLLSelectGetD(KLLSelect_t *kllselect);
KLLs_t KLLSelectGetS(KLLSelect_t *kllselect);
KLLv_t KLLSelectGetV(KLLSelect_t *kllselect);
KLLm_t KLLSelectGetM(KLLSelect_t *kllselect);
KLLMgr_t *KLLSelectGetK(KLLSelect_t *kllselect);
KLLSelect_t KLLSelectFindKey(KLLMgr_t *kllmgr, KLLs_t key);
KLLSelect_t KLLSelectFindKeyNext(KLLSelect_t *start, KLLs_t key);
KLLSelect_t KLLSelectFindIndex(KLLMgr_t *kllmgr, size_t index);
KLLSelect_t KLLSelectFindIndexNext(KLLSelect_t *start, size_t index);

void KLLPrint(KLLMgr_t *kllmgr);
char *KLLName(void);
char *KLLVersion(void);

#endif
