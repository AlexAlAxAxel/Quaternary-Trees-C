#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "header.h"

//initializarea arborelui quaternar pentru compresie
void init(QTree **node, int width, int height)
{
    *node = (QTree *)malloc(sizeof(QTree));

    (*node)->x_st = 0;
    (*node)->y_st = 0;
    (*node)->x_dr = width - 1;
    (*node)->y_dr = height - 1;

    (*node)->fiu1 = NULL;
    (*node)->fiu2 = NULL;
    (*node)->fiu3 = NULL;
    (*node)->fiu4 = NULL;
}

//calcularea mean-ului in functie de coordonatele unui nod
long long int mean(QTree *node, Pixel **matrice)
{
    long long int mean = 0;
    //parcurge patratul determinat de coordonatele stanga-sus si dreapta-jos
    //stocate in structura
    for (int i = node->y_st; i <= node->y_dr; i++)
        for (int j = node->x_st; j <= node->x_dr; j++)
            mean = mean + (node->red - matrice[i][j].red) * (node->red - matrice[i][j].red) +
                   (node->green - matrice[i][j].green) * (node->green - matrice[i][j].green) +
                   (node->blue - matrice[i][j].blue) * (node->blue - matrice[i][j].blue);
    mean = mean / (3 * (node->x_dr - node->x_st + 1) * (node->y_dr - node->y_st + 1));
    return mean;
}

//functie care calculeaza valorile medii de red, green, blue ale unei
//zone determinate de coordonatele din colturile unui nod
Pixel valori_medii(QTree *node, Pixel **matrice)
{
    int red = 0, green = 0, blue = 0;
    for (int i = node->y_st; i <= node->y_dr; i++)
        for (int j = node->x_st; j <= node->x_dr; j++)
        {
            red = red + matrice[i][j].red;
            green = green + matrice[i][j].green;
            blue = blue + matrice[i][j].blue;
        }
    red = red / ((node->x_dr - node->x_st + 1) * (node->y_dr - node->y_st + 1));
    green = green / ((node->x_dr - node->x_st + 1) * (node->y_dr - node->y_st + 1));
    blue = blue / ((node->x_dr - node->x_st + 1) * (node->y_dr - node->y_st + 1));

    Pixel aux;
    aux.red = red;
    aux.green = green;
    aux.blue = blue;
    return aux;
}

//functia de construire a arborelui quaternar folosita pentru compresie
void construire(QTree **node, Pixel **matrice, int factor)
{
    Pixel aux = valori_medii(*node, matrice);

    (*node)->red = aux.red;
    (*node)->green = aux.green;
    (*node)->blue = aux.blue;

    int suma;
    suma = mean(*node, matrice);

    if (suma <= factor)
        return;

    //construieste fiii doar daca mean-ul este mai mare decat factorul
    //construirea fiului 1
    (*node)->fiu1 = (QTree *)malloc(sizeof(QTree));
    (*node)->fiu1->x_st = (*node)->x_st;
    (*node)->fiu1->y_st = (*node)->y_st;
    (*node)->fiu1->x_dr = (*node)->x_st + ((*node)->x_dr - (*node)->x_st) / 2;
    (*node)->fiu1->y_dr = (*node)->y_st + ((*node)->y_dr - (*node)->y_st) / 2;
    (*node)->fiu1->fiu1 = NULL;
    (*node)->fiu1->fiu2 = NULL;
    (*node)->fiu1->fiu3 = NULL;
    (*node)->fiu1->fiu4 = NULL;
    construire(&(*node)->fiu1, matrice, factor);

    //construirea fiului 2
    (*node)->fiu2 = (QTree *)malloc(sizeof(QTree));
    (*node)->fiu2->x_st = (*node)->x_st + ((*node)->x_dr - (*node)->x_st) / 2 + 1;
    (*node)->fiu2->y_st = (*node)->y_st;
    (*node)->fiu2->x_dr = (*node)->x_dr;
    (*node)->fiu2->y_dr = (*node)->y_st + ((*node)->y_dr - (*node)->y_st) / 2;
    (*node)->fiu2->fiu1 = NULL;
    (*node)->fiu2->fiu2 = NULL;
    (*node)->fiu2->fiu3 = NULL;
    (*node)->fiu2->fiu4 = NULL;
    construire(&(*node)->fiu2, matrice, factor);

    //construirea fiului 3
    (*node)->fiu3 = (QTree *)malloc(sizeof(QTree));
    (*node)->fiu3->x_st = (*node)->x_st + ((*node)->x_dr - (*node)->x_st) / 2 + 1;
    (*node)->fiu3->y_st = (*node)->y_st + ((*node)->y_dr - (*node)->y_st) / 2 + 1;
    (*node)->fiu3->x_dr = (*node)->x_dr;
    (*node)->fiu3->y_dr = (*node)->y_dr;
    (*node)->fiu3->fiu1 = NULL;
    (*node)->fiu3->fiu2 = NULL;
    (*node)->fiu3->fiu3 = NULL;
    (*node)->fiu3->fiu4 = NULL;
    construire(&(*node)->fiu3, matrice, factor);

    //construirea fiului 4
    (*node)->fiu4 = (QTree *)malloc(sizeof(QTree));
    (*node)->fiu4->x_st = (*node)->x_st;
    (*node)->fiu4->y_st = (*node)->y_st + ((*node)->y_dr - (*node)->y_st) / 2 + 1;
    (*node)->fiu4->x_dr = (*node)->x_st + ((*node)->x_dr - (*node)->x_st) / 2;
    (*node)->fiu4->y_dr = (*node)->y_dr;
    (*node)->fiu4->fiu1 = NULL;
    (*node)->fiu4->fiu2 = NULL;
    (*node)->fiu4->fiu3 = NULL;
    (*node)->fiu4->fiu4 = NULL;
    construire(&(*node)->fiu4, matrice, factor);
}

