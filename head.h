/*
Variables,constants,functions and others
*/

#pragma once
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <conio.h>



typedef struct contact {
	char name1[16];
	char name2[16];
	char nr[16];
	char city[16];
	char email[32];
	int id;
	struct contact * next;
	struct contact * prev;
}contact;



contact *Start = NULL;//Adresa la primul element din lista de baza
contact *start = NULL;//Adresa la primul element din lista de lucru
int nrcont = 0;//Nr de contacte din fisier
int n = 10;//Nr de contacte care se afiseaza
int option = 0;
char search[256] = "\0";
int snr = 0;//nr de caractere din search
HANDLE Handle;//output handle info
int cnr = 0;//contactul din browse cu highlight
int bnr;//nr de contacte din browse

void FirstRun();//first run 
void ShowMainPage();//show page menu
void CreateListFromFile();//copy contacts from file in to new double linked circular list
void ShowContacts();//show contacts from <start>
void Move(int x, int y);//move cursor
void TColor(char color[32]);//text color
void TurnC(BOOL t);//cursor visibility
void Options();//optiuni
void OptionR();//sterge optiunea curenta
void OptionS();//arata optiunea noua
void Search();//functia pentru tot procesul de cautare
void Filter();//alege contactele bune
void Word2toStr(char *in1, char *in2, char *out);//2 strings in 1
int WordinWord(char *what, char *in);//if the first string is in the second return 1 else return 0
void RemoveContacts();//sterge pagina cu contacte
void FreeList();
void Browse();
//void Sort();
void ContactInfo(contact *p);
void RemoveContactInfo(int i);
void DeleteContact(contact **p);
void AddContact();


//remove contacts from page
void RemoveContacts() {
	TurnC(FALSE);
	Move(10, 0);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j <48; j++)
			printf(" ");
		printf("\n");
	}
}

//2 strings in 1
void Word2toStr(char *in1, char *in2, char *out) {
	for (int i = 0; i < strlen(in1); i++)
		out[i] = in1[i];
	out[strlen(in1)] = ' ';
	for (int i = 0; i < strlen(in2); i++)
	{
		out[strlen(in1) + 1 + i] = in2[i];
	}
	out[strlen(in1) + 1 + strlen(in2)] = '\0';
}

//if the first string is in the second return 1 else return 0
int WordinWord(char *what, char *in) {
	int flag = 0, len1, len2;
	len1 = strlen(in);
	len2 = strlen(what);

	for (int i = 0; i <= len1 - len2; i++)
	{
		for (int j = i; j < i + len2; j++)
		{
			flag = 1;
			if (in[j] != what[j - i])
			{
				flag = 0;
				break;
			}
		}
		if (flag == 1)
			break;
	}
	return flag;
}

//first run 
void FirstRun() {
	//console settings
	TColor("green");
	TurnC(FALSE);
	system("mode 48,40");

	//copy contacts from file in to new double linked circular list
	CreateListFromFile();

	//copy list and show working list
	Filter();

	//show page menu
	ShowMainPage();

	//show n contacts
	ShowContacts();
}

//show page menu
void ShowMainPage() {
	TColor("bblue");
	printf("                Contact List                    ");//0,16
	TColor("green");
	printf("\n\nYou have %d contacts in your list.\n", nrcont);//2,0
	printf("\n\tExit");//4,8
	printf("\n\tSearch:");//5,8
	printf("\n\tAdd");//6,8
	printf("\n\tBrowse");//7,8
}

//copy contacts from file to new list
void CreateListFromFile() {
	int id = 0;
	nrcont = 0;
	contact *pos = Start;
	int line[80];
	FILE *input;
	input = fopen("contacts.txt", "r");
	while (fgets(line, 96, input)) {
		if (pos == NULL) {
			pos = Start = (contact*)malloc(sizeof(contact));
			pos->next = Start;
			pos->prev = Start;
		}
		else {
			pos->next = (contact*)malloc(sizeof(contact));
			pos->next->prev = pos;
			pos = pos->next;

		}
		sscanf(line, "%s %s %s %s %s", pos->name1, pos->name2, pos->nr, pos->city, pos->email);
		pos->id = id;
		id++;
		nrcont++;
	}
	Start->prev = pos;
	pos->next = Start;
	//Sort();
}

