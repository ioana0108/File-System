#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir
{
	char *name;
	struct Dir *parent;
	struct File *head_children_files;
	struct Dir *head_children_dirs;
	struct Dir *next;
} Dir;

typedef struct File
{
	char *name;
	struct Dir *parent;
	struct File *next;
} File;

void touch(Dir *parent, char *name)
{

	File *fisier; // = NULL

	File *p;
	p = parent->head_children_files;

	int ok = 1;

	if (p != NULL)
	{

		if (p->next != NULL)
		{
			while (p->next != NULL && ok == 1) // am parcurs lista de fisiere ale directorului curent
			{
				if (strcmp(p->name, name) == 0)
				{
					printf("File already exists\n");

					ok = 0;
				}
				if (ok == 1)
					p = p->next; // p se muta de la fisier la fisier pana cand ajunge sa fie ultimul fisier din lista, dupa care trebuie adaugat fisierul in cauza
			}
		}
		else
		{
			if (strcmp(p->name, name) == 0 || strcmp(parent->name, name) == 0)
			{
				ok = 0;
			}
		}

		if (ok == 1)
		{
			fisier = calloc(1, sizeof(File));

			fisier->name = strdup(name); // am atribuit fisierului numele introdus de la tastatura

			p->next = fisier; 

			fisier->parent = parent;

			fisier->next = NULL;
		}
		else
		{
			printf("File already exists\n");
		}
	}
	else
	{
		if (strcmp(parent->name, name) != 0)
		{
			fisier = calloc(1, sizeof(File));

			fisier->name = strdup(name); // am atribuit fisierului numele introdus de la tastatura

			parent->head_children_files = fisier;

			fisier->parent = parent;

			fisier->next = NULL;
		}
		else
		{
			printf("File already exists\n");
		}
	}

	
}

void mkdir(Dir *parent, char *name)
{

	Dir *dir; 
	
	Dir *p;

	int ok = 1;
	if (parent->head_children_dirs != NULL)
	{ // daca directorul curent are copii
		p = parent->head_children_dirs;

		/// trebuie verificat daca nu cumva este doar un singur element in fata elementului de adaugat, caz in care se va verifica doar cu elementul din fata daca are aceeasi denumire si cu parintele, caci nu va intra in while-ul de dupa altfel
		if (p->next != NULL)
		{
			while (p->next != NULL && ok == 1) // am parcurs lista de fisiere ale directorului curent
			{
				if (strcmp(p->name, name) == 0)
				{ // am verificat daca in directoul curent mai exista vreun fisier/director cu acelasi nume ca cel pe care vreau sa-l adaug
					printf("Directory already exists\n");
					ok = 0;
				}
				if (ok == 1)
					p = p->next;
			}
		}
		else
		{ // fiind al doilea director-copil, il compar doar cu primul copil si cu parintele
			if (strcmp(p->name, name) == 0 || strcmp(parent->name, name) == 0)
			{
				ok = 0;
			}
		}

		// acum parent->head_children_files este ultimul element din lista. Dupa el trebuie sa punem elementul/fisierul in cauza

		// daca denumirea deja exista in sistemul de directoare, atunci nu se modifica nimic, altfel se leaga directorul de ultimul director-copil din lista
		if (ok == 1)
		{
			dir = calloc(1, sizeof(Dir));

			dir->name = strdup(name); // am atribuit directorului numele introdus de la tastatura

			p->next = dir; // ???

			dir->parent = parent;

			dir->head_children_dirs = NULL; // nu are copii inca pt ca abia a fost creat

			dir->head_children_files = NULL;

			dir->next = NULL;
		}
		else
		{
			printf("Directory already exists\n");
		}
	}
	else
	{ // directorul curent nu are copii deci ii vom adauga primul director-copil
		if (strcmp(name, parent->name) != 0)
		{
			dir = calloc(1, sizeof(Dir));

			dir->name = strdup(name); // am atribuit directorului numele introdus de la tastatura

			parent->head_children_dirs = dir;

			dir->parent = parent;

			dir->head_children_dirs = NULL; // nu are copii inca pt ca abia a fost creat

			dir->head_children_files = NULL;

			dir->next = NULL;
		}
		else
		{
			printf("Directory already exists\n");
		}
	}
}

