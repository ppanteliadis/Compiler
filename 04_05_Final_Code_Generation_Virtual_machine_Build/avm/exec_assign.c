#include "oneheader.h"

char* vmtypes[]={
	"label_a",
	"global_a",
	"formal_a",
	"local_a",
	"number_a",
	"string_a",
	"bool_a",
	"nil_a",
	"userfunc_a",
	"libfunc_a",
	"retval_a"
};


char* memcell[] = {
	"number_m",
	"string_m",
	"bool_m",
	"table_m",
	"userfunc_m",
	"libfunc_m",
	"nil_m",
	"undef_m"
};


void avm_assign(avm_memcell* lv, avm_memcell* rv) {
	if (lv == rv) {					/* Same cells? Destructive to assign!*/
		return;
	}

	if (lv->type == table_m && 		/* Same tables? no need to assign! */
		rv->type == table_m && 
		lv->data.tableVal == rv->data.tableVal) {
		return;
	}

	if (rv->type == undef_m) {		/* From undefined r-value? warn! */
		avm_warning("assigning from \'undef\' content!");
	}

	avm_memcellclear(lv);			/* Clear old cell contents. */
	memcpy(lv, rv, sizeof(avm_memcell));

	/* Now take care of copied values or reference counting. */
	if (lv->type == string_m) {
		lv->data.strVal = strdup(rv->data.strVal);
	}
	else if (lv->type == table_m){
		avm_tableincrefcounter(lv->data.tableVal);
	}
}

void execute_assign(instruction* instr){
	avm_memcell* lv = avm_translate_operand(instr->result, (avm_memcell*)NULL);
	avm_memcell* rv = avm_translate_operand(instr->arg1, &ax);

	assert(lv && (&stack[AVM_STACKSIZE - 1] >= lv && lv > &stack[top] || lv == &retval));

	assert(rv);

	avm_assign(lv, rv);
}