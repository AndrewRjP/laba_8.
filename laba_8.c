#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define STR_LEN 31
#define SIZE 100

int size = 0;

typedef struct
{
  char name[STR_LEN];
  char language[STR_LEN];
  char currency[STR_LEN];
  double exchange_rate;
} COUNTRY;

int check_string(char* s)
{
  int isCorrect = 1;
  for (int i = 0; i < strlen(s) - 2; i++)
    if (!(isalpha(s[i]) || s[i] == ' ' || s[i] ==','))
    {
      isCorrect = 0;
      break;
    }
  return isCorrect;
}

int check_number(char* s)
{
  int isCorrect = 1;
  for (int i = 0; i < strlen(s) - 2; i++)
    if (!(isdigit(s[i]) || s[i] == '.' || s[i] == '-'))
    {
      isCorrect = 0;
      break;
    }
  return isCorrect;
}

int readFromFile(COUNTRY *c, char* fname)
{
  FILE *f = fopen(fname, "rb");
  if (!f) {
    printf("Can't open file \"%s\"!\n", fname);
    return 1;
  }
  char *estr;
  for(int i = 0; 1; i++){
    int add = 1;
    char name[STR_LEN];
    estr = fgets(name, STR_LEN, f);
    if (!estr && i == 0) {
      printf("file \"%s\" is empty!\n", fname);
      break;
    }
    if (!check_string(name)) {
      printf("Error: row %d. Name must consist only letters!\n", i*4 + 1);
      add = 0;
    }
    if (!estr) {
      printf("Not enough information about %d country!\n", size + 1);
      break;
    }

    char language[STR_LEN];
    estr = fgets(language, STR_LEN, f);
    if (!check_string(language)) {
      printf("Error: row %d. Language must consist only letters!\n", i * 4 + 2);
      add = 0;
    }
    if (!estr) {
      printf("Not enough information about %d country!\n", size + 1);
      break;
    }

    char currency[STR_LEN];
    estr = fgets(currency, STR_LEN, f);
    if (!check_string(currency)) {
      printf("Error: row %d. Currency must consist only letters!\n", i * 4 + 3);
      add = 0;
    }
    if (!estr) {
      printf("Not enough information about %d country!!\n", size + 1);
      break;
    }

    char rate[STR_LEN];
    estr = fgets(rate, STR_LEN, f);
    if (!check_number(rate)) {
      printf("Error: row %d. Exchange rate must be floating value!\n", i * 4 + 3);
      add = 0;
    }
    double exchange_rate = atof(rate);
    if (exchange_rate <= 0)
    {
      printf("Error: row %d. Exchange rate must be >= 0!\n", i * 4 + 3);
      add = 0;
    }    
    if (add)
    {
      strcpy(c[size].name, name);
      c[size].name[strlen(name) - 2] = '\0';
      strcpy(c[size].language, language);
      c[size].language[strlen(language) - 2] = '\0';
      strcpy(c[size].currency, currency);
      c[size].currency[strlen(currency) - 2] = '\0';
      c[size].exchange_rate = exchange_rate;
      size++;
    }  
    if(!estr) break;
  }
  fclose(f);
  return size > 0 ? 0 : 1;
}

void print(COUNTRY c[])
{
  for (int i = 0; i < (STR_LEN + 1) * 3 + 16; i++) printf("_");
  printf("\n| #   | %-30s| %-30s| %-30s| %-7s|\n", "Name", "Language", "Currency", "Rate");
  for (int i = 0; i < (STR_LEN + 1) * 3 + 16; i++) printf("-");
  for (int i = 0; i < size; i++)
  {
    printf("\n| %-4d| %-30s| %-30s| %-30s| %-7.3lf|\n", i+1, c[i].name, c[i].language, c[i].currency, c[i].exchange_rate);
    for (int i = 0; i < (STR_LEN + 1) * 3 + 16; i++) printf("-");
  }
}

int searchByName(COUNTRY c[], char* name)
{
  for (int i = 0; i < size; i++)
    if (strcmp(name, c[i].name) == 0)
      return i;
  return size;
}

void removeElement(COUNTRY c[], int ind)
{
  for (int i = ind; i < size - 1; i++)
    c[i] = c[i + 1];
  size--;
}

void addCountry(COUNTRY c[])
{
  char str[STR_LEN];
  printf("Enter name: ");
  while(1)
  {
    fgets(str, STR_LEN, stdin);
    str[strlen(str) - 1] = '\0';
    if (!check_string(str))
      printf("Error: Name must consist only letters!\nTry again: ");
    else break;
  }
  strcpy(c[size].name, str);
  printf("Enter language: ");
  while (1)
  {
    fgets(str, STR_LEN, stdin);
    str[strlen(str) - 1] = '\0';
    if (!check_string(str))
      printf("Error: Language must consist only letters!\nTry again: ");
    else break;
  }
  strcpy(c[size].language, str);

  printf("Enter Currency: ");
  while (1)
  {
    fgets(str, STR_LEN, stdin);
    str[strlen(str) - 1] = '\0';
    if (!check_string(str))
      printf("Error: Currency must consist only letters!\nTry again: ");
    else break;
  }
  strcpy(c[size].currency, str);

  printf("Enter Exchange rate: ");
  while (1)
  {
    fgets(str, STR_LEN, stdin);
    str[strlen(str) - 1] = '\0';
    if (!check_number(str) || atof(str) <= 0)
      printf("Error: Exchange rate must be floating value and must be >= 0!\nTry again: ");
    else break;
  }
  c[size].exchange_rate = atof(str);
  size++;
}

void SaveToFile(COUNTRY *c, char* fname)
{
    FILE *fout = fopen(fname, "wb");
    for(int i = 0; i < size; i++)
    {
        fprintf(fout, "%s\n", c[i].name);
        fprintf(fout, "%s\n", c[i].language);
        fprintf(fout, "%s\n", c[i].currency);
        fprintf(fout, "%lf", c[i].exchange_rate);
        if(i != size-1) fprintf(fout, "\n");
    }
    fclose(fout);   
}

int main(void)
{
  COUNTRY countries[SIZE];
  char fname[STR_LEN];
  printf("Enter name of file with data: ");
  scanf("%s", fname);
  getchar();
  if (readFromFile(countries, fname) == 0)
  {
    print(countries);
    printf("\n\n");
    printf("Enter name of country, witch must be deleted: ");
    char name[STR_LEN];
    fgets(name, STR_LEN, stdin);
    name[strlen(name) - 1] = '\0';
    int index = searchByName(countries, name);
    if (index >= size) printf("Not found!\n");
    else
    {
      removeElement(countries, index);
      print(countries);
      printf("\n\n");
    }
    for (int i = 0; i < 2; i++)
    {
      printf("Enter data about %d country:\n", i + 1);
      addCountry(countries);
      printf("\n");
    }
    print(countries);
    printf("\n\n");
  }
  SaveToFile(countries, fname); 
  
  getchar();
  return 0;
}
