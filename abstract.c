#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#undef _DEBUG /* Release build */
#include "abstract.h"

/********************** Lexicography utility functions ************************/
/* Return char* duplicate */
char *mystrdup(char *Buffer)
{
	char *ptrDupBuffer = NULL;
	/* Anything to duplicate? */
	if(Buffer != NULL)
	{
		/* Alloc a new buffer and then ..*/
		if((ptrDupBuffer = (char*)malloc(strlen(Buffer) + 1)) != NULL)
			strcpy(ptrDupBuffer, Buffer);/* duplicate Buffer! */
	}
	/* Return. */
	return ptrDupBuffer;
}

/* Turn Char (Greek or English) to Uppercase Char and retun it */
/* On impossible conversion, return Char unchanged. */
int toupperGr(unsigned char Char)
{
	/* Conversion table */
	unsigned char Win1253L[34] = "АБЦДЕФГХИЙКЛМНОПЯСТУЖВЬЫэщчъЭЩЧЗШР",
	Win1253U[34] = "абцдефгхийклмнопястужвьы╒╦╧╨╪╬©зшс",
	*ptrCharP = NULL;
	/* Search for Char in lowercase table */
	ptrCharP = (char*)memchr(Win1253L, Char, sizeof(Win1253L));
	/* Anything found? */
	if(ptrCharP == NULL)
	{
		/* No, could it be an English one? */
		if(isalpha(Char))
			return toupper(Char); /* Yes, toupper it! */
		else
			return Char; /* No, return it unchanged */
	}
	else/* Yes, return the appropriate uppercase Char from uppercase table */
		return Win1253U[ptrCharP - Win1253L];
}

/* Turn Char (Greek or English) to Lowercase Char and retun it */
/* On impossible conversion, return Char unchanged. */
int tolowerGr(unsigned char Char)
{
	/* Conversion table */
	unsigned char Win1253L[34] = "АБЦДЕФГХИЙКЛМНОПЯСТУЖВЬЫэщчъЭЩЧЗШР",
	Win1253U[34] = "абцдефгхийклмнопястужвьы╒╦╧╨╪╬©зшс",
	*ptrCharP = NULL;
	/* Search for Char in lowercase table */
	ptrCharP = (char*)memchr(Win1253U, Char, sizeof(Win1253L));
	/* Anything found? */
	if(ptrCharP == NULL)
	{
		/* No, could it be an English one? */
		if(isalpha(Char))
			return tolower(Char); /* Yes, toupper it! */
		else
			return Char; /* No, return it unchanged */
	}
	else
		/* Yes, return the appropriate uppercase Char from uppercase table */
		return Win1253L[ptrCharP - Win1253U];
}

/* Return S uppercased */
char *struprGr(char *S)
{
	int Count;
	/* Anything to process? */
	if(S != NULL)
	{
		/* Byte to byte processing */
		for(Count = 0; Count < (int)strlen(S); Count++)
			S[Count] = toupperGr(S[Count]);
	}
	return S;
}

/* Return S lowercased */
char *strlwrGr(char *S)
{
	int Count;
	/* Anything to process? */
	if(S != NULL)
	{
		/* Byte to byte processing */
		for(Count = 0; Count < (int)strlen(S); Count++)
			S[Count] = tolowerGr(S[Count]);
	}
	/* Return. */
	return S;
}

/* Return a new S with no accent symbols (else returns a S duplicate!) */
char *strnoaccent(const char *S)
{
	char *ptrS = NULL;
	/* Copy S to ptrS */
	if((ptrS = mystrdup((char*)S)) == NULL)
	{
		perror("strnoaccent:mystrdup");
		free(ptrS); 
		ptrS = NULL;
	}
	else
	{
		int Count;
		/* Conversion table */
		//unsigned char UpperAccent[9] = "╒╦╧╨╪╬©зш";
		//unsigned char UpperSimple[9] = "аегиоуыиу";
		
		/*------------------------------------------------1st change--------------------------------*/
		char UpperAccent[9] = "╒╦╧╨╪╬©зш";
		char UpperSimple[9] = "аегиоуыиу";
		/*------------------------------------------------1st change--------------------------------*/
		
		/* Convert ptrS to uppercase */
		ptrS = struprGr(ptrS);
		/* Remove Greek accent symbols */
		for(Count = 0; Count < (int)strlen(ptrS); Count++)
		{
			/* Is ptrS[Count] char .. */
			char *ptrCharP = memchr(UpperAccent, ptrS[Count], sizeof(UpperAccent));
			/* ..accented? */
			if(ptrCharP == NULL)
				continue; /* No */
			/* Yes! Remove its accent */
			ptrS[Count] = UpperSimple[ptrCharP - UpperAccent];
		}
	}
	/* Return a new, unaccented, ptrS */
	return ptrS;
}
/* Return 1 if S is the name of a known Greek Day else 0 (-1 on error!) */
int isday(const char *S)
{
	const unsigned char *DayName[] =
	{ "деутеяа", "тяитг", "тетаятг", "пелптг", "паяасйеуг", "саббато", "йуяиайг" };
	
	return isstr((char*)S, (unsigned char**)DayName, sizeof(DayName)/sizeof(unsigned char*));
}

/* Return 1 if S is the name of a known Greek Month else 0 (-1 on error!) */
int ismonth(const char *S)
{
	const unsigned char *MonthName[] = {
	"иамоуаяиос", "жебяоуаяиос", "лаятиос",
	"апяикиос" , "лаиос" , "иоумиос",
	"иоукиос" , "ауцоустос" , "септелбяиос",
	"ойтобяиос" , "моелбяиос" , "дейелбяиос",
	"иамоуаягс" , "жебяоуаягс" , "лаятгс",
	"апяикгс" , "лагс" , "иоумгс",
	"иоукгс" , "септелбягс" , "ойтыбягс",
	"моелбягс" , "дейелбягс" , "жкебаягс",
	"иамоуаяиоу", "жебяоуаяиоу", "лаятиоу" ,
	"апяикиоу" , "лаиоу" , "иоумиоу" ,
	"иоукиоу" , "ауцоустоу" , "септелбяиоу",
	"ойтыбяиоу" , "моелбяиоу" , "дейелбяиоу",
	"жкебаяиоу" , "иам" , "жеб" ,
	"лая" , "апя" , "лаи" ,
	"иоу" , "иоук" , "ауц" ,
	"сеп" , "ойт" , "мое" ,
	"дей"};
	
	return isstr((char*)S, (unsigned char**)MonthName, sizeof(MonthName)/sizeof(unsigned char*));
}

