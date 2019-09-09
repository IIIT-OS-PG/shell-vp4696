#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<iostream>
#include<string.h>
#include<fstream>
#include<stdlib.h>
#include<stdio.h>

using namespace std;

void init()
{
  struct passwd *p;
  uid_t  uid;
  int destination;
  ofstream fp;
  char temp[100];
  const char* t;
  char cdir[100];
  char *base;

  getcwd(cdir, sizeof(cdir));
  base = basename(cdir);

  if ((p = getpwuid(uid = getuid())) == NULL)
  { 

  } 
  else {

  	string line; 

  	ofstream fout; 

 	fout.open("mbash.txt"); 
  
    fout<<"USERNAME: "<<p->pw_name << endl; 
    fout<<"uid: "<<p->pw_uid << endl; 
    fout<<"HOME: "<<p->pw_dir << endl; 
    fout<<"PATH: bash: /usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin"<<endl; 
    fout<<"PS1: "<<"~/"<<base;
    fout.close(); 

    // ifstream fin;
    // fin.open("mbash.txt");  
    // while (!fin.eof()) { 
    //     getline(fin, line); 
    //     cout << line << endl; 
    // } 
    // fin.close(); 

  }

}
