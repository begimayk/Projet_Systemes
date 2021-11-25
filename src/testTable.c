//Begimay KONUSHBAEVA
//Joel KONGOLO
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "table.h"

int main(){
  
  int n;  //nombre de valeur à stocker dans la liste;
  
  char v[256];
   
   //on recupere le nombre des valeurs à stocker dans la liste
   printf("saisir le nombre de valeur dans la liste : ");
   scanf("%d",&n);
   
   //Allocation dynamique
   PTable_entry table =(PTable_entry) malloc(sizeof(Table_entry)); 
   
  // lecture des n valeurs
  for(int i=1;i<=n; i++){
    printf("Saisir la valeur n°%d\n",i);
    scanf("%s",v);
    store(&table, i,v);
  }
  
  //Affichage de la table
  printf("------------------------AFICHAGE DE LA TABLE------------------------\n");
  display(table);
  
  printf("------------------------RECHERCHE D'UNE VALEUR------------------------\n");
  
  //Recherche d'une valeur existante
  int clef;
  printf("saisir la clé : ");
  scanf("%d",&clef);
  char* res= malloc(sizeof(char));
  res=lookup(table, clef);
  if(res!=NULL){
    printf("la valeur trouvée est : ");
    for(int i=0; i<strlen(res);i++){
      printf("%c",res[i]);
    }
    printf("\n");
  }
  else {  //Dans le cas ou la valeur est absente
    printf("la valeur de la clé %d n'existe pas\n", clef);
  }
  
  
  
   return 0;  

}