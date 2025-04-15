#include "libKillist.h"
#include "libMaskman.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int buildup(void);
int buildups(void);
int selectiter(void);
int selectrm(void);
int selectprepost(void);
int selectfind(void);
int count(void);
int inception(void);

int buildup(void)
{
    int retcode = 0;
    KLLMgr_t *kllmgr = KLLInit();
    void *vval = malloc(42);

    fprintf(stdout, "# Test buildup\n");

    KLLPushD(kllmgr, "The", 42.42);
    KLLPushI(kllmgr, "System", -42);
    KLLPushS(kllmgr, "Is", "of a Down");
    KLLPushU(kllmgr, "Down", 42);
    KLLPushV(kllmgr, "!", vval);
    KLLPushM(kllmgr, "end", 224);

    KLLPrint(kllmgr);

    KLLFinalize(&kllmgr);
    if (kllmgr != NULL)
    {
        fprintf(stderr, "KLLMGR not NULL after finalize!\n");
        ++retcode;
    }

    if (retcode == 0)
    {
        fprintf(stdout, "## Test buildup Successful\n");
    }
    return retcode;
}

int buildups(void)
{
    int retcode = 0;
    KLLMgr_t *kllmgr = NULL;
    size_t max0 = 9, max1 = 9, max2 = 999;
    /* size_t length = snprintf(NULL, 0, "%lu", max2) + 1; */
    /* char buf[length]; */
    KLLu_t i0, i1, i2;

    fprintf(stdout, "# Test buildups\n");

    for (i0 = 0; i0 < max0; ++i0)
    {
        kllmgr = KLLInit();
        for (i1 = 0; i1 < max1; ++i1)
        {
            KLLClear(kllmgr);
            for (i2 = 0; i2 < max2; ++i2)
            {
                KLLPushU(kllmgr, "foo", i2);
            }
        }
        KLLFinalize(&kllmgr);
    }

    if (retcode == 0)
    {
        fprintf(stdout, "## Test buildups Successful\n");
    }
    return retcode;
}

