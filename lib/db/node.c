/* This file is part of the PkgUtils from EltaninOS
 * See LICENSE file for copyright and license details.
 */
#include <err.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"

struct node *
addelement(const void *value)
{
	struct node *new;

	if (!(new = malloc(1 * sizeof(*new))))
		err(1, "malloc");

	new->data = strdup(value);

	return new;
}

void
freenode(struct node *old)
{
	free(old->data);
	free(old);
}

struct node *
popnode(struct node **sp)
{
	struct node *old;

	old = *sp;
	*sp = old->next;

	return old;
}

void
pushnode(struct node **sp, struct node *new)
{
	new->next = *sp;
	*sp = new;
}