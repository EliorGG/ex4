/******************
Name: Elior Golan
ID: 331516922
Assignment: ex4
*******************/
#include <stdio.h>
#include <string.h>

#define PYRAMID_LEVELS 5
#define MAX_GRID_SIZE 20
#define NEUTRAL_COLUMN -2
/*
 We mark 0 in VALID because when we want to initialize arrays with 4 and 5,
 we want everything to be free to put queens/words. Also, it is convenient to initialize an array with zeros.
*/
#define VALID 0
#define INVALID 1
/*
 At the beginning of task 4, we say that there is no previous queen,
 so to avoid confusion we will mark the first previous queen with 2.
*/
#define MAX_SLOTS_NUM 100
#define MAX_CROSSWORD_SIZE 30
#define MAX_WORDS_NUM 100
//max word size is 16 and not 15 because of the \0
#define MAX_WORD_SIZE 16
#define MAX_SLOT_SIZE 15

//slot struct for task5:
typedef struct {
    int row;
    int column;
    int length;
    char direction;
}Slot;
/*
All function annotations will be during implementation
But basically each helper receives the input data and calculates the result
*/
//task1 functions:
void task1_robot_paths();
int task1Helper(int x, int y);
//task2 functions:
void task2_human_pyramid();
float task2Helper(float pyramid[][PYRAMID_LEVELS], int i,int j);
//task3 functions:
void task3_parenthesis_validator();
char oppositeParenthesis(char parenthesis);
int isOpenParenthesis(char parenthesis);
int isClosedParenthesis(char parenthesis);
int task3Helper(char lastParenthesis);
//task4 functions:
void task4_queens_battle();
int task4Helper(char grid[][MAX_GRID_SIZE],int board[][MAX_GRID_SIZE],int capturedColors[][MAX_GRID_SIZE],
    int allowedColumns[MAX_GRID_SIZE],int row,int column,int prevQueenColumn,int size);
void fillColor(char grid[][MAX_GRID_SIZE],int capturedColors[][MAX_GRID_SIZE],int i,int j,char color,int fillingValue,
    int size);
//task5 functions:
void task5_crossword_generator();
int isFitIn(char board[][MAX_CROSSWORD_SIZE],Slot slot,char word[MAX_WORD_SIZE],char slotChange[MAX_SLOT_SIZE],
    int index);
void safeDelete(char board[][MAX_CROSSWORD_SIZE],Slot slot,char slotChange[MAX_SLOT_SIZE],int index);
int task5Helper(char board[][MAX_CROSSWORD_SIZE],int size,Slot slots[MAX_SLOTS_NUM],int slotsNum,
    char words [MAX_WORDS_NUM][MAX_WORD_SIZE],int availableWords[MAX_WORDS_NUM],int wordsNum,int currentSlot,
    int currentWord);

int main()
{
    int task = -1;
    do
    {
        printf("Choose an option:\n"
               "1. Robot Paths\n"
               "2. The Human Pyramid\n"
               "3. Parenthesis Validation\n"
               "4. Queens Battle\n"
               "5. Crossword Generator\n"
               "6. Exit\n");

        if (scanf("%d", &task))
        {
            switch (task)
            {
            case 6:
                printf("Goodbye!\n");
                break;
            case 1:
                task1_robot_paths();
                break;
            case 2:
                task2_human_pyramid();
                break;
            case 3:
                task3_parenthesis_validator();
                break;
            case 4:
                task4_queens_battle();
                break;
            case 5:
                task5_crossword_generator();
                break;
            default:
                printf("Please choose a task number from the list.\n");
                break;
            }
        }
        else
        {
            scanf("%*s");
        }

    } while (task != 6);
}