int selectiter(void)
{
    int retcode = 0;
    KLLMgr_t *kllmgr = NULL;
    size_t icnt, icntmax = 999;
    size_t gold0 = 0, test0 = 0;
    KLLu_t gold1 = 0, test1 = 0;
    KLLi_t gold2 = 0, test2 = 0;
    KLLd_t gold3 = 0.0, test3 = 0.0;
    KLLu_t gold4 = 0, test4 = 0;
    KLLs_t tmp4, val4 = "foobar";
    KLLu_t gold5 = 0, test5 = 0;
    KLLs_t tmp5, val5 = "baz";
    KLLu_t gold6 = 0, test6 = 0;
    size_t icnt6, icntmax6 = 9;
    KLLu_t *val6 = NULL;
    mskmn_t gold7;
    KLLm_t tmp7;
    size_t test7 = 0;
    mskmn_init(&gold7, 224);
    KLLSelect_t kllselect;

    fprintf(stdout, "# Test selectiter\n");

    kllmgr = KLLInit();

    /* index */
    for (icnt = 0; icnt < icntmax; ++icnt)
    {
        gold0 += icnt; /* index for later */
        KLLPushU(kllmgr, "fo", (KLLu_t)icnt);
    }
    for (KLLSelectInit(kllmgr, &kllselect); kllselect.at != NULL; KLLSelectNext(&kllselect))
    {
        test0 += KLLSelectGetIndex(&kllselect);
    }
    KLLClear(kllmgr);

    /* U */
    for (icnt = 0; icnt < icntmax; ++icnt)
    {
        gold1 += (KLLu_t)icnt + 42;
        KLLPushU(kllmgr, "fo", (KLLu_t)icnt + 42);
    }

    for (KLLSelectInit(kllmgr, &kllselect); kllselect.at != NULL; KLLSelectNext(&kllselect))
    {
        test1 += KLLSelectGetU(&kllselect);
    }
    KLLClear(kllmgr);

    /* I */
    for (icnt = 0; icnt < icntmax; ++icnt)
    {
        gold2 += -1 * (KLLi_t)icnt - 42;
        KLLPushI(kllmgr, "fo", -1 * (KLLi_t)icnt - 42);
    }
    for (KLLSelectInit(kllmgr, &kllselect); kllselect.at != NULL; KLLSelectNext(&kllselect))
    {
        test2 += KLLSelectGetI(&kllselect);
    }
    KLLClear(kllmgr);

    /* D */
    for (icnt = 0; icnt < icntmax; ++icnt)
    {
        test3 = (KLLd_t)icnt * 1.42;
        gold3 += test3;
        KLLPushD(kllmgr, "fo", test3);
    }
    test3 = 0.0;
    for (KLLSelectInit(kllmgr, &kllselect); kllselect.at != NULL; KLLSelectNext(&kllselect))
    {
        test3 += KLLSelectGetD(&kllselect);
    }
    KLLClear(kllmgr);

    /* S */
    for (icnt = 0; icnt < icntmax; ++icnt)
    {
        ++gold4;
        KLLPushS(kllmgr, "fo", val4);
    }
    for (KLLSelectInit(kllmgr, &kllselect); kllselect.at != NULL; KLLSelectNext(&kllselect))
    {
        tmp4 = KLLSelectGetS(&kllselect);
        if (strcmp(tmp4, val4) == 0)
        {
            ++test4;
        }
    }
    KLLClear(kllmgr);

    /* key */
    for (icnt = 0; icnt < icntmax; ++icnt)
    {
        ++gold5;
        KLLPushU(kllmgr, val5, (KLLu_t)icnt);
    }
    for (KLLSelectInit(kllmgr, &kllselect); kllselect.at != NULL; KLLSelectNext(&kllselect))
    {
        tmp5 = KLLSelectGetKey(&kllselect);
        if (strcmp(tmp5, val5) == 0)
        {
            ++test5;
        }
    }
    KLLClear(kllmgr);

    /* scary V */
    for (icnt = 0; icnt < icntmax; ++icnt)
    {
        val6 = (KLLu_t *)malloc(icntmax6 * sizeof(KLLu_t));
        for (icnt6 = 0; icnt6 < icntmax6; ++icnt6)
        {
            val6[icnt6] = icnt * icntmax6 + icnt6;
            gold6 += val6[icnt6];
        }
        KLLPushV(kllmgr, "fo", val6);
    }
    for (KLLSelectInit(kllmgr, &kllselect); kllselect.at != NULL; KLLSelectNext(&kllselect))
    {
        val6 = (KLLu_t *)KLLSelectGetV(&kllselect);
        for (icnt6 = 0; icnt6 < icntmax6; ++icnt6)
        {
            test6 += val6[icnt6];
        }
    }
    KLLClear(kllmgr);

    /* M */
    test7 = 0;
    for (icnt = 0; icnt < icntmax; ++icnt)
    {
        KLLPushM(kllmgr, "fo", 224);
    }
    for (KLLSelectInit(kllmgr, &kllselect); kllselect.at != NULL; KLLSelectNext(&kllselect))
    {
        tmp7 = KLLSelectGetM(&kllselect);
        if (tmp7->bitarray[0] != gold7.bitarray[0] || tmp7->bits != gold7.bits || tmp7->size != gold7.size)
        {
            ++test7;
            fprintf(stderr,
                    "msg=\"selectiter M bitarrays [0]\" level=error tag=\"selectiter_M_bitarrays_0\" "
                    "val_tmp7_bitarray_0=%c val_gold7_bitarray=%c\n",
                    tmp7->bitarray[0], gold7.bitarray[0]);
            fprintf(stderr,
                    "msg=\"selectiter M bits\" level=error tag=\"selectiter_M_bits\" val_tmp7_bits=%zu "
                    "val_gold7_bits=%zu\n",
                    tmp7->bits, gold7.bits);
            fprintf(stderr,
                    "msg=\"selectiter M size\" level=error tag=\"selectiter_M_size\" val_tmp7_size=%zu "
                    "val_gold7_size=%zu\n",
                    tmp7->size, gold7.size);
        }
    }
    mskmn_finalize(&gold7);
    KLLClear(kllmgr);

    /* check 'em */
    if (test0 != gold0)
    {
        fprintf(stderr, "Test value 0 (%lu) not gold (%lu)!\n", test0, gold0);
        ++retcode;
    }
    if (test1 != gold1)
    {
        fprintf(stderr, "Test value 1 (%" PRIu64 ") not gold (%" PRIu64 ")!\n", test1, gold1);
        ++retcode;
    }
    if (test2 != gold2)
    {
        fprintf(stderr, "Test value 2 (%" PRIi64 ") not gold (%" PRIi64 ")!\n", test2, gold2);
        ++retcode;
    }
    if (test3 != gold3)
    {
        fprintf(stderr, "Test value 3 (%.16e) not gold (%.16e)!\n", test3, gold3);
        ++retcode;
    }
    if (test4 != gold4)
    {
        fprintf(stderr, "Test value 4 (%" PRIu64 ") not gold (%" PRIu64 ")!\n", test4, gold4);
        ++retcode;
    }
    if (test5 != gold5)
    {
        fprintf(stderr, "Test value 5 (%" PRIu64 ") not gold (%" PRIu64 ")!\n", test5, gold5);
        ++retcode;
    }
    if (test6 != gold6)
    {
        fprintf(stderr, "Test value 6 (%" PRIu64 ") not gold (%" PRIu64 ")!\n", test6, gold6);
        ++retcode;
    }
    if (test7 != 0)
    {
        fprintf(stderr, "Test value 7 (%zu) not what it should be (0)!\n", test7);
        ++retcode;
    }

    KLLFinalize(&kllmgr);

    if (retcode == 0)
    {
        fprintf(stdout, "## Test selectiter Successful\n");
    }
    return retcode;
}

