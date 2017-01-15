#include<stdio.h>

int getchar(void) { return 1; }
void putchar(int x) {}

/// Defined as power of 2 - so we can do & instead of %
#define RING_BUF_MAX (1<<7)

#define MEMZERO(p, l) do{int mzi; for(mzi=0;mzi<(l);mzi++)(p)[mzi]=0x0;}while(0)

typedef struct ring_buf
{
    unsigned int head, tail;

    unsigned char buf[RING_BUF_MAX];

} ring_buf_t;

void 
ring_buf_memset(ring_buf_t *ring)
{
    MEMZERO(ring->buf, sizeof(ring->buf));
    ring->head = 0;
    ring->tail = 0;
}

int
ring_buf_size(ring_buf_t *ring)
{
    return (0xff & ( ring->head - ring->tail));
}

int
ring_buf_push(ring_buf_t *ring, unsigned char c)
{
    if(ring_buf_size(ring) == RING_BUF_MAX)
        return 0;

    ring->buf[ring->head & (RING_BUF_MAX-1)] = c;

    ring->head++;

    return 1;
}

unsigned char
ring_buf_pop(ring_buf_t *ring)
{
    if(ring_buf_size(ring)){
        unsigned char c = ring->buf[ring->tail & (RING_BUF_MAX-1)];

        ring->tail++;

        return c;
    }
    return (unsigned char)0;
}

#ifdef TEST_RING
int main(void)
{
    int i, c;
    ring_buf_t ring;
    ring_buf_memset(&ring);

    for(i = 30; i ; i--){
        c = getchar();

        if(c == EOF) break;

        if(c == '\n') continue;     // Skip newlines

        if(c == '.'){
            unsigned char a = ring_buf_pop(&ring);

            if(a == (unsigned char)0)
                printf("ring_buf_pop failed\n");
            else
                putchar(a);  
        }
        else{
            if(!ring_buf_push(&ring, c))
                printf("ring_buf_push failed\n");
            }

        if(!ring_buf_size(&ring))
            putchar('\n');

    }
    putchar('\n');
    
    printf("Finished - final size (should be zero): %i\n", ring_buf_size(&ring));

    return (ring_buf_size(&ring));
}
#endif
