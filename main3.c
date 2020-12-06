#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
//#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


#define	MAX_IMG_SIZE 40000000 // 4Mpx


struct s_pgm {
	char nazwa[50];
	int szer, wys, max_v;
	short unsigned int** obraz;
};

int dajInt() {
	unsigned int i = 0;
	int liczba, loop = 0;
	char buff[1000];
	do {
		scanf("%999s", buff);
		for (i = 0; i < strlen(buff); i++) {
			if (!(isdigit(buff[i]) || buff[i] == '-') || buff[i] == '.') {
				printf("Podaj liczbe calkowita! : ");
				loop = 1;
				break;
			}
			loop = 0;
		}
	} while (loop == 1 || buff[0] == '\0' || buff[0] == '\n');
	liczba = atol(buff);
	return liczba;
}

float dajFloat() {
	int i;
	short unsigned int loop = 0;
	float liczba;
	char buff[1000];
	do {
		scanf("%999s", buff);
		for (i = 0; i < strlen(buff); i++) {
			if (!(isdigit(buff[i]) || buff[i] == '.' || buff[i] == '-')) {
				printf("Podaj liczbe! : ");
				loop = 1;
				break;
			}
			buff[strlen(buff)] = '\0';
			loop = 0;
		}
	} while (loop == 1 || buff[0] == '\0' || buff[0] == '\n');
	liczba = atof(buff);
	return liczba;
}

void dajNazwe(char* nazwa_pliku, char* rozsz) {
	int i;
	short unsigned int loop = 0;
	char t = '\0';
	char buff[1000];
	nazwa_pliku[0] = '\0';
	do {
		printf("Podaj nazwe pliku %s: ", rozsz);
		scanf("%999s", buff);
		if (strlen(buff) > 50) {
			printf("Nazwa pliku jest za dluga!\n");
			continue;
		}
		for (i = 0; i < strlen(buff); i++) {
			t = buff[i];
			if (t == '/' || t == '\\' || t == '$' || t == '.') {
				printf("Nazwa zawiera niedozwolone znaki!\n");
				loop = 1;
				break;
			}
			loop = 0;
		}
	} while (loop == 1 || buff[0] == '\0' || buff[0] == '\n');
	sprintf(nazwa_pliku, "%s.%s", buff, rozsz);
	return;
}

int tak_czy_nie() {
	short unsigned int q = 0;
	do {
		printf("	1) Tak\n	2) Nie\n");
		q = dajInt();
		if (q == 2) {
			return 0;
		}
		else if (q == 1) {
			return 1;
		}
		else {
			printf("Podaj liczbe 1 lub 2\n");
			continue;
		}

	} while (!(q == 1 || q == 2));
	printf("\n");
	return 0;
}

void swapInt(int* a, int* b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
	return;
}

void szum_sip(int max_v, int szer, int wys, short unsigned int** obraz) {
	short unsigned int prawdop = 0;
	int i = 0, j = 0;
	srand(time(NULL));
	do {
		printf("Podaj wypelnienie szumem w %%\n");
		prawdop = dajInt();
		if (prawdop > 100 || prawdop < 0) {
			continue;
		}
		else
			break;
	} while (1);
	for (i = 0; i < wys; i++) {
		for (j = 0; j < szer; j++) {
			if (prawdop >= (rand() % 101)) {
				if ((rand() % 2) == 0) {
					obraz[i][j] = 0;
				}
				else {
					obraz[i][j] = max_v;
				}
			}
			else
				continue;
		}
	}
	puts("Pomyslnie zaszumiono obraz");
	return;
}

