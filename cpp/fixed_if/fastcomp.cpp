#include "fastcomp.h"
#include <sys/mman.h>

#include <algorithm>

unsigned char f_start[] = { F_START_M };
unsigned char f_stop[] = { F_STOP_M };

long pagesize()
{
    static const long pagesize = sysconf(_SC_PAGE_SIZE);
    return pagesize;
}

long pagemask()
{
    static const long pagemask = ~(pagesize() - 1);
    return pagemask;
}

/* from mprotect(2)
 * int mprotect(const void *addr, size_t len, int prot);
 * addr must be aligned to a page boundary.
 */
unsigned char * align_address(unsigned char * p)
{
    size_t tmp = reinterpret_cast<size_t>(p);
    return reinterpret_cast<unsigned char*>(tmp & pagemask());
}

size_t align_size(size_t size)
{
    if (size < pagesize())
        return pagesize();
    else
        return (size & pagemask()) + pagesize() * 2;
}

#define EXPECTED_BLOCK_SIZE 1024

__attribute__((noinline))
int fix_if()
{
    unsigned char *return_address, *mark, *end_address;
    int difference;

    return_address = *reinterpret_cast<unsigned char**>(reinterpret_cast<unsigned char*>(__builtin_frame_address(0)) + sizeof(void*));

    if (mprotect(align_address(return_address - EXPECTED_BLOCK_SIZE), pagesize(), PROT_READ | PROT_EXEC))
        return -1;

    /*
     * -O3 could place marks anywhere
    end_address = return_address + 1024*2; // expected if(...) size
    std::reverse_iterator<unsigned char*> from(return_address), to(mark);
    std::reverse_iterator<unsigned char*> n_to(f_start), n_from(f_start+sizeof(f_start));
    std::reverse_iterator<unsigned char*> begin = std::search(from, to, n_from, n_to);
    if (begin == to)
        return -11;
    */
    mark = std::search(return_address - EXPECTED_BLOCK_SIZE, return_address + EXPECTED_BLOCK_SIZE, f_start, f_start+sizeof(f_start));
    if (mark == return_address + EXPECTED_BLOCK_SIZE)
        return -11;

    mark = mark - 5; // start of the begin mark. 5 == jmp size
    difference = return_address - mark - 5; // 5 == jmp opcode size

    if (mprotect(align_address(mark), align_size(difference), PROT_READ | PROT_WRITE | PROT_EXEC))
        return -2;

    *reinterpret_cast<unsigned int*>(mark+1) = difference; // 1 == jmp opcode 0xe9, so we write to offset

    if (mprotect(align_address(mark), align_size(difference), PROT_EXEC))
        return -3;

    return 0;
}

