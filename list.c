#include<stdio.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

int command[6]={0,0,0,0,0,0};
int max,min,day;
char path[256]="./";
struct stat st;
void list(char path[]);
void test(struct stat st,char name[]);
int main(int argc,char *argv[])
{
    for(int i=1;i<argc;i++)
	{
		if(strcmp("-r",argv[i])==0) command[0]=1;
		else if(strcmp("-a",argv[i])==0) command[1]=1;
		else if(strcmp("-l",argv[i])==0)
		{
			min=atoi(argv[i+1]);
			command[2]=1;
			i++;
		}
		else if(strcmp("-h",argv[i])==0)
		{
			max=atoi(argv[i+1]);
			command[3]=1;
			i++;
		}
		else if(strcmp("-m",argv[i])==0)
		{
			day=atoi(argv[i+1]);
            command[4]=1;
            i++;
	
		}
		else if(strcmp("--",argv[i])==0) break;
		else if(argv[i][0]=='-')
		{
			int l=strlen(argv[i]);
			for(int j=1;j<l;j++)
			{
				if(argv[i][j]=='a')
				{
					command[1]=1;
				}
				if(argv[i][j]=='r')
				{
					command[0]=1;
				}
			}
		}
		else strcpy(path,argv[i]);
		
	}
	if(strcmp(path,"*")==0) strcpy(path,"./");
	if(stat(path,&st)==-1)
	{
		printf("路径不存在!\n");
		return 1;
	}
	
	if(S_ISDIR(st.st_mode))
	{
		list(path);
	}
	else
	{
		if(command[1]==1||(command[1]==0&&path[0]!='.'))
		{
			test(st,path);
		}
	}	
	
	printf("\n");
	return 0; 
}

void list(char path[])
{
	DIR *dir=opendir(path);
	struct dirent *myDirent=NULL;
	char temp[256]="";
	int ret=0;
	
	if(dir!=NULL)
	{
		while((myDirent=readdir(dir))!=NULL)
		{
			
			strcpy(temp,path);
			strcat(temp,myDirent->d_name);
			ret=stat(temp,&st);
			if(command[1]==0)
            {
                if(myDirent->d_name[0]=='.') continue;
            }

			if(!S_ISDIR(st.st_mode))
			{
                test(st,myDirent->d_name);
			}
			else
			{
				if(command[0]==1)
				{
					strcat(temp,"/");
					list(temp);
				}
			}
		}
	}
}

void test(struct stat st,char name[])
{
	int valid=1;
	time_t raw_time;
    if(command[3]==1)
   	{
        if(st.st_size>max) valid=0;
    }
    if(command[2]==1)
    {
        if(st.st_size<min) valid=0;
    }
    if(command[4]==1)
    {
		time(&raw_time);
        if((raw_time-st.st_mtime)/86400>=day) valid=0;
    }
    if(valid==1)
    {
        printf("%s\t",name);
	}

}