int selectrm(void)
{
    int retcode = 0;
    KLLu_t icnt, icntmax = 9;
    KLLMgr_t *kllmgr;
    KLLSelect_t kllselect;
    KLLu_t gold1, test1;
    KLLSelect_t sgold1, stest1;

    fprintf(stdout, "# Test selectrm\n");

    kllmgr = KLLInit();

    /* in the middle (1) */
    gold1 = 0;
    test1 = 0;

    /* populate list and create gold value */
    for (icnt = 0; icnt < icntmax; ++icnt)
    {
        if (icnt != icntmax / 2)
        {
            gold1 += icnt;
        }
        KLLPushU(kllmgr, "fo", icnt);
    }

    /* select at icntmax / 2 and remove */
    for (KLLSelectInit(kllmgr, &kllselect); kllselect.at != NULL; KLLSelectNext(&kllselect))
    {
        if (kllselect.index == (size_t)icntmax / 2)
        {
            break;
        }
    }
    KLLSelectRm(kllmgr, &kllselect);
    stest1 = kllselect;

    /* sum again */
    for (KLLSelectInit(kllmgr, &kllselect); kllselect.at != NULL; KLLSelectNext(&kllselect))
    {
        if (kllselect.index == (size_t)icntmax / 2)
        {
            sgold1 = kllselect;
        }
        test1 += KLLSelectGetU(&kllselect);
    }

    KLLClear(kllmgr);

    if (test1 != gold1)
    {
        fprintf(stderr, "Test value 1 (%" PRIu64 ") not gold (%" PRIu64 ")!\n", test1, gold1);
        ++retcode;
    }

    if (stest1.index != sgold1.index)
    {
        fprintf(stderr, "Test value select 1 index (%ld) not gold (%ld)!\n", stest1.index, sgold1.index);
        ++retcode;
    }

    if (stest1.at != sgold1.at)
    {
        fprintf(stderr, "Test value select 1 at (%p) not gold (%p)!\n", (void *)stest1.at, (void *)sgold1.at);
        ++retcode;
    }

    if (stest1.pre != sgold1.pre)
    {
        fprintf(stderr, "Test value select 1 pre (%p) not gold (%p)!\n", (void *)stest1.pre, (void *)sgold1.pre);
        ++retcode;
    }

    /* the first (2) */
    gold1 = 0;
    test1 = 0;

    /* populate list and create gold value */
    /* start at 1 to ensure that the summation check is valid */
    for (icnt = 1; icnt < icntmax; ++icnt)
    {
        if (icnt != 1)
        {
            gold1 += icnt;
        }
        KLLPushU(kllmgr, "fo", icnt);
    }

    /* select at 0 and remove */
    for (KLLSelectInit(kllmgr, &kllselect); kllselect.at != NULL; KLLSelectNext(&kllselect))
    {
        if (kllselect.index == 0)
        {
            break;
        }
    }
    KLLSelectRm(kllmgr, &kllselect);
    stest1 = kllselect;

    /* sum again */
    for (KLLSelectInit(kllmgr, &kllselect); kllselect.at != NULL; KLLSelectNext(&kllselect))
    {
        if (kllselect.index == 0)
        {
            sgold1 = kllselect;
        }
        test1 += KLLSelectGetU(&kllselect);
    }

    KLLClear(kllmgr);

    if (test1 != gold1)
    {
        fprintf(stderr, "Test value 2 (%" PRIu64 ") not gold (%" PRIu64 ")!\n", test1, gold1);
        ++retcode;
    }

    if (stest1.index != sgold1.index)
    {
        fprintf(stderr, "Test value select 2 index (%ld) not gold (%ld)!\n", stest1.index, sgold1.index);
        ++retcode;
    }

    if (stest1.at != sgold1.at)
    {
        fprintf(stderr, "Test value select 2 at (%p) not gold (%p)!\n", (void *)stest1.at, (void *)sgold1.at);
        ++retcode;
    }

    if (stest1.pre != sgold1.pre)
    {
        fprintf(stderr, "Test value select 2 pre (%p) not gold (%p)!\n", (void *)stest1.pre, (void *)sgold1.pre);
        ++retcode;
    }

    /* the last (3) */
    gold1 = 0;
    test1 = 0;

    /* populate list and create gold value */
    /* start at 1 to ensure that the summation check is valid */
    for (icnt = 0; icnt < icntmax; ++icnt)
    {
        if (icnt != icntmax - 1)
        {
            gold1 += icnt;
        }
        KLLPushU(kllmgr, "fo", icnt);
    }

    /* select at 0 and remove */
    for (KLLSelectInit(kllmgr, &kllselect); kllselect.at != NULL; KLLSelectNext(&kllselect))
    {
        if (kllselect.index == icntmax - 1)
        {
            break;
        }
    }
    KLLSelectRm(kllmgr, &kllselect);
    stest1 = kllselect;

    /* sum again */
    for (KLLSelectInit(kllmgr, &kllselect); kllselect.at != NULL; KLLSelectNext(&kllselect))
    {
        if (kllselect.index == icntmax - 2)
        {
            sgold1 = kllselect;
        }
        test1 += KLLSelectGetU(&kllselect);
    }

    KLLClear(kllmgr);

    if (test1 != gold1)
    {
        fprintf(stderr, "Test value 3 (%" PRIu64 ") not gold (%" PRIu64 ")!\n", test1, gold1);
        ++retcode;
    }

    if (stest1.index != sgold1.index)
    {
        fprintf(stderr, "Test value select 3 index (%ld) not gold (%ld)!\n", stest1.index, sgold1.index);
        ++retcode;
    }

    if (stest1.at != sgold1.at)
    {
        fprintf(stderr, "Test value select 3 at (%p) not gold (%p)!\n", (void *)stest1.at, (void *)sgold1.at);
        ++retcode;
    }

    if (stest1.pre != sgold1.pre)
    {
        fprintf(stderr, "Test value select 3 pre (%p) not gold (%p)!\n", (void *)stest1.pre, (void *)sgold1.pre);
        ++retcode;
    }

    /* while looping (4) */
    gold1 = 0;
    test1 = 0;

    /* populate list and create gold value */
    for (icnt = 0; icnt < icntmax; ++icnt)
    {
        if (icnt % 2 == 0)
        {
            gold1 += icnt;
        }
        KLLPushU(kllmgr, "fo", icnt);
    }

    /* sum again */
    icnt = 0;
    for (KLLSelectInit(kllmgr, &kllselect); kllselect.at != NULL; KLLSelectNext(&kllselect))
    {
        if (icnt % 2 != 0)
        {
            KLLSelectRm(kllmgr, &kllselect);
            ++icnt;
        }
        test1 += KLLSelectGetU(&kllselect);
        ++icnt;
    }

    KLLClear(kllmgr);

    if (test1 != gold1)
    {
        fprintf(stderr, "Test value 4 (%" PRIu64 ") not gold (%" PRIu64 ")!\n", test1, gold1);
        ++retcode;
    }

    /* finished! */
    KLLFinalize(&kllmgr);

    if (retcode == 0)
    {
        fprintf(stdout, "## Test selectrm Successful\n");
    }
    return retcode;
}

