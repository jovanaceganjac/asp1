#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct mina {
	int x;
	int y;
	double r;
	int broj;
	struct mina* next;
} mina;

typedef struct graf {
	mina** lista;
	int n;
} graf;

double rastojanje(mina* m1, mina* m2) {
	return sqrt(pow(m1->x - m2->x, 2) + pow(m1->y - m2->y, 2));
}

void ispis(graf* G) {
	for (int i = 0; i < G->n; i++) {
		for (mina* tek = G->lista[i]; tek; tek = tek->next) {
			if (tek->next) {
				printf("%d->", tek->broj);
			}
			else {
				printf("%d\n", tek->broj);
			}
		}
	}
}

void dodajGranu(graf* G, int i, int j) {
	mina* tek;
	for (tek = G->lista[i]; tek->next; tek = tek->next);
	mina* m = malloc(sizeof(mina));
	m->x = G->lista[j]->x;
	m->y = G->lista[j]->y;
	m->r = G->lista[j]->r;
	m->broj = G->lista[j]->broj;
	m->next = NULL;
	tek->next = m;
}

void ukloniGranu(graf* G, int i, int j) {
	mina* tek, * prev;
	for (tek = G->lista[i], prev = NULL; tek && tek->broj != G->lista[j]->broj; prev = tek, tek = tek->next);
	if (!tek || !prev) return;
	prev->next = tek->next;
	free(tek);
}

void dodajCvor(graf* G, int x, int y, double r) {
	mina* m = malloc(sizeof(mina));
	m->broj = G->n++;
	m->x = x;
	m->y = y;
	m->r = r;
	m->next = NULL;
	G->lista = realloc(G->lista, G->n * sizeof(mina*));
	G->lista[G->n - 1] = m;
}

void ukloniCvor(graf* G, int broj) {
	for (int i = 0; i < G->n; i++) {
		ukloniGranu(G, i, broj);
	}

	for (int i = 0; i < G->n; i++) {
		for (mina* tek = G->lista[i]; tek; tek = tek->next) {
			if (tek->broj > broj) tek->broj--;
		}
	}

	for (int i = broj; i < G->n - 1; i++) {
		G->lista[i] = G->lista[i + 1];
	}
	G->n--;
	G->lista = realloc(G->lista, G->n * sizeof(mina*));

}

void efikasnost(graf* G, int br) {
	int* visit = calloc(G->n, sizeof(int));
	int* stack = calloc(G->n, sizeof(int));
	int top = -1;
	int ukupno = 1;

	stack[++top] = br;
	visit[br] = 1;
	printf("Eksplodirale su mine: ");
	while (top >= 0) {
		int temp = stack[top--];
		printf("%d ", temp);
		for (mina* tek = G->lista[temp]->next; tek; tek = tek->next) {
			if (!visit[tek->broj]) {
				ukupno++;
				visit[tek->broj] = 1;
				stack[++top] = tek->broj;
			}
		}
	}
	printf("\nUkupan broj ekslodiranih mina: %d\n", ukupno);
	free(stack);
	free(visit);
}

void najvecaEfikasnost(graf* G) {
	int max = 0, m;
	for (int i = 0; i < G->n; i++) {
		int* visit = calloc(G->n, sizeof(int));
		int* stack = calloc(G->n, sizeof(int));
		int top = -1;
		int ukupno = 1;
		stack[++top] = i;
		visit[i] = 1;
		while (top >= 0) {
			int temp = stack[top--];
			for (mina* tek = G->lista[temp]->next; tek; tek = tek->next) {
				if (!visit[tek->broj]) {
					ukupno++;
					visit[tek->broj] = 1;
					stack[++top] = tek->broj;
				}
			}
		}	
		if (ukupno > max) {
			m = i;
			max = ukupno;
		}
		free(stack);
		free(visit);
	}

	printf("Mina najvece efikasnosti je %d i njena efikasnost je %d\n", m, max);

}

