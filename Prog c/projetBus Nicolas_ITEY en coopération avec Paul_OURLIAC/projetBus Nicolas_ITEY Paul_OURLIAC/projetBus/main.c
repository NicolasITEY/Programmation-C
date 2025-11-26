//NOM prénom étudiant 1 :
//NOM prénom étudiant 2 :

#include "SDL.h"
#include "maSDL.h"
#include "SDL_VSYNC.h"

//#include "types.h"
#include "ligneBus.h"
#include "listeDouble.h"
#include "triliste.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*--------- Main ---------------------*/
int main(int argc, char* argv[])
{
    //SDL
    //fênetre dans windows
    SDL_Window *gWindow = NULL;
    SDL_Init(SDL_INIT_VIDEO);

    //la texture (chargée dans la ram de la carte graphique) target representant tout le jeu
    SDL_Renderer* gRenderer = NULL;

    //Les textures, le type Ltexture est défini dans SDL_VSYNC.h
    LTexture gSpriteBus1;
    LTexture gSpriteBus2;
    LTexture gSpriteBus3;
    LTexture gSpriteBus4;
    LTexture gSpriteArretBus;

    //Start up SDL and create window
	if( !init(&gWindow,&gRenderer) )  //initialise la fenetre SDL et charge UNE texture
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadAnimation(&gSpriteBus1, gRenderer, "./data/4bus.png")
            || !loadAnimation(&gSpriteBus2, gRenderer, "./data/4bus.png")
            || !loadAnimation(&gSpriteBus3, gRenderer, "./data/4bus.png")
            || !loadAnimation(&gSpriteBus4, gRenderer, "./data/4bus.png")
            || !loadSprite(&gSpriteArretBus, gRenderer, "./data/arret_bus_50x27.png"))

        {
			fprintf(stdout,"Échec de chargement du sprite (%s)\n",SDL_GetError());
		}
		else
		{
        //A COMMENTER quand vous en aurez assez de cliquer sur ces popups ^^
        //message("Welcome in BusProject","Ceci est un point de depart de votre future interface de vos lignes de Bus");
        //message("et fin","ECHAP->quitter, + vos touches + D/V pour sauvegarer/restaurer un bus et le reseau de lignes");

        /**********************************************************************/
        /*                                                                    */
        /*              DEFINISSEZ/INITIALISER ICI VOS VARIABLES              */
        /*                                                                    */
        /**********************************************************************/

        int nbrdeligne=3;

        //TlisteStation *lignes = creedeslignesdebus(&nbrdeligne);
        TlisteStation *lignes = convertV2();




        printf("%d\n",nbrdeligne);
        //création d'un (seul) bus
        printf("ok");
        Tbus bus1 = creeBus(1,lignes[0]);
        Tbus bus2 = creeBus(2,lignes[1]);
        Tbus bus3 = creeBus(3,lignes[2]);
        Tbus bus4;
        //affiche sur la console les stations et troncons des lignes de bus


        //Current animation frame
        int frame = 0;

        //affiche et initialise le sprite du bus au départ
        Affiche_Sprite(&gSpriteBus1, gRenderer, getPosXBus( bus1 ), getPosYBus( bus1 ), getIdFrame(frame));
        Affiche_Sprite(&gSpriteBus2, gRenderer, getPosXBus( bus2 ), getPosYBus( bus2 ), getIdFrame(frame));
        Affiche_Sprite(&gSpriteBus3, gRenderer, getPosXBus( bus3 ), getPosYBus( bus3 ), getIdFrame(frame));

        // boucle principale du jeu
        int cont = 1;
        int  incXDeplSpriteBus1 = 0, incYDeplSpriteBus1 = 0; //déplacement du sprite du bus à chaque passage dans la boucle principale
        int  incXDeplSpriteBus2 = 0, incYDeplSpriteBus2 = 0; //déplacement du sprite du bus à chaque passage dans la boucle principale
        int  incXDeplSpriteBus3 = 0, incYDeplSpriteBus3 = 0; //déplacement du sprite du bus à chaque passage dans la boucle principale
        int  incXDeplSpriteBus4 = 0, incYDeplSpriteBus4 = 4; //déplacement du sprite du bus à chaque passage dans la boucle principale

        bool isboucle = false ;
        while ( cont != 0 ){
                SDL_PumpEvents(); //do events

                /***********************************************************************/
                /*                                                                     */
                //APPELEZ ICI VOS FONCTIONS QUI FONT EVOLUER LE "JEU"
                /*                                                                     */
                /***********************************************************************/

                //deplaceBus gère le déplacement du bus sur sa ligne, selon son sens du parcours de la ligne;
                //met à jour les variations en X et Y pour déplacer le sprite du Bus (cf ligne 151)
                deplaceBus(bus1, getSensParcours(bus1), &incXDeplSpriteBus1, &incYDeplSpriteBus1);
                deplaceBus(bus2, getSensParcours(bus2), &incXDeplSpriteBus2, &incYDeplSpriteBus2);
                deplaceBus(bus3, getSensParcours(bus3), &incXDeplSpriteBus3, &incYDeplSpriteBus3);
                if (nbrdeligne == 4){
                        deplaceBus(bus4, getSensParcours(bus4), &incXDeplSpriteBus4, &incYDeplSpriteBus4);
                }
                //afficheCoordonneesBus( bus1 );  //utile?

                const Uint8* pKeyStates = SDL_GetKeyboardState(NULL);
                //les touches sont lues en Qwerty
                if ( pKeyStates[SDL_SCANCODE_W] ){

                        /* Ajouter vos appels de fonctions ci-dessous qd le joueur appuye sur Z */
                        printf("\nTouche Z, sens déplacement depart vers terminus\n");
                        setSensParcours(bus1, depart_vers_arrivee );
                }
                if ( pKeyStates[SDL_SCANCODE_S] ){

                        printf("\nTouche S, sens déplacement terminus vers depart\n");
                        setSensParcours(bus1, arrivee_vers_depart );
                }

                if ( pKeyStates[SDL_SCANCODE_1] ){

                        printf("\nTouche 1, Bus au depart de la ligne 1\n");
                        busSurStation(bus1, lignes[0], depart_vers_arrivee);
                        Affiche_Sprite(&gSpriteBus1, gRenderer, getPosXBus( bus1 ), getPosYBus( bus1 ), getIdFrame(frame));
                }
                if ( pKeyStates[SDL_SCANCODE_2] ){

                        printf("\nTouche 2, Bus au départ de la ligne 2\n");
                        busSurStation(bus1, lignes[1], depart_vers_arrivee);
                        Affiche_Sprite(&gSpriteBus1, gRenderer, getPosXBus( bus1 ), getPosYBus( bus1 ), getIdFrame(frame));
                }
                if ( pKeyStates[SDL_SCANCODE_3] ){

                        printf("\nTouche 3, Bus au départ de la ligne 3\n");
                        busSurStation(bus1, lignes[2], depart_vers_arrivee);
                        Affiche_Sprite(&gSpriteBus1, gRenderer, getPosXBus( bus1 ), getPosYBus( bus1 ), getIdFrame(frame));
                }
                if ( pKeyStates[SDL_SCANCODE_4] ){
                        if (nbrdeligne == 4){
                            printf("\nTouche 4, Bus au départ de la ligne 4\n");
                            busSurStation(bus1, lignes[3], depart_vers_arrivee);
                            Affiche_Sprite(&gSpriteBus1, gRenderer, getPosXBus( bus1 ), getPosYBus( bus1 ), getIdFrame(frame));
                        }
                }

                if ( pKeyStates[SDL_SCANCODE_ESCAPE] ){

                        printf("\nTouche ECHAP");
                        cont = 0;  //sortie de la boucle
                }
                if ( pKeyStates[SDL_SCANCODE_T] ){
                        if (getIdLigneActuelleDuBus(bus1)==1){

                            printf("\nTouche T, tri des troncon;\n");
                            TlisteStation listetroncon = getlistetroncon(lignes[0]);
                            printf("\nTri par cout de maintenance :\n");
                            tri_selection_liste(listetroncon,comp_coutmaintenance);
                            afficheListe(listetroncon);
                            printf("\nTri par date de maintenance :\n");
                            tri_selection_liste(listetroncon,compdatedemaintenance);
                            afficheListe(listetroncon);

                        }
                        if(getIdLigneActuelleDuBus(bus1)==2) {

                            printf("\nTouche T, tri des troncon;\n");
                            TlisteStation listetroncon = getlistetroncon(lignes[1]);
                            printf("\nTri par cout de maintenance :\n");
                            tri_selection_liste(listetroncon,comp_coutmaintenance);
                            afficheListe(listetroncon);
                            printf("\nTri par date de maintenance :\n");
                            tri_selection_liste(listetroncon,compdatedemaintenance);
                            afficheListe(listetroncon);

                        }
                        if(getIdLigneActuelleDuBus(bus1)==3) {

                            printf("\nTouche T, tri des troncon;\n");
                            TlisteStation listetroncon = getlistetroncon(lignes[2]);
                            printf("\nTri par cout de maintenance :\n");
                            tri_selection_liste(listetroncon,comp_coutmaintenance);
                            afficheListe(listetroncon);
                            printf("\nTri par date de maintenance :\n");
                            tri_selection_liste(listetroncon,compdatedemaintenance);
                            afficheListe(listetroncon);

                        }

                }
                if ( pKeyStates[SDL_SCANCODE_Q] ){
                        if (getIdLigneActuelleDuBus(bus1)==1){

                            printf("\nTouche A, tri des arret;\n");
                            TlisteStation listearret = getlistearret(lignes[0]);
                            printf("\nTri par cout de maintenance :\n");
                            tri_selection_liste(listearret,comp_coutmaintenance);
                            afficheListe(listearret);
                            printf("\nTri par date de maintenance :\n");
                            tri_selection_liste(listearret,compdatedemaintenance);
                            afficheListe(listearret);

                        }
                        if(getIdLigneActuelleDuBus(bus1)==2) {

                            printf("\nTouche A, tri des arret;\n");
                            TlisteStation listearret = getlistearret(lignes[1]);
                            printf("\nTri par cout de maintenance :\n");
                            tri_selection_liste(listearret,comp_coutmaintenance);
                            afficheListe(listearret);
                            printf("\nTri par date de maintenance :\n");
                            tri_selection_liste(listearret,compdatedemaintenance);
                            afficheListe(listearret);

                        }
                        if(getIdLigneActuelleDuBus(bus1)==3) {

                            printf("\nTouche A, tri des arret;\n");
                            TlisteStation listearret = getlistearret(lignes[2]);
                            printf("\nTri par cout de maintenance :\n");
                            tri_selection_liste(listearret,comp_coutmaintenance);
                            afficheListe(listearret);
                            printf("\nTri par date de maintenance :\n");
                            tri_selection_liste(listearret,compdatedemaintenance);
                            afficheListe(listearret);

                        }


                }
                if ( pKeyStates[SDL_SCANCODE_F] ){

                    printf("\nTouche F, fusion de ligne;\n");
                    if (nbrdeligne != 4){
                        lignes = createNewListeFromFusion(lignes,&nbrdeligne,0,1);
                        bus4 = creeBus(4,lignes[3]);
                        Affiche_Sprite(&gSpriteBus4, gRenderer, getPosXBus( bus4 ), getPosYBus( bus4 ), getIdFrame(frame));

                    }
                    //afficheListe(lignes[4]);
                    printf("\n%d\n",nbrdeligne);
                }
                if ( pKeyStates[SDL_SCANCODE_X] ) {
					int a;
					if (getIdLigneActuelleDuBus(bus1)==1) {
						afficheListe(lignes[0]);
						printf("Nous voici dans la ligne de bus 1, quelle station souhaiter vous suprimer \n");
						scanf("%d",&a);
						lignes[0] = Suppstation(lignes[0],a);
						afficheListe(lignes[0]);
						busSurStation(bus1, lignes[0], depart_vers_arrivee);
						Affiche_Sprite(&gSpriteBus1, gRenderer, getPosXBus( bus1 ), getPosYBus( bus1 ), getIdFrame(frame));

					}
					if (getIdLigneActuelleDuBus(bus1)==2) {
						afficheListe(lignes[1]);
						printf("Nous voici dans la ligne de bus 2, qu'elle station souhaiter vous suprimer \n");
						scanf("%d",&a);
						lignes[1] = Suppstation(lignes[1],a);

						busSurStation(bus1, lignes[1], depart_vers_arrivee);
						Affiche_Sprite(&gSpriteBus1, gRenderer, getPosXBus( bus1 ), getPosYBus( bus1 ), getIdFrame(frame));

					}
					if (getIdLigneActuelleDuBus(bus1)==3) {
						afficheListe(lignes[2]);
						printf("Nous voici dans la ligne de bus 3 , qu'elle station souhaiter vous suprimer \n");
						scanf("%d",&a);
						lignes[2] = Suppstation(lignes[2],a);
						afficheListe(lignes[2]);
						busSurStation(bus1, lignes[2], depart_vers_arrivee);
						Affiche_Sprite(&gSpriteBus1, gRenderer, getPosXBus( bus1 ), getPosYBus( bus1 ), getIdFrame(frame));

					}
                }
                if ( pKeyStates[SDL_SCANCODE_B] ){


                    if (!isboucle){
                        printf("\nTouche B, creation de boucle;\n");
                        ligneboucle(lignes[0]);
                        isboucle = true;

                    }
                }


                //affichage du jeu à chaque tour

                //on efface toute la fenêtre
                efface_fenetre_texture(gRenderer);

                //Déplacement de sprite du bus sur la texture
                Deplace_Sprite(&gSpriteBus1, gRenderer, incXDeplSpriteBus1,incYDeplSpriteBus1,getIdFrame(frame));
                Deplace_Sprite(&gSpriteBus2, gRenderer, incXDeplSpriteBus2,incYDeplSpriteBus2,getIdFrame(frame));
                Deplace_Sprite(&gSpriteBus3, gRenderer, incXDeplSpriteBus3,incYDeplSpriteBus3,getIdFrame(frame));
                if (nbrdeligne == 4){
                        Deplace_Sprite(&gSpriteBus4, gRenderer, incXDeplSpriteBus4,incYDeplSpriteBus4,getIdFrame(frame));
                }
                //réaffichage à chaque tour de toutes les stations
                for (int i = 0; i < nbrdeligne; i++){
                        if (i == 0 && isboucle){
                            DessineUneLigneBusboucle(lignes[i], gSpriteArretBus, gRenderer);

                        } else {
                            DessineUneLigneBus(lignes[i], gSpriteArretBus, gRenderer);
                        }

                }
                //affichage de la texture ainsi mis à jour
                maj_fenetre_texture(gRenderer);

                SDL_Delay(1); //valeur du délai modifiable en fonction de votre CPU
                ++frame;
            }
        //fin boucle du jeu
        }
	}


	//Free resources and close SDL
	free_Ltexture(&gSpriteBus1);
	free_Ltexture(&gSpriteBus2);
	free_Ltexture(&gSpriteBus3);
	free_Ltexture(&gSpriteArretBus);
	close(gWindow, gRenderer);

    return 0;
}
