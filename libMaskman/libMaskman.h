#ifndef lib_Maskman
#define lib_Maskman

#include <stddef.h>

typedef unsigned char mskmnslot_t;

struct mskmn_s
{
    size_t size;           /* 64 bits assuming x86_64 */
    size_t bits;           /* 64 bits assuming x86_64 */
    mskmnslot_t *bitarray; /* 64 bits assuming x86_64 */
};
typedef struct mskmn_s mskmn_t; /* 192 bits assuming x86_64 */

void mskmn_init(mskmn_t *obj, const size_t bits);
void mskmn_reset(mskmn_t *obj);
void mskmn_finalize(mskmn_t *obj);

void mskmn_set(mskmn_t *obj, const size_t bit);

void mskmn_print_binary(const mskmn_t *const obj);
void mskmn_print_hex(const mskmn_t *const obj);
void mskmn_print_delim(const char *const delim);
void mskmn_print_memory(void);

#endif