void filtr_g(struct s_pgm* img) {
	short unsigned int gaus3x3[3][3] = {
	{1, 2, 1},
	{2, 4, 2},
	{1, 2, 1}
	};
	unsigned int i, j, k, m, tmp_v, tmp_s, tmp_w, suma = 0, suma_wag = 0;
	short unsigned int** obrazcpy;
	obrazcpy = malloc(img->wys * sizeof(short unsigned int*));
	for (i = 0; i < img->wys; i++) {
		obrazcpy[i] = malloc(img->szer * sizeof(short unsigned int));
	}
	for (i = 0; i < img->wys; i++) {
		for (j = 0; j < img->szer; j++) {
			obrazcpy[i][j] = img->obraz[i][j];
		}
	}
	for (i = 0; i < img->wys; i++) {
		for (j = 0; j < img->szer; j++) {
			suma = 0;
			suma_wag = 0;
			for (k = 0; k < 3; k++) {
				for (m = 0; m < 3; m++) {
					if ((i - 1 + k) >= 0 && (i - 1 + k) < img->wys && (j - 1 + m) >= 0 && (j - 1 + m) < img->szer) {
						tmp_s = obrazcpy[i - 1 + k][j - 1 + m];
						tmp_w = gaus3x3[k][m];
						suma += tmp_s * tmp_w;
						suma_wag += tmp_w;
					}
					else
						continue;
				}
			}
			//printf("%u, %u\n", suma, suma_wag);
			tmp_v = (suma / suma_wag);
			if (tmp_v > img->max_v)
				tmp_v = img->max_v;
			if (tmp_v < 0)
				tmp_v = 0;
			img->obraz[i][j] = tmp_v;
			//printf("%u", img->obraz[i][j]);
		}
	}
	for (i = 0; i < img->wys; i++) {
		free(obrazcpy[i]);
	}
	free(obrazcpy);
	//printf("Pomyslnie przefiltrowano obraz '%s' filtrem Gaussa z oknem 3x3.\n", img->nazwa);
	return;
}

void obrot_p(struct s_pgm* img) {
	unsigned int i = 0, j = 0;
	short unsigned int** tmp_obraz;
	tmp_obraz = malloc((img->szer) * sizeof(short unsigned int*));
	for (i = 0; i < img->szer; i++) {
		tmp_obraz[i] = malloc(img->wys * sizeof(short unsigned int));
	}
	for (i = 0; i < img->wys; i++) {
		for (j = 0; j < img->szer; j++) {
			tmp_obraz[j][img->wys - i - 1] = img->obraz[i][j];
		}
	}
	img->obraz = tmp_obraz;
	//free(tmp_obraz);	
	swapInt(&img->szer, &img->wys);
	return;
}

void negatyw(int max_v, int szer, int wys, short unsigned int** obraz) {
	int i = 0, j = 0;
	for (i = 0; i < wys; i++) {
		for (j = 0; j < szer; j++) {
			obraz[i][j] = max_v - obraz[i][j];
		}
	}
	puts("Pomyslnie utworzono negatyw");
}
///*

void zapis_csv(unsigned int* arg, long unsigned int* wyniki, int PODZIALKA, char* nazwa_pliku) {
	int i = 0;
	FILE* wsk_pliku;
	printf("Czy chcesz zapisac plik '%s'\n", nazwa_pliku);
	if (!tak_czy_nie())
		return;
	wsk_pliku = fopen(nazwa_pliku, "w");
	for (i = 0; i < PODZIALKA; i++) {
		fprintf(wsk_pliku, "%u;%lu\n", arg[i], wyniki[i]);
	}
	fclose(wsk_pliku);
	printf("Zapisano plik: '%s'\n", nazwa_pliku);
}

void histogram(struct s_pgm* img) {
	int i = 0, j = 0;
	char nazwa[150];
	char tmp_nazwa[100];
	unsigned int* arg;
	long unsigned int* tab_histo;
	arg = calloc(img->max_v + 2, sizeof(unsigned int));
	tab_histo = calloc(img->max_v + 2, sizeof(long unsigned int));
	strcpy(tmp_nazwa, img->nazwa);
	for (i = 0; i < strlen(tmp_nazwa); i++) {
		if (tmp_nazwa[i] == '.')
			tmp_nazwa[i] = '\0';
	}
	sprintf(nazwa, "%s_histogram.csv", tmp_nazwa);
	for (i = 0; i < img->max_v + 1; i++) {
		arg[i] = i;
	}
	for (i = 0; i < (img->wys); i++) {
		for (j = 0; j < img->szer; j++) {
			tab_histo[img->obraz[i][j]]++;
		}
	}
	zapis_csv(arg, tab_histo, img->max_v + 1, nazwa);
	free(arg);
	free(tab_histo);
}
//*/

