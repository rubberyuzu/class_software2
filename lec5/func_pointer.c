#include <stdio.h>
#define SIZE 10

int add_one(int x){
	return x+1;
}
int add_two(int x){
	return x+2;
}

void apply(int v[SIZE], int (*fp)(int)){
	int i;
	for (i = 0; i < SIZE; ++i){
		v[i] = (*fp)(v[i]);
	}
}

int main(){
	int i;
	int v[SIZE];

	for (i = 0; i < SIZE; ++i){
		v[i] = i;
	}

	apply(v, add_one);
	apply(v, add_two);

	for (i = 0; i < SIZE; ++i){
		printf("%dth = %d\n",i, v[i]);
	}
}