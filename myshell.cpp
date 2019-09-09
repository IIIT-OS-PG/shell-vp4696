#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX 100

#include "parser.h"
#include "init.h"

using namespace std;

pid_t child_pid;
int stat_loc;

void err_sys(const char* x) 
{ 
    perror(x); 
    exit(0); 
}

int cd(char *path) 
{
    return chdir(path);
}

void redirectIO(char **, int);
int executePipe(char **, int); 

int main() {
    
                char input[MAX];

                int ic=0;
                int noOfPipes=0, op_single_redirection=0, ip_single_redirection=0, op_double_redirection=0,ip_double_redirection=0;    

                char cdir[MAX];
                char olddir[MAX];
                char *base;
                char *home;

                init();
 
                getcwd(cdir, sizeof(cdir));
                getcwd(olddir, sizeof(olddir));
                base = basename(cdir);
                cout<<":"<<base<<"~/"<<"$";

                while(1) {
        
                            char* ip[MAX];
       
                            fgets(input, 200 , stdin);
        
                            if(input[0] == '\n') 
                            {
                                        getcwd(cdir, sizeof(cdir));
                                        base = basename(cdir);
                                        chdir(cdir);
                                        cout<<":"<<base<<"~/"<<"$";  
                                        continue;       
                            }

                            int count = parser(input, ip);

                            if(strcmp(ip[0], "exit") == 0)
                            {
                               break;
                            }

                            if(strcmp(ip[0], "echo") == 0 && strcmp(ip[1], "$PATH") == 0)
                            {

                                FILE* statusf;
                                char * pp;
                                char c[50];

                                strcpy(c,"PATH");

                                statusf = fopen("mbash.txt", "r"); 
                             
                                while(fgets(c, 100, statusf)) 
                                {
                                            if(strncmp(c, "PATH:", 5) != 0)
                                                    continue;
                                            pp = c+6;
                                            cout<<pp<<endl;
                                            break;
                                }
                                
                                getcwd(cdir, sizeof(cdir));
                                base = basename(cdir);
                                cout<<":"<<base<<"~/"<<"$";
                                continue;    
                            }

                            if(strcmp(ip[0], "hostname")==0)
                            {
                                char hostname[1024];
                                
                                gethostname(hostname, 1024);

                                puts(hostname);
                                
                                getcwd(cdir, sizeof(cdir));
                                base = basename(cdir);
                                cout<<":"<<base<<"~/"<<"$";
                                continue;    
                            }        

                            if(strcmp(ip[0], "history")==0)
                            {
                                  
                            }

                            if(strcmp(ip[0], "export")==0)
                            {

                               if(false)
                               {

                                char * pp;
                                ofstream fout; 
                                struct passwd *p;

                                string str(pp);  
                                string ctt;
                                getline(cin,ctt);

                                ctt.erase(ctt.begin(),ctt.begin()+18);

                                str.append(ctt);

                                getcwd(cdir, sizeof(cdir));
                                base = basename(cdir);

                                fout.open("mbash.txt"); 
  
                                fout<<"USERNAME:"<<p->pw_name << endl; 
                                fout<<"uid:"<<p->pw_uid << endl; 
                                fout<<"HOME:"<<p->pw_dir << endl; 
                                fout<<"PATH:"<<str<<endl; 
                                fout<<"PS1:"<<base<<"~/"<<"$";
                                fout.close();  
                               }

                                getcwd(cdir, sizeof(cdir));
                                base = basename(cdir);
                                cout<<":"<<base<<"~/"<<"$";
                                continue; 
                            }                                                

                            for(ic=0;ic<count;ic++)                   //Here, calculating number of pipes(if any!!!)
                            {
                                    if(!strcmp(ip[ic], "|"))
                                    {
                                        noOfPipes++;
                                    }
                            }

                            for(ic=0;ic<count;ic++)                    //Here, calculating number of <(if any!!!)
                            {
                                    if(!strcmp(ip[ic], "<"))
                                    {
                                        ip_single_redirection++;
                                    }
                            }

                            for(ic=0;ic<count;ic++)                     //Here, calculating number of >(if any!!!)
                            {
                                    if(!strcmp(ip[ic], ">"))
                                    {
                                        op_single_redirection++;
                                    }
                            }

                            for(ic=0;ic<count;ic++)                      //Here, calculating number of >>(if any!!!)
                            {
                                    if(!strcmp(ip[ic], ">>"))
                                    {
                                        op_double_redirection++;
                                    }
                            }

                            for(ic=0;ic<count;ic++)                       //Here, calculating number of <<(if any!!!)
                            {
                                    if(!strcmp(ip[ic], "<<"))
                                    {
                                        ip_double_redirection++;
                                    }
                            }

                            // cout<<"Pipes:"<<noOfPipes<<endl;
                            // cout<<">"<<op_single_redirection<<endl;
                            // cout<<"<"<<ip_single_redirection<<endl;
                            // cout<<">>"<<op_double_redirection<<endl;
                            // cout<<"<<"<<ip_double_redirection<<endl;
        
                            if(noOfPipes == 0 && ip_single_redirection==0 && op_single_redirection==0 && op_double_redirection==0 && ip_double_redirection==0)
                            {
                                if (strcmp(ip[0], "cd") == 0) 
                                {

                                                if(!ip[1])
                                                {
                                                        getcwd(olddir, sizeof(olddir));
                                                        chdir("/");
                                                        cout<<"~/"<<"$";
                                                        continue;
                                                }

                                                else if(strcmp(ip[1], "~") == 0)
                                                {
                                                        getcwd(olddir, sizeof(olddir));
                                                        string sz="/";
                                                        strcpy(ip[1], sz.c_str());
                                                        chdir(ip[1]);
                                                        cout<<ip[1]<<"~"<<"$";
                                                        continue;
                                                }
                    
                                                else if(strcmp(ip[1], "-") == 0)
                                                {
                                                        base = basename(olddir);
                                                        cout<<olddir<<endl;
                                                        chdir(olddir);
                                                        cout<<":"<<base<<"~/"<<"$";  
                                                        continue;       
                                                }

                                                else 
                                                {
                                                    //perror(ip[1]);
                                                    getcwd(olddir, sizeof(olddir));
                                                    if(cd(ip[1])<0)
                                                    {
                                                        cout<<"Error while changing directory";
                                                    }
                                                }
                                }    
                                else
                                {
                                                pid_t pid = fork();
    
                                                if (pid == -1) 
                                                {
                                                        cout<<"Error while creating child";
                                                }
                                                else if (pid == 0)                                        //It's a child process
                                                {
                                                        if(execvp(ip[0], ip)<0)
                                                        {
                                                            cout<<"No such command found!!!"<<endl;
                                                            exit(1);
                                                        }
                                                }
                                                else                                                      //It's a parent process
                                                {
                                                        int childStatus;
                                                        waitpid(pid, &childStatus, 0);
                                                }
                                }

                            }                    
                            else
                            {
                                if(noOfPipes != 0 )
                                {
                                    executePipe(ip, noOfPipes);
                                }
                                        //   >                            <                            >>                            <<                    
                                else if(op_single_redirection!=0 || ip_single_redirection!=0 || op_double_redirection!=0 || ip_double_redirection!=0)
                                {
                                       redirectIO(ip , count); 
                                }
            
                            }

                            noOfPipes = 0;
                            ip_single_redirection=0;
                            op_single_redirection=0;
                            op_double_redirection=0;
                            ip_double_redirection=0;
                            
                            getcwd(cdir, sizeof(cdir));
                            base = basename(cdir);
                            chdir(base);
                            cout<<":"<<base<<"~/"<<"$";
        
                     }
return 0;
}