void zapis_pgm(struct s_pgm* img) {
	int i = 0, j = 0, k = 0, nextChar = 0, diff = 0;
	char strMax[10];
	char strLiczba[10];
	char nn[100];
	for (i = 0; i < strlen(img->nazwa); i++) {
		if (img->nazwa[i] == '.')
			img->nazwa[i] = '\0';
	}
	sprintf(nn, "%s_kopia.pgm", img->nazwa);
	printf("Plik zostanie zapisany jako '%s'\nCzy chcesz nadac nowa nazwe?:\n", nn);
	if (tak_czy_nie())
		dajNazwe(nn, "pgm");
	FILE* wsk_pliku = fopen(nn, "w");
	fprintf(wsk_pliku, "P2\n%d %d\n%d\n", img->szer, img->wys, img->max_v);
	sprintf(strMax, "%d", img->max_v);
	for (i = 0; i < img->wys; i++) {
		for (j = 0; j < img->szer; j++) {
			nextChar += strlen(strMax);
			if (nextChar >= (50 - strlen(strMax))) {
				fprintf(wsk_pliku, "\n");
				nextChar = 0;
			}
			sprintf(strLiczba, "%u", img->obraz[i][j]);
			diff = strlen(strMax) - strlen(strLiczba);
			for (k = 0; k < diff; k++) {
				fprintf(wsk_pliku, " ");
			}
			fprintf(wsk_pliku, "%u ", img->obraz[i][j]);
		}
	}
	fclose(wsk_pliku);
	printf("Zapisano plik: '%s'\n", nn);
}

void odczyt_pgm(struct s_pgm* img, char** komentarze) {
	long int nextNum = 0;
	int i = 0, j = 0, k = 0, nextDigit = 0, kom = 0;
	//short unsigned int loop = 0;
	//size_t obraz_size = 0;
	//char *tmpKomentarze;
	char tmpBuff[2000];
	char strLiczba[20];
	char nazwa_pliku[50];
	short unsigned int* tmpObraz;
	FILE* wsk_pliku;
	tmpObraz = malloc(MAX_IMG_SIZE * sizeof(short unsigned int));
	//tmpKomentarze = malloc(150);
	do {
		dajNazwe(nazwa_pliku, "pgm");
		wsk_pliku = fopen(nazwa_pliku, "r");
		if (wsk_pliku == NULL) {
			printf("Nie udalo sie odczytac pliku.\n");
			continue;
		}
		else
			break;
	} while (1);
	strcpy(img->nazwa, nazwa_pliku);
	//img = malloc(sizeof(img));    
	while (fgets(tmpBuff, 2000, wsk_pliku) != NULL) {
		if (tmpBuff[0] == '#') {
			if (kom == 0) {
				sprintf(*komentarze + strlen(*komentarze), "Komentarze pliku: '%s':\n", img->nazwa);
			}
			//tmpKomentarze = realloc(tmpKomentarze, (kom + 1) * 150);
			*komentarze = realloc(*komentarze, (strlen(*komentarze) + 300) * sizeof(char));
			sprintf(*komentarze + strlen(*komentarze), "%s", tmpBuff);
			kom++;
			continue;
		}
		if (j == 0 && strcmp(tmpBuff, "P2\n") == 1) {
			puts("Podany plik nie jest w standardzie ASCII!");
			break;
		}
		else if (j == 0) {
			j++;
			continue;
		}
		//odczyt rozmiarow
		if (j > 0) {
			if ((img->szer) * (img->wys) > MAX_IMG_SIZE) {
				printf("Blad odczytu! Obraz jest za duzy.\n");
				break;
			}
			for (i = 0; i < strlen(tmpBuff); i++) {
				if (i > 0 && isdigit(tmpBuff[i - 1]) && isspace(tmpBuff[i]) && strLiczba[0] != '\0' && j == 1) {
					nextDigit = 0;
					if (nextNum == 0) {
						img->szer = atol(strLiczba);
					}
					if (nextNum == 1) {
						img->wys = atol(strLiczba);
					}
					nextNum++;
					continue;
				}
				if (i > 0 && isdigit(tmpBuff[i - 1]) && isspace(tmpBuff[i]) && strLiczba[0] != '\0' && j == 2) {
					nextDigit = 0;
					img->max_v = atol(strLiczba);
					nextNum = 0;
					continue;
				}
				//printf("szerokosc = %d, wys = %d\n", img->szer, img->wys);              
				if (j == 3 && i == 0) {
					nextNum = 0;
					img->obraz = malloc((img->wys) * sizeof(short unsigned int*));
					for (k = 0; k < img->wys; k++) {
						img->obraz[k] = malloc(img->szer * sizeof(short unsigned int));
					}
				}
				//*img = realloc(*img, (sizeof(*img) + ((img->szer) * (img->wys) * sizeof(short unsigned int))));     
				//*img_ptr = realloc(*img_ptr, (count + 1) * (sizeof(*img_ptr) + (img->szer * img->wys * sizeof(short unsigned int))));  
				if (i > 0 && isdigit(tmpBuff[i - 1]) && isspace(tmpBuff[i]) && strLiczba[0] != '\0' && j > 2) {
					//wypelnienie tablicy obrazu
					nextDigit = 0;
					//printf("nextNum: %d\n", nextNum);               	
					tmpObraz[nextNum] = atol(strLiczba);
					nextNum++;
					continue;
				}if (isspace(tmpBuff[i])) {
					nextDigit = 0;
					strLiczba[0] = '\0';
					continue;
				}
				else if (isdigit(tmpBuff[i])) {
					strLiczba[nextDigit] = tmpBuff[i];
					strLiczba[nextDigit + 1] = '\0';
					nextDigit++;
				}
				else {
					puts("Blad odczytu");
					break;
				}
				//printf("%s\n", strLiczba);
			}
		}
		j++;
	}
	nextNum = 0;
	for (i = 0; i < img->wys; i++) {
		for (j = 0; j < img->szer; j++) {
			img->obraz[i][j] = tmpObraz[nextNum++];
		}
	}
	fclose(wsk_pliku);
	free(tmpObraz);
	printf("\nPomyslnie odczytano plik %s\n", img->nazwa);
	printf("szerokosc = %d, wys = %d\n", img->szer, img->wys);
	printf("max wartosc = %d\n", img->max_v);
	//return *img;
}

