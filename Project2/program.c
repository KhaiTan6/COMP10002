// Name: Khai Syuen Tan
// ID: 1190030

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define MAXWORDS 100       // The maximum number of words in the dictionary.
#define FORMLETTERS 25     // Maximum number of letters in a variation form.
#define ROOTLETTERS 22     // Maximum number of letters in a root word.
#define MAXPOSTAG 5        // Maximum number of pos tag per word.
#define POSTAGLETTERS 4    // Maximum number of letters per pos tag.
#define WHITESPACES 4      // Blank spaces between pos tags.
#define MAXFORMS 4         // Maximum number of forms per word.
#define FORMS_NUM 4        // A Number that seperates each form.

typedef struct{
    char root[ROOTLETTERS];
    char POS[MAXPOSTAG * POSTAGLETTERS + WHITESPACES];
    char form[MAXFORMS * FORMLETTERS + FORMS_NUM];
} word_t[MAXWORDS];

typedef struct node node_t;

struct node {
	char data[FORMLETTERS];
	node_t * next;
};

typedef struct {
	node_t * head;
	node_t * foot;
} list_t;

//-------------------- prototypes --------------------//
int create_dict(word_t dict);
void print_stage_1(word_t dict);
void print_stage_2(word_t dict, int word_count);
void print_stage_4(word_t dict, list_t list, int word_count);
int tot_form_count(word_t dict, int word_count);
int sentence_word_num(list_t list);
// This function below is obtained from 
//https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/getword.c
int getword(char W[], int limit);
//These functions below (line 49-51) are obtained from 
// https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/listops.c
list_t * make_empty_list(void);
list_t * insert_at_foot(list_t * list, char value[]);
void free_list(list_t * list);
int cmpfunc (const void * a, const void * b);
void POS_tagging(list_t list, word_t dict, int word_count,char * POStag[]);

int main(int argc, char * argv[]){
    word_t dictionary;
    int word_count = create_dict(dictionary);
    
    list_t * list;
    char temp[FORMLETTERS];
    list = make_empty_list();
    while(getword(temp, FORMLETTERS) != EOF){
        list = insert_at_foot(list, temp);
    }
    
    int num_word_sentence = sentence_word_num(*list);
    char* POStag[num_word_sentence];
    //POS_tagging(*list, dictionary, num_word_sentence, POStag); //(INCOMPLETE)
    
    print_stage_1(dictionary);
    print_stage_2(dictionary, word_count);
    printf("==========================Stage 3==========================\n");
    node_t * temp_node = list->head;
    for(int i = 0; i < num_word_sentence; i++){
        printf("%-26s", temp_node->data);
        printf("%s\n", POStag[i]);
        temp_node = temp_node->next;
    }
    print_stage_4(dictionary, *list, num_word_sentence);
    
    free_list(list);
    list = NULL;
}

// Takes in a empty dictionary and store values into each component:
// root,POS and form. Returns the total number of words read in.
int create_dict(word_t dict){
    int i = 0;
    char ch;
    int word_count = 0;
    while((ch = getchar()) != '*'){
        if(ch == '$'){
            scanf("%s", dict[i].root);
            word_count += 1;
            continue;
        }
        if(ch == '#'){
            scanf("%c", dict[i].form);
            if(dict[i].form[0] == '\n'){
                i++;
                continue;
            }else{
                scanf("%s", dict[i].form+1);
            }
            i++;
            continue;
        }
        if(ch == '\n'){
            continue;
        }
        else{
            dict[i].POS[0] = ch;
            scanf("%[^\n]",dict[i].POS+1);
        }        
    }
    return word_count;
}

// Borrowed function. (See prototypes above.)
// Extract a single word out of the standard input, of not
// more than limit characters. Argument array W must be
// limit+1 characters or bigger. 
int getword(char W[], int limit) {
	int c, len = 0;
	/* first, skip over any non alphabetics */
	while ((c = getchar()) != EOF && !isalpha(c)) {
		/* do nothing more */
	}
    if (c == EOF) {
        return EOF;
	}
	/* ok, first character of next word has been found */
	W[len++] = c;
	while (len<limit && (c = getchar()) != EOF && isalpha(c)) {
		/* another character to be stored */
		W[len++] = c;
	}
	/* now close off the string */
	W[len] = '\0';
	return 0;
}

// Borrowed function. (See prototypes above.)
// Creates an empty list, which contains head and foot that points to NULL.
list_t * make_empty_list(void) {
	list_t * list;
	list = (list_t *)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;
	return list;
}

// Borrowed function. (See prototypes above.)
// Takes in value, packaged it into node_t, and insert into list.
list_t * insert_at_foot(list_t * list, char value[]) {
	node_t * new;
	new = (node_t *)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	//new->data = value;
	strcpy(new->data,value);
    new->next = NULL;
	if (list->foot == NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = new;
	} else {
		list->foot->next = new;
		list->foot = new;
	}
	return list;
}

// Borrowed function. (See prototypes above.)
// Frees list by unallocating the memories.
void free_list(list_t * list) {
	node_t *curr, *prev;
	assert(list!=NULL);
	curr = list->head;
	while (curr) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}
	free(list);
}

// comparator function.
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

// --------------- incomplete function ---------------//
// searches dictionary.root for same word. If matched, copy dictionary.POS into
// POStag. else, copy in "NOT_FOUND".  
void POS_tagging(list_t list, word_t dict, int word_count,char*POStag[]){
    node_t* temp = list.head;
    int i = 0;
    char* holder;
    while(temp != NULL){
        holder = (char*)bsearch(temp->data, dict, MAXWORDS-1, 
                 sizeof(dict[0]),(int(*)(const void*, const void*))strcmp);
        temp = temp->next;
        if(holder != NULL){
            strcpy(POStag[i], dict[i].POS);
        }else{
            strcpy(POStag[i], "NOT_FOUND");
        }
        i++;
    }
}

// Takes in a list and returns the total number of words in it.
int sentence_word_num(list_t list){
    int count = 0;
    node_t* temp = list.head;
    while(temp != NULL){
        temp = temp->next;
        count += 1;
    }
    return count;
}

// Takes in dicitonary and number of words in it. Returns total number of 
// forms of all words in dictionary.
int tot_form_count(word_t dict, int word_count){
    int forms_count = 0;
    for(int i = 0; i < word_count; i++){
        int num_of_forms = 0;
        for(int j = 0; j < 104; j++){
            if(isdigit(dict[i].form[j])){
                forms_count += 1;
                num_of_forms += 1;
            }
            if(num_of_forms == 4){
                break;
            }
        }
    }
    return forms_count;
}

void print_stage_1(word_t dict){
    printf("==========================Stage 1==========================\n");
    printf("Word 0: %s\n", dict[0].root);
    printf("POS: %s\n", dict[0].POS);
    printf("Form: %s\n", dict[0].form);
}

void print_stage_2(word_t dict, int word_count){
    printf("==========================Stage 2==========================\n");
    printf("Number of words: %d\n", word_count);
    printf("Average number of variation forms per word: %.2f\n", (double)
           tot_form_count(dict,word_count)/word_count);
}

void print_stage_4(word_t dict, list_t list, int word_count){
    printf("==========================Stage 4==========================\n");
    node_t* temp = list.head;
    for(int i = 0; i < word_count; i++){
        printf("%-26s\n", temp->data);
        temp = temp->next;
    }
}

// Comments: Tough... can't complete in time and trying to understand how 
//           bsearch() works fries my brain. Hope that this is enough to pass. 
//           :D. (Algorithms are still awesome!)