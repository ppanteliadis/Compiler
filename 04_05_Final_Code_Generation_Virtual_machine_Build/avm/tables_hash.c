/*******************************************************************
 * @file   tables_hash.h                                           *
 * @author Steve Cangonj <cangonj@csd.uoc.gr>, 2871                *
 * @author Ioannis-Pavlos Panteliadis <panteliad@csd.uoc.gr>, 2794 *
 * @author Kiriakos Paraskevas <paraskevas@csd.uoc.gr>, 2832       *
 *                                                                 *
 * @brief                                                          *
 *******************************************************************/


#include "oneheader.h"


/**
 * @brief
 */
unsigned int numHash(double numVal) {
	return (unsigned int)numVal % (unsigned int)AVM_TABLE_HASHSIZE;
}


/**
 * @brief
 */
unsigned int strHash(const char* pcKey) {
	size_t ui;
	unsigned int uiHash = 0U;
	for (ui = 0U; pcKey[ui] != '\0'; ui++)
		uiHash = uiHash * 65599 + pcKey[ui];
	uiHash = uiHash % (unsigned int)AVM_TABLE_HASHSIZE - 1;
	return uiHash;
}


/**
 * @brief
 */
unsigned int userfuncHash(unsigned int funcVal) {
	return funcVal % (unsigned int)AVM_TABLE_HASHSIZE;
}



/**
 * @brief
 */
avm_table_bucket* numLookup(avm_table_bucket** t, avm_memcell* k) {
	unsigned int pos = numHash(k->data.numVal);

	avm_table_bucket* buckets = t[pos];
	while (buckets) {
		if (buckets->key->data.numVal == k->data.numVal) {
			return buckets;
		}
		buckets = buckets->next;
	}

	return (avm_table_bucket*)NULL;
}


/**
 * @brief
 */
avm_table_bucket* strLookup(avm_table_bucket** t, avm_memcell* k) {
	unsigned int pos = strHash(k->data.strVal);

	avm_table_bucket* buckets = t[pos];
	while (buckets) {
		if (!strcmp(buckets->key->data.strVal, k->data.strVal)) {
			return buckets;
		}
		buckets = buckets->next;
	}

	return (avm_table_bucket*)NULL;
}

/**
 * @brief
 */
avm_table_bucket* boolLookup(avm_table_bucket** t, avm_memcell* k) {
	unsigned int pos = k->data.boolVal;		/* 0 and 1 */

	avm_table_bucket* buckets = t[pos];
	while (buckets) {
		if (buckets->key->data.boolVal == k->data.boolVal) {
			return buckets;
		}
		buckets = buckets->next;
	}

	return (avm_table_bucket*)NULL;
}


/**
 * @brief
 */
avm_table_bucket* libfuncLookup(avm_table_bucket** t, avm_memcell* k) {
	unsigned int pos = strHash(k->data.libfuncVal);		/* Libfuncs are strings */

	avm_table_bucket* buckets = t[pos];
	while (buckets) {
		if (buckets->key->data.libfuncVal == k->data.libfuncVal) {
			return buckets;
		}
		buckets = buckets->next;
	}

	return (avm_table_bucket*)NULL;
}



/**
 * @brief
 */
avm_table_bucket* userfuncLookup(avm_table_bucket** t, avm_memcell* k) {
	unsigned int pos = userfuncHash(k->data.funcVal);

	avm_table_bucket* buckets = t[pos];
	while (buckets) {
		if (buckets->key->data.funcVal == k->data.funcVal) {
			return buckets;
		}
		buckets = buckets->next;
	}

	return (avm_table_bucket*)NULL;
}




/**
 * @brief 
 */
avm_table_bucket* tableLookup(avm_table_bucket** t, avm_memcell* k) {
	unsigned int pos = numHash(k->data.tableVal->refCounter);

	avm_table_bucket* buckets = t[pos];
	while (buckets) {
		if (buckets->key->data.numVal == k->data.numVal) {
			return buckets;
		}
		buckets = buckets->next;
	}

	return (avm_table_bucket*)NULL;
}






