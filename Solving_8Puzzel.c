                        //   solve 8 puzzel    //

#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<limits.h>
#define N 3
#define EMPTY 0
#define Depth 30    // change Depth  : 20 30 40 50 

enum {DOWN = 3,RIGHT = 1,LEFT = -1,UP = -3};

int matrix_solution[N][N]={{ 1 , 2 , 3 },
                           { 4 , 5 , 6 },
                           { 7 , 8 , 0 }};

typedef struct tree
{
    int info;
    int score;
    struct tree *up;
    struct tree *left;
    struct tree *right;
    struct tree *down;
}tree;

// test the tree if it is empty
int tree_empty(tree *tr){
    if(tr == NULL) return 1;
    return 0;
}
// test the tree if it is empty 
int leaf(tree *tr){
    if(tree_empty(tr->down) && tree_empty(tr->left) && tree_empty(tr->right) && tree_empty(tr->up) && !tree_empty(tr)) return 1;
    return 0;
}
// create new node 
tree* create_node(int u,int score){
    tree *tr = malloc(sizeof(tree));
    if(tr != NULL)
    {
        tr->info=u;
        tr->score=score;
        tr->up=NULL;
        tr->left=NULL;
        tr->right=NULL;
        tr->down=NULL;
    }
    return tr;
}

// the function return integer representing the minimum number of moves needed for the position to be in place
int min_distance(int pos_array,int pos_matrix_solution){
    return abs((pos_array/3)-(pos_matrix_solution/3))+abs((pos_array%3)-(pos_matrix_solution%3));
}
//  the number of matrix numbers that are out of place 
int number_numbMatrix_out(int array[N][N]){
    int counter=0;
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(array[i][j] == 0) continue;
            if(array[i][j] != matrix_solution[i][j]) counter++;
        }
    }
    return counter;
}
//  the function return sum of all the min_distance for the position in array except the empty
int score_array(int array[N][N]){
    int sum=0,position_empty;
    for(int i=0 ; i < N ; i++){
        for(int j=0 ; j < N ; j++){
            if(array[i][j] != EMPTY){
                position_empty = array[i][j] - 1;
                sum += min_distance(i*3+j,position_empty);  // i*3+j representing the position of the array
            }
        }
    }
    return sum;
}
//  the function can move to the direcion and return the position of empty
int move_to_direction(int array[N][N],int direction){
    int temp,pos_empty;

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
           if(array[i][j] == EMPTY)
            {
               temp = array[direction/3][direction%3];
               array[direction/3][direction%3] = EMPTY;
               array[i][j] = temp;
               pos_empty=i*3+j;
               return pos_empty;
            }
        }
    }
}
//  fill in the table with the possible directions except for the old one
void Fill_dir_possible(int array[N][N],int tab_dir[4],int pos_empty){
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(array[i][j] == EMPTY){
                if(i-1 != -1 && (i-1)*3+j != pos_empty)
                  tab_dir[0]=(i-1)*3+j;  //up
                if(j-1 != -1 && i*3+j-1 != pos_empty)
                  tab_dir[1]=i*3+j-1;   //left
                if(j+1 != 3 && i*3+j+1 != pos_empty)
                  tab_dir[2]=i*3+j+1;   //right
                if(i+1 != 3 && (i+1)*3+j != pos_empty)
                  tab_dir[3]=(i+1)*3+j;  //dawn
                return;
            }
        }
    }
}
// the function best_score return sum of score array and the number numbMatrix out of place  
int best_score(int array[N][N]){
    return score_array(array)+number_numbMatrix_out(array);
}
// copy table2 in the table1
void copy_tab(int tab1[Depth],int tab2[Depth]){
    for(int i=0;i<Depth-1;i++)
    tab1[i]=tab2[i+1];  
}