void ls(Dir *parent)
{

	Dir *p_dir; // cu el vom parcurge lista de directoare din directorul parametru

	if (parent->head_children_dirs != NULL)
	{

		p_dir = parent->head_children_dirs;

		if (p_dir->next != NULL)
		{

			while (p_dir->next != NULL)
			{

				printf("%s\n", p_dir->name);

				p_dir = p_dir->next;

			}
			printf("%s\n", p_dir->name);
		}
		else
		{
			printf("%s\n", p_dir->name);
		}
	}

	File *p_fis; //calloc

	if (parent->head_children_files != NULL)
	{

		p_fis = parent->head_children_files;

		if (p_fis->next != NULL)
		{
			while (p_fis->next != NULL)
			{

				printf("%s\n", p_fis->name);

				p_fis = p_fis->next;
			}
			// p-fis are acum adresa ultimului fisier
			printf("%s\n", p_fis->name);
		}
		else
		{
			printf("%s\n", p_fis->name);
		}
	}
	//else {
	//	printf("Directorul curent nu are fisiere.\n");
	//}
}

void rm(Dir *parent, char *name)
{

	// Iau un pointer cu care voi parcurge lista de fisiere ale directorului curent
	File *p;

	File *prec;

	// Studiez cazul in care directorul are fisiere
	int sters = 0, am_afisat_deja = 0;
	if (parent->head_children_files != NULL)
	{

		p = parent->head_children_files;

		// Cazul in care fisierul de sters este primul din lista de fisiere-copil
		if (strcmp(p->name, name) == 0)
		{
			if (p->next != NULL)
			{ // daca fisierul de sters este primul dar nu si singurul element din lista, atunci primul element din lista va fi elementul de dupa cel care va fi sters
				sters = 1;
				parent->head_children_files = p->next; // oare mai trebuie si p->next =NULL ??
				free(p->name);
				//free(p->next);
				free(p);
			}
			else
			{ // altfel, daca fisierul de sters este primul si singurul element din lista, atunci lista de fisiere-copil ale directorului comun trebuie sa fie golita
				sters = 1;
				parent->head_children_files = NULL; // pointerul catre primul copil din lista de fisiere nu va mai pointa la nimic
				
				free(p->name);
				//free(p->next);
				free(p);
				free(parent->head_children_files);

			}
		}
		else
		{ // fisierul de sters nu este primul fisier, deci p va primi adresa urmatorului element. De asemenea, nefiind primul element, trebuie sa retin cumva adresa elementului de dinaintea lui p ca sa-i fac legatura cu elementul de dupa p
			if (p->next == NULL)
			{
				am_afisat_deja = 1;
				printf("Could not find the file\n");
			}
			else
			{
				prec = p; // nu am unde altundeva sa-l initializez pe prec
				p = p->next;
			}
		}

		// Cazul in care fisierul de sters nu este singurul fisier din director si nici nu este primul.

		if (sters == 0 && am_afisat_deja == 0)
		{

			if (p->next != NULL && sters == 0)
			{

				// parcurg lista de fisiere ale directorului pana gasesc fisierul de sters (cat timp sters = 0)
				while (p->next != NULL && sters == 0)
				{
					if (strcmp(p->name, name) == 0)
					{ // daca am gasit fisierul
						sters = 1;
						prec->next = p->next; 
						free(p->name);
						free(p);
					}
					else
					{			  // daca nu l-am gasit, p trece la urmatorul element din lista
						prec = p; // actualizez valoarea lui prec
						p = p->next;
					}
				}
				// Daca fisierul de sters este ultimul din lista, atunci la finalul while-ului, p va fi adresa ultimului element din lista
				if (sters == 0 && strcmp(p->name, name) == 0)
				{
					sters = 1;
					prec->next = NULL;

					free(prec->next);
					free(p->name);
					free(p);
				}
			}
			else
			{ // fisierul este al doilea si ultimul fisier, deci nu va intra in while-ul de deasupra, caci p->next == NULL
				if (sters == 0 && strcmp(p->name, name) == 0)
				{
					sters = 1;
					prec->next = NULL;

					free(prec->next);
					free(p->name);

					free(p);
				}
				else
				{
					am_afisat_deja = 1;
					printf("Could not find the file\n"); 
				}
			}
		}

		if (sters == 0 && am_afisat_deja == 0)
		{
			printf("Could not find the file\n");
		}
	}
	else
	{ // Daca directorul curent nu are niciun fisier-copil
		printf("Could not find the file\n");
	}
}