void task1_robot_paths(){
    printf("Please enter the coordinates of the robot (column, row):\n");
    int column=0,row=0;
    //input
    scanf( "%d %d",&column,&row);
    //output
    printf("The total number of paths the robot can take to reach home is: %d\n",task1Helper(column,row));
}
/*
The function receives a position in a coordinate system,
and calculates how many ways there are from that point to the origin of the coordinates.
*/
int task1Helper(int x, int y) {
    //base case(reaching the origin):
    if(x==0 && y==0)
        return 1;
    //edge case(exiting the axis system):
    if(x<0 || y<0)
        return 0;
    //Recursive call
    //(calculating how many options we get from a step-down and how many from a step left and a sum of both)
    return task1Helper(x-1,y)+task1Helper(x,y-1);
}

void task2_human_pyramid(){
    float pyramid[PYRAMID_LEVELS][PYRAMID_LEVELS]={0};
    //input:
    printf("Please enter the weights of the cheerleaders:\n");
    for(int i=0;i<PYRAMID_LEVELS;i++) {
        for(int j=0;j<i+1;j++) {
            scanf(" %f",&pyramid[i][j]);
            if(pyramid[i][j]<0) {
                //If a negative number is entered, then we will return to the menu.
                printf("Negative weights are not supported.\n");
                return;
            }
        }
    }
    //output:
    printf("The total weight on each cheerleader is:\n");
    for(int i=0;i<PYRAMID_LEVELS;i++) {
        for(int j=0;j<i+1;j++)
            printf("%.2f ",task2Helper(pyramid,i,j));
        printf("\n");
    }

}

/*
The function receives the pyramid with the weights of each cheerleader,
a row in the pyramid, and a position in the row.
The function calculates how much weight the cheerleader has at the resulting position.
*/
float task2Helper(float pyramid[][PYRAMID_LEVELS], int i,int j) {
    //base case(The top of the pyramid):
    if(i==0)
        return pyramid[i][j];
    //edge cases(The tip of the pyramid, meaning that above us there is only one cheerleader):
    if(j==i)
        return 0.5*task2Helper(pyramid,i-1,j-1)+pyramid[i][j];
    if(j==0)
        return 0.5*task2Helper(pyramid,i-1,j)+pyramid[i][j];
    return 0.5*task2Helper(pyramid,i-1,j-1)+0.5*task2Helper(pyramid,i-1,j)+pyramid[i][j];
}

void task3_parenthesis_validator()
{
    printf("Please enter a term for validation:\n");
    //Clearing the buffer from whiteSpaces after receiving the number 3
    scanf(" ");
    /*
     Calling the function (we haven't read anything from the buffer yet,
     so there is no initial opening parenthesis, so we send \0 to the function)
    */
    if(task3Helper('\0'))
        printf("The parentheses are balanced correctly.\n");
    else
        printf("The parentheses are not balanced correctly.\n");
}
/*
 The function receives the last opening parenthesis in the buffer and
 returns true or false depending on what is left in the buffer matches that parenthesis
 and also whether it is correct in terms of the parenthesis itself.
 If the last opening parenthesis in the string was already closed\ and there was no parenthesis at the beginning,
 then will send to the function \0
 */
