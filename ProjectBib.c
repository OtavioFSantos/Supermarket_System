#include"ProjectBib.h"
int SIZE;

struct prod{
    char name[30];
	int code, amountstock;
	float price;
};

struct ListProd{
	struct prod prod;
	struct ListProd *next;
};

struct prod* product = NULL;



void createList(void){
	int i;
	FILE *fp;
	fp = fopen("stock.txt", "r");
	if(fp == NULL){ 
        fprintf(stderr, "\nError Stock.\n"); 
        exit(0); 
    } 
	for(i = 0; i < SIZE; ++i){
		fscanf(fp, "%s%d%f%d", (product + i)->name, &(product + i)->code, &(product + i)->price, &(product + i)->amountstock);
	}
	fclose(fp);
}

int menu(void){	
	int choice = 0;
	do{
		printf("\n\n\n===== Supermarket =====\n");
		printf("1. Buy a product;\n");
        printf("2. Remove merchandise from stock;\n");
		printf("3. Register a new merchandise;\n");
		printf("4. Check stock;\n");
		printf("5. Update product in stock;\n");
		printf("6. Exit.\n");
		printf("===== Please, choose what you want to do: "); scanf("%d", &choice);
		if(choice < 1 || choice > 6)
			printf("\nPlease, type a valid number.\n");
	}while(choice < 1 || choice > 6);
	getchar();
	return choice;
}

void check(void){
	int c;
	FILE *fp;
	fp = fopen("stock.txt", "r");
	if(fp == NULL){ 
        fprintf(stderr, "\nError Stock.\n"); 
        exit(0); 
    }
	printf("\n\n\n----- Stock -----\n");
	for(c = 0; c < SIZE; ++c){
        printf("Product %d:\n\nName: %s\nCode: %d\n", c+1, (product + c)->name, (product + c)->code);
		printf("Price: %.2f$\nAmount in Stock: %d\n", (product + c)->price, (product + c)->amountstock);
		printf("---------------------------------\n");
	}
	fclose(fp);
}

void regist(void){
	struct ListProd *start, *list = createChain();
	int choice, testId = 0, i;
	start = list;
	FILE *fp;
	fp = fopen("stock.txt", "a");
	if(fp == NULL){ 
        fprintf(stderr, "\nError Stock.\n"); 
        exit(0); 
    }
	printf("\n\n\n----- Register -----\n");
	do{
		printf("Enter the product name: "); scanf("%s", (product)->name);
        do{
			testId = 0;
            printf("Enter the product code: "); scanf("%d", &(product)->code);
            for(i = 0, list = start; i < SIZE; i++, list = list->next){
                if(list->prod.code==(product)->code){
                    printf("Code is already in use to: \"%s\".",list->prod.name);
                    i = SIZE;
                    testId = 1;
                }
            }
        }while(testId);

		printf("Enter the product price: "); scanf("%f", &(product)->price);
		printf("Enter the amount in stock: "); scanf("%d", &(product)->amountstock);
		fprintf(fp,"%s %d %.2f %d\n", (product)->name, (product)->code, (product)->price, (product)->amountstock);
		SIZE++;
		updateSize();
		printf("\nDo you want to register another product(1.Yes 2.No)? "); scanf("%d", &choice);
		printf("---------------------------------\n");
	}while(choice == 1);
	fclose(fp);
	cleanMemory(list);
	createList();
}

void buy(void){
	int code, amount, c, choice;
	float money, finalPrice = 0;
	FILE *fpinvoice, *fptemp;
	fpinvoice = fopen("invoice.txt", "w");
	if(fpinvoice == NULL){ 
        fprintf(stderr, "\nError Invoice.\n"); 
        exit(0); 
    }
	fprintf(fpinvoice, "INVOICE\n");
	printf("\n\n\n----- Buy -----\n");
	do{
		printf("\nEnter the product code to be purchased: "); scanf("%d", &code);
		fptemp = fopen("buy.tmp", "w");
		if(fptemp == NULL){ 
        	fprintf(stderr, "\nError Buy.\n"); 
        	exit(0); 
    	}
		for(c = 0; c < SIZE; ++c){
			if((product+c)->code == code){
				if((product+c)->amountstock <= 0){
					printf("Product out of stock.\n");
					break;
				}else{
					do{
						printf("Enter the quantity to be purchased(0 - %d): ", (product+c)->amountstock); scanf("%d", &amount);
					}while(amount > (product+c)->amountstock || amount < 0);
					printf("Price: %.2fR$\n", amount*(product+c)->price);
					finalPrice += amount*(product+c)->price;
					(product+c)->amountstock = (product+c)->amountstock - amount;
				}
				fprintf(fpinvoice,"Purchased product: %s\nCode: %d\nAmount: %d\n", (product+c)->name, (product+c)->code, amount);
				fprintf(fpinvoice,"Price: %.2f$\n-------------------\n", amount*(product+c)->price);
			}
			fprintf(fptemp,"%s %d %.2f %d\n", (product+c)->name, (product+c)->code, (product+c)->price, (product+c)->amountstock);
		}
		fclose(fptemp);
		do{
			printf("\nDo you want to buy one more product(1.Yes 2.No)? "); scanf("%d", &choice);
		}while(choice < 1 || choice > 2);
	}while(choice == 1);
	printf("Final Price: %.2fR$\nEnter the money received: ", finalPrice); scanf("%f", &money);
	while(money < finalPrice){
		printf("Insufficient money! Missing %.2f$\nEnter the money received again: ", (money-finalPrice)*-1); scanf("%f", &money);
	}
	printf("Small change: %.2f$\n", money - finalPrice);
	fprintf(fpinvoice, "Final price: %.2f$\nMoney received: %.2f$\nSmall change: %.2f$\n", finalPrice, money, money - finalPrice);
	fclose(fpinvoice);
	remove("stock.txt");
	rename("buy.tmp", "stock.txt");
}

