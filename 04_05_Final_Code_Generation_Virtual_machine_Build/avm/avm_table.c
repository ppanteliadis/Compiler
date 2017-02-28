#include "oneheader.h"

memclear_func_t memclearFuncs[] = {
	0,                  /* number */
	memclear_string,
	0,                  /* bool */
	memclear_table,
	0,                  /* userfunc */
	0,                  /* libfunc */
	0,                  /* nil */
	0                   /* undef */
};



avm_table* avm_tablenew(void) {
	avm_table* t = (avm_table*)malloc(sizeof(avm_table));
	AVM_WIPEOUT(*t);

	t->refCounter = t->total = 0;

	avm_tablebucketsinit(t->numIndexed);
	avm_tablebucketsinit(t->strIndexed);

	/* NEW STUFF */
	avm_tablebucketsinit(t->libfuncIndexed);
	avm_tablebucketsinit(t->userfuncIndexed);
	avm_tablebucketsinit(t->boolIndexed);
	avm_tablebucketsinit(t->tableIndexed);
	/* NEW STUFF */

	return t;
}


void avm_tabledestroy(avm_table* t) {
	avm_tablebucketsdestroy(t->strIndexed);
	avm_tablebucketsdestroy(t->numIndexed);
	/* NEW STUFF */
	avm_tablebucketsdestroy(t->userfuncIndexed);
	avm_tablebucketsdestroy(t->libfuncIndexed);
	avm_tablebucketsdestroy(t->boolIndexed);
	avm_tablebucketsdestroy(t->tableIndexed);


	free(t);
}


/**
 *
 */
avm_memcell* avm_makevalue(avm_memcell* value) {
	avm_memcell* newvalue = (avm_memcell*)malloc(sizeof(avm_memcell));
	newvalue->type = value->type;
	size_t size;
	switch (value->type){
		case number_m:
			newvalue->data.numVal = value->data.numVal;
			break;
		case string_m:
			size = strlen(value->data.strVal);
			newvalue->data.strVal = (char*)malloc(size * sizeof(char));
			strcpy(newvalue->data.strVal, value->data.strVal);
			break;
		case bool_m:
			newvalue->data.boolVal = value->data.boolVal;
			break;
		case table_m:
			newvalue->data.tableVal = value->data.tableVal;
			avm_tableincrefcounter(value->data.tableVal);
			break;
		case userfunc_m:
			newvalue->data.funcVal = value->data.funcVal;
			break;
		case libfunc_m:
			newvalue->data.libfuncVal = value->data.libfuncVal;
			break;
		case nil_m:
		case undef_m:
			break;
		default:
			fprintf(stderr, "Undefined avm_memcell type\n");
			assert(0);
	}
	return newvalue;
}

/**
 *
 */
avm_memcell* avm_makekey(avm_memcell* key) {
	avm_memcell* newkey = (avm_memcell*)malloc(sizeof(avm_memcell));
	newkey->type = key->type;
	size_t size;
	switch (key->type){
		case number_m:
			newkey->data.numVal = key->data.numVal;
			break;
		case string_m:
			size = strlen(key->data.strVal);
			newkey->data.strVal = (char*)malloc(size * sizeof(char));
			strcpy(newkey->data.strVal, key->data.strVal);
			break;
		case bool_m:
			newkey->data.boolVal = key->data.boolVal;
			break;
		case table_m:
			newkey->data.tableVal = key->data.tableVal;
			avm_tableincrefcounter(key->data.tableVal);
			break;
		case userfunc_m:
			newkey->data.funcVal = key->data.funcVal;
			break;
		case libfunc_m:
			newkey->data.libfuncVal = key->data.libfuncVal;
			break;
		case nil_m:
		case undef_m:
			break;
		default:
			fprintf(stderr, "Undefined avm_memcell type\n");
			assert(0);
	}
	return newkey;
}


/**
 * @brief
 */
avm_table_bucket* avm_appendtable(avm_table_bucket** t, avm_memcell* key, avm_memcell* value, unsigned int pos) {
	avm_table_bucket* bucket = (avm_table_bucket*)malloc(sizeof(avm_table_bucket));

	if (!bucket) {
		fprintf(stderr, "avm_appendtable: Memory allocation error\n");
		exit(-1);
	}

	bucket->key = avm_makekey(key);
	bucket->value = avm_makevalue(value);

	bucket->next = t[pos];
	t[pos] = bucket;

	return bucket;
}


/**
 * @brief
 */