int task3Helper(char lastParenthesis) {
    char c;
    //Getting the next character in the string
    scanf("%c",&c);
    //base case with a check that there are no remaining unclosed brackets
    if(c=='\n')
        return lastParenthesis=='\0';
    /*
     If we closed the parentheses with an inappropriate parentheses, we will clear the buffer to the end
     (in the previous condition we made sure that we are not at the end, so there is definitely something in the buffer)
     and return 0 because there is no longer a chance that the string will be valid.
    */
    if(isClosedParenthesis(c) && c!=oppositeParenthesis(lastParenthesis)) {
        scanf("%*[^\n]");
        return 0;
    }
    //We closed the parenthesis: we can move on.
    if(isClosedParenthesis(c) && c==oppositeParenthesis(lastParenthesis))
        return 1;
    /*If new brackets are opened then we will first look for the last parenthesis that was opened and after that
    we have found it, we will look for the parenthesis of the first one. (keeping the order)
    But if not, we'll just keep looking for a bracket(Therefore, we initialize x to a true value).
    */
    int x=1;
    if(isOpenParenthesis(c))
        x=task3Helper(c);
    return x&&task3Helper(lastParenthesis);
}
//help functions for task3Helper:
/*
The function receives an opening parenthesis and returns the parenthesis that matches it.
If we did not receive an opening parenthesis, the function will return \0.
*/
char oppositeParenthesis(char parenthesis) {
        if (parenthesis == '(')
            return ')';
        if(parenthesis == '[')
            return ']';
        if(parenthesis == '{')
            return '}';
        if(parenthesis == '<')
            return '>';
        return '\0';

}
//The function accepts a character and returns whether it is an opening parenthesis or not.
int isOpenParenthesis(char parenthesis) {
    return parenthesis == '(' || parenthesis == '[' || parenthesis == '{' || parenthesis == '<';
}
//The function accepts a character and returns whether it is a closing parenthesis or not.
int isClosedParenthesis(char parenthesis) {
    return parenthesis==')' || parenthesis== ']' || parenthesis=='}' || parenthesis=='>';
}

void task4_queens_battle(){
    char grid[MAX_GRID_SIZE][MAX_GRID_SIZE]={0};
    int board[MAX_GRID_SIZE][MAX_GRID_SIZE]={0};
    //input size:
    printf("Please enter the board dimensions:\n");
    int size=0;
    scanf("%d",&size);
    //input colors on the board:
    printf("Please enter a %d*%d puzzle board:\n",size,size);
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            scanf(" %c",&grid[i][j]);
    /*
     Creating arrays that mark where queens are allowed to be placed,
     we will initialize them to VALID because at the beginning we did not mark any queens,
     so they are allowed to be placed anywhere.
     */
    int capturedColors[MAX_GRID_SIZE][MAX_GRID_SIZE]={VALID},allowedColumns[MAX_GRID_SIZE]={VALID};
    /*
     *output(First we will try to place a queen at 0,0).
    The neutralizing column is -2 because at the beginning we have no previous queen,
    so in order not to disturb the first queen we enter -2
    (an explanation of the meaning of the previous queen will be given later).
    */
    if(task4Helper(grid,board,capturedColors,allowedColumns,0,0,NEUTRAL_COLUMN,size)) {
        printf("Solution:\n");
        for(int i=0;i<size;i++) {
            for(int j=0;j<size;j++) {
                if(board[i][j]==0)
                    printf("* ");
                else
                    printf("X ");
            }
            printf("\n");
        }
    }
    else
        printf("This puzzle cannot be solved.\n");

}
/*
The function receives: the board divided into colors, the queens that we have already marked,
the board where it is written which colors are invalid,0 an array of invalid columns,
and the column of the previous queen (to check diagonals)
and also receives the position of the queen that is being tried to be placed.
We only need the column of the previous queen because when we check diagonals
we only need to make sure we don't put it in 2 columns next to it
And that's because the function works so that each time we go down a row.
Each time the operation tries to place a queen in a row,
marks in the appropriate places which places on the board it prohibits and then drops a row
if the queen's position there does not give the solution, backtracking is done
 */
