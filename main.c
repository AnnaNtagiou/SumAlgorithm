#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <io.h> /* POSIX */

#undef _DEBUG /* Do not include _DEBUG code in the executable */

#include "abstract.h"

#define _WINDOWSOS_ /* Run Windows specific code (chcp 1253 console utility) */
#define _DEF_TOLERANCE_ 0 /* Default abstraction tolerance */
#define _DEF_SENT_RECK 45 /* Default sentences importance reckoning percentage */

/* main.c prototypes */
int ProcessSentence(Sentence*, NameEntry *DBHigh, const int nrHighDBLen, NameEntry *DBMed,
const int nrMedDBLen, const int nrSentenceOrder, const int nrSIReck);
int ReturnParamInt(char **argv, int argc, const char *P, const int nrDefault, int *nrError);
int _SentQCmp(const void*, const void*);

/* Program entry point */
int main(int argc, char* argv[])
{
	/* Sentence Container */
	Sentence *ptrSentence = NULL;
	/* Sentence Container Length */
	int nrSents = 0, nrSentCount = 0, nrPased;
	/* DB Containers */
	NameEntry *ptrHighWordDB = NULL,
	*ptrMedWordDB = NULL;
	/* DB Containers Length */
	int nrHighWordDBLen = 0,
	nrMedWordDBLen = 0;
	/* Various */
	char *ptrFileContents = NULL;
	int nrError, nrTolerance = _DEF_TOLERANCE_, nrIgnoredSentences = 0, nrSentImpRound;
	float fSentImp;
	FILE *fOut = NULL;
	
	/* Windows OS? */
	#ifdef _WINDOWSOS_
	/* To see Greek on stdout, set chcp to 1253 (Windows console fonr should be "Lucida Console") */
	system("chcp 1253"); /* POSIX */
	#endif
	
	fprintf(stderr, "Abstract\n\n");
	/* Parameters? */
	if(argc <= 1 || argc > 4)
	{
		fprintf(stderr, "Usage: abstract \"input file\" (\"output file\")\n");
		return 1;
	}
	/* Validate Parameters */
	if(access(argv[1], 0) == -1)
	{
		fprintf(stderr, "DOS Error: File not found - %s\n", argv[1]);
		return 1;
	}
	/* Output to argv[2] or stdout? */
	if(argc == 3)
	{
		/* Redirect stdout to argv[2] file */
		if((fOut = freopen(argv[2], "w", stdout)) == NULL)
			fprintf(stderr, "DOS Error: Cannot create file or redirect STDIN - %s\n", argv[2]);
	}
	/* Print file info */
	fprintf(stderr, " -File input :%s\n", argv[1]);
	fprintf(stderr, " -File output:%s\n", argc == 3? argv[2]: "stdout");
	fprintf(stderr, "\n");
	/* Print Tolerance settings */
	fprintf(stderr, " -Correlation tolerance: >%d\n", nrTolerance);
	/* Load input file */
	if((ptrFileContents = LoadFileContents(argv[1], ptrFileContents)) == NULL)
		return 1; /* Error! */
	/* Profile entire text words frequencies */
	fprintf(stderr, " -Word frequency profiling ...");
	/* Build word profile for words that appear >= 6 times in text */
	if((ptrHighWordDB = MakeWordsProfile(ptrFileContents, 6, 6, ptrHighWordDB, &nrHighWordDBLen)) == NULL && nrHighWordDBLen)
	{
		/* Error! */
		free(ptrFileContents);
		FreeDB(ptrHighWordDB, nrHighWordDBLen);
		/* Close redir stream */
		if(fOut != NULL)
			fclose(fOut);
		return 1;
	}
	/* Build word profile for words that appear >= 3 to 5 times in text */
	if((ptrMedWordDB = MakeWordsProfile(ptrFileContents, 3, 5, ptrMedWordDB, &nrMedWordDBLen)) == NULL && nrMedWordDBLen)
	{
		/* Error! */
		free(ptrFileContents);
		FreeDB(ptrHighWordDB, nrHighWordDBLen);
		/* Close redir stream */
		if(fOut != NULL)
			fclose(fOut);
		return 1;
	}
	/* Advance from progress indicator */
	fprintf(stderr, "\n");
	/* Load text sentences to ptrSentence ...*/
	ptrSentence = GetTextSentences(ptrFileContents, &nrSents, &nrError);
	/* Result? */
	if(nrError)
	{
		/* Error! */
		free(ptrFileContents);
		FreeDB(ptrHighWordDB, nrHighWordDBLen);
		FreeDB(ptrMedWordDB, nrMedWordDBLen);
		/* Close redir stream */
		if(fOut != NULL)
			fclose(fOut);
		return 1;
	}
	/* Calculate sentences importance */
	fSentImp = (_DEF_SENT_RECK * nrSents) / 100.0;
	/* Round percentage to integer (optimistic) */
	nrSentImpRound = ceil(fSentImp);
	/* Enough sentences to calculate importance? */
	if((int)fSentImp)
		fprintf(stderr, " -Sentence(s) importance reckoning: %.2f%%->%d%% (tolerance ±%d%%)\n", fSentImp, nrSentImpRound, _DEF_SENT_RECK);
	else
		fSentImp = -1.0;
	/* Process sentences */
	for(nrSentCount = 0; nrSentCount < nrSents; nrSentCount++)
	{
		/* For easier reference point ptrSentence[ntSentCount] to ptrSent */
		Sentence *ptrSent = &ptrSentence[nrSentCount];
		#ifdef _DEBUG
			/* Dump each sentence (debug) */
			printf("Sentece Count: %d - %d\n", nrSentCount, nrSents);
			printf("%s\n\n", ptrSent->Str);
		#endif
		/* Process sentence */
		ProcessSentence(ptrSent, ptrHighWordDB, nrHighWordDBLen, ptrMedWordDB,
		nrMedWordDBLen, nrSentCount, nrSentImpRound);
	}
	/* Diagnostic for the user */
	fprintf(stderr, " -%d Sentence(s) structure discern ...", nrSentCount);
	/* QSort */
	//qsort(ptrSentence, nrSents, sizeof(Sentence), _SentQCmp);
	fprintf(stderr, "complete!\n\n");
	/* Return Results */
	for(nrSentCount = 0; nrSentCount < nrSents; nrSentCount++)
	{
		/* Write Sentences with Score more than nTolerance */
		if(ptrSentence[nrSentCount].Score > nrTolerance)
		{
		printf("Sentence #%d/#%d (Score: %d):\n%s\n",
		nrSentCount + 1, nrSents,
		ptrSentence[nrSentCount].Score, ptrSentence[nrSentCount].Str);
		}
		else
			nrIgnoredSentences++;
	}
	/* Calculate abstraction rate */
	if(nrIgnoredSentences && nrSentCount)
		fprintf(stderr, " -Sentences overruled %.2f%%\n", (float)((float)nrIgnoredSentences / (float)nrSentCount) * 100);
	/* Print sentences in a essay-like form (without score) */
	printf("\n\n");
	for(nrSentCount = 0; nrSentCount < nrSents; nrSentCount++)
		if(ptrSentence[nrSentCount].Score > nrTolerance)
			printf("%s\n", ptrSentence[nrSentCount].Str);
	/* Free input */
	free(ptrFileContents); ptrFileContents = NULL;
	/* Free Sentences */
	FreeTextSentences(ptrSentence, nrSents);
	/* Free DB subsystem */
	FreeDB(ptrHighWordDB, nrHighWordDBLen);
	FreeDB(ptrMedWordDB, nrMedWordDBLen);
	/* Close redir stream */
	if(fOut != NULL)
		fclose(fOut);
	#ifdef _DEBUG
		fprintf(stderr, "\nEnd of program, press Enter to exit..");
		getchar();
	#endif
	return 0;
	}