//stergerea arborelui quaternar din memorie
void deleteTree(QTree **node)
{
    if (*node == NULL)
        return;

    deleteTree(&(*node)->fiu1);
    deleteTree(&(*node)->fiu2);
    deleteTree(&(*node)->fiu3);
    deleteTree(&(*node)->fiu4);

    free(*node);
    *node = NULL;
}

//initializarea coadei folosita pentru BFS
void init_queue(Queue *q)
{
    q->head = NULL;
    q->tail = NULL;
    q->len = 0;
}

//adaugarea in coada
void add_queue(Queue *q, QTree *nou)
{
    if (q->len == 0)
    {
        q->head = nou;
        q->tail = nou;
    }
    else
    {
        q->tail->next = nou;
        q->tail = nou;
    }

    q->len++;
}

//stergerea din coada
void pop_queue(Queue *q)
{
    QTree *aux;

    aux = q->head->next;
    q->head = aux;

    q->len--;
}

//functie de completarea vectorului pentru compresie
void completare_vector(QuadtreeNode **vector, QTree *root, int *parcurgere, int *nr_frunze)
{
    Queue q2;
    //initializarea cozii si adaugarea radacinii
    init_queue(&q2);
    add_queue(&q2, root);
    //parcurgem arborele in latime si completam vectorul
    while (q2.len != 0)
    {
        (*parcurgere)++;
        QTree *nod;
        nod = q2.head;
        if (nod->fiu1 != NULL)
        {
            add_queue(&q2, nod->fiu1);
            add_queue(&q2, nod->fiu2);
            add_queue(&q2, nod->fiu3);
            add_queue(&q2, nod->fiu4);

            (*vector + *parcurgere)->top_left = nod->fiu1->indice;
            (*vector + *parcurgere)->top_right = nod->fiu2->indice;
            (*vector + *parcurgere)->bottom_right = nod->fiu3->indice;
            (*vector + *parcurgere)->bottom_left = nod->fiu4->indice;
        }
        else
        {
            (*nr_frunze)++;
            (*vector + *parcurgere)->top_left = -1;
            (*vector + *parcurgere)->top_right = -1;
            (*vector + *parcurgere)->bottom_right = -1;
            (*vector + *parcurgere)->bottom_left = -1;
        }

        (*vector + *parcurgere)->red = nod->red;
        (*vector + *parcurgere)->green = nod->green;
        (*vector + *parcurgere)->blue = nod->blue;
        (*vector + *parcurgere)->area = (nod->x_dr - nod->x_st + 1) * (nod->y_dr - nod->y_st + 1);

        pop_queue(&q2);
    }
}