int task4Helper(char grid[][MAX_GRID_SIZE],int board[][MAX_GRID_SIZE],int capturedColors[][MAX_GRID_SIZE],
    int allowedColumns[MAX_GRID_SIZE],int row,int column,int prevQueenColumn,int size) {
    // base case(all rows finished):
    if(row==size)
        return 1;
    //If we failed to place a queen in the-i row:
    if(column==size)
        return 0;
    //first trying next column,else we are moving on:
    if(task4Helper(grid,board,capturedColors,allowedColumns,row,column+1,prevQueenColumn,size))
        return 1;
    //Attempt to place a queen, if the attempt fails, we will do backtracking.
    //As I mentioned before, we are trying to check that we are not on the columns adjacent to the queen above us.
    if(capturedColors[row][column]==VALID && prevQueenColumn-1!=column && prevQueenColumn+1!=column && allowedColumns[column]==VALID) {
        /*
         Marking on the board where we marked the queen
         and marking in the appropriate arrays which places it is not allowed to place now
        */
        board[row][column]=1;
        allowedColumns[column]=INVALID;
        fillColor(grid,capturedColors,row,column,grid[row][column],INVALID,size);
        int isThisStepSolve=task4Helper(grid,board,capturedColors,allowedColumns,row+1,0,column,size);
        if(isThisStepSolve)
            return 1;
        //backtracking:
        board[row][column]=0;
        allowedColumns[column]=VALID;
        fillColor(grid,capturedColors,row,column,grid[row][column],VALID,size);
    }
    //If placing a queen here isn't solving the board::
    return 0;
}
/*
 The action receives a location color and value,
The function fills the entire area with the same color with the given value.
*/
void fillColor(char grid[][MAX_GRID_SIZE],int capturedColors[][MAX_GRID_SIZE],int i,int j,char color,int fillingValue,
    int size) {
    //base case: We went beyond the boundaries of the board or out the boundaries of th color-area.
    if(i==size || j==size || i<0 || j<0)
        return;
    if(grid[i][j]!=color)
        return;
    //if we fill the value here already we will stop:
    if(capturedColors[i][j]==fillingValue)
        return;
    //filling the value and continue "spreading":
    capturedColors[i][j]=fillingValue;
    //sides:
    fillColor(grid,capturedColors,i+1,j,color,fillingValue,size);
    fillColor(grid,capturedColors,i-1,j,color,fillingValue,size);
    fillColor(grid,capturedColors,i,j-1,color,fillingValue,size);
    fillColor(grid,capturedColors,i,j+1,color,fillingValue,size);
    //diagonals:
    fillColor(grid,capturedColors,i-1,j-1,color,fillingValue,size);
    fillColor(grid,capturedColors,i-1,j+1,color,fillingValue,size);
    fillColor(grid,capturedColors,i+1,j-1,color,fillingValue,size);
    fillColor(grid,capturedColors,i+1,j+1,color,fillingValue,size);
}

void task5_crossword_generator(){
    int size=0,slotsNum=0;
    //input size:
    printf("Please enter the dimensions of the crossword grid:\n");
    scanf("%d",&size);
    //input number of slots:
    printf("Please enter the number of slots in the crossword:\n");
    scanf("%d",&slotsNum);
    printf("Please enter the details for each slot (Row, Column, Length, Direction):\n");
    //input the data of the slots:
    Slot slots[MAX_SLOTS_NUM];
    for(int i=0;i<slotsNum;i++) {
        scanf(" %d",&slots[i].row);
        scanf(" %d",&slots[i].column);
        scanf(" %d",&slots[i].length);
        scanf(" %c",&slots[i].direction);
    }
    //input number of words while making sure that the number of the words is not less than the number of slots
    int wordsNum=0;
    printf("Please enter the number of words in the dictionary:\n");
    do {
        scanf(" %d",&wordsNum);
        if(wordsNum<slotsNum)
            printf("The dictionary must contain at least %d words. Please enter a valid dictionary size:\n",slotsNum);
    }while(wordsNum<slotsNum);
    char words[MAX_WORDS_NUM][MAX_WORD_SIZE];
    printf("Please enter the words for the dictionary:\n");
    for(int i=0;i<wordsNum;i++) {
        scanf(" %s",words[i]);
    }
    //Creating a blank board(all words are available at first):
    char board[MAX_CROSSWORD_SIZE][MAX_CROSSWORD_SIZE]={0};
    int availableWords[MAX_WORDS_NUM]={VALID};
    //output(They start checking from the first slot and the first word.):
    if(task5Helper(board,size,slots,slotsNum,words,availableWords,wordsNum,0,0)) {
        for(int i=0;i<size;i++) {
            printf("| ");
            for(int j=0;j<size;j++) {
                if(board[i][j]==0)
                    printf("# | ");
                else
                    printf("%c | ",board[i][j]);
            }
            printf("\n");
        }
    }
    else
        printf("This crossword cannot be solved.\n");
}
/*
The operation receives the board and the inputs(data of the board),
the operation goes through each slot and on each slot tries to place a word.
The operation modifies the board accordingly to If the word fits the slot.
but if in the end we find that the rest of the board is not solved in this way,
we delete the change that occurred in the board (we do not delete what was on the board before we created the word)
 */
