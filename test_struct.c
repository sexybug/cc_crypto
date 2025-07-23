
#include <stdint.h>

typedef struct {
    uint32_t words[7];
    uint32_t len;
} test_struct;

size_t get_len(test_struct *ts)
{
    return sizeof(ts->words);
}

int main(void)
{
    test_struct ts = { .words = { 1, 2, 3, 4, 5, 6, 7 }, .len = 7 };
    size_t len = sizeof(ts.words);

    size_t len2 = get_len(&ts);
    return 0;
}