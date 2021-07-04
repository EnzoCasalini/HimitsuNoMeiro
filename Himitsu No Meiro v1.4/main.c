#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include "constantes.h"

void changeDoorCondi (Door tab[], int Doornumber, char dungeon[2][SIZEY][SIZEX+1], int numLab, bool fake)
{
    if (fake != true)
    {
        tab[Doornumber].condi = -(tab[Doornumber].condi);
    }
    else
    {
        tab[Doornumber].condi = 1;
    }

    if (tab[Doornumber].condi == 1)
    {
        dungeon[numLab][tab[Doornumber].coords.y][tab[Doornumber].coords.x] = '5';
    }
    else
    {
        dungeon[numLab][tab[Doornumber].coords.y][tab[Doornumber].coords.x] = '1';
    }
}

bool conditionSC (SDL_Rect PlayerPos, Door doors[], int numDoor, int DoorCondi)
{
    bool deplacPossible;

    switch (DoorCondi)
    {
    case  1 :
        deplacPossible = (PlayerPos.x == doors[numDoor].coords.x && PlayerPos.y == doors[numDoor].coords.y && doors[numDoor].condi ==  1);
        break;

    case -1 :
        deplacPossible = (PlayerPos.x == doors[numDoor].coords.x && PlayerPos.y == doors[numDoor].coords.y && doors[numDoor].condi == -1);
        break;

    }
    return deplacPossible;
}

// Fonction permettant de copier la texture d'un rectangle à un autre afin d'afficher l'image du mur et du sol.

void ShowTexture (SDL_Renderer* renderer, SDL_Texture* texture, char dungeon[2][SIZEY][SIZEX+1], int numLab)
{
    SDL_Rect Rect_source;
    SDL_Rect Rect_destinataire;
    Rect_source.w = WIDTH_TILE;
    Rect_source.h = HEIGHT_TILE;
    Rect_destinataire.w = WIDTH_TILE;
    Rect_destinataire.h = HEIGHT_TILE;

    for (int i = 0; i < NB_BLOCS_WIDTH2; i++)
    {
        for (int j = 0; j < NB_BLOCS_HEIGHT2; j++)
        {
            Rect_destinataire.x = i * WIDTH_TILE;
            Rect_destinataire.y = j * HEIGHT_TILE;
            // Ici on utilise '-0' car en soustrayant 0 à un chiffre en ASCII on obtient le chiffre réel.
            // Cette ligne nous permet donc de récupérer soit le mur, soit le sol, en fonction de si on se trouve sur un '1' ou un '0' dans le tableau.
            Rect_source.x = (dungeon[numLab][j][i] - '0') * WIDTH_TILE;
            Rect_source.y = 0;
            SDL_RenderCopy(renderer, texture, &Rect_source, &Rect_destinataire);
        }
    }
}

int MoveBox (int x1, int y1, int x2, int y2, Coord box[], char dungeon[2][SIZEY][SIZEX+1], int goal)
{
    for (int i = 0; i < 3; i++)
    {
        if (x1 == box[i].x && y1 == box[i].y) // Si la case devant le joueur est une boîte.
        {
            if (dungeon[1][y2][x2] == '0') // Si la case devant la caisse est un objectif.
            {
                box[i].x = x2;
                box[i].y = y2;
                dungeon[1][y1][x1] = '1';
                dungeon[1][y2][x2] = 'M';
                goal++;
            }
            else
            {
                box[i].x = x2;
                box[i].y = y2;
                dungeon[1][y2][x2] = '4';
                dungeon[1][y1][x1] = '1';
            }
        }
    }
    return goal;
}

int MoveMonster (int tab[], char dungeon[2][SIZEY][SIZEX+1], Mobs mobs[], int numLab)
{
    if (numLab == 1)
    {
        for (int i = 0; i < 4; i++)
        {
            switch (tab[i])
            {
            case 0 :
            case 1 :
            case 2 :
            case 3 :
                      // Si le mob est collé au mur gauche.
                      if (dungeon[1][mobs[i].coords.y][mobs[i].coords.x - 1] == '8')
                      {
                          mobs[i].placement = 1;
                      }

                      // Si le mob est collé au mur droit.
                      if (dungeon[1][mobs[i].coords.y][mobs[i].coords.x + 1] == '8')
                      {
                          mobs[i].placement = 2;
                      }

                      switch (mobs[i].placement)
                      {
                          case 1 : if (dungeon[1][mobs[i].coords.y][mobs[i].coords.x + 1] != '8')
                                    {
                                        // Déplacement jusqu'au mur de droite.
                                        dungeon[1][mobs[i].coords.y][mobs[i].coords.x] = '1';
                                        mobs[i].coords.x++;
                                        dungeon[1][mobs[i].coords.y][mobs[i].coords.x] = 'E';
                                        break;
                                    }
                          case 2 : if (dungeon[1][mobs[i].coords.y][mobs[i].coords.x - 1] != '8')
                                    {
                                        // Déplacement jusqu'au mur de gauche.
                                        dungeon[1][mobs[i].coords.y][mobs[i].coords.x] = '1';
                                        mobs[i].coords.x--;
                                        dungeon[1][mobs[i].coords.y][mobs[i].coords.x] = 'F';
                                        break;
                                    }
                      }
            }
        }
    }
}