/* Process Sentence and return Score or -1 error */
int ProcessSentence(Sentence *S, NameEntry *DBHigh, const int nrHighDBLen, NameEntry *DBMed, const int nrMedDBLen, const int nrSentenceOrder, const int nrSIReck)
{
	/* Score sentence by importance reckoning */
	SentenceImportance(S, 7, nrSentenceOrder, nrSIReck);
	/* Score sentence "Word Count" */
	if(!SentenceWordScore(S))
	{
		perror("ProcessSentence:SentenceWordScore");
		return -1;
	}
	/* Score sentence "Proper Names" */
	if(!SentenceProperNamesScore(S)) /*, NameDB, nrNameDBLen))*/
	{
		perror("ProcessSentence:SentenceProperNamesScore");
		return -1;
	}
	/* Score sentence Numeric Data */
	if(!SentenceNumericDataScore(S))
	{
		perror("ProcessSentence:SentenceNumericDataScore");
		return -1;
	}
	/* Score sentence Day-Date Data */
	if(!SentenceDateScore(S))
	{
		perror("ProcessSentence:SentenceDateScore");
		return -1;
	}
	/* Score sentence by word repetition */
	if(!SentenceWordRepetition(S, 10, DBHigh, nrHighDBLen))
	{
		perror("ProcessSentence:SentenceWordRepetition(10)");
		return -1;
	}
	if(!SentenceWordRepetition(S, 5, DBMed, nrMedDBLen))
	{
		perror("ProcessSentence:SentenceWordRepetition(5)");
		return -1;
	}
	/* Score sentence Quotes */
	if(!SentenceScoreQuotes(S))
	{
		perror("ProcessSentence:SentenceScoreQuotes");
		return -1;
	}
	/* Return. */
	return 1;
}
/* Sentences qsort-sorting function */
int _SentQCmp(const void *A, const void *B)
{
	Sentence *ptrA = (Sentence*)A, *ptrB = (Sentence*)B;
	if(ptrA->Score > ptrB->Score)
		return -1;
	if(ptrA->Score < ptrB->Score)
		return 1;
	return 0;
}
