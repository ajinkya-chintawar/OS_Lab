#include<iostream>
#include<deque>
#include<string.h>
#include<fstream>
#include<stdlib.h>
typedef struct PCB
{
    int TTL, TLL, JOB_ID;
    int P[10];
    int D[10];
    int F;
} PCB;
using namespace std;
void load ();
int find ();
void startexecution ();
void executeuserprogram ();
int MOS ();
void TERMINATE(int);
int calculate_address ();
void go_to_end ();
void reset_memory ();
int ALLOCATE ();
void SIMULATION();
char memory[300][4] = { ' '	, ' ' }, IR[4], C, R[4];

string next_to_amj = "cdss", data;
int blocks = 0, IC = 0, m = 0,VA, data_counter, address, row_no, col_no, SI =
                                0, PTR, TTC, TLC, PI, TI, RA,flag_for_end,CH1,CH2,CH3,IOI;
bool occupied[30] = { false };
string instruction;
PCB pcb_obj;

char BUFFER[10][4],AUX[100][4];
deque<string> EB;
deque<string> IFB;
deque<string> OFB;
deque<PCB> LQ;
deque<PCB> RQ;
deque<PCB> IOQ;
deque<PCB> TQ;
    
ifstream infile ("test.txt");
ofstream outfile;
std::string::size_type sz;	// alias of size_t
int main ()
{
    outfile.open ("output.txt");
    //load ();
    startexecution();
    infile.close ();
    outfile.close ();
}

int find (string trimmed_string)
{
    string::iterator it;
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
    string temp,file_content,end_check;
    end_check = data.substr(0,4);
    if(end_check != "$END")
	{
		while (1)
	    {
	    	if (!getline (infile, file_content))
	            break;			//Accept a string
	        					//from file
	        temp = file_content.substr (0, 4);	//compare temp to check
											// the actions to be perform
	        if (temp == "$END")	//break the while loop
	        {
	        	break;
	        }
	    }
	}
	flag_for_end = 1;
    reset_memory();
    return;    
}

void reset_memory ()
{
	string buf;
    for (int i = 0; i < 300; i++)
        for (int j = 0; j < 4; j++)
            memory[i][j] = ' ';
    for (int i = 0; i < 100; i++)
        for (int j = 0; j < 4; j++)
            AUX[i][j] = '0';
    occupied[0] = true;
    for(int i=1;i<30;i++)
    	occupied[i] = false;
    for(int i = 1;i <= 10;i++)
    	EB.push_back(buf);
    IC = 0;
    PI = 0;
    TI = 0;
    TTC = 0;
    TLC = 0;
}

PCB init_PCB (string file_content)
{
	PCB pcb;
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
    for(int i = 0;i<10;i++)
    {
    	pcb.P[i] = 21;
    	pcb.D[i] = 21;
    }		
    return pcb;
}

int ALLOCATE ()
{
    int page_no;
    while (1)
        {
            page_no = rand () % 30;
            if (!occupied[page_no])
                {
                    occupied[page_no] = true;
                    break;
                }
        }
    return page_no;
}

int ADDRESS_MAP (int VA1)
{
    int loc,fno,RA;
    if (VA1 >= 100)
    {
       PI = 2;
    	MOS();
       return 0;
    }
    loc = PTR + (VA1 / 10);
	if(memory[loc][0] == '0')
	{
        PI = 3;
        MOS();
        if(flag_for_end)
        	return -1;
    }    
	fno = (memory[loc][2] - '0') * 10 + (memory[loc][3] - '0');
    RA = (fno * 10) + (VA1 % 10);
    return RA;
}