int main(int argc, char* argv[])
{
    // Définition de toutes les variables.

    SDL_Window   *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture  *textureJ = NULL, *TLaby = NULL, *TDialog = NULL, *TAstuce = NULL;
    SDL_Surface  *surfaceJ[4] = {NULL}, *SLaby = NULL, *SDialog = NULL, *SAstuce = NULL;
    SDL_Rect PlayerPos;
    Door doors[10];
    Coord buttons[8];
    Mobs mobs[4];
    int MobsMovement [4] = {1, 0, 1, 0};
    Coord Box[3];
    Coord Fairy;
    Coord Rock;
    Coord Gorilla;
    int end = 0;
    int key = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    int c = 0;
    int precedentX = 0;
    int precedentY = 0;
    int numLab = 0;
    int var1 = 0;
    int var2 = 0;
    int hammer = 0;
    int goal = 0;
    int tempsPrecedent = 0, tempsActuel = 0;
    int GorillaAnim = 0;
    int FirstTime = 0;
    int AnimationFinale = 0;

    // Création des surfaces des images qui changent en fonction d'une action.

    surfaceJ[DOWN] = IMG_Load("./Images/NewPlayerDown2.jpg");
    surfaceJ[UP] = IMG_Load("./Images/NewPlayerUp2.jpg");
    surfaceJ[LEFT] = IMG_Load("./Images/NewPlayerLeft2.jpg");
    surfaceJ[RIGHT] = IMG_Load("./Images/NewPlayerRight2.jpg");


    // Initialisation des diverses coordonnées.

    for (int numLab = 0; numLab < 2; numLab++)
    {
        for (int y = 0; y < SIZEY; y++)
        {
            for (int x = 0; x < SIZEX; x++)
            {
                switch(dungeon[numLab][y][x])
                {
                case '5' :
                    doors[i].coords.x = x;
                    doors[i].coords.y = y;
                    doors[i].condi = 1;
                    if (i == 8)
                    {
                        doors[i].condi = -1;
                    }
                    i++;
                    break;
                case '2' :
                    buttons[j].x = x;
                    buttons[j].y = y;
                    j++;
                    break;
                case 'E' :
                    mobs[k].coords.x = x;
                    mobs[k].coords.y = y;
                    k++;
                    break;
                case 'F' :
                    mobs[k].coords.x = x;
                    mobs[k].coords.y = y;
                    k++;
                case '?' :
                    Fairy.x = x;
                    Fairy.y = y;
                    break;
                case '4' :
                    Box[c].x = x;
                    Box[c].y = y;
                    c++;
                    break;
                case '7' :
                    Rock.x = x;
                    Rock.y = y;
                    break;
                case 'G' :
                    Gorilla.x = x;
                    Gorilla.y = y;
                }
            }
        }
    }


    // On fixe la position initiale du joueur.

    PlayerPos.x = 17;
    PlayerPos.y = 1;


    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    // Création de la fenêtre du jeu.

    window = SDL_CreateWindow("Labyrinthe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH_WINDOW, HEIGHT_WINDOW, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);


    // Création de la surface et de la tetxure du labyrinthe.


    SLaby = IMG_Load("./Images/Finally3.jpg");
    if (SLaby == NULL)
    {
        fprintf (stderr, "%s\n",SDL_GetError());
        return EXIT_FAILURE;
    }

    TLaby = SDL_CreateTextureFromSurface(renderer, SLaby);
    if (TLaby == NULL)
    {
        fprintf (stderr, "%s\n",SDL_GetError());
        return EXIT_FAILURE;
    }

    SDialog = IMG_Load("./Images/Dialog.jpg");
    if (SDialog == NULL)
    {
        fprintf (stderr, "%s\n",SDL_GetError());
        return EXIT_FAILURE;
    }

    TDialog = SDL_CreateTextureFromSurface(renderer, SDialog);
    if (TDialog == NULL)
    {
        fprintf (stderr, "%s\n",SDL_GetError());
        return EXIT_FAILURE;
    }

    SAstuce = IMG_Load("./Images/astuce.jpg");
    if (SAstuce == NULL)
    {
        fprintf (stderr, "%s\n",SDL_GetError());
        return EXIT_FAILURE;
    }

    TAstuce = SDL_CreateTextureFromSurface(renderer, SAstuce);
    if (TAstuce == NULL)
    {
        fprintf (stderr, "%s\n",SDL_GetError());
        return EXIT_FAILURE;
    }

    textureJ = SDL_CreateTextureFromSurface(renderer, surfaceJ[RIGHT]);

    // Rectangles (joueur, énigme, astuce)

    SDL_Rect rect;
    rect.x = WIDTH_TILE * PlayerPos.x;
    rect.y = HEIGHT_TILE * PlayerPos.y;
    rect.w = 40;
    rect.h = 40;

    SDL_Rect rect2;
    rect2.x = 780;
    rect2.y = 158;
    rect2.w = 400;
    rect2.h = 200;

    SDL_Rect rectAstuce;
    rectAstuce.x = 500;
    rectAstuce.y = 396;
    rectAstuce.w = 290;
    rectAstuce.h = 84;

    // Gestion des évènements.

    while (end != 1)
    {
        while(SDL_PollEvent(&event))
        {
        switch (event.type)
        {
        case SDL_QUIT :
            end = 1;
            break;
        case SDL_KEYDOWN :
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE :
                end = 1;
                break;
            case SDLK_UP :
                textureJ = SDL_CreateTextureFromSurface(renderer, surfaceJ[UP]);
                if  (dungeon[numLab][PlayerPos.y - 1][PlayerPos.x] == '8'  ||  dungeon[numLab][PlayerPos.y - 1][PlayerPos.x] == '?' || dungeon[numLab][PlayerPos.y - 1][PlayerPos.x] == '6' || dungeon[numLab][PlayerPos.y - 1][PlayerPos.x] == 'M') break;
                if (numLab == 1)
                {
                     // Condition pour les déplacements avec caisses.
                     if ((dungeon[numLab][PlayerPos.y - 1][PlayerPos.x] == '4') && (dungeon[numLab][PlayerPos.y - 2][PlayerPos.x] == '8' || dungeon[numLab][PlayerPos.y - 2][PlayerPos.x] == '4' || dungeon[numLab][PlayerPos.y - 2][PlayerPos.x] == '6' || dungeon[numLab][PlayerPos.y - 2][PlayerPos.x] == 'M')) break;
                     // Déplacement de la caisse.
                     goal = MoveBox(PlayerPos.x, PlayerPos.y - 1, PlayerPos.x, PlayerPos.y - 2, Box, dungeon, goal);
                }
                     // Déplacement du joueur.
                     PlayerPos.y--;
                if (numLab == 0)
                {
                    if (conditionSC(PlayerPos, doors, 2,  1)) PlayerPos.y++;
                    if (conditionSC(PlayerPos, doors, 2, -1)) PlayerPos.y = doors[2].coords.y;
                    if (conditionSC(PlayerPos, doors, 3,  1)) PlayerPos.y++;
                    if (conditionSC(PlayerPos, doors, 3, -1)) PlayerPos.y = doors[3].coords.y;
                    if (conditionSC(PlayerPos, doors, 4,  1)) PlayerPos.y++;
                    if (conditionSC(PlayerPos, doors, 4, -1)) PlayerPos.y = doors[4].coords.y;
                    if (conditionSC(PlayerPos, doors, 5,  1)) PlayerPos.y++;
                    if (conditionSC(PlayerPos, doors, 5, -1)) PlayerPos.y = doors[5].coords.y;
                }
                else
                {
                    if (conditionSC(PlayerPos, doors, 7,  1)) PlayerPos.y++;
                    if (conditionSC(PlayerPos, doors, 7, -1)) PlayerPos.y = doors[7].coords.y;
                    if (conditionSC(PlayerPos, doors, 8,  1)) PlayerPos.y++;
                    if (conditionSC(PlayerPos, doors, 8, -1)) PlayerPos.y = doors[8].coords.y;
                }
                break;
            case SDLK_DOWN :
                textureJ = SDL_CreateTextureFromSurface(renderer, surfaceJ[DOWN]);
                if  (dungeon[numLab][PlayerPos.y + 1][PlayerPos.x] == '8' || dungeon[numLab][PlayerPos.y + 1][PlayerPos.x] == '6') break;
                     // Condition pour les déplacements avec caisses.
                if (numLab == 1)
                {
                     if ((dungeon[numLab][PlayerPos.y + 1][PlayerPos.x] == '4') && (dungeon[numLab][PlayerPos.y + 2][PlayerPos.x] == '8' || dungeon[numLab][PlayerPos.y + 2][PlayerPos.x] == '4' || dungeon[numLab][PlayerPos.y + 2][PlayerPos.x] == '6')) break;
                     // Déplacement de la caisse.
                     goal = MoveBox(PlayerPos.x, PlayerPos.y + 1, PlayerPos.x, PlayerPos.y + 2, Box, dungeon, goal);
                }
                     // Déplacement du joueur.
                     PlayerPos.y++;
                if (numLab == 0)
                {
                    if (conditionSC(PlayerPos, doors, 2,  1)) PlayerPos.y--;
                    if (conditionSC(PlayerPos, doors, 2, -1)) PlayerPos.y = doors[2].coords.y;
                    if (conditionSC(PlayerPos, doors, 3,  1)) PlayerPos.y--;
                    if (conditionSC(PlayerPos, doors, 3, -1)) PlayerPos.y = doors[3].coords.y;
                    if (conditionSC(PlayerPos, doors, 4,  1)) PlayerPos.y--;
                    if (conditionSC(PlayerPos, doors, 4, -1)) PlayerPos.y = doors[4].coords.y;
                    if (conditionSC(PlayerPos, doors, 5,  1)) PlayerPos.y--;
                    if (conditionSC(PlayerPos, doors, 5, -1)) PlayerPos.y = doors[5].coords.y;
                }
                else
                {
                    if (conditionSC(PlayerPos, doors, 7,  1)) PlayerPos.y--;
                    if (conditionSC(PlayerPos, doors, 7, -1)) PlayerPos.y = doors[7].coords.y;
                    if (conditionSC(PlayerPos, doors, 8,  1)) PlayerPos.y--;
                    if (conditionSC(PlayerPos, doors, 8, -1)) PlayerPos.y = doors[8].coords.y;
                }
                break;
            case SDLK_RIGHT :
                textureJ = SDL_CreateTextureFromSurface(renderer, surfaceJ[RIGHT]);
                if  (dungeon[numLab][PlayerPos.y][PlayerPos.x + 1] == '8'  ||  dungeon[numLab][PlayerPos.y][PlayerPos.x + 1] == '?' || dungeon[numLab][PlayerPos.y][PlayerPos.x + 1] == '7' || dungeon[numLab][PlayerPos.y][PlayerPos.x + 1] == '6' || dungeon[numLab][PlayerPos.y][PlayerPos.x + 1] == 'M') break;
                if (numLab == 1)
                {
                     // Condition pour les déplacements avec caisses.
                     if ((dungeon[numLab][PlayerPos.y][PlayerPos.x + 1] == '4') && (dungeon[numLab][PlayerPos.y][PlayerPos.x + 2] == '8' || dungeon[numLab][PlayerPos.y][PlayerPos.x + 2] == '4' || dungeon[numLab][PlayerPos.y][PlayerPos.x + 2] == '6' || dungeon[numLab][PlayerPos.y][PlayerPos.x + 2] == 'M')) break;
                     // Déplacement de la caisse.
                     goal = MoveBox(PlayerPos.x + 1, PlayerPos.y, PlayerPos.x + 2, PlayerPos.y, Box, dungeon, goal);
                }
                     // Déplacement du joueur.
                     PlayerPos.x++;

                if (numLab == 0)
                {
                    if (conditionSC(PlayerPos, doors, 1,  1)) PlayerPos.x--;
                    if (conditionSC(PlayerPos, doors, 1, -1)) PlayerPos.x = doors[1].coords.x;
                    if (conditionSC(PlayerPos, doors, 0,  1)) PlayerPos.x--;
                    if (conditionSC(PlayerPos, doors, 0, -1)) PlayerPos.x = doors[0].coords.x;
                }
                else
                {
                    if (conditionSC(PlayerPos, doors, 6,  1)) PlayerPos.x--;
                    if (conditionSC(PlayerPos, doors, 6, -1)) PlayerPos.x = doors[6].coords.x;
                    if (conditionSC(PlayerPos, doors, 9,  1)) PlayerPos.x--;
                    if (conditionSC(PlayerPos, doors, 9, -1)) PlayerPos.x = doors[9].coords.x;
                }
                break;
            case SDLK_LEFT :
                textureJ = SDL_CreateTextureFromSurface(renderer, surfaceJ[LEFT]);
                if  (dungeon[numLab][PlayerPos.y][PlayerPos.x - 1] == '8' || dungeon[numLab][PlayerPos.y][PlayerPos.x - 1] == '6' || dungeon[numLab][PlayerPos.y][PlayerPos.x - 1] == 'M') break;
                if (numLab == 1)
                {
                     // Condition pour les déplacements avec caisses.
                     if ((dungeon[numLab][PlayerPos.y][PlayerPos.x - 1] == '4') && (dungeon[numLab][PlayerPos.y][PlayerPos.x - 2] == '8' || dungeon[numLab][PlayerPos.y][PlayerPos.x - 2] == '4' || dungeon[numLab][PlayerPos.y][PlayerPos.x - 2] == '6' || dungeon[numLab][PlayerPos.y][PlayerPos.x - 2] == 'M')) break;
                     // Déplacement de la caisse.
                     goal = MoveBox(PlayerPos.x - 1, PlayerPos.y, PlayerPos.x - 2, PlayerPos.y, Box, dungeon, goal);
                }
                     // Déplacement du joueur.
                     PlayerPos.x--;
                if (numLab == 0)
                {
                    if (conditionSC(PlayerPos, doors, 1,  1)) PlayerPos.x++;
                    if (conditionSC(PlayerPos, doors, 1, -1)) PlayerPos.x = doors[1].coords.x;
                    if (conditionSC(PlayerPos, doors, 0,  1)) PlayerPos.x++;
                    if (conditionSC(PlayerPos, doors, 0, -1)) PlayerPos.x = doors[0].coords.x;
                }
                else
                {
                    if (conditionSC(PlayerPos, doors, 6,  1)) PlayerPos.x++;
                    if (conditionSC(PlayerPos, doors, 6, -1)) PlayerPos.x = doors[6].coords.x;
                    if (conditionSC(PlayerPos, doors, 9,  1)) PlayerPos.x++;
                    if (conditionSC(PlayerPos, doors, 9, -1)) PlayerPos.x = doors[9].coords.x;
                }
                break;
            }
            break;
        }
        }


        ////////////////////// CHANGEMENT DE L'ÉTAT DE LA PORTE (OUVERTE/FERMÉE) //////////////////////


        if (PlayerPos.x != precedentX || PlayerPos.y != precedentY) // Condition pour ne pas réactiver le bouton si le joueur reste sur le bouton.
        {
            if (numLab == 0)
            {
                if (buttons[0].x == PlayerPos.x && buttons[0].y == PlayerPos.y)
                {
                    changeDoorCondi(doors, 1, dungeon, numLab, false);
                    changeDoorCondi(doors, 2, dungeon, numLab, false);
                }
                if (buttons[1].x == PlayerPos.x && buttons[1].y == PlayerPos.y)
                {
                    changeDoorCondi(doors, 3, dungeon, numLab, false);
                    changeDoorCondi(doors, 0, dungeon, numLab, false);
                }
                if (buttons[2].x == PlayerPos.x && buttons[2].y == PlayerPos.y)
                {
                    changeDoorCondi(doors, 5, dungeon, numLab, false);
                    changeDoorCondi(doors, 2, dungeon, numLab, false);
                }
                if (buttons[3].x == PlayerPos.x && buttons[3].y == PlayerPos.y)
                {
                    changeDoorCondi(doors, 4, dungeon, numLab, false);
                    changeDoorCondi(doors, 0, dungeon, numLab, false);
                }
            }
            else
            {
               if (buttons[4].x == PlayerPos.x && buttons[4].y == PlayerPos.y)
                {
                    changeDoorCondi(doors, 9, dungeon, numLab, false);
                }
                if (buttons[5].x == PlayerPos.x && buttons[5].y == PlayerPos.y)
                {
                    changeDoorCondi(doors, 6, dungeon, numLab, false);
                    changeDoorCondi(doors, 8, dungeon, numLab, false);
                }
                if (buttons[6].x == PlayerPos.x && buttons[6].y == PlayerPos.y)
                {
                    changeDoorCondi(doors, 9, dungeon, numLab, false);
                    changeDoorCondi(doors, 6, dungeon, numLab, false);
                }
                if (buttons[7].x == PlayerPos.x && buttons[7].y == PlayerPos.y)
                {
                    changeDoorCondi(doors, 7, dungeon, numLab, false);
                    changeDoorCondi(doors, 8, dungeon, numLab, false);
                    changeDoorCondi(doors, 9, dungeon, numLab, false);
                }
            }
        }

        //////////////////////// RÉCUPÉRATION DE LA CLÉ ///////////////////////

        if (dungeon[numLab][PlayerPos.y][PlayerPos.x] == ':')
        {
            if (numLab == 0)
            {
                dungeon[numLab][PlayerPos.y][PlayerPos.x] = '1';
                key = 1;
                dungeon[numLab][10][6] = '9';
            }
            else
            {
                dungeon[numLab][PlayerPos.y][PlayerPos.x] = '1';
                key = 1;
                goal = 0;
            }
        }

        ///////////////////// GESTION DE LA PORTE DE SORTIE ////////////////////

        if (dungeon[numLab][PlayerPos.y][PlayerPos.x] == ';' && key != 1)
        {
            PlayerPos.x++;
        }
        if (dungeon[numLab][PlayerPos.y][PlayerPos.x - 1] == ';' && key == 1)
        {
            dungeon[numLab][PlayerPos.y][PlayerPos.x - 1] = '<';
        }
        if (dungeon[numLab][PlayerPos.y][PlayerPos.x] == '<' && key == 1)
        {
            if (numLab == 0)
            {
                PlayerPos.x = 30;
                PlayerPos.y = 1;
                end = 2;
            }
            else
            {
                end = 1;
                AnimationFinale = 2;
            }
        }

        /////////////////////////// GESTION DES PIÈGES /////////////////////////////

            if (dungeon[numLab][PlayerPos.y][PlayerPos.x] == '9')
            {
                if (numLab == 0)
                {
                    for (int i = 0; i < 6; i++)
                    {
                        changeDoorCondi(doors, i, dungeon, numLab, true);
                    }
                    PlayerPos.x = 17;
                    PlayerPos.y = 1;
                }
                else
                {
                    for (int i = 6; i < 10; i++)
                    {
                        if (i == 8)
                        {
                            doors[i].condi = -1;
                            dungeon[numLab][doors[i].coords.y][doors[i].coords.x] = '1';
                        }
                        else
                        {
                            changeDoorCondi(doors, i, dungeon, numLab, true);
                        }
                    }
                    PlayerPos.x = 30;
                    PlayerPos.y = 1;
                }
            }

        ////////////////////////// GESTION DE L'ÉNIGME ///////////////////////////

        if (dungeon[numLab][PlayerPos.y][PlayerPos.x] == '@')
        {
            var1 = 1;
        }
        else if (dungeon[numLab][PlayerPos.y][PlayerPos.x] == 'A' && dungeon[1][precedentY][precedentX] == '@' && var1 == 1)
        {
            var2 = 1;
        }
        else if (dungeon[numLab][PlayerPos.y][PlayerPos.x] == 'B' && dungeon[1][precedentY][precedentX] == 'A' && var2 == 1 && hammer == 0)
        {
            hammer = 1;
            SDL_RenderSetScale(renderer, 1.0f, 2.0f);
            rect.x = PlayerPos.x * WIDTH_TILE;
            rect.y = PlayerPos.y * HEIGHT_TILE;
             while (hammer != 5)
             {
                switch (hammer)
                {
                    case 1 : dungeon[numLab][Rock.y - 2][Rock.x] = 'C';
                             break;
                    case 2 : dungeon[numLab][Rock.y - 1][Rock.x] = 'C';
                             break;
                    case 3 : dungeon[numLab][Rock.y][Rock.x] = 'D';
                             break;
                    case 4 : dungeon[numLab][Rock.y][Rock.x] = '2';
                             dungeon[numLab][Rock.y - 2][Rock.x] = '8';
                             dungeon[numLab][Rock.y - 1][Rock.x] = '>';
                             break;
                }
                SDL_RenderClear(renderer);
                ShowTexture(renderer, TLaby, dungeon, numLab);
                SDL_RenderCopy(renderer, textureJ, NULL, &rect);
                SDL_RenderPresent(renderer);
                SDL_Delay(300);
                hammer++;
             }
            SDL_Delay(1000);
            buttons[7].x = Rock.x;
            buttons[7].y = Rock.y;
            SDL_RenderSetScale(renderer, 1.0f, 1.0f);
        }


        ////// DÉPLACEMEENT DES MONSTRES //////

        tempsActuel = SDL_GetTicks();
        if (tempsActuel - tempsPrecedent > 1000) /* Si 30 ms se sont écoulées */
        {
            MoveMonster(MobsMovement, dungeon, mobs, numLab);
            tempsPrecedent = tempsActuel; /* Le temps "actuel" devient le temps "precedent" pour nos futurs calculs */
        }

        ///// ÉVÈNEMENTS LIÉS À LA DEUXIÈME MAP //////

        if (numLab == 1)
        {
            if (FirstTime == 0)
            {
                dungeon[numLab][doors[8].coords.y][doors[8].coords.x] = '1';
                FirstTime++;
            }
            switch(dungeon[numLab][PlayerPos.y][PlayerPos.x])
            {
                case 'E' :
                case 'F' :
                case '>' :
                case '=' :
                case '3' :
                case 'N' : end = 1; AnimationFinale = 1; break;
                case 'G' : while (GorillaAnim != 5)
                           {
                                textureJ = SDL_CreateTextureFromSurface(renderer, NULL);
                                SDL_RenderClear(renderer);
                                ShowTexture(renderer, TLaby, dungeon, numLab);
                                SDL_RenderCopy(renderer, textureJ, NULL, &rect);
                                SDL_RenderPresent(renderer);
                                switch (GorillaAnim)
                                {
                                    case 0 : dungeon[numLab][Gorilla.y][Gorilla.x] = 'H';
                                             break;
                                    case 1 : dungeon[numLab][Gorilla.y][Gorilla.x] = 'I';
                                             dungeon[numLab][Gorilla.y + 1][Gorilla.x - 1] = 'J';
                                             break;
                                    case 2 : dungeon[numLab][Gorilla.y][Gorilla.x] = 'G';
                                             dungeon[numLab][Gorilla.y + 1][Gorilla.x - 1] = '8';
                                             dungeon[numLab][Gorilla.y + 1][Gorilla.x - 2] = 'K';
                                             break;
                                    case 3 : dungeon[numLab][Gorilla.y + 1][Gorilla.x - 2] = '1';
                                             if (doors[8].condi == 1)
                                             {
                                                dungeon[numLab][Gorilla.y + 2][Gorilla.x - 3] = 'L';
                                             }
                                             else
                                             {
                                                 dungeon[numLab][Gorilla.y + 2][Gorilla.x - 3] = 'K';
                                             }
                                             break;
                                    case 4 : if (doors[8].condi == 1)
                                             {
                                                dungeon[numLab][Gorilla.y + 2][Gorilla.x - 3] = '5';
                                             }
                                             else
                                             {
                                                dungeon[numLab][Gorilla.y + 2][Gorilla.x - 3] = '1';
                                             }
                                             PlayerPos.x = 15;
                                             PlayerPos.y = 5;
                                             textureJ = SDL_CreateTextureFromSurface(renderer, surfaceJ[DOWN]);
                                             break;
                                }
                                SDL_Delay(300);
                                GorillaAnim++;
                           }
            }
        }

        // OBTENTION DE LA CLÉ DE SORTIE SI LES 3 CAISSES SONT SUR LES OBJECTIFS

        if (goal == 3)
        {
            dungeon[numLab][2][16] = ':';
        }

        /////////////////////// GESTION DE LA PREMIÈRE FIN //////////////////////////

        if (end == 2)
        {
            SDL_SetWindowSize(window, WIDTH_WINDOW2, HEIGHT_WINDOW2);
            SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

            end = 0;
            numLab = 1;
            key = 0;

            PlayerPos.x = 30;
            PlayerPos.y = 1;
        }

        precedentX = PlayerPos.x;
        precedentY = PlayerPos.y;

        rect.x = PlayerPos.x * WIDTH_TILE;
        rect.y = PlayerPos.y * HEIGHT_TILE;

        SDL_RenderClear(renderer);

        ShowTexture(renderer, TLaby, dungeon, numLab);
        if (dungeon[numLab][PlayerPos.y - 1][PlayerPos.x] == '?' && hammer == 0)
        {
            SDL_RenderCopy(renderer, TDialog, NULL, &rect2);
        }
        if (PlayerPos.x == 15 && PlayerPos.y == 5 && numLab == 1)
        {
            SDL_RenderCopy(renderer, TAstuce, NULL, &rectAstuce);
        }
        SDL_RenderCopy(renderer, textureJ, NULL, &rect);

        SDL_RenderPresent(renderer);

    }


    ///////// ANIMATION GAME OVER ////////


    if (AnimationFinale == 1)
    {
        SDL_RenderClear(renderer);
        SDL_SetWindowSize(window, 800, 800);
        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        int AnimGameOver = 0;
        SDL_Surface *SGameOver[6] = {NULL};
        SDL_Texture *TGameOver = NULL;

        SGameOver[0] = IMG_Load("./Images/GameOver.jpg");
        SGameOver[1] = IMG_Load("./Images/GameOver2.jpg");
        SGameOver[2] = IMG_Load("./Images/GameOver3.jpg");
        SGameOver[3] = IMG_Load("./Images/GameOver4.jpg");
        SGameOver[4] = IMG_Load("./Images/GameOver5.jpg");
        SGameOver[5] = IMG_Load("./Images/GameOver6.jpg");

        if (SGameOver == NULL)
        {
            fprintf (stderr, "%s\n",SDL_GetError());
            return EXIT_FAILURE;
        }

        SDL_Rect rectGameOver;

        rectGameOver.w = 800;
        rectGameOver.h = 800;
        rectGameOver.y = 0;
        rectGameOver.x = 0;

        TGameOver = SDL_CreateTextureFromSurface(renderer, SGameOver[0]);
        while(AnimGameOver != 6)
        {
            switch(AnimGameOver)
            {
                case 0 : TGameOver = SDL_CreateTextureFromSurface(renderer, SGameOver[0]); break;
                case 1 : TGameOver = SDL_CreateTextureFromSurface(renderer, SGameOver[1]); break;
                case 2 : TGameOver = SDL_CreateTextureFromSurface(renderer, SGameOver[2]); break;
                case 3 : TGameOver = SDL_CreateTextureFromSurface(renderer, SGameOver[3]); break;
                case 4 : TGameOver = SDL_CreateTextureFromSurface(renderer, SGameOver[4]); break;
                case 5 : TGameOver = SDL_CreateTextureFromSurface(renderer, SGameOver[5]); break;
            }
            AnimGameOver++;
            if (SGameOver == NULL)
            {
                fprintf (stderr, "%s\n",SDL_GetError());
                return EXIT_FAILURE;
            }
            if (TGameOver == NULL)
            {
                fprintf (stderr, "%s\n",SDL_GetError());
                return EXIT_FAILURE;
            }
            SDL_RenderCopy(renderer, TGameOver, NULL, &rectGameOver);
            SDL_RenderPresent(renderer);
            SDL_Delay(200);
        }
        SDL_Delay(800);
    }
    else if (AnimationFinale == 2)
    {
        SDL_RenderClear(renderer);
        SDL_SetWindowSize(window, 800, 800);
        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

        int AnimVictory = 0;
        SDL_Surface *SVictory[3] = {NULL};
        SDL_Texture *TVictory = NULL;

        SVictory[0] = IMG_Load("./Images/Victory.jpg");
        SVictory[1] = IMG_Load("./Images/Victory2.jpg");
        SVictory[2] = IMG_Load("./Images/Victory3.jpg");

        if (SVictory == NULL)
        {
            fprintf (stderr, "%s\n",SDL_GetError());
            return EXIT_FAILURE;
        }

        SDL_Rect rectVictory;

        rectVictory.w = 800;
        rectVictory.h = 800;
        rectVictory.y = 0;
        rectVictory.x = 0;

        TVictory = SDL_CreateTextureFromSurface(renderer, SVictory[0]);
        while(AnimVictory != 3)
        {
            switch(AnimVictory)
            {
                case 0 : TVictory = SDL_CreateTextureFromSurface(renderer, SVictory[0]); break;
                case 1 : TVictory = SDL_CreateTextureFromSurface(renderer, SVictory[1]); break;
                case 2 : TVictory = SDL_CreateTextureFromSurface(renderer, SVictory[2]); break;
            }
            AnimVictory++;
            if (SVictory == NULL)
            {
                fprintf (stderr, "%s\n",SDL_GetError());
                return EXIT_FAILURE;
            }
            if (TVictory == NULL)
            {
                fprintf (stderr, "%s\n",SDL_GetError());
                return EXIT_FAILURE;
            }
            SDL_RenderCopy(renderer, TVictory, NULL, &rectVictory);
            SDL_RenderPresent(renderer);
            SDL_Delay(300);
        }
        SDL_Delay(700);
    }


    // Destruction de toutes les surfaces et textures ainsi que du renderer et de la fenêtre.


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
