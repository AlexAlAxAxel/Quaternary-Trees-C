In functia main, in functie de primul argument din linia de comanda, am facut
urmatoarele lucruri:

-Daca primul argument este "-c" sau "-m", am extras factorul de compresie, am
deschis fisierele de input si output, apoi am facut citirea. Am citit primul rand
intr-o variabila, apoi width-ul si height-ul de pe a doua linie, valoarea maxima
a unei culori de pe a treia linie, apoi matricea de pixeli (linie cu linie).
	-Daca primul argument este "-c", atunci initializeaza si construieste
"normal" un arbore quaternar (explic mai jos ce face fiecare functie in parte). 
Apoi, parcurg arborele in latime (BFS) cu ajutorul unei cozi pentru a numerota
nodurile cu indicii corespunzatori. Dupa asta, declar si aloc memoriei vectorului,
dupa care apelez functia prin care il completez cu informatiile care trebuiesc 
afisate in fisierul comprimat. La sfarsit, afisez informatiile si eliberez memoria
alocata vectorului.
	-Daca primul argument nu este "-c", atunci poate fi doar "-m". In cazul
acesta, daca al doilea argument este "h", se construieste arborele cu functia
"construireh", altfel (daca al doilea argument este "v") se apeleaza functia
"construirev". Apoi, se completeaza matricea de pixeli cu functia "construire2"
dupa care se afiseaza informatiile (imaginea) in fisierul de output.
La sfarsit, se elibereaza arborele, matricea si se inchid fisierele deschise.

-Daca primul argument este "-d", se deschid fisierele, se citeste numarul de frunze
si numarul de noduri, apoi vectorul din fisierul comprimat. Se initializeaza si se
construieste arborele quaternar folosind vectorul mentionat anterior. Apoi, se
completeaza matricea de pixeli, dupa care se afiseaza informatiile in fisierul de
output. La sfarsit se elibereaza vectorul, arborele, matricea si se inchid
fisierele deschise la inceput.


	Functiile din header.c

Functia init:
Aloca memorie radacinii, seteaza coordonatele patratului descris de acesta cu
(0, 0) si (width-1, height-1) (unde width si height sunt citite din fisierul de 
input) si da celor 4 fii valoarea de NULL.

Functia mean:
Calculeaza valoarea lui mean, folosindu-se de coordonatele unui nod (mai exact, de
coordonatele acestuia care descriu un patrat) si de matricea de pixeli.

Functia valori_medii:
Calculeaza valoarile medii ale lui red, green si blue, folosindu-se de coordonatele
unui nod (mai exact, de coordonatele acestuia care descriu un patrat) si de matricea
de pixeli. Functia returneaza un pixel ce contine cele 3 valori medii.

Functia construire:
Functia primeste ca parametru radacina arborelui care urmeaza sa fie construit. 
Pentru fiecare nod, calculeaza valorile medii ale lui red, green si blue
caracteristice nodului curent, apoi mean-ul. Daca mean-ul este <= factor, atunci
functia se opreste. Daca mean-ul este > factor, atunci, pentru fiecare fiu,
aloca memorie imparte coordonatele caracteristice pentru fiecare (pentru fiu1,
coordonatele patratului din stanga-sus, fiu2, coordonatele patratului din dreapta-sus
si tot asa...). Pentru cei 4 fii ai fiilor se da valoarea NULL, apoi se apeleaza
recursiv functia pentru fiecare fiu al nodului curent.

Functia deleteTree:
Este functia care elibereaza memoria alocata unui arbore.

Functia init_queue:
Este functia care initializeaza o coada.

Functia add_queue
Este functia care adauga un nod de tip *Qtree in coada.

Functia pop_queue
Este functia care sterge un nod de tip *Qtree din coada.

Functia completare_vector
Parcurge arborele folosind o parcurgere de tip BFS si completeaza vectorul cu datele
corespunzatoare pentru fisierul de compresie, folosindu-se de indicii nodurilor.
Observatie: este irelevant tipul de parcurgere, deoarece nodurile oricum sunt
numerotate, deci adaugarea in vector ar fi fost corecta oricum. Folosirea BFS-ului
este o simpla alegere, ar fi functionat cu orice tip de parcurgere.

Functia init2:
Este functia de initializare a radacinii arborelui, folosit la decompresie. Aloca
memorie radacinii, extrage din primul element din vectorul din input valorile medii
de red, green, blue, width-ul si height-ul (care sunt egale intre ele si egale
cu radical din size-ul gasit in element) si ii da indicelui radacinii valorea 0.
Fiilor radacinii le da valoarea NULL.

Functia construire2:
Daca nodul este frunza, adica unul dintre cele 4 indexuri ale fiilor este -1, atunci
iese din functie. Altfel, pentru fiecare fiu al nodului curent face urmatoarele
lucruri: aloca memorie, ii seteaza coordonatele (ca in construirea precedenta)
stanga-sus si dreapta-jos, ii seteaza indicele (folosindu-se de elementul care
ii corespunde din vector, adica element[index], unde element este vectorul si 
index este indexul nodului curent) si cele 3 valori ale lui red, blue, green.
Le da celor 4 fii valorile de NULL, apoi apeleaza functia recursiv pentru fiecare
fiu.

Functia completare_matrice:
Se parcurge arborele si, pentru fiecare nod frunza, se completeaza in matrice,
folosindu-se de coordonatele acestuia, pixelii cu valorile medii ale nodului.
Aceeasi observatie ca la functia "completare_vector", am folosit o parcurgere de
tip BFS, dar se putea folosi orice fel parcurgere.

Functia mean2:
Calculeaza valoarea lui mean, folosindu-se de coordonatele date in antet si de
matricea de pixeli.

Functia valori_medii2:
Calculeaza valoarile medii ale lui red, green si blue, folosindu-se de date in
antet si de matricea de pixeli. Functia returneaza un pixel ce contine cele 3
valori medii.

Functia construireh:
Calculeaza valorile medii si mean-ul nodului curent folosindu-se de coordonatele
din antet. Ii aloca memorie nodului curent, ii atribuie valorile medii calculate
pentru red, green, blue si le da fiilor valorile de NULL. Daca mean-ul este <= factor,
atunci iese din functie. Altfel, apeleaza functia recursiv pentru toti cei 4 fii in
ordine inversa pentru fiii 1, 2 si 3, 4. Practic, este aceeasi logica ce este si la
prima construire (folosita pentru compresare), doar ca nu se mai foloseste de 
coordonatele nodului curent, al carui patrat il imparte in 4 patrate si "da" fiecarui
fiu cate unul, ci se foloseste de coordonatele din antet, iar fiilor le da unul dintre
cele 4 patrate, doar ca in ordine inversa pentru fiii 1, 2 si 3, 4.

Functia construirev:
Functia face exact acelasi lucru ca functia precedenta, cu diferenta ca se apeleaza in 
ordine inversa functiile pentru fiii 1, 4 si 2, 3.

Functia completare_matrice2:
Parcurge arborele, calculeaza pentru fiecare valorile medii ale lui red, green, blue,
iar daca nodul este frunza, atunci pune in matrice pixeli cu valorile medii determinate
anterior. Functia se foloseste de coordonatele date in antet in loc de coordonatele
nodului (ca in ultimele doua functii) pentru a sti ce zona completeaza in matrice.