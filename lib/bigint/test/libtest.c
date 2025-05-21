#include "bigint.h"
#include <stdio.h>


int main(){
	BigInt x = big_int_constructor(0, 4, 0x446F6E27742067, 0x6F20757020746865, 0x72652E2E2E204974, 0x2773206461726B2E);
	printf(x.hex);
}
