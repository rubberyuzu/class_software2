#include <stdio.h>
#include <stdlib.h>

void shuffle(int ary[],int size)
{
    for(int i=0;i<size;i++)
    {
        int j = rand()%size;
        int t = ary[i];
        ary[i] = ary[j];
        ary[j] = t;
    }
}

int main(){
    int list[10]={0,1,2,3,4,5,6,7,8,9};
    shuffle(list,10);
    int i;
    for (i = 0; i < 10; ++i)
    {
    	printf("%d\n", list[i]);
    }
}