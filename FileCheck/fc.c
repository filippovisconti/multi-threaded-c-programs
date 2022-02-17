#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define abort(msg)   \
	do               \
	{                \
		printf(msg); \
		exit(1);     \
	} while (0)

/*Si scriva una funzione C con la seguente interfaccia void file_check(char *file_name, int num_threads).
Tale funzione dovrà lanciare num_thread nuovi threads,
in modo che ciascuno di essi legga stringhe dallo standard input,
e per ogni stringa letta verifichi l’occorrenza di tale stringa all’interno di ciascuna riga del file
il cui path è identificato tramite il parametro file_name,
e stampi la stringa su standard output in caso affermativo.*/

typedef struct Parametri
{
	FILE *fp;
	char *s;
} p;

pthread_spinlock_t ptspin;

void *checker(void *param)
{
	p *par = (p *)param;
	char *res = NULL;
	do
	{
		char buf[30];

		pthread_spin_lock(&ptspin);
		res = fgets(buf, 30, par->fp);
		pthread_spin_unlock(&ptspin);

		// buf[strlen(buf)] = "\0";
		if (strcmp(par->s, buf) == 0)
		{
			printf("String found: %s, %s\n", par->s, buf);
			res = NULL;
		}

	} while (res != NULL);
}

void file_check(char *file_name, int num_threads)
{
	printf("file_check invoked. Ctrl + C to abort beforehand\n");

	pthread_spin_init(&ptspin, PTHREAD_PROCESS_PRIVATE);
	int i = 3; // number of times the cycle is repeated
	do
	{
		printf("Insert a string: ");
		char string[30];
		fgets(string, 30, stdin);
		// string[strlen(string)] = "\0";

		FILE *fp = fopen(file_name, "r");
		if (fp == NULL)
			abort("File pointer failed.\n");

		p *args = malloc(sizeof(p)); // preparing parameters for checker();
		args->fp = fp;
		args->s = string;

		pthread_t ptids[num_threads];
		for (int i = 0; i < num_threads; i++)
		{
			pthread_create(ptids + i, NULL, checker, (void *)args);
		}

		for (int i = 0; i < num_threads; i++)
		{
			pthread_join(ptids[i], NULL);
		}

	} while (i--);

	pthread_spin_destroy(&ptspin);
}

void main()
{
	file_check("text.txt", 8);
}