void rmdir(Dir *parent, char *name)
{

	// Iau un pointer cu care voi parcurge lista de fisiere ale directorului curent
	Dir *p;
	Dir *prec;

	File *f;
	// Studiez cazul in care directorul are fisiere
	int sters = 0, am_afisat_deja = 0;
	if (parent->head_children_dirs != NULL) 
	{

		p = parent->head_children_dirs;

		// Cazul in care fisierul de sters este primul din lista de fisiere-copil
		if (strcmp(p->name, name) == 0)
		{
			if (p->next != NULL)
			{ // daca fisierul de sters este primul dar nu si singurul element din lista, atunci primul element din lista va fi elementul de dupa cel care va fi sters
				sters = 1;
				parent->head_children_dirs = p->next; // oare mai trebuie si p->next =NULL ??

				free(p->name);
				free(p->head_children_dirs);
				free(p->head_children_files);
				free(p);
			}
			else
			{ // altfel, daca fisierul de sters este primul si singurul element din lista, atunci lista de fisiere-copil ale directorului comun trebuie sa fie golita
				sters = 1;
				parent->head_children_dirs = NULL; // pointerul catre primul copil din lista de fisiere nu va mai pointa la nimic
				
				free(p->name);
				free(p->head_children_dirs);
				free(p->head_children_files);
				free(p);
			}
		}
		else
		{ // fisierul de sters nu este primul fisier, deci p va primi adresa urmatorului element. De asemenea, nefiind primul element, trebuie sa retin cumva adresa elementului de dinaintea lui p ca sa-i fac legatura cu elementul de dupa p
			if (p->next == NULL)
			{
				am_afisat_deja = 1;
				printf("Could not find the dir\n");
			}
			else
			{
				prec = p; 
				p = p->next;
			}
		}

		// Cazul in care fisierul de sters nu este singurul fisier din director si nici nu este primul.

		if (sters == 0 && am_afisat_deja == 0)
		{

			if (p->next != NULL && sters == 0)
			{

				// parcurg lista de fisiere ale directorului pana gasesc fisierul de sters (cat timp sters = 0)
				while (p->next != NULL && sters == 0)
				{
					if (strcmp(p->name, name) == 0)
					{ // daca am gasit fisierul
						sters = 1;
						prec->next = p->next; 

						free(p->name);
						free(p->head_children_dirs);
						free(p->head_children_files);
						free(p);
					}
					else
					{			  // daca nu l-am gasit, p trece la urmatorul element din lista
						prec = p; // actualizez valoarea lui prec
						p = p->next;
					}
				}
				// Daca fisierul de sters este ultimul din lista, atunci la finalul while-ului, p va fi adresa ultimului element din lista
				if (sters == 0 && strcmp(p->name, name) == 0)
				{
					sters = 1;
					prec->next = NULL;

					free(p->name);
					free(p->head_children_dirs);
					free(p->head_children_files);
					free(p);
				}
			}
			else
			{ // fisierul este al doilea si ultimul fisier, deci nu va intra in while-ul de deasupra, caci p->next == NULL
				if (sters == 0 && strcmp(p->name, name) == 0)
				{
					sters = 1;
					prec->next = NULL;

					free(p->name);
					free(p->head_children_dirs);
					free(p->head_children_files);
					free(p);
				}
				else
				{
					am_afisat_deja = 1;
					printf("Could not find the dir\n"); // daca intra in if-ul asta inseamna ca erau doar 2 fisiere in lista. primul nu era cel de sters, nici al doilea nu era, deci fisierul nu exista
				}
			}
		}

		if (sters == 0 && am_afisat_deja == 0)
		{
			printf("Could not find the dir\n");
		}
	}
	else
	{ // Daca directorul curent nu are niciun fisier-copil
		printf("Could not find the dir\n");
	}
}

