#include <stdio.h>
#include <stddef.h>
#include <limits.h>

char mem[20000];

// Structure definition to contain metadata of each block allocated or deallocated
struct meta_block
{
    size_t size;
    int free;
    struct meta_block *next;
};

struct meta_block *freeptr = (void *)mem;

void initialize()
{
    freeptr->size = 20000 - sizeof(struct meta_block);
    freeptr->free = 1;
    freeptr->next = NULL;
}

void makepart(struct meta_block *fitting_slot, size_t size)
{
    struct meta_block *new = (void *)(((void *)fitting_slot) + size + sizeof(struct meta_block));
    new->size = (fitting_slot->size) - size - sizeof(struct meta_block);
    new->free = 1;
    new->next = fitting_slot->next;
    fitting_slot->size = size;
    fitting_slot->free = 0;
    fitting_slot->next = new;
}

void *Allocate(size_t noOfBytes)
{
    struct meta_block *curr, *bestFit, *prev;
    void *result;
    if (!(freeptr->size))
    {
        initialize();
        printf("Initialized memory!!\n");
    }
    
    curr = freeptr;
    bestFit = NULL;
    prev = NULL;

    while (curr != NULL)
    {
        if (curr->free && curr->size >= noOfBytes && (bestFit == NULL || curr->size < bestFit->size))
        {
            bestFit = curr;
            prev = prev;
        }
        prev = curr;
        curr = curr->next;
    }

    if (bestFit != NULL)
    {
        if (bestFit->size == noOfBytes)
        {
            bestFit->free = 0;
            result = (void *)(++bestFit);
            printf("Exact fitting meta_block allocated\n");
            return result;
        }
        else if (bestFit->size > (noOfBytes + sizeof(struct meta_block)))
        {
            makepart(bestFit, noOfBytes);
            result = (void *)(++bestFit);
            printf("Best fitting meta_block allocated with a makepart\n");
            return result;
        }
    }
    
    result = NULL;
    printf("Sorry. No sufficient memory to allocate\n");
    return result;
}

void merge()
{
    struct meta_block *curr, *prev;
    curr = freeptr;
    while (curr != NULL)
    {
        if (curr->next != NULL && curr->free && curr->next->free)
        {
            curr->size += (curr->next->size) + sizeof(struct meta_block);
            curr->next = curr->next->next;
        }
        prev = curr;
        curr = curr->next;
    }
}

void My_Free(void *ptr)
{
    if (((void *)mem <= ptr) && (ptr <= (void *)(mem + 20000)))
    {
        struct meta_block *curr = ptr;
        --curr;
        curr->free = 1;
        merge();
    }
    else
        printf("Please provide a valid pointer allocated by Allocate\n");
}

int main()
{
    int n;
    printf("Enter the amount of memory you want to allocate:");
    scanf("%d",&n);
    int *p = (int *)Allocate(n * sizeof(int));
    char *q = (char *)Allocate(200 * sizeof(char));
    int *r = (int *)Allocate(1000 * sizeof(int));
    My_Free(p);
    char *w = (char *)Allocate(700);
    My_Free(r);
    int *k = (int *)Allocate(500 * sizeof(int));
    printf("Allocation and deallocation--> successfully!!!\n");
    return 0;
}