int selectprepost(void)
{
    int retcode = 0;
    size_t iind = 0;
    KLLu_t icnt = 10;
    KLLMgr_t *kllmgr;
    KLLSelect_t testat;
    KLLSelect_t testpre, testpost;
    KLLNode_t *nodes[5];

    fprintf(stdout, "# Test selectprepost\n");

    kllmgr = KLLInit();

    /* populate with 5 entries */
    for (iind = 0; iind < 5; ++iind)
    {
        nodes[iind] = KLLPushU(kllmgr, "fo", icnt++);
    }

    /* get the middle one */
    KLLSelectInit(kllmgr, &testat);
    for (iind = 1; iind < 3; ++iind)
    {
        KLLSelectNext(&testat);
    }

    /* get the selects */
    testpost = testat;
    KLLSelectNext(&testpost);
    testpre = testat;
    KLLSelectPrev(kllmgr, &testpre);

    /* check stuff */
    if (testpre.pre != nodes[0])
    {
        fprintf(stderr, "Pre pre not correct!\n");
        ++retcode;
    }
    if (testpre.at != nodes[1])
    {
        fprintf(stderr, "Pre not correct!\n");
        ++retcode;
    }
    if (testpre.index != 1)
    {
        fprintf(stderr, "Pre index not correct!\n");
        ++retcode;
    }
    if (testpost.pre != nodes[2])
    {
        fprintf(stderr, "Post pre not correct!\n");
        ++retcode;
    }
    if (testpost.at != nodes[3])
    {
        fprintf(stderr, "Post not correct!\n");
        ++retcode;
    }
    if (testpost.index != 3)
    {
        fprintf(stderr, "Post index not correct!\n");
        ++retcode;
    }

    /* finished! */
    KLLFinalize(&kllmgr);

    if (retcode == 0)
    {
        fprintf(stdout, "## Test selectprepost Successful\n");
    }
    return retcode;
}