void cd(Dir **target, char *name)
{
	// Iau un p de tip Dir cu care voi parcurge directoarele-copil ale directorului curent
	Dir *p;

	int gasit = 0;

	if ((*target)->parent != NULL) // cazul in care se primeste ".."
	{
		if (strcmp(name, (*target)->parent->name) == 0)
		{
			gasit = 1;
			(*target) = (*target)->parent;
		}
	}

	if (gasit == 0)
	{

		if ((*target)->head_children_dirs != NULL)
		{
			p = (*target)->head_children_dirs;
			if (p->next != NULL)
			{ // nu e un singur director in acest director
				while (p->next != NULL && gasit == 0)
				{
					if (strcmp(p->name, name) == 0)
					{
						gasit = 1;
						(*target) = p;
					}
					else
					{
						p = p->next;
					}
					
				} // pe ultimul nu-l baga in seama

				if (strcmp(p->name, name) == 0)
				{
					gasit = 1;
					(*target) = p;
				}

				if (gasit == 0 && strcmp(p->name, name) == 0)
				{ //ultimul director nu a fost verificat
					(*target) = p;
				}
			}
			else
			{ // este singurul director din directorul curent
				if (strcmp(p->name, name) == 0)
				{
					gasit = 1;
					(*target) = p;
				}
			}

			if (gasit == 0)
			{
				printf("No directories found!\n");
			}
		}
		else
		{
			printf("No directories found!\n");
		}
	}
}

char *invertString(char *string)
{
	char *copy;
	copy = calloc(MAX_INPUT_LINE_SIZE, sizeof(char));
	strcpy(copy, string);

	int i;
	int length = strlen(copy);
	for(i = length - 1; i >= 0; i--)
	{
		*(string + (length - 1) - i ) = *(copy + i) ;
	}

	return string;

}

char *pwd(Dir *target) 
{
	/* 

	Dir *p;

	p = target;


	if(p->parent != NULL)
	{

		pwd(p->parent);

		printf("/%s", p->name);
	
	}
	
	if(p->parent == NULL)
	{
		printf("/%s", p->name);
	}

	Ar fi fost foarte frumos rezolvat recursiv, ca mai sus in comentariu, dar fie:  */


	Dir *p;

	p = target;

	char *s;
	s = calloc(MAX_INPUT_LINE_SIZE, sizeof(char));

	strcat(s, p->name);
	invertString(s);
	strcat(s, "/");

	if(p->parent != NULL)
	{
		while(p->parent != NULL)
		{
			p = p->parent;
			invertString(p->name);
			strcat(s, p->name);
			strcat(s, "/");
		}
		
	} // la final p = home

	invertString(s);

	return s;
}

void stop(Dir *target)
{
	
	while(target->head_children_dirs != NULL)
	{
		rmdir(target, target->head_children_dirs->name);

	}

	while(target->head_children_files != NULL)
	{
		rm(target, target->head_children_files->name);

	}

	free(target->name);
	free(target);

}

