#include<stdlib.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
using namespace std;
#define ctoi(_c) ((int)(_c) - int('A'))
#define COL 3
#define ROW 3

/* Similar to Leetcode 425WordSquares
 *(1) Using Trie to saved the words in dictionary. Mark Leaf if the words completes!
 *(2) Key is to define next step-> 8 directions, and also prevent looping.
 *(3) Once hit leaf print out the seq.
 */

class TrieNode{
public:
    bool isleaf;
    TrieNode *child[26];
}; 
TrieNode* init(bool IsLeaf) {
        TrieNode* cur = new TrieNode();
        for(int i =0; i<26; i++){
            cur->child[i] = NULL;
        }
        cur->isleaf = IsLeaf ;
        return cur;
}

/*mark leaf, update root moves on*/
void insert(TrieNode *root, const char *input){
    if (!input) return;
    int len = strlen(input);
    TrieNode *tmp = root;
    for (int i = 0; i<len; i++) {
        int index = ctoi(input[i]);
        if (!tmp->child[index]) tmp->child[index] = init(false);
        if (i == len -1 ) {
            tmp->child[index]->isleaf = true;
        }
        tmp = tmp->child[index];  
    }  
    return;
}
/*edge+visited*/
bool valid (int row, int col, bool visited[ROW][COL]){
     if(row>=ROW || row<0 || col >=COL || col <0 || visited[row][col]) return false;
     return true;
}
/*
  key
  if leaf node is hit, Bingo 
  keep search from 8 directions, 
  update visited no loop in the current path, unmark it once done
*/
void startSearch (char input[ROW][COL], int row, int col, TrieNode* root, bool visited[ROW][COL], string str){
          if(root->isleaf) printf("Find %s\n", str.c_str());
          if(!valid(row, col, visited)) return;
          //validate search 
          visited[row][col] = true;
          for (int i = 0; i< 26; i++){
               if (root->child[i]){
                   char tmp = (char)((int)'A'+i);
                   if (valid(row+1, col, visited) && input[row+1][col] == tmp){
                       startSearch(input, row+1, col, root->child[i], visited, str+tmp);
                   }
                   if (valid(row, col+1, visited) && input[row][col+1] == tmp){
                       startSearch(input, row, col+1, root->child[i], visited, str+tmp);
                   }
                   if (valid(row+1, col+1, visited) && input[row+1][col+1] == tmp){
                       startSearch(input, row+1, col+1, root->child[i], visited, str+tmp);
                   }
                   if (valid(row-1, col, visited) && input[row-1][col] == tmp){
                       startSearch(input, row-1, col, root->child[i], visited, str+tmp);
                   }
                   if (valid(row, col-1, visited) && input[row][col-1] == tmp){
                       startSearch(input, row, col-1, root->child[i], visited, str+tmp);
                   }
                   if (valid(row-1, col-1, visited) && input[row-1][col-1] == tmp){
                       startSearch(input, row-1, col-1, root->child[i], visited, str+tmp);
                   }
                   if (valid(row+1, col-1, visited) && input[row+1][col-1] == tmp){
                       startSearch(input, row+1, col-1, root->child[i], visited, str+tmp);
                   }
                   if (valid(row-1, col+1, visited) && input[row-1][col+1] == tmp){
                       startSearch(input, row-1, col+1, root->child[i], visited, str+tmp);
                   }
               } 
          }
          visited[row][col] = false;  //don't forget to set it back
          return;
}
/*
init the dictionary root and generate the dictionary
init input array
init visited array
find the start, if matches with any children of the root,  go deeper;
*/
int main(int argc, char* argv[]){
    const char *dictionary[] = {"GEEKS", "FOR", "QUIZ", "GEE"}; // warning
    TrieNode *root = init(false);
    int len = sizeof(dictionary)/sizeof(dictionary[0]);
    for (int i =0; i< len; i++){
        insert(root, dictionary[i]);
    }
    
    char input[ROW][COL] = {{'G','I', 'Z'}, {'U','E','K'}, {'Q','S','E'}};
    bool visited[ROW][COL];
    memset(visited, 0, sizeof(bool)*ROW*COL); 
    string str = "";   //! 
    for(int i = 0; i <ROW; i++){
        for(int j = 0; j <COL; j++){
           int index = ctoi(input[i][j]);
           if (root->child[index]) {
               str = input[i][j]; 
               startSearch(input, i, j, root->child[index], visited, str);
           }
        }
    }
    return 0;
}
