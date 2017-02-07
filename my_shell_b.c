#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

int check_cd(const char* command){
	/*This function is for checking the 
	  capital words if it is "cd".*/
  	char* cd = "cd ";
  	int i = 0;
  	for(i = 0; cd[i]; i++){
    	if(command[i] != cd[i]){
      	return -1;
    	}
  	}
  	return 0;
}

//These two global variables is for 
//tracking history. 
char *collection[10];
int count = 0;

void record_history(char *command){
	/*This function is for record the recent
	  ten command lines, except \n.*/
	if(strcmp(command,"\n")!=0){
  		collection[count%10]=strdup(command);
  		count++;
	}
}

void history() {
	/*This function is for print out the 
	  recent ten command lines.*/
  	int i, j;
  	if(count>=10){
  		for(i=count,j=0;j<10;i++,j++){
    		printf("\t%d: %s",j+1,collection[i%10]);
  		}
  	}
	else{
		for(i = 0; i < count; i++)
			printf("\t%d: %s",i+1,collection[i]);
	}
}
//Main function
int main(){
  	//First Page:
  	printf("\t\tWeclome to my shell!\n\n");
  	//Variants:
  	char command[100]; //Store command line
  	char buffer[100];
  	int i = 0;
  	for(i=0; i<10;i++){
		collection[i]=NULL;
	}

  	while(1){
    	printf("cla280@my_shell -> ");
    	bool isInternal = false;//This is for detecting if 
    							//it is internal command
    	fgets(command,100,stdin);
    	record_history(command);
    	if(feof(stdin)){		//ctrl+D to quit
    		printf("\n");
    		exit(0);
    	}

    	if(strcmp(command,"exit\n")==0){
    		/*This is for quit the program.*/
	      	exit(0);
	    }
	    
	    if(check_cd(command)==0){
	    	/*This is for changing the directory.*/
	    	isInternal = true;
	        int i = 3, j = 0;
	        char rest_command[100];
	        int num;
	        for(num =0; num <100;num++){
				rest_command[num]=0;
			}
	        for(;command[i]!='\n';i++,j++){
	          rest_command[j] = command[i];
	        }
	        if(chdir(rest_command) != 0){
	          printf("bash: cd %s: No such a file or directory\n",rest_command);
	        }
	      }
	    else if(strcmp(command,"\n")==0){isInternal = true;}
	    else if(strcmp(command,"history\n")==0){
	    	/*This is for check history.*/
	    	isInternal = true;
	       	history();
	    }
	    else if(strcmp(command,"pwd\n")==0){
	    	/*This is for checking current working directory.*/
	    	isInternal = true;
	       	getcwd(buffer,sizeof(buffer));
	       	printf("%s\n",buffer);
	    } 
	    //to capture command line for execvp function
	    char* line[100];
    	char* temp;
    	temp = strtok(command," \n");
    	int count = 0;
    	while(temp!=NULL){
    		line[count] = temp;
    		temp = strtok(NULL, " \n");
    		count++;
    	}
    	line[count] = NULL; 
	    //Forking program:
	    pid_t pid;
	    pid = fork();
	    if (pid < 0){
	      	perror("Error\n");
	      	return -1;
	    }
	    else if (pid == 0){	//this is child process
	    	if(isInternal==false){	//this is for capture the error command
	    		if(execvp(line[0],line)==-1){
	    			printf("Command cannot found.\n" );
	    	 		exit(0);
	    		}
	    	}
			exit(0);
	    }
	    else{	//this is parent process
	    	waitpid(-1,NULL,0);      	
	    } 
	}
	return 0;
}
