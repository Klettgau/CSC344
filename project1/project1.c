#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
//	TO DO List
//add amount to the free function
//some more error checking

typedef struct indivNode{
   int value;
  struct  indivNode *next;
  struct  indivNode *prior; 
} node;
// for each 256 ascii char
typedef struct  character{
    int writeVal;
    int direction;
    int newState; //double check
}tapeChar;


int getSize(node **);
node* genNode(int);
void printList(node**);
void printListPos(node**,node**);
void printList(node**);
void runMachine(tapeChar***,int,int,node **,node**);
void readFile(node**,node**,tapeChar***,int *,int *);
void initStateTable(tapeChar ****,int);
void freeList(node**);
void initList(node**,node**);
void freeStates(tapeChar ***,int);
int stateAmt=0;
node *head,*tail;
tapeChar **stateTable;
int start=0;
int end=0;

initList(&head,&tail);
printf("enter the name of the file \n");
readFile(&tail,&head,&stateTable,&start,&end);
printf("Writing tape \n");
printf("Intial contents ");
//printf("size %d \n",getSize(&head));
printList(&head);
printf("\n");
runMachine(&stateTable,start,end,&head,&tail);
printf("Output:");
printList(&head);
freeList(&head);
freeStates(&stateTable,stateAmt);

}
void addNodeHead(node **head,int val){
   node *newNode = genNode(val);
   (*head)->prior = newNode;
   newNode->next= (*head);
   newNode->prior = NULL;
   (*head) = newNode;
}

void addNodeTail(node **tail,int val){
   node *newNode = genNode(val);
   newNode->next=NULL; //this is the new tail
   newNode->prior = *tail;   
   (*tail)->next = newNode;
   *tail = newNode; 
}

node* genNode(int val){
node *tmp= malloc(sizeof(node));
tmp->next =NULL;
tmp->prior=NULL;
tmp->value =val;//value read from file
return tmp;
}




void printListPos(node **head,node** currPos){
node *curr;
int counter =0;
for(curr=*head;curr!=NULL;curr=curr->next){
    if(*currPos ==curr)
        printf("---->cur");
    
    printf("pos %d : node %d dec and char :%c \n",counter,curr->value,curr->value);
    counter++;}

}
void printList(node **head){
node *curr;
int counter =0;
for(curr=*head;curr!=NULL;curr=curr->next){
    printf("%c",curr->value);
    counter++;}

}


//b is 66
void moveLeft(node ** currPos,node **head){
    if((*currPos)->prior ==NULL){
        addNodeHead(head,66);
        *currPos = (*head); //old head
                 }
else{
(*currPos)= (*currPos)->prior;
}
}

void moveRight(node **currPos,node **tail){
    if((*currPos)->next ==NULL){
         addNodeTail(tail,66);
         *currPos = *tail;
           }
 else{   (*currPos)= (*currPos)->next;}

}
int getSize(node**head){ //helper method to see length
    node *present;
    int count =0;
    
    for(present=(*head);present!=NULL;present=present->next){
        count++;}
    return count;  
}
//main func that will read file and update struct
void readFile(node **tail,node **head,tapeChar ***stateTable,int *startState,int *endState){
int counter=0;
FILE *source;
char fileLoc[30];
char fileName[30];
int userPassed =0; //false
  while(!userPassed){
    fgets(fileLoc,29,stdin);
    fileLoc[strlen(fileLoc)-1] = '\0';
    sscanf(fileLoc,"%s",fileName);
    source =fopen(fileName, "r");
    if(source ==NULL){
        printf("please try again \n");
      }
    else{userPassed=1;}
   }
int fileLine =0;     
int chk=0;
int readVal,state,writeVal,  direct, newState;
writeVal= readVal=state=direct=newState=0;
char *token,*endPt;
char delim[] =" ";
char tmp[100];
while(fgets(tmp,99,source)!=NULL){
    int loop =0;
    int rows =0;
  switch(fileLine){
     case 0:
       while(tmp[loop] != '\0'){
           if(tmp[loop] == '\n'){
             loop=0;
             break;} //we got to the end of the line
         if(loop ==0){
           (*head)->value =tmp[loop];
           loop++;
          }
         else{
         addNodeTail(tail,tmp[loop]);
         loop++;}        
       }
        //printList(head);
        break;
    case 1://pass amt as rows
         if(sscanf(tmp,"%d",&rows)==1){
           initStateTable(&stateTable,rows);
           stateAmt= rows;
               }
          else{ printf("Please ensure the input file is formatted correctly \n");
           exit(0);}      
        break;
    case 2:
          sscanf(tmp,"%d",startState);      
        break;
    case 3:
          sscanf(tmp,"%d",endState);
        break;
    default: //all lines after line 5
           for(int i=0;i<10;i++){//ten is the length of given file 5 vals plus space
             if(i%2==0){
                switch(i){
         case 0: //evens 
              state = strtol(tmp,&endPt,10);
              if(state==0 &&errno!=0){
                 printf("strtol %cfailed  %s\n",state,endPt); 
                  }
               break;
         case 2://just spaces
               readVal = tmp[i];
               break;
         case 4://just spaces
               writeVal = (int)tmp[i];
                if(state ==0 &&errno!=0){
                       if(state ==NULL)
                          printf("state is null %d i value %d lineNum %d writeVal%d and charState %c\n",state,i,fileLine,writeVal,tmp[i]);
                   
                           }
            (*stateTable)[(char)state][readVal].writeVal =writeVal;
               break;
         case 6://just spaces
               direct = tmp[i];
                if(state ==NULL &&errno!=0)
                   printf("state is null. i value %d lineNum %d writeVal%d and charState %c %dDirection\n",i,fileLine,writeVal,tmp[i],direct);
            (*stateTable)[state][readVal].direction = direct;
               break;
         case 8://just spaces
               newState = strtol(&tmp[i],&endPt,10);
            (*stateTable)[state][readVal].newState = newState; 
               break;}       
       }} //for and if
     }//switch
  fileLine++;
        }
}//func

