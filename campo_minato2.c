#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

int tab[9][9];		// Contiene informazioni sul contenuto delle caselle
int scop[9][9];		// Contiene informazioni su quali caselle sono state scoperte
int band[9][9];		// Contiene informazioni sulle bandierine poste dall'utente
int part=0;			// Segnala l'inizio della prima partita
int quadscop[71];	// Contiene le posizioni degli 0 scoperti in una sequenza innestata da una casella contenente 0
int vittoria=0;		// Conteggia i quadrati scoperti dall'utente per segnalare un'eventuale vittoria

void istr(void);
void menu(void);
void ins_coord(void);
void scopri_quad(int,int);
int band_mod(int);
void view(void);
void viewend(void);
int rdcoord(int*,int*);
void reset(int[9][9]);
void gen_mine(int[9][9]);
void comp_mat(int[9][9]);

int main(void)
{
	printf("\nDigita \'m\'/\'M\' + Invio in qualunque momento della partita per tornare al menù di gioco.\n");
	menu();
	
	return 0;
}

/* istr:
Stampa a video le istruzioni di gioco.*/
void istr(void)
{
	int x=0,y=0;
	
	printf("\nBenvenuto in Campo Minato, il gioco scritto per te in C! ");
	getchar();
	printf("\nIstruzioni di gioco:");
	printf("\nTi verrà presentata una tabella quadrata 9x9 che rappresenta un campo minato. ");
	getchar();
	printf("Ogni casella coperta è segnalata da un asterisco (*). ");
	getchar();
	printf("Sotto a 10 caselle tra le 81 totali è nascosta una mina che, esplodendo, fa terminare la partita. ");
	getchar();
	printf("L'obiettivo del gioco è scoprire tutte le caselle sotto alle quali non si nasconde alcuna mina. ");
	getchar();
	printf("Il giocatore deve scegliere quali caselle scoprire, digitando le coordinate (numeri interi da 1 a 9) nel formato (x.y). ");
	getchar();
	printf("Facciamo un esempio: supponiamo di voler scoprire la casella (3.4). ");
	getchar();
	printf("\n");
	
	for(int i=0;i<9;i++)
	{
		printf("%d\t",i);
		for(int j=0;j<9;j++)
			printf("*\t");
		printf("\n");
	}
	printf("\n/\t1\t2\t3\t4\t5\t6\t7\t8\t9\n\n");
	
	printf("In questo caso è necessario digitare il contenuto della parentesi: (3.4).\nProva: ");
	while( rdcoord(&x,&y)!=1 || x!=3 || y!=4 )
		printf("Così non va bene. Riprova: ");
	
	printf("\n");
	for(int i=0;i<9;i++)
	{
		printf("%d\t",i+1);
		for(int j=0;j<9;j++)
			if( i==2 && j==3 )
				printf("0\t");
			else
				printf("*\t");
		printf("\n");
	}
	printf("\n/\t1\t2\t3\t4\t5\t6\t7\t8\t9\n\n");
	
	printf("Perfetto! Ecco come appare la tabella dopo aver scoperto la casella (3.4). ");
	getchar();
	printf("\nUna volta scoperta una casella, vi sono tre possibilità: ");
	getchar();
	printf("1. Sotto la casella c'è una mina: in tal caso il gioco termina. ");
	getchar();
	printf("2. Sotto la casella non c'è una mina, ma nelle caselle circostanti (ovvero quelle che si trovano nel quadrato 3x3 il cui centro è costituito dalla casella stessa) vi sono una o più mine: in tal caso, la casella viene scoperta e mostra un numero corrispondente a quello delle mine situate nelle caselle adiacenti. ");
	getchar();
	printf("3. Sotto la casella non c'è una mina e nemmeno nelle caselle circostanti: in tal caso, viene scoperto l'intero quadrato 3x3 il cui centro è costituito dalla casella stessa. Nel caso in cui anche una o più delle caselle adiacenti sia tale da essere isolata da mine, il processo viene ripetuto per ognuna di esse. ");
	getchar();
	printf("Facciamo un esempio: supponiamo che nelle vicinanze della casella scoperta precedentemente vi siano due mine. ");
	getchar();
	
	printf("\n");
	for(int i=0;i<9;i++)
	{
		printf("%d\t",i+1);
		for(int j=0;j<9;j++)
			if( i==2 && j==3 )
				printf("2\t");
			else
				printf("*\t");
		printf("\n");
	}
	printf("\n/\t1\t2\t3\t4\t5\t6\t7\t8\t9\n\n");
	
	printf("In questo caso è comparso un 2 in posizione (3.4). Infatti, se scopriamo le caselle circostanti... ");
	getchar();
	
	printf("\n");
	for(int i=0;i<9;i++)
	{
		printf("%d\t",i+1);
		for(int j=0;j<9;j++)
			if( i==2 && j==3 )
				printf("2\t");
			else if( (i==2 && j==2) || (i==1 && j==4) )
				printf("M\t");
			else if( i>=1 && i<=3 && j>=2 && j<=4 )
				printf("0\t");
			else
				printf("*\t");
		printf("\n");
	}
	printf("\n/\t1\t2\t3\t4\t5\t6\t7\t8\t9\n\n");
	
	printf("... ecco apparire 2 mine, segnalate dalla lettera M. ");
	getchar();
	printf("Vi è inoltre la possibilità di porre una bandierina (segnalata dalla lettera B) su una casella coperta nella quale si suppone vi sia una mina. ");
	getchar();
	printf("Infine è sempre possibile, in qualunque momento della partita, accedere al menù di gioco digitando, durante l'inserimento delle coordinate, la lettera \'m\' o \'M\'. ");
	getchar();
	printf("\nLe istruzioni finiscono qui. Buon divertimento! ");
	getchar();
	
	return;
}