/* Return 1 if S is numeric else 0 */
int isnumeric(const char *S)
{
	return (S != NULL && strlen(S)) ? strspn(S, "0123456789") == strlen(S): 0;
}

/* Return 1 if S is a year abbreviation (like '60 '70 '80 etc) else 0 (on error return -1) */
int isabbrevyear(const char *S)
{
	/* Anything to process? */
	if(S != NULL)
	{
		char *ptrAbbrevPos = NULL;
		int AbbrevPos;
		/* Year abbrevation should be in form 'nn (3 chars wide) */
		if(strlen(S) == 3)
		{
			/* Abbrevation char ' position? */
			if((ptrAbbrevPos = memchr(S, '\'', strlen(S))) == NULL)
				return 0; /* Not found */
			/* Found, convert it to int for easier reference */
			AbbrevPos = ptrAbbrevPos - S;
			/* After abbrevation char should follow a numeric .. */
			return (AbbrevPos + 1 < (int)strlen(S))? isnumeric(&S[AbbrevPos + 1]): 0;
		}
	}
	/* Nothing! */
	return 0;
}

/* Return 1 if S begins with a capital letter else 0 */
int issemantictoken(const char *S)
{
	if(S != NULL && strlen(S))
	{
		/* Capital letters table */
		unsigned char Caps[64] = "абцдефгхийклмнопястужвьы╒╦╧╨╪╬©зшQWERTYUIOPASDFGHJKLZXCVBNM";
		/* Is first S letter capital? */
		return memchr(Caps, S[0], strlen(Caps)) != NULL;
	}
	/* Return. */
	return 0;
}

/* Return 1 if S is a Greek atom token else 0 */
int islowergramtoken(const char *S)
{
	/* Greek pronouns */
	const unsigned char *ptrPronouns[] =
	{ "ецы" ,"елема","елеис","елас","лоу" ,"ле" ,"лас" ,"есу" ,"есема" ,
	"есас" ,"соу" ,"се" ,"сас" ,"аутос","аутоу","аутои","аутым","аутоус",
	"тос" ,"тоу" ,"том" ,"тои" ,"тоус" ,"аутг" ,"аутгс","аутгм","аутес" ,
	"тг" ,"тгс" ,"тгм" ,"тес" ,"тоус" ,"тис" ,"ауто" ,"аутоу","аута" ,
	"то" ,"тоу" ,"та" ,"тоус","есеис","аута" ,"стгм" ,"стг" ,"ста" ,
	"сто" ,"стом" ,"стоус","стым","стис"
	};
	/* Greek articles */
	const unsigned char *ptrArticles[] =
	{ "о" ,"г" ,"то" ,
	"ои" ,"тым","тоус",
	"г" ,"тгс","тг" ,
	"тис","тоу","та" ,
	"том","тгм"
	};
	/* Greek prepositions (Ancient & Modern since they sometimes "interpolate" among texts) */
	const unsigned char *ptrPrepositions[] =
	{ "ама" , "паяа" , "йата" , "диа" , "лета" , "амти" , "алжи", "пеяи", "епи", "апо",
	"упо" , "упея" , "еис" , "ем" , "ей" , "ен" , "пяо" , "пяос", "сум", "авяи",
	"левяи" , "амеу" , "выяис" , "пкгм", "емейа", "емейем", "ла" , "мг" ,
	"емамтиом", "емамти", "емамтиа"
	};
	/* Greek atoms */
	const unsigned char *ptrAtoms[] =
	{ "ыс", "сам", "циа", "еыс", "пяотоу", "ма", "ам", "ха", "еимаи", "гтам" };
	
	int R = 0;
	/* Remove accent and uppercase S to ptrS */
	char *ptrS = strnoaccent(S);
	/* Count Word Freq */
	R += isstr(ptrS, (unsigned char**)ptrPronouns, sizeof(ptrPronouns)/sizeof(char*));
	R += isstr(ptrS, (unsigned char**)ptrArticles, sizeof(ptrArticles)/sizeof(char*));
	R += isstr(ptrS, (unsigned char**)ptrPrepositions, sizeof(ptrPrepositions)/sizeof(char*));
	R += isstr(ptrS, (unsigned char**)ptrAtoms, sizeof(ptrAtoms)/sizeof(char*));
	/* Free local resources */
	free(ptrS);
	/* Return. */
	return R;
}
/* Return 1 if S match any of *Data else 0 (on error -1) */
int isstr(char *S, unsigned char **Data, int nrSize)
{
	int R = 0;
	/* Anything to process? */
	if(S != NULL && strlen(S))
	{
		int Count;
		unsigned char *ptrS = NULL;
		/* Remove any accent from S and capitalize it */
		if((ptrS = strnoaccent(S)) == NULL)
		return -1; /* Memory error? */
		/* Search abbrevation table.. */
		for(Count = 0; Count < nrSize; Count++)
		if(!strcmp(ptrS, Data[Count])) /* Anything? */
		{
			R = 1;
			break;
		}
		/* Free local resources */
		free(ptrS);
	}
	return R; /* Not a known Greek abbrevation */
}

