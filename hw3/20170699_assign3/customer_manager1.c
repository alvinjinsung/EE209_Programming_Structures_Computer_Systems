// name: 최진선
// # of assignment: 3
// name of the file: customer_manager1.c

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"


#define UNIT_ARRAY_SIZE 1024

struct UserInfo {
  char *name;                // customer name
  char *id;                  // customer id
  int purchase;              // purchase amount (> 0)
};

struct DB {
  struct UserInfo *pArray;   // pointer to the array
  int curArrSize;            // current array size (max # of elements)
  int numItems;              // # of stored items, needed to determine
			     // # whether the array should be expanded
			     // # or not
};


/*--------------------------------------------------------------------*/
DB_T
CreateCustomerDB(void)
{
    DB_T d;

    /* memory for struct DB */
    d = (DB_T) calloc(1, sizeof(struct DB));
    if (d == NULL)
        /* check if memory is allocated properly */
    {
      fprintf(stderr, "Can't allocate a memory for DB_T\n");
      return NULL;
    }

    d->curArrSize = UNIT_ARRAY_SIZE; // start with 1024 elements

    /* memory for d->pArray */
    d->pArray = (struct UserInfo *)calloc(d->curArrSize,
                 sizeof(struct UserInfo));
    if (d->pArray == NULL)
        /* check if memory is allocated properly */
    {
      fprintf(stderr, "Can't allocate a memory for array of size %d\n",
          d->curArrSize);
      free(d);
      return NULL;
    }

    d->numItems = 0; // initial # of items
    return d;

}
/*--------------------------------------------------------------------*/
void
DestroyCustomerDB(DB_T d)
{
    int i;

    /* free each user info */
    for (i=0; i<(d->curArrSize); i++) {
        free(d->pArray[i].id);
        free(d->pArray[i].name);
    }

    /* free array */
    free(d->pArray);

    free(d);
}
/*--------------------------------------------------------------------*/
int
RegisterCustomer(DB_T d, const char *id,
                 const char *name, const int purchase)
{
    /* check if d, id, name is NULL or purchase<=0 */
    if ((d == NULL) || (id == NULL) || (name == NULL) || (purchase<=0)) {
        fprintf(stderr, "New item registration failed\n");
        return (-1);
    }

    int i;

    /* check if same id or name exist */
    for (i=0; i<(d->curArrSize); i++) {
        if (d->pArray[i].id && d->pArray[i].name) {
            if ((strcmp(d->pArray[i].id, id) == 0) ||
            (strcmp(d->pArray[i].name, name) == 0)) {
                fprintf(stderr, "New item registration failed\n");
                return (-1);
            }
        }
    }

    /* expand array size if neccessary */
    if (d->numItems == d->curArrSize) {
        int i;
        d->curArrSize += 1024;
        d->pArray = (struct UserInfo *)realloc(d->pArray,
                (d->curArrSize)*sizeof(struct UserInfo));

        /* initialize each element to 0 */
        for(i=((d->curArrSize)-1024); i<(d->curArrSize) ; i++) {
            d->pArray[i].id = 0, d->pArray[i].name = 0, d->pArray[i].purchase = 0;
        }
    }

    i=0;

    /* finding empty location */
    while (d->pArray[i].id != 0) {
        i++;
    }

    /* registering id */
    d->pArray[i].id = strdup(id);
    /* check if memory is properly allocated */
    if (d->pArray[i].id == NULL) {
        fprintf(stderr, "Can't allocate a memory for new item\n");
        return (-1);
    }

    /* registering name */
    d->pArray[i].name = strdup(name);
    /* check if memory is properly allocated */
    if (d->pArray[i].name == NULL) {
        fprintf(stderr, "Can't allocate a memory for new item\n");
        return (-1);
    }

    /* registering purchase */
    d->pArray[i].purchase = purchase;

    d->numItems += 1;

    return (0);

}
/*--------------------------------------------------------------------*/
int
UnregisterCustomerByID(DB_T d, const char *id)
{
    /* check if d, id is NULL*/
    if (d == NULL || id == NULL) {
        fprintf(stderr, "No item to be unregistered\n");
        return (-1);
    }

    int i;

    /* find the userinfo with const char *id and free it */
    for (i=0; i<(d->curArrSize); i++) {
        if (d->pArray[i].id && strcmp(d->pArray[i].id, id) == 0) {
            free(d->pArray[i].id);
            free(d->pArray[i].name);
            d->pArray[i].id = 0;
            d->pArray[i].name = 0;
            d->pArray[i].purchase = 0;

            d->numItems -= 1;

            return (0);
        }
    }

    return (-1);
}

/*--------------------------------------------------------------------*/
int
UnregisterCustomerByName(DB_T d, const char *name)
{
    /* check if d, name is NULL */
    if (d == NULL || name == NULL) {
        fprintf(stderr, "No item to be unregistered\n");
        return (-1);
    }

    int i;

    /* find the userinfo with const char *name and free it */
    for (i=0; i<(d->curArrSize); i++) {
        if (d->pArray[i].name && strcmp(d->pArray[i].name, name) == 0) {
            free(d->pArray[i].id);
            free(d->pArray[i].name);
            d->pArray[i].id = 0;
            d->pArray[i].name = 0;
            d->pArray[i].purchase = 0;

            d->numItems -= 1;

            return (0);
        }
    }

    return (-1);
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByID(DB_T d, const char* id)
{
    /* check if d, id is NULL*/
    if (d == NULL || id == NULL) {
        fprintf(stderr, "No item to get purchase\n");
        return (-1);
    }

    int i;

    /* find the userinfo with same const *id and return purchase */
    for (i=0; i<(d->curArrSize); i++) {
        if (d->pArray[i].id && strcmp(d->pArray[i].id, id) == 0) {
            return (d->pArray[i].purchase);
        }
    }

    return (-1);
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByName(DB_T d, const char* name)
{
    /* check if d, name is NULL*/
    if (d == NULL || name == NULL) {
        fprintf(stderr, "No item to get purchase\n");
        return (-1);
    }

    int i;

    /* find the userinfo with same const *name and return purchase */
    for (i=0; i<(d->curArrSize); i++) {
        if (d->pArray[i].name && strcmp(d->pArray[i].name, name) == 0) {
            return (d->pArray[i].purchase);
        }
    }

    return (-1);
}
/*--------------------------------------------------------------------*/
int
GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
{
    /* check if d, fp is NULL*/
    if (d == NULL || fp == NULL) {
        fprintf(stderr, "Function can't get purchase\n");
        return (-1);
    }

    int value=0; // sum of return value of fp on each userinfo
    int i;

    /* perform fp on each userinfo and add to value */
    for (i=0; i<(d->curArrSize); i++) {
        if (d->pArray[i].id != 0) {
            value += fp(d->pArray[i].id, d->pArray[i].name,
                        d->pArray[i].purchase);
        }
    }

    return value;

}

