// name: 최진성
// # of assignment: 3
// name of the file: customer_manager2.c

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

enum {HASH_MULTIPLIER = 65599};
typedef struct HashTable* HashTable_T;

#define UNIT_TABLE_SIZE 1024

struct UserInfo {
    char *name;                // customer name
    char *id;                  // customer id
    int purchase;              // purchase amount (> 0)
    struct UserInfo *UIid;     // pointer to the next UserInfo based on id
    struct UserInfo *UIname;   // pointer to the next UserInfo based on name
};

struct DB {
    HashTable_T ht_id;         // Hash Table by id
    HashTable_T ht_name;       // Hash Table by name
    int numItems;              // # of stored items
    int cursize;               // current size of the array
};

struct HashTable {
    struct UserInfo *hArray[UNIT_TABLE_SIZE];
    // pointer to array of UNIT_TABLE_SIZE
};

HashTable_T CreateHashTable(void)
/* Creating HashTable */
{
    HashTable_T t;
    t = (HashTable_T) calloc(1, sizeof(struct HashTable));
    return t;
}

static int hash_function(const char *pcKey, int iBucketCount)

/* Return a hash code for pcKey that is between 0 and iBucketCount-1,
   inclusive. Adapted from the EE209 lecture notes. */
{
    int i;
    unsigned int uiHash = 0U;
    for (i = 0; pcKey[i] != '\0'; i++)
        uiHash = uiHash * (unsigned int)HASH_MULTIPLIER
                 + (unsigned int)pcKey[i];
    return (int)(uiHash % (unsigned int)iBucketCount);
}

int item_search_id(HashTable_T ht, const char *key, int iBucketCount)
/* Return 1 if item exist with id=key. return 0 if fails.
   Adapted from the EE209 lecture notes. */
{
    struct UserInfo *p;
    int h = hash_function(key, iBucketCount);

    for (p = ht->hArray[h]; p != NULL; p = p->UIid) {
        if (p->id && strcmp(p->id, key) == 0) {
            return 1;
        }
    }
    return 0;
}

int item_search_name(HashTable_T ht, const char *key, int iBucketCount)
/* Return 1 if item exist with name=key. return 0 if fails.
   Adapted from the EE209 lecture notes. */
{
    struct UserInfo *p;
    int h = hash_function(key, iBucketCount);

    for (p = ht->hArray[h]; p != NULL; p = p->UIname) {
        if (p->name && strcmp(p->name, key) == 0) {
            return 1;
        }
    }
    return 0;
}


/*--------------------------------------------------------------------*/
DB_T
CreateCustomerDB(void)
{
    DB_T d;

    /* memory for struct DB */
    d = (DB_T) calloc(1, sizeof(struct DB));
    /* check if memory is allocated properly */
    if (d == NULL) {
        fprintf(stderr, "Can't allocate a memory for DB_T\n");
        return NULL;
    }

    /* memory for idhashtable */
    d->ht_id = CreateHashTable();
    /* check if memory is allocated properly */
    if (d->ht_id == NULL) {
        fprintf(stderr, "Can't allocate a memory for HashTable_T\n");
        return NULL;
    }

    /* memory for namehashtable */
    d->ht_name = CreateHashTable();
    /* check if memory is allocated properly */
    if (d->ht_name == NULL) {
        fprintf(stderr, "Can't allocate a memory for HashTable_T\n");
        return NULL;
    }

    d->cursize = UNIT_TABLE_SIZE; // initialize cursize as UNIT_TABLE_SIZE
    d->numItems = 0; // initialize numitems as 0

    int i;

    /* initializing each hashtable component as NULL */
    for (i=0; i<d->cursize; i++) {
        d->ht_id->hArray[i] = NULL;
        d->ht_name->hArray[i] = NULL;
    }

    return d;

}
/*--------------------------------------------------------------------*/
void
DestroyCustomerDB(DB_T d)
{
    struct UserInfo *p;
    struct UserInfo *nextp;
    int i;

    /* free each userinfo */
    for (i=0; i<d->cursize; i++) {
        for (p=d->ht_id->hArray[i]; p != NULL; p=nextp) {
            nextp = p->UIid;
            free(p);
        }
    }

    /* free hashtable */
    free(d->ht_id);
    free(d->ht_name);

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

    /* check if same id exist */
    if (item_search_id(d->ht_id, id, d->cursize)) {
        fprintf(stderr, "New item registration failed\n");
        return (-1);
    }

    /* check if same name exist */
    if (item_search_name(d->ht_name, name, d->cursize)) {
        fprintf(stderr, "New item registration failed\n");
        return (-1);
    }

    /* memory for new userinfo */
    struct UserInfo *p = (struct UserInfo*)calloc(1,
            sizeof(struct UserInfo));
    /* check if memory is allocated properly */
    if (p == NULL) {
        fprintf(stderr, "Can't allocate a memory for new item\n");
        return (-1);
    }
    int h_id = hash_function(id, d->cursize);     // hash for id
    int h_name = hash_function(name, d->cursize); // hash for name

    p->id = strdup(id);     // id registration
    p->name = strdup(name); // name registration
    p->purchase = purchase; // purchase registration

    /* UIid registration which points to hArray[h_id] */
    if (d->ht_id->hArray[h_id] == NULL) {
        p->UIid = NULL;
    }
    else
        p->UIid = d->ht_id->hArray[h_id];

    /* hArray[h_id] points to new userinfo */
    d->ht_id->hArray[h_id] = p;

    /* UIname registration which points to hArray[h_name] */
    if (d->ht_name->hArray[h_name] == NULL) {
        p->UIname = NULL;
    }
    else
        p->UIname = d->ht_name->hArray[h_name];

    /* hArray[h_name] points to new userinfo */
    d->ht_name->hArray[h_name] = p;

    d->numItems += 1; // increase # of items

    return (0);

}
/*--------------------------------------------------------------------*/
int
UnregisterCustomerByID(DB_T d, const char *id)
{
    /* check if d or id is NULL */
    if (d == NULL || id == NULL) {
        fprintf(stderr, "No item to be unregistered\n");
        return (-1);
    }

    /* check if # of items is 0 */
    if (d->numItems == 0) {
        fprintf(stderr, "No item to be unregistered\n");
        return (-1);
    }

    struct UserInfo *p; // pointer to check the matching userinfo
    struct UserInfo *temp = NULL; // pointer to previous userinfo
    char *username; // saving the name of found userinfo
    int h_id = hash_function(id, d->cursize); // hash for id
    int h_name; // hash for name
    int indicator = 0; // indicator if userinfo is found

    for (p = d->ht_id->hArray[h_id]; p != NULL;
                            temp = p, p = p->UIid) {
        /*checking if id matches */
        if (p->id && strcmp(p->id, id) == 0) {
            /* changing the linking structure so when free,
               it does not damage the DB */
            if (temp == NULL) {
                d->ht_id->hArray[h_id]=p->UIid;
            }
            else
                temp->UIid = p->UIid;

            username = strdup(p->name); // saving the name of found userinfo
            h_name = hash_function(username, d->cursize); // hash for name
            indicator = 1; // change to 1 when item found
            break;
        }
    }

    if (indicator == 1){
        temp = NULL;
        for (p = d->ht_name->hArray[h_name]; p != NULL;
                                temp = p, p = p->UIname) {
            /* checking if name matches */
            if (p->name && strcmp(p->name, username) == 0) {
                /* changing the linking structure so when free,
                   it does not damage the DB */
                if (temp == NULL) {
                    d->ht_name->hArray[h_name]=p->UIname;
                }
                else
                    temp->UIname = p->UIname;

                /* free the target item */
                free(p);

                d->numItems -= 1; // # of items decrease
                return 0;
            }
        }

    }

    return (-1);

}

