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
void MOS(int );
void calculate_address();
void go_to_end();
void reset_memory();
char memory[100][4],ir[4],c,R;
string next_to_amj="cdss",data;
int blocks=0,ic=0,m=0,data_counter,address,row_no,col_no;
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
	cout<<"String is " <<trimmed_string<<endl;
	for (it= trimmed_string.begin(); it != trimmed_string.end(); it++)
		if(*it == 'H')
		{
			cout<<"returning 0"<<endl;
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
		{ 
			cout<<"end found line is "<<file_content<<endl;
			return;
		}
	}
}
void reset_memory()
{
	for(int i=0;i<100;i++)
		for(int j=0;j<4;j++)
			memory[i][j] = ' ';
			
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
			ic=m=data_counter=address=0;
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
    	
    	}		
    	else		// Start executing control cards    	
    		startexecution();
    }
    
    
}
void startexecution()
{
	ic = 0;
	executeuserprogram();
}
void executeuserprogram()
{
	for(int ic = 0;ic < m; ic++)
	{
		string temp,temp1,instruction;
		for(int j=0;j<4;j++)
			ir[j] = memory[ic][j];			
		temp = ir[0];
		temp1 = ir[1];
		instruction = temp + temp1;
		cout<<instruction<<ir[2]<<ir[3]<<endl;
		if(instruction == "GD")
		{
			MOS(1);
		}
		else if(instruction == "PD")
		{
			MOS(2);
		}
		else if(instruction == "LR")
		{
			calculate_address();
			R = memory[row_no][col_no];
			cout<<"value of r is "<<R<<endl;
		}
		else if(instruction == "SR")
		{
			calculate_address();			
			memory[row_no][col_no] = R;
			cout<<"In sr row nd cols are "<<row_no<<"\t"<<col_no<<endl;
			cout<<"value of mem is "<<memory[row_no][col_no]<<endl;
		}
		else if(instruction == "CR")
		{
			calculate_address();			
			char cmp = memory[row_no][col_no];
			if(cmp == R)
				c = 'T';
		}
		else if(instruction == "BT")
		{
			cout<<"C is "<<c<<endl;
			if(c == 'T')
				ic = ((ir[2] - 48) * 10) + (ir[3] - 48)-1;
			c = 'F';
			cout<<"Instruction counter is now "<<ic<<endl;
		}
		 
	}
}
void MOS(int si)
{
	switch(si)
	{
		case 1:		//GD Operation will take place here 
				{		//i.e reading a line after DTA pointed by data_counter
					string temp;
					data = "";
					ir[3] = 0;					
					calculate_address();			
					getline(infile,data);
					cout<<"Data is "<<data<<endl;
					temp = data.substr(0,4);
					if(temp == "$END" || temp == "")
					{
						cout<<"Insufficient Arguments Passed";
						reset_memory();
						ic=m=data_counter=address=0;
						outfile<<"\n\n";
					}					
					if(row_no > 100)
					{
						cout<<"Memory is exceeded"<<endl;
						cout<<"row no "<<row_no<<endl;
						reset_memory();
						ic=m=data_counter=address=0;
						outfile<<"\n\n";
						go_to_end();
					}
					int counter = 0;
					for(int i = row_no;counter<(data.length());i++)
						for(int j = 0;j<4 && counter<(data.length());j++)
						{
							memory[i][j] = data[counter];
							counter += 1;
							cout<<"i is\t"<<i<<"j is "<<j<<"\t"<<data.length()<<endl;							
						}
					break;
				}

		case 2:			// In PD Data is written to output file
						// from ex 30 to 39 mem location (10 words)
				{
					string space = " ";
					cout<<"In PD"<<ir[0]<<ir[1]<<ir[2]<<ir[3]<<endl;
					ir[3] = 0;
					calculate_address();
					int count = 0;
					char* buffer = new char[1];			
					for(int row = row_no;count < 10;row++)
					{
						count++;
						for(int col = 0;col < 4;col++)
						{
							/*
							if(memory[row][col] == ' ')
							{
								//cout<<"\t";
								outfile<<"\t";
							}
							else*/
							{
								cout<<"Writting "<<memory[row][col]<<endl;
								outfile<<memory[row][col];
							}	
						}
					}
					outfile<<"\n";
					break;
				}
				
		case 3:
				cout<<"Halt Here";
					
	}
	//infile.close();
}
void calculate_address()
{
	row_no = (ir[2] - 48) * 10;
	col_no = (ir[3] - 48);
}

