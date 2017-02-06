#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

//The three functions belowing is for finding
//the target file or directory.
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

int check_cat(const char* command){
	/*This function is for checking the 
	  capital words if it is "cat".*/
  	char* cat = "cat ";
  	int i = 0;
  	for(i = 0; cat[i]; i++){
    	if(command[i] != cat[i]){
      	return -1;
    	}
  	}
  	return 0;
}

int check_rm(const char* command){
	/*This function is for checking the 
	  capital words if it is "rm".*/
  	char* rm = "rm ";
  	int i = 0;
  	for(i = 0; rm[i]; i++){
    	if(command[i] != rm[i]){
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

int main(){

  	//First Page:
  	printf("\t\tWeclome to my shell!\n\n");
  	//Variants:
  	char command[100]; //Store command line
  	char buffer[100];
  	char* sys_call;
  	int i = 0;
  	for(i=0; i<10;i++){
		collection[i]=NULL;
	}
  	while(1){
    	printf("cla280@my_shell -> ");
    	fgets(command,100,stdin);
    	record_history(command);
    	if(feof(stdin)){
    		printf("\n");
    		exit(0);
    	}

    	if(strcmp(command,"exit\n")==0){
    		/*This is for quit the program.*/
	      	exit(0);
	    }

	    if(check_cd(command)==0){
	    	/*This is for changing the directory.*/
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
	    else if(strcmp(command,"\n")==0){}
	    else if(strcmp(command,"history\n")==0){
	    	/*This is for check history.*/
	       	history();
	    }
	    else if(strcmp(command,"pwd\n")==0){
	    	/*This is for checking current working directory.*/
	       	getcwd(buffer,sizeof(buffer));
	       	printf("%s\n",buffer);
	    }
	    else if(strcmp(command,"ls\n")!=0
	    	&&check_cat(command)!=0
	    	&&check_rm(command)!=0
	    	&&strcmp(command,"clear\n")!=0){
	       	printf("Command not found\n");
	    }
	    //Forking program:
	    pid_t pid;
	    pid = fork();
	    if (pid < 0){
	      	perror("Error\n");
	      	return -1;
	    }
	    else if (pid == 0){
	      	if(check_cat(command)==0){
	      		/*This is for cat directory.*/
	      		int i = 4, j = 0;
	      		char after_cat[100];
	      		int num;
	        	for(num =0; num <100;num++){
					after_cat[num]=0;
				}
	      		for(; command[i]!='\n'; i++,j++){
	      			after_cat[j] = command[i];
	      		}
				char* arg[] = {"cat", after_cat, 0};
				execvp(arg[0],arg);
	      	}
			else if(strcmp(command,"ls\n")==0){
				/*This is for listing directory contents.*/
	      		char* arg[] = {"ls", 0};
				execvp(arg[0],arg);
	      	}
	      	else if(check_rm(command)==0){
	      		/*This is for removing files.*/
	      		int i = 3, j = 0;
	      		char after_rm[100];
	      		int num;
	        	for(num =0; num <100;num++){
					after_rm[num]=0;
				}
	      		for(; command[i]!='\n'; i++,j++){
	      			after_rm[j] = command[i];
	      		}
				char* arg[] = {"rm", after_rm, 0};
				execvp(arg[0],arg);
	      	}
	      	else if(strcmp(command,"clear\n")==0){
	      		char* arg[] = {"clear", 0};
				execvp(arg[0],arg);
	      	}
	      	else exit(0);
	    }
	    else{
	      	waitpid(-1,NULL,0);
	    }
	}
	return 0;
}