void eksplozijaRakete(graf* G, mina raketa) {
	int* niz = calloc(G->n, sizeof(int));
	for (int i = 0; i < G->n; i++) {
		if (rastojanje(&raketa, G->lista[i]) <= raketa.r) {
			int* visit = calloc(G->n, sizeof(int));
			int* stack = calloc(G->n, sizeof(int));
			int top = -1;
			stack[++top] = i;
			visit[i] = 1;
			niz[i]++;
			while (top >= 0) {
				int temp = stack[top--];
				niz[temp]++;
				for (mina* tek = G->lista[temp]->next; tek; tek = tek->next) {
					if (!visit[tek->broj]) {
						visit[tek->broj] = 1;
						stack[++top] = tek->broj;
					}
				}
			}
			free(stack);
			free(visit);
		}
	}

	printf("Eksplodirale su mine: ");
	for (int i = 0; i < G->n; i++) {
		if (niz[i]) {
			printf("%d ", i);
		}
	}
	printf("\n");

}


double povrsinaPreseka(mina* m1, mina* m2) {
	double d = sqrt((m1->x - m2->x) * (m1->x - m2->x) + (m1->y - m2->y) * (m1->y - m2->y));
	double r1 = m1->r;
	double r2 = m2->r;

	if (d >= r1 + r2) {
		return 0;
	}

	if (d <= fabs(r1 - r2)) {
		if (r1 < r2)  return 3.14 * r1 * r1;
		else return 3.14 * r2 * r2;
	}

	return (r1 * r1 * acos((d * d + r1 * r1 - r2 * r2) / (2 * d * r1))) +
		(r2 * r2 * acos((d * d + r2 * r2 - r1 * r1) / (2 * d * r2))) -
		(0.5 * sqrt((-d + r1 + r2) * (d + r1 - r2) * (d - r1 + r2) * (d + r1 + r2)));
}


void povrsinaEksplozije(graf* G) {
	int max = 0, m;
	for (int i = 0; i < G->n; i++) {
		int* visit = calloc(G->n, sizeof(int));
		int* stack = calloc(G->n, sizeof(int));
		int top = -1;
		int ukupno = 1;
		stack[++top] = i;
		visit[i] = 1;
		while (top >= 0) {
			int temp = stack[top--];
			for (mina* tek = G->lista[temp]->next; tek; tek = tek->next) {
				if (!visit[tek->broj]) {
					ukupno++;
					visit[tek->broj] = 1;
					stack[++top] = tek->broj;
				}
			}
		}
		if (ukupno > max) {
			m = i;
			max = ukupno;
		}
		free(stack);
		free(visit);
	}


	int* eksplodirale = calloc(G->n, sizeof(int));

	do {
		int* visit = calloc(G->n, sizeof(int));
		int* stack = calloc(G->n, sizeof(int));
		int top = -1;
		int ukupno = 1;

		stack[++top] = m;
		visit[m] = 1;
		while (top >= 0) {
			int temp = stack[top--];
			eksplodirale[temp]++;
			for (mina* tek = G->lista[temp]->next; tek; tek = tek->next) {
				if (!visit[tek->broj]) {
					ukupno++;
					visit[tek->broj] = 1;
					stack[++top] = tek->broj;
				}
			}
		}
		free(stack);
		free(visit);
	} while (0);

	double povrsina = 3.14 * G->lista[m]->r * G->lista[m]->r;

	do {
		int* visit = calloc(G->n, sizeof(int));
		int* stack = calloc(G->n, sizeof(int));
		int top = -1;

		stack[++top] = m;
		visit[m] = 1;
		while (top >= 0) {
			int temp = stack[top--];
			for (mina* tek = G->lista[temp]->next; tek; tek = tek->next) {
				if (!visit[tek->broj]) {
					povrsina += 3.14 * G->lista[tek->broj]->r * G->lista[tek->broj]->r - povrsinaPreseka(G->lista[temp], tek);
					visit[tek->broj] = 1;
					stack[++top] = tek->broj;
				}
			}
		}
		free(stack);
		free(visit);
	} while (0);

	printf("Ukupuna povrsina eksplozije je %lf\n", povrsina);

}