/* menu:
Stampa a video il menù di gioco.*/
void menu(void)
{
	char scelta = '\0';
	
	while(scelta != '5' )
	{
		printf("\nMenù di gioco:\n");
		printf("0. Istruzioni di gioco.\n");
		printf("1. Inizia nuova partita.\n");
		printf("2. Riprendi partita.\n");
		printf("3. Aggiungi bandierine.\n");
		printf("4. Rimuovi bandierine.\n");
		printf("5. Esci.\n> ");

		scelta = getchar();
		getchar();
		
		switch(scelta)
		{
			case '0': istr();
			break;
			case '1': 	vittoria=0;
						part=1;
						reset(tab);
						reset(scop);
						reset(band);
						reset(tab);
						reset(scop);
						reset(band);
						gen_mine(tab);
						comp_mat(tab);
						ins_coord();
			break;
			case '2':	if( part==0 )
							printf("Nessuna partita in corso.");
						else
							ins_coord();
			break;
			case '3':	if( part==0 )
							printf("Nessuna partita in corso.");
						else
						{
							if( band_mod(1)!=-1 )
								ins_coord();
						}
			break;
			case '4':	if( part==0 )
							printf("Nessuna partita in corso.");
						else
						{
							if( band_mod(0)!=-1 )
								ins_coord();
						}
			break;
			case '5':
			break;
			default: printf("Opzione non disponibile.");
			break;
		}
		
		printf("\n");
	}
	
	//return;
}

/* ins_coord:
Chiede all'utente di inserire le coordinate della casella da scoprire.*/
void ins_coord(void)
{
	int fine=1,x,y,test;
	
	while( fine==1 )
	{
		printf("\n");
		view();
		printf("\n");
		
		if( vittoria==71 )
		{
			printf("\nCOMPLIMENTI! HAI VINTO! ");
			getchar();
			printf("\n\n\n");
			viewend();
			fine=0;
		}
		else
		{
			printf("Inserisci le coordinate della casella da scoprire (x.y): ");
			while( (test=rdcoord(&x,&y))==0 )
				printf("Errore nell'inserimento delle coordinate (x.y). Riprova: ");
			if( test==-1 )
				fine=0;
			else
			{
				if( scop[x-1][y-1]==1 )
					printf("\nLa casella è già scoperta.\n");
				else if( tab[x-1][y-1]==-1 )
				{
					printf("\nBOOM! Hai fatto esplodere una mina!\n\nGAME OVER ");
					getchar();
					printf("\n\n\n");
					viewend();
					fine=0;
				}
				else if( tab[x-1][y-1]==0 )
				{
					for(int i=0;i<71;i++)
						quadscop[i]=0;
					
					scopri_quad(x-1,y-1);
				}
				else
				{
					scop[x-1][y-1]=1;
					vittoria++;
				}
			}
		}
	}
	
	return;
}

/* scopri_quad:
Scopre la serie di quadrati collegati allo 0, come descritto nelle istruzioni di gioco.*/
void scopri_quad(int x,int y)
{
	int n;
	for(n=0;quadscop[n]!=0;n++);
	quadscop[n]=(10*(x+1))+(y+1);
	
	for(int i=x-1;i<=x+1;i++)
		for(int j=y-1;j<=y+1;j++)
			if( i>=0 && i<9 && j>=0 && j<9 )
				if( scop[i][j]==0 )
				{
					scop[i][j]=1;
					vittoria++;
				}
	
	for(int i=x-1;i<=x+1;i++)
		for(int j=y-1;j<=y+1;j++)
			if( i>=0 && i<9 && j>=0 && j<9 && !(i==x && j==y) )
				if( tab[i][j]==0 )
				{
					int test=1;
					for(int c=0;quadscop[c]!=0;c++)
						if( quadscop[c]==(10*(i+1))+(j+1) )
							test=0;
					if( test==1 )
						scopri_quad(i,j);
				}
					
	return;
}

