#include <unistd.h>  //Для getwd
#include <limits.h>  //Для PATH_MAX
#include <stdio.h>   //Для printf

int main (void)
{    
   // Буфер в который будет помещен путь
   // к текущей директории 
   char PathName[PATH_MAX];
   // Переменная, в которую буднт помещен указатель на PathName
   char PN;

   // Определяем путь к текущей директории  
   PN = getwd(PathName);
   
   // Вывод результата на консоль  
   if (PN == NULL) printf (“Ошибка определения пути”);
   else printf (“Текущая директория: %s\n”,PathName);

   return 0;
}
