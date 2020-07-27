/* This file contains the main code */

#include<iostream.h> 
#include<fstream.h>
#include<stdio.h>
#include<iomanip.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include<dir.h>
#include<dos.h>
#include<bios.h>
#include<graphics.h>

void screen();
void drawdoublelinebox(int,int,int,int);
void drawsinglelinebox(int,int,int,int);
void normal();


#define datafile "E.txt"
#define recsize 78
#define max 59
#define bucketsize 3
int hash(char a[]);
fstream file;
char buffer[100];

class Loan
{
public:
  char account_no[12],bank_id[10],cust_name[12],phone[12],occupation[7],loan_type[7],branch_id[8],ROI[5];
public:
	void initial();
	void read();
        void pack();
        void unpack();
	int  retrieve(int addr,char k[],int i);
	void datadisp();
	int  remove(int addr,char k[]);
	void modify(int addr ,char k[]);
};

int myatoi(char *k)
{
   int res=0;
   for(int i=0;k[i]!='\0';++i)
   {
      if(k[i]==58)
	 continue;
      res=res*10+k[i]-48;
   }
   //cout<<res;
   return res;
}

void opener(fstream &file,char *fn,int mode)
{
	file.open(fn,mode);
	if(!file)
	{
		cout<<"UNABLE TO OPEN THE FILE\n"<<fn;
		getch();
		exit(1);
	}
}

void Loan::initial()
{
	int i,j;
	file.open(datafile,ios::in);
	if(!file)
	{
		file.open(datafile,ios::out);
		if(!file)
		{
			cout<<"UNABLE TO OPEN THE FILE\n";
			getch();
			exit(1);
		}
		for(i=0;i<max;i++)
		{
			file.seekp(i*recsize*3,ios::beg);
			for(j=0;j<(recsize*3)-2;j++)
			   file<<"#";
			file<<endl;
		}
		cout<<"EMPTY FILE CREATED\n";
	}
	file.close();
	return;
}
void Loan::read()
{
	int addr,len,len1,len2,len3,len4,len5;
	char acc_no[12],dbank_id[10],dbranch_id[8];

   LABEL:
	cout<<"\n\t\t\tENTER THE ACCOUNT NUMBER: ";
	gets(acc_no);
	len=strlen(acc_no);
      	addr=hash(acc_no);
	if(len!=8)
	{
		cout<<"\n\t  ENTER THE VALID ACCOUNT NUMBER(8 CHARACTERS): ";
		goto LABEL;
	}

	if(retrieve(addr,acc_no,0)>0)
	{	
		cout<<"!!*** ACCOUNT NUMBER IS ALREADY PRESENT, WE CANNOT ADD IT TO THE HASH FILE ***!!\n";
		goto LABEL;
	}
	file.close();
	strcpy(account_no,acc_no);

      DESC:
	cout<<"\n\t\t\tENTER BANK ID: ";
	gets(dbank_id);
	len1=strlen(dbank_id);
	if(len1!=8||dbank_id[0]>63||dbank_id[1]<63||dbank_id[2]<63||dbank_id[3]<63 ||dbank_id[4]<63 ||dbank_id[5]<63||dbank_id[6]<63||dbank_id[7]<63||dbank_id[8]>63)
	{
		cout<<"\n\t  ENTER THE VALID BANK ID(8 CHARACTERS): ";
		goto DESC;
	}
	file.close();
	strcpy(bank_id,dbank_id);

     NAME:
	cout<<"\n\t\t\tENTER THE CUSTOMER NAME: ";
	gets(cust_name);
	int i=0;
		while(cust_name[i]!='\0')
		{
			 if(cust_name[i]<63||i>9)
			{
				  cout<<"\n\t  ENTER THE VALID CUSTOMER NAME(LESS THAN 10 CHARACTERS): ";
				 goto NAME;
			}
			i++;
		}

     PH:
	i=0;
	cout<<"\n\t\t\tENTER THE PHONE: ";
	gets(phone);
	len3=strlen(phone);
	while(phone[i]!='\0')
	{
	    if(phone[i]>63 ||i>9 ||len3!=10)
	    {
	      cout<<"\n\t  ENTER VALID PHONE NUMBER: ";
	      goto PH;
	    }
	      i++;
	}
  /* if(i!=11)
{
     cout<<"\n\t  ENTER VALID PHONE NUMBER: ";
	      goto PH;

}*/
    OCC:
	int ch;
	cout<<"\n\t\t\tENTER THE OCCUPATION: \n\t\t\t1.Government\n\t\t\t2.Private\n\t\t\t3.Others\n\t\t\t ";
	cin>>ch;
	switch(ch){
		case 1: strcpy(occupation,"Govt");
			break;
		case 2: strcpy(occupation,"prvt");
			break;
		case 3: strcpy(occupation,"others");
			break;
		default: goto OCC;
	}

     LOAN:
	cout<<"\n\t The loan types are:\n ";
   cout<<"\n\t\t\t 1.Education loan";
   cout<<"\n\t\t\t 2.Home loan";
   cout<<"\n\t\t\t 3.Car loan\n";
   int chi;
   cout<<"\n\t\t\t Enter choice:\t ";
   cin>>chi;
   if(chi==1)
   {
	strcpy(loan_type,"Edu");
	strcpy(ROI,"10");
   }
   else if(chi==2)
   {
	strcpy(loan_type,"Home");
	strcpy(ROI,"12");
   }
   else
   {
	strcpy(loan_type,"Car");
	strcpy(ROI,"18");
   }


    BRANCH:
	cout<<"\n\t\t\tENTER THE BRANCH ID: ";
	gets(dbranch_id);
	len2=strlen(dbranch_id);
	if(len2!=5)
	{
		cout<<"\n\t  ENTER THE VALID BRANCH ID(5 CHARACTERS): \n";
		goto BRANCH;
	}
	file.close();
	strcpy(branch_id,dbranch_id);

	pack();
	return;
}
void Loan::pack()
{
	strcpy(buffer,account_no);		strcat(buffer,"|");
	strcat(buffer,bank_id);			strcat(buffer,"|");
	strcat(buffer,cust_name);		strcat(buffer,"|");
	strcat(buffer,phone);			strcat(buffer,"|");
	strcat(buffer,occupation);		strcat(buffer,"|");
	strcat(buffer,loan_type);		strcat(buffer,"|");
	strcat(buffer,branch_id);		strcat(buffer,"|");
	strcat(buffer,ROI);        		strcat(buffer,"|");
}

