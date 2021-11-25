//Begimay KONUSHBAEVA
//Joel KONGOLO
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "table.h"
#define LMAX 128

void controller(PTable_entry table, int N){
    
    //on initialise la commande à 0 on met sa valeur entrée par l'utilisateur dans la suite 
    int commande=0;
    // k est la cle que l'on demande a l'utilisateur dans la suite 
    int k; 
    // v est la chaine que l'on demande à l'utilisateur dans la suite
    char v[128];
    // on declare le tableau de N tubes 
    int t[N][2];
    for(int i=0; i<N; i++){
    // on ouvre chaque tube
        pipe(t[i]);
    }
    //le tube où les node ecrivent pour le controller
    int tube[2];
    pipe(tube);

//
    for(int i=0; i < N; i++){
    // la boucle pour créer N processus fils 
        if(fork()==0){
        //on ferme le tube où les nodes n'ont pas le droit de lire
            close(tube[0]); 
            int in;
            // in est le tube en extremité de lecture
            if(i==0){
            // dans le node 0 on lit du dernier node ( donc node N-1)
                in = t[N-1][0];
            }else{
            // sinon on lit dans le node precedent
                in =  t[i-1][0];
            }
            int out;
            // out est le tube en extremité de l'ecriture

            out = t[i][1];
            
            //on ferme tous les tubes dont on n'aura pas besoin
            for(int i=0; i<N; i++){
                if(t[i][0]!=in){
                // tous les tubes a part in en extremité de lecture
                    close(t[i][0]);
                }
                if(t[i][1]!=out){
                // tous les tubes a part out en extremité de l'ecrirure
                    close(t[i][1]);
                }
            }
            while(1){
                //on lit la commande du node precedent et de node N-1 pour le node 0
                read(in, &commande, sizeof(int));
                
                
                switch(commande){
                //comportement de chaque commande
                    case 0:
                    // commande exit reçue dans tous les nodes
                    //on tue chaque processus fils
                        exit(0);
                    case 1:
                    // commande set reçue 
                        read(in, &k, sizeof(int));
                    // on lit la cle que nous avons demandé au controller
                    
                        read(in, v, LMAX*(sizeof(char)));
                    //on lit la chaine de caracteres que nous avons demandé au controller

                        if(i==k%N){
                        //si le bon node est atteint
                            store(&table, k, v);
                        // la cle aveec la valeur entrée seront stokée dans le dictionnaire de ce processus
                        }
                        else{
                        //sinon on passe commande, cle et la valeur au node suivant
                            write(out, &commande, sizeof(int));
                            write(out, &k, sizeof(int));
                            write(out, v, sizeof(v));
                        }

                    break;
                    case 2:
                        //commande 2 reçue
                        //on lit la cle que nous avons demandé au controller
                        read(in, &k, sizeof(int));
                        
                        if(i==k%N){
                        // si le bon node est atteint
                        //on recherche la valeur dans le dictionnaire de ce node
                            char *val = lookup(table, k);
                        //on envoie la valeur trouvée au controller
                            write(tube[1], val, sizeof(val));
                        
                        }else{
                        //sinon on passe la commande avec la cle au node suivant
                            write(out, &commande, sizeof(int));
                            write(out, &k, sizeof(int));
                        }
                    break;
                     case 3:
                        //commande 3 reçue

                        if(table->next !=NULL){
                        //si le dictionnaire de node n'est pas vide
                            printf("process : %d\n", getpid());
                        //on affiche ce dictionnaire
                            display(table);
                            sleep(1);
                        }
                    break;
                }
            }


            exit(0);
        }
    
    
    }
    sleep(1);
  //on ferme le tube du controller ou il peut pas ecrire
    close(tube[1]);
    int continuer = 1;
    //si continuer est 1 on continue a interagir avec l'utilisateur
    while(continuer){
        //on demande le controller de saisir la commande
        printf("Saisir commande (0 = exit, 1 = set, 2 = lookup, 3 = dump): ");
        scanf("%d", &commande);
        
        switch(commande){
        //comportement de chaque commande
            case 0:
            //La commande 0: exit
                for(int i=0; i<N; i++){
                //la commande 0 est envoyé a tous les fils simultanement
                    write(t[i][1], &commande, sizeof(int));
                }
                printf("bye bye !\n");
                //Tous les processus fils meurs, le programme fini l'execution
                continuer = 0;
                break;
            case 1:
            //La commande 1: set
                printf("Saisir la cle (decimal number): ");
                scanf(" %d", &k);
                //on rdemande au controller de saisir la cle
                printf("Saisir la valeur (chaine de caracteres, max 128 chars) : ");
                //on demande au controller de saisir la valeur
                scanf("%s", v);

                //on ecrit ces données 1 par 1 dans le processus node 0
                write(t[0][1], &commande, sizeof(int));
                write(t[0][1], &k, sizeof(int));
                write(t[0][1], v, sizeof(v));
                //on attends l'execution des processus node
                sleep(1);
            break;
            case 2:
            //commande 2 : lookup
            //on demande au controller de saisir la cle
                printf("Saisir la cle (decimal number): ");
                scanf(" %d", &k);

            //on ecrit ces données dans le processus node 0
                write(t[0][1], &commande, sizeof(int));
                write(t[0][1], &k, sizeof(int));
            //on attends l'execution des processus node
                sleep(1);
                read(tube[1], v, sizeof(v));
                printf("Valeur trouvée = %s\n", v);
            break;
            case 3:
            //commande 3 : dump
                for(int i=0; i<N; i++){
                //cette commande est envoyé a tous les processus node simultanement
                    write(t[i][1], &commande, sizeof(int));
                }
                //on attends l'execution des processus node
                sleep(1);
            break;
        }
    }
   

    
}

int main(int argc, char **argv){
    // N est le nombre de processus à créer
    int N;
    if(argc == 2){
        //gestion des cas d'erreur d'entree
        if(atoi(argv[1])==0 || atoi(argv[1])<=0){
            perror("Mauvais argument");
        }else{
            // recuperation de N passé en argument de l'execution
            N = atoi(argv[1]);
            // on initialise un dictionnaire vide
        PTable_entry table = (PTable_entry)(malloc(sizeof(PTable_entry))); 
        //appel a la fonction controller
        controller(table, N);
        // on attends la mort de tous les processus fils
        while(wait(NULL)!=-1);
        }
    // gestion des cas d'erreur de l'entrée
    }else{
        perror("Mauvais nombre d'arguments");
    }
    
}