void tree(Dir *target, int level)
{
		
	Dir *p;

	Dir *dir;

	File *fis;

	char spatii[MAX_INPUT_LINE_SIZE] = "";
	char spatiu[MAX_INPUT_LINE_SIZE] = "    ";


	if(target->head_children_dirs != NULL)
	{
		p = target->head_children_dirs;
	}
	else
	{
		p = target;
	}


	dir = p;

	int i;
	for(i = 1; i <= level; i++) 
	{
		strcat(spatii, spatiu);
	}


	while(dir->next != NULL)
	{
	
		printf("%s", spatii);
		
		printf("%s\n", dir->name);


		if(dir->head_children_dirs != NULL)
		{
			tree(dir, level + 1);
		}
		

		dir = dir->next;

	}

	if(dir != NULL && dir != target) 
	{
		printf("%s", spatii);
		printf("%s\n", dir->name); 
	}
	

	if(dir->head_children_dirs != NULL)
	{
		tree(dir, level + 1);
	}
	else // nu mai are copii directoare, deci poate are fisiere
	{
		if(dir->head_children_files != NULL)
		{
			if(dir != target)
			{
				fis = dir->parent->head_children_files;
			}
			else
			{
				fis = dir->head_children_files;
			}

			while(fis->next != NULL)
			{
				printf("%s", spatii);
				printf("%s\n", fis->name);

				fis = fis->next;
			}
			printf("%s", spatii);
			printf("%s\n", fis->name);

		}
		
	}
	
	if(dir != target)
	{

		if(dir->parent->head_children_files != NULL) // ahaaaaa dir->parent e NUUUULLLLLLL
		{

			fis = dir->parent->head_children_files;

			while(fis->next != NULL)
			{

				printf("%s", spatii);
				printf("%s\n", fis->name);

				fis = fis->next;
			}// pe ultimul nu-l afis

			printf("%s", spatii);
			printf("%s\n", fis->name);

		}

	}
	
}

void mv(Dir *parent, char *oldname, char *newname) 
{
	// Iau un p de tip Dir cu care voi parcurge directoarele-copil ale directorului curent
	Dir *dir;
	dir = calloc(1, sizeof(Dir)); 

	File *fis;
	fis = calloc(1, sizeof(File));

	int gasit = 0;

	int folosit = 0;

	if (parent->head_children_dirs != NULL || parent->head_children_files != NULL)
	{
		
		if(parent->head_children_dirs != NULL)
		{
			dir = parent->head_children_dirs;
		}

		if(parent->head_children_files != NULL)
		{
			fis = parent->head_children_files;
		}
		
		if(dir->next != NULL)
		{
			while(dir->next != NULL && gasit == 0 && folosit == 0) //modif
			{
				if(strcmp(dir->name, newname) == 0)
				{
					folosit = 1; //modif

				}
				else
				{
					dir = dir->next;
				}

			}
			if(folosit == 0 && strcmp(dir->name, newname) == 0) 
			{
				folosit = 1;
			}

		}

		if(parent->head_children_dirs != NULL)
		{
			dir = parent->head_children_dirs;
		}

		if(fis->next != NULL)
		{
			while(fis->next != NULL && gasit == 0 && folosit == 0) 
			{
				if(strcmp(fis->name, newname) == 0)
				{
					folosit = 1; 

				}
				else
				{
					fis = fis->next;
				}

			}
			if(folosit == 0 && strcmp(fis->name, newname) == 0) 
			{
				folosit = 1;
			}

		}
		
		if(parent->head_children_files != NULL)
		{
			fis = parent->head_children_files;
		}


		if (dir->next != NULL)
		{ // nu e un singur director in acest director

			while (dir->next != NULL && gasit == 0 && folosit == 0)
			{
				if (strcmp(dir->name, oldname) == 0 && folosit == 0)
				{
					gasit = 1;

					rmdir(parent, dir->name);

					mkdir(parent, newname);

				}
				else
				{
					dir = dir->next;
				}
				
				
			} // pe ultimul nu-l baga in seama

			if(gasit == 0 && folosit == 0)
			{
				if (strcmp(dir->name, oldname) == 0)
				{
					gasit = 1;

					rmdir(parent, dir->name);

					mkdir(parent, newname);
				}
			}

		}
		else
		{ // este singurul director din directorul curent
			if(dir->name != NULL)
			{
				if(gasit == 0 && dir->name != NULL && folosit == 0)
				{
					if (strcmp(dir->name, oldname) == 0 && strcmp(dir->name, newname) != 0)
					{
						gasit = 1;

						rmdir(parent, dir->name);

						mkdir(parent, newname);

					}

				}
				

			}
			
		}


		if (fis->next != NULL)
		{ // nu e un singur director in acest director

			while (fis->next != NULL && gasit == 0 && folosit == 0) 
			{
				if (strcmp(fis->name, oldname) == 0 && folosit == 0) 
				{
					gasit = 1;

					rm(parent, fis->name);

					touch(parent, newname);

				}
				else
				{
					fis = fis->next;
				}
				
			} // pe ultimul nu-l baga in seama

			if(gasit == 0 && folosit == 0) //modif
			{

				if (strcmp(fis->name, oldname) == 0)
				{
					gasit = 1;

					rm(parent, fis->name);

					touch(parent, newname);

				}

			}
			
		}
		else
		{ // este singurul director din directorul curent
			if(fis->name != NULL)
			{
				if(gasit == 0 && fis->name != NULL && folosit == 0) 
				{

					if (strcmp(fis->name, oldname) == 0 && strcmp(fis->name, newname) != 0) 
					{
						gasit = 1;

						rm(parent, fis->name);

						touch(parent, newname);

					}

				}
			}

		}

		if(folosit == 1)
		{
			printf("File/Director already exists\n");
		}
		else
		{
			if (gasit == 0)
			{
				printf("File/Director not found\n");
			}

		}
		
	}
	else
	{
		printf("File/Director not found\n");
	} 
	
}