void menu() {
	//char nazwa_pliku[50];
	int wybor = 0, wybor1 = 0, wybor2 = 0, wybor3 = 0, struct_count = 0, i = 0, k = 0;
	size_t ns = 0; // suma wszystkich pikseli z poprzednich do allokacji
	struct s_pgm* tab_struct;
	char* komentarze;
	//short unsigned int* tmp_ptr;
	tab_struct = malloc(sizeof(struct s_pgm) + (MAX_IMG_SIZE * sizeof(short unsigned int)));
	komentarze = malloc(300 * sizeof(char));
	komentarze[0] = '\0';
	//short unsigned int** obraz_ptr;
	do {
		puts("1) Odczytaj obraz PGM");
		puts("2) Zapisz obraz PGM");
		puts("3) Przetwarzanie obrazu");
		puts("4) Wyswietl odczytane komentarze");
		puts("5) Usun z listy obrazow");
		puts("6) Wyjdz");
		while (1) {
			wybor = dajInt();
			if (wybor <= 0 || wybor > 6) {
				printf("Podaj liczbe z zakresu 1 - 6: ");
				continue;
			}
			else {
				break;
			}
		}
		if (wybor == 1) {
			struct_count++;
			if (struct_count > 1) {
				for (k = 0; k < struct_count - 1; k++) {
					ns += (tab_struct[k].szer * tab_struct[k].wys + 1);
				}
				tab_struct = realloc(tab_struct, (struct_count * sizeof(struct s_pgm)) + ((MAX_IMG_SIZE + ns) * sizeof(short unsigned int)));
				//tab_struct = realloc(tab_struct, (struct_count * (sizeof(struct s_pgm) + (MAX_IMG_SIZE * sizeof(short unsigned int))))); //statyczna allokacja obraz
			}
			odczyt_pgm(tab_struct + struct_count - 1, &komentarze);
		}
		if (wybor == 2) {
			if (struct_count == 1) {
				wybor1 = 1;
			}
			else {
				printf("Wybierz obraz do zapisu.\n");
				for (i = 0; i < struct_count; i++) {
					printf("	%d) '%s'\n", i + 1, tab_struct[i].nazwa);
				}
				do {
					wybor1 = dajInt();
					if (wybor1 > struct_count || wybor1 < 0) {
						printf("Podaj liczbe z zakresu 0 do %d: ", struct_count);
						continue;
					}
					else
						break;
				} while (1);
			}
			zapis_pgm((tab_struct + wybor1 - 1));
			wybor = 0;
		}
		if (wybor == 3) {
			if (struct_count == 1) {
				wybor1 = 1;
			}
			else {
				printf("Wybierz obraz do przetworzenia.\n");
				for (i = 0; i < struct_count; i++) {
					printf("	%d) '%s'\n", i + 1, tab_struct[i].nazwa);
				}
				do {
					wybor1 = dajInt();
					if (wybor1 > struct_count || wybor1 < 1) {
						printf("Podaj liczbe z zakresu 1 do %d: ", struct_count);
						continue;
					}
					else
						break;
				} while (1);
			}
			puts("	1) Obrot");
			puts("	2) Szum sol i pieprz");
			puts("	3) Histogram");
			puts("	4) Negatyw");
			puts("	5) Filtr Gaussa");
			puts("	6) Anuluj");
			do {
				wybor2 = dajInt();
				if (wybor2 > 6 || wybor2 < 1) {
					printf("Podaj liczbe z zakresu 1 do 6: ");
					continue;
				}
				else
					break;
			} while (1);
			if (wybor2 == 1) {
				puts("	1) Obrot w prawo o 90 stopni");
				puts("	2) Obrot o 180 stopni");
				puts("	3) Obrot w lewo o 90 stopni");
				do {
					wybor3 = dajInt();
					if (wybor3 > 3 || wybor3 < 1) {
						printf("Podaj liczbe z zakresu 1 do 3: ");
						continue;
					}
					break;
				} while (1);
				for (i = 0; i < wybor3; i++) {
					obrot_p(tab_struct + wybor1 - 1);
				}
				//obrot_p(&tab_struct[wybor1 - 1].szer, &tab_struct[wybor1 - 1].wys, tab_struct[wybor1 - 1].obraz);
			}
			else if (wybor2 == 2) {
				szum_sip(tab_struct[wybor1 - 1].max_v, tab_struct[wybor1 - 1].szer, tab_struct[wybor1 - 1].wys, tab_struct[wybor1 - 1].obraz);
			}
			else if (wybor2 == 3) {
				histogram(tab_struct + wybor1 - 1);
			}
			else if (wybor2 == 4) {
				negatyw(tab_struct[wybor1 - 1].max_v, tab_struct[wybor1 - 1].szer, tab_struct[wybor1 - 1].wys, tab_struct[wybor1 - 1].obraz);
			}
			else if (wybor2 == 5) {
				printf("Podaj ilosc przebiegow: ");
				do {
					wybor3 = dajInt();
					if (wybor3 > 50 && wybor3 < 0) {
						printf("Podaj liczbe z zakresu 1 do 50: ");
						continue;
					}
					break;
				} while (1);
				for (i = 0; i < wybor3; i++) {
					filtr_g(tab_struct + wybor1 - 1);
				}
			}
			else {
				wybor = 0;
				continue;
			}
		}
		if (wybor == 4) {
			printf("\n%s\n", komentarze);
			wybor = 0;
		}
		if (wybor == 5) {
			printf("Wybierz obraz do usuniecia z pamieci podrecznej: \n");
			puts("    0) Anuluj");
			for (i = 0; i < struct_count; i++) {
				printf("    %d) '%s'\n", i + 1, tab_struct[i].nazwa);
			}
			do {
				wybor1 = dajInt();
				if (wybor1 > struct_count&& wybor1 < 0) {
					printf("Podaj liczbe z zakresu 0 do %d", struct_count);
				}
			} while (wybor1 > struct_count&& wybor1 < 0);
			if (wybor1 == 0) {
				continue;
			}
			for (i = 0; i < tab_struct[wybor1 - 1].wys; i++) {
				free(tab_struct[wybor1 - 1].obraz[i]);
			}
			free(tab_struct[wybor1 - 1].obraz);
			for (i = wybor1 - 1; i < struct_count; i++) {
				tab_struct[i] = tab_struct[i + 1];
			}
			struct_count--;
			if (struct_count > 1) {
				for (k = 0; k < struct_count - 1; k++) {
					ns += (tab_struct[k].szer * tab_struct[k].wys + 1);
				}
				tab_struct = realloc(tab_struct, (struct_count * sizeof(struct s_pgm)) + ((MAX_IMG_SIZE + ns) * sizeof(short unsigned int)));
			}
		}
		if (wybor == 6) {
			printf("\nCzy chcesz zakonczyc prace programu?\n");
			if (tak_czy_nie())
				break;
			//wybor = 0;
		}
		wybor = 0;
	} while (wybor != 6);
	free(tab_struct);
	//return;
}

int main() {
	menu();
	printf("\nProgram zakonczyl dzialanie.\n");
	return 0;
}