//initializarea folosita pentru decompresie
void init2(QTree **node, QuadtreeNode element)
{
    *node = (QTree *)malloc(sizeof(QTree));

    int width, height;
    width = sqrt(element.area);
    height = width;

    (*node)->red = element.red;
    (*node)->green = element.green;
    (*node)->blue = element.blue;

    (*node)->x_st = 0;
    (*node)->y_st = 0;
    (*node)->x_dr = width - 1;
    (*node)->y_dr = height - 1;

    (*node)->indice = 0;

    (*node)->fiu1 = NULL;
    (*node)->fiu2 = NULL;
    (*node)->fiu3 = NULL;
    (*node)->fiu4 = NULL;
}

//construirea arborelui folosit pentru decompresie
//index reprezinta indexul in vector al nodului curent
void construire2(QTree **node, QuadtreeNode *element, int index)
{
    //daca nodul e frunza
    if (element[index].top_left == -1)
        return;

    //daca nodul nu e frunza
    //construirea fiului 1
    (*node)->fiu1 = (QTree *)malloc(sizeof(QTree));
    (*node)->fiu1->x_st = (*node)->x_st;
    (*node)->fiu1->y_st = (*node)->y_st;
    (*node)->fiu1->x_dr = (*node)->x_st + ((*node)->x_dr - (*node)->x_st) / 2;
    (*node)->fiu1->y_dr = (*node)->y_st + ((*node)->y_dr - (*node)->y_st) / 2;

    (*node)->fiu1->indice = element[index].top_left;
    (*node)->fiu1->red = element[element[index].top_left].red;
    (*node)->fiu1->green = element[element[index].top_left].green;
    (*node)->fiu1->blue = element[element[index].top_left].blue;

    (*node)->fiu1->fiu1 = NULL;
    (*node)->fiu1->fiu2 = NULL;
    (*node)->fiu1->fiu3 = NULL;
    (*node)->fiu1->fiu4 = NULL;
    construire2(&(*node)->fiu1, element, element[index].top_left);

    //construirea fiului 2
    (*node)->fiu2 = (QTree *)malloc(sizeof(QTree));
    (*node)->fiu2->x_st = (*node)->x_st + ((*node)->x_dr - (*node)->x_st) / 2 + 1;
    (*node)->fiu2->y_st = (*node)->y_st;
    (*node)->fiu2->x_dr = (*node)->x_dr;
    (*node)->fiu2->y_dr = (*node)->y_st + ((*node)->y_dr - (*node)->y_st) / 2;

    (*node)->fiu2->indice = element[index].top_right;
    (*node)->fiu2->red = element[element[index].top_right].red;
    (*node)->fiu2->green = element[element[index].top_right].green;
    (*node)->fiu2->blue = element[element[index].top_right].blue;

    (*node)->fiu2->fiu1 = NULL;
    (*node)->fiu2->fiu2 = NULL;
    (*node)->fiu2->fiu3 = NULL;
    (*node)->fiu2->fiu4 = NULL;
    construire2(&(*node)->fiu2, element, element[index].top_right);

    //construirea fiului 3
    (*node)->fiu3 = (QTree *)malloc(sizeof(QTree));
    (*node)->fiu3->x_st = (*node)->x_st + ((*node)->x_dr - (*node)->x_st) / 2 + 1;
    (*node)->fiu3->y_st = (*node)->y_st + ((*node)->y_dr - (*node)->y_st) / 2 + 1;
    (*node)->fiu3->x_dr = (*node)->x_dr;
    (*node)->fiu3->y_dr = (*node)->y_dr;

    (*node)->fiu3->indice = element[index].bottom_right;
    (*node)->fiu3->red = element[element[index].bottom_right].red;
    (*node)->fiu3->green = element[element[index].bottom_right].green;
    (*node)->fiu3->blue = element[element[index].bottom_right].blue;

    (*node)->fiu3->fiu1 = NULL;
    (*node)->fiu3->fiu2 = NULL;
    (*node)->fiu3->fiu3 = NULL;
    (*node)->fiu3->fiu4 = NULL;
    construire2(&(*node)->fiu3, element, element[index].bottom_right);

    //construirea fiului 4
    (*node)->fiu4 = (QTree *)malloc(sizeof(QTree));
    (*node)->fiu4->x_st = (*node)->x_st;
    (*node)->fiu4->y_st = (*node)->y_st + ((*node)->y_dr - (*node)->y_st) / 2 + 1;
    (*node)->fiu4->x_dr = (*node)->x_st + ((*node)->x_dr - (*node)->x_st) / 2;
    (*node)->fiu4->y_dr = (*node)->y_dr;

    (*node)->fiu4->indice = element[index].bottom_left;
    (*node)->fiu4->red = element[element[index].bottom_left].red;
    (*node)->fiu4->green = element[element[index].bottom_left].green;
    (*node)->fiu4->blue = element[element[index].bottom_left].blue;

    (*node)->fiu4->fiu1 = NULL;
    (*node)->fiu4->fiu2 = NULL;
    (*node)->fiu4->fiu3 = NULL;
    (*node)->fiu4->fiu4 = NULL;
    construire2(&(*node)->fiu4, element, element[index].bottom_left);
}