avm_memcell* avm_tablegetelem(avm_table* t, avm_memcell* key) {
	/*FIXME*/
	assert(t);
	assert(key);
	avm_table_bucket* item;
	switch (key->type){
		case number_m:
			item = numLookup(t->numIndexed, key);
			break;
		case string_m:
			item = strLookup(t->strIndexed, key);
			break;
		case bool_m:
			item = boolLookup(t->boolIndexed, key);
			break;
		case table_m:
			item = tableLookup(t->tableIndexed, key);
			break;
		case userfunc_m:
			item = userfuncLookup(t->userfuncIndexed, key);
			break;
		case libfunc_m:
			item = libfuncLookup(t->libfuncIndexed, key);
			break;
		case nil_m:
			item = (avm_table_bucket*)malloc(sizeof(avm_table_bucket));
			item->value->type = nil_m;
			break;
		case undef_m:
			item = (avm_table_bucket*)malloc(sizeof(avm_table_bucket));
			item->value->type = undef_m;
			break;
		default:
			fprintf(stderr, "Undefined avm_memcell type\n");
			assert(0);
	}
	return item->value;
}


/**
 * @brief
 */
void avm_tablesetelem(avm_table* t, avm_memcell* key, avm_memcell* value) {
	/*FIXME*/
	assert(t);
	assert(key);
	assert(value);
	avm_table_bucket* item;
	unsigned int pos;

	switch (key->type){
		case number_m:
			item = numLookup(t->numIndexed, key);
			if (!item) {
				pos = numHash(key->data.numVal);
				avm_appendtable(t->numIndexed, key, value, pos);
				t->total++;
			}
			else {
				item->value = avm_makevalue(value);
			}
			break;
		case string_m:
			item = strLookup(t->strIndexed, key);
			if (!item) {
				pos = strHash(key->data.strVal);
				avm_appendtable(t->strIndexed, key, value, pos);
				t->total++;
			}
			else {
				item->value = avm_makevalue(value);
			}
			break;
		case bool_m:
			item = boolLookup(t->boolIndexed, key);
			if (!item) {
				pos = key->data.boolVal;
				avm_appendtable(t->boolIndexed, key, value, pos);
				t->total++;
			}
			else {
				item->value = avm_makevalue(value);
			}
			break;
		case table_m:
			item = tableLookup(t->tableIndexed, key);
			if (!item) {
				pos = numHash(key->data.tableVal->refCounter);
				avm_appendtable(t->tableIndexed, key, value, pos);
				t->total++;
			}
			else {
				item->value = avm_makevalue(value);
			}
			break;
		case userfunc_m:
			item = userfuncLookup(t->userfuncIndexed, key);
			if (!item) {
				pos = userfuncHash(key->data.funcVal);
				avm_appendtable(t->userfuncIndexed, key, value, pos);
				t->total++;
			}
			else {
				item->value = avm_makevalue(value);
			}
			break;
		case libfunc_m:
			item = libfuncLookup(t->libfuncIndexed, key);
			if (!item) {
				pos = strHash(key->data.libfuncVal);
				avm_appendtable(t->libfuncIndexed, key, value, pos);
				t->total++;
			}
			else {
				item->value = avm_makevalue(value);
			}
			break;
		case nil_m:
			avm_error("nil is an illegal table value\n");
			break;
		case undef_m:
			avm_error("undefined is an illegal table value\n");
			break;
		default:
			fprintf(stderr, "Undefined avm_memcell type\n");
			assert(0);
	}
}


void avm_tableincrefcounter(avm_table* t) {
	++t->refCounter;
}


/**
 * @brief Decrements the reference counter of a table.
 *        Automatic garbage collection when counter gets zero.
 */
void avm_tabledecrefcounter(avm_table* t) {
	assert(t->refCounter > 0);
	if (!--t->refCounter)
		avm_tabledestroy(t);
}



void avm_tablebucketsinit(avm_table_bucket** p) {
	unsigned i;
	for (i = 0; i < AVM_TABLE_HASHSIZE; ++i) {
		p[i] = (avm_table_bucket*)0;
	}
}

void avm_tablebucketsdestroy(avm_table_bucket** p) {
	unsigned i;
	for (i = 0; i < AVM_TABLE_HASHSIZE; ++i, ++p) {
		avm_table_bucket* b;
		for (b = *p; b;) {
			avm_table_bucket* del = b;
			b = b->next;
			avm_memcellclear(del->key);
			avm_memcellclear(del->value);
			free(del);
		}
		p[i] = (avm_table_bucket*)0;
	}
}

/**
 * @brief When a cell is cleared, it has to destroy all dynamic data
 *        content or reset it's reference to a table.
 */
void avm_memcellclear(avm_memcell* m) {
	if (m->type != undef_m) {
		memclear_func_t f = memclearFuncs[m->type];
		if (f) {
			(*f)(m);
		}
		m->type = undef_m;
	}
}

void memclear_table(avm_memcell* m) {
	assert(m->data.tableVal);
	avm_tabledecrefcounter(m->data.tableVal);
}

void memclear_string(avm_memcell* m) {
	assert(m->data.strVal);
	free(m->data.strVal);
}
