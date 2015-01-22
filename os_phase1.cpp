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
char memory[100][4],ir[4];
string next_to_amj="cdss",data;
int blocks=0,ic=0,c=0,m=0,R,data_counter,address,row_no,col_no;
ifstream infile("test.txt");
int main()
{
	load();
}
int find()
{
	string :: iterator it;
	cout<<"String is " <<next_to_amj<<endl;
	for (it= next_to_amj.begin(); it != next_to_amj.end(); it++)
		if(*it == 'H')
		{
			cout<<"returning 0"<<endl;
			return 0;
		}
	return 1;	
	//int found = s2.get('H');
	
	//cout <<s2<<" "<< "returning " <<(found)<<endl;
	
	//return 0;
}

void load()
{
	int flag=0;

	
	while (infile)
  	{

    	string file_content,sub_string,temp,trimmed_string;
    	if (!getline( infile, file_content)) break; //Accept a string 
    												//from file
    	temp = file_content.substr(0,4); //compare temp to check 
										 // the actions to be perform    	
    	if(temp == "$END")	//break the while loop
    		break;	
    	else if(temp == "$AMJ") //for $AMJ go on accepting
    							//from file till you get H
    							//And store it in memory initial blocks
    	{
    		while(getline( infile, next_to_amj))  //Read File till we get H  		
    		{
    			while(next_to_amj != "" && find()) //Read lines next to amj
    			{
	    			trimmed_string = next_to_amj.substr(0,4);
	    			next_to_amj.erase(0,4);
					memory[m][0]=trimmed_string[0]; //Store this instructions
					memory[m][1]=trimmed_string[1];	//in memory which is available   			
	    			memory[m][2]=trimmed_string[2];
	    			memory[m][3]=trimmed_string[3];
	    			m++;
	    			if(m % 10 == 0)
	    				blocks++; 	//1 block = 10 m
	    		}    		
	    		if(!find()) //After reading particular line if
	    			break;	//next_to_amj is H then instructions are finished
				    		//memory[m][0] = 'H'; 		
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
		cout<<instruction<<endl;
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
			R = (int)memory[row_no][col_no];
			cout<<"value of r is "<<R<<endl;
		}
		else if(instruction == "SR")
		{
			calculate_address();			
			memory[row_no][col_no] = (char)R;			
			cout<<"value of mem is "<<memory[row_no][col_no]<<endl;
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
					calculate_address();			
					getline(infile,data);
					cout<<"Data is "<<data<<endl;
					temp = data.substr(0,4);		
					if(temp == "$END" || temp == "")
					{
						cout<<"Insufficient Arguments Passed";
						exit(0);
					}					
					if(row_no > 100)
					{
						cout<<"Memory is exceeded"<<endl;
						cout<<"row no "<<row_no<<endl;
						exit(0);
					}
					int counter = 0;
					for(int i = row_no;counter<(data.length()-1);i++)
						for(int j = 0;j<4 && counter<(data.length()-1);j++)
						{
							memory[i][j] = data[counter];
							counter += 1;
							cout<<"i is\t"<<i<<"j is "<<j<<"\t"<<data.length()<<endl;
							
						}
					break;
				}

		case 2:
				cout<<"Read Data Here";
		case 3:
				cout<<"Halt Here";
					
	}
	infile.close();
}
void calculate_address()
{
	address = (ir[2]-48) * 10 + (ir[3] - 48);
	row_no = address / 4;
	col_no = address % 4;	
}