//completarea matricei folosita la decompresie
void completare_matrice(QTree *root, Pixel **matrice)
{
    Queue q1;
    init_queue(&q1);
    add_queue(&q1, root);
    while (q1.len != 0)
    {
        QTree *nod;
        nod = q1.head;

        //daca nodul nu e frunza
        if (nod->fiu1 != NULL)
        {
            add_queue(&q1, nod->fiu1);
            add_queue(&q1, nod->fiu2);
            add_queue(&q1, nod->fiu3);
            add_queue(&q1, nod->fiu4);
        }
        //daca nodul e frunza
        else
        {
            Pixel aux;
            aux.red = nod->red;
            aux.green = nod->green;
            aux.blue = nod->blue;
            //pune pixelii in matrice
            for (int i = nod->y_st; i <= nod->y_dr; i++)
                for (int j = nod->x_st; j <= nod->x_dr; j++)
                    matrice[i][j] = aux;
        }
        pop_queue(&q1);
    }
}

//calcularea mean-ului in functie de coordonatele date
long long int mean2(int x_st, int y_st, int x_dr, int y_dr, Pixel **matrice, QTree *node)
{
    long long int mean = 0;
    for (int i = y_st; i <= y_dr; i++)
        for (int j = x_st; j <= x_dr; j++)
            mean = mean + (node->red - matrice[i][j].red) * (node->red - matrice[i][j].red) +
                   (node->green - matrice[i][j].green) * (node->green - matrice[i][j].green) +
                   (node->blue - matrice[i][j].blue) * (node->blue - matrice[i][j].blue);
    mean = mean / (3 * (x_dr - x_st + 1) * (y_dr - y_st + 1));
    return mean;
}

//calcularea valorilor medii in functie de coordonatele date
Pixel valori_medii2(int x_st, int y_st, int x_dr, int y_dr, Pixel **matrice)
{
    //calculam valorile medii pentru red, green, blue
    int red = 0, green = 0, blue = 0;
    for (int i = y_st; i <= y_dr; i++)
        for (int j = x_st; j <= x_dr; j++)
        {
            red = red + matrice[i][j].red;
            green = green + matrice[i][j].green;
            blue = blue + matrice[i][j].blue;
        }
    red = red / ((x_dr - x_st + 1) * (y_dr - y_st + 1));
    green = green / ((x_dr - x_st + 1) * (y_dr - y_st + 1));
    blue = blue / ((x_dr - x_st + 1) * (y_dr - y_st + 1));
    Pixel aux;
    aux.red = red;
    aux.green = green;
    aux.blue = blue;
    return aux;
}

