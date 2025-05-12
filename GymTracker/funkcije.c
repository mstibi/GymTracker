#define _CRT_SECURE_NO_WARNINGS
#include "header.h"

int validirajImePrezime(const char* unos) {
	for (int i = 0; unos[i] != '\0' && unos[i] != '\n'; ++i)
		if (!isalpha((unsigned char)unos[i])) return 0;
	return 1;
}

int validirajID(const char* unos) {
	for (int i = 0; unos[i] != '\0' && unos[i] != '\n'; ++i)
		if (!isdigit((unsigned char)unos[i])) return 0;
	return 1;
}

// Učitavanje članova iz datoteke u dinamički niz pokazivača
CLAN** ucitajClanoveIzDatoteke(FILE* file, int* brojClanova) {
	CLAN** clanovi = NULL;
	*brojClanova = 0;
	char ime[100], prezime[100];
	int id;
	fseek(file, 0, SEEK_SET);
	while (fscanf(file, "%99s %99s %d", ime, prezime, &id) == 3) {
		CLAN* novi = malloc(sizeof(CLAN));
		novi->ime = malloc(strlen(ime) + 1);
		novi->prezime = malloc(strlen(prezime) + 1);
		strcpy(novi->ime, ime);
		strcpy(novi->prezime, prezime);
		novi->id = id;
		clanovi = realloc(clanovi, (*brojClanova + 1) * sizeof(CLAN*));
		clanovi[*brojClanova] = novi;
		(*brojClanova)++;
	}
	return clanovi;
}

// Spremanje članova u datoteku
void spremiClanoveUDatoteku(FILE* file, CLAN** clanovi, int brojClanova) {
	freopen(NULL, "w", file); // Očisti datoteku
	for (int i = 0; i < brojClanova; i++)
		fprintf(file, "%s %s %d\n", clanovi[i]->ime, clanovi[i]->prezime, clanovi[i]->id);
}

// Oslobađanje memorije svih članova
void oslobodiClanove(CLAN** clanovi, int brojClanova) {
	for (int i = 0; i < brojClanova; i++) {
		free(clanovi[i]->ime);
		free(clanovi[i]->prezime);
		free(clanovi[i]);
	}
	free(clanovi);
}

// Dodavanje člana
void dodajClana(CLAN*** clanovi, int* brojClanova) {
	char buffer[100];
	CLAN* novi = malloc(sizeof(CLAN));
	// Ime
	do {
		printf("Ime: ");
		fgets(buffer, sizeof(buffer), stdin);
		buffer[strcspn(buffer, "\n")] = 0;
		if (!validirajImePrezime(buffer) || strlen(buffer) == 0)
			printf("Ime smije sadrzavati samo slova. Pokusaj ponovno.\n");
		else {
			novi->ime = malloc(strlen(buffer) + 1);
			strcpy(novi->ime, buffer);
		}
	} while (!novi->ime);

	// Prezime
	do {
		printf("Prezime: ");
		fgets(buffer, sizeof(buffer), stdin);
		buffer[strcspn(buffer, "\n")] = 0;
		if (!validirajImePrezime(buffer) || strlen(buffer) == 0)
			printf("Prezime smije sadrzavati samo slova. Pokusaj ponovno.\n");
		else {
			novi->prezime = malloc(strlen(buffer) + 1);
			strcpy(novi->prezime, buffer);
		}
	} while (!novi->prezime);

	// ID
	int duplicateID;
	do {
		duplicateID = 0;
		printf("ID: ");
		fgets(buffer, sizeof(buffer), stdin);
		buffer[strcspn(buffer, "\n")] = 0;
		if (!validirajID(buffer) || strlen(buffer) == 0) {
			printf("ID smije sadržavati samo znamenke. Pokusaj ponovno.\n");
			continue;
		}
		novi->id = atoi(buffer);
		for (int i = 0; i < *brojClanova; i++) {
			if ((*clanovi)[i]->id == novi->id) {
				printf("Clan s unesenim ID-om vec postoji. Odaberi drugi ID.\n");
				duplicateID = 1;
				break;
			}
		}
	} while (duplicateID || !validirajID(buffer) || strlen(buffer) == 0);

	*clanovi = realloc(*clanovi, (*brojClanova + 1) * sizeof(CLAN*));
	(*clanovi)[*brojClanova] = novi;
	(*brojClanova)++;
	printf("Clan je uspjesno dodan.\n");
}

// Brisanje člana
void izbrisiClana(CLAN*** clanovi, int* brojClanova, int id) {
	int found = 0;
	for (int i = 0; i < *brojClanova; i++) {
		if ((*clanovi)[i]->id == id) {
			free((*clanovi)[i]->ime);
			free((*clanovi)[i]->prezime);
			free((*clanovi)[i]);
			for (int j = i; j < *brojClanova - 1; j++)
				(*clanovi)[j] = (*clanovi)[j + 1];
			(*brojClanova)--;
			*clanovi = realloc(*clanovi, (*brojClanova) * sizeof(CLAN*));
			printf("Clan je izbrisan.\n");
			found = 1;
			break;
		}
	}
	if (!found) printf("Clan nije pronaden.\n");
}

// Ispis svih članova
void ispisClanova(CLAN** clanovi, int brojClanova) {
	printf("Clanovi teretane:\n");
	for (int i = 0; i < brojClanova; i++)
		printf("Ime: %s %s\nID: %d\n\n", clanovi[i]->ime, clanovi[i]->prezime, clanovi[i]->id);
}