// this function stores the best possible path with the lowest cost by minimum sum the score until leaf
void min_path(tree *tr,int Tab[Depth],int i,int s,int *min,int Path[Depth]){
    if(tree_empty(tr)) 
      return;
    i++;
    if(leaf(tr))
    {
        s+=tr->score;
        if(s < *min)
        {
            *min = s;
            Tab[i] = tr->info;
            copy_tab(Path,Tab);     
        }
        return;
    }
    s += tr->score;
    Tab[i] = tr->info;
    min_path(tr->up,Tab,i,s,min,Path);
    min_path(tr->down,Tab,i,s,min,Path);
    min_path(tr->left,Tab,i,s,min,Path);
    min_path(tr->right,Tab,i,s,min,Path);
}
//  create tree of directions possible until depth and return that tree
tree* create_tree_paths(tree *tr,int array[N][N],int d, int pos_old){
    int tab_direction[4]={-1,-1,-1,-1},best_s,pos_empty;
    tree *p ;
    if(d == Depth || tree_empty(tr)) 
      return NULL;
    Fill_dir_possible(array,tab_direction,pos_old);
    for(int i=0;i<4;i++)
    {
        if(tab_direction[i] != -1)
        {
            pos_empty = move_to_direction(array,tab_direction[i]);
            switch(i)
            {
                case 0:best_s = best_score(array);
                        tr->up = create_node(tab_direction[i],best_s);
                        p = create_tree_paths(tr->up,array,d+1,pos_empty);
                        tr->up = p;
                        break;
                case 1:best_s = best_score(array);
                        tr->left = create_node(tab_direction[i],best_s);
                        p = create_tree_paths(tr->left,array,d+1,pos_empty);
                        tr->left = p;
                        break;
                case 2:best_s = best_score(array);
                        tr->right = create_node(tab_direction[i],best_s);
                        p = create_tree_paths(tr->right,array,d+1,pos_empty);
                        tr->right = p;
                        break;
                case 3:best_s = best_score(array);
                        tr->down = create_node(tab_direction[i],best_s);
                        p = create_tree_paths(tr->down,array,d+1,pos_empty);
                        tr->down = p;
                        break;

            }
            move_to_direction(array,pos_empty);
        }else{
            switch(i){
                case 0: tr->up = NULL;break;
                case 1: tr->left = NULL;break;
                case 2: tr->right = NULL;break;
                case 3: tr->down = NULL;break;
            }
        }
    }
    return tr;
}
//  free the tree 
void free_tree(tree *tr){
    if(tree_empty(tr)) return;
    free_tree(tr->left);
    free_tree(tr->down);
    free_tree(tr->right);
    free_tree(tr->up);
    free(tr);
}

int testing_is_solution(int array[N][N]){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(array[i][j] != matrix_solution[i][j])
            return 0;
        }
    }
    return 1;
}
// print path solution
void print_solve(int PATH[Depth],int pos_old,int cpt)
{
    int temp;
    if(cpt != 0)
    temp = PATH[cpt] - PATH[cpt - 1];
    else  temp = PATH[0] - pos_old;
    switch(temp)
    {
        case DOWN : printf("\n DOWN "); break;
        case RIGHT : printf("\n RIGHT "); break;
        case LEFT : printf("\n LEFT "); break;
        case UP : printf("\n  UP "); break;
    }
}


/* this function solves problem by finding the best path with the lowest cost in a tree with depth
   and when you do not find the solution in this tree free it and create another tree with the same depth 
   as before and search and so on .....
*/
void solve_puzzle(int array[N][N],int pos_empty)
{
    int Tab[Depth],Path[Depth],min;
        tree *tr =create_node(-1,0);
    while(1){
        min = INT_MAX;
        for(int i=0;i<Depth;i++)
        {
            Path[i] = -1;
            Tab[i] = -1;
        }
        tr = create_tree_paths(tr,array,0,pos_empty);
        min_path(tr,Tab,-1,0,&min,Path);
        free_tree(tr); 
        for(int i=0;i<Depth-1;i++)
        {
            if(Path[i] != -1)
            {
                print_solve(Path,pos_empty,i);
                move_to_direction(array,Path[i]);
                if(testing_is_solution(array))
                {
                   printf("\n FOUND SOLUTION \n");
                   return;
                }
            }
        }
        tr = create_node(Path[Depth-2],0);
        pos_empty = Path[Depth-3];
    }
}

int main(){
    int array[N][N],position_empty;
    printf("\n|------------------- SOLVING 8 PUZZEL-------------------|");
    printf("\n Enter the form matrix :\n");
    //  reading the array 
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            scanf("%d",&array[i][j]);
            if(array[i][j] == EMPTY)
            position_empty = i*3+j;
        }
    }
    // solve and print the solution
    solve_puzzle(array,position_empty);
    return 0;
}
