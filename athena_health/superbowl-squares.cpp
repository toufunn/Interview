#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
using namespace std;
const double baseprob[]={0.5, 0.3, 0.2}; 

/*
 * The key is to understand One "Round"  ---- Both team will have their turns to defeat! 
 * Always Be patient to understand the puzzle! It is hard indeed. Be calm and confident!
 * To simplified the issue, we should start solving puzzles from final points for each team. 
 * After N round: what are the possiblities to get [0-2*N]?!  check function: cal simple recursive!
 * P(A|B) = P(A)*P(B) The possibilites to have a paticular match A:B = P(A)*P(B);
 * Almost there! Have a MAX_STACK/priority_queue to keep 10 top possibilities.
 */

/*DP input round,score*/
double cal (int r, int s,const double* base, int blen) {  
    double sum = 0.0;
	if (r == 1 && s >=0 && s<blen) {
		return base[s];
	} else if (r >1) {
		for (int i = 0; i<blen; i++) {
			sum += (cal(r-1,s-i, base, blen))*base[i];
		}
	}
    return sum; // need to retrun the sum!!!
}

/*For sorting and final score, simple class*/
struct node{
  int teama;
	int teamb;
	double prob;
	node(int a, int b, double p) {
		teama = a;
		teamb = b;
		prob = p;
	}
};

/*Define compare function*/
typedef bool (*comp)(node, node);
bool compare(node A, node B){
	return A.prob > B.prob;
}

int main(int argc, char* argv[]){
  /*5 round top 10*/
	int N = atoi(argv[1]);
	int blen = atoi(argv[2]);
	priority_queue<node, vector<node>, comp> proq(compare);
	for (int a = 0; a <= 2*N; a++){
	    for(int b = 0; b <= 2*N; b++){
		      double cur = cal(N,a,baseprob,blen)*cal(N, b, baseprob, blen);
    	    node tmp = node(a, b, cur);
		      printf("a=%d, b=%d Probability=%lf\n", a, b, cur);
          proq.push(tmp);
          if(proq.size()>10){
		          proq.pop();
	        }
		  }
	}
  
	int i = 0;
	while(proq.size()){
	    node cur = proq.top();
	    proq.pop();
		  printf("The %dth Probability of %lf score:%d-%d\n", i, cur.prob, cur.teama, cur.teamb);
      i++; 
	}
}