/* band_mod:
Permette all'utente di aggiungere o rimuovere le bandierine dalla tabella, inserendone le coordinate.*/
int band_mod(int n)
{
	int test,x,y;
	
	while(1)
	{
		printf("\n");
		view();
		printf("\n");
		
		if( n==1 )
			printf("Inserisci le coordinate della bandierina da aggiungere (x.y): ");
		else
			printf("Inserisci le coordinate della bandierina da rimuovere (x.y): ");
			
		while( (test=rdcoord(&x,&y))==0 )
			printf("Errore nell'inserimento delle coordinate (x.y). Riprova: ");
			
		if( test==-1 )
			return -1;
		else if( scop[x-1][y-1]==1 )
			printf("La casella selezionata è già scoperta.\n");
		else if( n==1 && band[x-1][y-1]==1 )
			printf("E' già presente una bandierina sulla casella selezionata.\n");
		else if( n==0 && band[x-1][y-1]==0 )
			printf("Non è presente alcuna bandierina sulla casella selezionata.\n");
		else
		{
			if( n==1 )
			{
				band[x-1][y-1]=1;
				printf("\n");
			}
			else
			{
				band[x-1][y-1]=0;
				printf("\n");
			}
		}
	}
			
	return 0;
}

/* view:
Stampa a video la tabella di gioco.*/
void view(void)
{
	for(int i=0;i<9;i++)
	{
		printf("%d\t",i+1);
		for(int j=0;j<9;j++)
			if( band[i][j]==1 && scop[i][j]==0 )
				printf("B\t");
			else if( scop[i][j]==0 )
				printf("*\t");
			else
				printf("%d\t",tab[i][j]);
		printf("\n");
	}
	printf("\n/\t1\t2\t3\t4\t5\t6\t7\t8\t9\n");
	
	return;
}

void viewend(void)
{
	printf("Ecco la tabella completamente scoperta:\n\n");
	
	for(int i=0;i<9;i++)
	{
		printf("%d\t",i+1);
		for(int j=0;j<9;j++)
			if( tab[i][j]==-1 )
				printf("M\t");
			else
				printf("%d\t",tab[i][j]);
		printf("\n");
	}
	printf("\n/\t1\t2\t3\t4\t5\t6\t7\t8\t9\n\n");
	
	printf("\t\t    Copyright Alessandro Felisi 2016\n\n");
	
	return;
}

/* rdcoord:
Funzione che prende in ingresso due puntatori a int.
Chiede all'utente di inserire due numeri da 1 a 9 nel formato (x.y).
Nel caso in cui l'utente digiti il carattere 'm' o 'M', restituisce il valore -1.
Altrimenti, nel caso in cui vi sia un errore o l'utente inserisca le coordinate in un formato sbagliato, restituisce il valore 0.
Nel caso in cui l'utente inserisca le coordinate correttamente, assegna agli int associati ai puntatori i valori delle coordinate e restituisce il valore 1.*/
int rdcoord(int *x,int *y)
{
	char str[BUFSIZ];
	
	if( fgets(str,BUFSIZ,stdin)==NULL )
		return 0;
	
	if( (str[0]=='m' || str[0]=='M') && (str[1]=='\n' || str[1]=='\0') )
		return -1;
	else if( !( isdigit(str[0])!=0 && str[1]=='.' && isdigit(str[2])!=0 && (str[3]=='\n' || str[3]=='\0') ) || str[0]=='0' || str[2]=='0' )
		return 0;
		
	*x=atoi(&str[0]);
	*y=atoi(&str[2]);
	
	return 1;
}

/* reset:
Funzione che prende in ingresso una matrice 9x9.
Assegna 0 a ogni valore della matrice.*/
void reset(int v[9][9])
{
	for(int i=0;i<9;i++)
		for(int j=0;j<9;j++)
			v[i][j]=0;
			
	return;
}

/* gen_mine:
Funzione che prende in ingresso una matrice 9x9.
Genera 10 numeri casuali da 1 a 81 differenti tra loro e assegna il valore -1 alla posizione corrispondente a ciascun numero nella matrice.*/
void gen_mine(int v[9][9])
{
	int pos[10],aux,test;
	int x,y;
	srand(time(NULL));
	
	for(int i=0;i<10;i++)
	{
		do
		{
			test=1;
			aux=rand()%81+1;
			for(int j=0;j<i;j++)
				if( aux==pos[j] )
					test=0;
		}
		while( test==0 );
		pos[i]=aux;
	}
	
	for(int i=0;i<10;i++)
	{
		x=pos[i]/9;
		y=pos[i]%9;
		v[x][y]=-1;
	}
	
	return;
}

/* comp_mat:
Funzione che prende in ingresso una matrice 9x9.
La matrice in ingresso si suppone già formattata attraverso reset() e gen_mine().
Ad ogni posizione differente da -1 viene assegnato un valore intero corrispondente al numero di caselle circostanti il cui valore è -1.*/
void comp_mat(int v[9][9])
{
	int c;
	for(int i=0;i<9;i++)
		for(int j=0;j<9;j++)
			if( v[i][j]==0 )
			{
				c=0;
				for(int a=i-1;a<=i+1;a++)
					for(int b=j-1;b<=j+1;b++)
						if( a>=0 && a<9 && b>=0 && b<9 && v[a][b]==-1 )
							c++;
				v[i][j]=c;
			}
			
	return;
}