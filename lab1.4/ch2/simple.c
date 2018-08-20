#include <linux/init.h>
#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/utsname.h>

#ifndef N
#define N 10
#endif

//global struct
typedef struct {
	int ID;
	char *name;
	int age;
}Student;

Student students[N];
unsigned char cont = 0;

// Custom functions.
void uptime(void);
void getSystemInfo(void);
void add(int, char[], int);
void list(void);

// This function is called when the module is loaded.
int simple_init(void)
{
	printk(KERN_INFO "Loading Module\n");
	uptime();
	getSystemInfo();
	add(21, "Fer", 21);
	add(31, "Fer", 19);
	add(43, "Fer", 25);
	list();
       return 0;
}

// This function is called when the module is removed.
void simple_exit(void) {
	printk(KERN_INFO "Removing Module\n");
}


// This functions gets the uptime
void uptime() {
  printk("Uptime: %i\n", jiffies_to_msecs(get_jiffies_64())/1000);
}


// This function gets the general system information
void getSystemInfo(){  
  struct new_utsname *buf;
  buf = utsname();

  printk("Nodename: %s\n",buf->nodename);
  printk("Machine: %s\n",buf->machine);
  printk("Sysname: %s\n",buf->sysname);
  printk("Release: %s\n",buf->release);
  printk("Version: %s\n",buf->version);


#ifdef __linux__
  printk("Domain Name: %s\n", buf->domainname); // GNU extension
#endif
}

void add(int sID, char *sName, int sAge){
/*
	struct Student newStudent;
	newStudent.ID=sID;
	char* i;
	unsigned char cnt=0;
	for(i=sName; *i != NULL; i++){
		newStudent.name[cnt] = sName[cnt++];
	}
	newStudent.age=sAge;*/
	students[cont].ID = sID;
	students[cont].age=sAge;
	students[cont++].name=sName;
	
}

void list(){
	int i;
	for(i=0; i<N;i++){
		printk("Students name: %s, ID: %d, AGE: %d\n", students[i].name, students[i].ID, students[i].age);
	}
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