/* Return 1 if S is an abbreviated Greek word else 0 (on error -1) */
int isabbrevword(const char *S)
{
	int R = 0;
	/* Anything to process? */
	if(S != NULL && strlen(S))
	{
		int R;
		/* Most common Greek abbrevations */
		const unsigned char *ptrAbbrev[] =
		{ /*"й.й.", "йкп", "йтк", "л.в.", "п.в.", "й.а.", "й.", "*/
		"аццк." , "аеяомаут." , "ахк." , "аицупт." , "аит." , "аитиок.",
		"айк." , "акб." , "акк." , "амадипк.", "амадя." , "амак." ,
		"амапт." , "амасукк." , "амат." , "аматок." , "амаж." , "амхя." ,
		"амхяыпок.", "амяхыпым." , "амол." , "амт." , "амтд." , "амтих." ,
		"амтий." , "амтилетах.", "амтым.", "аоя." , "апаяелж." , "апаяв." ,
		"апкок." , "апкоп." , "апоб." , "апод." , "апогвгяоп.", "апох." ,
		"апок." , "апяос." , "аяаб." , "аяал." , "аяхя." , "аяихлт.",
		"аяйтийок.", "аялем." , "аям." , "аямгт." , "аяс." , "аяв." ,
		"аяс." , "дот." , "дыя." , "ебя." , "ехм." , "ехмок." ,
		"еид." , "еидйот." , "еия." , "ей." , "еййк." , "ейжя." ,
		"еккгм." , "екмст." , "ель." , "ем." , "емакк." , "емеяц." ,
		"емест." , "емм." , "енайок.","енек." , "енолак." , "епаццекл.",
		"епамак." , "епейт." , "епидя.", "епих." , "епияя." , "епис." ,
		"епист." , "епитат." , "епиж." , "епым." , "еяяимоп." , "еяыт." ,
		"етул." , "еуж." , "фыцяж.", "фыок." , "гх." , "гкейтяок.",
		"гкейтяом.", "глиж." , "гв." , "гвгя." , "гвгяоп." , "гволил.",
		"х." , "хеатя." , "хеок." , "хет." , "хгк." , "хягсйеиок.",
		"иапым." , "иатя." , "ид." , "лтжяд." , "лтв." , "лух.",
		"л.в." , "маут." , "меоекк.","меок." , "меот." , "мкат.",
		"мол." , "мот." , "оий." , "оийод." , "оийок." , "оийом.",
		"оккамд." , "ол." , "ом." , "омол." , "опт." , "ояхоця.",
		"ояист." , "ояуйт." , "оуд." , "оус." , "оусиастийоп.","пах.","паид." , 
		"пак." , "пакаиомт.","пакаиот.", "пая." ,"паяац.",
		"паяак." , "паяакк." , "паяетул.", "жя." , "пеятул." ,"паяыв.",
		"п.д." , "пеяик." , "пеяс." , "пкгх." , "пкгяож." ,"под.",
		"поигт." , "пок." , "поккапк.","поятоцак.","пос." ,"пяб.",
		"пяц." , "пяй." , "пяоек.", "пяогц." , "пяох." ,"сумтек.",
		"сумтолоця.","св." , "свгл." , "т." , "тайт." ,"тек." ,
		"тевм." , "тевмок." , "тгкеоя.","т.л." , "тосй." ,"тоуяй." ,
		"тяоп." , "тяопоп." , "тсицц." ,"туп." , "убя." ,"уп." ,
		"упея." , "упеях." , "упеяс." ,"упох." , "упойоя." ,"аяваиок.",
		"аявит." , "астяок." , "астяом.","ат." , "атт." ,"аутоп.",
		"ажгя." , "ожол." , "аь." ,"беб." , "бем." ,"биок." ,
		"биовгл." , "бк." , "бкав." ,"боя." , "бот." ,"боукц.",
		"цакк." , "цемийот." , "цемоб." ,"цеял." , "цеыяц." ,"цеык.",
		"цеыл." , "цеып." , "цк." ,"цкупт." , "цкысс." ,"цмыл.",
		"цяалл." , "цулм." , "дам." ,"деийт." , "дгк." ,"дглот.",
		"диайя." , "диак." , "диакейт.","диасп." , "диаж." ,"диав." ,
		"диех." , "дий." , "дист." , "дижх." , "дижхоццоп." ,"имд." ,
		"испам." , "ист." , "исвуяоп.","итак." , "иым." ,"й.а." ,
		"йатак." , "йатат." , "й.д." , "й.е." , "йимгл." ,"йкгт.",
		"йкит." , "йоимым." , "й.о.й." , "йп." , "йт." ,"йтц." ,
		"йтгт." , "йтк." , "йуя." , "йуяиок.", "каий." ,"каийотя.",
		"каоця." , "кат." , "коц." , "коцист.", "коцот." ,"лацеия.",
		"лахгл." , "лецех." , "лее." , "л.л.е." , "леиыт." ,"лекк.",
		"лесож." , "летах." , "летай." , "летапк.", "летая." ,"летепих.",
		"летепияя.", "летеыя." , "летом." , "летоус.", "летя." ,"лгв.",
		"лгвамок." , "лояжок." , "лоус." , "лпе." , "лпп." ,"лсм.",
		"лсмкат." , "лтж." , "пяопаяак.","пяос." , "пяосаял." ,"пяост.",
		"пяосжым." , "пяот." , "пяотайт.","пяовыя.", "пяож." ,"пят.",
		"пт." , "п.в." , "яглат." ,"ягтоя." , "яим." ,"яоул.",
		"яыс." , "самсйя." , "сек." ,"сгласиок.","сглд." ,"сглея.",
		"сглит." , "сйыпт." , "скаб." ,"спам." , "спамиот." ,"стат.",
		"стея." , "стицл." , "стяат." ,"суцц." , "суцйя." ,"сулпея.",
		"сулпк." , "сулпкейт." , "сулпяож.","сулпт." , "сулж." ,"сулжуя.",
		"сум." , "сумаия." , "сумаисх.","сумд." , "сумейд." ,"сумгх.",
		"сумгя." , "сумх." , "сумиф." ,"сумопт.", "сумт." ,"упойыя.",
		"упок." , "упот." , "уповыя." ,"усткат.", "жаял." ,"жикок.",
		"жикос." , "жикот." , "жоимий." ,"жя." , "жус." ,"жусиок.",
		"жым." , "жымгемт." , "жымгт." ,"жымок." , "жытоцяж." ,"васл." ,
		"вцж." , "веик." , "вгл." ,"вкеу." , "вя." ,"вяом." ,
		"ьув." , "ьувам." , "ьувиатя.","й." , "й.й" ,"йа." ,
		"йос." , "йй." , "цем.", "д.", "еуац.", "лив.", "иыам.", "ц.",
		"жик.", "дгл.", "бас.", "йым.", "амдя."
		};
		#ifdef _DEBUG
		int D = isstr(S, ptrAbbrev, sizeof(ptrAbbrev) / sizeof(char*));
		fprintf(stderr, "AbbrevWord Input: \"%s\"\n", S);
		fprintf(stderr, "AbbrevWord Ret. : %d\n", D);
		return D;
		#else
		/* Match abbrevation */
		return isstr((char*)S, (unsigned char**)ptrAbbrev, sizeof(ptrAbbrev) / sizeof(char*));
		#endif
	}
	/* Return. */
	return 0;
}

