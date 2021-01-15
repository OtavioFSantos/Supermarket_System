/*
Supermarket System
Author: Otávio Fernandes dos Santos
*/

#include <stdio.h>
#include <stdlib.h>
#include"ProjectBib.h"
#include"ProjectBib.c"

int main(){
    int choice;
	size();
    product = (struct prod *)malloc(SIZE * sizeof(struct prod));
	createList();
	createChain();
    for (;;){
		choice = menu();
		switch (choice){
		case 1:
			buy();
			break;
		case 2:
			remov();
			break;
		case 3:
			regist();
			break;
        case 4:
			check();
			break;
		case 5:
			update();
			break;
		case 6:
			exit(0);
			break;
		}
	}
	return 0;
}