void runMachine(tapeChar ***stateTable,int startState,int stopState,node **head,node **tail){
// you get the start state,read value, use it to get value fro,m stateTable load instruct execute and loop until we end in the nd state then print out lnked list
    int currState= startState;
    int valueRead = (*head)->value;
      node *currPos = malloc(sizeof(node));
    currPos = (*head);
    int nextState,valueWrite,direction;
    
  while(currState<stopState){
     nextState = (*stateTable)[currState][valueRead].newState;
     valueWrite = (*stateTable)[currState][valueRead].writeVal;
     direction = (*stateTable)[currState][valueRead].direction;
    // printf("currState %d nextState %d writeVal %d direction %d \n",currState,nextState,valueWrite,direction);
     (currPos)->value = valueWrite;
     if(direction ==82){
        moveRight(&currPos,tail);
      //printListPos(head,&currPos);
        }
     else{   
             moveLeft(&currPos,head); 
             //printListPos(head,&currPos);
          }
      currState=nextState;
      valueRead = currPos->value;
      }
}


void printTable(tapeChar ***stateTable, int amtRow){
for(int w=0;w<amtRow;w++){
    for(int i=0;i<256;i++){
         if((*stateTable)[w][i].newState !=87)
             printf("nextState %d direction%d writeVal%d\n", (*stateTable)[w][i].newState,(*stateTable)[w][i].direction,(*stateTable)[w][i].writeVal);

         }
     }   
}

//create the state table
void initStateTable(tapeChar ****stateTable,int amtRow){
*(*stateTable) = malloc(sizeof(tapeChar *)*amtRow);
for(int w=0;w<amtRow;w++){
    *((*(*stateTable))+w)= (tapeChar *)malloc(sizeof(tapeChar)*256);
    for(int i=0;i<256;i++){
        (*(*stateTable))[w][i].writeVal =57;
        (*(*stateTable))[w][i].direction =90;
        (*(*stateTable))[w][i].newState =87;
      }
   }

}
//create the double linked list
void initList(node **head,node **tail){
*head = malloc(sizeof(node)); //always first ele
*tail = malloc(sizeof(node)); //always last ele
(*head)->value = 66;
*tail = *head;
(*head)->prior = NULL;
//head and tail point to same node
}



void freeStates(tapeChar ***stateTable , int amtRow){
    for(int w=0;w<amtRow;w++){
       free(*(((*stateTable))+w));
      } 
    free((*stateTable));
    *stateTable =NULL;
}

void freeList(node ** head){
    node *next;
    node *curr;
   int loop=0;
    for(curr=*head;curr!=NULL;loop++){
         next = curr->next;
         free(curr);//last free is issue
         curr=NULL;
        curr = next;
       }
*head=NULL;
}