/*********************** Sentence handling functions **************************/
/* Store Contents from file into a Buffer. */
char *LoadFileContents(const char *filename, char *Contents)
{
	/* Filename Provided. */
	if(!filename)
	{
		fprintf(stderr,"{NULL} file name:%s\n", strerror(errno));
		return NULL;
	}
	else
	{
		/* File Pointer. */
		FILE *file = NULL;
		/* Buffer. */
		char FileBuffer[BUFSIZ] = "";
		int nLen = 0, Index = 0;
		/* Test for file operation. */
		if((file = fopen(filename,"rt")) == NULL)
		{
			fprintf(stderr,"Open Error: Can not open %s file: %s\n", filename,
			strerror(errno));
			return NULL;
		}
		for(;;)
		{
			if((fgets(FileBuffer, BUFSIZ, file)) == NULL)
				break;
			/* Now trim it. */
			if(FileBuffer[strlen(FileBuffer)-1] == '\n')
				FileBuffer[strlen(FileBuffer)-1] = '\0'; /* Not trim the '\n' but replace it with ' '. */
			/* Test if zero length. */
			if(!strlen(FileBuffer))
				continue;
			/* In each iteration. */
			if((Contents = (char*)realloc(Contents, (nLen += strlen(FileBuffer)+1) * sizeof(char))) == NULL)
			{
				fprintf(stderr,"Buffer Memory Reallocation Fault:%s\n", strerror(errno));
				fclose(file);
				return NULL;
			}
			/* Put into the Contents the line read or append it. */
			!Index ? strcpy(Contents, FileBuffer) : strcat(Contents,FileBuffer); Index++;
			memset(FileBuffer, 0, sizeof(FileBuffer));
		}
		/* Close file. */
		fclose(file);
		/* Return. */
		return Contents;
	}
}