int selectfind(void)
{
    int retcode = 0;
    size_t iind = 0;
    KLLu_t icnt = 10;
    KLLMgr_t *kllmgr;
    KLLSelect_t testat;

    fprintf(stdout, "# Test selectfind\n");

    kllmgr = KLLInit();

    /* populate with 5 wrong entries first */
    for (iind = 0; iind < 5; ++iind)
    {
        KLLPushU(kllmgr, "fo", icnt++);
    }
    KLLPushU(kllmgr, "bar", icnt++);
    for (iind = 0; iind < 5; ++iind)
    {
        KLLPushU(kllmgr, "fo", icnt++);
    }
    KLLPushU(kllmgr, "bar", icnt++);

    /* KEY */
    /* find something expected */
    testat = KLLSelectFindKey(kllmgr, "bar");
    if (testat.index != 5)
    {
        fprintf(stderr, "Successful index of key search (%lu) not correct (%u)!\n", testat.index, 5);
        ++retcode;
    }

    /* find nothing */
    testat = KLLSelectFindKey(kllmgr, "baz");
    if (testat.at != NULL)
    {
        fprintf(stderr, "Unsuccessful location (%p) not correct (%p)!\n", (void *)testat.at, NULL);
        ++retcode;
    }

    /* INDEX */
    /* find something expected */
    testat = KLLSelectFindIndex(kllmgr, 7);
    if (testat.index != 7)
    {
        fprintf(stderr, "Successful index search (%lu) not correct (%u)!\n", testat.index, 7);
        ++retcode;
    }

    /* find nothing */
    testat = KLLSelectFindIndex(kllmgr, 77);
    if (testat.at != NULL)
    {
        fprintf(stderr, "Unsuccessful index search (%p) not correct (%p)!\n", (void *)testat.at, NULL);
        ++retcode;
    }

    /* NEXT KEY */
    /* find something expected */
    testat = KLLSelectFindKey(kllmgr, "bar");
    KLLSelectNext(&testat);
    testat = KLLSelectFindKeyNext(&testat, "bar");
    if (testat.index != 11)
    {
        fprintf(stderr, "Successful index of next key search (%lu) not correct (%u)!\n", testat.index, 11);
        ++retcode;
    }

    /* find nothing */
    testat = KLLSelectFindKey(kllmgr, "bar");
    KLLSelectNext(&testat);
    testat = KLLSelectFindKeyNext(&testat, "baz");
    if (testat.at != NULL)
    {
        fprintf(stderr, "Unsuccessful pointer of next key search not correct!\n");
        ++retcode;
    }

    /* NEXT INDEX */
    /* find something expected */
    testat = KLLSelectFindIndex(kllmgr, 3);
    testat = KLLSelectFindIndexNext(&testat, 7);
    if (testat.index != 7)
    {
        fprintf(stderr, "Successful index of next index search (%lu) not correct (%u)!\n", testat.index, 11);
        ++retcode;
    }

    /* find nothing */
    testat = KLLSelectFindIndex(kllmgr, 3);
    testat = KLLSelectFindIndexNext(&testat, 77);
    if (testat.at != NULL)
    {
        fprintf(stderr, "Unsuccessful pointer of next index search not correct!\n");
        ++retcode;
    }

    /* finished! */
    KLLFinalize(&kllmgr);

    if (retcode == 0)
    {
        fprintf(stdout, "## Test selectfind Successful\n");
    }
    return retcode;
}

