#include <sys/types.h>

#include <stdio.h>

#include "arg.h"
#include "config.h"
#include "compat.h"

struct node {
	void *data;
	struct node *next;
};

typedef struct {
	struct node *dirs;
	struct node *files;
	struct node *flags;
	struct node *mdeps;
	struct node *rdeps;
	off_t size;
	off_t pkgsize;
	char *name;
	char *version;
	char *license;
	char *description;
} Package;

extern int z_errno;

/* ar.c */
int uncomp(int, int);
int unarchive(int);

/* db.c */
Package * db_open(const char *);
void db_close(Package *);

/* fgetline.c */
ssize_t fgetline(char *, size_t, FILE *);

/* fs.c */
int copy(const char *, const char *);
int move(const char *, const char *);
int remove(const char *);

/* net.c */
int netfd(char *, int, const char *);

/* node.c */
struct node * addelement(const void *);
void freenode(struct node *);
struct node * popnode(struct node **);
int pushnode(struct node **, struct node *);

/* util.c */
unsigned filetosum(int);
unsigned strtohash(char *);
size_t stoll(const char *, long long, long long, int);
