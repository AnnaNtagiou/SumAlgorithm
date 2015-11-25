#ifndef _abstract_H
#define _abstract_H
/* Standard Delimiters for tokenise functions. */
static char *DELIM = " ;,.!/\()\n\t";
/* Sentence structure. This structure holds a pointer to a string which will
* hold the sentence and the score of the sentence stored.
*/
typedef struct _Sentence{
	char *Str;
	int Score;
}Sentence;
/* Token structure. This structure is responsible for holding the sentence tokens. */
typedef struct _Tokeniser{
	char **token;
	int tokencount;
}Tokeniser;
/* Database structure. This structure is responsible for holding the database tokens. */
typedef struct _NameEntry
{
	char *Name;
}NameEntry;
/* Lexicography utility functions */
char *mystrdup(char *Buffer);
int toupperGr(const unsigned char Char);
int tolowerGr(const unsigned char Char);
char *struprGr(char *S);
char *strlwrGr(char *S);
char *strnoaccent(const char *S);
int isday(const char *S);
int ismonth(const char *S);
int isnumeric(const char *S);
int isabbrevyear(const char *S);
int isabbrevword(const char *S);
int issemantictoken(const char *S);
int islowergramtoken(const char *S);
int isstr(char *S, unsigned char **Data, int nrSize);
/* Sentence handling functions. */
char *LoadFileContents(const char *filename, char *Contents);
Sentence *GetTextSentences(const char *FileContents, int *nrSentences, int *Error);
void FreeTextSentences(Sentence *S, const int nrSentences);
/* Tokinise functions. */
Tokeniser *FillTokeniserBySentence(char *inputSent, int ignoreCharacters,const char *delim, int *Tokens);
void FreeTokeniser(Tokeniser *t);
/* Score functions. */
int SentenceWordScore(Sentence *s);
int SentenceNumericDataScore(Sentence *s);
int SentenceProperNamesScore(Sentence *s);
int SentenceDateScore(Sentence *s);
int SentenceScoreQuotes(Sentence *s);
void SentenceImportance(Sentence *s, const int nrScore, const int nrOrder, const int nrReckoning);
NameEntry* MakeWordsProfile(char*, const int nrMin, const int nrMax, NameEntry *Container, int *nrContainerLen);
int SentenceWordRepetition(Sentence *s, const int nAddScore, NameEntry *Container, const int nrContainerLen);
/* Database (Container) functions. */
void FreeDB(NameEntry *Container, const int nrWords);
int _DBQCmp(const void*, const void*);
int IsDBStr(NameEntry *Container, const int nrWords, const char *Name);
int IsDBToken(NameEntry *Container, const int nrWords, Tokeniser*);
NameEntry *InsertWordToMemDB(NameEntry*, char *Word, int *nrWords, int *Error);
int SortMemDB(NameEntry*, const int nrWords);
#endif
