#include<iostream>
#include<vector>
#include<string.h>
#include<fstream>
#include<stdlib.h>
typedef struct PCB
{
    int TTL, TLL, JOB_ID;
} PCB;
using namespace std;
void load ();
int find ();
void startexecution ();
void executeuserprogram ();
void MOS ();
void TERMINATE(int);
int calculate_address ();
void go_to_end ();
void reset_memory ();
int ALLOCATE ();
void SIMULATION();
char memory[300][4] = { ' ', ' ' }, IR[4], C, R[4];

string next_to_amj = "cdss", data;
int blocks = 0, IC = 0, m = 0,VA, data_counter, address, row_no, col_no, SI =
                                0, PTR, TTC, TLC, PI, TI, RA;
bool occupied[30] = { false };
string instruction;
PCB pcb;

ifstream infile ("test.txt");
ofstream outfile;
std::string::size_type sz;	// alias of size_t
int main ()
{
    outfile.open ("output.txt");
    load ();
    infile.close ();
    outfile.close ();
}

int find (string trimmed_string)
{
    string::iterator it;
//cout<<"String is " <<trimmed_string<<endl;
    for (it = trimmed_string.begin (); it != trimmed_string.end (); it++)
        if (*it == 'H')
            {
                memory[m][0] = 'H';
                return 0;
            }
    return 1;
}

void go_to_end ()
{
    string
    temp,
    file_content;
    while (1)
        {
            if (!getline (infile, file_content))
                break;			//Accept a string
//from file
            temp = file_content.substr (0, 4);	//compare temp to check
// the actions to be perform
            if (temp == "$END")	//break the while loop
                return;
        }
    //cout<<"Ending all   ";
    //load();
    //RA = 298;
        exit(0);
}

void reset_memory ()
{
    for (int i = 0; i < 300; i++)
        for (int j = 0; j < 4; j++)
            memory[i][j] = ' ';
    for(int i=0;i<30;i++)
    	occupied[i] = false;
    PI = 0;
    TI = 0;
    TTC = 0;
    TLC = 0;
}

PCB init_PCB (string file_content)
{
    string
    job_id = file_content.substr (4, 4);
    int
    job_id1 = std::stoi (job_id, &sz);
    pcb.JOB_ID = job_id1;
    string
    TTL = file_content.substr (8, 4);
    int
    TTL1 = std::stoi (TTL, &sz);
    pcb.TTL = TTL1;
    string
    TLL = file_content.substr (12, 4);
    int
    TLL1 = std::stoi (TLL, &sz);
    pcb.TLL = TLL1;
	cout<<"job details "<<pcb.JOB_ID<<"\t"<<pcb.TTL<<"\t"<<pcb.TLL<<endl;
    return pcb;
}

int ALLOCATE ()
{
    int
    page_no;
    while (1)
        {
            page_no = rand () % 30;
            if (!occupied[page_no])
                {
                    occupied[page_no] = true;
                    break;
                }
        }
    cout << "Allocated " << page_no << "\n";
    return page_no;
}

int ADDRESS_MAP (int VA1)
{
    int loc,fno,RA,flag1=1;
    if (VA1 >= 100)
    {
       PI = 2;
    	MOS();
       return 0;
    }
    loc = PTR + (VA1 / 10);
	if(memory[loc][0] == '0')
	{
        cout << "Returninggggggg page fault   " << endl;
        PI = 3;
        return -1;
    }    
	fno = (memory[loc][2] - '0') * 10 + (memory[loc][3] - '0');
    RA = (fno * 10) + (VA1 % 10);
    cout << "raaaaaaa " << RA << endl;
    return RA;
}

void set_pt ()
{
    int counter = PTR;
    while (counter < (PTR + 9))
    {
        for (int i = 0; i < 4; i++)
            memory[counter][i] = '0';
        counter++;
    }
}

void update_page_table (int frame_no)
{
    for (int i = PTR; i < (PTR + 10); i++)
    {
        if (memory[i][0] == '0')
        {
            memory[i][0] = '1';
            int dec = (frame_no / 10);
            int unit = (frame_no % 10);
            memory[i][2] = (dec + '0');
            memory[i][3] = (unit + '0');
            cout << "Updatedddddd " << i << "\t" << memory[i][2] << "\t" <<
            							memory[i][3] << frame_no << endl;
            break;
        }
    }
}

