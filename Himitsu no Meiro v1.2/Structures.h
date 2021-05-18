#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED

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
    int hp;
}
Player;

typedef struct
{
    Coord coords;
    int placement;
}
Mobs;



#endif // STRUCTURES_H_INCLUDED
