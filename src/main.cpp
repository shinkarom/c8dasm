#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

typedef unsigned short int usint;

int address;

void int_to_hex(usint n,stringstream &str)
{
	if(n>=10) str<<char(n-10+65); else str<<char(n+48);
}

void three_bytes(usint n,stringstream &str)
{
	if(n<255) str<<"0";
	if(n<15) str<<"0";
	str<<(n&0xFFF);
}

void reg1(usint n, stringstream &str)
{
	usint reg = ((n&0x0F00)>>8)&0x0F;
	str<<"V";
	int_to_hex(reg,str);
}

void reg2(usint n, stringstream &str)
{
	usint reg = (n&0xF0)>>4;
	str<<"V";
	int_to_hex(reg,str);
}

void get_byte(usint n, stringstream &str)
{
	usint bt = (n&0x00FF);
	if(bt<16) str<<"0";
	str<<hex<<bt;
}

string parse(usint param)
{
	stringstream s;
	s<<uppercase<<hex;
	//1
	if((param&0xF000)==0x1000)
	{
		s<<"JP ";
		three_bytes(param&0x0FFF,s);
	} else
	//CLS
	if(param==0x00E0)
	{
		s<<"CLS";
	} else
	//RET
	if(param==0x00EE)
	{
		s<<"RET";
	} else		
	//2
	if((param&0xF000)==0x2000)
	{
		s<<"CALL ";
		three_bytes(param&0x0FFF,s);
	} else
	//3
	if((param&0xF000)==0x3000)
	{
		s<<"SE ";
		reg1(param,s);
		s<<", ";
		get_byte(param,s);
	} else		
	//4
	if((param&0xF000)==0x4000)
	{
		s<<"SNE ";
		reg1(param,s);
		s<<", ";
		get_byte(param,s);	
	} else
	//5
	if((param&0xF00F)==0x5000)
	{
		s<<"SE ";
		reg1(param,s);
		s<<", ";
		reg2(param,s);	
	} else
	//6
	if((param&0xF000)==0x6000)
	{
		s<<"LD ";
		reg1(param,s);
		s<<", ";
		get_byte(param,s);
	} else	
	//7
	if((param&0xF000)==0x7000)
	{
		s<<"ADD ";
		reg1(param,s);
		s<<", ";
		get_byte(param,s);
	} else	
	//8		
	if((param&0xF00F)==0x8000)
	{
		s<<"LD ";
		reg1(param,s);
		s<<", ";
		reg2(param,s);	
	} else
	if((param&0xF00F)==0x8001)
	{
		s<<"OR ";
		reg1(param,s);
		s<<", ";
		reg2(param,s);	
	} else
	if((param&0xF00F)==0x8002)
	{
		s<<"AND ";
		reg1(param,s);
		s<<", ";
		reg2(param,s);	
	} else
	if((param&0xF00F)==0x8003)
	{
		s<<"XOR ";
		reg1(param,s);
		s<<", ";
		reg2(param,s);	
	} else
	if((param&0xF00F)==0x8004)
	{
		s<<"ADD ";
		reg1(param,s);
		s<<", ";
		reg2(param,s);	
	} else
	if((param&0xF00F)==0x8005)
	{
		s<<"SUB ";
		reg1(param,s);
		s<<", ";
		reg2(param,s);	
	} else
	if((param&0xF00F)==0x8000)
	{
		s<<"SHR ";
		reg1(param,s);
	} else
	if((param&0xF00F)==0x8007)
	{
		s<<"SUBN ";
		reg1(param,s);
		s<<", ";
		reg2(param,s);	
	} else
	if((param&0xF00F)==0x800E)
	{
		s<<"SHL ";
		reg1(param,s);	
	} else
	//9
	if((param&0xF00F)==0x9000)
	{
		s<<"SNE ";
		reg1(param,s);
		s<<", ";
		reg2(param,s);	
	} else
	//A
	if((param&0xF000)==0xA000)
	{
		s<<"LD I, ";
		three_bytes(param&0x0FFF,s);
	} else	
	//B
	if((param&0xF000)==0xB000)
	{
		s<<"JP V0, ";
		three_bytes(param&0x0FFF,s);
	} else
	//C
	if((param&0xF000)==0xC000)
	{
		s<<"RND ";
		reg1(param,s);
		s<<", ";
		get_byte(param,s);
	} else
	//D
	if((param&0xF000)==0xD000)
	{
		s<<"DRW ";
		reg1(param,s);
		s<<", ";
		reg2(param,s);
		s<<", ";
		usint nibble = (param&0x000F);
		s<<nibble;
	} else
	//E
	if((param&0xF0FF)==0xE09E)
	{
		s<<"SKP ";
		reg1(param, s);
	} else
	if((param&0xF0FF)==0xE0A1)
	{
		s<<"SKNP ";
		reg1(param, s);
	} else
	//F
	if((param&0xF0FF)==0xF007)
	{
		s<<"LD ";
		reg1(param, s);
		s<<", DT";
	} else
	if((param&0xF0FF)==0xF00A)
	{
		s<<"LD ";
		reg1(param, s);
		s<<", K";
	} else
	if((param&0xF0FF)==0xF015)
	{
		s<<"LD DT, ";
		reg1(param, s);
	} else		
	if((param&0xF0FF)==0xF018)
	{
		s<<"LD ST, ";
		reg1(param, s);
	} else	
	if((param&0xF0FF)==0xF01E)
	{
		s<<"ADD I, ";
		reg1(param, s);
	} else	
	if((param&0xF0FF)==0xF029)
	{
		s<<"LD F, ";
		reg1(param, s);
	} else	
	if((param&0xF0FF)==0xF033)
	{
		s<<"LD B, ";
		reg1(param, s);
	} else
	if((param&0xF0FF)==0xF055)
	{
		s<<"LD [I], ";
		reg1(param, s);
	} else	
	if((param&0xF0FF)==0xF065)
	{
		s<<"LD ";
		reg1(param, s);
		s<<", [I]";
	}
	return s.str();
}

string process(usint param)
{
	usint hi = (param>>8)&0xFF;
	usint lo = param&0x00FF;
	stringstream s;
	s<<uppercase<<hex<<address<<" ";
	if(hi<16) s<<'0';
	s<<hi;
	if(lo<16) s<<'0'; 
	s<<lo;
	s<<" "<<parse(param);
	return s.str();
}

int main(int argc, char *argv[])
{

	if(argc==1)
	{
		cout<<"Please specify input file.";
		return 1;
	}
	ofstream ofs;
	if(argc>=3)
	{
		ofs.open(argv[2],ios::out|ios::trunc);
		if(!ofs.is_open()){
			cout<<"Unable to open input file.";
			return 1;
		}			
	}
	address = 0x200;
	
	ifstream ifs(argv[1],ios::in|ios::binary);
	if(!ifs.is_open()){
		cout<<"Unable to open input file.";
		return 1;
	}

	unsigned char command[2];
	while(!ifs.eof())
	{
	ifs.read((char*)command,2);
	usint thecommand = command[0]*256+command[1];	
	string result = process(thecommand);
	cout<<result<<endl;
	if(argc>=3)
	{
	ofs<<result<<endl;	
	}
	address+=2;	
	}
	return 0;
}