//free from memory list
void FreeList() {
	bnr = 0;
	if (start == NULL || start == start->next)
		return;
	contact *p = start->next;
	start->prev->next = NULL;
	while (p != NULL) {
		free(start);
		start = p;
		p = p->next;
	}

}

//copy only good contacts
void Filter() {
	bnr = 0;

	if (Start == NULL)
		start = NULL;
	else {

		contact *p = Start;
		start = NULL;
		contact *t = start;
		do {
			char *in1 = p->name1, *in2 = p->name2, *out = (char*)malloc(sizeof(char)*(strlen(p->name1) + strlen(p->name2) + 2));
			Word2toStr(in1, in2, out);
			if (WordinWord(search, out) || search[0] == '\0') {
				bnr++;
				//copy
				if (t == NULL) {
					t = start = (contact*)malloc(sizeof(contact));
					t->next = start;
					t->prev = start;
				}
				else {
					t->next = (contact*)malloc(sizeof(contact));
					t->next->prev = t;
					t = t->next;
				}
				strcpy(t->name1, p->name1);
				strcpy(t->name2, p->name2);
				strcpy(t->nr, p->nr);
				strcpy(t->city, p->city);
				strcpy(t->email, p->email);
				t->id = p->id;
			}
			p = p->next;

		} while (p != Start);
		if (t != NULL) {
			t->next = start;
			start->prev = t;
		}
	}
}

//show contacts
void ShowContacts() {
	contact *p = start;
	Move(9, 0);
	TColor("green");
	if (p != NULL) {

		for (int i = 0; i < n; i++)
		{
			printf("\n%s %s", p->name1, p->name2);
			p = p->next;
			if (p == start)
				break;
		}
	}
	else {
		TColor("red");
		printf("\nNu s-au gasit contacte!");
	}
}

//move cursor
void Move(int x, int y) {
	COORD c;
	c.X = y;
	c.Y = x;
	SetConsoleCursorPosition(Handle, c);
}

//text color
void TColor(char color[32]) {
	Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (strcmp(color, "green") == 0)
		SetConsoleTextAttribute(Handle, FOREGROUND_GREEN);
	else if (strcmp(color, "red") == 0)
		SetConsoleTextAttribute(Handle, FOREGROUND_RED);
	else if (strcmp(color, "blue") == 0)
		SetConsoleTextAttribute(Handle, FOREGROUND_BLUE);
	else if (strcmp(color, "bgreen") == 0)
		SetConsoleTextAttribute(Handle, BACKGROUND_GREEN);
	else if (strcmp(color, "bblue") == 0)
		SetConsoleTextAttribute(Handle, BACKGROUND_BLUE);

}

//Cursor visibility
void TurnC(BOOL t) {
	CONSOLE_CURSOR_INFO inf;
	GetConsoleCursorInfo(Handle, &inf);
	inf.bVisible = t;
	SetConsoleCursorInfo(Handle, &inf);
}

//option show
void OptionS() {
	TColor("blue");
	Move(4 + option, 8);
	if (option == 0)
		printf("Exit");
	else if (option == 1)
		printf("Search:");
	else if (option == 2)
		printf("Add");
	else if (option == 3)
		printf("Browse");
}

//option remove
void OptionR() {
	TColor("green");
	Move(4 + option, 8);
	if (option == 0)
		printf("Exit");
	else if (option == 1)
		printf("Search:");
	else if (option == 2)
		printf("Add");
	else if (option == 3)
		printf("Browse");
}

