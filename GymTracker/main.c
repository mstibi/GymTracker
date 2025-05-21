#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "header.h"

typedef enum {
	DODAJ_CLANA = 1,
	IZBRISI_CLANA,
	UREDI_CLANA,
	ISPISI_CLANOVE,
	SORTIRAJ_PO_IMENU,
	SORTIRAJ_PO_PREZIMENU,
	PRETRAZI_PO_ID,
	NAJVECI_ID,
	REMOVE_DATOTEKE,   
	RENAME_DATOTEKE,   
	IZLAZ
} IzbornikOpcija;

int main(void) {
	
	int id = 0;
	IzbornikOpcija odabir;
	FILE* file = fopen(trenutnaDatoteka, "a+");  
	if (!file) {
		perror("Greska pri otvaranju datoteke");
		return 1;
	}
	CLAN** clanovi = ucitajClanoveIzDatoteke(file, &brojClanova);
	fclose(file);

	do {
		printf("\n");
		printf("========================================\n");
		printf("|         GYM MANAGEMENT SYSTEM        |\n");
		printf("========================================\n");
		printf("| 1 - Dodaj clana                      |\n");
		printf("| 2 - Izbrisi clana                    |\n");
		printf("| 3 - Uredi clana                      |\n");    
		printf("| 4 - Ispisi sve clanove               |\n");
		printf("| 5 - Sortiraj po imenu                |\n");
		printf("| 6 - Sortiraj po prezimenu            |\n");
		printf("| 7 - Pretrazi po ID-u                 |\n");
		printf("| 8 - Prikazi clana s najvecim ID-em   |\n");
		printf("| 9 - Obrisi datoteku                  |\n");
		printf("|10 - Preimenuj datoteku               |\n");
		printf("|11 - Izlaz                            |\n");
		printf("========================================\n");
		printf("Odabir: ");
		if (scanf("%d", (int*)&odabir) != 1) {
			printf("Neispravan unos!\n");
			while (fgetc(stdin) != '\n');
			continue;
		}
		while (getchar() != '\n' && !feof(stdin));

		switch (odabir) {
		case DODAJ_CLANA:
			dodajClana(&clanovi, &brojClanova);
			break;
		case IZBRISI_CLANA:
			printf("Unesi ID clana za brisanje: ");
			if (scanf("%d", &id) == 1) {
				while (getchar() != '\n' && !feof(stdin));
				izbrisiClana(&clanovi, &brojClanova, id);
			}
			else {
				printf("Neispravan unos!\n");
				while (getchar() != '\n' && !feof(stdin));
			}
			break;
		case UREDI_CLANA:
			printf("Unesi ID clana za uređivanje: ");
			if (scanf("%d", &id) == 1) {
				while (getchar() != '\n' && !feof(stdin));
				urediClana(clanovi, brojClanova, id);
			}
			else {
				printf("Neispravan unos!\n");
				while (getchar() != '\n' && !feof(stdin));
			}
			break;
		case ISPISI_CLANOVE:
			ispisClanova(clanovi, brojClanova);
			break;
		case SORTIRAJ_PO_IMENU:
			ispisSortiranihClanovaPoImenu(clanovi, brojClanova);
			break;
		case SORTIRAJ_PO_PREZIMENU:
			ispisSortiranihClanovaPoPrezimenu(clanovi, brojClanova);
			break;
		case PRETRAZI_PO_ID:
			printf("Unesi ID clana za pretragu: ");
			if (scanf("%d", &id) == 1) {
				while (getchar() != '\n' && !feof(stdin));
				pretrazivanje(clanovi, brojClanova, id);
			}
			else {
				printf("Neispravan unos!\n");
				while (getchar() != '\n' && !feof(stdin));
			}
			break;
		case NAJVECI_ID:
			najveciID(clanovi, brojClanova);
			break;
		case REMOVE_DATOTEKE:
			fclose(file); // Zatvori prije brisanja ako je otvorena
			obrisiDatoteku();
			// Ponovno otvori datoteku ako je potrebno
			file = fopen(trenutnaDatoteka, "a+");
			break;
		case RENAME_DATOTEKE:
			preimenujDatoteku();
			break;
		case IZLAZ:
			printf("Hvala na koristenju programa! Dovidjenja.\n");
			break;
		default:
			printf("Nepostojeca opcija!\n");
			break;
		}
	} while (odabir != IZLAZ);

	// Spremi sve članove u datoteku na izlazu
	file = fopen(trenutnaDatoteka, "w");
	if (!file) {
		perror("Greska pri otvaranju datoteke za spremanje");
	}
	else {
		spremiClanoveUDatoteku(file, clanovi, brojClanova);
		fclose(file);
	}
	oslobodiClanove(clanovi, brojClanova);

	return 0;
}

//4,5,7,10,11,17 odradeno prosli put

/*
1.	CRUD operacije
Create/Read/Delete su implementirani

2.	Primitivni tipovi podataka
Korišteni int, char*.

3.	Složeni tipovi podataka
Struktura CLAN i dinamički nizovi.

6.	Primjena ključne riječi static za globalne i lokalne varijable.
static int usporedbaClanova();

8.	extern - global.c

9.	Makro inline
static inline int validirajImePrezime();

12.korišteni pokazivači

13.	korištene strukture i funkcije

14.	Zaštita parametara
svi parametri su mi const

15.	ne koristim vla polja, tipa koristim char buffer u dodajClana

16.	koristim dinamicko zauzimanje memorije

18.	Sigurno brisanje memorije – u oslobodiClanove - Postavljanje clanovi[i] = NULL nakon free()

19.	Podrska za txt. Datoteke

20.	Fseek u UcitajClanoveIzDatoteke

21.	Remove() i rename() NeRade

22.	Korisim perror

23.	Koristim qsort()

24.	Bsearch()

25.	Rekurzija binarnapretraga

26.	koristim i qsort i bsearch
*/