void load ()
{
    int
    flag = 0;
    reset_memory ();
    while (infile)
    {
        int frame_no,fm;
        string file_content,sub_string,temp,trimmed_string;
        if (!getline (infile, file_content))
            break;			//Accept a string
							//from file
        temp = file_content.substr (0, 4);	//compare temp to check
											// the actions to be perform
        if (temp == "$END")  	//break the while loop
        {
								//Reset all values here
            reset_memory ();
            IC = m = data_counter = address = 0;
            outfile << "\n\n";
            continue;
			//break;
        }
        else if (temp == "$AMJ")	//for $AMJ go on accepting
									//from file till you get H
									//And store it in memory initial blocks
        {
            PCB pcb_obj = init_PCB (file_content);
            PTR = ALLOCATE ();
            PTR *= 10;
            PTR += 1;
            set_pt ();
            flag = 0;
            while (getline (infile, next_to_amj))  	//Read File till we get H
            {
                frame_no = ALLOCATE ();
                update_page_table (frame_no);
                frame_no *= 10;
                fm =frame_no;
                while (next_to_amj != "")  	//Read lines next to amj
                {
                    trimmed_string = next_to_amj.substr (0, 4);
                    if (!find (trimmed_string))
                    {
                    	memory[frame_no][0]='H';
                        flag = 1;
                        break;
                    }
                    next_to_amj.erase (0, 4);
                    memory[frame_no][0] = trimmed_string[0];	//Store this instructions
                    memory[frame_no][1] = trimmed_string[1];	//in memory which is available
                    memory[frame_no][2] = trimmed_string[2];
                    memory[frame_no][3] = trimmed_string[3];
                    frame_no++;
                }
                for (int i =fm;i<=frame_no;i++)
                	cout<<memory[i][0]<<memory[i][1]<<memory[i][2]<<memory[i][3]<<endl;
                if (flag == 1)	//We got h as substring in inner loop so break
                    break;
            }
        }
        else if (temp == "$DTA")	// Start executing control cards
            startexecution ();
    }
}

void startexecution ()
{
    IC = 0;
    executeuserprogram ();
}

void executeuserprogram ()
{
    int address,row;
    string end = "H";

    while (1)
    {
    	RA = ADDRESS_MAP(IC);
    	IC++;
    	cout<<"Instruction no is     "<<IC<<endl;
        string temp,temp1;
        for (int j = 0; j < 4; j++)
            IR[j] = memory[RA][j];
        temp = IR[0];
        temp1 = IR[1];
        instruction = temp + temp1;
        cout <<"iNSTR IS   " <<instruction << IR[2] << IR[3] << endl;
        if(instruction.compare(end) == 1)
        {
        	cout<<"Exitting "<<endl;
        	break;

        }
        VA = calculate_address ();
        RA = ADDRESS_MAP (VA);
        if (instruction == "GD")
        {
            SI = 1;		//For GD System interupt is set to 1
            MOS ();
        }
        else if (instruction == "PD")
        {
            SI = 2;		//For PD System interupt is set to 2
            //TLC++;
            MOS ();
        }
        else if (instruction == "LR")
        {
            cout << "In lrrrrrrrr " << "\t" << VA << "\t" <<
                 memory[RA][2] << memory[RA][3] << endl;
            if(PI == 3)
            	TERMINATE(3);
            for (int i = 0; i < 4; i++)
                R[i] = memory[RA][i];
            cout << "value of r is " << R << endl;
        }
        else if (instruction == "SR")
        {
        	PI = 0;
            int allocator = ALLOCATE();
            address = VA/10;
            memory[address][0] = 0;
            memory[address][2] = ((allocator/10) + '0');
            memory[address][3] = ((allocator%10) + '0');
            allocator *= 10;
            cout << "In srrrrrrrr " << "\t" << address << "\t" << VA << "\t" <<
                 "fdg sdddddc " << endl;
            for (int i = 0; i < 4; i++)
                memory[allocator][i] = R[i];
            cout << "value of mem is " << memory[allocator] << endl;
        }
        else if (instruction == "CR")
        {                    
            int flag = 0;
            for (int i = 0; i < 4; i++)
                if (memory[RA][i] != R[i])
                    flag = 1;
            if (flag == 0)
                C = 'T';
            else
                C = 'F';
            flag = 0;
        }
        else if (instruction == "BT")
        {
            cout << "C is " << C << endl;
            if (C == 'T')
            {    IC = ((IR[2] - 48) * 10) + (IR[3] - 48) ;
            	cout<<"chaanged"<<endl;
            }
            C = 'F';
            cout << "Instruction counter is now " << IC << endl;
        }
         else if (instruction == "H")
        {
        	SI = 3;
        	MOS();
        }    
        else
        {
            PI = 1;
        }
        SIMULATION();
        cout<<"INSTR ENDING  "<<endl;

        // cout<<"vgtvdhbnjhyg "<<memory[RA][0]<<endl;	
		//cout<<"ra isssssssssssssss "<<RA<<"\t"<<memory[RA][0]<<endl;
    }
}