//search
void Search() {
	Move(5, 16);
	TColor("red");
	TurnC(TRUE);
	while (1) {
		int c = getch();
		if (c == 27) {
			TurnC(FALSE);
			strcpy(search, "\0");
			Move(5, 16);
			for (int i = 0; i < snr; i++)
				printf(" ");
			snr = 0;
			RemoveContacts();
			FreeList();
			Filter();
			ShowContacts();
			return;
		}
		else if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == ' ') {
			search[snr] = c;
			snr++;
			search[snr] = '\0';
			TurnC(FALSE);
			Move(5, 16);
			TColor("red");
			printf("%s", search);
			Filter();
			RemoveContacts();
			ShowContacts();
			Move(5, 16 + snr);
			TurnC(TRUE);
		}
		else if (c == 13)
			Browse();
		else if (c == 8 && snr > 0) {
			search[snr] = search[snr + 1];
			snr--;
			search[snr] = '\0';
			TurnC(FALSE);
			Move(5, 16);
			TColor("red");
			printf("%s ", search);
			Filter();
			RemoveContacts();
			ShowContacts();
			Move(5, 16 + snr);
			TurnC(TRUE);
		}
	}
}

//Options
void Options() {
	OptionS();
	while (1) {
		int c = getch();
		if (c == 72 || c == 80)//daca apas sus sau jos se face scrisu curent in verde
			OptionR();

		if (c == 72) {//daca apas in sus se vopseshte precedentu in albastru
			if (option == 0)
				option = 3;
			else
				option--;
			OptionS();
		}
		else if (c == 80) {//daca apas in sus se vopseshte urmatorul in albastru
			if (option == 3)
				option = 0;
			else
				option++;
			OptionS();
		}
		else if (c == 13) {//daca apas enter
			if (option == 0)
				exit(1);
			else if (option == 1)
				Search();
			else if (option == 2) {
				AddContact();
				nrcont++;
				Move(2, 0);
				TColor("green");
				printf("You have %d contacts in your list.", nrcont);

			}
			else if (option == 3)
				Browse();

		}
	}
}

//Move through contacts on the page
void Browse() {
	TurnC(FALSE);
	TColor("bgreen");
	Move(8, 0);
	printf("Press ENTER to see selected contact info.\n");
	printf("Press BACKSPACE to delete selected contact.");
	int printed = 0;
	int off = 0;
	contact *p = start;
	contact *t = start;

	while (start != NULL) {

		if (off != 0) {
			RemoveContacts();
			ShowContacts();
		}
		TColor("red");
		Move(10 + cnr, 0);
		printf("%s %s", p->name1, p->name2);

		int c = getch();
		if (c == 27) {
			start = t;
			Move(8, 0);
			for (int i = 0; i < 48; i++)
				printf(" ");
			Move(9, 0);
			for (int i = 0; i < 48; i++)
				printf(" ");
			if (search[0] == '\0') {

				cnr = 0;
				start = t;
				strcpy(search, "\0");
				Move(5, 16);
				for (int i = 0; i < snr; i++)
					printf(" ");
				snr = 0;
				RemoveContacts();
				FreeList();
				Filter();
				ShowContacts();
				RemoveContactInfo(1);
				Move(5, 16);
				return;
			}
			else {
				RemoveContactInfo(1);
				Move(5, 16 + snr);
				TurnC(TRUE);
				return;
			}
		}
		else if (c == 80)//down
		{
			if (off < 0)
				off = 0;
			if (cnr < 9 && (cnr <= bnr - 2) && off == 0) {
				TColor("green");
				Move(10 + cnr, 0);
				for (int i = 0; i < 32; i++)
					printf(" ");
				Move(10 + cnr, 0);
				printf("%s %s", p->name1, p->name2);
				cnr++;
				p = p->next;
			}
			else if (cnr == 9 || (cnr == bnr - 1)) {
				off++;
				start = start->next;
				p = p->next;
			}
		}
		else if (c == 72)//up
		{
			if (off > 0)
				off = 0;
			if (cnr > 0 && off == 0) {
				TColor("green");
				Move(10 + cnr, 0);
				for (int i = 0; i < 32; i++)
					printf(" ");
				Move(10 + cnr, 0);
				printf("%s %s", p->name1, p->name2);
				cnr--;
				p = p->prev;
			}
			else if (cnr == 9 || cnr == 0) {
				off--;
				start = start->prev;
				p = p->prev;
			}
		}
		else if (c == 13) {
			ContactInfo(p);
		}
		else if (c == 8) {
			nrcont--;
			Move(2, 0);
			for (int i = 0; i < 48; i++)
			{
				printf(" ");
			}
			Move(2, 0);
			TColor("green");
			printf("You have %d contacts in your list.", nrcont);

			if (Start == NULL)
				return;

			DeleteContact(&p);
			off = 0;
			cnr = 0;
			RemoveContacts();
			ShowContacts();


		}
	}
}