/* Splits the input buffer that hold the file contents into an array of valid
* sentences. The number of sentences stored is returned as an extra parameter.
* Any Error will be reported to *Error variable.
*/
Sentence *GetTextSentences(const char *FileContents, int *nrSentences, int *Error)
{
	if(!FileContents)
	{
		fprintf(stderr,"GetTextSentences:{NULL} file contents:%s\n", strerror(errno));
		return NULL;
	}
	else
	{
		/* Index for our structure array. */
		int Index = 0, i = 0;
		int LetterCount = 0;
		int nLen = 0, skippedChars = 0;
		/* Default start point. */
		int startPoint = 0;
		/* Sentence structure to be returned. */
		Sentence *s = NULL;
		/* Pointer to next of the next character. */
		unsigned char *ptrToNextNextChar = NULL;
		unsigned char *ptrToPreviousPreviousChar = NULL;
		unsigned char *oldStatePtr = NULL;
		unsigned char *next = NULL;
		unsigned char *previous = NULL;
		unsigned char *ptrToChar = (char*)FileContents;
		/* Test for abrevword. */
		unsigned char *Abrev = NULL;
		unsigned char AbrevWord[BUFSIZ] = "";
		/* Init Error to zero. */
		*Error = 0;
		/* Do we have any trailing whitespace chars in the first line. */
		while(isspace((int)*ptrToChar))
		{
			skippedChars++;
			ptrToChar++;
		}
		/* Main Loop. */
		for(i = 0; i < (int)strlen(FileContents); i++)
		{
			/* Store character from file. */
			unsigned char Char = FileContents[i];
			/* Get the next char. */
			ptrToChar = (char*)&FileContents[i];
			Abrev = (char*)&FileContents[i];
			next = i < (int)strlen(FileContents)-1 ? ptrToChar+1 : NULL;
			previous = i > 1 ? ptrToChar-1 : NULL;
			/* Check two letters farther. */
			ptrToNextNextChar = i < (int)strlen(FileContents)-2 ? ptrToChar+2 : NULL;
			/* Check two letters before. */
			ptrToPreviousPreviousChar = i > 2 ? ptrToChar-2 : NULL;
			if(Char == '.')
			{
				/*Advance once to get to the next char. */
				ptrToChar++;
				/* Save oldstate. */
				oldStatePtr = ptrToChar;
				/* Check two places front or two places before for another dot. */
				if(ptrToNextNextChar != NULL && *ptrToNextNextChar == '.' || ptrToPreviousPreviousChar != NULL && *ptrToPreviousPreviousChar == '.')
					continue;
				if(next != NULL && isdigit((int)*next) && previous != NULL && isdigit((int)*previous))
					continue;
				/* Test for abrevword. */
				if(ptrToNextNextChar != NULL && *ptrToNextNextChar == '.')
					Abrev = ptrToNextNextChar;
				while(!isspace((int)*Abrev))
				{
					LetterCount++;
					Abrev--;
				}
				/* Advance to skip the space. */
				if(isspace((int)*Abrev))
					Abrev++;
				/* Copy to buffer the string. */
				strncpy(AbrevWord, Abrev, LetterCount);
				AbrevWord[LetterCount] = '\0';
				if(isabbrevword(AbrevWord))
				{
					LetterCount = 0;
					memset(AbrevWord, 0 , sizeof(AbrevWord));
					continue;
				}
				LetterCount = 0;
				/* We have a valid dot here. */
				while(isspace(((int)*ptrToChar)))
					ptrToChar++;
				/* We have skipped unwanted characters till the first valid letter. */
				if(issemantictoken((char*)ptrToChar) || *ptrToChar == '\0')
				{
					/* Allocate a sentence. */
					/* Get space for the new sentence. */
					if((s = realloc(s,(Index+1)*sizeof(Sentence))) == NULL)
					{
						fprintf(stderr,"GetTextSentences:Memory Reallocation Fault:%s\n",
						strerror(errno));
						return NULL;
					}
					/* If we carry skipped chars from the begining. Calculate the sentence length. */
					nLen = i - startPoint - skippedChars + 1;
					/* Get space for the sentence to get stored. */
					if((s[Index].Str =(char*)calloc(nLen+1, sizeof(char))) == NULL)
					{
						fprintf(stderr,"GetTextSentences:Allocation Fault:%s\n",
						strerror(errno));
						*Error = 1;
						break;             
					}
					/* Save sentence. */
					strncpy(s[Index].Str,&FileContents[startPoint+skippedChars], nLen);
					s[Index].Str[nLen] = '\0';
					s[Index].Score = 0;
					/* Advance and set new indexes values. */
					Index++;
					startPoint = i+1;
					/* Reset skipped chars. */
					skippedChars = 0;
					/* Calculate skipped characters. */
					ptrToChar = oldStatePtr;
					while(isspace((int)*ptrToChar))
					{
						skippedChars++;
						ptrToChar++;
					}
				}
				/* We did not find any Cap letter. */
				else
				{
					fprintf(stderr,"GetTextSentences: Capital letter was not found after the dot. Invalid input.\n");
					*Error = 1;
					break;
				}
			}
		}
		/* Exit loop. */
		/* Error, Clean the mess. */
		if(*Error)
		{
			for(Index = Index-1; Index >=0; Index--)
			{
				/* There must be a sentence to free it. */
				if(s)
				{
					if(s[Index].Str)
					{
						free(s[Index].Str);
						s[Index].Str = NULL;
					}
				}
			}
			/* Free the whole sentence array. */
			if(s)
			{
				free(s);
				s = NULL;
				*nrSentences = 0;
				return NULL;
			}
		}
		/* Case no Error reported return safely. */
		*nrSentences = Index;
		return s;
	}
}
/* Fill a Tokeniser Structure with the content - words from the sentence,
* this structure will be used to find the correct score of the sentence
* according to the criteria used.
*/
Tokeniser *FillTokeniserBySentence(char *inputSent, int ignoreCharacters,const char *delim, int *Tokens)
{
	if(!inputSent)
	{
		fprintf(stderr,"FillTokeniserBySentence:{NULL} input sentence:%s\n", strerror(errno));
		return NULL;
	}
	else
	{
		/* Index counter. */
		int Index = 0, TokenIndex = 0, pointerIndex = 0;
		/* Pointer to token. */
		char *pToken = NULL;
		char *inputSentDup = mystrdup(inputSent);
		/* Tokeniser return value. */
		Tokeniser *t = NULL;
		/* Test for token with strtok. */
		if(!(pToken = strtok(inputSentDup, delim == NULL ? " " : delim)))
		{
			/* Case one token and less than 3 chars no need to save anything. */
			if((int)strlen(inputSentDup) < ignoreCharacters)
				return NULL;
			/* Return the whole sentence, must be a word. */
			if(!(t = calloc(Index+1, sizeof(Tokeniser))))
			{
				fprintf(stderr,"FillTokeniserBySentence: Memory Allocation Fault: %s\n", strerror(errno));
				return NULL;
			}
			else
			{
				if(!(t->token = calloc(Index, sizeof(char*))))
				{
					fprintf(stderr,"FillTokeniserBySentence: Memory Pointer String Allocation Fault: %s\n", strerror(errno));
					free(t); t = NULL;
					return NULL;
				}
				if(!(t->token[TokenIndex] = calloc(strlen(inputSentDup)+1, sizeof(char))))
				{
					fprintf(stderr,"FillTokeniserBySentence: Memory String Allocation Fault: %s\n", strerror(errno));
					free(t->token); t->token = NULL;
					free(t); t = NULL;
					return NULL;
				}
				/* Save the token and return safely. */
				else
				{
					strcpy(t->token[TokenIndex], inputSentDup);
					t->tokencount = TokenIndex+1;
					/* Tokens. */
					*Tokens = TokenIndex+1;
					return t;
				}
			}
		}
		else
		{
			/* Get memory for the structure. */
			/* Return the whole sentence, must be a word. */
			if(!(t = calloc(Index+1, sizeof(Tokeniser))))
			{
				fprintf(stderr,"FillTokeniserBySentence: Memory Allocation Fault:%s\n", strerror(errno));
				return NULL;
			}
			/* Loop to get the tokens, ignore small non special ones. */
			/* Code can be replaced with small tokens specified by the user, should check. */
			do
			{
				/* Ignore. */
				if((int)strlen(pToken) < ignoreCharacters)
					continue;
				/* Get the pointer. */
				if(!(t->token = realloc(t->token,(pointerIndex+1)*sizeof(char*))))
				{
					fprintf(stderr,"FillTokeniserBySentence: Memory Pointer String Allocation Fault: %s\n", strerror(errno));
					free(t); t = NULL;
					return NULL;
				}
				/* Get space for the token. */
				if(!(t->token[pointerIndex] = calloc((int)strlen(pToken)+1, sizeof(char))))
				{
					fprintf(stderr,"FillTokeniserBySentence: Memory String Allocation Fault: %s\n", strerror(errno));
					for(Index = 0; Index < pointerIndex+1; Index++)
					{
						if(t->token[Index])
						{
							free(t->token[Index]);
							t->token[Index] = NULL;
						}
					}
					free(t); t = NULL;
					return NULL;
				}
				/* Copy the token into the tokeniser. */
				else
				{
					strcpy(t->token[pointerIndex], pToken);
					/* Advance pointers. */
					pointerIndex++;
				}
			}while((pToken = strtok(NULL, delim == NULL ? " " : delim)) != NULL);
		}/* End for else. */
		/* Return. */
		*Tokens = pointerIndex;
		t->tokencount = pointerIndex;
		free(inputSentDup);
		return t;
	}
}

