#ifndef HEADER_H_
#define HEADER_H_
#include <stdint.h>

typedef struct QuadtreeNode
{
    unsigned char blue, green, red;
    uint32_t area;
    int32_t top_left, top_right;
    int32_t bottom_left, bottom_right;
} __attribute__((packed)) QuadtreeNode;

//structura cu care stochez pixelii
typedef struct Pixel
{
    unsigned char red, green, blue;
} Pixel;

//structura de arbore quaternar
typedef struct QTree
{
    int indice, mean;
    unsigned char blue, green, red;

    //coordonatele stanga-sus
    int x_st, y_st;

    //coordonatele dreapta-jos
    int x_dr, y_dr;

    struct QTree *fiu1, *fiu2, *fiu3, *fiu4, *next;
} QTree;

//structura pentru coada
typedef struct Queue
{
    struct QTree *head, *tail;
    int len;
} Queue;

void init(QTree **node, int width, int height);

void construire(QTree **node, Pixel **matrice, int index);

void deleteTree(QTree **node);

void init_queue(Queue *q);

void add_queue(Queue *q, QTree *nou);

void pop_queue(Queue *q);

void completare_vector(QuadtreeNode **vector, QTree *root, int *parcurgere, int *nr_frunze);

void init2(QTree **node, QuadtreeNode element);

void construire2(QTree **node, QuadtreeNode *element, int index);

Pixel valori_medii(QTree *node, Pixel **matrice);

void construireh(QTree **node, Pixel **matrice, int factor, int x_st, int y_st, int x_dr, int y_dr);

void construirev(QTree **node, Pixel **matrice, int factor, int x_st, int y_st, int x_dr, int y_dr);

void completare_matrice2(QTree **node, Pixel **matrice, int factor, int x_st, int y_st, int x_dr, int y_dr);

void completare_matrice(QTree *root, Pixel **matrice);

#endif