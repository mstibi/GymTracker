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
        if (!novi) {
            perror("Greska pri alokaciji");
            exit(EXIT_FAILURE);
        }

        novi->ime = malloc(strlen(ime) + 1);
        novi->prezime = malloc(strlen(prezime) + 1);

        if (!novi->ime || !novi->prezime) {
            perror("Greska pri alokaciji");
            exit(EXIT_FAILURE);
        }

        strcpy(novi->ime, ime);
        strcpy(novi->prezime, prezime);
        novi->id = id;

        CLAN** temp = realloc(clanovi, (*brojClanova + 1) * sizeof(CLAN*));
        if (!temp) {
            perror("Greska pri realokaciji");
            exit(EXIT_FAILURE);
        }
        clanovi = temp;
        clanovi[*brojClanova] = novi;
        (*brojClanova)++;
    }
    return clanovi;
}

// Spremanje članova u datoteku
void spremiClanoveUDatoteku(FILE* file, CLAN* const* clanovi, int brojClanova) {
    freopen(NULL, "w", file); // Očisti datoteku
    for (int i = 0; i < brojClanova; i++)
        fprintf(file, "%s %s %d\n", clanovi[i]->ime, clanovi[i]->prezime, clanovi[i]->id);
}

// Oslobađanje memorije svih članova
void oslobodiClanove(CLAN** clanovi, int brojClanova) {
    for (int i = 0; i < brojClanova; i++) {
        if (clanovi[i]) {
            free(clanovi[i]->ime); clanovi[i]->ime = NULL;
            free(clanovi[i]->prezime); clanovi[i]->prezime = NULL;
            free(clanovi[i]); clanovi[i] = NULL;
        }
    }
    free(clanovi);
}