/* Free a Sentences array */
void FreeTextSentences(Sentence *S, const int nrSentences)
{
	/* Anything to process? */
	if(S != NULL)
	{
		int Count;
		/* Free each S item */
		for(Count = 0; Count < nrSentences; Count++)
			free(S[Count].Str);
		/* Free S it self */
		free(S); S = NULL;
	}
}

/* Free a Tokeniser structure from memory. */
void FreeTokeniser(Tokeniser *t)
{
	if(t)
	{
		int Index = 0;
		for(Index = 0; Index < t->tokencount; Index++)
		{
			if(t->token[Index])
			{
				free(t->token[Index]);
				t->token[Index] = NULL;
			}
		}
		free(t->token); t->token = NULL;
		free(t); t = NULL;
	}
}

/****************************** Database Functions ****************************/
/* qsort function for Name database functions */
int _DBQCmp(const void *A, const void *B)
{
	NameEntry *ptrA = (NameEntry*)A,
	*ptrB = (NameEntry*)B;
	return strcmp(ptrA->Name, ptrB->Name);
}
/* Return 1 if Name exists in the database Container records else 0, on error -1 */
int IsDBStr(NameEntry *Container, const int nrWords, const char *Name)
{
	/* Anything to process? */
	if(Container != NULL && nrWords > 0)
	{
		NameEntry Key;
		/* Duplicate Name into as a NameEntry key */
		if((Key.Name = mystrdup((char*)Name)) != NULL)
		{
			/* Store bsearch result to ptrR */
			char *ptrR = bsearch(&Key, Container, nrWords, sizeof(NameEntry), _DBQCmp);
			/* Free local resources */
			free(Key.Name); Key.Name = NULL;
			/* Return bsearch result */
			return ptrR != NULL;
		}
		else
		return -1;
	}
	else
	return 0;
}

/* Works like IsDBStr but accepts Tokeniser* instead of const char* (-1 on error) */
int IsDBToken(NameEntry *Container, const int nrWords, Tokeniser *Tok)
{
	int TokCount,
	Count = 0;
	char *ptrNoAccTok = NULL;
	/* Iterate Tokeniser->token char* and if token is a person-name increase count by 1 */
	for(TokCount = 0; TokCount < Tok->tokencount; TokCount++)
	{
		/* Remove accents and capitalize Tok->token */
		if((ptrNoAccTok = strnoaccent(Tok->token[TokCount])) == NULL)
		return -1;
		/* Is Tok->token in database container? */
		if(IsDBStr(Container, nrWords, ptrNoAccTok))
		Count++; /* Yes, increase it's count */
		/* Free local resources */
		free(ptrNoAccTok); ptrNoAccTok = NULL;
	}
	/* Return found person-names */
	return Count;
}

/* Free NameEntry* */
void FreeDB(NameEntry *Container, const int nrWords)
{
	if(Container != NULL)
	{
		int Count;
		for(Count = 0; Count < nrWords; Count++)
		free(Container[Count].Name);
		free(Container); Container = NULL;
	}
}

/* Append Word to a in-memory NameEntry DB Container */
NameEntry *InsertWordToMemDB(NameEntry *Container, char *Word, int *nrWords, int *Error)
{
	/* No error */
	*Error = 0;
	/* Anything to process? */
	if(Word != NULL && strlen(Word))
	{
		int Count;
		NameEntry Key;
		char *ptrNoAccentWord = NULL;
		/* Calculate Container Length */
		int ContainerLen = *nrWords? (*nrWords + 1) * sizeof(NameEntry): sizeof(NameEntry);
		/* Remove Word accent & uppercase it */
		if((ptrNoAccentWord = strnoaccent(Word)) == NULL)
		{
			*Error = 1;
			return Container;
		}
		/* Is Container non-empty? */
		if(Container != NULL && *nrWords)
		/* Conduct linear search before append word */
		for(Count = 0; Count < *nrWords; Count++)
		if(!strcmp(Container[Count].Name, ptrNoAccentWord))
		{
			/* Special case - word exists */
			*Error = 2;
			/* Free local resources */
			free(ptrNoAccentWord);
			/* Return. */
			return Container;
		}
		/* Grow database memory container */
		if((Container = (NameEntry*)realloc(Container, ContainerLen)) == NULL)
		{
			*Error = 1;
			perror("InsertWordToMemDB:realloc");
		}
		else
		{
			/* Allocate a large enough buffer to store database record */
			if((Container[*nrWords].Name = (char*)calloc(strlen(ptrNoAccentWord) + 1, sizeof(char))) == NULL)
			{
				*Error = 1;
				perror("InsertWordToMemDB:calloc");
			}
			/* Append Word to Container */
			strcpy(Container[*nrWords].Name, ptrNoAccentWord);
			/* Increase Word count */
			*nrWords += 1;
		}
		/* Free local resources */
		free(ptrNoAccentWord);
	}
	return Container;
}

/* Sort Container, on error return 0 else 1 */
int SortMemDB(NameEntry *Container, const int nrWords)
{
	/* Anything to process? */
	if(Container != NULL && nrWords)
	{
		/* Q-Sort! */
		qsort(Container, nrWords, sizeof(NameEntry), _DBQCmp);
		/* Ok! */
		return 1;
	}
	/* Return. */
	return 0;
}

/* Return 1 on success else 0 on error */
/* We do not count Greek Articles & Numerics */
int SentenceWordScore(Sentence *s)
{
	/* Anything to process? */
	if(s != NULL && strlen(s->Str))
	{
		int nrTok;
		/* Tokenise s */
		Tokeniser *ptrTok = FillTokeniserBySentence(s->Str, 3, DELIM, &nrTok);
		/* Any token? */
		if(ptrTok != NULL)
		{
			int nrCount;
			/* Reset nrTok */
			nrTok = 0;
			/* Count tokens excluding numerics */
			for(nrCount = 0; nrCount < ptrTok->tokencount; nrCount++)
			{
				/* Is Numeric? */
				if(!isnumeric(ptrTok->token[nrCount]))
					nrTok++;
			}
		}
		else
			return 0; /* No tokens found! */
		/* Is nrTok >= 12? */
		if(nrTok > 12)
		s->Score -= 7; /* Yes! Decrease sentence score! (was 10)*/
		/* Free local resources */
		FreeTokeniser(ptrTok);
	}
	/* Return. */
	return 1;
}

