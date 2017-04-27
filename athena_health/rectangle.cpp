/*Leetcode 84. Largest Rectangle in Histogram*/
/*
* (0) DataStruct->Stack(index), internally it should be an increasing array. 
* (1) POP: if cur height is less than stack.top(). Push will always happened.
* (2) Once Popped on a specfic height {y}, we need to get the rectangle bonded by this height, we need to know corresponding length = left-right.
* (3) After poped, left = stack.top()+1,   right=hight[curindex-1].  Think it in a reverse way.
      there can't be a height within [left+1,y] is less than y. Otherwise left and y can't be next to each other.
      there also can't be a height within [y,cur-1] is less than y. Otherwise y can't be in the stack.
* (4) Don't forget to see if stack is empty at the end. if not, calculate rectangle like (3).
*/
class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        int max = 0;
        /*Index array*/
        vector<int> index;
        for(int i =0; i<heights.size(); i++){
            while(index.size() && heights[index.back()] >= heights[i]){
                int h = heights[index.back()];
                index.pop_back();
                //int left = index[index.size()-1];!!!
                int left = index.size() > 0 ? index.back() : -1;
                if ((i-left-1)*h > max) max = (i-left-1)*h;
            }
            index.push_back(i);
        }
        int i = heights.size();
        while(index.size()){
            int h = heights[index.back()];
            index.pop_back();
            int left = index.size() > 0 ? index.back() : -1;
            if ((i-left-1)*h > max) max = (i-left-1)*h;
           // printf("hight=%d width=%d left=%d\n", h,i-left-1, left);

        }
    
        return max;
    }
};