int hash(char account_no[])
{
int i=0,sum=0,c,len;
len=strlen(account_no);
if(len%2==1)
    len++;
for(i=0;i<len;i++)
    sum=(sum+((account_no[i]-48)));
c=sum%max;
cout<<"\n\t\t    HASH KEY GENERATED IS: "<<c<<endl;
return c;
}

void store(int addr)
{
char dummy[10];
int flag=0,i;
file.open(datafile,ios::in|ios::out);
file.seekg(addr*recsize*bucketsize,ios::beg);
file.getline(dummy,5,'\n');
if(strcmp(dummy,"####")==0 ||strcmp(dummy,"$$$$")==0)
{
	file.seekp(addr*recsize*bucketsize,ios::beg);
	file<<buffer;
	clrscr();
	textcolor(YELLOW);
	textbackground(BLACK);
	cout<<"\n\n\n\n";
	for(int q=0;q<80;q++) cout<<'~';
	for(int w=0;w<80;w++) cout<<'*';
	cout<<"\n\n\t\t   LOAN RECORD INSERTED SUCCESSFULLY....!!\n\n";
	for(int e=0;e<80;e++) cout<<'*';
	cout<<"\n";
	for(int r=0;r<80;r++) cout<<'~';
	getch();
	flag=1;
}
file.seekg(addr*recsize*bucketsize+recsize,ios::beg);
file.getline(dummy,5,'\n');
if((strcmp(dummy,"####")==0||strcmp(dummy,"$$$$")==0) && flag!=1)
{
	file.seekp(addr*recsize*bucketsize+recsize,ios::beg);
	file<<buffer;clrscr();
	textcolor(YELLOW);
	textbackground(BLACK);
	cout<<"\n\n\n\n";
	for(int q=0;q<80;q++) cout<<'~';
	for(int w=0;w<80;w++) cout<<'*';
	cout<<"\n\n\t\t   LOAN RECORD INSERTED SUCCESSFULLY....!!\n\n";
	for(int e=0;e<80;e++) cout<<'*';
	cout<<"\n";
	for(int r=0;r<80;r++) cout<<'~';
	getch();
	flag=1;
}
file.seekg(addr*recsize*bucketsize+(2*recsize),ios::beg);
file.getline(dummy,5,'\n');
if((strcmp(dummy,"####")==0||strcmp(dummy,"$$$$")==0)&&flag!=1)
{
	file.seekp(addr*recsize*bucketsize+(2*recsize),ios::beg);
	file<<buffer;
	clrscr();
	textcolor(YELLOW);
	textbackground(BLACK);
	cout<<"\n\n\n\n";
	for(int q=0;q<80;q++) cout<<'~';
	for(int w=0;w<80;w++) cout<<'*';
	cout<<"\n\n\t\t   LOAN RECORD INSERTED SUCCESSFULLY....!!\n\n";
	for(int e=0;e<80;e++) cout<<'*';
	cout<<"\n";
	for(int r=0;r<80;r++) cout<<'~';
	getch();
	flag=1;
}
if(flag!=1)
{
for(i=addr+1;i!=addr;i++)
{
	if(i%max==0)
		i=0;
	file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")==0||strcmp(dummy,"$$$$")==0)
	{
		clrscr();
		textcolor(YELLOW);
		textbackground(BLACK);		
		cout<<"\n\n\n\n";
		for(int t=0;t<80;t++) cout<<'~';
		for(int y=0;y<80;y++) cout<<'*';
		cout<<"\n\n\t\t\tCOLLISION HAS OCCURED....!!\n\n";
		cout<<"\t      HOME ADDRESS IS: "<<addr<<" & ACTUAL ADDRESS IS: "<<i<<"\n";
		file.seekp(i*recsize*bucketsize,ios::beg);
		file<<buffer;
		textcolor(YELLOW);
		textbackground(BLACK);
		cout<<"\n\n\t\t   LOAN RECORD INSERTED SUCCESSFULLY....!!\n\n";
		for(int u=0;u<80;u++) cout<<'*';
		cout<<"\n";
		for(int f=0;f<80;f++) cout<<'~';
		getch();
		flag=1;
		break;
	}
	file.seekg(i*recsize*bucketsize+recsize,ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")==0||strcmp(dummy,"$$$$")==0)
	{
		clrscr();
		textcolor(YELLOW);
		textbackground(BLACK);	
		cout<<"\n\n\n\n";
		for(int t=0;t<80;t++) cout<<'~';
		for(int y=0;y<80;y++) cout<<'*';
		cout<<"\n\n\t\t\tCOLLISION HAS OCCURED....!!\n\n";
		cout<<"\t      HOME ADDRESS IS: "<<addr<<" & ACTUAL ADDRESS IS: "<<i<<"\n";
		file.seekp(i*recsize*bucketsize+recsize,ios::beg);
		file<<buffer;
		textcolor(YELLOW);
		textbackground(BLACK);
		cout<<"\n\n\t\t   LOAN RECORD INSERTED SUCCESSFULLY....!!\n\n";
		for(int u=0;u<80;u++) cout<<'*';
		cout<<"\n";
		for(int f=0;f<80;f++) cout<<'~';
		getch();
		flag=1;
		break;
	}
	file.seekg(i*recsize*bucketsize+(2*recsize),ios::beg);
	file.getline(dummy,5,'\n');
       if(strcmp(dummy,"####")==0||strcmp(dummy,"$$$$")==0)
	{
		clrscr();
		textcolor(YELLOW);
		textbackground(BLACK);	
		cout<<"\n\n\n\n";
		for(int t=0;t<80;t++) cout<<'~';
		for(int y=0;y<80;y++) cout<<'*';
		cout<<"\n\n\t\t\tCOLLISION HAS OCCURED....!!\n\n";
		cout<<"\t      HOME ADDRESS IS: "<<addr<<" & ACTUAL ADDRESS IS: "<<i<<"\n";
		file.seekp(i*recsize*bucketsize+(2*recsize),ios::beg);
		file<<buffer;
		textcolor(YELLOW);
		textbackground(BLACK);
		cout<<"\n\n\t\t   LOAN RECORD INSERTED SUCCESSFULLY....!!\n\n";
		for(int u=0;u<80;u++) cout<<'*';
		cout<<"\n";
		for(int f=0;f<80;f++) cout<<'~';
		getch();
		flag=1;
		break;
	}
}
}
if(i==addr && (flag!=1))
{
	clrscr();
	textcolor(YELLOW);
	textbackground(BLACK);
	cout<<"\n\n\n\n";
	for(int o=0;o<80;o++) cout<<'~';
	for(int a=0;a<80;a++) cout<<'*';
	cout<<setw(23)<<" "<<"\n\n\t*** HASH FILE IS FULL, LOAN RECORD CANT BE INSERTED ***\n\n";
	for(int s=0;s<80;s++) cout<<'*';
	for(int d=0;d<80;d++) cout<<'~';
	getch();
}
getch();
file.close();
return;
}

void Loan::unpack()
{
	char dummy[100];
	file.getline(bank_id,10,'|');
	file.getline(cust_name,12,'|');
	file.getline(phone,12,'|');
	file.getline(occupation,7,'|');
	file.getline(loan_type,7,'|');
	file.getline(branch_id,8,'|');
	file.getline(ROI,5,'|');
	file.getline(dummy,100,'#');
}

int Loan::retrieve(int addr,char k[],int l)  //search
{
int found=0,i;
char dummy[10];
i=addr;
file.open(datafile,ios::in);
cout<<setiosflags(ios::left);
//if(l==1)
do
{
	file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")==0)
		break;
    	file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(account_no,10,'|');
	if(strcmp(account_no,k)==0)
	{
		found=1;
		if(l==1)
		{
		unpack();
		clrscr();
		textcolor(15);
	      textbackground(YELLOW);		
	   cout<<endl;   
	   cout<<setw(18)<<" "<<"THE SEARCHED LOAN RECORD IS DISPLAYED"<<endl<<endl;
	   for(int f=0;f<80;f++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ACCOUNT NUMBER      :  "<<setw(10)<<account_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" BANCK ID            :  "<<setw(9)<<bank_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME       :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" OCCUPATION          :  "<<setw(5)<<occupation<<endl<<endl;
	   cout<<setw(20)<<" "<<" LOAN TYPE           :  "<<setw(5)<<loan_type<<endl<<endl;
	   cout<<setw(20)<<" "<<" BRANCH ID   I       :  "<<setw(6)<<branch_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" RATE OF INTEREST    :  "<<setw(5)<<ROI<<endl<<endl;
	   for(int g=0;g<80;g++) cout<<'*';
	   cout<<endl<<setw(25)<<" ";
	  			
	   cout<<"LOAN RECORD FOUND...!!"<<endl;		
		}
		break;
	}
	file.seekg(i*recsize*bucketsize+recsize,ios::beg);
	file.getline(account_no,10,'|');
	if(strcmp(account_no,"#########")==0)
	      break;
	if(strcmp(account_no,k)==0)
	{
		found=1;
		if(l==1)
		{
		unpack();
		clrscr();
		textcolor(15);
	      textbackground(YELLOW);	
	   cout<<endl;   
	   cout<<setw(18)<<" "<<"THE SEARCHED LOAN RECORD IS DISPLAYED"<<endl<<endl;
	   for(int f=0;f<80;f++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ACCOUNT NUMBER      :  "<<setw(10)<<account_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" BANCK ID            :  "<<setw(9)<<bank_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME       :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" OCCUPATION          :  "<<setw(5)<<occupation<<endl<<endl;
	   cout<<setw(20)<<" "<<" LOAN TYPE           :  "<<setw(5)<<loan_type<<endl<<endl;
	   cout<<setw(20)<<" "<<" BRANCH ID   I       :  "<<setw(6)<<branch_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" RATE OF INTEREST    :  "<<setw(5)<<ROI<<endl<<endl;
	   for(int g=0;g<80;g++) cout<<'*';
	   cout<<endl<<setw(25)<<" ";
	  			
	cout<<"LOAN RECORD FOUND...!!"<<endl;
		}		
		break;
	}

	file.seekg(i*recsize*bucketsize+(2*recsize),ios::beg);
	file.getline(account_no,10,'|');
	if(strcmp(account_no,"#########")==0)
	      break;
	if(strcmp(account_no,k)==0)
	{
		found=1;
		if(l==1)
		{
		unpack();
		clrscr();
		textcolor(15);
	      textbackground(YELLOW);	
	   cout<<endl;   
	   cout<<setw(18)<<" "<<"THE SEARCHED LOAN RECORD IS DISPLAYED"<<endl<<endl;
	   for(int f=0;f<80;f++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ACCOUNT NUMBER      :  "<<setw(10)<<account_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" BANCK ID            :  "<<setw(9)<<bank_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME       :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" OCCUPATION          :  "<<setw(5)<<occupation<<endl<<endl;
	   cout<<setw(20)<<" "<<" LOAN TYPE           :  "<<setw(5)<<loan_type<<endl<<endl;
	   cout<<setw(20)<<" "<<" BRANCH ID   I       :  "<<setw(6)<<branch_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" RATE OF INTEREST    :  "<<setw(5)<<ROI<<endl<<endl;
	   for(int g=0;g<80;g++) cout<<'*';
	   cout<<endl<<setw(25)<<" ";
	  			
	cout<<"LOAN RECORD FOUND...!!"<<endl;
		}		
		break;
	}
	else
	{
		i++;
		if(i%max==0)
			i=0;
	}
}while(i!=addr);
if(found==0 && l==1)
{
		textcolor(15);
	      textbackground(YELLOW);
	cout<<setw(23)<<" "<<"\n\n\t!!*** GIVEN LOAN RECORD DOESNOT EXISTS IN HASH FILE ***!!\n\n";
	for(int h=0;h<80;h++) cout<<'~';
	for(int j=0;j<80;j++) cout<<'~';
	getch();
}
	/*if(l==0)
	    cout<<found<<endl;*/

return found ;
}

void Loan::datadisp()
{
	file.open(datafile,ios::in);
	int i=0;
	char dummy[10];
	cout<<setiosflags(ios::left);
	while(i<max)
	{
	 if(file.eof())
		break;
	file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")!=0 && strcmp(dummy,"$$$$")!=0)
	{
		file.seekg(i*recsize*bucketsize,ios::beg);
		file.getline(account_no,10,'|');
		unpack();
		clrscr();
	      //drawdoublelinebox(17,7,60,30);
	      textcolor(15);
	      textbackground(RED);	
	   cout<<endl<<endl<<endl;
	   cout<<setw(25)<<" "<<"DISPLAYING LOAN RECORDS"<<endl<<endl;
	   for(int i=0;i<80;i++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ACCOUNT NUMBER      :  "<<setw(10)<<account_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" BANCK ID            :  "<<setw(9)<<bank_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME       :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" OCCUPATION          :  "<<setw(5)<<occupation<<endl<<endl;
	   cout<<setw(20)<<" "<<" LOAN TYPE           :  "<<setw(5)<<loan_type<<endl<<endl;
	   cout<<setw(20)<<" "<<" BRANCH ID   I       :  "<<setw(6)<<branch_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" RATE OF INTEREST    :  "<<setw(5)<<ROI<<endl<<endl;
	   for(int k=0;k<80;k++) cout<<'*';
	   cout<<endl<<endl<<setw(20)<<" ";
	   getch();	

	}
	file.seekg(i*recsize*bucketsize+recsize,ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")!=0 && strcmp(dummy,"$$$$")!=0)
	{
		 file.seekg(i*recsize*bucketsize+recsize,ios::beg);
		file.getline(account_no,10,'|');
		unpack();
	     clrscr();
		textcolor(15);
	      textbackground(RED);		
	   cout<<endl<<endl<<endl;
	   cout<<setw(25)<<" "<<"DISPLAYING LOAN RECORDS"<<endl<<endl;
	   for(int i=0;i<80;i++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ACCOUNT NUMBER      :  "<<setw(10)<<account_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" BANCK ID            :  "<<setw(9)<<bank_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME       :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" OCCUPATION          :  "<<setw(5)<<occupation<<endl<<endl;
	   cout<<setw(20)<<" "<<" LOAN TYPE           :  "<<setw(5)<<loan_type<<endl<<endl;
	   cout<<setw(20)<<" "<<" BRANCH ID   I       :  "<<setw(6)<<branch_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" RATE OF INTEREST    :  "<<setw(5)<<ROI<<endl<<endl;
	   for(int k=0;k<80;k++) cout<<'*';
	   cout<<endl<<endl<<setw(20)<<" ";
	   getch();	


	}
	file.seekg(i*recsize*bucketsize+(2*recsize),ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")!=0 && strcmp(dummy,"$$$$")!=0)
	{
		file.seekg(i*recsize*bucketsize+(2*recsize),ios::beg);
		file.getline(account_no,10,'|');
		unpack();
		clrscr();
		textcolor(15);
	      textbackground(RED);		
	   cout<<endl<<endl<<endl;
	   cout<<setw(25)<<" "<<"DISPLAYING LOAN RECORDS"<<endl<<endl;
	   for(int i=0;i<80;i++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ACCOUNT NUMBER      :  "<<setw(10)<<account_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" BANCK ID            :  "<<setw(9)<<bank_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME       :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" OCCUPATION          :  "<<setw(5)<<occupation<<endl<<endl;
	   cout<<setw(20)<<" "<<" LOAN TYPE           :  "<<setw(5)<<loan_type<<endl<<endl;
	   cout<<setw(20)<<" "<<" BRANCH ID   I       :  "<<setw(6)<<branch_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" RATE OF INTEREST    :  "<<setw(5)<<ROI<<endl<<endl;
	   for(int k=0;k<80;k++) cout<<'*';
	   cout<<endl<<setw(20)<<" ";
	   getch();	
	}
	i++;
	}
	clrscr();
	textcolor(15);
	      textbackground(RED);
	cout<<"\n\n\n\n\n\n\n\n\n\t\t=============================================\n"<<endl;	
	cout<<"\t\t     !!*** NO RECORDS TO DISPLAY ***!!"<<endl;
	cout<<"\n\t\t=============================================\n"<<endl;	
	getch();
	file.close();
}



int Loan::remove(int addr,char k[])
{
int found=0,i;
char dummy[10];
i=addr;
int s;

file.open(datafile,ios::in|ios::out);
cout<<setiosflags(ios::left);
do
{
	file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")==0)
		break;
        file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(account_no,10,'|');
	if(strcmp(account_no,"#########")==0)
	      break;
	if(strcmp(account_no,k)==0)
	{
		found=1;
		unpack();
		clrscr();
		textcolor(15);
	      textbackground(1);		
	   cout<<endl<<endl;
	   cout<<setw(26)<<" "<<"LOAN RECORD DISPLAYED"<<endl;
	   for(int l=0;l<80;l++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ACCOUNT NUMBER      :  "<<setw(10)<<account_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" BANCK ID            :  "<<setw(9)<<bank_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME       :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" OCCUPATION          :  "<<setw(5)<<occupation<<endl<<endl;
	   cout<<setw(20)<<" "<<" LOAN TYPE           :  "<<setw(5)<<loan_type<<endl<<endl;
	   cout<<setw(20)<<" "<<" BRANCH ID   I       :  "<<setw(6)<<branch_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" RATE OF INTEREST    :  "<<setw(5)<<ROI<<endl<<endl;
	   for(int z=0;z<80;z++) cout<<'*';
	   cout<<endl<<setw(20)<<" ";
	   	
		textcolor(15);
	      textbackground(1);	
		cout<<"LOAN RECORD TO BE DELETED IS FOUND"<<endl;
		file.seekp(i*recsize*bucketsize,ios::beg);
	      for(s=0;s<recsize;s++)
		   file.put('$');
		cout<<setw(25)<<" "<<"  RECORD IS DELETED... !!"<<endl;
		getch();
		file.close();
		break;
	}
	file.seekg(i*recsize*bucketsize+recsize,ios::beg);
	file.getline(account_no,10,'|');
		if(strcmp(account_no,"#########")==0)
		 break;
	if(strcmp(account_no,k)==0)
	{
		found=1;
		unpack();
		clrscr();
		textcolor(15);
	      textbackground(1);		
	   cout<<endl<<endl;   
	   cout<<setw(26)<<" "<<"LOAN RECORD DISPLAYED"<<endl;
	   for(int l=0;l<80;l++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ACCOUNT NUMBER      :  "<<setw(10)<<account_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" BANCK ID            :  "<<setw(9)<<bank_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME       :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" OCCUPATION          :  "<<setw(5)<<occupation<<endl<<endl;
	   cout<<setw(20)<<" "<<" LOAN TYPE           :  "<<setw(5)<<loan_type<<endl<<endl;
	   cout<<setw(20)<<" "<<" BRANCH ID   I       :  "<<setw(6)<<branch_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" RATE OF INTEREST    :  "<<setw(5)<<ROI<<endl<<endl;
	   for(int z=0;z<80;z++) cout<<'*';
	   cout<<endl<<setw(20)<<" ";
	  
		textcolor(15);
	      textbackground(1);					
	cout<<"LOAN RECORD TO BE DELETED IS FOUND"<<endl;
		file.seekp(i*recsize*bucketsize+recsize,ios::beg);
	      for(s=0;s<recsize;s++)
		   file.put('$');
		cout<<setw(25)<<" "<<"  RECORD IS DELETED... !!"<<endl;
		getch();
		break;
	}

	file.seekg(i*recsize*bucketsize+(2*recsize),ios::beg);
	file.getline(account_no,10,'|');
	if(strcmp(account_no,k)==0)
	{
		found=1;
		unpack();
		clrscr();
		textcolor(15);
	      textbackground(1);			
	   cout<<endl<<endl;
	   cout<<setw(26)<<" "<<"LOAN RECORD DISPLAYED"<<endl;
	   for(int l=0;l<80;l++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ACCOUNT NUMBER      :  "<<setw(10)<<account_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" BANCK ID            :  "<<setw(9)<<bank_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME       :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" OCCUPATION          :  "<<setw(5)<<occupation<<endl<<endl;
	   cout<<setw(20)<<" "<<" LOAN TYPE           :  "<<setw(5)<<loan_type<<endl<<endl;
	   cout<<setw(20)<<" "<<" BRANCH ID   I       :  "<<setw(6)<<branch_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" RATE OF INTEREST    :  "<<setw(5)<<ROI<<endl<<endl;
	   for(int z=0;z<80;z++) cout<<'*';
	   cout<<endl<<setw(20)<<" ";		

		textcolor(15);
	      textbackground(1);		
		cout<<"LOAN RECORD TO BE DELETED IS FOUND"<<endl;
		file.seekp(i*recsize*bucketsize+(2*recsize),ios::beg);
	       for(s=0;s<recsize-2;s++)
		   file.put('$');
		   file.put('\n');
		cout<<setw(25)<<" "<<"  RECORD IS DELETED... !!"<<endl;
		getch();
		break;
	}
	else
	{
		i++;
		if(i%max==0)
			i=0;
	}
}while(i!=addr);
if(found==0)
{
	textcolor(15);
	      textbackground(1);
	cout<<setw(23)<<" "<<"\n\n\t!!*** GIVEN LOAN RECORD DOESNOT EXISTS IN HASH FILE ***!!\n\n";
	for(int k=0;k<80;k++) cout<<'^';
	for(int j=0;j<80;j++) cout<<'^';
	getch();
}
return found;
}

void Loan::modify(int addr, char k[])
{
int found=0,i;
char dummy[10];
i=addr;
int s;
int x;

file.open(datafile,ios::in|ios::out);
cout<<setiosflags(ios::left);
do
{
	file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")==0)
		break;
       file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(account_no,10,'|');
	if(strcmp(account_no,k)==0)
	{
		found=1;
		unpack();
		clrscr();
		textcolor(YELLOW);
	      textbackground(0);			
	   cout<<endl;
	   cout<<setw(18)<<" "<<"LOAN RECORD TO BE MODIFIED IS DISPLAYED"<<endl<<endl;
	   for(int c=0;c<80;c++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ACCOUNT NUMBER      :  "<<setw(10)<<account_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" BANCK ID            :  "<<setw(9)<<bank_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME       :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" OCCUPATION          :  "<<setw(5)<<occupation<<endl<<endl;
	   cout<<setw(20)<<" "<<" LOAN TYPE           :  "<<setw(5)<<loan_type<<endl<<endl;
	   cout<<setw(20)<<" "<<" BRANCH ID   I       :  "<<setw(6)<<branch_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" RATE OF INTEREST    :  "<<setw(5)<<ROI<<endl<<endl;
	   for(int v=0;v<80;v++) cout<<'*';
	   cout<<endl<<setw(20)<<" ";
	 	
		textcolor(YELLOW);
	      textbackground(0);		
		cout<<"LOAN RECORD TO BE MODIFIED IS FOUND"<<endl;		
		getch();
		file.seekp(i*recsize*bucketsize,ios::beg);
	      for(s=0;s<recsize;s++)
		   file.put('$');
		  cout<<"\t\t\tRECORD DELETED...!!"<<endl;
		  getch();
		   file.close();
		clrscr();
		textcolor(15);
	      textbackground(1);
		cout<<"\n";
	 	for(int b=0;b<80;b++) cout<<'~';
		for(int n=0;n<80;n++) cout<<'*';
		  cout<<"\t\t\tENTER THE NEW(MODIFIED) LOAN RECORD: \n";
		  getch();
		read();
		x=hash(account_no);
		store(x);

		cout<<"\t\t    RECORD IS MODIFIED SUCCESSFULLY...!!\n";
		getch();
		break;
	}
	file.seekg(i*recsize*bucketsize+recsize,ios::beg);
	file.getline(account_no,10,'|');
	if(strcmp(account_no,"#########")==0)
	      break;

	if(strcmp(account_no,k)==0)
	{
		found=1;
		unpack();
		clrscr();
		textcolor(YELLOW);
	      textbackground(0);	
	   cout<<endl;
	   cout<<setw(18)<<" "<<"LOAN RECORD TO BE MODIFIED IS DISPLAYED"<<endl<<endl;
	   for(int c=0;c<80;c++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ACCOUNT NUMBER      :  "<<setw(10)<<account_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" BANCK ID            :  "<<setw(9)<<bank_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME       :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" OCCUPATION          :  "<<setw(5)<<occupation<<endl<<endl;
	   cout<<setw(20)<<" "<<" LOAN TYPE           :  "<<setw(5)<<loan_type<<endl<<endl;
	   cout<<setw(20)<<" "<<" BRANCH ID   I       :  "<<setw(6)<<branch_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" RATE OF INTEREST    :  "<<setw(5)<<ROI<<endl<<endl;
	   for(int v=0;v<80;v++) cout<<'*';
	   cout<<endl<<setw(20)<<" ";
	
		 textcolor(YELLOW);
	      textbackground(0);
		cout<<"LOAN RECORD TO BE MODIFIED IS FOUND"<<endl;
		getch();
		file.seekp(i*recsize*bucketsize+recsize,ios::beg);
	      for(s=0;s<recsize;s++)
		   file.put('$');
		cout<<"\t\t\tRECORD DELETED...!!"<<endl;
		  getch();
		   file.close();
		clrscr();
		textcolor(15);
	      textbackground(1);
		cout<<"\n";
	 	for(int b=0;b<80;b++) cout<<'~';
		for(int n=0;n<80;n++) cout<<'*';
		  cout<<"\t\t\tENTER THE NEW(MODIFIED) LOAN RECORD: \n";
		  getch();
		read();
		x=hash(account_no);
		store(x);

		cout<<"\t\t    RECORD IS MODIFIED SUCCESSFULLY...!!\n";
		getch();
		break;
	}

	file.seekg(i*recsize*bucketsize+(2*recsize),ios::beg);
	file.getline(account_no,10,'|');
	if(strcmp(account_no,"#########")==0)
	      break;

	if(strcmp(account_no,k)==0)
	{
		found=1;
		unpack();
		clrscr();
		textcolor(YELLOW);
	      textbackground(0);	
	   cout<<endl;
	   cout<<setw(18)<<" "<<"LOAN RECORD TO BE MODIFIED IS DISPLAYED"<<endl<<endl;
	   for(int c=0;c<80;c++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ACCOUNT NUMBER      :  "<<setw(10)<<account_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" BANCK ID            :  "<<setw(9)<<bank_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME       :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" OCCUPATION          :  "<<setw(5)<<occupation<<endl<<endl;
	   cout<<setw(20)<<" "<<" LOAN TYPE           :  "<<setw(5)<<loan_type<<endl<<endl;
	   cout<<setw(20)<<" "<<" BRANCH ID   I       :  "<<setw(6)<<branch_id<<endl<<endl;
	   cout<<setw(20)<<" "<<" RATE OF INTEREST    :  "<<setw(5)<<ROI<<endl<<endl;
	   for(int v=0;v<80;v++) cout<<'*';
	   cout<<endl<<setw(20)<<" ";
	  
		textcolor(YELLOW);
	      textbackground(0);
		cout<<"LOAN RECORD TO BE MODIFIED IS FOUND"<<endl;
		getch();
		file.seekp(i*recsize*bucketsize+(2*recsize),ios::beg);
	       for(s=0;s<recsize-2;s++)
		   file.put('$');
		   file.put('\n');
		  cout<<"\t\t\tRECORD DELETED...!!"<<endl;
		  getch();
		   file.close();
		clrscr();
		textcolor(15);
	      textbackground(1);
		cout<<"\n";
	 	for(int b=0;b<80;b++) cout<<'~';
		for(int n=0;n<80;n++) cout<<'*';
		  cout<<"\t\t\tENTER THE NEW(MODIFIED) LOAN RECORD: \n";
		  getch();
		read();
		x=hash(account_no);
		store(x);

		cout<<"\t\t    RECORD IS MODIFIED SUCCESSFULLY...!!\n";
		getch();
		break;
	}
	else
	{
		i++;
		if(i%max==0)
			i=0;
	}
}while(i!=addr);
if(found==0)
{
	textcolor(15);
	      textbackground(1);
	cout<<setw(23)<<" "<<"\n\n\t!!*** GIVEN LOAN RECORD DOESNOT EXISTS IN HASH FILE ***!!\n\n";
	for(int k=0;k<80;k++) cout<<'=';
	for(int j=0;j<80;j++) cout<<'^';
	getch();
}
}



/*Douoblebox design*/

void drawdoublelinebox(int x,int y,int len,int bdt)
{
	gotoxy(x,y);
	cprintf("É");
		gotoxy(x,y+bdt);
	cprintf("È");
	gotoxy(x+len,y);
	cprintf("»");
	gotoxy(x+len,y+bdt);
	cprintf("¼");
	for(int i=x+1;i<x+len;i++)
	{
		gotoxy(i,y);
		cprintf("Í");
		gotoxy(i,y+bdt);
		cprintf("Í");
	}
	for(int j=y+1;j<y+bdt;j++)
	{
		gotoxy(x,j);
		cprintf("º");
		gotoxy(x+len,j);
		cprintf("º");
			}
}

/*Double box design*/

void drawsinglelinebox(int x,int y,int len,int bdt)
{                                            // (x,y) : co.ord. of 1st tip
	gotoxy(x,y);                         // len : length of box
	cprintf("Ú");                        // bdt : breadth of box
	gotoxy(x,y+bdt);
	cprintf("À");
	gotoxy(x+len,y);
	cprintf("¿");
	gotoxy(x+len,y+bdt);
	cprintf("Ù");

	for(int i=x+1;i<x+len;i++)
	{
		gotoxy(i,y);
		cprintf("Ä");
		gotoxy(i,y+bdt);
		cprintf("Ä");
	}
	for(int j=y+1;j<y+bdt;j++)
	{
		gotoxy(x,j);
		cprintf("³");
		gotoxy(x+len,j);
		cprintf("³");
	}
}
void normal()  // to obtain normal background
{
	textattr(WHITE); // textattr sets text attributes for text-window functions
	textcolor(WHITE);// textcolor selects a new character color in text mode
	textbackground(BLACK);//textbackground selects a new text background color
	_setcursortype(_NORMALCURSOR); /* sets cursor type to normal underscore type cursor  */
	//return;
}

void screen()
{
 int g=19,h=10;

     textbackground(GREEN);
     gotoxy(24,3);
     textcolor(YELLOW);
     cprintf("        LOAN MANAGEMENT      ");
     gotoxy(24,4);
     textcolor(15);
     cprintf(" ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ ");
     drawdoublelinebox(17,7,44,14);
     gotoxy(28,7);
     textbackground(RED);
	 textcolor(YELLOW);
	 cprintf("Û  M A I N    M E N U Û" );
     normal();
     textcolor(15);
     gotoxy(g,h++);
     cout<<"	     þ LOAN RECORD";
     gotoxy(g,h++);
     cout<<"	     þ QUIT";
     gotoxy(g,h++);
     gotoxy(g,h++);
     gotoxy(g,h++);
     normal();
}


void main()
{
int ch,addr,l,choice, found=0;
char sacc_no[10];
Loan p;
p.initial();
clrscr();
do
{
    	screen();
	//cout<<"\n============================"<<endl;
	//cout<<"1.read loan details\n2.search loan record\n3.display loan record\n4.delete loan record\n5.modify record\n6.exit\n";
      // cout<<"============================"<<endl;
	cout<<"PLEASE ENTER YOUR CHOICE [1-2] :  ";
	cin>>ch;
	switch(ch)
	{
	case 1:do
	      {
	      clrscr();
	      drawdoublelinebox(17,7,44,14);
	      textcolor(0);
	      textbackground(15);
		  clrscr();
		  textbackground(BLACK);
	      gotoxy(25,3);
	      textcolor(YELLOW);
	      cprintf("        LOAN RECORDS       ");
	      gotoxy(25,4);
	      cprintf(" ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ ");
	      int z=19,q=10;
	      textcolor(YELLOW);
	      drawdoublelinebox(17,7,44,14);
	      gotoxy(28,7);
	      textbackground(RED);
	      textcolor(YELLOW);
	      cprintf("Û  M A I N    M E N U Û" );
	      normal();
	      textcolor(WHITE);
	      gotoxy(z,q++);
	      cout<<"     1.>  READ LOAN DETAILS";
	      gotoxy(z,q++);
	      cout<<"     2.>  SEARCH LOAN RECORD";
		  gotoxy(z,q++);
	      cout<<"     3.>  DISPLAY LOAN DETAILS";
	      gotoxy(z,q++);
	      cout<<"     4.>  DELETE LOAN RECORD";
	      gotoxy(z,q++);
		cout<<"     5.>  MODIFY LOAN RECORD";
	      gotoxy(z,q++);
	      cout<<"     6.>  QUIT";
	      gotoxy(z,q++);
	      gotoxy(z,q++);
	      gotoxy(z,q++);
	      cout<<"    PLEASE ENTER YOUR CHOICE [1-6]  : ";
	      cin>>choice;
	 switch(choice)
	 {
	case 1:clrscr();
		cout<<"\n";
	 	for(int b=0;b<80;b++) cout<<'~';
		cout<<setw(24)<<" "<<"ENTER THE LOAN RECORD DETAILS \n";
		for(int h=0;h<80;h++) cout<<'~';
		cout<<"\n";
		getch();
	 LABEL: p.read();
		addr=hash(p.account_no);
		store(addr);
		getch();
		file.close();
		break;
	case 2:clrscr();
		textcolor(15);
	      textbackground(YELLOW);
		cout<<"\n\n\n\n";
		cout<<"\t\t\t  SEARCHING LOAN RECORD\n\n";
		for(int e=0;e<80;e++) cout<<'~';
		//cout<<"\n";
		for(int w=0;w<80;w++) cout<<'~';
		cout<<"\n";
		cout<<setw(20)<<" "<<"ENTER THE ACCOUNT NUMBER TO BE SEARCHED: "<<endl;
		cout<<setw(36)<<" ";
		cin>>sacc_no;
		cout<<"\n";
		addr=hash(sacc_no);
		p.retrieve(addr,sacc_no,1);
		getch();
		file.close();
		break;
	case 3://cout<<"vehicle details:"<<endl;
		textcolor(15);
	      textbackground(RED);	
	       p.datadisp();
	       file.close();
		break;
       case 4://p.del();
		clrscr();
		textcolor(15);
	      textbackground(1);
		cout<<"\n\n\n\n";
		cout<<"\t\t\t  DELETING LOAN RECORD\n\n";
		for(int x=0;x<80;x++) cout<<'^';
		//cout<<"\n";
		for(int y=0;y<80;y++) cout<<'^';
		cout<<"\n";
		cout<<setw(20)<<" "<<"ENTER THE ACCOUNT NUMBER TO BE DELETED: "<<endl;
		cout<<setw(36)<<" ";
		cin>>sacc_no;
		cout<<"\n";
		addr=hash(sacc_no);
		p.remove(addr,sacc_no);
		getch();
		file.close();
		break;
       case 5:clrscr();
		textcolor(15);
	      textbackground(1);
		cout<<"\n\n\n\n";
		cout<<"\t\t\t  MODIFYING LOAN RECORD\n\n";
		for(int u=0;u<80;u++) cout<<'=';
		//cout<<"\n";
		for(int r=0;r<80;r++) cout<<'^';
		cout<<"\n";
		cout<<setw(20)<<" "<<"ENTER THE LOAN RECORD TO MODIFY: \n";
	      	cout<<setw(36)<<" ";
		cin>>sacc_no;
	      addr=hash(sacc_no);
		cout<<"\n";
	      p.modify(addr,sacc_no);
		getch();
	     file.close();
	      break; 
	case 6:clrscr();
			 screen();
			 break;
	default:sound(600);delay(500);nosound();
		break;
	}
	}while(choice!=6);   break;

case 2:exit(0);

default: sound(100);delay(500);nosound(); break;
}
}while(ch!=2);

       file.close();
}



