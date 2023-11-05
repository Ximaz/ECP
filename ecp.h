/*
** ECP : Easy C Programming
**
** General purposes :
**
** The main goal of this library is to implement some functions and
** data structures in order not to have to re-implement them in
** projects, avoiding duplicated code.
**
*/

/*
** When importing this library, you must define some macros according
** to the features you want to use :
** - ECP_DICT_IMPLEMENTATION
*/

#pragma once

#ifdef ECP_DICT_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Apparently, this is required for WSL */
typedef __ssize_t ssize_t;

typedef struct s_ecp_dict {
    char **keys;
    char **values;
    ssize_t length;
    ssize_t (*find)(struct s_ecp_dict *self, char const *key);
    char *(*get)(struct s_ecp_dict *self, char const *key);
    int (*set)(struct s_ecp_dict *self, char const *key,
	       char const *value);
    void (*print)(struct s_ecp_dict *self);
} ecp_dict_t;

ssize_t ecp_dict_find(ecp_dict_t *self, char const *key)
{
    ssize_t idx = 0;

    if (NULL == key)
	return -1;
    for (; idx < self->length; ++idx)
	if (0 == strcmp(key, self->keys[idx]))
	    return idx;
    return -1;
}

char *ecp_dict_get(ecp_dict_t *self, char const *key)
{
    ssize_t idx = self->find(self, key);

    if (-1 == idx)
	return NULL;
    return self->values[idx];
}

int ecp_dict_set(ecp_dict_t *self, char const *key, char const *value)
{
    ssize_t idx = -1;

    if (NULL == key || NULL == value)
	return 0;
    idx = self->find(self, key);
    if (-1 != idx) {
	free(self->values[idx]);
	self->values[idx] = strdup(value);
	return NULL != self->values[idx];
    }
    ++self->length;
    self->keys = (char **) realloc(self->keys,
				   sizeof(char *) * self->length);
    if (NULL == self->keys)
	return 0;
    self->values = (char **) realloc(self->values,
				     sizeof(char *) * self->length);
    if (NULL == self->values)
	return 0;
    self->keys[self->length - 1] = strdup(key);
    if (NULL == self->keys)
	return 0;
    self->values[self->length - 1] = strdup(value);
    return NULL != self->values[self->length - 1];
}

void ecp_print_dict(ecp_dict_t *self)
{
    ssize_t idx = 0;

    if (NULL == self) {
	printf("The pointer passed to ecp_print_dict is NULL.\n");
	return;
    }
    printf("Dict keys/values length : %ld\n\n{\n", self->length);
    for (; idx < self->length; ++idx)
	printf("    \"%s\": \"%s\"%c\n",
	       self->keys[idx],
	       self->values[idx],
	       idx < self->length - 1 ? ',' : '\0');
    printf("}\n");
}

ecp_dict_t *ecp_new_dict(void)
{
    ecp_dict_t *dict = (ecp_dict_t *) malloc(sizeof(ecp_dict_t));

    if (NULL == dict)
	return NULL;
    dict->length = 0;
    dict->keys = NULL;
    dict->values = NULL;
    dict->find = ecp_dict_find;
    dict->get = ecp_dict_get;
    dict->set = ecp_dict_set;
    dict->print = ecp_print_dict;
    return dict;
}

void ecp_destroy_dict(ecp_dict_t *self)
{
    ssize_t idx = 0;

    if (NULL == self)
	return;
    for (; idx < self->length; ++idx) {
	if (NULL != self->keys && NULL != self->keys[idx])
	    free(self->keys[idx]);
	if (NULL != self->values && NULL != self->values[idx])
	    free(self->values[idx]);
    }
    free(self->keys);
    free(self->values);
    free(self);
}

#endif