int main()
{
	Dir *radacina;
	radacina = calloc(1, sizeof(Dir));

	// Am creat primul director din sistem - radacina
	radacina->name = strdup("home");
	radacina->parent = NULL;
	radacina->head_children_dirs = NULL;
	radacina->head_children_files = NULL;
	radacina->next = NULL;

	char *input;
	input = calloc(MAX_INPUT_LINE_SIZE, sizeof(char));

	char *p;
	char *oldname;
	char *newname;

	Dir *dir_crt;
	dir_crt = radacina;


	int ok = 1;
	int nivel = 0;

	do
	{
		/*
		Summary:
			Reads from stdin a string and breaks it down into command and in
			case it needs into a name.
		*/

		fgets(input, MAX_INPUT_LINE_SIZE, stdin); // am primit inputul format din minim 2 cuvinte

		p = strtok(input, " "); // p pointeaza catre primul cuvant din input

		/// in functie de comanda primita, accesata prin p, se va apela functia corespunzatoare
		if (strcmp(p, "touch") == 0)
		{
			p = strtok(NULL, "\n");

			touch(dir_crt, p);
		}

		if (strcmp(p, "mkdir") == 0)
		{
			p = strtok(NULL, "\n");

			mkdir(dir_crt, p);
		}

		if (strcmp(p, "ls\n") == 0)
		{
			ls(dir_crt);
		}

		if (strcmp(p, "rm") == 0)
		{
			p = strtok(NULL, "\n");

			rm(dir_crt, p);

		}

		if (strcmp(p, "rmdir") == 0)
		{
			p = strtok(NULL, "\n");

			rmdir(dir_crt, p);

		}

		if (strcmp(p, "cd") == 0)
		{
			p = strtok(NULL, "\n");
			if (strcmp(p, "..") == 0)
			{ // daca in p are ".."
				if (dir_crt->parent != NULL)
				{
					cd(&dir_crt, dir_crt->parent->name);
				}
			}
			else
			{
				cd(&dir_crt, p);
			}
		}

		

		if (strcmp(p, "tree\n") == 0)
		{

			tree(dir_crt, nivel);

		}

		if (strcmp(p, "mv") == 0)
		{
			p = strtok(NULL, " ");

			oldname = p;

			p = strtok(NULL, "\n");

			newname = p;

			mv(dir_crt, oldname, newname);

		}

		if (strcmp(p, "pwd\n") == 0)
		{
			char *s;
			s = pwd(dir_crt);
			printf("%s\n", s);

			free(s);

		}

		if (strcmp(p, "stop\n") == 0)
		{
			stop(radacina);

			ok = 0;

		}

	} while (ok != 0);

	free(input);
}