void MOS ()
{
    int row_no1;
    if (TI == 0 )
    {
        switch (SI)
        {
            case 1:  	//GD Operation will take place here
            {
                //i.e reading a line after DTA pointed by data_counter
                string temp;
                data = "";
                PI = 0;
                //row_no1 = ADDRESS_MAP (VA);
				//cout<<"Row no after calculating "<<row_no1<<endl;
				address = PTR + (VA/10);
				int allocator = ALLOCATE();
				cout<<"allocating in gd "<<VA<<"\t"<<address<<endl;
				memory[address][0] = 0;
				memory[address][2] = ((allocator/10) + '0');
				memory[address][3] = ((allocator%10) + '0');
                allocator *= 10;
				
                getline (infile, data);
                cout << "Data is " << data << "\t" << row_no << endl;
                temp = data.substr (0, 4);
                if (temp == "$END" || temp == "")
                {
                    cout << "Out Of Data \n";
                    TERMINATE(1);                   
                }
                // if (row_no1 > 300)
                // {
                //         cout << "Memory is exceeded \n" << endl;
                //         cout << "row no " << row_no << endl;
                //         reset_memory ();
                //         IC = m = data_counter = address = 0;
                //         outfile << "Memory is exceeded \n";
                //         outfile << "\n\n";
                //         go_to_end ();
                // }
                int counter = 0;
				//cout<<"Row nno is "<<allocator<<endl;
                for (int i = allocator; counter < (data.length ()); i++)
                    for (int j = 0; j < 4 && counter < (data.length ()); j++)
                    {
                        cout << "Storing Data at " << i << "\t" << "\t" <<
                             data[counter] << endl;
                        memory[i][j] = data[counter];
                        counter += 1;
                    }
                break;
            }
            case 2:		// In PD Data is written to output file
						// from ex 30 to 39 mem location (10 words)
            {
            	if(TLC > pcb.TLL)
           			TERMINATE(2);           		
              	cout<<"In pd pi is "<<RA;

              	if(PI == 3)
                	TERMINATE(6);
                int row;
                string space = " ";
                cout << "In PD" << IR[2] << IR[3] << endl;
				//IR[3] = 0;
                //cout << "Address in pd issssssss " << VA << "\t"<<endl;
                int count = 0;
                //char *buffer = new char[1];
                for (int row = RA; count < 10; row++)
                {
                    count++;
                    for (int col = 0; col < 4; col++)
                    {
                        if (memory[row][col] != ' ')
                        {
                            outfile << memory[row][col];
                            cout << "Writting " << memory[row][col] << endl;
                        }
                    }
                }
                outfile << "\n";
                break;
            }
            case 3:
            {
                TERMINATE(0);
            }
        }
    }
    ////////////////////////////////////////// cocde ..................../////////////////////
    else if(TI == 2)
        {

            switch (SI)
                {
                case 1:  	//TERMINATE (3)
                {
                    TERMINATE(3);
                    break;
                }
                case 2:  	// WRITE, THEN TERMINATE (3)
                {
                	//cout<<"cminggggggggggg herrrrrrrrrrrr "<<endl;
                    TERMINATE (3);
                    break;
                }
                case 3:   //TERMINATE (0)
                {
                    TERMINATE(0);
                    break;
                }
                }
        }
        /****************************************/
	if(TI == 0)
	{
	  switch(PI)
	  {
	    case 1:
	      TERMINATE(4);
	      break;
	    case 2:
	      TERMINATE(5);
	      break;
	    case 3:
	    	cout<<"InValid pageeeee faulttt "<<endl;
	      TERMINATE(6);
	      break;
	  }
	}
	else if(TI == 2)
	{
	  switch(PI)
	  {
	    case 1:
	      TERMINATE(7);
	      break;
	    case 2:
	      TERMINATE(8);
	      break;
	    case 3:
	      TERMINATE(3);
	      break;
		default:
			cout<<"terminatingggggggggg    "<<endl;
			TERMINATE(3);
			break;	  
	  }

	}
    /////////////////////////////////////////////////////////////////////////////////////
    /*
    else
    {
    switch(PI)
    {
    }
    }
    */
    SI = 0;
}

int calculate_address ()
{
    VA = (IR[2] - '0') * 10 + (IR[3] - '0');
//cout<<"edrtfyhjkl ftyghkj ythuj "<<IR[2]<<"\t"<<IR[3]<<"\t"<<VA<<"\t"<<endl;
    return VA;
}


void TERMINATE(int ERR_CODE)
{
    switch(ERR_CODE)
        {
        case 0:
            outfile<<"NO ERROR\n";
            break;
        case 1:
            outfile<<"OUT OF DATA\n";
            break;
        case 2:
            outfile<<"LINE LIMIT EXCEEDED\n";
            break;
        case 3:
            outfile<<"TIME LIMIT EXCEEDED\n";
            break;
        case 4:
            outfile<<"OPERATION CODE ERROR\n";
            break;
        case 5:
            outfile<<"OPERAND ERROR\n";
            break;
        case 6:
            outfile<<"INVALID PAGE FAULT\n";
            break;
        case 7:
            outfile<<"TIME LIMIT EXCEEDED And OPERATION CODE ERROR\n";
            break;
        case 8:
            outfile<<"TIME LIMIT EXCEEDED And OPERAND ERROR\n";
            break;
        }
    	//go_to_end();
    	outfile<<"Job Details \t"<<pcb.JOB_ID<<"\t"<<pcb.TTL<<"\t"<<pcb.TLL<<endl;
        exit(0);
}

void SIMULATION()
{
    TTC++;
    if(TTC >= pcb.TTL)
        {
        	TI = 2;
            cout<<"Setting TI as "<<TI<<endl;
            MOS();
        }

}
