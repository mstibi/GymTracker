#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "header.h"

typedef enum {
	DODAJ_CLANA = 1,
	IZBRISI_CLANA,
	ISPISI_CLANOVE,
	SORTIRAJ_PO_IMENU,
	SORTIRAJ_PO_PREZIMENU,
	PRETRAZI_PO_ID,
	NAJVECI_ID,
	IZLAZ
} IzbornikOpcija;

int main(void) {
	int id = 0;
	int brojClanova = 0;
	IzbornikOpcija odabir;
	FILE* file = fopen("gym.txt", "a+");
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
		printf("| 3 - Ispisi sve clanove               |\n");
		printf("| 4 - Sortiraj po imenu                |\n");
		printf("| 5 - Sortiraj po prezimenu            |\n");
		printf("| 6 - Pretrazi po ID-u                 |\n");
		printf("| 7 - Prikazi clana s najvecim ID-em   |\n");
		printf("| 8 - Izlaz                            |\n");
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
		case ISPISI_CLANOVE:
			ispisClanova(clanovi, brojClanova);
			break;
		case SORTIRAJ_PO_IMENU:
			break;
		case SORTIRAJ_PO_PREZIMENU:
			break;
		case PRETRAZI_PO_ID:
			
			break;
		case NAJVECI_ID:
			break;
		case IZLAZ:
			break;
		default:
			break;
		}
	} while (odabir != IZLAZ);

	
	return 0;
}
//4,5,7,10,11,17