/*--------------------------------------------------------------------*/
int
UnregisterCustomerByName(DB_T d, const char *name)
{
    /* check if d or name is NULL */
    if (d == NULL || name == NULL) {
        fprintf(stderr, "No item to be unregistered\n");
        return (-1);
    }

    /* check if # of items is 0 */
    if (d->numItems == 0) {
        fprintf(stderr, "No item to be unregistered\n");
        return (-1);
    }

    struct UserInfo *p; // pointer to check the matching userinfo
    struct UserInfo *temp = NULL; // pointer to previous userinfo
    char *userid; // saving the id of found userinfo
    int h_name = hash_function(name, d->cursize); // hash for name
    int h_id; // hash for id
    int indicator = 0; // indicator if userinfo is found

    for (p = d->ht_name->hArray[h_name]; p != NULL;
                            temp = p, p = p->UIname) {
        /*checking if name matches */
        if (p->name && strcmp(p->name, name) == 0) {
            /* changing the linking structure so when free,
               it does not damage the DB */
            if (temp == NULL) {
                d->ht_name->hArray[h_name]=p->UIname;
            }
            else
                temp->UIname = p->UIname;

            userid = strdup(p->id); // saving the id of found userinfo
            h_id = hash_function(userid, d->cursize); // hash for id
            indicator = 1; // change to 1 when item found
            break;
        }
    }

    if (indicator == 1){
        temp = NULL;
        for (p = d->ht_id->hArray[h_id]; p != NULL;
                                temp = p, p = p->UIid) {
            /* checking if id matches */
            if (p->id && strcmp(p->id, userid) == 0) {
                /* changing the linking structure so when free,
                   it does not damage the DB */
                if (temp == NULL) {
                    d->ht_id->hArray[h_id]=p->UIid;
                }
                else
                    temp->UIid = p->UIid;

                /* free the target item */
                free(p);

                d->numItems -= 1; // # of items decrease
                return 0;
            }
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

    struct UserInfo *p;
    int h = hash_function(id, d->cursize);

    /* find the userinfo with same const *id and return purchase */
    for (p = d->ht_id->hArray[h]; p != NULL; p = p->UIid) {
        if (p->id && strcmp(p->id, id) == 0) {
            return (p->purchase);
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

    struct UserInfo *p;
    int h = hash_function(name, d->cursize);

    /* find the userinfo with same const *name and return purchase */
    for (p = d->ht_name->hArray[h]; p != NULL; p = p->UIname) {
        if (p->name && strcmp(p->name, name) == 0) {
            return (p->purchase);
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
    int h;

    struct UserInfo *p;

    /* perform fp on each userinfo and add to value */
    for (h=0; h<d->cursize; h++) {
        for (p = d->ht_id->hArray[h]; p != NULL; p = p->UIid) {
            value += fp(p->id, p->name, p->purchase);
        }
    }

    return value;

}

