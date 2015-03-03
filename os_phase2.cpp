#include<iostream>
#include<vector>
#include<string>
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
char memory[300][4] = { '', '' }, IR[4], C, R[4];

string next_to_amj = "cdss", data;
int blocks = 0, IC = 0, m = 0, data_counter, address, row_no, col_no, SI =
                                0, PTR, TTC, TLC, PI, TI, RA;
bool occupied[30] = { false };

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
}

void reset_memory ()
{
    for (int i = 0; i < 300; i++)
        for (int j = 0; j < 4; j++)
            memory[i][j] = '';
}

PCB init_PCB (string file_content)
{
    PCB
    pcb;
    string
    job_id = file_content.substr (5, 4);
    int
    job_id1 = std::stoi (job_id, &sz);
    pcb.JOB_ID = job_id1;
    string
    TTL = file_content.substr (9, 4);
    int
    TTL1 = std::stoi (TTL, &sz);
    pcb.TTL = TTL1;
    string
    TLL = file_content.substr (13, 4);
    int
    TLL1 = std::stoi (TLL, &sz);
    pcb.TLL = TLL1;
//cout<<"job details "<<pcb.JOB_ID<<"\t"<<pcb.TTL<<"\t"<<pcb.TLL<<endl;
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

int ADDRESS_MAP (int VA, int valid_page_fault)
{
    int
    loc,
    fno,
    RA;
    if (VA >= 100)
        {
            PI = 2;
            return 0;
        }
    //PI = 3 for page fault;
    loc = PTR + (VA / 10);
    if (valid_page_fault)  	//valid page fault
        {
            cout << "Returninggggggg " << endl;
            int
            allocator = ALLOCATE ();
            memory[loc][0] = 0;
            memory[loc][2] = ((allocator / 10) + '0');
            memory[loc][3] = ((allocator % 10) + '0');
//return loc;
        }
    cout << "admap " << loc << "\t" << memory[loc][2] << memory[loc][3] << endl;
    fno = (memory[loc][2] - '0') * 10 + (memory[loc][3] - '0');
    RA = (fno * 10) + (VA % 10);
    cout << "raaaaaaa " << RA << endl;
    return RA;
}

void set_pt ()
{
    int
    counter = PTR;
    while (counter < (PTR + 9))
        {
            for (int i = 0; i < 4; i++)
                memory[counter][i] = '0';
            counter++;
        }
    cout << "ftghjk " << counter << endl;
}

void update_page_table (int frame_no)
{
    for (int i = PTR; i < (PTR + 10); i++)
        {
            if (memory[i][0] == '0')
                {
                    memory[i][0] = '1';
                    memory[i][1] = '1';	//For Program Card
                    int
                    dec = (frame_no / 10);
                    int
                    unit = (frame_no % 10);
                    memory[i][2] = (dec + '0');
                    memory[i][3] = (unit + '0');
                    cout << "Updatedddddd " << i << "\t" << memory[i][2] << "\t" <<
                         memory[i][3] << frame_no << endl;
                    break;
                }
        }
}

/*
int find_frame()
{
for(int i = PTR; i< (PTR + 10);i++)
{
if(memory[i][0] == '1' && memory[i][1] == '1')
{
memory[i][0] = 2;
int frame_no = (memory[i][2] - '0') * 10 + (memory[i][3] - '0');
cout<<"fnooooooo "<<frame_no<<endl;
return frame_no;
}
}
return 0;
}
*/
void load ()
{
    int
    flag = 0;
    reset_memory ();
    while (infile)
        {
            int
            frame_no;
            string
            file_content,
            sub_string,
            temp,
            trimmed_string;
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
                    PCB
                    pcb_obj = init_PCB (file_content);
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
                            while (next_to_amj != "")  	//Read lines next to amj
                                {
                                    trimmed_string = next_to_amj.substr (0, 4);
                                    if (!find (trimmed_string))
                                        {
                                            flag = 1;
                                            break;
                                        }
                                    next_to_amj.erase (0, 4);
                                    memory[frame_no][0] = trimmed_string[0];	//Store this instructions
                                    memory[frame_no][1] = trimmed_string[1];	//in memory which is available
                                    memory[frame_no][2] = trimmed_string[2];
                                    memory[frame_no][3] = trimmed_string[3];
                                    frame_no++;
//if(m % 10 == 0)
// blocks++; //1 block = 10 m
                                }
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
    int
    VA,
    address,
    row;
//for(IC = 0;IC <= m; IC++)
//for(int i=PTR;i<(PTR+10);i++)
// cout<<i<<memory[i][0]<<memory[i][1]<<memory[i][2]<<memory[i][3]<<endl;
    RA = ADDRESS_MAP (IC, 0);
    while (IC < 23)
        {
//cout<<"kijuhtyrfyghknjmljuiyt"<<RA<<endl;
            string
            temp,
            temp1,
            instruction;
            for (int j = 0; j < 4; j++)
                IR[j] = memory[RA][j];
            temp = IR[0];
            temp1 = IR[1];
            instruction = temp + temp1;
            cout << instruction << IR[2] << IR[3] << endl;
            VA = ((IR[2] - '0') * 10) + (IR[3] - '0');
            RA = ADDRESS_MAP (VA, 0);
            if (instruction == "GD")
                {
                    SI = 1;		//For GD System interupt is set to 1
                    MOS ();
                }
            else if (instruction == "PD")
                {
                    SI = 2;		//For PD System interupt is set to 2
                    MOS ();
                }
            else if (instruction == "LR")
                {
                    VA = calculate_address ();
                    address = ADDRESS_MAP (VA, 0);
                    cout << "In lrrrrrrrr " << "\t" << address << "\t" << VA << "\t" <<
                         memory[address][2] << memory[address][3] << endl;
//row = ((memory[address][2] -'0')*10) + (memory[address][3]-'0');
//row *= 10;
//address *= 10;
                    for (int i = 0; i < 4; i++)
                        R[i] = memory[address][i];
                    cout << "value of r is " << R << endl;
                }
            else if (instruction == "SR")
                {
                    VA = calculate_address ();
                    if (VA % 10 == 0)
                        address = ADDRESS_MAP (VA, 1);
                    else
                        address = ADDRESS_MAP (VA, 0);
                    /*
                    int allocator = ALLOCATE();
                    memory[address][0] = 0;
                    memory[address][2] = ((allocator/10) + '0');
                    memory[address][3] = ((allocator%10) + '0');
                    */
                    cout << "In srrrrrrrr " << "\t" << address << "\t" << VA << "\t" <<
                         "fdg sdddddc " << endl;
//row = ((memory[address][2] - '0') * 10) + (memory[address][3] - '0');
//row *= 10;
//cout<<"rowwwwwwwww in srrrrrrr "<<row<<endl;
                    for (int i = 0; i < 4; i++)
                        memory[address][i] = R[i];
                    cout << "value of mem is " << memory[address] << endl;
                }
            else if (instruction == "CR")
                {
                    calculate_address ();
                    int
                    flag = 0;
                    for (int i = 0; i < 4; i++)
                        if (memory[row_no][i] != R[i])
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
                        IC = ((IR[2] - 48) * 10) + (IR[3] - 48) - 1;
                    C = 'F';
                    cout << "Instruction counter is now " << IC << endl;
                }
            else
                {
                    PI = 1;

                }
            IC++;
            SIMULATION();
//cout<<"passing ic "<<IC<<endl;
            RA = ADDRESS_MAP (IC, 0);
//cout<<"ra isssssssssssssss "<<RA<<"\t"<<memory[RA][0]<<endl;
        }
}

void MOS ()
{
    int
    VA,
    row_no1;
    if (TI == 0 )
        {
            switch (SI)
                {
                case 1:  	//GD Operation will take place here
                {
                    //i.e reading a line after DTA pointed by data_counter
                    string
                    temp;
                    data = "";
                    VA = calculate_address ();
                    row_no1 = ADDRESS_MAP (VA, 1);
//cout<<"Row no after calculating "<<row_no1<<endl;
//int allocator = ALLOCATE();
//memory[row_no1][0] = 0;
//memory[row_no1][2] = ((allocator/10) + '0');
//memory[row_no1][3] = ((allocator%10) + '0');
                    getline (infile, data);
                    cout << "Data is " << data << "\t" << row_no << endl;
                    temp = data.substr (0, 4);
                    if (temp == "$END" || temp == "")
                        {
                            cout << "Out Of Data \n";
                            reset_memory ();
                            IC = m = data_counter = address = 0;
                            outfile << "Out Of Data \n";
                            outfile << "\n\n";
                        }
                    if (row_no1 > 300)
                        {
                            cout << "Memory is exceeded \n" << endl;
                            cout << "row no " << row_no << endl;
                            reset_memory ();
                            IC = m = data_counter = address = 0;
                            outfile << "Memory is exceeded \n";
                            outfile << "\n\n";
                            go_to_end ();
                        }
                    int
                    counter = 0;
//cout<<"Row nno is "<<allocator<<endl;
                    for (int i = row_no1; counter < (data.length ()); i++)
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
                    int
                    row;
                    string
                    space = " ";
                    cout << "In PD" << IR[2] << IR[3] << endl;
//IR[3] = 0;
                    VA = calculate_address ();
                    int
                    address = ADDRESS_MAP (VA, 0);
                    cout << "Address in pd issssssss " << VA << "\t" << address <<
                         endl;
                    int
                    count = 0;
                    char *
                    buffer = new char[1];
                    for (row = address; count < 10; row++)
                        {
                            count++;
                            for (int col = 0; col < 4; col++)
                                {
                                    if (memory[row][col] != '')
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
                    int
                    row;
                    string
                    space = " ";
                    cout << "In PD" << IR[2] << IR[3] << endl;
//IR[3] = 0;
                    VA = calculate_address ();
                    int
                    address = ADDRESS_MAP (VA, 0);
                    cout << "Address in pd issssssss " << VA << "\t" << address <<
                         endl;
                    int
                    count = 0;
                    char *
                    buffer = new char[1];
                    for (row = address; count < 10; row++)
                        {
                            count++;
                            for (int col = 0; col < 4; col++)
                                {
                                    if (memory[row][col] != '')
                                        {
                                            outfile << memory[row][col];
                                            cout << "Writting " << memory[row][col] << endl;
                                        }
                                }
                        }
                    outfile << "\n";
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
    int
    VA;
    VA = (IR[2] - '0') * 10 + (IR[3] - '0');
//cout<<"edrtfyhjkl ftyghkj ythuj "<<IR[2]<<"\t"<<IR[3]<<"\t"<<VA<<"\t"<<endl;
    return VA;
}


void TERMINATE(int ERR_CODE)
{
    switch(ERR_CODE)
        {
        case 0:
            cout<<"NO ERROR\n";
            break;
        case 1:
            cout<<"OUT OF DATA\n";
            break;
        case 2:
            cout<<"LINE LIMIT EXCEEDED\n";
            break;
        case 3:
            cout<<"TIME LIMIT EXCEEDED\n";
            break;
        case 4:
            cout<<"OPERATION CODE ERROR\n";
            break;
        case 5:
            cout<<"OPERAND ERROR\n";
            break;
        case 6:
            cout<<"INVALID PAGE FAULT\n";
            break;
        case 7:
            cout<<"TIME LIMIT EXCEEDED\n :: OPERATION CODE ERROR\n";
            break;
        case 8:
            cout<<"TIME LIMIT EXCEEDED\n :: OPERAND ERROR\n";
            break;
        }
    go_to_end();
}

void SIMULATION()
{
    TTC++;
    if(TTC == pcb.TLL)
        {
            TI = 2;
        }

}
