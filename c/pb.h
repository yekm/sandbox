#include <stdio.h>
#include <ctype.h>

static inline void
pb(const char* m, const void* _p, unsigned len)
{
    FILE * o = stderr;

    void chardump(const unsigned char * p, unsigned len) {
        fprintf(o, "    ");
        for (unsigned j = 0; j<len; j++)
        {
            if (isprint((int)p[j]))
                fprintf(o, "%c", p[j]);
            else
                fprintf(o, ".");
        }
    }

    const unsigned char* p = (const unsigned char*)_p;
    fprintf(o, "%s %p len:%d(0x%x):\n", m, p, len, len);

    unsigned i = 0;
    for (; i < len; ++i) {
        if (i > 0 && i % 32 == 0) {
            chardump(p+i-32, 32);
            fprintf(o, "\n");
        }
        fprintf(o, "%2.2x ", (unsigned)p[i]);
    }

    unsigned l = len % 32;
    unsigned j = i;
    if (l == 0)
        l = 32;
    else
        while (j++ % 32) fprintf(o, "   ");;

    chardump(p+i-l, l);

    fprintf(o, "\n");
    fflush(o);
    fflush(stdout);
}