int count(void)
{
    int retcode = 0;
    KLLu_t test1, gold1;
    KLLMgr_t *kllmgr;

    fprintf(stdout, "# Test count\n");

    kllmgr = KLLInit();

    /* KEY */
    /* populate with 5 wrong entries first */
    for (test1 = 0; test1 < 5; ++test1)
    {
        KLLPushU(kllmgr, "fo", test1);
    }
    KLLPushU(kllmgr, "bar", test1);
    for (test1 = 0; test1 < 5; ++test1)
    {
        KLLPushU(kllmgr, "fo", test1);
    }
    KLLPushU(kllmgr, "bar", test1);

    /* "fo" : 10 */
    gold1 = 10;
    test1 = KLLCountKey(kllmgr, "fo");
    if (test1 != gold1)
    {
        fprintf(stderr, "fo test:gold is %" PRIu64 ":%" PRIu64 "\n", test1, gold1);
        ++retcode;
    }

    /* "bar" : 2 */
    gold1 = 2;
    test1 = KLLCountKey(kllmgr, "bar");
    if (test1 != gold1)
    {
        fprintf(stderr, "bar test:gold is %" PRIu64 ":%" PRIu64 "\n", test1, gold1);
        ++retcode;
    }

    /* "baz" : 0 */
    gold1 = 0;
    test1 = KLLCountKey(kllmgr, "baz");
    if (test1 != gold1)
    {
        fprintf(stderr, "baz test:gold is %" PRIu64 ":%" PRIu64 "\n", test1, gold1);
        ++retcode;
    }

    KLLClear(kllmgr);

    /* VAL I */
    for (test1 = 0; test1 < 5; ++test1)
    {
        KLLPushI(kllmgr, "fo", (KLLi_t)test1);
    }
    for (test1 = 0; test1 < 5; ++test1)
    {
        KLLPushI(kllmgr, "fo", (KLLi_t)test1);
    }

    /* 3 : 2 */
    gold1 = 2;
    test1 = KLLCountI(kllmgr, 3);
    if (test1 != gold1)
    {
        fprintf(stderr, "IVAL successful test:gold is %" PRIu64 ":%" PRIu64 "\n", test1, gold1);
        ++retcode;
    }

    /* 6 : 0 */
    gold1 = 0;
    test1 = KLLCountI(kllmgr, 6);
    if (test1 != gold1)
    {
        fprintf(stderr, "IVAL unsuccessful test:gold is %" PRIu64 ":%" PRIu64 "\n", test1, gold1);
        ++retcode;
    }

    KLLClear(kllmgr);

    /* VAL U */
    for (test1 = 0; test1 < 5; ++test1)
    {
        KLLPushU(kllmgr, "fo", (KLLu_t)test1);
    }
    for (test1 = 0; test1 < 5; ++test1)
    {
        KLLPushU(kllmgr, "fo", (KLLu_t)test1);
    }

    /* 4 : 2 */
    gold1 = 2;
    test1 = KLLCountU(kllmgr, 4);
    if (test1 != gold1)
    {
        fprintf(stderr, "UVAL successful test:gold is %" PRIu64 ":%" PRIu64 "\n", test1, gold1);
        ++retcode;
    }

    /* 6 : 0 */
    gold1 = 0;
    test1 = KLLCountU(kllmgr, 6);
    if (test1 != gold1)
    {
        fprintf(stderr, "UVAL unsuccessful test:gold is %" PRIu64 ":%" PRIu64 "\n", test1, gold1);
        ++retcode;
    }

    KLLClear(kllmgr);

    /* VAL D */
    for (test1 = 0; test1 < 5; ++test1)
    {
        KLLPushD(kllmgr, "fo", (KLLd_t)test1 + 0.42);
    }
    for (test1 = 0; test1 < 5; ++test1)
    {
        KLLPushD(kllmgr, "fo", (KLLd_t)test1 + 0.42);
    }

    /* 4.42 : 2 */
    gold1 = 2;
    test1 = KLLCountD(kllmgr, 4.42);
    if (test1 != gold1)
    {
        fprintf(stderr, "DVAL successful test:gold is %" PRIu64 ":%" PRIu64 "\n", test1, gold1);
        ++retcode;
    }

    /* 6 : 0 */
    gold1 = 0;
    test1 = KLLCountD(kllmgr, 42.42);
    if (test1 != gold1)
    {
        fprintf(stderr, "DVAL unsuccessful test:gold is %" PRIu64 ":%" PRIu64 "\n", test1, gold1);
        ++retcode;
    }

    KLLClear(kllmgr);

    /* VAL S */
    for (test1 = 0; test1 < 5; ++test1)
    {
        KLLPushS(kllmgr, "fo", "winning");
    }
    for (test1 = 0; test1 < 5; ++test1)
    {
        KLLPushS(kllmgr, "fo", "losing");
    }

    /* winning : 5 */
    gold1 = 5;
    test1 = KLLCountS(kllmgr, "winning");
    if (test1 != gold1)
    {
        fprintf(stderr, "SVAL successful test:gold is %" PRIu64 ":%" PRIu64 "\n", test1, gold1);
        ++retcode;
    }

    /* 6 : 0 */
    gold1 = 0;
    test1 = KLLCountS(kllmgr, "betternotfind");
    if (test1 != gold1)
    {
        fprintf(stderr, "SVAL unsuccessful test:gold is %" PRIu64 ":%" PRIu64 "\n", test1, gold1);
        ++retcode;
    }

    KLLClear(kllmgr);

    /* finished! */
    KLLFinalize(&kllmgr);

    if (retcode == 0)
    {
        fprintf(stdout, "## Test count Successful\n");
    }
    return retcode;
}