void redirectIO(char **ip,int count)
{
    int file_des,cnt=0;
    int op_single_redirection=0, ip_single_redirection=0, op_double_redirection=0, ip_double_redirection=0; 


    int tp=0;
    do                             
    {
        if(strcmp(ip[tp], "<") == 0)
        {
            ip_single_redirection++;                        //Here, checking ip_single_redi(if any!!!)
        }

        if(strcmp(ip[tp], ">") == 0)
        {
            op_single_redirection++;                        //Here, checking op_single_redi(if any!!!)
        }
    

        if(strcmp(ip[tp], ">>") == 0)
        {
            op_double_redirection++;                        //Here, checking op_double_redi(if any!!!)
        }
    
        if(strcmp(ip[tp], "<<") == 0)
        {
            ip_double_redirection++;                        //Here, checking ip_double_redi(if any!!!)
        }
        tp++;
    }while(tp<count);    


    // cout<<">"<<op_single_redirection<<endl;
    // cout<<"<"<<ip_single_redirection<<endl;
    // cout<<">>"<<op_double_redirection<<endl;
    // cout<<"<<"<<ip_double_redirection<<endl;


        int i=0;
        do
        {
            if(strcmp(ip[i],">") == 0)                                    //Removing > operator (if any)!!!
            {
                    ip[i]=NULL;
                    break;
            }
  
            if(strcmp(ip[i],"<") == 0)                                    //Removing < operator (if any)!!!
            {
                    ip[i]=NULL;
                    break;
            }
  
            if(strcmp(ip[i],">>") == 0)                                   //Removing >> operator (if any)!!!
            {
                    ip[i]=NULL;
                    break;
            }
  
            if(strcmp(ip[i],"<<") == 0)                                   //Removing << operator (if any)!!!
            {
                    ip[i]=NULL;
                    cnt=i+1;
                    break;
            }
            i++;
        } while (i<count); 
        
  char *f=ip[count-1];
  pid_t pid = fork();  

    if (pid == -1)
    {
        cout<<"Error while creating child!!!";
    }
    if(op_single_redirection==1)
        {

            if((file_des = open(f, O_CREAT | O_WRONLY | O_TRUNC, 0644)) < 0)
            {
                    cout<<"Error while opening file!!!";
            }

            dup2(file_des, 1);
            close(file_des);
            if(execvp(ip[0], ip)<0)
            {
                    cout<<"Error executing op single redirection!!!"<<endl;
                    exit(1);
            }
        }
    else if (pid == 0)
    {
        
        if(ip_single_redirection==1)
        {

            if((file_des = open(f,O_RDONLY, 0644)) < 0)
            {
                cout<<"Error while opening file!!!";
            }

            dup2(file_des, 0);
            close(file_des);
            if(execvp(ip[0], ip)<0)
            {
                    cout<<"Error executing ip single redirection!!!"<<endl;
                    exit(1);
            }
       
        }
        else if(op_double_redirection==1)
        {
                
            if((file_des = open(f, O_APPEND|O_CREAT, 0644)) < 0)
            {
                cout<<"Error while opening file!!!";
            }

            dup2(file_des, 1);
            close(file_des);
            if(execvp(ip[0], ip)<0)
            {
                    cout<<"Error executing op double redirection!!!"<<endl;
                    exit(1);
            }
        }
        else if(ip_double_redirection==1)
        {

                    string s[20];
                    int z=0;
                    getline (cin, s[z]);
                    z++;
                    while(s[z-1] != ip[cnt])
                    {
                        getline (cin, s[z]);
                        z++;
                    }

                    for(int j=0;j<z-1;j++)
                    {
                        cout<<s[j]<<endl;
                    } 
        }
  }
else
{  
       wait(NULL);  
}

}


