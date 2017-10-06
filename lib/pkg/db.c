#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pkg.h"

/* number plus colon */
#define PREFIX (sizeof(char) * 2)

#define Name        6382843298
#define LongName    249786571779427685
#define Version     229390708560831
#define License     229372341555262
#define Description 13751386334653244867U
#define LongDesc    249786571779069171
#define MBSize      6383314208
#define RDependency 229394969610032
#define MDependency 7569334749620571
#define Directory   193404666
#define File        6382564547
#define Flag        6382568041

static unsigned long
hash(char *str)
{
	size_t hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) ^ c;

	return hash;
}

Package *
db_open(const char *file)
{
	char **sp, *buf = NULL, *p;
	FILE *fp;
	Package *pkg;
	size_t size = 0;
	ssize_t len;
	struct node **np;

	if (!(fp = fopen(file, "r")))
		goto failure;

	if (!(pkg = malloc(1 * sizeof(*pkg))))
		goto failure;

	pkg->name        = NULL;
	pkg->version     = NULL;
	pkg->license     = NULL;
	pkg->description = NULL;
	pkg->rdeps       = NULL;
	pkg->mdeps       = NULL;
	pkg->dirs        = NULL;
	pkg->files       = NULL;

	while ((len = getline(&buf, &size, fp)) != EOF) {
		buf[len - 1] = '\0'; /* remove trailing newline */
		sp = NULL;
		np = NULL;

		/* ignore blank lines */
		if (buf == NULL || *buf == '\0')
			continue;

		for (p = buf; *buf != ':'; buf++)
			continue;
		*buf++ = '\0';

		switch (hash(p)) {
		case Name:
			sp = &pkg->name;
			break;
		case Version:
			sp = &pkg->version;
			break;
		case License:
			sp = &pkg->license;
			break;
		case Description:
			sp = &pkg->description;
			break;
		case RDependency:
			np = &pkg->rdeps;
			break;
		case MDependency:
			np = &pkg->mdeps;
			break;
		case Directory:
			np = &pkg->dirs;
			break;
		case File:
			np = &pkg->files;
			break;
		}

		if (sp && !(*sp = strdup(buf)))
			goto err;

		if (np && pushnode(np, addelement(buf)) < 0)
			goto err;

		/* return pointer to right place */
		buf = p;
	}

	goto done;
err:
	db_close(pkg);
failure:
	pkg = NULL;
done:
	free(buf);

	if (fp)
		fclose(fp);

	return pkg;
}

void
db_close(Package *pkg) {
	free(pkg->name);
	free(pkg->version);
	free(pkg->license);
	free(pkg->description);

	while (pkg->rdeps)
		freenode(popnode(&pkg->rdeps));
	while (pkg->mdeps)
		freenode(popnode(&pkg->mdeps));
	while (pkg->dirs)
		freenode(popnode(&pkg->dirs));
	while (pkg->files)
		freenode(popnode(&pkg->files));

	free(pkg);
}