int task5Helper(char board[][MAX_CROSSWORD_SIZE],int size,Slot slots[MAX_SLOTS_NUM],int slotsNum,
    char words [MAX_WORDS_NUM][MAX_WORD_SIZE],int availableWords[MAX_WORDS_NUM],int wordsNum,int currentSlot,
    int currentWord) {
    //base case(all slots are full):
    if(currentSlot == slotsNum)
        return 1;
    //No word matched the current slot, so the puzzle is unsolvable:
    if(currentWord==wordsNum)
        return 0;
    //If we have already used this word then we will move on to the next word.
    if(availableWords[currentWord]==INVALID)
        return task5Helper(board,size,slots,slotsNum,words,availableWords,wordsNum,currentSlot,currentWord+1);
    /*
    Now we will try to place the word in the slot,
    we will create an array that will store the change that was made in the board,
    if no change was made anywhere there will be 0 in the array
     */
    char slotChange[MAX_SLOT_SIZE]={0};
    if((int)strlen(words[currentWord])==slots[currentSlot].length &&
        isFitIn(board,slots[currentSlot],words[currentWord],slotChange,0)) {
        //marking that this word is used:
        availableWords[currentWord] = INVALID;
        //recursive call:
        if(task5Helper(board,size,slots,slotsNum,words,availableWords,wordsNum,currentSlot+1,0))
            return 1;
        //this word isn't used so will unmarking it:
        availableWords[currentWord] = VALID;
    }
    //If the word position in this slot is not the solution, we backtrack with the array we created specifically.
    //then we will go to the next word
    safeDelete(board,slots[currentSlot],slotChange,0);
    return task5Helper(board,size,slots,slotsNum,words,availableWords,wordsNum,currentSlot,currentWord+1);
}
/*
The operation receives a word and a slot.
The operation attempts to place the word in the slot and saves any changes made to the board in the array.
 */
int isFitIn(char board[][MAX_CROSSWORD_SIZE],Slot slot,char word[MAX_WORD_SIZE],char slotChange[MAX_SLOT_SIZE],
    int index) {
    //base case:
    if(index==(int)strlen(word))
        return 1;
    //Calculation of which character on the board is being tested now:
    int row=slot.row,column=slot.column;
    if(slot.direction == 'V')
        row+=index;
    if(slot.direction=='H')
        column+=index;
    //doesnt match:
    if(board[row][column]!=0 && board[row][column] != word[index])
        return 0;
    //if the board was empty we will save the change:
    if(board[row][column]==0)
        slotChange[index]=word[index];
    board[row][column]=word[index];
    return isFitIn(board,slot,word,slotChange,index+1);
}
//Performing a "backtrack", deleting the changes in the board
void safeDelete(char board[][MAX_CROSSWORD_SIZE],Slot slot,char slotChange[MAX_SLOT_SIZE],int index){
    //base case:
    if(index==slot.length)
        return;
    //calculation of which character on the board is being tested now:
    int row=slot.row,column=slot.column;
    if(slot.direction=='V')
        row+=index;
    if(slot.direction=='H')
        column+=index;
    //if was a change we will change it back:
    if(slotChange[index]!=0) {
        board[row][column]=0;
    }
    safeDelete(board,slot,slotChange,index+1);
}