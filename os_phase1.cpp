#include<iostream>
#include<vector>
#include<cstring>
#include<fstream>

using namespace std;
void load();
int find();
void startexecution();
void executeuserprogram();
char memory[100][4],ir[4];
string next_to_amj="cdss";
int blocks=0,ic=0,c=0,m=0;
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

	ifstream infile("test.txt");

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
		for(int j=0;j<4;j++)
		{
			ir[j] = memory[ic][j];
			
		}
	
}
