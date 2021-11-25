//Begimay KONUSHBAEVA
//Joel KONGOLO
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "table.h"
#define LMAX 128

void controller(int N){
    
    //on initialise la commande à 0 on met sa valeur entrée par l'utilisateur dans la suite 
    int commande=0;
    
    int t[N][2];

    // Ouverture des N pipes +1 pour le père
    for(int i=0; i<N+1; i++){
        if(pipe(t[i])!=-1){
          pipe(t[i]);
        }else{
          perror("Erreur de l'ouverture de tube");
        }
        
    }
  //le tube où les processus fils ecrivent pour le processus pere
    int tube[2];
    if(pipe(tube)!=-1){
      pipe(tube);
    }else{
      perror("Erreur de l'ouverture de tube");
    }

    // la boucle pour créer N processus fils
    for(int i=0; i < N; i++){
        int n=fork();

        if(n==-1){                              //Cas d'erreur fork
            perror("fork");
        }
        if(n==0){
            close(tube[0]);
            int in;                             // in est le tube en extremité de lecture
            
            if(i==0){                           // Si on est dans le node 0, on lit du dernier node ( donc node N-1)
                in = t[N-1][0];
            }else{                              // sinon on lit dans le node precedent
                in =  t[i-1][0];
            }
            int out;                            // out est le tube en extremité de l'ecriture
            out = t[i][1];

            //Affichage des pid des processus fils
            printf("On a créé un processus Node %d, avec le pid %d \n", i,  getpid());

            //on lit la commande du node precedent et de node N-1 pour le node 0
            read(in, &commande, sizeof(int));

            //Affichage de la valeur lue pour chaque processus créé
            printf("La valeur %d lue par le Node %d ,avec le pid  : %d \n",commande,  i ,getpid());

            //Ecriture de la valeur lue
            commande= commande + 1; 
            //Pour ce test chaque processus fils incrémente de 1 la valeur reçue
            if(write(out, &commande, sizeof(int))!=-1){
        
              write(out, &commande, sizeof(int));
            }else{
            // cas d'erreur de la foction write
              perror("Erreur de l'ecriture");
            }
                
            //Affichage de la valeur écrite
            printf("La valeur %d ecrite par le Node %d ,avec le pid  : %d\n",commande, i,getpid());
            if(i==0){
              write(tube[1], &commande, sizeof(int));
            }
    
            exit(0);
        }
    
    
    }
    
    sleep(1);       //on attends 1s avant d'executer les instructions de processus pere
    close(tube[1]);
    

    printf("Saisir la commande : \n");          //On demande à l'utilisateur de saisir la commande 
    scanf("%d", &commande);                     //lecture de la commande sur l'entree standard
    write(t[0][1], &commande, sizeof(int));     //ecriture de la commande dans le node 0
    sleep(1);

    //Stocke la valeur finale qui doit revenir au pere
    int z;

    //Le père lit la valeur finale 
    read(tube[0], &z, sizeof(int));

    //Fermeture du pipe 
    close(t[N][0]);
    
    //Affichage de la valeur finale par le père
    printf("La valeur revenue au pere : %d\n", z);

    exit(1);
   
}

int main(int argc, char **argv){
    int N;          // N est le nombre de processus à créer
    if(argc == 2){
        //gestion des cas d'erreur d'entree
        if(atoi(argv[1])==0 || atoi(argv[1])<=0){
            perror("Mauvais argument");
        }else{
            // recuperation de N passé en argument de l'execution
            N = atoi(argv[1]);
            // on initialise un dictionnaire vide
            //appel a la fonction controller
            controller(N);
            // on attends la mort de tous les processus fils
            while(wait(NULL)!=-1);
        }
    // gestion des cas d'erreur de l'entrée
    }else{
        perror("Mauvais nombre d'arguments");
    }
    
}