#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "Structures.h"

#define SIZEX 32
#define SIZEY 12

void changeDoorCondi (Door tab[], int Doornumber)
{
    tab[Doornumber].condi = -(tab[Doornumber].condi);
}


bool conditionSC (Player player, Door doors[], int numDoor, int DoorCondi)
{
    bool deplacPossible;

    switch (DoorCondi)
    {
        case  1 : deplacPossible = (player.coords.x == doors[numDoor].coords.x && player.coords.y == doors[numDoor].coords.y && doors[numDoor].condi ==  1); break;

        case -1 : deplacPossible = (player.coords.x == doors[numDoor].coords.x && player.coords.y == doors[numDoor].coords.y && doors[numDoor].condi == -1); break;

    }
    return deplacPossible;
}


void MoveMonster (int keybind, char dungeon[2][SIZEY][SIZEX+1], Mobs mobs[])
{
    for (int i = 0; i < 4; i++)
    {
        switch (keybind)
        {
        case 5 :
        case 3 :
        case 2 :
        case 1 :
                  // Si le mob est collé au mur gauche.
                  if (dungeon[1][mobs[i].coords.y][mobs[i].coords.x - 1] == '#')
                  {
                      mobs[i].placement = 1;
                  }

                  // Si le mob est collé au mur droit.
                  if (dungeon[1][mobs[i].coords.y][mobs[i].coords.x + 1] == '#')
                  {
                      mobs[i].placement = 2;
                  }

                  switch (mobs[i].placement)
                  {
                      case 1 : if (dungeon[1][mobs[i].coords.y][mobs[i].coords.x + 1] != '#')
                                {
                                    // Déplacement jusqu'au mur de droite.
                                    mobs[i].coords.x++;
                                    break;
                                }
                      case 2 : if (dungeon[1][mobs[i].coords.y][mobs[i].coords.x - 1] != '#')
                                {
                                    // Déplacement jusqu'au mur de gauche.
                                    mobs[i].coords.x--;
                                    break;
                                }
                  }
        }
    }
}

int MoveBox (int x1, int y1, int x2, int y2, Coord box[], char dungeon[2][SIZEY][SIZEX+1], int goal)
{
    for (int i = 0; i < 3; i++)
    {
        if (x1 == box[i].x && y1 == box[i].y) // Si la case devant le joueur est une boîte.
        {
            if (dungeon[1][y2][x2] == 'H') // Si la case devant la caisse est un objectif.
            {
                box[i].x = x2;
                box[i].y = y2;
                dungeon[1][y2][x2] = 'C';
                dungeon[1][y1][x1] = ' ';
                goal++;
                printf("╔═════════════════════════════╗\n");
                printf("║    Oвjecтiғ plαcé ! (%d/3)   ║\n", goal);
                printf("╚═════════════════════════════╝\n\n");
            }
            else
            {
                box[i].x = x2;
                box[i].y = y2;
                dungeon[1][y2][x2] = 'C';
                dungeon[1][y1][x1] = ' ';
            }
        }
    }
    return goal;
}


void showMap (Player player, char dungeon[2][SIZEY][SIZEX+1], int numLab, Door doors[], Mobs mobs[])
{
    int i;
    int m = 0;
    if (numLab == 0)
         i = 0;
    else
         i = 6;


        for (int y = 0; y < SIZEY; y++)
        {
            for (int x = 0; x < SIZEX; x++)
            {
                if (numLab == 1 && x == mobs[m].coords.x && y == mobs[m].coords.y)
                {
                    printf("☻");
                    m++;
                }
                else if (x == player.coords.x && y == player.coords.y)
                {
                    printf("P");
                }
                else
                {
                    switch (dungeon[numLab][y][x])
                    {
                        case '#' : if (y == 0)
                                   {
                                       printf("▄"); break;
                                   }
                                   else if (y == SIZEY - 1)
                                   {
                                       printf("▀"); break;
                                   }
                                   else
                                   {
                                       printf("█");break;
                                   }
                        case 'D' : if (doors[i].condi == 1)
                                   {
                                       printf("Ø");
                                       i++; break;
                                   }
                                   else
                                   {
                                       printf("O");
                                       i++; break;
                                   }
                        case 'A' :
                        case ' ' : printf(" "); break;
                        case 'B' :
                        case 'T' :
                        case 'F' : printf("o"); break;
                        case 'E' : printf("║"); break;
                        case 'K' : printf("δ"); break;
                        case 'P' : printf(" "); break;
                        case 'L' : printf("▒"); break;
                        case '1' : printf("1"); break;
                        case '2' : printf("2"); break;
                        case '3' : printf("3"); break;
                        case 'C' : printf("Ξ"); break;
                        case 'H' : printf("☼"); break;
                        case 'M' : printf(" "); break;
                        case 'R' : printf("◊"); break;
                        case 'U' : printf("◙"); break;
                        case 'J' : printf("Ψ"); break;
                        case 'Z' : printf("◘"); break;

                    }
                }
            }
            printf("\n");
        }
}

