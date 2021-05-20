#include<stdio.h>

int main () {

   FILE *fp;
   char buf[] = "";

   // fp = fopen( "file.txt" , "w");


   for(int i = 0; i < 0xFFFF; ++i) 
   {
       fwrite(buf, 1, sizeof(buf) , stderr);
   }

   // fclose(fp);
  
   return(0);
}



