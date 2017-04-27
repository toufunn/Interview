#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<unordered_map>
#include<vector>
#include<string>
using namespace std;

/*Key:  String Manipulations+DFS+pruning path if not valid.
 *Issue hit: if not pruning, run time become exponential, and some mem leak. 
 *1st Q:
 *"Efficiently" recursively generate the candidate by pre-creating the index array [details will be given later]
 *Filter out unqualified combinations while generating the candidate by using the last_Index_removed!
 *(I forgot it at the 1st place and the run time became exponential!)
 *Flagging X with removable position. Once completely removed the REMOVAL, start to eliminate 'X'.
 *Using unordered_map to see if this combination is duplicated or not.
 *2Q: Generalize the issue:
 *Save intermediate seq list (unordered_map), make it as  "given", and follow the 1st Q.
 *Extension: [reflected on my code]
 *The number of removals is not limited to 2. 
 *The removable character are also not limited to morse code. 
 * Explanation for index array
 /* Generate occurrence lists  len = strlen(REMOVAL)
     * e.g give="*-_-***", removal="*-*"
     * index will have 3 elements
     * index[0] = occurrence of STAR from given, which is [0,4,5,6]
     * index[1] = occurrence of DASH from given, which is [1,3]
     * index[2] = occurrence of SEP from given, which is [2]
  */

typedef unordered_map<string, int>Mymap;
enum morse_symbol{
    STAR = 0,
    DASH,
    SEP,
    MAX_SYM,
};
const char morse[] = {'*', '-', '_'};

inline void myexit(const char* target, int error) {
    printf("%s failed to be allocated at %s : %d\n", target, __func__, __LINE__);
    exit(error);
}

/*
 * using unordered_map to check duplicates. 
 * Its internal implementation is very close to java hashmap
 * <key,value> key = remaining_sequence, here value doesn't matter
 */
inline static void updateMap(Mymap *m, char* input, int* sum) {
    if (!input) return;
    string s(input);
    auto it = m->find(s);
    if (it != m->end()) {
        return;
    } else {
        m->insert({s, 1});
        (*sum)++;
    }
}

/*
 *Generate remaining sequence
 *Remove X flag from src
 */
inline static void compress(char* src, char* target, char removal) {
    if (!target || !src || !removal) return;
    int len = strlen(src);
    int index = 0;
    for (int i =0; i < len; i++) {
        if (src[i] == removal) continue;
        target[index++] = src[i];
    }
    target[index] ='\0';
    return;
}

/*
 * Recursively generate candidate sequence by flaging removal position with X
 * Remove those X position to get real remaining one.
 * If this remaining sequenece is not a duplicate, sum will be incremented.
 */
static void generate(char* start, int** index, int* middle, int depth, int maxdepth, Mymap* m, int* sum, int last) {
    if (depth == maxdepth) {
        /*flagging finisned*/
        char* remain = strdup(start);
        if (!remain) myexit("remain", -1);
        compress(start, remain, 'X');
        updateMap(m, remain, sum);
        free(remain);
        remain = NULL;
        return;
    }
    for (int i =0; i < middle[depth]; i++) {
        /*filter out unqualified combination*/
        if (index[depth][i] > last) {
            char *next = strdup(start);
            if (!next) myexit("next", -1);
            next[index[depth][i]] ='X';
            generate(next, index, middle, depth+1, maxdepth, m, sum, index[depth][i]);
            free(next);
            next = NULL;
        }
    }
    return;
}

/*
 * args: given, removal, map
 * @return the number of unique sequence
 */
static int hidden_internal(const char* given, const char* removal, Mymap* m) {
    int  sum                   = 0;
    char *start                = NULL;
    int  **index               = NULL;
    int  *indexLength          = NULL;
    int  indexNum              = 0;
    int  error                 = -1;
    int  symbolLength[MAX_SYM] = {0};
    int  *symbol[MAX_SYM];

    if ( !given || !removal || !m ) {
        myexit("Invalid Input", error);
    }

    /*Initalize space for saving occurence of morse symbols */
    indexNum = strlen(removal);
    index = (int**)malloc(indexNum* sizeof(int*));
    indexLength = (int*)malloc(indexNum* sizeof(int));
    if (!index  || !indexLength) {
        myexit("index", error);
    }
    memset(indexLength, 0, sizeof(int)*indexNum);
    for (int i = 0; i < indexNum ; i++) {
        index[i] = (int*)malloc(strlen(given)*sizeof(int));
        if (!index[i]) {
            myexit("index", error);
        }
        memset(index[i], 0, strlen(given)*sizeof(int));
    }
    for (int i = 0; i < MAX_SYM; i++) {
        symbol[i] = (int*)malloc(strlen(given)*sizeof(int));
        if (!symbol[i]) {
            myexit("symbol", error);
        }
        memset(symbol[i], 0, strlen(given)*sizeof(int));
    }

    /*list of STAR,DASH,SEP occurence from given*/
    for (int i =0; i < (int)strlen(given); i++) {
        for (int j =0; j < MAX_SYM; j++) {
            if (given[i] == morse[j]) {
                symbol[j][symbolLength[j]++] = i;
                break;
            }
        }
    }
    /* Generate occurence lists based on given
     * e.g give="*-_-***", removal="*-*"
     * index will have 3 elements
     * index[0] = occurence of STAR from given, which is [0,4,5,6]
     * index[1] = occurence of DASH from given, which is [1,3]
     * index[2] = occurence of SEP from given, which is [2]
     */
    for (int i =0; i < (int)strlen(removal); i++) {
        for (int j =0; j< MAX_SYM; j++) {
            if (removal[i] == morse[j]) {
                memcpy(index[i], symbol[j], (symbolLength[j]*sizeof(int)));
                indexLength[i] = symbolLength[j];
                break;
            }
        }
    }
    start = strdup(given);
    if (!start) {
        myexit("start", error);
    }
    generate(start, index, indexLength, 0, strlen(removal), m, &sum, -1);

    /*Free resource*/
    free(start);
    free(indexLength);
    if(index){
        for (int i = 0; i < indexNum ; i++) {
            free(index[i]);
        }
        free(index);
    }
    for (int i = 0; i < MAX_SYM; i++) {
        free(symbol[i]);
    }
    return sum;
}

/*Find unique morse code after serval removal*/
int main(int argc, char *argv[]) {
    int  sum         = 0;
    char *given      = NULL;
    char *removal    = NULL;
    int  iteration   = 0;

    if (argc < 3) {
        printf("Please give vaild nums of inputs(>2)\n e.g ./hidden_msg Given Removal1 <optional>\n");
        return -1;
    }

    given = argv[1];
    iteration = argc-1;
    vector<Mymap> Table;

    for (int i = 0; i< iteration; i++) {
        Mymap tmp;
        Table.push_back(tmp);
    }
    Table[0].insert({string(given), 1});

    /*Ideally num of removals are not limited to 2*/
    for (int i = 2; i < argc; i++) {
        removal = argv[i];
        Mymap m = Table[i-2];
        int unique = 0;
        for (unordered_map<string, int>::iterator it = m.begin(); it != m.end(); ++it) {
            unique += hidden_internal((it->first).c_str(), removal, &Table[i-1]);
        }
        if (i == argc-1) sum = unique;
    }

    printf("Total unique token after removal: %d\n", sum);
    return sum;
}