int MainMenu()
{
    int answer = 0;
    int play = 0;
    int quit = 0;

    do
    {

        printf("╔════════════════════════╗\n");
        printf("║     Meɴυ Priɴcipαl     ║\n");
        printf("║ 1 ► Joυer              ║\n");
        printf("║ 2 ► Rèɢleѕ dυ jeυ      ║\n");
        printf("║ 3 ► Qυiттer            ║\n");
        printf("╚════════════════════════╝\n");

        scanf("%d", &answer);

        switch (answer)
        {
            case 1 : play = 1; break;
            case 2 : printf("╔════════════════════════════════════════════════════╗\n");
                     printf("║                   Rèɢleѕ dυ jeυ                    ║\n");
                     printf("║Niveαυ 1 :                                          ║\n");
                     printf("║► Voυѕ devez voυѕ écнαpper dυ lαвyriɴтнe.           ║\n");
                     printf("║► Leѕ porтeѕ ғerмéeѕ ѕoɴт repréѕeɴтéeѕ pαr deѕ 'Ø'. ║\n");
                     printf("║► Leѕ porтeѕ oυverтeѕ ѕoɴт repréѕeɴтéeѕ pαr deѕ 'O'.║\n");
                     printf("║► Leѕ вoυтoɴѕ ѕoɴт repréѕeɴтéѕ pαr deѕ 'o'.         ║\n");
                     printf("║Voтre вυт eѕт d'αcтiver leѕ вoυтoɴѕ αғiɴ d'oυvrir   ║\n");
                     printf("║dιғғéreɴтeѕ porтeѕ eт de voυѕ ғrαyer υɴ cнeмιɴ      ║\n");
                     printf("║jυѕqυ'ὰ lα ѕorтιe ('║') eɴ évιтαɴт leѕ pιèɢeѕ !     ║\n");
                     printf("║Il voυѕ ғαυdrα récυpérer lα clé repréѕeɴтée pαr υɴ  ║\n");
                     printf("║'δ' αғιɴ de déverroυιller lα ѕorтιe !               ║\n");
                     printf("║                                                    ║\n");
                     printf("║Niveαυ 2 :                                          ║\n");
                     printf("║► Deѕ iɴѕтrυcтιoɴѕ αppαrαîтroɴт αυ coυrѕ de ce      ║\n");
                     printf("║ɴiveαυ αғιɴ de voυѕ diriɢer. Fαiтeѕ-y αттeɴтioɴ.    ║\n");
                     printf("║                                                    ║\n");
                     printf("║1     ► Reveɴir αυ мeɴυ priɴcipαl                   ║\n");
                     printf("║Autre ► Qυiттer le jeυ                              ║\n");
                     printf("╚════════════════════════════════════════════════════╝\n");


                     int ans = 0;
                     scanf("%d", &ans);

                     if (ans == 1)
                        break;


            case 3 : quit = 1;
            return quit;
            break;
        }

    } while (play != 1);
    return 2;

}
int main()
{
    if (!SetConsoleOutputCP(65001))
    printf ("ERREUR\n");

    ///////////////DÉCLARATION DES VARIABLES/////////////////

    char dungeon [2][SIZEY][SIZEX+1] = {
                                        {
                                            "###################             ",
                                            "E        D  B#B  P#             ",
                                            "#B      F#   D    #             ",
                                            "##D#######   #    #             ",
                                            "#   ######   ###D##             ",
                                            "#   ######   #    #             ",
                                            "#   ######   #B   #             ",
                                            "#  F#######D##### #             ",
                                            "##D#######   #### #             ",
                                            "#      ###   #### #             ",
                                            "#K     ###        #             ",
                                            "###################             "  },

                                        {
                                            "################################",
                                            "#H#       #      Z#     ##    A#",
                                            "#         C       #B ##D#####D##",
                                            "#  #########C##   ####L L##    #",
                                            "###    #####  ##D###    R##M   #",
                                            "###           #U   # LLLL##   M#",
                                            "###   #####C# #    #  LLL##    #",
                                            "##H   H####   #    #L   L##M   #",
                                            "###############    #LLL L##   M#",
                                            "#                  #23  J##    #",
                                            "E                  #1    D    B#",
                                            "################################" } };


    Player player;
    Door doors[10];
    Coord buttons[7];
    Mobs mobs[4];
    Coord Box[3];
    Coord Arrive;
    Coord Totem;
    Coord TP;
    Coord ResetButton;
    int end = 0;
    int key = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    int c = 0;
    player.hp = 10;
    int precedentX = 0;
    int precedentY = 0;
    int numLab = 0;
    int leave = 0;



    ///////////// AFFICHAGE DU MENU PRINCIPAL /////////////

    printf("╔════════════════════════╗\n");
    printf("║     Bienvenue dans     ║\n");
    printf("║    нiмiтѕυ ɴo мeiro    ║\n");
    printf("╚════════════════════════╝\n");

    int Menu = MainMenu();
    if (Menu == 1)
    {
        printf("╔═══════════════════════╗\n");
        printf("║   Oɴ qυiттe le jeυ !  ║\n");
        printf("╚═══════════════════════╝\n\n");
        return 0;
    }


    printf("╔════════════════════════════════════════╗\n");
    printf("║   Téléporтαтioɴ dαɴѕ le lαвyriɴтнe...  ║\n");
    printf("║             Boɴɴe Cнαɴce !             ║\n");
    printf("╚════════════════════════════════════════╝\n\n");

////////////////INITIALISATION DES COORDS DU PLAYER////////////////

    for (int numLab = 0; numLab < 2; numLab++)
    {
        for (int y = 0; y < SIZEY; y++)
        {
            for (int x = 0; x < SIZEX; x++)
            {
                switch(dungeon[numLab][y][x])
                {
                    case 'P' : player.coords.x = x;
                               player.coords.y = y;
                               break;
                    case 'D' : doors[i].coords.x = x;
                               doors[i].coords.y = y;
                               doors[i].condi = 1;
                               i++;
                               break;
                    case 'B' : buttons[j].x = x;
                               buttons[j].y = y;
                               j++;
                               break;
                    case 'M' : mobs[k].coords.x = x;
                               mobs[k].coords.y = y;
                               k++;
                               break;
                    case 'A' : Arrive.x = x;
                               Arrive.y = y;
                               break;
                    case 'J' : Totem.x = x;
                               Totem.y = y;
                               break;
                    case 'U' : TP.x = x;
                               TP.y = y;
                               break;
                    case 'C' : Box[c].x = x;
                               Box[c].y = y;
                               c++;
                               break;
                }
            }
        }
    }


///////////////////////////LOGIQUE DU JEU/////////////////////////////
////////////////////////////PREMIÈRE MAP//////////////////////////////

    do
    {
        ////////////////////////// AFFICHAGE DES INFOS DU JOUEUR //////////////////////////

        printf("HP : ");
        for (int i = 0; i < player.hp; i++)
        {
            printf("■");
        }
        printf(" %d\n", player.hp);

        //////////////////////////// AFFICHAGE DE L'AIR DE JEU ///////////////////////////

        showMap(player, dungeon, 0, doors, mobs);

        //////////////////////////////// GESTION DES COMMANDES //////////////////////////////

        printf ("\n");
        printf ("5 : ▲ Hαυт\n");
        printf ("1 : ◄ Gαυcнe\n");
        printf ("3 : ► Droiтe\n");
        printf ("2 : ▼ Bαѕ\n");
        printf ("8 : Sorтir dυ proɢrαммe\n");
        printf ("\n");

        printf ("Voтre cнoiх  : ");

        int keybind = 0;
        scanf ("%d", &keybind);

        printf ("\n\n");

            ////////////////////// CONDITIONS POUR LES MURS ET PORTES /////////////////////////


        switch ( keybind )
         {
          case 5 : if ( dungeon[numLab][player.coords.y-1][player.coords.x   ] != '#' ) player.coords.y--;
                   if (conditionSC(player, doors, 2,  1)) player.coords.y++;
                   if (conditionSC(player, doors, 2, -1)) player.coords.y = doors[2].coords.y - 1;
                   if (conditionSC(player, doors, 3,  1)) player.coords.y++;
                   if (conditionSC(player, doors, 3, -1)) player.coords.y = doors[3].coords.y - 1;
                   if (conditionSC(player, doors, 4,  1)) player.coords.y++;
                   if (conditionSC(player, doors, 4, -1)) player.coords.y = doors[4].coords.y - 1;
                   if (conditionSC(player, doors, 5,  1)) player.coords.y++;
                   if (conditionSC(player, doors, 5, -1)) player.coords.y = doors[5].coords.y - 1;
                   break;
          case 1 : if ( dungeon[numLab][player.coords.y  ][player.coords.x -1] != '#' ) player.coords.x--;
                   if (conditionSC(player, doors, 1,  1)) player.coords.x++;
                   if (conditionSC(player, doors, 1, -1)) player.coords.x = doors[1].coords.x - 1;
                   if (conditionSC(player, doors, 0,  1)) player.coords.x++;
                   if (conditionSC(player, doors, 0, -1)) player.coords.x = doors[0].coords.x - 1;
                   break;
          case 3 : if ( dungeon[numLab][player.coords.y  ][player.coords.x +1] != '#' ) player.coords.x++;
                   if (conditionSC(player, doors, 1,  1)) player.coords.x--;
                   if (conditionSC(player, doors, 1, -1)) player.coords.x = doors[1].coords.x + 1;
                   if (conditionSC(player, doors, 0,  1)) player.coords.x--;
                   if (conditionSC(player, doors, 0, -1)) player.coords.x = doors[0].coords.x + 1;
                   break;
          case 2 : if ( dungeon[numLab][player.coords.y+1][player.coords.x   ] != '#' ) player.coords.y++;
                   if (conditionSC(player, doors, 2,  1)) player.coords.y--;
                   if (conditionSC(player, doors, 2, -1)) player.coords.y = doors[2].coords.y + 1;
                   if (conditionSC(player, doors, 3,  1)) player.coords.y--;
                   if (conditionSC(player, doors, 3, -1)) player.coords.y = doors[3].coords.y + 1;
                   if (conditionSC(player, doors, 4,  1)) player.coords.y--;
                   if (conditionSC(player, doors, 4, -1)) player.coords.y = doors[4].coords.y + 1;
                   if (conditionSC(player, doors, 5,  1)) player.coords.y--;
                   if (conditionSC(player, doors, 5, -1)) player.coords.y = doors[5].coords.y + 1;
                   break;
          case 8 : end = 1;
                   printf("╔═══════════════════════╗\n");
                   printf("║   Oɴ qυiттe le jeυ !  ║\n");
                   printf("╚═══════════════════════╝\n\n");
                   leave = 1;
         }


         ////////////////////// CHANGEMENT DE L'ÉTAT DE LA PORTE (OUVERTE/FERMÉE) //////////////////////


        if (player.coords.x != precedentX || player.coords.y != precedentY) // Condition pour ne pas réactiver le bouton si le joueur reste sur le bouton.
        {
             if (buttons[0].x == player.coords.x && buttons[0].y == player.coords.y)
             {
                 changeDoorCondi(doors, 1);
                 changeDoorCondi(doors, 2);
             }
             if (buttons[1].x == player.coords.x && buttons[1].y == player.coords.y)
             {
                 changeDoorCondi(doors, 3);
                 changeDoorCondi(doors, 0);
             }
             if (buttons[2].x == player.coords.x && buttons[2].y == player.coords.y)
             {
                 changeDoorCondi(doors, 5);
                 changeDoorCondi(doors, 2);
             }
             if (buttons[3].x == player.coords.x && buttons[3].y == player.coords.y)
             {
                 changeDoorCondi(doors, 4);
                 changeDoorCondi(doors, 0);
             }
        }

        //////////////////////// RÉCUPÉRATION DE LA CLÉ ///////////////////////

        if (dungeon[numLab][player.coords.y][player.coords.x] == 'K')
        {
            dungeon[numLab][player.coords.y][player.coords.x] = ' ';
            key = 1;
            printf("╔══════════════════════════════════════════════╗\n");
            printf("║    Voυѕ αvez oвтeɴυ lα clé de lα ѕorтie !    ║\n");
            printf("╚══════════════════════════════════════════════╝\n\n");
            dungeon[numLab][10][6] = 'T';
        }

        /////////////////////// GESTION DU BOUTON SECRET //////////////////////

        if (dungeon[numLab][player.coords.y][player.coords.x] == 'T')
        {
            player.coords.x = 17;
            player.coords.y = 1;
            for (int i = 0; i < 6; i++)
            {
                doors[i].condi = 1;
            }
        }

        ///////////////////// GESTION DE LA PORTE DE SORTIE ////////////////////

        if (dungeon[numLab][player.coords.y][player.coords.x] == 'E' && key != 1)
        {
            printf("╔═══════════════════════════════════════════════════════╗\n");
            printf("║   Voυѕ αvez вeѕoiɴ d'υɴe clé poυr oυvrir ceттe porтe  ║\n");
            printf("╚═══════════════════════════════════════════════════════╝\n\n");
            player.coords.x++;
        }
        if (dungeon[numLab][player.coords.y][player.coords.x] == 'E' && key == 1)
        {
            printf("╔════════════════════════════════════════════════════╗\n");
            printf("║     Brαvo ! Voυѕ αvez ғiɴi le preмier ɴiveαυ !     ║\n");
            printf("╚════════════════════════════════════════════════════╝\n\n");
            player.coords.x = Arrive.x;
            player.coords.y = Arrive.y;
            end = 1;
        }

        /////////////////////////// GESTION DES HP /////////////////////////////

        if (dungeon[numLab][player.coords.y][player.coords.x] == 'F')
        {
            dungeon[numLab][player.coords.y][player.coords.x] = ' ';
            printf("╔═════════════════════════════════════════════════╗\n");
            printf("║     Voυѕ êтeѕ тoмвé dαɴѕ υɴ pιèɢe (-5 HP) !     ║\n");
            printf("╚═════════════════════════════════════════════════╝\n\n");
            player.hp -= 5;
        }

        if (player.hp <= 0)
        {
            printf("HP : %d\n\n", player.hp);
            printf("╔═══════════════════════════════════╗\n");
            printf("║     Voυѕ êтeѕ мorт : GAME OVER    ║\n");
            printf("╚═══════════════════════════════════╝\n\n");
            end = 2;
        }

        precedentX = player.coords.x;
        precedentY = player.coords.y;
    }
    while (end != 1 && end != 2);

    if (end == 2)
        return 0;
    if (leave == 1)
        return 0;

    printf("╔═════════════════════════════════════════════════╗\n");
    printf("║     Voυѕ αrrivez dαɴѕ υɴe ɴoυvelle ѕαlle...     ║\n");
    printf("╚═════════════════════════════════════════════════╝\n\n");

    // Déclaration des nouvelles variables.
    numLab = 1;
    end = 0;
    int var1 = 0;
    int var2 = 0;
    int var3 = 0;
    int hammer = 0;
    int pos_precX = 0;
    int pos_precY = 0;
    int goal = 0;
    int z = 0;
    key = 0;

    do
    {
        //////////////////////////// AFFICHAGE DE L'AIR DE JEU ///////////////////////////

        doors[7].condi = -1; // On ouvre la première porte.
        showMap(player, dungeon, 1, doors, mobs);

        printf ("\n");
        printf ("5 : ▲ Hαυт\n");
        printf ("1 : ◄ Gαυcнe\n");
        printf ("3 : ► Droiтe\n");
        printf ("2 : ▼ Bαѕ\n");
        printf ("8 : Sorтir dυ proɢrαммe\n");
        printf ("\n");

        printf ("Voтre cнoiх  : ");

        int keybind = 0;
        scanf ("%d", &keybind);

        printf ("\n\n");

        ///////////////////// GESTION DES NOUVEAUX DÉPLACEMENTS ///////////////////

        switch (keybind)
        {
            case 5 : // Condition pour les déplacements basiques (Murs et Totem).
                     if  (dungeon[numLab][player.coords.y - 1][player.coords.x] == '#'  ||  dungeon[numLab][player.coords.y - 1][player.coords.x] == 'J') break;
                     // Condition pour les déplacements avec caisses.
                     if ((dungeon[numLab][player.coords.y - 1][player.coords.x] == 'C') && (dungeon[numLab][player.coords.y - 2][player.coords.x] == '#' || dungeon[numLab][player.coords.y - 2][player.coords.x] == 'C')) break;
                     // Déplacement de la caisse.
                     goal = MoveBox(player.coords.x, player.coords.y - 1, player.coords.x, player.coords.y - 2, Box, dungeon, goal);
                     // Déplacement du joueur.
                     player.coords.y--;

                     // Condition pour ne pas passer les portes fermées mais passer les portes ouvertes.

                     if (conditionSC(player, doors, 6,   1)) player.coords.y++;
                     if (conditionSC(player, doors, 6,  -1)) player.coords.y = doors[6].coords.y - 1;
                     if (conditionSC(player, doors, 7,   1)) player.coords.y++;
                     if (conditionSC(player, doors, 7,  -1)) player.coords.y = doors[7].coords.y - 1;
                     if (conditionSC(player, doors, 8,   1)) player.coords.y++;
                     if (conditionSC(player, doors, 8,  -1)) player.coords.y = doors[8].coords.y - 1;
                     break;

            case 1 : // Condition pour les déplacements basiques.
                     if  (dungeon[numLab][player.coords.y][player.coords.x - 1] == '#') break;
                     // Condition pour les déplacements avec caisses.
                     if ((dungeon[numLab][player.coords.y][player.coords.x - 1] == 'C') && (dungeon[numLab][player.coords.y][player.coords.x - 2] == '#' || dungeon[numLab][player.coords.y][player.coords.x - 2] == 'C')) break;
                     // Déplacement de la caisse.
                     goal = MoveBox(player.coords.x - 1, player.coords.y, player.coords.x - 2, player.coords.y, Box, dungeon, goal);
                     // Déplacement du joueur.
                     player.coords.x--;

                     // Condition pour ne pas passer les portes fermées mais passer les portes ouvertes.

                     if (conditionSC(player, doors, 9,  1)) player.coords.x++;
                     if (conditionSC(player, doors, 9, -1)) player.coords.x = doors[9].coords.x - 1;

                     // Condition si on se déplace et on rentre dans un monstre.

                     for (int i = 0; i < 4; i++)
                     {
                         if (player.coords.y == mobs[i].coords.y && player.coords.x == mobs[i].coords.x)
                         {
                             printf("╔══════════════════════════════════════════════╗\n");
                             printf("║    Voυѕ voυѕ êтeѕ ғαiт тυé pαr υɴ мoɴѕтre    ║\n");
                             printf("╚══════════════════════════════════════════════╝\n\n");
                             end = 1;
                         }
                     }
                     break;

            case 3 : // Condition pour les déplacements basiques (Murs, Totem et Caillou).
                     if  (dungeon[numLab][player.coords.y][player.coords.x + 1] == '#'  ||  dungeon[numLab][player.coords.y][player.coords.x + 1] == 'J' || dungeon[numLab][player.coords.y][player.coords.x + 1] == 'R') break;
                     // Condition pour les déplacements avec caisses.
                     if ((dungeon[numLab][player.coords.y][player.coords.x + 1] == 'C') && (dungeon[numLab][player.coords.y][player.coords.x + 2] == '#' || dungeon[numLab][player.coords.y][player.coords.x + 2] == 'C')) break;
                     // Déplacement de la caisse.
                     goal = MoveBox(player.coords.x + 1, player.coords.y, player.coords.x + 2, player.coords.y, Box, dungeon, goal);
                     // Déplacement du joueur.
                     player.coords.x++;

                     // Condition pour ne pas passer les portes fermées mais passer les portes ouvertes.

                     if (conditionSC(player, doors, 9,  1)) player.coords.x--;
                     if (conditionSC(player, doors, 9, -1)) player.coords.x = doors[9].coords.x + 1;

                     // Condition si on se déplace et on rentre dans un monstre.

                     for (int i = 0; i < 4; i++)
                     {
                         if (player.coords.y == mobs[i].coords.y && player.coords.x == mobs[i].coords.x)
                         {
                             printf("╔══════════════════════════════════════════════╗\n");
                             printf("║    Voυѕ voυѕ êтeѕ ғαiт тυé pαr υɴ мoɴѕтre    ║\n");
                             printf("╚══════════════════════════════════════════════╝\n\n");
                             end = 1;
                         }
                     }
                     break;

            case 2 : // Condition pour les déplacements basiques.
                     if  (dungeon[numLab][player.coords.y + 1][player.coords.x] == '#') break;
                     // Condition pour les déplacements avec caisses.
                     if ((dungeon[numLab][player.coords.y + 1][player.coords.x] == 'C') && (dungeon[numLab][player.coords.y + 2][player.coords.x] == '#' || dungeon[numLab][player.coords.y + 2][player.coords.x] == 'C')) break;
                     // Déplacement de la caisse.
                     goal = MoveBox(player.coords.x, player.coords.y + 1, player.coords.x, player.coords.y + 2, Box, dungeon, goal);
                     // Déplacement du joueur.
                     player.coords.y++;

                     // Condition pour ne pas passer les portes fermées mais passer les portes ouvertes.

                     if (conditionSC(player, doors, 6,   1)) player.coords.y--;
                     if (conditionSC(player, doors, 6,  -1)) player.coords.y = doors[6].coords.y + 1;
                     if (conditionSC(player, doors, 7,   1)) player.coords.y--;
                     if (conditionSC(player, doors, 7,  -1)) player.coords.y = doors[7].coords.y + 1;
                     if (conditionSC(player, doors, 8,   1)) player.coords.y--;
                     if (conditionSC(player, doors, 8,  -1)) player.coords.y = doors[8].coords.y + 1;
                     break;

            case 8 : end = 1;
                     printf("╔═══════════════════════╗\n");
                     printf("║   Oɴ qυiттe le jeυ !  ║\n");
                     printf("╚═══════════════════════╝\n\n");
        }

        // Explication de la première étape.

        if (player.coords.x == doors[7].coords.x && player.coords.y == doors[7].coords.y + 1)
        {
            printf("╔═════════════════════════════════════════════╗\n");
            printf("║    Éviтez leѕ мoɴѕтreѕ poυr ѕυrvivre '☻'!   ║\n");
            printf("╚═════════════════════════════════════════════╝\n\n");
        }

        ////// DÉPLACEMEENT DES MONSTRES //////

        MoveMonster(keybind, dungeon, mobs);

        // Si le joueur rencontre un monstre il meurt.

        for (int i = 0; i < 4; i++)
        {
            if (player.coords.x == mobs[i].coords.x && player.coords.y == mobs[i].coords.y)
            {
                printf("╔══════════════════════════════════════════════╗\n");
                printf("║    Voυѕ voυѕ êтeѕ ғαiт тυé pαr υɴ мoɴѕтre    ║\n");
                printf("╚══════════════════════════════════════════════╝\n\n");
                end = 1;
            }
        }

        /////// OUVERTURE DE LA PORTE APRÈS LA PREMIÈRE ÉPREUVE ///////

        if (player.coords.x != pos_precX || player.coords.y != pos_precY) // Condition pour ne pas réactiver le bouton si le joueur reste sur le bouton.
        {
             if (buttons[5].x == player.coords.x && buttons[5].y == player.coords.y)
             {
                 changeDoorCondi(doors, 9);
             }
        }

        //////// AFFICHAGE DU SECRET ////////

        if (player.coords.x == Totem.x && player.coords.y - 1 == Totem.y)
        {
            printf("╔════════════════════════════════════════════════════╗\n");
            printf("║     Voυѕ reɴcoɴтrez υɴ тoтeм ѕυr leqυel il y α     ║\n");
            printf("║      écriт : 'ѕαvez-voυѕ coмpтez jυѕqυ'ὰ 3 ?'      ║\n");
            printf("║     Coɴѕeil : Éviтez lα lαve ('▒').                ║\n");
            printf("╚════════════════════════════════════════════════════╝\n\n");
        }

        //////////// TOUTES LES CONDITIONS POUR QUE SEULEMENT SI LE JOUEUR FAIT DANS L'ORDRE "123", IL OBTIENNE UN MARTEAU ////////////

        if (dungeon[1][player.coords.y][player.coords.x] == '1')
        {
            var1 = 1;
        }
        else if (dungeon[1][player.coords.y][player.coords.x] == '2' && dungeon[1][pos_precY][pos_precX] == '1' && var1 == 1)
        {
            var2 = 1;
        }
        else if (dungeon[1][player.coords.y][player.coords.x] == '3' && dungeon[1][pos_precY][pos_precX] == '2' && var2 == 1)
        {
            hammer = 1;
            printf("╔═════════════════════════════════════╗\n");
            printf("║   Voυѕ αvez oвтeɴυ υɴ мαrтeαυ !     ║\n");
            printf("║   Peυт-êтre poυvez-voυѕ déтrυire    ║\n");
            printf("║   υɴ cαilloυ αvec ce derɴier ('◊')  ║\n");
            printf("╚═════════════════════════════════════╝\n\n");
        }


        // Si le joueur va sur la lave, il meurt.

        if (dungeon[1][player.coords.y][player.coords.x] == 'L')
        {
            printf("╔═══════════════════════════════════════╗\n");
            printf("║     Voυѕ êтeѕ тoмвé dαɴѕ lα lαve !    ║\n");
            printf("╚═══════════════════════════════════════╝\n\n");
            end = 1;
        }

        //////////////// CONDITION SI LE JOUEUR EST DEVANT LE CAILLOU À CASSER ///////////////

        if (dungeon[1][player.coords.y][player.coords.x + 1] == 'R')
        {
            if (hammer == 1)
            {
                printf("╔════════════════════════════════════════════════════════════════╗\n");
                printf("║     Voυlez-voυѕ cαѕѕez le cαilloυ ? ('o' : oυi | 'ɴ' : ɴoɴ)    ║\n");
                printf("╚════════════════════════════════════════════════════════════════╝\n\n");
                char ans = 0;
                scanf("%c", &ans);
                switch (ans)
                {
                    case 'o' :  dungeon[1][player.coords.y][player.coords.x + 1] = 'B';
                                buttons[6].x = player.coords.x + 1;
                                buttons[6].y = player.coords.y;
                                printf("╔══════════════════════════════════════════════╗\n");
                                printf("║     Il y αvαiт υɴ вoυтoɴ ѕoυѕ le cαilloυ !   ║\n");
                                printf("╚══════════════════════════════════════════════╝\n\n");
                                break;
                    case 'n' :  printf("╔══════════════════════════════════════════════╗\n");
                                printf("║    Voυѕ ɴe voυlez pαѕ cαѕѕer le cαilloυ...   ║\n");
                                printf("╚══════════════════════════════════════════════╝\n\n");
                                player.coords.x--;
                               break;
                    default : break;
                }
            }
            else
            {
                printf("╔═══════════════════════════════════════════════════════════╗\n");
                printf("║   Il voυѕ ғαυdrαiт υɴ мαrтeαυ poυr cαѕѕer ce cαilloυ...   ║\n");
                printf("╚═══════════════════════════════════════════════════════════╝\n\n");
            }
        }


        /////// OUVERTURE DE LA PORTE APRÈS LA DEUXIÈME ÉPREUVE ///////

        if (player.coords.x != pos_precX || player.coords.y != pos_precY) // Condition pour ne pas réactiver le bouton si le joueur reste sur le bouton.
        {
             if (buttons[6].x == player.coords.x && buttons[6].y == player.coords.y)
             {
                 changeDoorCondi(doors, 6);
             }
        }

        // Téléportation just avant la dernière salle, ouverture de la porte de cette dernière salle.
        if (player.coords.x == buttons[4].x && player.coords.y == buttons[4].y)
        {
                player.coords.x = TP.x;
                player.coords.y = TP.y;
                changeDoorCondi(doors, 8);
                printf("╔═══════════════════════════════╗\n");
                printf("║    Voυѕ voυѕ êтeѕ тéléporтé   ║\n");
                printf("╚═══════════════════════════════╝\n\n");
        }


        // Explication de l'objectif de la salle.

        if (player.coords.x == doors[8].coords.x && player.coords.y == doors[8].coords.y - 1 && goal != 3)
        {
            printf("╔════════════════════════════════════════════════════╗\n");
            printf("║   Voυѕ eɴтrez dαɴѕ υɴe ɴoυvelle ѕαlle avec des     ║\n");
            printf("║   caisses 'Ξ' et des plateformes '☼'. Peυт-êтre    ║\n");
            printf("║   devriez-voυѕ мeттre cнαqυe cαiѕѕe ѕυr υɴe        ║\n");
            printf("║   plαтeғorмe ?                                     ║\n");
            printf("║   Si voυѕ вloqυez υɴe cαiѕѕe, voυѕ poυvez reѕeт le ║\n");
            printf("║   ɴiveαυ eɴ αppυyαɴт ѕυr le вoυтoɴ eɴ нαυт ὰ droiтe║\n");
            printf("║   de lα ѕαlle '◘'.                                 ║\n");
            printf("╚════════════════════════════════════════════════════╝\n\n");
        }

        // BOUTON RESET QUI REMET LES CAISSES À LEUR POSITIONS INITIALES

        if (dungeon[1][player.coords.y][player.coords.x] == 'Z' && goal != 3)
        {
            // Suppression de la caisse.
            dungeon[1][Box[0].y][Box[0].x] = ' ';
            // Repositionnement à la position initiale.
            dungeon[1][2][10] = 'C';
            // Remise des bonnes coordonnées.
            Box[0].x = 10;
            Box[0].y = 2;

            dungeon[1][Box[1].y][Box[1].x] = ' ';
            dungeon[1][3][12] = 'C';
            Box[1].x = 12;
            Box[1].y = 3;
            dungeon[1][Box[2].y][Box[2].x] = ' ';
            dungeon[1][6][11] = 'C';
            Box[2].x = 11;
            Box[2].y = 6;

            // Repositionnement des objectifs si jamais ils ont disparu.

            dungeon[1][1][1] = 'H';
            dungeon[1][7][2] = 'H';
            dungeon[1][7][6] = 'H';
            goal = 0;
        }

        // OBTENTION DE LA CLÉ DE SORTIE SI LES 3 CAISSES SONT SUR LES OBJECTIFS

        if (goal == 3)
        {
            key = 1;
            if (z == 0)
            {
                printf("╔══════════════════════════════════════════════╗\n");
                printf("║    Voυѕ αvez oвтeɴυ lα clé de lα ѕorтie !    ║\n");
                printf("╚══════════════════════════════════════════════╝\n\n");
                z = 1;
            }
        }

        // CONDITION POUR LA PORTE DE SORTIE

        if (dungeon[numLab][player.coords.y][player.coords.x] == 'E' && key != 1)
        {
            printf("╔═══════════════════════════════════════════════════════╗\n");
            printf("║   Voυѕ αvez вeѕoiɴ d'υɴe clé poυr oυvrir ceттe porтe  ║\n");
            printf("╚═══════════════════════════════════════════════════════╝\n\n");
            player.coords.x++;
        }
        if (dungeon[numLab][player.coords.y][player.coords.x] == 'E' && key == 1)
        {
            printf("╔════════════════════════════════════════════════════╗\n");
            printf("║   Brαvo ! Voυѕ êтeѕ ѕorтi dυ lαвyriɴтнe vivαɴт !   ║\n");
            printf("╚════════════════════════════════════════════════════╝\n\n");
            player.coords.x = Arrive.x;
            player.coords.y = Arrive.y;
            end = 1;
        }

        pos_precX = player.coords.x;
        pos_precY = player.coords.y;

    } while (end != 1);


return 0;
}