int inception(void)
{
    int retcode = 0;
    KLLu_t i0, gold = 0, test = 0;
    KLLMgr_t *kllmgr, *kval, *kptr;
    KLLSelect_t kslct_mgr, kslct_val;

    fprintf(stdout, "# Test inception\n");

    kllmgr = KLLInit();
    kval = KLLInit();

    /* create gold and list */
    for (i0 = 0; i0 < 42; ++i0)
    {
        gold += i0;
        KLLPushU(kval, "fo", i0);
    }
    KLLPushK(kllmgr, "is_array", kval);

    kslct_mgr = KLLSelectFindKey(kllmgr, "is_array");
    kptr = KLLSelectGetK(&kslct_mgr);

    for (KLLSelectInit(kptr, &kslct_val); kslct_val.at != NULL; KLLSelectNext(&kslct_val))
    {
        test += KLLSelectGetU(&kslct_val);
    }

    KLLFinalize(&kllmgr);

    fprintf(stdout, "gold = %" PRIu64 " ; test = %" PRIu64 "\n", gold, test);
    if (test != gold)
    {
        ++retcode;
        fprintf(stderr, "test != gold!\n");
    }
    if (kval != kptr)
    {
        ++retcode;
        fprintf(stderr, "kval != kptr!\n");
    }

    if (retcode == 0)
    {
        fprintf(stdout, "## Test inception Successful\n");
    }
    return retcode;
}

int main(int argc, char *argv[])
{
    int i0;
    int retcode = 0;

    if (argc < 2)
    {
        return ++retcode;
    }

    for (i0 = 1; i0 < argc; ++i0)
    {
        if (strcmp(argv[i0], "--buildup") == 0)
        {
            retcode += buildup();
        }
        else if (strcmp(argv[i0], "--buildups") == 0)
        {
            retcode += buildups();
        }
        else if (strcmp(argv[i0], "--selectiter") == 0)
        {
            retcode += selectiter();
        }
        else if (strcmp(argv[i0], "--selectrm") == 0)
        {
            retcode += selectrm();
        }
        else if (strcmp(argv[i0], "--selectprepost") == 0)
        {
            retcode += selectprepost();
        }
        else if (strcmp(argv[i0], "--selectfind") == 0)
        {
            retcode += selectfind();
        }
        else if (strcmp(argv[i0], "--count") == 0)
        {
            retcode += count();
        }
        else if (strcmp(argv[i0], "--inception") == 0)
        {
            retcode += inception();
        }
        else
        {
            fprintf(stderr, "Did not recognize test option!\n");
            ++retcode;
        }
    }

    return retcode;
}
