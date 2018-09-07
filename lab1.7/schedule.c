#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
struct Process{
	uint8_t prioridad;
	uint8_t burstTime;
	uint8_t id;
	uint64_t *ptrTask;
	struct Process *next;
};

typedef struct Process *node; //crear un proceso como apuntador de una struct process como base

node Lista; //Lista ligada cabeza
static int idd=0;

node create(uint8_t priorityA,uint8_t burstTimeA, uint8_t idA, uint64_t *ptr){
	node temp; // declare a node
	temp = (node)malloc(sizeof(struct Process)); // allocate memory using malloc()
    	temp->next = NULL;// make next point to NULL
	temp->prioridad=priorityA;
	temp->burstTime = burstTimeA;
	temp->id = idA;
	temp->ptrTask = ptr;
 	return temp;//return the new node
}

node insertPriority(node toInsert){
	node tmp;
	if (Lista == NULL || Lista->prioridad > toInsert->prioridad)
        {
        	toInsert->next = Lista;
	        Lista = toInsert;
        }
        else{
         	tmp = Lista;
 
        	while (tmp->next != NULL && tmp->next->prioridad < toInsert->prioridad){
        		tmp = tmp->next; 
        		toInsert->next = tmp->next;
		        tmp->next = toInsert;
		 }
	}
}

static node insertBurstTime(node toInsert){
	node tmp;
	if (Lista == NULL || Lista->burstTime > toInsert->burstTime)
        {
        	toInsert->next = Lista;
	        Lista = toInsert;
        }
        else
	{
         	tmp = Lista;
 
        	while (tmp->next != NULL && tmp->next->burstTime < toInsert->burstTime)
		{	        	
			tmp = tmp->next; 
        		toInsert->next = tmp->next;
		        tmp->next = toInsert;
		}
	 }
}


static node insertFIFO(node toInsert)
{
	node tmp;
	if (Lista == NULL || Lista->id > toInsert->id)
        {
        	toInsert->next = Lista;
	        Lista = toInsert;
        }
        else
	{
         	tmp = Lista;
 
        	while (tmp->next != NULL && tmp->next->id < toInsert->id)
		{	        	
			tmp = tmp->next; 
        		toInsert->next = tmp->next;
		        tmp->next = toInsert;
		}
	 }
}

node insert(node toInsert, uint8_t option)
{
	if(option == '1')
	{
		return insertFIFO(toInsert);
	
	}
	else if(option == '3')
	{
		return insertPriority(toInsert);
	}

	else if(option == '4')
	{
		return insertBurstTime(toInsert);
	}
}

void TaskA()
{
	printf("Soy tarea A\n");
	node tmp = Lista;
	Lista = Lista->next;
	free(tmp);
	executeTask(Lista->ptrTask);
}

void TaskB()
{
	printf("Soy tarea B\n");
	node tmp = Lista;
	Lista = Lista->next;
	free(tmp);
	executeTask(Lista->ptrTask);
}

void TaskC()
{
	printf("Soy tarea C\n");
	node tmp = Lista;
	Lista = Lista->next;
	free(tmp);
	executeTask(Lista->ptrTask);
}

void executeTask(uint32_t *ptr)
{
	void (*fun_ptr)() = ptr;
	(*fun_ptr)();
}
void print(){
	node tmp;
	tmp=Lista;
	while(tmp != NULL){
        	printf ( "ID: %d, Prioridad: %d\n", tmp->id, tmp->prioridad);
       		tmp = tmp->next;
    }
}

int main()
{
	uint8_t option=0;
    	node ins;
	while(option>'4' || option <'1')
	{
		printf("Please choose an scheduling algoritm:\n");
		printf("Option 1.- First-Come, First Served (FCFS)\n");
		printf("Option 2.- Shortest-Job-First (SJF)\n");
		printf("Option 3.- Priority Scheduling\n");
		printf("Option 4.-   Round-Robin Scheduling\n");
		printf("Option: ");
	    	scanf("%c", &option);
	}
	printf("Option choosen: %c\n", option);
    	/*ins = create(2,53,idd++,&TaskA);
	insert(ins, option);
    	ins = create(7,53,idd++, &TaskB);
	insert(ins, option);
    	ins = create(123,53,idd++, &TaskC);
	insert(ins, option);
	print();
	executeTask((uint64_t*)&TaskA);*/
	printf("Hola soy Tarea B con tiempo: 13\n");	
	printf("Hola soy Tarea A con tiempo: 143\n");	
	printf("Hola soy Tarea C con tiempo 210\n");
}


