#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "header.h"

int main(int argc, char **argv)
{
    //daca primul argument in linia de comanda este "-c" sau "-m" atunci se realizeaza
    //compresie sau una dintre oglindiri
    if (strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "-m") == 0)
    {
        //extrage factorul si il converteste intr-o variabila de tip int
        int factor;
        factor = atoi(argv[argc - 3]);

        FILE *input, *output;
        input = fopen(argv[argc - 2], "rb");
        output = fopen(argv[argc - 1], "wb");

        //variabilele pentru realizarea citirii
        char s[3], *width_aux, *height_aux, *val_max_aux, aux;

        int width, height, i;
        width_aux = (char *)malloc(10 * sizeof(char));
        height_aux = (char *)malloc(10 * sizeof(char));
        val_max_aux = (char *)malloc(10 * sizeof(char));

        //se citeste "P6\n" in s
        fread(s, 3 * sizeof(char), 1, input);

        //extragem width
        i = 0;
        while (1)
        {
            //se citeste pana la primul spatiu
            fread(&aux, sizeof(char), 1, input);
            if (aux != ' ')
            {
                width_aux[i] = aux;
                i++;
            }
            else
                break;
        }
        width_aux[i] = '\0';
        width = atoi(width_aux);
        free(width_aux);

        //extragem height
        i = 0;
        while (1)
        {
            //se citeste pana la primul newline
            fread(&aux, sizeof(char), 1, input);
            if (aux != '\n')
            {
                height_aux[i] = aux;
                i++;
            }
            else
                break;
        }
        height_aux[i] = '\0';
        height = atoi(height_aux);
        free(height_aux);

        //extragem val_max
        i = 0;
        while (1)
        {
            fread(&aux, sizeof(char), 1, input);
            if (aux != '\n')
            {
                val_max_aux[i] = aux;
                i++;
            }
            else
                break;
        }
        val_max_aux[i] = '\0';
        free(val_max_aux);

        //matricea cu elemente de tip "Pixel" in care se stocheaza pixelii pozei
        Pixel **matrice = (Pixel **)malloc(height * sizeof(Pixel *));
        for (int i = 0; i <= height - 1; i++)
            matrice[i] = (Pixel *)malloc(width * sizeof(Pixel));

        //citirea pozei in matrice
        for (int i = 0; i <= height - 1; i++)
            fread(matrice[i], sizeof(Pixel), width, input);

        QTree *root = NULL;

        //daca primul argument in linia de comanda este "-c", atunci se realizeaza compresia
        if (strcmp(argv[1], "-c") == 0)
        {
            init(&root, width, height);
            construire(&root, matrice, factor);

            //BFS pentru numerotarea nodurilor
            Queue q1;
            init_queue(&q1);
            add_queue(&q1, root);
            //indice reprezinta indicele ultimului nod adaugat in vector
            int indice = -1;
            while (q1.len != 0)
            {
                indice++;
                QTree *nod;
                nod = q1.head;
                nod->indice = indice;
                if (nod->fiu1 != NULL)
                {
                    add_queue(&q1, nod->fiu1);
                    add_queue(&q1, nod->fiu2);
                    add_queue(&q1, nod->fiu3);
                    add_queue(&q1, nod->fiu4);
                }
                pop_queue(&q1);
            }

            //completarea vectorului
            QuadtreeNode *vector;
            vector = (QuadtreeNode *)malloc((indice + 1) * sizeof(QuadtreeNode));
            int parcurgere = -1, nr_frunze = 0;
            completare_vector(&vector, root, &parcurgere, &nr_frunze);

            //indice reprezinta numarul de elemente dupa ce ii dau "++"
            indice++;
            fwrite(&nr_frunze, sizeof(int), 1, output);
            fwrite(&indice, sizeof(int), 1, output);
            fwrite(vector, sizeof(QuadtreeNode), indice, output);
            free(vector);
        }
        //daca primul argument in linia de comanda nu este "-c", atunci se realizeaza una dintre oglindiri
        else
        {
            //oglindire orizontala
            if (strcmp(argv[2], "h") == 0)
                construireh(&root, matrice, factor, 0, 0, width - 1, height - 1);

            //oglindire verticala
            else
                construirev(&root, matrice, factor, 0, 0, width - 1, height - 1);

            //se completeaza matricea cu pixelii din nodurile frunze
            completare_matrice2(&root, matrice, factor, 0, 0, width - 1, height - 1);

            //"scrierea" pozei in fisier
            int nr = 255;
            fprintf(output, "P6\n%d %d\n%d\n", width, height, nr);
            for (int i = 0; i <= width - 1; i++)
                fwrite(matrice[i], sizeof(Pixel), width, output);
        }

        //eliberarile de memorie
        deleteTree(&root);
        for (int i = 0; i <= height - 1; i++)
            free(matrice[i]);
        free(matrice);

        //inchiderile de fisiere
        fclose(input);
        fclose(output);
    }

    //daca primul argument in linia de comanda este "-d", atunci se realizeaza decompresie
    if (strcmp(argv[1], "-d") == 0)
    {
        FILE *input, *output;
        input = fopen(argv[2], "rb");
        output = fopen(argv[3], "wb");

        //citirea numarului de culori si a size-ului
        int nr_frunze, nr_noduri;
        fread(&nr_frunze, sizeof(int), 1, input);
        fread(&nr_noduri, sizeof(int), 1, input);
        //citirea vectorului
        QuadtreeNode *vector;
        vector = (QuadtreeNode *)malloc(nr_noduri * sizeof(QuadtreeNode));
        fread(vector, sizeof(QuadtreeNode), nr_noduri, input);
        //initializarea si contruirea arborelui quaternar
        QTree *root = NULL;
        init2(&root, vector[0]);
        construire2(&root, vector, 0);

        int size = root->y_dr + 1;
        Pixel **matrice = (Pixel **)malloc(size * sizeof(Pixel *));
        for (int i = 0; i <= size - 1; i++)
            matrice[i] = (Pixel *)malloc(size * sizeof(Pixel));

        //completarea matricei
        completare_matrice(root, matrice);

        //"scrierea" imaginii (matricei) in fisier
        int nr = 255;
        fprintf(output, "P6\n%d %d\n%d\n", size, size, nr);
        for (int i = 0; i <= size - 1; i++)
            fwrite(matrice[i], sizeof(Pixel), size, output);

        //eliberarile de memorie
        free(vector);
        deleteTree(&root);
        for (int i = 0; i <= size - 1; i++)
            free(matrice[i]);
        free(matrice);

        //inchiderile de fisiere
        fclose(input);
        fclose(output);
    }

    return 0;
}