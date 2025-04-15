#include <stdio.h>
#include <stddef.h>

enum ShimMemType_e
{
    mt_malloc=0,
    mt_foo,
    mt_size
};
typedef enum ShimMemType_e ShimMemType;

struct ShimMemNode_s
{
    void *pntr; /*!< Location that memory is housed. */
    struct ShimMemNode_s *nxt; /*!< Pointer to next node. */
};
typedef struct ShimMemNode_s ShimMemNode;

struct ShimOuter_s
{
    struct ShimMemNode_s mem[(size_t)mt_size];
};
typedef struct ShimOuter_s ShimOuter;

int main(void)
{
    printf("sizeof(ShimMemType) = %ld\n", sizeof(ShimMemType));
    printf("sizeof(ShimMemNode) = %ld\n", sizeof(ShimMemNode));
    printf("sizeof(ShimOuter) = %ld\n", sizeof(ShimOuter));
}