int main() {
	int p, k;
	int x, y;
	double r;
	graf* G = NULL;
	FILE* in = fopen("graf2.txt", "r");
	mina raketa;

	while (1) {
		printf("1. Formiranje grafa\n");
		printf("2. Dodavanje cvora u graf\n");
		printf("3. Uklanjanje cvora iz grafa\n");
		printf("4. Dodavanje grane u graf\n");
		printf("5. Uklanjanje grane iz grafa\n");
		printf("6. Ispis reprezentacije grafa\n");
		printf("7. Efikasnost eksplozije mine\n");
		printf("8. Pronalazenje mine najvece epslozivne efikasnosti\n");
		printf("9. Raketa\n");
		printf("10. Povrsina eksplozije mine najvece epslozivne efikasnosti\n");
		printf("11. Brisanje grafa\n");
		printf("0. Kraj\n");
		scanf("%d", &p);

		switch (p)
		{
		case 1:
			G = malloc(sizeof(graf));
			fscanf(in, "%d\n", &G->n);
			G->lista = malloc(G->n * sizeof(mina*));
			for (int i = 0; i < G->n; i++) {
				mina* m = malloc(sizeof(mina));
				fscanf(in, "%d %d %lf\n", &m->x, &m->y, &m->r);
				m->broj = i;
				m->next = NULL;
				G->lista[i] = m;
			}

			for (int i = 0; i < G->n; i++) {
				for (int j = 0; j < G->n; j++) {
					if (i != j && rastojanje(G->lista[i], G->lista[j]) <= G->lista[i]->r) {
						mina* m = malloc(sizeof(mina));
						m->x = G->lista[j]->x;
						m->y = G->lista[j]->y;
						m->r = G->lista[j]->r;
						m->broj = G->lista[j]->broj;
						m->next = NULL;
						mina* tek;
						for (tek = G->lista[i]; tek->next; tek = tek->next);
						tek->next = m;
					}
				}
			}
			break;
		case 2:
			printf("Unesite koordinate i poluprecnik nove mine\n");
			scanf("%d %d %lf", &x, &y, &r);
			dodajCvor(G, x, y, r);
			break;
		case 3:
			printf("Unesite redni broj mine koju zelite da izbacite\n");
			scanf("%d", &x);
			ukloniCvor(G, x);
			break;
		case 4:
			printf("Unesite redne brojeve cvorova izmedju kojih ocete da dodate novu granu\n");
			scanf("%d %d", &p, &k);
			dodajGranu(G, p, k);
			break;
		case 5:
			printf("Unesite redne brojeve cvorova izmedju kojih ocete da uklonite granu\n");
			scanf("%d %d", &p, &k);
			ukloniGranu(G, p, k);
			break;
		case 6:
			ispis(G);
			break;
		case 7:
			printf("Unesite broj mine\n");
			scanf("%d", &k);
			efikasnost(G, k);
			break;
		case 8:
			najvecaEfikasnost(G);
			break;
		case 9:
			printf("Unesite koordinate i radius rakete\n");
			scanf("%d %d %lf", &raketa.x, &raketa.y, &raketa.r);
			eksplozijaRakete(G, raketa);
			break;
		case 10:
			povrsinaEksplozije(G);
			//printf("Povrsina preseka %lf\n", povrsinaPreseka(*(G->lista[0]), *(G->lista[1])));
			break;
		case 11:
			for (int i = 0; i < G->n; i++) {
				mina* tek = G->lista[i];
				while (tek) {
					mina* old = tek;
					tek = tek->next;
					free(old);
				}
			}
			free(G->lista);
			free(G);
			break;
		default:
			return 0;
			break;
		}

	}

}