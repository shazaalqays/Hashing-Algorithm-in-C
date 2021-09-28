#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include<math.h>

int hashKey(unsigned char *str);
int hashcode1(int key);
int hashcode2(int key);
int size_of_hashtable();
void init_array();
int get_prime();
void insert(int key, char value);
void display();
void del_element(int key);

struct data {
	int key;
	char value;
	char doc; // for document
};

struct hashtable_item {
	int flag;
	/*
	 * flag = 0 : data not present
	 * flag = 1 : some data already present
	 * flag = 2 : data was present,but deleted
	*/
	struct data *item;
};

// createing array of structure
struct hashtable_item *array;

int max = 50;
int size = 0;
int prime = 3;
int w[1024];

int main(int argc, char *argv[]) {
	int choice, key, word, doc, n, c;
	FILE *filePointer; 
	DIR *FD;
//	char *buffer = 0;
	int buffer =0;
	long length;
	struct dirent *dataToBeRead; // Declare the variable for the data to be read from directory 
	array = (struct hashtable_item*) malloc(max * sizeof(struct hashtable_item));
	init_array();
    // Open the existing directory using opendir() 
    FD = opendir("D:\\Algorithms Analysis\\files") ; // change path!
      
    // Check if this FD is null which maybe if the directory does not exist 
    if ( FD == NULL ){ 
        printf( "file failed to open.\n" ) ; 
    } 
    else{
        // Read the dataToBeRead from the directory using readdir() method 
        while( (dataToBeRead = readdir(FD)) != NULL){ 
            // Print the dataToBeRead  
            printf( "%s\n" , dataToBeRead->d_name ) ;
            FILE *file = fopen(dataToBeRead, "r");
//            if (file){
				fseek (file, 0, SEEK_END);
				length = ftell (file);
				fseek (file, 0, SEEK_SET);
				buffer = malloc (length);
				if (buffer){
    				fread (buffer, 1, length, file);
				}
				fclose (file);
//			}
			if (buffer){
				// start to process data
				key = hashKey(buffer);
			}
			insert(key,buffer);
        } 
        // Closing the directory using closedir() 
        closedir(FD) ; 
    }
    display();
    printf("\n size of table is %d ", size_of_hashtable());
	return 0;
}

// function takes a string and return integer value which is the key
int hashKey(unsigned char *str){
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash x 33 + c */
    return hash;
}

int hashcode1(int key){
	printf("key %d\n", key);
	printf("key mod max %d",key % max );
	return (key % max);
}

int hashcode2(int key){
	return (prime - (key % prime));
}

int size_of_hashtable(){
	return size;
}

/* initializes array */
void init_array(){
	int i;
	for(i = 0; i < max; i++)
        {
		array[i].item = NULL;
		array[i].flag = 0;
	}
	prime = get_prime();
}


/* returns largest prime number less than size of array */
int get_prime(){
	int i,j;
	for (i = max - 1; i >= 1; i--)
        {
		int flag = 0;
		for (j = 2; j <= (int)sqrt(i); j++)
                {
			if (i % j == 0)
                        {
				flag++;
			}
		}
		if (flag == 0)
                {
			return i;
		}
	}
	return 3;
}

void insert(int key, char value){
	int hash1 = hashcode1(key);
	printf("%d\n", hash1);
	int hash2 = hashcode2(key);
	printf("%d\n", hash2);
	int index = hash1;
	/* create new data to insert */
	struct data *new_item = (struct data*) malloc(sizeof(struct data));
	new_item->key = key;
	new_item->value = value;
	if (size == max){
		printf("Hash Table is full, cannot insert more items \n");
		return;
	}
 
	/* probing through other array elements */
	while (array[index].flag == 1) {
		if (array[index].item->key == key){
			printf("Key already present, hence updating its value \n\n");
			array[index].item->value = value;
			return;
		}
		index = (index + hash2) % max; 
		if (index == hash1){
			printf("Add is failed \n");
			return;
		}
		printf("probing \n");
	}
	array[index].item = new_item;
	array[index].flag = 1;
	size++;
	FILE *fp = fopen ("E:\\Computer Engineering\\9th Semester\\Algorithms Analysis\\Homework 2\\table.txt","wb");
	fprintf (fp,"Word: %d", new_item->key);  
	fclose (fp);
	printf("Key (%d) has been inserted \n\n", key);
}


/* displays all elements of array */
//void display(){
//	int i;
//	for (i = 0; i < max; i++)
//        {
//		if (array[i].flag != 1)
//                {
//			printf("Array[%d] has no elements \n", i);
//		}
//		else
//                {
//			printf("Array[%d] has elements \n Key (%d) and Value (%d) \n", i, array[i].item->key, array[i].item->value);
//		}
//	}
//}

void display(){
	FILE *fptr;
	int num;
	if ((fptr = fopen("E:\\Computer Engineering\\9th Semester\\Algorithms Analysis\\Homework 2\\table.txt","r")) == NULL){
		printf("Error! opening file");
		exit(1);
   }
   fscanf(fptr,"%d", &num);

   printf("Words: %d", num);
   fclose(fptr); 
}
 
/* to remove an element from the array */
void del_element(int key){
	int hash1 = hashcode1(key);
	int hash2 = hashcode2(key);
	int index = hash1;
 
	if (size == 0)
        {
		printf("\n Hash Table is empty \n");
		return;
	}
 
	/* probing through other elements */
	while (array[index].flag != 0)
        {
		if (array[index].flag == 1  &&  array[index].item->key == key)
                {
			array[index].item = NULL;
			array[index].flag = 2;
			size--;
			printf("Key (%d) has been removed \n", key);
			return;
		}
		index = (index + hash2) % max;
		if (index == hash1)
                {
			break;
		}
	}
 
	printf("Key (%d) does not exist \n", key);
 
}


