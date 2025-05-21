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
void spremiClanoveUDatoteku(FILE* file, CLAN* const* clanovi, int brojClanova);
void oslobodiClanove(CLAN** clanovi, int brojClanova);

// CRUD
void dodajClana(CLAN*** clanovi, int* brojClanova);
void izbrisiClana(CLAN*** clanovi, int* brojClanova, int id);
void urediClana(CLAN** clanovi, int brojClanova, int id);
void ispisClanova(CLAN* const* clanovi, int brojClanova);
void ispisSortiranihClanovaPoImenu(CLAN* const* clanovi, int brojClanova);
void ispisSortiranihClanovaPoPrezimenu(CLAN* const* clanovi, int brojClanova);
void pretrazivanje(CLAN* const* clanovi, int brojClanova, int id);
void najveciID(CLAN* const* clanovi, int brojClanova);
void obrisiDatoteku(void);
void preimenujDatoteku(void);

// extern 
extern int brojClanova;
extern char* trenutnaDatoteka;

#pragma once
#endif