void set_pt (int PTR)
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
            break;
        }
    }
}
int get_next_empty(int arr[10])
{
	for (int i = 0;i < 10;i++)
		if(arr[i] == 21)
			return i;
}
void IR1()
{
	//PCB pcb_obj;
	cout<<"cming in IR1 "<<endl;
	//int frame_no;
    //char buf[10];
	string buf;
	cout<<"eb is "<<EB.empty()<<endl;
	if(!EB.empty())
	{
		buf = EB.front();
		EB.pop_front();
	}
	else
		return;
	//cout<<"string is "<<buf<<endl;
    
    string sub_string,temp,trimmed_string;
    if (!getline (infile, buf))
        return;			//Accept a string
						//from file
    cout<<"string is "<<buf<<endl;
    temp = buf.substr (0, 4);	//compare temp to check
										// the actions to be perform
    if (temp == "$END")  	//break the while loop
    {							
        LQ.push_back(pcb_obj);
        cout<<"Pushing pcb with id "<<pcb_obj.JOB_ID<<endl;
        //EB.push_back(buf);
    }
    else if (temp == "$AMJ")	//for $AMJ go on accepting
								//from file till you get H
								//And store it in memory initial blocks
    {
    	cout<<"setting pcb "<<endl;
    	pcb_obj = init_PCB (buf);
        pcb_obj.F = 'P';
    	cout<<"setting pcb with id "<<pcb_obj.JOB_ID<<endl;
	
    }
    else if (temp == "$DTA")	// Start executing control cards
        pcb_obj.F = 'D';
    else
    {
    	IFB.push_back(buf);
    	cout<<"Added in ifb "<<endl;
    }
    EB.push_back(buf);
}
void IR3()
{
	cout<<"cmin in ir 3"<<endl;
	int i,temp,count=0;
	string buf;
	cout<<IFB.empty()<<endl;
	if(!IFB.empty())
	{
		buf = IFB.front();
		IFB.pop_front();		
		cout<<"in ib data is "<<buf<<endl;
		for( i = 0;i < 100; i +=10 )	//Getting track for storage
			if(AUX[i][0] == '0')
			{
				if(pcb_obj.F == 'P')
				{
					temp = get_next_empty(pcb_obj.P);
					pcb_obj.P[temp] = (i/10);
					cout<<"temp ALLOCATEd is "<<i<<"\t"<<temp<<endl;
					break;
				}
				else
				{
					temp = get_next_empty(pcb_obj.D);
					pcb_obj.D[temp] = (i/10);
					cout<<"temp ALLOCATEddddd is "<<i<<endl;
					break;
				}
			}
		// for(int i1 = 0 ; i1 < 10; i1++)
		// 	cout<<pcb_obj.P[i1]<<endl;
		
		int track = i;
		//int count = 0;
		for(int j = track;j < (track + 10);j++ )
		{
			for(int k = 0;k < 4;k++)
			{
				//cout<<"in loop value are "<<j<<"\t"<<k<<"\t"<<count<<"\t"<<buf.length()<<endl;
				if(count >= buf.length())
					break;
				AUX[j][k] = buf[count];
				count++;
			}
			if(count > buf.length())
				break;
		}
	}
	// else
	// 	return;
	else if(!LQ.empty())
	{
		cout<<"LOADING         "<<endl;
		PCB pcb;
		int i1,track_no,frame_no,fm;
		pcb = LQ.front();
		LQ.pop_front();
		cout<<"Checking p of pcb "<<pcb.JOB_ID<<endl;
		for(i1 = 0 ; i1 < 10; i1++)
			cout<<pcb_obj.P[i1]<<endl;
		for(i1 = 0 ; i1 < 10; i1++)
			if(pcb_obj.P[i1] != 21)
			{
				track_no = pcb_obj.P[i1];
				pcb_obj.P[i1] = 21 ;
				cout<<"Initialsised       "<<pcb.P[i1]<<endl;
				break;
			}
		cout<<"got pcard at "<<i1<<endl;
		if(i1 != 10)
		{
			LQ.push_back(pcb);
			//return;
		}	
		if(i1 == 10)
		{
			RQ.push_back(pcb);
			return;
		}	
		frame_no = ALLOCATE ();
        update_page_table (frame_no);
        frame_no *= 10;
        fm =frame_no;
        track_no *= 10;
        int tk = track_no;
        cout<<"frame_no is "<<frame_no<<endl;
        while(AUX[track_no][0] != '0')
        {
             memory[frame_no][0] = AUX[track_no][0];	//Store this instructions
             memory[frame_no][1] = AUX[track_no][1];	//in memory which is available
             memory[frame_no][2] = AUX[track_no][2];
             memory[frame_no][3] = AUX[track_no][3];
            // AUX[track_no][0] = '0';
            // AUX[track_no][1] = '0';
            // AUX[track_no][2] = '0';
            // AUX[track_no][3] = '0';
            frame_no++;
            track_no++;
        }
        cout<<"Prog stored at "<<fm<<" ft "<<endl;
        for (int i =fm;i<frame_no;i++)
        	cout<<memory[i][0]<<memory[i][1]<<memory[i][2]<<memory[i][3]<<endl;
	}
}
void IR2()
{
	
}

void startexecution ()
{
    IC = 0;
    executeuserprogram ();
}

void executeuserprogram ()
{
	reset_memory();
	PTR = ALLOCATE ();
    PTR *= 10;
    PTR += 1;
    set_pt (PTR);
    
	int i = 0;
	while(i < 350)
    {
    	SIMULATION();
    	i++;
    }	
    for(int i = 0;i<100;i++)
    	cout<<AUX[i][0]<<AUX[i][1]<<AUX[i][2]<<AUX[i][3]<<endl;
    cout<<"Prog in  mem is "<<endl;
    for(int i = 0;i<300;i++)
    	cout<<memory[i][0]<<memory[i][1]<<memory[i][2]<<memory[i][3]<<endl;
    
    //if(PI != 0 || TI !=0 )
    //	MOS();
 //    if(flag_for_end)
	// {
	// 	flag_for_end = 0;
	// 	break;
	// }
	// TTC++;
 //    }
//    reset_memory();
}

bool isvalid(char num)
{
	if(num >= '0' && num <= '9')
		return true;
	return false;
}
int calculate_address ()
{
	if(isvalid(IR[2]) && isvalid(IR[3]))
    	VA = (IR[2] - '0') * 10 + (IR[3] - '0');
    else 
    {
    	PI = 2;
    	SIMULATION();
    	MOS();
    	return 0;
    }
    return VA;
}
int MOS()
{
	switch(IOI)
	{
		case 1 : 
				IR1();
				break;
		case 2 : 
				IR2();
				break;
		case 3 : 
				IR1();
				IR2();
				break;
		case 4 : 
				IR3();
				break;
		case 5 : 
				IR1();
				IR3();
				break;
		case 6 : 
				IR2();
				IR3();
				break;

	}
	IOI = 0;
}
void SIMULATION()
{	
	CH1++;
	if(CH1 == 5)
	{
		IOI += 1;
		CH1 = 0;
	}	
	CH2++;
	if(CH2 == 5)
	{
		IOI += 2;
		CH2 = 0;
	}
	CH3++;
	if(CH3 == 2)
	{
		IOI += 4;
		CH3 = 0;
	}	
	cout<<"Contents in channel are "<<endl;
	string t1=" ";PCB t2;
	if(!IFB.empty())
	{
		t1 = IFB.front();
	}
	if(!LQ.empty())
		t2 = LQ.front();
	
	cout<<t1<<"\t"<<t2.JOB_ID<<endl;
    /*if(TTC >= pcb.TTL)
        {
        	TI = 2;
            MOS();
        }
    */
    if(IOI != 0)
    	MOS();    
}