void update(void){
	int choice, code, c;
	FILE *fp;
	printf("\n\n\n----- Update -----\n");
	do{
		printf("\nEnter the product code to be updated: "); scanf("%d", &code);
		fp = fopen("update.tmp", "w");
		if(fp == NULL){ 
			fprintf(stderr, "\nError Update.\n"); 
			exit(0); 
		}
		for(c = 0; c < SIZE; ++c){
			if((product+c)->code == code){
				printf("Enter the new price of %s: ", (product+c)->name); scanf("%f", &(product+c)->price);
				printf("Enter the new quantity in stock: "); scanf("%d", &(product+c)->amountstock);
			}
			fprintf(fp,"%s %d %.2f %d\n", (product+c)->name, (product+c)->code, (product+c)->price, (product+c)->amountstock);
		}
		fclose(fp);
		do{
			printf("\nDo you want to update one more product(1.Yes 2.No)? "); scanf("%d", &choice);
		}while(choice < 1 || choice > 2);
	}while(choice == 1);
	remove("stock.txt");
	rename("update.tmp", "stock.txt");
}

void remov(void){
	int i, r, aux = 0, testId = 1;
	struct ListProd *temp, *list, *start;
	FILE *fp;
	printf("\n\n\n----- Remove -----\n");
	printf("\nEnter the product code to be removed:");
	scanf("%d",&r);

	list = createChain();
	start = list;

    for(i = 0, list = start; i < SIZE; i++, list = list->next){
        if(list->prod.code == r){
            testId = 0;
        }
    }
	if(testId){
		printf("Invalid code.");
	}else{
		list = start;

		for(i = 0; i < SIZE; i++, list = list->next){
			if(list->prod.code==r){
				aux = 1;
				if(i == 0){
					start = list->next;
				}else{
					temp->next = list->next;
				}
				free(list);
				i = SIZE--;
			}else{
				temp = list;
			}
		}
		updateSize();
		fp = fopen("stock.txt", "w");
		if(fp == NULL){ 
			fprintf(stderr, "\nError Stock.\n"); 
			exit(0);
		}
		list = start;
		for(i = 0; i < SIZE; i++, list = list->next){
			fprintf(fp , "%s %d %.2f %d\n", list->prod.name, list->prod.code, list->prod.price, list->prod.amountstock);
		}
		if(aux)
			printf("\nProduct successfully removed.\n");
		fclose(fp);
		cleanMemory(list);
		createList();
	}
}

void size(){
	FILE *fp;
	fp = fopen("size.txt", "r");
	if(fp == NULL){ 
        fprintf(stderr, "\nError Size.\n"); 
        exit(0); 
    }
	fscanf(fp, "%d", &SIZE);
	fclose(fp);
}

void updateSize(){
	FILE *fp;
	fp = fopen("size.txt", "w");
	if(fp == NULL){ 
        fprintf(stderr, "\nError Size.\n"); 
        exit(0); 
    }
	fprintf(fp,"%d",SIZE);
	fclose(fp);
}

struct ListProd *createChain(){
    FILE *fp;
    int i;
	struct ListProd *list, *start;

    list = (struct ListProd *) malloc(sizeof(struct ListProd));
    start = list;

    fp = fopen("stock.txt","r");
	if(fp == NULL){ 
        fprintf(stderr, "\nError Stock.\n"); 
        exit(0); 
    }

    for(i = 0; i < SIZE; i++, list = list->next){
		fscanf(fp,"%s",list->prod.name);
		fscanf(fp,"%d",&list->prod.code);
		fscanf(fp,"%f",&list->prod.price);
		fscanf(fp,"%d",&list->prod.amountstock);
		list->next = (struct ListProd *) malloc(sizeof(struct ListProd));
		list->next->next = NULL;
	}
	
    fclose(fp);
	return (struct ListProd *)start;
}

void cleanMemory(struct ListProd *list){
	struct ListProd *temp;
	for(; list->next != NULL; list = temp){
		temp = list->next;
		free(list);
	}
}