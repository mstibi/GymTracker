#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
	char* ime;
	char* prezime;
	int id;
} CLAN;

// Validacija
int validirajImePrezime(const char* unos);
int validirajID(const char* unos);

// Dinamički rad s članovima
CLAN** ucitajClanoveIzDatoteke(FILE* file, int* brojClanova);
void spremiClanoveUDatoteku(FILE* file, CLAN** clanovi, int brojClanova);
void oslobodiClanove(CLAN** clanovi, int brojClanova);

// CRUD
void dodajClana(CLAN*** clanovi, int* brojClanova);
void izbrisiClana(CLAN*** clanovi, int* brojClanova, int id);
void ispisClanova(CLAN** clanovi, int brojClanova);


#endif