//add contact
void AddContact() {
	TColor("bgreen");
	Move(20, 0);
	printf("Press Enter to add contact and ESC to cancel");
	TColor("green");
	Move(21, 0);
	printf("  FirstName:\n SecondName:\n     Number:\n       City:\n      Email:");

	Move(27, 1);
	printf("ADD         CANCEL");
	int coption = 0;
	typedef struct {
		int len;
		char name[18];
	}field;
	field fields[5];
	for (int i = 0; i < 5; i++)
	{
		fields[i].name[0] = '\0';
		fields[i].len = 0;
	}
	while (1) {
		TurnC(FALSE);
		for (int i = 0; i < 5; i++)
		{
			TColor("red");
			Move(21 + i, 13);
			printf("%s ", fields[i].name);
		}
		Move(21 + coption, 13 + fields[coption].len);
		TurnC(TRUE);

		int ch = getch();
		if (ch==9) {
			if (coption == 4)
				coption = 0;
			else
				coption++;
		}
		else if (ch == 27) {
			TurnC(FALSE);
			TColor("red");
			Move(27, 13);
			printf("CANCEL");
			while (1)
			{

				int key = getch();
				if (key == 27) {
					TColor("green");
					Move(27, 13);
					printf("CANCEL");
					break;
				}
				else if (key == 13) {
					Move(20, 0);
					for (int i = 0; i < 10; i++)
					{
						for (int j = 0; j < 48; j++)
						{
							printf(" ");
						}
						printf("\n");
					}
					return;
				}
				
			}
		}
		else if (ch == 13 && fields[0].len>1 && fields[1].len>1 && fields[2].len>1 && fields[3].len>1 && fields[4].len>1) {
			TurnC(FALSE);
			TColor("red");
			Move(27, 1);
			printf("ADD");


			while (1) {

				int key = getch();
				if (key == 27) {
					TColor("green");
					Move(27, 1);
					printf("ADD");
					break;

				}
				else if (key == 13) {
					char name[33];//lowest name

					//add contact

					//save info into new node
					contact *cont;
					cont = (contact*)malloc(sizeof(contact));
					strcpy(cont->name1, fields[0].name);
					strcpy(cont->name2, fields[1].name);
					strcpy(cont->nr, fields[2].name);
					strcpy(cont->city, fields[3].name);
					strcpy(cont->email, fields[4].name);

					//put node in to the beggining of the list
					Start->prev->next = cont;
					cont->prev = Start->prev;
					cont->next = Start;
					Start->prev = cont;
					Start = cont;

					//create a new list
					contact* list;
					list = (contact*)malloc(sizeof(contact));
					contact* tmp = list;

					//while (Start->next!=Start)
					while (1) {

						//find the lowest name in Start
						contact *p = Start;
						char tname[33];

						Word2toStr(p->name1, p->name2, tname);
						strcpy(name, tname);
						while (1) {
							Word2toStr(p->name1, p->name2, tname);
							if (strcmp(tname, name) < 0) {
								strcpy(name, tname);
							}
							p = p->next;
							if (p == Start)
								break;
						}


						//search node with the lowest name in start
						p = Start;
						while (1) {
							Word2toStr(p->name1, p->name2, tname);
							if (strcmp(tname, name) == 0)
								break;
							p = p->next;

						}
						//add new node in new list
						strcpy(tmp->name1, p->name1);
						strcpy(tmp->name2, p->name2);
						strcpy(tmp->nr, p->nr);
						strcpy(tmp->city, p->city);
						strcpy(tmp->email, p->email);
						tmp->next = (contact*)malloc(sizeof(contact));
						tmp->next->prev = tmp;
						tmp = tmp->next;


						//delete node in start with lowest name
						if (p == Start) {
							Start->next->prev = Start->prev;
							Start->prev->next = Start->next;
							contact *temp = Start->next;
							free(Start);
							Start = temp;
						}
						else {

						p->next->prev = p->prev;
						p->prev->next = p->next;
						free(p);
						}
						if (Start->next == Start)
							break;
					}

					//add last node in list Start node
					contact *p = Start;
					strcpy(tmp->name1, p->name1);
					strcpy(tmp->name2, p->name2);
					strcpy(tmp->nr, p->nr);
					strcpy(tmp->city, p->city);
					strcpy(tmp->email, p->email);
					tmp->next = list;
					list->prev = tmp;
   					Start = list;
					//freelist,filter,delete ctacts, show contacts
					FreeList();
					Filter();
					RemoveContacts();
					ShowContacts();
					Move(20, 0);
					for (int i = 0; i < 10; i++)
					{
						for (int j = 0; j < 48; j++)
						{
							printf(" ");
						}
						printf("\n");
					}


					FILE *out;
					contact *t = Start;
					out = fopen("contacts.txt", "w");
					while (t != NULL)
					{
						fprintf(out, "%s %s %s %s %s\n", t->name1, t->name2, t->nr, t->city, t->email);
						t = t->next;
						if (t == Start)
							break;
					}
					if (Start == NULL) {
						fprintf(out, "");
					}
					fclose(out);
					return;
				}
			}
		

		}
		else if (fields[coption].len < 16 && ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == ' ' || ch == '+' || ch == '@' || ch == '.' || ch == '-') {
			if (!GetAsyncKeyState(VK_UP) && !GetAsyncKeyState(VK_DOWN) && !GetAsyncKeyState(VK_LEFT) && !GetAsyncKeyState(VK_RIGHT)) {

			fields[coption].name[fields[coption].len] = ch;
			fields[coption].len++;
			fields[coption].name[fields[coption].len] = '\0';
			}

		}
		else if (fields[coption].len > 0 && ch == 8) {
			fields[coption].name[fields[coption].len] = fields[coption].name[fields[coption].len + 1];
			fields[coption].len--;
			fields[coption].name[fields[coption].len] = '\0';
		}


	}

}

