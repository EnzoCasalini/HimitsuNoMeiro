#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

#define SIZEX 32
#define SIZEY 12
#define WIDTH_TILE          40
#define HEIGHT_TILE         40

// Fenêtre de base

#define NB_BLOCS_WIDTH      19
#define NB_BLOCS_HEIGHT     12
#define WIDTH_WINDOW        WIDTH_TILE * NB_BLOCS_WIDTH
#define HEIGHT_WINDOW       HEIGHT_TILE * NB_BLOCS_HEIGHT

// Fenêtre redimensionnée

#define NB_BLOCS_WIDTH2     32
#define NB_BLOCS_HEIGHT2    12
#define WIDTH_WINDOW2       WIDTH_TILE * NB_BLOCS_WIDTH2
#define HEIGHT_WINDOW2      HEIGHT_TILE * NB_BLOCS_HEIGHT2


enum {DOWN, UP, LEFT, RIGHT};
enum {WALL, EMPTY, PLAYER};

typedef struct
{
    int x;
    int y;
}
Coord;

typedef struct
{
    Coord coords;
    int condi;
}
Door;

typedef struct
{
    Coord coords;
    int placement;
}
Mobs;


char dungeon [2][SIZEY][SIZEX+1] =
{
    {
        "8868886688866688886             ",
        ";111111115112821118             ",
        "8211111198111511118             ",
        "6858868888111811116             ",
        "6111888666111888588             ",
        "6111886888111811116             ",
        "6111668888111821118             ",
        "8119886668858888618             ",
        "8858888888111688816             ",
        "8111111668111886816             ",
        "8:11111886111111118             ",
        "8886688888886868888             "
    },

    {
        "88886668888688866888868668866668",
        "80811111118111111181151168211118",
        "8111111111411111116G988588688186",
        "811866868866488111886831>6811118",
        "888111188888116858881111788E1118",
        "668111111111118111181=33>88111F8",
        "886111886884816111N811>=>6891126",
        "680111068881118111163111388E1118",
        "88866886886666811118>=31>88111F8",
        "81111111111111111118ABO1?6811118",
        ";1111111111111111118@11115111126",
        "66688886688688866668888688866888"
    }
};

#endif // CONSTANTES_H_INCLUDED