// Dodavanje člana
void dodajClana(CLAN*** clanovi, int* brojClanova) {
    char buffer[100];
    CLAN* novi = malloc(sizeof(CLAN));
    if (!novi) {
        perror("Greska pri alokaciji");
        exit(EXIT_FAILURE);
    }
    novi->ime = NULL;
    novi->prezime = NULL;

    // Ime
    do {
        printf("Ime: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        if (!validirajImePrezime(buffer) || strlen(buffer) == 0) {
            printf("Ime smije sadrzavati samo slova. Pokusaj ponovno.\n");
            novi->ime = NULL;
        }
        else {
            novi->ime = malloc(strlen(buffer) + 1);
            if (!novi->ime) {
                perror("Greska pri alokaciji");
                exit(EXIT_FAILURE);
            }
            strcpy(novi->ime, buffer);
        }
    } while (!novi->ime);

    // Prezime
    do {
        printf("Prezime: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        if (!validirajImePrezime(buffer) || strlen(buffer) == 0) {
            printf("Prezime smije sadrzavati samo slova. Pokusaj ponovno.\n");
            novi->prezime = NULL;
        }
        else {
            novi->prezime = malloc(strlen(buffer) + 1);
            if (!novi->prezime) {
                perror("Greska pri alokaciji");
                exit(EXIT_FAILURE);
            }
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
    if (!*clanovi) {
        perror("Greska pri realokaciji");
        exit(EXIT_FAILURE);
    }
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
void ispisClanova(CLAN* const* clanovi, int brojClanova) {
    printf("Clanovi teretane:\n");
    for (int i = 0; i < brojClanova; i++)
        printf("Ime: %s %s\nID: %d\n\n", clanovi[i]->ime, clanovi[i]->prezime, clanovi[i]->id);
}

// Usporedba po imenu
static int usporedbaClanova(const void* a, const void* b) {
    CLAN* const* ca = (CLAN* const*)a;
    CLAN* const* cb = (CLAN* const*)b;
    return strcmp((*ca)->ime, (*cb)->ime);
}

// Usporedba po prezimenu
static int usporedbaClanovaPoPrezimenu(const void* a, const void* b) {
    CLAN* const* ca = (CLAN* const*)a;
    CLAN* const* cb = (CLAN* const*)b;
    return strcmp((*ca)->prezime, (*cb)->prezime);
}

// Sortirani ispis po imenu
void ispisSortiranihClanovaPoImenu(CLAN* const* clanovi, int brojClanova) {
    if (brojClanova == 0) {
        printf("Nema clanova za ispis.\n");
        return;
    }
    CLAN** kopija = calloc(brojClanova, sizeof(CLAN*));
    for (int i = 0; i < brojClanova; i++) kopija[i] = (CLAN*)clanovi[i];
    qsort(kopija, brojClanova, sizeof(CLAN*), usporedbaClanova);
    printf("Sortirani clanovi teretane po imenu:\n");
    for (int i = 0; i < brojClanova; i++)
        printf("Ime: %s %s\nID: %d\n\n", kopija[i]->ime, kopija[i]->prezime, kopija[i]->id);
    free(kopija);
}

// Sortirani ispis po prezimenu
void ispisSortiranihClanovaPoPrezimenu(CLAN* const* clanovi, int brojClanova) {
    if (brojClanova == 0) {
        printf("Nema clanova za ispis.\n");
        return;
    }
    CLAN** kopija = calloc(brojClanova, sizeof(CLAN*));
    for (int i = 0; i < brojClanova; i++) kopija[i] = (CLAN*)clanovi[i];
    qsort(kopija, brojClanova, sizeof(CLAN*), usporedbaClanovaPoPrezimenu);
    printf("Sortirani clanovi teretane po prezimenu:\n");
    for (int i = 0; i < brojClanova; i++)
        printf("Ime: %s %s\nID: %d\n\n", kopija[i]->ime, kopija[i]->prezime, kopija[i]->id);
    free(kopija);
}

static int usporediID(const void* key, const void* clan) {
    int trazeniID = *(const int*)key;
    const CLAN* c = *(const CLAN* const*)clan;
    return trazeniID - c->id;
}

// Pretraga člana po ID-u
void pretrazivanje(CLAN** clanovi, int brojClanova, int id) {
    if (brojClanova == 0) {
        printf("Nema clanova za pretragu.\n");
        return;
    }

    // Privremeno sortiraj po ID-u
    CLAN** temp = malloc(brojClanova * sizeof(CLAN*));
    memcpy(temp, clanovi, brojClanova * sizeof(CLAN*));
    qsort(temp, brojClanova, sizeof(CLAN*), usporediID);

    // Koristi rekurzivnu binarnu pretragu
    int pronadenIndex = binarnaPretraga(temp, 0, brojClanova - 1, id);

    if (pronadenIndex != -1) {
        printf("Clan je pronaden:\nIme: %s %s\nID: %d\n",
            temp[pronadenIndex]->ime, temp[pronadenIndex]->prezime, temp[pronadenIndex]->id);
    }
    else {
        printf("Clan nije pronaden.\n");
    }

    free(temp);
}


// Najveći ID
void najveciID(CLAN* const* clanovi, int brojClanova) {
    if (brojClanova == 0) {
        printf("Nema pronadenih clanova.\n");
        return;
    }
    int max = clanovi[0]->id, idx = 0;
    for (int i = 1; i < brojClanova; i++)
        if (clanovi[i]->id > max) { max = clanovi[i]->id; idx = i; }
    printf("Clan koji ima najveci ID:\nIme: %s\nPrezime: %s\nID: %d\n", clanovi[idx]->ime, clanovi[idx]->prezime, clanovi[idx]->id);
}

void urediClana(CLAN** clanovi, int brojClanova, int id) {
    for (int i = 0; i < brojClanova; i++) {
        if (clanovi[i]->id == id) {
            CLAN* clan = clanovi[i];
            char buffer[100];
            int noviID;
            int duplicate;

            // Ažuriraj ime
            do {
                printf("Trenutno ime: %s\n", clan->ime);
                printf("Novo ime (Enter za zadrzati): ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                if (strlen(buffer) == 0) break;

                if (!validirajImePrezime(buffer)) {
                    printf("Ime smije sadrzavati samo slova!\n");
                    continue;
                }

                free(clan->ime);
                clan->ime = malloc(strlen(buffer) + 1);
                if (!clan->ime) {
                    perror("Greska pri alokaciji");
                    exit(EXIT_FAILURE);
                }
                strcpy(clan->ime, buffer);
                break;
            } while (1);

            // Ažuriraj prezime
            do {
                printf("Trenutno prezime: %s\n", clan->prezime);
                printf("Novo prezime (Enter za zadrzati): ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                if (strlen(buffer) == 0) break;

                if (!validirajImePrezime(buffer)) {
                    printf("Prezime smije sadrzavati samo slova!\n");
                    continue;
                }

                free(clan->prezime);
                clan->prezime = malloc(strlen(buffer) + 1);
                if (!clan->prezime) {
                    perror("Greska pri alokaciji");
                    exit(EXIT_FAILURE);
                }
                strcpy(clan->prezime, buffer);
                break;
            } while (1);

            // Ažuriraj ID
            do {
                printf("Trenutni ID: %d\n", clan->id);
                printf("Novi ID (Enter za zadržati): ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                if (strlen(buffer) == 0) break;

                if (!validirajID(buffer)) {
                    printf("ID mora biti broj!\n");
                    continue;
                }

                noviID = atoi(buffer);
                if (noviID == clan->id) break;

                duplicate = 0;
                for (int j = 0; j < brojClanova; j++) {
                    if (clanovi[j]->id == noviID) {
                        duplicate = 1;
                        break;
                    }
                }

                if (duplicate) {
                    printf("ID vec postoji!\n");
                    continue;
                }

                clan->id = noviID;
                break;
            } while (1);

            printf("Clan azuriran!\n");
            return;
        }
    }
    printf("Clan s ID-om %d nije pronaden.\n", id);
}

void obrisiDatoteku(void) {
    if (remove(trenutnaDatoteka) == 0) {  
        printf("Datoteka %s je uspjesno obrisana.\n", trenutnaDatoteka);
        trenutnaDatoteka = "gym.txt";  
    }
    else {
        perror("Greska pri brisanju datoteke");
    }
}


void preimenujDatoteku(void) {
    char novoIme[100];
    printf("Unesi novo ime datoteke (npr. gym_backup.txt): ");
    fgets(novoIme, sizeof(novoIme), stdin);
    novoIme[strcspn(novoIme, "\n")] = 0;

    if (strlen(novoIme) == 0) {
        printf("Niste unijeli novo ime!\n");
        return;
    }

    if (rename(trenutnaDatoteka, novoIme) == 0) {
        printf("Datoteka je uspjesno preimenovana u %s.\n", novoIme);
        trenutnaDatoteka = _strdup(novoIme);
    }
    else {
        perror("Greska pri preimenovanju datoteke");
    }
}
static int binarnaPretraga(CLAN** clanovi, int lijevo, int desno, int trazeniID) {
    if (lijevo > desno)
        return -1;

    int sredina = lijevo + (desno - lijevo) / 2;
    int trenutniID = clanovi[sredina]->id;

    if (trenutniID == trazeniID)
        return sredina;
    else if (trenutniID < trazeniID)
        return binarnaPretraga(clanovi, sredina + 1, desno, trazeniID);
    else
        return binarnaPretraga(clanovi, lijevo, sredina - 1, trazeniID);
}