/* Return 1 on success else 0 on error */
int SentenceProperNamesScore(Sentence *s)
{
	/* Anything to process? */
	if(s != NULL && strlen(s->Str))
	{
		int TokLen, TokCount;
		Tokeniser *ptrTok = NULL;
		/* Tokenise s to ptrTok */
		if((ptrTok = FillTokeniserBySentence(s->Str, 4, DELIM, &TokLen)) == NULL)
			return 0;
		for(TokCount = 0; TokCount < TokLen; TokCount++)
		{
			/* Load ptrTok->token[TokCount] into ptrToken for easy reference */
			char *ptrToken = ptrTok->token[TokCount];
			/* First Letter must be Capital and not the First Token */
			if(TokCount && issemantictoken(ptrToken))
			{
				int CharCount,
				TrueSemantic = 1;
				/* Check if rest characters are lowercased.. */
				for(CharCount = 1; CharCount < (int)strlen(ptrToken); CharCount++)
				{
					if(issemantictoken(&ptrToken[CharCount]))
					{
						TrueSemantic = 0;
						break;
					}
				}
				/* ..are? */
				if(!TrueSemantic)
					continue; /* No! Ignore it */
				/* Could it be a day or a month name? */
				if(isday(ptrToken) || ismonth(ptrToken))
					continue; /* Yes, ignore it */
				/* The Letter conforms with "Proper Names" rule */
				s->Score += 5;
				/* For debugging */
				#ifdef _DEBUG
				printf("S:%s\n", ptrToken);
				#endif
			}
		}
		/* Free local resources */
		FreeTokeniser(ptrTok);
	}
	/* Return. */
	return 1;
}

/* +5 for sentences containg 1 numeric data, -5 for sentences with 2+ numeric data */
/* Return 1 on success 0 on error */
int SentenceNumericDataScore(Sentence *s)
{
	/* Anything to process? */
	if(s != NULL && strlen(s->Str))
	{
		Tokeniser *ptrTok = NULL;
		int TokSize, TokCount = 0, NumericDataCount = 0;
		/* Tokenise s to ptrTok */
		if((ptrTok = FillTokeniserBySentence(s->Str, 4, DELIM, &TokSize)) == NULL)
			return 0;
		/* Count Tokeniser numeric data */
		for(TokCount = 0; TokCount < TokSize; TokCount++)
			if(isnumeric(ptrTok->token[TokCount]))
				NumericDataCount++;
		/* Free local resource */
		FreeTokeniser(ptrTok);
		/* Score estimation */
		if(NumericDataCount == 1)
		s->Score += 5;
		if(NumericDataCount >= 2)
		s->Score -= 5;
		/* Return success. */
		return 1;
	}
	/* Return failure. */
	return 0;
}

/* +5 for sentences with day or dates */
int SentenceDateScore(Sentence *s)
	{
	/* Anything to process? */
	if(s != NULL && strlen(s->Str))
	{
			Tokeniser *ptrTok = NULL;
		int TokSize, TokCount = 0, NumericDataCount = 0;
		/* Tokenise s to ptrTok */
		if((ptrTok = FillTokeniserBySentence(s->Str, 3, DELIM, &TokSize)) == NULL)
			return 0;
		/* Count Tokeniser numeric data */
		for(TokCount = 0; TokCount < TokSize; TokCount++)
		{
			/* Do I deal with numeric dates? */
			if(isnumeric(ptrTok->token[TokCount]))
				continue; /* Skip, I deal with literal dates! */
			/* No! Could it be a abbrevated year token? */
			if(isabbrevyear(ptrTok->token[TokCount]))
			{
				/* Yes it is a year abbrevation! */
				#ifdef _DEBUG
				printf("AbbrevYear: %s\n", ptrTok->token[TokCount]);
				#endif
				NumericDataCount = 1;
				break;
			}
			/* No! Could it be a month token? */
			if(ismonth(ptrTok->token[TokCount]))
			{
				/* Yes it is a month token */
				#ifdef _DEBUG
				printf("MonthTok: %s\n", ptrTok->token[TokCount]);
				#endif
				NumericDataCount = 1;
				break;
			}
			/* No! Could it be a day token? */
			if(isday(ptrTok->token[TokCount]))
			{
				/* Yes it is a day token */
				#ifdef _DEBUG
				printf("DayTok: %s\n", ptrTok->token[TokCount]);
				#endif
				NumericDataCount = 1;
				break;
			}
		}
		/* Free local resource */
		FreeTokeniser(ptrTok);
		/* Score estimation */
		if(NumericDataCount >= 1)
		s->Score += 5;
		/* Return success. */
		return 1;
	}
	/* Return failure. */
	return 0;
}

/* Search s tokens for Container tokens and if found increase s->Score by nAddScore */
/* Return 1 if ok, else 0 */
int SentenceWordRepetition(Sentence *s, const int nAddScore, NameEntry *Container, const int nrContainerLen)
{
	/* Anything to process? */
	if(s != NULL && strlen(s->Str) && Container != NULL && nrContainerLen)
	{
		int nrTokCount;
		/* Tokenise s */
		Tokeniser *ptrTok = FillTokeniserBySentence(s->Str, 3, DELIM, &nrTokCount);
		/* Error? */
		if(ptrTok == NULL)
		{
			/* Yes! */
			perror("SentenceWordRepetition:FillTokeniserBySentence");
			return 0;
		}
		/* Search ... */
		if((nrTokCount = IsDBToken(Container, nrContainerLen, ptrTok)) == -1)
		{
			/* Error! */
			FreeTokeniser(ptrTok);
			perror("SentenceWordRepetition:IsDBToken");
			return 0;
		}
		/* Anything found? */
		if(nrTokCount)
			s->Score += nAddScore; /* Yes! Increase score! */
		/* Free local resources */
		FreeTokeniser(ptrTok);
	}
	/* Return. */
	return 1;
}

