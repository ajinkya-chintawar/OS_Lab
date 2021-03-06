#include<iostream>
#include<vector>
#include<cstring>
#include<fstream>
#include<stdlib.h>

using namespace std;
void load();
int find();
void startexecution();
void executeuserprogram();
void MOS();
void calculate_address();
void go_to_end();
void reset_memory();
char memory[100][4] = {' ',' '},IR[4],C,R[4];
string next_to_amj="cdss",data;
int blocks=0,IC=0,m=0,data_counter,address,row_no,col_no,SI = 0;
ifstream infile("test.txt");
ofstream outfile;
int main()
{
	outfile.open("output.txt");
	load();
	infile.close();
	outfile.close();
}
int find(string trimmed_string)
{
	string :: iterator it;
	//cout<<"String is " <<trimmed_string<<endl;
	for (it= trimmed_string.begin(); it != trimmed_string.end(); it++)
		if(*it == 'H')
		{
			memory[m][0]='H';
			return 0;
		}
	return 1;	
}
void go_to_end()
{
	string temp,file_content;
	while(1)
	{
		if (!getline( infile, file_content)) break; //Accept a string 
    												//from file
    	temp = file_content.substr(0,4); //compare temp to check 
										 // the actions to be perform    	
    	if(temp == "$END")	//break the while loop
			return;
	}
}
void reset_memory()
{
	for(int i=0;i<100;i++)
		for(int j=0;j<4;j++)
			memory[i][j] = ' ';
			
}	
	
void load()
{
	int flag=0;
	reset_memory();
	
	while (infile)
  	{

    	string file_content,sub_string,temp,trimmed_string;
    	if (!getline( infile, file_content)) break; //Accept a string 
    												//from file
    	temp = file_content.substr(0,4); //compare temp to check 
										 // the actions to be perform    	
    	if(temp == "$END")	//break the while loop
		{
			//Reset all values here
			reset_memory();
			IC=m=data_counter=address=0;
			outfile<<"\n\n";
			continue;
		}
    	else if(temp == "$AMJ") //for $AMJ go on accepting
    							//from file till you get H
    							//And store it in memory initial blocks
    	{
    		flag=0;
    		while(getline( infile, next_to_amj))  //Read File till we get H  		
    		{
    			while(next_to_amj != "") //Read lines next to amj
    			{
	    			trimmed_string = next_to_amj.substr(0,4);
	    			if(!find(trimmed_string))
	    			{
	    				flag=1;
	    				break;
	    			}
	    			next_to_amj.erase(0,4);
					memory[m][0]=trimmed_string[0]; //Store this instructions
					memory[m][1]=trimmed_string[1];	//in memory which is available   			
	    			memory[m][2]=trimmed_string[2];
	    			memory[m][3]=trimmed_string[3];
	    			m++;
	    			//if(m % 10 == 0)
	    			//	blocks++; 	//1 block = 10 m
	    		}    		
	    		if(flag == 1)	//We got h as substring in inner loop so break
					break; 		
    		}
    		cout<<"code in memory \n";
			for(int i=0;i<=m;i++)
			{
				for(int j=0;j<4;j++)
					cout<<memory[i][j];
				cout<<"\n";
			}
    	}		
    	else if(temp == "$DTA")		// Start executing control cards    	
    		startexecution();
		
    }
    
    
}
void startexecution()
{
	IC = 0;
	executeuserprogram();
}
void executeuserprogram()
{
	for(int IC = 0;IC <= m; IC++)
	{
		
		string temp,temp1,instruction;
		for(int j=0;j<4;j++)
		{
			IR[j] = memory[IC][j];
		//	cout<<"Instruction we storing is \t"<<IR[j]<<"\t"<<memory[IC][j];
		}
		for(int i=0;i<4;i++)
			cout<<IR[i]<<endl;
		cout<<"**************"<<endl;
	
		cout<<"\n";
		temp = IR[0];
		temp1 = IR[1];
		instruction = temp + temp1;
		cout<<instruction<<IR[2]<<IR[3]<<endl;
		if(instruction == "GD")
		{
			SI = 1; //For GD System interupt is set to 1
			MOS();
		}
		else if(instruction == "PD")
		{
			SI = 2; //For GD System interupt is set to 2
			MOS();
		}
		else if(instruction == "LR")
		{
			calculate_address();
			for(int i=0;i<4;i++)
				R[i] = memory[row_no][i];
			cout<<"value of r is "<<R<<endl;
		}
		else if(instruction == "SR")
		{
			calculate_address();
			for(int i=0;i<4;i++)
				memory[row_no][i] = R[i];
			cout<<"value of mem is "<<memory[row_no]<<endl;
		}
		else if(instruction == "CR")
		{
			calculate_address();
			int flag =0;
			for(int i=0;i<4;i++)
				if(memory[row_no][i] != R[i])
					flag = 1;
			if(flag == 0)
				C = 'T';
			else
				C = 'F';
			flag = 0;
		}
		else if(instruction == "BT")
		{
			cout<<"C is "<<C<<endl;
			if(C == 'T')
				IC = ((IR[2] - 48) * 10) + (IR[3] - 48)-1;
			C = 'F';
			cout<<"Instruction counter is now "<<IC<<endl;
		}
		 
	}
}
void MOS()
{
	switch(SI)
	{
		case 1:		//GD Operation will take place here 
				{		//i.e reading a line after DTA pointed by data_counter
					string temp;
					data = "";
					calculate_address();			
					getline(infile,data);
					cout<<"Data is "<<data<<endl;
					temp = data.substr(0,4);
					if(temp == "$END" || temp == "")
					{
						cout<<"Out Of Data \n";
						reset_memory();
						IC=m=data_counter=address=0;
						outfile<<"Out Of Data \n";
						outfile<<"\n\n";
					}					
					if(row_no > 100)
					{
						cout<<"Memory is exceeded \n"<<endl;
						cout<<"row no "<<row_no<<endl;
						reset_memory();
						IC=m=data_counter=address=0;
						outfile<<"Memory is exceeded \n";
						outfile<<"\n\n";
						go_to_end();
					}
					int counter = 0;
					cout<<"Row nno is "<<row_no<<endl;
					for(int i = row_no;counter<(data.length());i++)
						for(int j = 0;j<4 && counter<(data.length());j++)
						{
							cout<<"Storing Data"<<endl;
							memory[i][j] = data[counter];
							counter += 1;							
						}
					break;
				}

		case 2:			// In PD Data is written to output file
						// from ex 30 to 39 mem location (10 words)
				{
					string space = " ";
					cout<<"In PD"<<IR[0]<<IR[1]<<IR[2]<<IR[3]<<endl;
					//IR[3] = 0;
					calculate_address();
					int count = 0;
					char* buffer = new char[1];			
					for(int row = row_no;count < 10;row++)
					{
						count++;
						for(int col = 0;col < 4;col++)
						{
								if( memory[row][col] != ' ')
									outfile<<memory[row][col];	
						}
					}
					outfile<<"\n";
					break;
				}
				
		case 3:
				cout<<"Halt Here";
					
	}
	SI = 0;
}
void calculate_address()
{
	int a,b;
	for(int i=0;i<4;i++)
		cout<<IR[i]<<endl;
	cout<<"**************"<<endl;
	b = IR[3] - '0';
	a = IR[2] - '0';
	row_no = a * 10 + b;
	col_no = b;
	cout<<row_no<<"\t"<<b<<endl;
}