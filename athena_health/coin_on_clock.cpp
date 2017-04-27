#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<string>
#include<unordered_map>
#include<vector>
#include<climits>
using namespace std;
#define clock 12
#define NOTINIT -1
enum coin{
penny=0,
nickle,
dimme,
};
const int steps[] = {1,5,10};
const char faces[] = {'P','N','D'};

/* Q1:
 * The key is find a way to translate "Placing Seq" into valid ClockFace. (%12)
 * Pruning Condition:
 * ClockFace can't have two coin on the same place + Restriction is 4coins Each
 * Also the starting place shouldn't matter!!! Please filter out that possiblity for the 1st Q.
 * Q2:
 * MAX = sum(clockface*value); 
 * Once we have final seq from Q1, we understand the all 12 "hour" could be starting place for any seq
 * We shold calculate all posibility and updating max value.
 * But remember the array start from 0-11, we need to convert it to 1-12.
 */

/*input: seq check if seq is valid as well as the num of coin is not exceed 12*/
bool isValid(unordered_map<string, string>& res, string s, int pcount, int ncount, int dcount, int visited[clock]){
     /*Safety check*/
     if(pcount<0 || ncount <0 || dcount <0) return false;
     const char *input = s.c_str();
     int len = strlen(input);
     /*clock face index*/
     int index = 0;
     int curcoin = 0;
     for (int i= 0; i<len; i++){
        if (input[i] == 'P'){
            index += steps[penny];
            curcoin = penny;
        }else if(input[i] == 'N'){
            index += steps[nickle];
            curcoin = nickle;
        }else if(input[i] == 'D'){
            index += steps[dimme];
            curcoin = dimme;
        }
        //if over 12
        index = index%clock;
        if(visited[index]!=NOTINIT){
            return false;
        }else{
            visited[index] = curcoin;
        }
     }
     return true;
}

/*DFS Enum all possibilities and if not valid, just prune the this path*/
void nextStep(unordered_map<string, string>& res, string s, int pcount, int ncount, int dcount){
    int visited[clock];
    for( int i = 0;i<clock; i++){
        visited[i] = NOTINIT; 
    }
    if (isValid(res, s, pcount,ncount,dcount, visited)) {
        if (s.size() == clock) {
            /*Bingo*/
            //regenerate the clock face
            string clockface = "";
            for(int i  = 0; i < clock; i++){
                 clockface += faces[visited[i]];
            }
            
            res.insert({s,clockface});
        } 
         nextStep(res, s+'P',pcount-1, ncount, dcount);
         nextStep(res, s+'N',pcount, ncount-1, dcount);
         nextStep(res, s+'D',pcount, ncount, dcount-1);
     }
}
void updateMax(string s,int *max){
    const char *in = s.c_str();
    for(int i =0; i<clock; i++){
        int cur = 0;
        for(int j =0;j<clock; j++){
            int value = 0;
            if (in[j] == 'P') {
                value =1;
            }
            if (in[j] == 'N') {
                value =5;
            }
            if (in[j] == 'D') {
                value =10;

            }
            /*We should use [1-12]*/
            cur += value*((i+j)%12+1);
        }
        if (cur >*max)  *max = cur;
     }

}
int main(int argc, char* argv[]){

    unordered_map<string, string> res;
    string start = "";
    int max = INT_MIN;
    nextStep(res,start,4,4,4);
    /*Show the seq*/
    printf("Final SEQ:\n");
    unordered_map<string, string>::iterator it;
    for (it=res.begin(); it!=res.end(); it++) {
        printf("%s\n",(it->first).c_str());
        updateMax(it->second,&max);
    }
    printf("MAX is %d:\n", max);
    return 0;
}