//elete contact
void DeleteContact(contact **p) {
	if (Start->next == Start) {
		Start = NULL;
		start = NULL;
	}
	else {

		contact *t = *p;
		int id = t->next->id;
		contact *m = Start;
		while (t->id != m->id)
			m = m->next;
		if (m == Start) {
			Start->next->prev = Start->prev;
			Start->prev->next = Start->next;
			m = m->next;
			free(Start);
			Start = m;
		}
		else {
			contact *tmp = m->next;
			m->next->prev = m->prev;
			m->prev->next = m->next;
			free(m);
			m = tmp;
		}
		FreeList();
	}
	Filter();
	*p = start;
	FILE *out;
	contact *t = Start;
	out = fopen("contacts.txt", "w");
	while (t != NULL)
	{
		fprintf(out, "%s %s %s %s %s\n", t->name1, t->name2, t->nr, t->city, t->email);
		t = t->next;
		if (t == Start)
			break;
	}
	if (Start == NULL) {
		fprintf(out, "");
	}
	fclose(out);
}

//Contact info
void ContactInfo(contact *p) {
	RemoveContactInfo(0);
	TurnC(FALSE);
	TColor("green");
	Move(21, 0);
	printf("\t     %s \n \t     %s \n \t     %s \n \t     %s \n \t     %s ", p->name1, p->name2, p->nr, p->city, p->email);
	TColor("blue");
	Move(21, 0);
	printf("  FirstName:\n SecondName:\n     Number:\n       City:\n      Email:");
}

//Remove contact info
void RemoveContactInfo(int i) {
	if (i) {

		Move(21, 0);
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 48; j++)
			{
				printf(" ");
			}
			printf("\n");
		}
	}
	else {
		for (int i = 0; i < 5; i++)
		{
			Move(21 + i, 10);
			for (int j = 0; j < 48; j++)
			{
				printf(" ");
			}
		}
	}
}