//construirea arborelui oglindit orizontal
void construireh(QTree **node, Pixel **matrice, int factor, int x_st, int y_st, int x_dr, int y_dr)
{
    Pixel aux;
    aux = valori_medii2(x_st, y_st, x_dr, y_dr, matrice);

    (*node) = (QTree *)malloc(sizeof(QTree));
    (*node)->red = aux.red;
    (*node)->green = aux.green;
    (*node)->blue = aux.blue;
    (*node)->fiu1 = NULL;
    (*node)->fiu2 = NULL;
    (*node)->fiu3 = NULL;
    (*node)->fiu4 = NULL;

    int suma;
    suma = mean2(x_st, y_st, x_dr, y_dr, matrice, *node);

    if (suma <= factor)
        return;

    //se inverseaza nodurile 1<->2 si nodurile 3<->4
    construireh(&(*node)->fiu2, matrice, factor, x_st, y_st, x_st + (x_dr - x_st) / 2, y_st + (y_dr - y_st) / 2);
    construireh(&(*node)->fiu1, matrice, factor, x_st + (x_dr - x_st) / 2 + 1, y_st, x_dr, y_st + (y_dr - y_st) / 2);
    construireh(&(*node)->fiu4, matrice, factor, x_st + (x_dr - x_st) / 2 + 1, y_st + (y_dr - y_st) / 2 + 1, x_dr, y_dr);
    construireh(&(*node)->fiu3, matrice, factor, x_st, y_st + (y_dr - y_st) / 2 + 1, x_st + (x_dr - x_st) / 2, y_dr);
}

//construirea arborelui oglindit vertical
void construirev(QTree **node, Pixel **matrice, int factor, int x_st, int y_st, int x_dr, int y_dr)
{
    Pixel aux;
    aux = valori_medii2(x_st, y_st, x_dr, y_dr, matrice);

    (*node) = (QTree *)malloc(sizeof(QTree));
    (*node)->red = aux.red;
    (*node)->green = aux.green;
    (*node)->blue = aux.blue;
    (*node)->fiu1 = NULL;
    (*node)->fiu2 = NULL;
    (*node)->fiu3 = NULL;
    (*node)->fiu4 = NULL;

    int suma;
    suma = mean2(x_st, y_st, x_dr, y_dr, matrice, *node);

    if (suma <= factor)
        return;

    //se inverseaza nodurile 1<->4 si nodurile 2<->3
    construirev(&(*node)->fiu4, matrice, factor, x_st, y_st, x_st + (x_dr - x_st) / 2, y_st + (y_dr - y_st) / 2);
    construirev(&(*node)->fiu3, matrice, factor, x_st + (x_dr - x_st) / 2 + 1, y_st, x_dr, y_st + (y_dr - y_st) / 2);
    construirev(&(*node)->fiu2, matrice, factor, x_st + (x_dr - x_st) / 2 + 1, y_st + (y_dr - y_st) / 2 + 1, x_dr, y_dr);
    construirev(&(*node)->fiu1, matrice, factor, x_st, y_st + (y_dr - y_st) / 2 + 1, x_st + (x_dr - x_st) / 2, y_dr);
}

//completarea matricei folosita la ambele oglindiri
void completare_matrice2(QTree **node, Pixel **matrice, int factor, int x_st, int y_st, int x_dr, int y_dr)
{
    Pixel aux;
    aux.red = (*node)->red;
    aux.green = (*node)->green;
    aux.blue = (*node)->blue;

    //daca e frunza
    if ((*node)->fiu1 == NULL)
    {
        for (int i = y_st; i <= y_dr; i++)
            for (int j = x_st; j <= x_dr; j++)
                matrice[i][j] = aux;
        return;
    }

    completare_matrice2(&(*node)->fiu1, matrice, factor, x_st, y_st, x_st + (x_dr - x_st) / 2, y_st + (y_dr - y_st) / 2);
    completare_matrice2(&(*node)->fiu2, matrice, factor, x_st + (x_dr - x_st) / 2 + 1, y_st, x_dr, y_st + (y_dr - y_st) / 2);
    completare_matrice2(&(*node)->fiu3, matrice, factor, x_st + (x_dr - x_st) / 2 + 1, y_st + (y_dr - y_st) / 2 + 1, x_dr, y_dr);
    completare_matrice2(&(*node)->fiu4, matrice, factor, x_st, y_st + (y_dr - y_st) / 2 + 1, x_st + (x_dr - x_st) / 2, y_dr);
}