int executePipe(char **a, int tot_pipes) 
{
    
    int cmds=tot_pipes+1,pipes,i=0,fd[2*tot_pipes],pid,status;
    int iter=0,p=0,zz=1,start_cmd,cs[20],temp=0,close_fd=0;
    
        cs[0]=0;
        while(a[p]!=NULL)                           //Removing pipes and seperating the commands!!!
        {
            pipes=0;
            if(!strcmp(a[p], "|"))
            {
                //cout<<"*****"<<a[p];
                a[p]=NULL;
                //cout<<"*****"<<cs[zz];
                cs[zz]=p+1;
                pipes=p+1;
                zz++;
            }
            p++;
        }

       int temp2;
       for(i=0;i<tot_pipes;i++)
       {
          if (pipes!=0)
          {
               pipes++;
          }
          temp2=i*2;
          if(pipe(fd + temp2) < temp) 
          {
                cout<<"There was some error executing pipe!!!";
          }
       }

        i=0;
        do
        {
        int filedes[tot_pipes];
        start_cmd=cs[i];        
        pid=fork();
       
        if(pid == temp) 
        {
            
            if(i<tot_pipes)
            {
                filedes[i]=pipes;
                if(dup2(fd[iter + 1], 1) < temp)
                {
                    cout<<"Some error while calling dup2!!!";
                }
            }

            if(iter!=0)
            {
                if (pipes!=0)
                {
                        filedes[i]=pipes;
                }
                if(dup2(fd[iter-2], temp) < temp)
                {
                    cout<<"Some error while calling dup2!!!";
                }
            }

            close_fd=0;
            do
            {
                close(fd[close_fd]);
                close_fd++;

            }while(close_fd<2*tot_pipes);
           
            if( execvp(a[start_cmd], a+start_cmd) < temp)
            {
                cout<<"Some error executing commands after seperating from pipes!!!";
            }
        }
        else if(pid < temp)
        {
            cout<<"Some error in creating child!!!";
        }
        
        iter=iter+2;
        i++;

       }while(i<cmds); 
        
    close_fd=0;
    do
    {
        close(fd[close_fd]);
        close_fd++;

    }while(close_fd<2*tot_pipes);

    for(i = 0; i < tot_pipes + 1; i++)
    {
        waitpid(child_pid, &stat_loc, temp);
    }
    
    return 0;
}