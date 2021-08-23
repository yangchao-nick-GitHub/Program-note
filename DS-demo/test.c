#include <stdio.h>
#define BIGGEST_NUM

int main()
{
   int a = 14;
   int b = 8;
   int tmp = 0;
   while((a % b) != 0){
       tmp = a;
       a = b;
       b = tmp % b;
       fprintf(stdout,"%d %d\n",a,b);
   }

    fprintf(stdout,"res num : %d\n",b);
    return 0;
}