/* Setup a memory database with words that appear in text body more than 5 times */
NameEntry *MakeWordsProfile(char *ptrEntireText, const int nrMin, const int nrMax, NameEntry *Container, int *nrContainerLen)
{
	/* Anything to process? */
	if(ptrEntireText != NULL && strlen(ptrEntireText))
	{
		int nrTokLen, nrProgress = 0;
		/* Tokenise ptrEntireText (ignore tokens less than 4 characters) */
		Tokeniser *ptrTok = FillTokeniserBySentence(ptrEntireText, 4, DELIM, &nrTokLen);
		/* Sanity check for nrMin - nrMax */
		if(nrMin > nrMax || nrMax < nrMin || nrMin < 0 || nrMax < 0)
		{
			perror("MakeWordsProfile:Invalid nrMin/nrMax!");
			FreeTokeniser(ptrTok);
			return Container;
		}
		/* Ready? */
		if(ptrTok != NULL)
		{
			int nrWordCount,
			nrTokCount = 0,
			nrError = 0;
			char *ptrActiveWord = NULL;
			for(nrTokCount = 0; nrTokCount < ptrTok->tokencount; nrTokCount++)
			{
				int nrWordCount = 0,
				nrTokCountB,
				nrRuleAction = 0;
				/* Is a word or an numeric? */
				if(isnumeric(ptrTok->token[nrTokCount]))
				{
					#ifdef _DEBUG
					printf("Word Profiler numeric skip [%d]:\"%s\"\n", nrTokCount, ptrTok->token[nrTokCount]);
					#endif
					continue; /* It is numeric, skip it */
				}
				/* Convert ptrActiveWord to no accent & uppercase */
				ptrActiveWord = strnoaccent(ptrTok->token[nrTokCount]);
				/* Failure? */
				if(ptrActiveWord == NULL)
				{
					perror("\nMakeWordProfile:strnoaccent:ptrActiveWord");
					break;
				}
				/* Count ptrActiveWord appearance in ptr1Tok->tokens.. */
				for(nrTokCountB = 0; nrTokCountB < ptrTok->tokencount; nrTokCountB++)
				{
					/* Convert ptrTestWord to no accent & uppercase */
					char *ptrTestWord = strnoaccent(ptrTok->token[nrTokCountB]);
					/* Failure? */
					if(ptrTestWord == NULL)
					{
						perror("\nMakeWordProfile:strnoaccent:ptrTestWord");
						nrError = 1;
						break;
					}
					if(!strcmp(ptrActiveWord, ptrTestWord)) /*ptrTok->token[nrTokCountB]())*/
						if(nrTokCount != nrTokCountB)
							nrWordCount++;
					/* Free local resources */
					free(ptrTestWord); ptrTestWord = NULL;
				}
				/* Failure on nested for? */
				if(nrError)
					break; /* Yes, break! */
				/* Based on nrMin & nrMax decide what to do with nrWordCount */
				if(nrMin == nrMax)
					if(nrWordCount >= nrMax)
						nrRuleAction = 1;
				if(nrWordCount >= nrMin && nrWordCount <= nrMax)
					nrRuleAction = 1;
				/* Is Word frequency equal or more than nrMin - nrMax rule */
				if(nrRuleAction && !islowergramtoken(ptrActiveWord))
				{
					int nrError;
					/* Yes, store it to DBContainer for further use latter */
					#ifdef _DEBUG
					printf("Word Profiler [%d]:\"%s\"\n", nrWordCount, ptrActiveWord);
					#endif
					Container = InsertWordToMemDB(Container, ptrActiveWord, nrContainerLen, &nrError);
					/* Ok? */
					if(nrError && nrError != 2)
					{
						/* Error! */
						perror("\nMakeWordProfile:InsertWordToMemDB");
						free(ptrActiveWord);
						FreeDB(Container, *nrContainerLen);
						return NULL;
					}
				}
				/* Free local resources */
				free(ptrActiveWord); ptrActiveWord = NULL;
				/* Update progress indicator */
				if(++nrProgress >= 12)
				{
					nrProgress = 0;
					fprintf(stderr, ".");
				}
			}
			/* Free local resources */
			FreeTokeniser(ptrTok);
		}
		else
		perror("\nMakeWordsProfile:FillTokeniserBySentence");
	}
	/* Return. */
	return Container;
}

/* +5 for sentences containg proper-quotes (".." or <<..>>) inside */
int SentenceScoreQuotes(Sentence *s)
{
int R = 1;
	/* Anything to process? */
	if(s != NULL)
	{
		int nrQuoteOpen = 0,
		nrQuoteFound= 0,
		nrCount;
		/* Duplicate s to ptrS */
		char *ptrS = mystrdup(s->Str),
		*ptrQ = NULL;
		/* mystrdup failure? */
		if(ptrS == NULL)
			return 0; /* Failed! */
		/* Transform Greek quotes (<< & >>) to English ones (") */
		for(;;)
		{
			int nQPos;
			if((ptrQ = strstr(ptrS, "<<")) != NULL)
			{
				nQPos = ptrQ - ptrS;
				ptrS[nQPos] = ' ';
				ptrS[nQPos + 1] = '\"';
			}
			else
				if((ptrQ = strstr(ptrS, ">>")) != NULL)
				{
					ptrS[ptrQ - ptrS] = '\"';
					ptrS[(ptrQ - ptrS) + 1] = ' ';
				}
			else
				break;
		}
		/* Verify Quotes matching */
		for(nrCount = 0; nrCount < strlen(ptrS); nrCount++)
		if(ptrS[nrCount] == '\"')
		{
			nrQuoteOpen = !nrQuoteOpen;
			/* Signal that a Quote symbol has been found */
			nrQuoteFound= 1;
		}
		/* Any Quote found? */
		if(nrQuoteFound)
		{
			/* Unclosed quote? (Quote is sentence-wide) */
			if(!nrQuoteOpen)
			{
				/* Increase sentence score by 5 */
				s->Score += 5;
			}
		}
		/* Free local resources */
		free(ptrS);
	}
	/* Return. */
	return R;
}

/* nrScore for sentences in the begining of the text (denote while nrOrder <= nrReckoning) */
void SentenceImportance(Sentence *s, const int nrScore, const int nrOrder, const int nrReckoning)
{
	if(nrOrder <= nrReckoning)
    s->Score += nrScore;
}			

