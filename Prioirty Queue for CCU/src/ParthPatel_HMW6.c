/*
 ============================================================================
 Name        : ParthPatel_HMW6.c
 Author      : Parth Patel
 Version     :
 Copyright   : Your copyright notice
 Description : Prioity Queue for a critical care unit
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

const int maxQ = 20;
struct cust
{
	char id[5];
	int days;
}; typedef struct cust cust;

struct heap //heap
{
	cust* Arry[100];
	int	last;
	int	size;
	int MaxSize;
	int (*compareCust)(cust* cust1, cust* cust2);
}; typedef struct heap heap;

heap* heapCreate (int MaxSize,int(*compareCust)(cust* cust1, cust* cust2)) //initializes the heap
{
	heap* newheap;
	newheap=(heap*)malloc(sizeof (heap));
	if(!newheap)
		return NULL;
	newheap->size=0;
	newheap->last=-1;
	newheap->compareCust=compareCust;
	newheap->MaxSize=(int)pow(2,ceil(log2(MaxSize)))-1;
	return newheap;

}


void reheapUp(heap* heap, int index) //makes sure heap is heap after insert
{
	int parent;
	cust* temp;

	if(index)
	{

		parent= (index-1)/2;
		if(heap->compareCust(heap->Arry[index],heap->Arry[parent])>0) //if child is less then parent then swap
		{
			temp=heap->Arry[parent];
			heap->Arry[parent]=heap->Arry[index];
			heap->Arry[index]=temp;
			reheapUp(heap,parent);

		}

	}

	return;
}

bool heapInsert(heap* heap, cust* newcust) //adds a patient to the heap
{
	if(heap->size==0){ //if adding first patient
		heap->size=1;
		heap->last=0;
		heap->Arry[heap->last]=newcust;
		return true;
	}

	if(heap->last == heap->MaxSize -1) //if heap at max capacity
		return false;

	//general case
	++(heap->last); //increment size and index counter
	++(heap->size);
	heap->Arry[heap->last]=newcust; //adds patient to the end
	reheapUp(heap,heap->last); //calls reheap up to make sure the new patient is not the highest priority
	return true;

}

void reheapDown(heap* heap, int root) //moves the root down and the smallest node to the root to follow heap rules
{
	cust* temp;
	cust* leftchild;
	cust* rightchild;
	int largeindex;
	int last;
	int flag=0;

	last=heap->last;
	if((root*2 +1)<= last) //if there is a left child
	{
		leftchild=heap->Arry[root * 2 + 1]; //assign the left child
		if((root*2+2)<=last) //if there is a left and right child
			rightchild=heap->Arry[root * 2 + 2]; //assign the right child
		else
			flag=1; //set a flag for the loop below

		if((flag==1)|| heap->compareCust(leftchild,rightchild)>0) //if there is no right child or the left child is smaller
		{
			largeindex=root *2 +1; //puts the left in the variable to be moved
		}
			else
			{
			largeindex=root *2 +2; //right child is going to be moved

			}

		if(heap->compareCust(heap->Arry[root],heap->Arry[largeindex])<0) //if the root is larger then the child then the two need to be swapped
		{
			temp=heap->Arry[root];
			heap->Arry[root]=heap->Arry[largeindex];
			heap->Arry[largeindex]=temp;
			reheapDown(heap,largeindex);

		}
	}

	return;

}

cust* heapDelete(heap* heap , cust* root) //deletes the root and calls reheap down to make sure the heap is a heap with smallest node on top
{
	if(heap->size == 0) //if heap is empty
	{
		printf("No patients to serve!\n");
		return false;
	}

	root = heap->Arry[0]; //makes sure root is the beginning of the heap
	heap->Arry[0] = heap->Arry[heap->last]; //the root now holds the last patient in the system
	(heap->last)--; //decrement the size and last index tracker
	(heap->size)--;
	reheapDown(heap,0); //reheap down so that the smallest node goes to the root and the heap rules are followed
	return root;

}



int compareCust(cust* cust1,cust* cust2) //compares patient days so that the heap is formatted correctly
{
	cust c1;
	cust c2;

	c1=*(cust*)cust1;
	c2=*(cust*)cust2;

	if(c1.days>c2.days)
		return -1;
	else if (c1.days == c2.days)
		return 0;
	return 1;

}

cust* insertCust(void) //asks user for a patient and gets the information
{
	cust* newcust;
	newcust=(cust*)malloc(sizeof(cust));
	if(!newcust){
		printf("cannot add any more patients\n");
		exit(200);
		}
	else{
		printf("Enter a patient id:\n");
		fflush(stdout);
		scanf("%5s",newcust->id);
		fflush(stdin);
		printf("How many days since they arrived:\n");
		fflush(stdout);
		scanf("%d",&newcust->days);
		fflush(stdin);
		if(newcust->days>100) //makes sure days is not larger than 100
		{
			printf("We cannot serve patients after 100 days!!!\n");
			printf("How many days since they arrived?\n\n");
			fflush(stdout);
			scanf("%d",&newcust->days);
			fflush(stdin);
		}

	}
		return newcust;
}

/*void printQ(heap* newheap){ //for testing prints the entire heap
	int i;
	if (newheap->size==0){
		printf("no one in queue\n");
	}
	for(i=0;i<newheap->size;i++){
		printf("%s\n",newheap->Arry[i]->id);
		printf("%d\n",newheap->Arry[i]->days);
	}

	printf("\n\n");
}
*/


heap* orderPrint(heap* newheap){ //print in order of priority
	heap* tempQ;
	cust* root;
	int i=0,j=newheap->size;
	tempQ=heapCreate(maxQ,compareCust); //create a second heap that will become the new heap
	printf("Patient ID:		Days since arrival\n\n");
	if (newheap->size==0){ //makes sure old heap is populated
			printf("no one in queue\n");
		}
	else{ //if the old heap is populated delete the highest priority, then prints the value, and finally adds to the new heap
		while(i<j){
			root=heapDelete(newheap,root);
			printf("%-5s 			%-3d\n",root->id,root->days);
			heapInsert(tempQ,root);
			i++;
	}
	}
	 return tempQ; //tempQ becomes the new heap with the same information as before

}




int main(void) {
	heap* newQ;
	cust* newcust;
	newQ=heapCreate(maxQ,compareCust);
	char menu_option;

		 do{//prints menu as long as the q option is not pressed
			printf("\nMain Menu\n");
			printf("What would you like to do?\n");
			printf("a. Insert a new patient\n");
			printf("b. Serve the patient with the highest priority\n");
			printf("c. Display the patients in order of priority\n");
			//printf("d. Display the heap (for testing purposes delete later)\n");

			printf("q. Press q at this menu to quit.\n\n");
			fflush(stdout);
			scanf(" %c",&menu_option);
			fflush(stdin);


			switch(menu_option){
			case 'a': //Insert cust
				newcust=insertCust();
				heapInsert(newQ,newcust);
				break;
			case 'b': //delete root
				newcust=heapDelete(newQ,newcust);
				break;
			case 'c': //print in descending order
				newQ=orderPrint(newQ);
				break;
			//case 'd': //print whole heap for testing
				//printQ(newQ);
			//	break;
			case 'q': //breaks out if q is pressed
				break;
			default: //if other key is pressed then the error message is printed
				printf("invalid input\n");
					break;
			}
		}while(menu_option!='q');


	return 0;
}
