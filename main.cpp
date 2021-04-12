#include<iostream>
#include"console.h"
#include<fstream>
#include<time.h>
#include<conio.h>
#include<string>
#include<string.h>
#include<iomanip>
#include"Dohoa.h"
#include<math.h>
#define Maunen 70
#define Mauchu 700


const int MaxNodes = 10000;
int SoNgayDcMuon = 7; //so ngay toi da duoc muon sach
const int MienGiaTri = 30000;
static bool* MangRandom = new bool[MienGiaTri];
int SoLuongDocGia = 0;
int tdXBox = 65;
int tdYBox = 9;
using namespace std;
typedef char str[100];

str MenuMain[14] = {
"             Them Doc Gia             ", //1
"             Xoa Doc Gia              ", //2
"             Sua Doc Gia              ", //3
"          Mo Va Khoa Doc Gia          ", //4
"           Danh Sach Doc Gia          ", //5
"             Them Dau Sach            ", //6
"    DS Cac Dau Sach Theo The Loai     ", //7
"          Tim Thong Tin Sach          ", //8
"               Muon Sach              ", //9
"    DS Cac Sach Doc Gia Dang Muon     ", //10
"               Tra Sach               ", //11
"     DS Doc Gia Muon Sach Qua Han     ", //12
"   10 Sach Co So Lan Muon Nhieu Nhat  ", //13
"                Thoat                 "  //0
};


enum TRANGTHAI{UP, DOWN, RIGHT, LEFT, ENTER, BACK};

TRANGTHAI key(int z){
	if(z==224){
		char c;
		c = _getch();
		if(c == 72)
			return UP;
		if(c==80){
			return DOWN;
		}
		if(c== 77){
			return RIGHT;
		}
		if(c == 75){
			return LEFT;
		}
	}
	else if(z==13){
		return ENTER;
	}
	else if(z==27){
		return BACK;
	}
}

struct Date
{
	int Ngay;
	int Thang;
	int Nam;
};
struct DanhMucSach{
	string MaSach;
	int TrangThai;
	string vitri;
};
struct Node_Sach{
	DanhMucSach data;
	struct Node_Sach* pNext;
};


struct LIST_DMS{
	Node_Sach* pHead;
	Node_Sach* pTail;
};

struct DauSach
{
	string ISBN;
	string TenSach;
	int SoTrang;
	string TacGia;
	int NamXuatBan;
	string TheLoai;
	int SoLuongSach;
	int SoLanMuon;
	LIST_DMS List_DMS;
};	

struct MCONTRO
{
	int SoLuongDauS;
	DauSach* nodesDauSach[MaxNodes];
};

struct MuonTra
{
	string MaSach;
	Date NgayMuon;
	Date NgayTra;
	int TrangThai; // trang thai =0 la sach dang muon (chua tra), =1 la da tra, =2 : lam mat sach 
};
struct NODE_DOUBLELIST
{
	MuonTra data;
	struct NODE_DOUBLELIST* pNext;
	struct NODE_DOUBLELIST* pPrev;
};
struct DOUBLELIST
{
	NODE_DOUBLELIST* pHead;
	NODE_DOUBLELIST* pTail;
};
struct TheDocGia
{
	int MaThe;
	string Ho;
	string Ten;
	string Phai;
	int TrangThai;
	int SoLuongSachDangMuon;
	int SoNgayQuaHanMax;
	DOUBLELIST DoubleList_MuonTra;
};
struct NODE_TREE
{
	TheDocGia data;
	NODE_TREE* pLeft;
	NODE_TREE* pRight;
};

typedef NODE_TREE* TREE;


int Random()
{
	srand(time(NULL));
	int a;
	while (true)
	{
		a = 1 + rand() % MienGiaTri;
		if (MangRandom[a] == false)
		{
			MangRandom[a] = true;
			SoLuongDocGia++;
			return a;
		}
	}
}

int ChuyenThang(string str)
{
	if (str == "Jan")
	{
		return 1;
	}
	else if (str == "Feb")
	{
		return 2;
	}
	else if (str == "Mar")
	{
		return 3;
	}
	else if (str == "Apr")
	{
		return 4;
	}
	else if (str == "May")
	{
		return 5;
	}
	else if (str == "Jun")
	{
		return 6;
	}
	else if (str == "Jul")
	{
		return 7;
	}
	else if (str == "Aug")
	{
		return 8;
	}
	else if (str == "Sep")
	{
		return 9;
	}
	else if (str == "Oct")
	{
		return 10;
	}
	else if (str == "Nov")
	{
		return 11;
	}
	else if (str == "Dec")
	{
		return 12;
	}
}
void LayNgayHienTai(Date& d)
{
	// tra ve date/time hien tai dua tren system hien tai
	time_t hientai = time(0);
	// chuyen doi hientai thanh dang chuoi
	char* dt = ctime(&hientai);
	string str1, str2, str3;
	//ngay
	for (int i = 8; i < 10; i++)
	{
		str1.push_back(dt[i]);
	}
	d.Ngay = atoi(str1.c_str());
	// thang
	for (int i = 4; i < 7; i++)
	{
		str2.push_back(dt[i]);
	}
	d.Thang = ChuyenThang(str2);
	//nam
	for (int i = 20; i < 24; i++)
	{
		str3.push_back(dt[i]);
	}
	d.Nam = atoi(str3.c_str());
}
// Xu Ly Ngay
bool KTNamNhuan(int nam)
{
	if ((nam % 4 == 0 && nam % 100 != 0) || nam % 400 == 0)
	{
		return true;
	}
	else
		return false;
}
	// 1:ngay > ngay1; 2:ngay < ngay1; 3:ngay=ngay1
int SoSanhNgay(Date d, Date t) 
{
	if (d.Nam == t.Nam)
	{
		if (d.Thang == t.Thang)
		{
			if (d.Ngay > t.Ngay)
			{
				return 1;
			}
			else if (d.Ngay < t.Ngay)
			{
				return 2;
			}
			else
				return 3;
		}
		else if (d.Thang > t.Thang)
		{
			return 1;
		}
		else
			return 2;
	}
	else if (d.Nam > t.Nam)
	{
		return 1;
	}
	else
	{
		return 2;
	}
}


int SoNgayCuaThang(int thang)
{
	if (thang == 1 || thang == 3 || thang == 5 || thang == 7 || thang == 8 || thang == 10 || thang == 12)
	{
		return 31;
	}
	else if (thang == 2)
	{
		if (KTNamNhuan)
		{
			return 29;
		}
		else
			return 28;
	}
	else
		return 30;
}

void XuLyNhap_Chu(string & InPut, int toadoX, int& toadoY, int LenghtMax)
{
	int toadoXtam;
	char c;
	while (true)
	{
		char c = _getch();
		if (((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) && InPut.length() < LenghtMax)
		{
			InPut.insert(InPut.begin() + (whereX() - toadoX), c);
			if ((whereX() - toadoX) < InPut.length() - 1)
			{
				toadoXtam = whereX() + 1;
				gotoXY(toadoX, whereY());
				cout << "                                               ";
				gotoXY(toadoX, whereY());
				cout << InPut;
				gotoXY(toadoXtam, whereY());
			}
			else
				cout << c;
		}
		else if (c == 8 && whereX() > toadoX)
		{
			InPut.erase(InPut.begin() + (whereX() - toadoX) - 1);
			toadoXtam = whereX() - 1;
			gotoXY(toadoX, whereY());
			cout << "                                               ";
			gotoXY(toadoX, whereY());
			cout << InPut;
			gotoXY(toadoXtam, whereY());
		}
		else if (c == 32 && InPut.length() < LenghtMax && InPut.length() != 0 && InPut[whereX() - toadoX - 1] != ' ')
		{
			InPut.insert(InPut.begin() + (whereX() - toadoX), ' ');
			if ((whereX() - toadoX) < InPut.length() - 1)
			{
				toadoXtam = whereX() + 1;
				gotoXY(toadoX, whereY());
				cout << "                                               ";
				gotoXY(toadoX, whereY());
				cout << InPut;
				gotoXY(toadoXtam, whereY());
			}
			else
				cout << c;
		}
		else if (c == 13 && InPut.length() > 0)
		{
			toadoY = toadoY + 3;
			break;
		}
		else if (c == -32)
		{
			c = getch();
			if (c == 75 && whereX() > toadoX)
			{
				cout << "\b";
			}
			else if (c == 77 && whereX() < toadoX + InPut.length())
			{
				gotoXY(whereX() + 1, whereY());
			}
			else if (c == 80)
			{
				toadoY = toadoY + 3;
				break;
			}
			else if (c == 72)
			{
				toadoY = toadoY - 3;
				break;
			}
		}
	}
}

char Chuyen1SoSangKiTu(int n)
{
	switch (n)
	{
	case 1:
		return '1';
	case 2:
		return '2';
	case 3:
		return '3';
	case 4:
		return '4';
	case 5:
		return '5';
	case 6:
		return '6';
	case 7:
		return '7';
	case 8:
		return '8';
	case 9:
		return '9';
	case 0:
		return '0';
	}
}

void XuatPage(int page)
{
	gotoXY(110, 34);
	cout << "<-";
	gotoXY(115, 34);
	cout << "->";
	gotoXY(113, 34);
	cout << page;
}

// Nhap
string ChuyenSoSangString(int n)
{
	string str;
	int a;
	if (n == 0)
	{
		str = "0";
	}
	while (n > 0)
	{
		a = n % 10;
		Chuyen1SoSangKiTu(a);
		str.insert(str.begin() + 0, Chuyen1SoSangKiTu(a));
		n = n / 10;
	}
	return str;
}
string NoiChuoi(string str, int n)
{
	string str2;
	str2 = str;
	string str1 = ChuyenSoSangString(n);
	str2.push_back('_');
	for (int i = 0; i < str1.length(); i++)
	{
		str2.push_back(str1[i]);
	}
	return str2;
}
// Xuat
string ChuyenSoSangString1(int n)
{
	string str;
	int a;
	while (n > 0)
	{
		a = n % 10;
		Chuyen1SoSangKiTu(a);
		str.insert(str.begin() + 0, Chuyen1SoSangKiTu(a));
		n = n / 10;
	}
	return str;
}


void XuLyNhap_So(int& n, int toadoX, int& toadoY)
{
	int toadoXtam;
	string x;
	x = ChuyenSoSangString1(n);
	char c;
	while (true)
	{
		c = getch();
		if ((c >= 48 && c <= 57) && whereX() < toadoX + 47)
		{
			x.insert(x.begin() + (whereX() - toadoX), c);
			if ((whereX() - toadoX) < x.length() - 1)
			{
				toadoXtam = whereX() + 1;
				gotoXY(toadoX, whereY());
				cout << "                                               ";
				gotoXY(toadoX, whereY());
				cout << x;
				gotoXY(toadoXtam, whereY());
			}
			else
				cout << c;
		}
		else if (c == 8 && whereX() > toadoX)
		{
			x.erase(x.begin() + (whereX() - toadoX) - 1);
			toadoXtam = whereX() - 1;
			gotoXY(toadoX, whereY());
			cout << "                                               ";
			gotoXY(toadoX, whereY());
			cout << x;
			gotoXY(toadoXtam, whereY());
		}
		else if (c == 13 && x.length() > 0)
		{
			toadoY = toadoY + 3;
			n = atoi(x.c_str());
			break;
		}
		else if (c == -32)
		{
			c = getch();
			if (c == 75 && whereX() > toadoX)
			{
				cout << "\b";
			}
			else if (c == 77 && whereX() < toadoX + x.length())
			{
				gotoXY(whereX() + 1, whereY());
			}
			else if (c == 80)
			{
				if (x.empty())
				{
					n = 0;
				}
				else
				{
					n = atoi(x.c_str());
				}
				toadoY = toadoY + 3;
				break;
			}
			else if (c == 72)
			{
				if (x.empty())
				{
					n = 0;
				}
				else
				{
					n = atoi(x.c_str());
				}
				toadoY = toadoY - 3;
				break;
			}
		}
	}
}



void ChuanHoaChuoi(string& str)
{
	//Xoa khoang trong
	while (str[0] == ' ')
	{
		str.erase(str.begin() + 0);
	}
	while (str[str.length() - 1] == ' ')
	{
		str.erase(str.begin() + (str.length() - 1));
	}

	for (int i = 1; i < str.length() - 1; i++)
	{
		if (str[i + 1] == ' ' && str[i] == ' ')
		{
			str.erase(str.begin() + i);
			i--;
		}
	}
	//Viet thuong all lai
	for (int i = 0; i < str.length(); i++)
	{
		if ((65 <= str[i] && str[i] <= 90) && str[i] != ' ')
		{
			str[i] = str[i] + 32;
		}
	}
	//Viet hoa
	for (int i = 0; i < str.length(); i++)
	{
		if (i == 0 && (97 <= str[i] && str[i] <= 122))
		{
			str[0] = str[0] - 32;
		}
		if (str[i] == ' ' && (97 <= str[i+1] && str[i + 1] <= 122))
		{
			str[i + 1] = str[i + 1] - 32;
		}
	}
}

void VeHCN(int toadoX, int toadoY, int chieudai, int chieucao, int mauSet)
{
	SetColor(mauSet);
	for (int i = toadoX; i <= chieudai + toadoX; i++)
	{
		gotoXY(i, toadoY);
		if (i == toadoX)
			cout << (char)218;
		else if (i == chieudai + toadoX)
			cout << (char)191;
		else
			cout << (char)196;
	}
	for (int i = toadoX; i <= chieudai + toadoX; i++)
	{
		gotoXY(i, chieucao + toadoY);
		if (i == toadoX)
			cout << (char)192;
		else if (i == chieudai + toadoX)
			cout << (char)217;
		else
			cout << char(196);
	}
	for (int i = toadoY + 1; i < chieucao + toadoY; i++)
	{
		gotoXY(toadoX, i);
		cout << (char)179;
	}
	for (int i = toadoY + 1; i < chieucao + toadoY; i++)
	{
		gotoXY(chieudai + toadoX, i);
		cout << (char)179;
	}
	SetColor(7);
}

void XoaManHinh(int tdX,int tdY,int chieudai,int chieucao)
{
	TextColor(7);
	for (int i = tdY; i < chieucao+tdY; i++)
	{
		for (int j = tdX; j < tdX + chieudai; j++)
		{
			gotoXY(j, i);
			cout << " ";
		}
	}
}

//======================= cau a ================
void GiaoDienThemDocGia(int toadoX, int toadoY)
{
	VeHCN(40,4,22,4,2);
	gotoXY(45,6);
	TextColor(3);
	cout<<"NHAP DOC GIA";
	SetColor(15);
	gotoXY(15, 10);
	cout << "Ma The:";
	VeHCN(35, 12, 50, 2, 4);
	gotoXY(15, 13);
	cout << "Nhap Ho va Ten Lot:";
	VeHCN(35, 15, 50, 2, 4);
	gotoXY(15, 16);
	cout << "Nhap Ten:";
	gotoXY(15, 19);
	cout << "Chon Phai:";
	VeHCN(75, 18, 10, 2, 4);
	gotoXY(toadoX + 5, toadoY + 7);
	cout << "Nam";
	gotoXY(toadoX + 20, toadoY + 7);
	cout << "Nu";
	gotoXY(toadoX + 15, toadoY + 10);
	cout << "Dang Ki";
	gotoXY(toadoX + 30, toadoY + 10);
	cout << "Cancel";

}
void ChonPhai(int toadoX, int toadoY, string& Phai, int& pointer2)
{
	ShowCur(0);
	int pointer1 = toadoX;
	while (true)
	{
		gotoXY(toadoX, toadoY);
		cout << "Nam";
		gotoXY(toadoX + 15, toadoY);
		cout << "Nu";

		if (kbhit())
		{
			char c = getch();
			if (c == -32)
			{
				c = getch();
				if (c == 75)
				{
					if (pointer1 == toadoX)
						pointer1 = toadoX + 15;
					else
						pointer1 = toadoX;
				}
				else if (c == 77)
				{
					if (pointer1 == toadoX + 15)
						pointer1 = toadoX;
					else
						pointer1 = toadoX + 15;
				}
				else if (c == 72)
				{
					ShowCur(1);
					pointer2 = pointer2 - 3;
					break;
				}
				else if (c == 80)
				{
					pointer2 = pointer2 + 3;
					break;
				}
			}
			if (c == 13)
			{
				gotoXY(135, 24);
				cout << "   ";
				gotoXY(pointer1, toadoY);
				SetColor(4);
				if (pointer1 == toadoX)
				{
					cout << "Nam";
					gotoXY(77, 19);
					cout << "Nam";
					Phai = "Nam";
				}
				else if (pointer1 == toadoX + 15)
				{
					cout << "Nu";
					gotoXY(77, 19);
					cout << "Nu";
					Phai = "Nu";
				}
				TextColor(7);
				break;
			}
		}

		gotoXY(pointer1, toadoY);
		SetColor(4);
		if (pointer1 == toadoX)
		{
			cout << "Nam";
		}
		else if (pointer1 == toadoX + 15)
		{
			cout << "Nu";
		}
		TextColor(7);
		Sleep(100);
	}
}

bool NhapDocGia(int toadoX, int toadoY,int &MaThe, string & Ho, string & Ten, string & Phai, int &TrangThai)
{
	GiaoDienThemDocGia(toadoX, toadoY);
	int pointer1 = toadoY;
	MaThe = Random();
	gotoXY(23,10);
	cout<<MaThe;
	TrangThai=1;
	while (true)
	{
		if (pointer1 == toadoY) // nhap ho
		{
			gotoXY(toadoX + 2 + Ho.length(), pointer1 + 1);
			XuLyNhap_Chu(Ho, toadoX + 2, pointer1,30);
			if (pointer1 < toadoY)
			{
				pointer1 = toadoY + 9;
			}
		}
		else if (pointer1 == toadoY + 3) // nhap ten
		{
			gotoXY(toadoX + 2 + Ten.length(), pointer1 + 1);
			XuLyNhap_Chu(Ten, toadoX + 2, pointer1,8);
		}
		else if (pointer1 == toadoY + 6) // gioi tinh
		{
			ChonPhai(toadoX + 5, toadoY + 7, Phai, pointer1);
		}
		else if (pointer1 == toadoY + 9) // save hoac huy
		{
			ShowCur(0);
			int pointer2 = toadoX + 15;
			while (true)
			{
				if (kbhit())
				{
					char c = getch();
					if (c == 13)
					{
						if (!Ho.empty() && !Ten.empty() && !Phai.empty() && pointer2 == toadoX + 15)
						{
							ChuanHoaChuoi(Ho);
							ChuanHoaChuoi(Ten);
							return true;
						}
						else if (pointer2 == toadoX + 30)
						{
							return false;
						}
						else
						{
							TextColor(7);
							gotoXY(60, 30);
							cout << "Vui Long Dien Day Du Thong Tin";
						}
					}
					else if (c == -32)
					{
						c = getch();
						if (c == 72)
						{
							TextColor(7);
							gotoXY(toadoX + 30, toadoY + 10);
							cout << "Cancel";
							gotoXY(toadoX + 15, toadoY + 10);
							cout << "Dang Ki";
							pointer1 = pointer1 - 3;
							break;
						}
						else if (c == 80)
						{
							TextColor(7);
							gotoXY(toadoX + 30, toadoY + 10);
							cout << "Cancel";
							gotoXY(toadoX + 15, toadoY + 10);
							cout << "Dang Ki";;
							pointer1 = toadoY;
							break;
						}
						else if (c == 75)
						{
							if (pointer2 == toadoX + 30)
							{
								pointer2 = toadoX + 15;
							}
							else
								pointer2 = toadoX + 30;
						}
						else if (c == 77)
						{
							if (pointer2 == toadoX + 15)
							{
								pointer2 = toadoX + 30;
							}
							else
								pointer2 = toadoX + 15;
						}
						gotoXY(pointer2, toadoY + 10);
					}
				}

				SetColor(5);
				gotoXY(pointer2, pointer1+1);
				if (pointer2 == toadoX + 15)
				{
					cout << "Dang Ki";
					TextColor(7);
					gotoXY(toadoX + 30, toadoY + 10);
					cout << "Cancel";
				}
				else if (pointer2 == toadoX + 30)
				{
					cout << "Cancel";
					TextColor(7);
					gotoXY(toadoX + 15, toadoY + 10);
					cout << "Dang Ki";
				}

				
			}
		}
		Sleep(100);
	}
}

void ThemTheDocGia(TREE & t, TheDocGia x)
{
	
	if (t == NULL)
	{	
		NODE_TREE* p = new NODE_TREE;
		p->data=x;
		if (!NhapDocGia(35, 12,p->data.MaThe, p->data.Ho, p->data.Ten, p->data.Phai, p->data.TrangThai))
		{
			return;
		}
		SoLuongDocGia++;

		p->data.SoLuongSachDangMuon = 0;
		p->data.SoNgayQuaHanMax = 0;
		p->data.TrangThai = 1;

		p->data.DoubleList_MuonTra.pHead = NULL;
		p->data.DoubleList_MuonTra.pTail = NULL;
		p->pLeft = NULL;
		p->pRight = NULL;
		t = p;
	}
	else
	{
		if (t->data.MaThe > x.MaThe)
		{
			ThemTheDocGia(t->pLeft, x);
		}
		else if (t->data.MaThe < x.MaThe)
		{
			ThemTheDocGia(t->pRight, x);
		}
	}
}
//===================== Sua doc gia ==============
void GiaoDienSuaDocGia(int toadoX, int toadoY)
{
	gotoXY(30,12);
	cout<<"SUA THONG TIN CUA DOC GIA";
	SetColor(15);
	gotoXY(15, 17);
	cout << "Ma The:";
	VeHCN(35, 19, 50, 2, 4);
	
	gotoXY(15, 20);
	cout << "Nhap Ho va Ten Lot:";
	VeHCN(35, 22, 50, 2, 4);
	gotoXY(15, 23);
	cout << "Nhap Ten:";
	gotoXY(15, 26);
	cout << "Chon Phai:";
	VeHCN(75, 25, 10, 2, 4);

	gotoXY(toadoX + 5, toadoY + 7);
	cout << "Nam";
	gotoXY(toadoX + 20, toadoY + 7);
	cout << "Nu";
	gotoXY(toadoX + 15, toadoY + 10);
	cout << "Cap nhat";
	gotoXY(toadoX + 30, toadoY + 10);
	cout << "Cancel";

}

void ChuyenTheDocGiaSangMangAll(TREE t, TheDocGia a[], int& i)
{
	if (t == NULL)
	{
		return;
	}
	else
	{
		ChuyenTheDocGiaSangMangAll(t->pLeft, a, i);
		a[i++] = t->data;
		ChuyenTheDocGiaSangMangAll(t->pRight, a, i);
	}
}

bool KiemTraMaDG(TREE &t, string &MaThe){
	TheDocGia a[MaxNodes];
	int n=0;
	ChuyenTheDocGiaSangMangAll(t,a,n);
	for(int i=0; i<n; i++){
		string m2=ChuyenSoSangString(a[i].MaThe);
		if(strstr(MaThe.c_str(),m2.c_str())!=NULL) 
			return true;
	}
	return false;
}

void LayThongTinDocGia(TREE t, int MaThe, TREE& tam)
{
	if (t == NULL)
	{
		return;
	}
	else if (t->data.MaThe == MaThe)
	{
		tam = t;
		return;
	}
	else if (MaThe > t->data.MaThe)
	{

		LayThongTinDocGia(t->pRight, MaThe, tam);
	}
	else
	{
		LayThongTinDocGia(t->pLeft, MaThe, tam);
	}
}

void XuLyNhap_MaDG(int& n, int toadoX, int& toadoY)
{
	int toadoXtam;
	string x;
	x = ChuyenSoSangString1(n);
	char c;
	while (true)
	{
		c = getch();
		if ((c >= 48 && c <= 57) && whereX() < toadoX + 47)
		{
			x.insert(x.begin() + (whereX() - toadoX), c);
			if ((whereX() - toadoX) < x.length() - 1)
			{
				toadoXtam = whereX() + 1;
				gotoXY(toadoX, whereY());
				cout << "                                               ";
				gotoXY(toadoX, whereY());
				cout << x;
				gotoXY(toadoXtam, whereY());
			}
			else
				cout << c;
		}
		else if (c == 8 && whereX() > toadoX)
		{
			x.erase(x.begin() + (whereX() - toadoX) - 1);
			toadoXtam = whereX() - 1;
			gotoXY(toadoX, whereY());
			cout << "                                               ";
			gotoXY(toadoX, whereY());
			cout << x;
			gotoXY(toadoXtam, whereY());
		}
		else if (c == 13 && x.length() > 0)
		{
			n = atoi(x.c_str());
			break;
		}
		else if (c == -32)
		{
			c = getch();
			if (c == 75 && whereX() > toadoX)
			{
				cout << "\b";
			}
			else if (c == 77 && whereX() < toadoX + x.length())
			{
				gotoXY(whereX() + 1, whereY());
			}
		}
	}
}

void NhapMaTheCanSua(int &MaThe)
{
	VeHCN(40,1,25,3,6);
	gotoXY(45,2);
	TextColor(215);
	cout<<"SUA DOC GIA";
	TextColor(10);
	VeHCN(36,6,20,2,5);
	gotoXY(10,7);
	cout<<"MOI BAN NHAP MA DOC GIA";
	gotoXY(38,7);
	int a=7;
	XuLyNhap_MaDG(MaThe,38,a);
}

string ChuyenTrangThaiDocGia(int &a){
	if(a==0){
		return "Bi khoa";
	}
	else if(a==1){
		return "Dang hoat dong";
	}
}
void SuaDocGia(TREE &t, int toadoX, int toadoY)
{
	int MaThe=0;
	int pointer1 = toadoY;
	TREE tam = NULL;
	NhapMaTheCanSua(MaThe);
	if(MaThe==0){
		gotoXY(30,15);
		cout<<"MA DOC GIA KHONG TON TAI";
		return;
	}
	string q = ChuyenSoSangString(MaThe);
	if(KiemTraMaDG(t,q) == false)
	{
		gotoXY(35,15);
		cout<<"MA DOC GIA KHONG TON TAI";
		gotoXY(30,35);
		system("pause");
		return;
	}
	LayThongTinDocGia(t, MaThe, tam);
	// luu lai cac gia tri
	string TamTen, TamPhai, TamHo;
	TamTen = tam->data.Ten;
	TamHo = tam->data.Ho;
	TamPhai = tam->data.Phai;
	GiaoDienSuaDocGia(toadoX,toadoY);
	gotoXY(toadoX + 15, toadoY + 10);
	cout << "Cap Nhat";
	gotoXY(25, pointer1-2);
	cout << tam->data.MaThe;
	gotoXY(toadoX + 2, pointer1 + 1);
	cout << tam->data.Ho;
	gotoXY(toadoX + 2, pointer1 + 4);
	cout << tam->data.Ten;
	gotoXY(77, pointer1+7);
	SetColor(4);
	cout <<tam->data.Phai;
	char c;
	while (true)
	{
		if (pointer1 == toadoY) // nhap ho
		{
			gotoXY(toadoX + 2 + tam->data.Ho.length(), pointer1 + 1);
			XuLyNhap_Chu(tam->data.Ho, toadoX + 2, pointer1, 30);
			if (pointer1 < toadoY)
			{
				pointer1 = toadoY + 9;
			}
		}
		else if (pointer1 == toadoY + 3) // nhap ten
		{
			gotoXY(toadoX + 2 + tam->data.Ten.length(), pointer1 + 1);
			XuLyNhap_Chu(tam->data.Ten, toadoX + 2, pointer1, 8);
		}
		else if (pointer1 == toadoY + 6) // gioi tinh
		{
			ChonPhai(toadoX + 5, toadoY + 7, tam->data.Phai, pointer1);
		}
		else if (pointer1 = toadoY + 9) // save hoac huy
		{
			int pointer2 = toadoX + 15;
			ShowCur(0);
			while (true)
			{
				TextColor(7);
				gotoXY(toadoX + 30, toadoY + 10);
				cout << "Cancel";
				gotoXY(toadoX + 15, toadoY + 10);
				cout << "Cap Nhat";

				if (kbhit())
				{
					c = getch();
					if (c == 13)
					{
						//Cap Nhat
						if (!tam->data.Ho.empty() && !tam->data.Ten.empty() && !tam->data.Phai.empty() && pointer2 == toadoX + 15)
						{
							ChuanHoaChuoi(tam->data.Ho);
							ChuanHoaChuoi(tam->data.Ten);
							gotoXY(35,30);
							cout<<"CAP NHAT THANH CONG";
							return;
						}
						//Cancel
						else if (pointer2 == toadoX + 30)
						{
							return;
						}
						// Khong Dien Du Thong Tin
						else
						{
							TextColor(7);
							gotoXY(60, 30);
							cout << "Vui Long Dien Day Du Thong Tin";
						}
					}
					else if (c == -32)
					{
						c = getch();
						if (c == 72)
						{
							pointer1 = pointer1 - 3;
							break;
						}
						else if (c == 80)
						{
							ShowCur(1);
							pointer1 = toadoY;
							break;
						}
						else if (c == 75)
						{
							if (pointer2 == toadoX + 30)
							{
								pointer2 = toadoX + 15;
							}
							else
								pointer2 = toadoX + 30;
						}
						else if (c == 77)
						{
							if (pointer2 == toadoX + 15)
							{
								pointer2 = toadoX + 30;
							}
							else
								pointer2 = toadoX + 15;
						}
						gotoXY(pointer2, toadoY + 10);
					}
				}

				SetColor(5);
				gotoXY(pointer2, toadoY + 10);
				if (pointer2 == toadoX + 15)
				{
					cout << "Cap Nhat";
				}
				else if (pointer2 == toadoX + 30)
				{
					cout << "Cancel";
				}
				TextColor(7);
				Sleep(100);
			}
		}
	}
}

// ======================= Xoa doc gia =====================

void XoaNode2Con(TREE &tam, TREE &t)
{
	if (t->pLeft != NULL)
	{
		XoaNode2Con(tam, t->pLeft);
	}
	else 
	{
		tam->data = t->data; 
		tam= t; 
		t = t->pRight; 
	}
}
void XuLyXoa(TREE &t,int x)
{
	if (x > t->data.MaThe)
	{
		XuLyXoa(t->pRight, x);
	}
	else if (x < t->data.MaThe)
	{
		XuLyXoa(t->pLeft, x);
	}
	else
	{
		TREE tam = t;
		if (t->pLeft == NULL)
		{
			t = t->pRight;
		}
		else if (t->pRight == NULL)
		{
			t = t->pLeft;
		}
		else if (t->pLeft != NULL && t->pRight != NULL)
		{
			XoaNode2Con(tam, t->pRight);
		}
		MangRandom[x] = false;
		delete tam;
	}
}


void GiaoDienNhapDGDeXoa(int &MaThe)
{
	VeHCN(60,1,21,3,6);
	gotoXY(65,2);
	TextColor(215);
	cout<<"XOA DOC GIA";
	TextColor(10);
	VeHCN(56,6,20,2,5);
	gotoXY(30,7);
	cout<<"MOI BAN NHAP MA DOC GIA";
	gotoXY(58,7);
	int a=7;
	XuLyNhap_MaDG(MaThe,58,a);
}
void XuatThongTin1DGDeXoa(TREE t)
{
	gotoXY(50,15);
	cout<<"Ma the:";
	gotoXY(50,17);
	cout<<"Ho&Ten:";
	gotoXY(50,19);
	cout<<"Phai:";
	gotoXY(50,21);
	cout<<"Trang thai:";
	gotoXY(65,15);
	cout<<t->data.MaThe;
	gotoXY(65,17);
	cout<<t->data.Ho+" "+t->data.Ten;
	gotoXY(65,19);
	cout<<t->data.Phai;
	gotoXY(65,21);
	cout<<ChuyenTrangThaiDocGia(t->data.TrangThai);
}

bool BanCoMuonXoaThe(){
	VeHCN(55,30,33,5,6);
	gotoXY(60,32);
	cout<<"BAN THUC SU MUON XOA THE?";
	int pointer = 60;
	char c;
	while(true)
	{
		gotoXY(60,34);
		cout<<"YES";
		gotoXY(66,34);
		cout<<"NO";
		while(kbhit()){
			c = getch();
			if(c == -32){
				c= getch();
				{
					if(c==75)
					{
						if(pointer == 60){
							pointer = 66;
						}else pointer = 60;
					}
					if(c==77)
					{
						if(pointer == 66){
							pointer = 60;
						}else pointer = 66;
					}
				}
			}
			else if(c == 13){
				switch(pointer)
				{
					case 60:
						{
							return true;
						}
					case 66:
						{
							return false;
						}
				}
			}
		}
		gotoXY(pointer, 34);
		SetColor(4);
		switch (pointer)
		{
		case 60:
			cout << "YES";
			break;
		case 66:
			cout << "NO";
			break;
		}
		TextColor(7);
		Sleep(100);	
	}
}
void XoaDocGia(TREE & t, MCONTRO & m, int tdX, int tdY)
{
	int MaDocGia = 0;
	GiaoDienNhapDGDeXoa(MaDocGia);
	if (MaDocGia == 0)
	{
		return;
	}
	string q = ChuyenSoSangString(MaDocGia);
	if(KiemTraMaDG(t,q) == false)
	{
		gotoXY(35,15);
		cout<<"MA DOC GIA KHONG TON TAI";
		gotoXY(30,35);
		system("pause");
		return;
	}
	TREE tam = NULL;
	LayThongTinDocGia(t, MaDocGia, tam);
	
	gotoXY(62, 12);
	cout << "Thong Tin Doc Gia";
	XuatThongTin1DGDeXoa(tam);	
	char c;
	int pointer = 26;
	while (true)
	{
		gotoXY(62, 26);
		cout << "Xoa The";
		gotoXY(80, 26);
		cout << "Tro Lai";
		if (kbhit())
		{
			c = getch();
			if (c == -32)
			{
				c = getch();
				if (c == 75)
				{
					if (pointer == 62)
					{
						pointer = 80;
					}
					else
						pointer = 62;
				}
				else if (c == 77)
				{
					if (pointer == 80)
					{
						pointer = 62;
					}
					else
						pointer = 80;
				}
			}
			else if (c == 13)
			{
				switch (pointer)
				{
				case 62:
					{
						if(BanCoMuonXoaThe()){
							XuLyXoa(t, tam->data.MaThe);
							SoLuongDocGia--;
							gotoXY(62,44);
							cout<<"XOA THANH CONG!";
							return;						
						}
						else
						{
							gotoXY(40,4);
							return;
						}
					}
				case 80:
					{
						gotoXY(40,4);
						return;
					}
				}
			}
			else if(c==27)
			{
				gotoXY(20,35);
				return;
			}
		}

		gotoXY(pointer,26);
		SetColor(4);
		switch (pointer)
		{
		case 62:
			cout << "Xoa The";
			break;
		case 80:
			cout << "Tro Lai";
			break;
		}
		TextColor(7);
		Sleep(100);
	}
}


//================================= XUAT DANH SACH DOC GIA ======================
string NoiHoTen(string ten, string ho)
{
	string tam;
	tam = ten;
	for (int i = 0; i < ho.length(); i++)
	{
		tam.push_back(ho[i]);
	}
	return tam;
}

void SapSepTheoTen(TREE t, TheDocGia a[], int& h)
{
	TheDocGia TheTam;
	ChuyenTheDocGiaSangMangAll(t, a, h);
	for (int i = 0; i < h - 1; i++)
	{
		for (int j = i + 1; j < h; j++)
		{
			if (NoiHoTen(a[i].Ten, a[i].Ho) > NoiHoTen(a[j].Ten, a[j].Ho))
			{
				TheTam = a[i];
				a[i] = a[j];
				a[j] = TheTam;
			}
		}
	}
}

string TrangThaiDocGia(int &n)
{
	if(n==0) return "The bi khoa";
	else return "The hoat dong";
}
void GiaoDienXuatDG()
{
	VeHCN(19,13,10,16,2);
	VeHCN(29,13,40,16,2);
	VeHCN(69,13,10,16,2);
	VeHCN(79,13,12,16,2);
}

void XuatDanhSachDocGiaBangMang(TheDocGia a[], int tam,int tam1, int tdX, int tdY)
{
	int j = 0;
	TextColor(7);
	for (int i = tam; i < tam1; i++)
	{
		gotoXY(tdX + 1, tdY + 4 + j);
		cout << ChuyenSoSangString(a[i].MaThe);
		gotoXY(tdX + 10 + 1, tdY + 4 + j);
		cout << a[i].Ho + " " + a[i].Ten;
		gotoXY(tdX + 50 + 1, tdY + 4 + j);
		cout << a[i].Phai;
		gotoXY(tdX + 60 + 1, tdY + 4 + j);
		cout << TrangThaiDocGia(a[i].TrangThai);
		j = j + 2;
	}
}

void XuatDanhSachDocGiaTheoPage(TheDocGia a[], int n, int tdX, int tdY)
{

	int page = 1;
	int tam = 0;
	int tam1 = 8;
	if (tam1 > n)
	{
		tam1 = n;
	}
	gotoXY(tdX + 1, tdY + 2 );
	cout << "Ma The";
	gotoXY(tdX + 10 + 1, tdY + 2 );
	cout << "Ho&Ten";
	gotoXY(tdX + 50 + 1, tdY + 2);
	cout << "Phai";
	gotoXY(tdX + 60 + 1, tdY + 2);
	cout << "Trang Thai";
	GiaoDienXuatDG();
	XuatDanhSachDocGiaBangMang(a, tam, tam1, tdX, tdY);
	XuatPage(page);
	char c;
	while (true)
	{
		if (kbhit())
		{
			c = getch();
			if (c == -32)
			{
				c = getch();
				if (c == 75)
				{
					if (tam != 0)
					{
						tam = tam - 8;
						if (tam < 0)
						{
							tam = 0;
						}
						tam1 = tam + 8;
						if (tam1 > n)
						{
							tam1 = n;
						}
						page--;
						XoaManHinh(tdX, tdY+4, 101, 20);
						GiaoDienXuatDG();
						XuatDanhSachDocGiaBangMang(a, tam, tam1, tdX, tdY);
						XuatPage(page);
					}
				}
				else if ( c== 77)
				{
					if (tam1 < n)
					{
						tam = tam + 8;
						tam1 = tam1 + 8;
						if (tam1 > n)
						{
							tam1 = n;
						}
						page++;
						XoaManHinh(tdX, tdY + 4, 101, 20);
						GiaoDienXuatDG();
						XuatDanhSachDocGiaBangMang(a, tam, tam1, tdX, tdY);
						XuatPage(page);
					}
				}
				
			}
			else if (c == 27)
			{
				return;
			}
		}
	}
}

string TachLayMaDauSach(string str)
{
	string str1;
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == '_')
		{
			break;
		}
		str1.push_back(str[i]);
	}
	return str1;
}

void XuatDanhSachDocGia(TREE t, int tdX, int tdY)
{
	int n = 0;
	TheDocGia* a = new TheDocGia[SoLuongDocGia];
	SapSepTheoTen(t, a, n);
	XuatDanhSachDocGiaTheoPage(a, n, tdX, tdY);
	delete[]a;
}

void MoVaKhoaTheDocGia(TREE &t, MCONTRO &m, int tdX,int tdY)
{
	int MaDocGia = 0;
	
	TREE tam;
	TextColor(7);
	VeHCN(tdX+18,tdY-6,30,2,5);
	gotoXY(tdX, tdY-5);
	cout<<"Nhap ma doc gia:";
	gotoXY(tdX+20,tdY-5);
	int q = tdY-5;
	XuLyNhap_MaDG(MaDocGia,tdX+20, q);
	if (MaDocGia == 0)
	{
		return;
	}
	LayThongTinDocGia(t, MaDocGia, tam);
	TextColor(5);
	gotoXY(50, 12);
	cout << "Thong Tin Doc Gia";
	TextColor(12);
	XuatThongTin1DGDeXoa(tam);
	VeHCN(50,32,30,6,4);
	TextColor(5);
	gotoXY(60,34);
	cout<<"THONG BAO";
	string MoOrKhoa[] = { "","Tro Lai" };
	if (tam->data.TrangThai == 1)
	{
		MoOrKhoa[0] = "Khoa";
	}
	else
		MoOrKhoa[0] = "Mo Khoa";
	char c;
	int pointer1 = 65;
	while (true)
	{
		gotoXY(65, tdY+14);
		cout << MoOrKhoa[0];
		gotoXY(80, tdY+14);
		cout << MoOrKhoa[1];

		if (kbhit())
		{
			c = getch();
			if (c == -32)
			{
				c = getch();
				if (c == 75)
				{
					if (pointer1 == 65)
					{
						pointer1 = 80;
					}
					else
					{
						pointer1 = 65;
					}
				}
				else if (c == 77)
				{
					if (pointer1 == 80)
					{
						pointer1 = 65;
					}
					else
					{
						pointer1 = 80;
					}
				}
			}
			else if(c==13)
			{
				if(pointer1 == 65)
				{
					if(tam->data.TrangThai == 0){
						for (NODE_DOUBLELIST* k = tam->data.DoubleList_MuonTra.pHead; k != NULL; k = k->pNext)
						{
							if (k->data.TrangThai == 2)
							{
								k->data.TrangThai = 1;
								for (int i = 0; i < m.SoLuongDauS; i++)
								{
									if (TachLayMaDauSach(k->data.MaSach) == m.nodesDauSach[i]->ISBN)
									{
										for (Node_Sach* h = m.nodesDauSach[i]->List_DMS.pHead; h != NULL; h = h->pNext)
										{
											if(k->data.MaSach == h->data.MaSach)
											{
												h->data.TrangThai = 3;
											}
										}
									}
								}
							}
						}
						tam->data.TrangThai = 1;
						gotoXY(54,36);
						cout<<"MO KHOA THANH CONG";
						gotoXY(10,42);
						return;
					}
					else
					{
						tam->data.TrangThai = 0;
						gotoXY(57,36);
						cout<<"KHOA THANH CONG";
						gotoXY(10,42);
						return;
					}

				}
				else if (pointer1 == 80)
				{
					gotoXY(60,36);
					cout<<"TRO LAI";
					gotoXY(10,42);
					return;
					
				}
			}
			else if (c == 27)
			{
				gotoXY(60,36);
				cout<<"TRO LAI";
				gotoXY(10,42);
				return;
			}
		}
		gotoXY(pointer1, tdY+14);
		TextColor(192);
		switch(pointer1)
		{
			case 65:
				{
					cout<<MoOrKhoa[0];
					break;
				}
			case 80:
				{
					cout<<MoOrKhoa[1];
					break;
				}
		}
		TextColor(7);
		Sleep(100);
	}
}


void SaveDanhSachDocGiavaMuonTra(TREE t, ofstream& FileOut, ofstream& FileOut1)
{
	if (t == NULL)
	{
		return;
	}
	else
	{
		FileOut << t->data.MaThe << ",";
		FileOut << t->data.Ho << " " << t->data.Ten << ",";
		FileOut << t->data.Phai << ",";
		FileOut << t->data.TrangThai<<","<<endl;
		for (NODE_DOUBLELIST* k = t->data.DoubleList_MuonTra.pHead; k != NULL; k = k->pNext)
		{
			FileOut1 << t->data.MaThe << "," << k->data.MaSach << ",";
			FileOut1 << k->data.NgayMuon.Ngay << "/" << k->data.NgayMuon.Thang << "/" << k->data.NgayMuon.Nam << ",";
			FileOut1 << k->data.NgayTra.Ngay << "/" << k->data.NgayTra.Thang << "/" << k->data.NgayTra.Nam << ",";
			FileOut1 << k->data.TrangThai << endl;
		}
		SaveDanhSachDocGiavaMuonTra(t->pLeft, FileOut, FileOut1);
		SaveDanhSachDocGiavaMuonTra(t->pRight, FileOut, FileOut1);
	}
}



void ThemTheDocGiaFile(TREE& t, int x, ifstream& FileIn)
{
	if (t == NULL)
	{
		NODE_TREE* p = new NODE_TREE;
		p->data.MaThe = x;
		//FileIn.seekg(1, ios::cur);
		FileIn.ignore();
		getline(FileIn, p->data.Ho, ' ');
		getline(FileIn, p->data.Ten, ',');
		getline(FileIn, p->data.Phai, ',');
		FileIn >> p->data.TrangThai;
		FileIn.ignore();
		p->pLeft = NULL;
		p->pRight = NULL;
		t = p;
	}
	else if (t->data.MaThe > x)
	{
		ThemTheDocGiaFile(t->pLeft, x, FileIn);
	}
	else
	{
		ThemTheDocGiaFile(t->pRight, x, FileIn);
	}
}


void LoadFileTheDocGia(TREE& t){
	int MaDocGia;
	ifstream FileIn;
	FileIn.open("ThemDocGia.txt", ios::in);
	string tam;
	while(true)
	{
		MaDocGia = 0;
		FileIn >> MaDocGia;
		if (MaDocGia == 0)
		{
			break;
		}
		MangRandom[MaDocGia] = true;
		SoLuongDocGia++ ;
		ThemTheDocGiaFile(t, MaDocGia, FileIn);
		if (FileIn.eof())
		{
			break;
		}
	}
	FileIn.close();
}


/////////////// DauSach //////////////////////////
bool KTMaTrung(MCONTRO DauS, string ISBN)
{
	for (int i = 0; i < DauS.SoLuongDauS ; i++)
	{
		if (DauS.nodesDauSach[i]->ISBN == ISBN)
		{
			return true;
		}
	}
	return false;
}
bool KTTenSach(string TenSach)
{
	for (int i = 0; i < TenSach.length(); i++)
	{
		if ((TenSach[i] >= 65 && TenSach[i] <= 90) || (TenSach[i] >= 97 && TenSach[i] <= 122))
		{
			return true;
		}
	}
	return false;
}



void GiaoDienNhap1DauSach(int toadoX, int toadoY)
{
	VeHCN(toadoX + 35, toadoY, 50, 2, 15);
	VeHCN(toadoX + 35, toadoY + 3, 50, 2, 15);
	VeHCN(toadoX + 35, toadoY + 6, 50, 2, 15);
	VeHCN(toadoX + 35, toadoY + 9, 50, 2, 15);
	VeHCN(toadoX + 35, toadoY + 12, 50, 2, 15);
	VeHCN(toadoX + 35, toadoY + 15, 50, 2, 15);
	VeHCN(toadoX + 35, toadoY + 18, 50, 2, 15);
	VeHCN(toadoX + 35, toadoY + 21, 50, 2, 15);
	gotoXY(toadoX, toadoY + 1);
	cout << "Nhap Ma Dau Sach:";
	gotoXY(toadoX, toadoY + 4);
	cout << "Nhap Ten Sach:";
	gotoXY(toadoX, toadoY + 7);
	cout << "Nhap So Trang:";
	gotoXY(toadoX, toadoY + 10);
	cout << "Nhap Tac Gia:";
	gotoXY(toadoX, toadoY + 13);
	cout << "Nhap Nam Xuat Ban:";
	gotoXY(toadoX, toadoY + 16);
	cout << "Nhap The Loai:";
	gotoXY(toadoX, toadoY + 19);
	cout << "Nhap Vi Tri Cua Dau Sach:";
	gotoXY(toadoX, toadoY + 22);
	cout << "Nhap So Luong Sach Cua Dau Sach:";
	gotoXY(toadoX + 30, toadoY + 25);
	cout << "Xac Nhan";
	gotoXY(toadoX + 60, toadoY + 25);
	cout << "Cancel";
}

void XuLyNhap_ISBN(string & InPut, int toadoX, int& toadoY)
{
	int toadoXtam;
	char c;
	while (true)
	{
		c = getch();
		if (((c >= 48 && c <= 57) || (c >= 65 && c <= 90)) && InPut.length() < 10)
		{
			InPut.insert(InPut.begin() + (whereX() - toadoX), c);
			if ((whereX() - toadoX) < InPut.length() - 1)
			{
				toadoXtam = whereX() + 1;
				gotoXY(toadoX, whereY());
				cout << "                                               ";
				gotoXY(toadoX, whereY());
				cout << InPut;
				gotoXY(toadoXtam, whereY());
			}
			else
				cout << c;
		}
		else if ((c >= 97 && c <= 122) && InPut.length() < 10)
		{
			c = c - 32;
			InPut.insert(InPut.begin() + (whereX() - toadoX), c);
			if ((whereX() - toadoX) < InPut.length() - 1)
			{
				toadoXtam = whereX() + 1;
				gotoXY(toadoX, whereY());
				cout << "                                               ";
				gotoXY(toadoX, whereY());
				cout << InPut;
				gotoXY(toadoXtam, whereY());
			}
			else
				cout << c;
		}
		else if (c == 8 && whereX() > toadoX)
		{
			InPut.erase(InPut.begin() + (whereX() - toadoX) - 1);
			toadoXtam = whereX() - 1;
			gotoXY(toadoX, whereY());
			cout << "                                               ";
			gotoXY(toadoX, whereY());
			cout << InPut;
			gotoXY(toadoXtam, whereY());
		}
		else if (c == 13 && InPut.length() > 0)
		{
			toadoY = toadoY + 3;
			break;
		}
		else if (c == -32)
		{
			c = getch();
			if (c == 75 && whereX() > toadoX)
			{
				cout << "\b";
			}
			else if (c == 77 && whereX() < toadoX + InPut.length())
			{
				gotoXY(whereX() + 1, whereY());
			}
			else if (c == 80)
			{
				toadoY = toadoY + 3;
				break;
			}
			else if (c == 72)
			{
				toadoY = toadoY - 3;
				break;
			}
		}
	}
}

void XuLyNhap_TenSach(string & InPut)
{
	while (true)
	{
		char c = getch();
		if ((c == '+' || c == '-' || c == '*' || c == '/' ||
			(c >= 65 && c <= 90) || (c >= 97 && c <= 122) || (c >= 48 && c <= 57)) && InPut.length() < 40)
		{
			InPut.insert(InPut.begin() + InPut.length(), c);
			cout << c;
		}
		else if (c == 8 && InPut.length() > 0)
		{
			InPut.erase(InPut.begin() + InPut.length() - 1);
			cout << "\b";
			cout << " ";
			cout << "\b";
		}
		else if (c == 27)
		{
			InPut.clear();
			return;
		}
		else if (c == 13 && InPut.length() > 0)
		{
			break;
		}
	}
}


void XuLyNhap_TenSach1(string & InPut, int toadoX, int& toadoY, int LenghtMax)
{
	int toadoXtam;
	char c;
	while (true)
	{
		c = getch();
		if (((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || (c >= 48 && c <= 57)||
			c == '+' || c == '-' || c == '*' || c == '/'||c=='.'||c=='_') && InPut.length() < LenghtMax)
		{
			InPut.insert(InPut.begin() + (whereX() - toadoX), c);
			if ((whereX() - toadoX) < InPut.length() - 1)
			{
				toadoXtam = whereX() + 1;
				gotoXY(toadoX, whereY());
				cout << "                                               ";
				gotoXY(toadoX, whereY());
				cout << InPut;
				gotoXY(toadoXtam, whereY());
			}
			else
				cout << c;
		}
		else if (c == 8 && whereX() > toadoX)
		{
			InPut.erase(InPut.begin() + (whereX() - toadoX) - 1);
			toadoXtam = whereX() - 1;
			gotoXY(toadoX, whereY());
			cout << "                                               ";
			gotoXY(toadoX, whereY());
			cout << InPut;
			gotoXY(toadoXtam, whereY());
		}
		else if (c == 32 && InPut.length() < LenghtMax && InPut.length() != 0 && InPut[whereX() - toadoX - 1] != ' ')
		{
			InPut.insert(InPut.begin() + (whereX() - toadoX), ' ');
			if ((whereX() - toadoX) < InPut.length() - 1)
			{
				toadoXtam = whereX() + 1;
				gotoXY(toadoX, whereY());
				cout << "                                               ";
				gotoXY(toadoX, whereY());
				cout << InPut;
				gotoXY(toadoXtam, whereY());
			}
			else
				cout << c;
		}
		else if (c == 13 && InPut.length() > 0)// nếu là phím enter
		{
			toadoY = toadoY + 3;
			break;
		}
		else if (c == -32)
		{
			c = getch();
			if (c == 75 && whereX() > toadoX)
			{
				cout << "\b";
			}
			else if (c == 77 && whereX() < toadoX + InPut.length())
			{
				gotoXY(whereX() + 1, whereY());
			}
			else if (c == 80)
			{
				toadoY = toadoY + 3;
				break;
			}
			else if (c == 72)
			{
				toadoY = toadoY - 3;
				break;
			}
		}
	}
}

int DemChuSo(int n)
{
	int dem = 0;
	while (n > 0)
	{
		n = n / 10;
		dem++;
	}
	return dem;
}

void XuLyNhap_ViTri(string& InPut, int toadoX, int& toadoY, int LenghtMax)
{
	int toadoXtam;
	char c;
	while (true)
	{
		c = getch();
		if (((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || (c >= 48 && c <= 57)) && InPut.length() < LenghtMax)
		{
			InPut.insert(InPut.begin() + (whereX() - toadoX), c);
			if ((whereX() - toadoX) < InPut.length() - 1)
			{
				toadoXtam = whereX() + 1;
				gotoXY(toadoX, whereY());
				cout << "                                               ";
				gotoXY(toadoX, whereY());
				cout << InPut;
				gotoXY(toadoXtam, whereY());
			}
			else
				cout << c;
		}
		else if (c == 8 && whereX() > toadoX)
		{
			InPut.erase(InPut.begin() + (whereX() - toadoX) - 1);
			toadoXtam = whereX() - 1;
			gotoXY(toadoX, whereY());
			cout << "                                               ";
			gotoXY(toadoX, whereY());
			cout << InPut;
			gotoXY(toadoXtam, whereY());
		}
		else if (c == 32 && InPut.length() < LenghtMax && InPut.length() != 0 && InPut[whereX() - toadoX - 1] != ' ')
		{
			InPut.insert(InPut.begin() + (whereX() - toadoX), ' ');
			if ((whereX() - toadoX) < InPut.length() - 1)
			{
				toadoXtam = whereX() + 1;
				gotoXY(toadoX, whereY());
				cout << "                                               ";
				gotoXY(toadoX, whereY());
				cout << InPut;
				gotoXY(toadoXtam, whereY());
			}
			else
				cout << c;
		}
		else if (c == 13 && InPut.length() > 0)
		{
			toadoY = toadoY + 3;
			break;
		}
		else if (c == -32)
		{
			c = getch();
			if (c == 75 && whereX() > toadoX)
			{
				cout << "\b";
			}
			else if (c == 77 && whereX() < toadoX + InPut.length())
			{
				gotoXY(whereX() + 1, whereY());
			}
			else if (c == 80)
			{
				toadoY = toadoY + 3;
				break;
			}
			else if (c == 72)
			{
				toadoY = toadoY - 3;
				break;
			}
		}
	}
}

void ThemSachTuDong(MCONTRO &DauS, string MaSach, string ViTri)
{
	Node_Sach* p = new Node_Sach;
	if (p == NULL)
	{
		cout << "\nCap phat that bai !";
		return;
	}
	p->data.MaSach = MaSach;
	p->data.TrangThai = 0;
	p->data.vitri = ViTri;
	p->pNext = NULL;
	if (DauS.nodesDauSach[DauS.SoLuongDauS]->List_DMS.pHead == NULL)
	{
		DauS.nodesDauSach[DauS.SoLuongDauS]->List_DMS.pHead = DauS.nodesDauSach[DauS.SoLuongDauS]->List_DMS.pTail = p;
	}
	else
	{
		DauS.nodesDauSach[DauS.SoLuongDauS]->List_DMS.pTail->pNext = p;
		DauS.nodesDauSach[DauS.SoLuongDauS]->List_DMS.pTail = p;
	}
}


bool NhapDauSach(MCONTRO & DauS, string & ViTri, int toadoX, int toadoY)
{

	GiaoDienNhap1DauSach(toadoX, toadoY);

	DauS.nodesDauSach[DauS.SoLuongDauS]->NamXuatBan = 2020;
	DauS.nodesDauSach[DauS.SoLuongDauS]->SoTrang = 0;
	DauS.nodesDauSach[DauS.SoLuongDauS]->SoLuongSach = 0;
	DauS.nodesDauSach[DauS.SoLuongDauS]->SoLanMuon = 0;
	gotoXY(toadoX + 37, toadoY+13);
	cout << DauS.nodesDauSach[DauS.SoLuongDauS]->NamXuatBan;
	gotoXY(toadoX + 37, toadoY + 7);
	cout << DauS.nodesDauSach[DauS.SoLuongDauS]->SoTrang;
	gotoXY(toadoX + 37, toadoY + 22);
	cout << DauS.nodesDauSach[DauS.SoLuongDauS]->SoLuongSach;

	char c;
	int pointer1 = toadoY;
	int pointer2 = toadoX + 30;
	ShowCur(1);
	while (true)
	{
		if (pointer1 == toadoY) // ma dau sach
		{
			gotoXY(toadoX + 37 + DauS.nodesDauSach[DauS.SoLuongDauS]->ISBN.length(), pointer1 + 1);
			XuLyNhap_ISBN(DauS.nodesDauSach[DauS.SoLuongDauS]->ISBN, toadoX + 37, pointer1);
			
			if (pointer1 < toadoY)
			{
				pointer1 = toadoY + 24;
			}
			
			if (DauS.nodesDauSach[DauS.SoLuongDauS]->ISBN.empty())
			{
				gotoXY(toadoX + 87, toadoY + 1);
				cout << "Ma Dau Sach Rong";
			}
			else if (KTMaTrung(DauS, DauS.nodesDauSach[DauS.SoLuongDauS]->ISBN))
			{
				gotoXY(toadoX + 87, toadoY + 1);
				cout << "Ma Dau Sach Trung";
			}
			else
			{
				gotoXY(toadoX + 87, toadoY + 1);
				cout << "                 ";
			}
		}
		else if (pointer1 == toadoY + 3) // ten sach
		{
			gotoXY(toadoX + 37 + DauS.nodesDauSach[DauS.SoLuongDauS]->TenSach.length(), pointer1 + 1);
			XuLyNhap_TenSach1(DauS.nodesDauSach[DauS.SoLuongDauS]->TenSach, toadoX + 37, pointer1,40);
			if (DauS.nodesDauSach[DauS.SoLuongDauS]->TenSach.empty())
			{
				gotoXY(toadoX + 87, toadoY + 4);
				cout << "Ten Sach Rong";
			}
			else if (!KTTenSach(DauS.nodesDauSach[DauS.SoLuongDauS]->TenSach))
			{
				gotoXY(toadoX + 87, toadoY + 4);
				cout << "Ten Khong Hop Le";
			}
			else
			{
				gotoXY(toadoX + 87, toadoY + 4);
				cout << "                 ";
			}
		}
		else if (pointer1 == toadoY + 6) // so trang
		{
			gotoXY(toadoX + 37 + DemChuSo(DauS.nodesDauSach[DauS.SoLuongDauS]->SoTrang), pointer1 + 1);
			XuLyNhap_So(DauS.nodesDauSach[DauS.SoLuongDauS]->SoTrang, toadoX + 37, pointer1);
			if (DauS.nodesDauSach[DauS.SoLuongDauS]->SoTrang <= 0)
			{
				gotoXY(toadoX + 87, toadoY + 7);
				cout << "So Trang Phai >0";
			}
			else if (DauS.nodesDauSach[DauS.SoLuongDauS]->SoTrang > 10000)
			{
				gotoXY(toadoX + 87, toadoY + 7);
				cout << "So Trang <10000";
			}
			else
			{
				gotoXY(toadoX + 87, toadoY + 7);
				cout << "                  ";
			}
		}
		else if (pointer1 == toadoY + 9)// tac gia
		{
			gotoXY(toadoX + 37 + DauS.nodesDauSach[DauS.SoLuongDauS]->TacGia.length(), pointer1 + 1);
			XuLyNhap_Chu(DauS.nodesDauSach[DauS.SoLuongDauS]->TacGia, toadoX + 37, pointer1,25);
			if (DauS.nodesDauSach[DauS.SoLuongDauS]->TacGia.empty())
			{
				gotoXY(toadoX + 87, toadoY + 10);
				cout << "Tac Gia Rong";
			}
			else
			{
				gotoXY(toadoX + 87, toadoY + 10);
				cout << "            ";
			}
		}
		else if (pointer1 == toadoY + 12) // nam
		{
			gotoXY(toadoX + 37 + DemChuSo(DauS.nodesDauSach[DauS.SoLuongDauS]->NamXuatBan), pointer1 + 1);
			XuLyNhap_So(DauS.nodesDauSach[DauS.SoLuongDauS]->NamXuatBan, toadoX + 37, pointer1);
			if (DauS.nodesDauSach[DauS.SoLuongDauS]->NamXuatBan > 2020 || DauS.nodesDauSach[DauS.SoLuongDauS]->NamXuatBan < 1000)
			{
				gotoXY(toadoX + 87, toadoY + 13);
				cout << "Nam K Hop Le";
			}
			else
			{
				gotoXY(toadoX + 87, toadoY + 13);
				cout << "             ";
			}
		}
		else if (pointer1 == toadoY + 15) // the loai
		{
			gotoXY(toadoX + 37 + DauS.nodesDauSach[DauS.SoLuongDauS]->TheLoai.length(), pointer1 + 1);
			XuLyNhap_Chu(DauS.nodesDauSach[DauS.SoLuongDauS]->TheLoai, toadoX + 37, pointer1,20);
			if (DauS.nodesDauSach[DauS.SoLuongDauS]->TheLoai.empty())
			{
				gotoXY(toadoX + 87, toadoY + 16);
				cout << "The Loai Rong";
			}
			else
			{
				gotoXY(toadoX + 87, toadoY + 16);
				cout << "              ";
			}
		}
		else if (pointer1 == toadoY + 18) // vi tri
		{
			gotoXY(toadoX + 37 + ViTri.length(), pointer1 + 1);
			XuLyNhap_ViTri(ViTri, toadoX + 37, pointer1,20);
			if (ViTri.empty())
			{
				gotoXY(toadoX + 87, toadoY + 19);
				cout << "Vi Tri Rong";
			}
			else
			{
				gotoXY(toadoX + 87, toadoY + 19);
				cout << "           ";
			}
		}
		else if (pointer1 == toadoY + 21) //so luong
		{
			gotoXY(toadoX + 37 + DemChuSo(DauS.nodesDauSach[DauS.SoLuongDauS]->SoLuongSach), pointer1 + 1);
			XuLyNhap_So(DauS.nodesDauSach[DauS.SoLuongDauS]->SoLuongSach, toadoX + 37, pointer1);
			if (DauS.nodesDauSach[DauS.SoLuongDauS]->SoLuongSach <= 0)
			{
				gotoXY(toadoX + 87, toadoY + 22);
				cout << "So Luong >0";
			}
			else if (DauS.nodesDauSach[DauS.SoLuongDauS]->SoLuongSach > 9999)
			{
				gotoXY(toadoX + 87, toadoY + 22);
				cout << "So Luong <10000";
			}
			else
			{
				gotoXY(toadoX + 87, toadoY + 22);
				cout << "                  ";
			}
		}
		else if (pointer1 == toadoY + 24)
		{
			ShowCur(0);
			SetColor(5);
			gotoXY(toadoX + 30, toadoY + 25);
			cout << "Xac Nhan";
			while (true)
			{
				if (kbhit())
				{
					c = getch();
					if (c == 13)
					{
						if (!DauS.nodesDauSach[DauS.SoLuongDauS]->ISBN.empty() && 
							!KTMaTrung(DauS, DauS.nodesDauSach[DauS.SoLuongDauS]->ISBN) && 
							!DauS.nodesDauSach[DauS.SoLuongDauS]->TheLoai.empty() && 
							!DauS.nodesDauSach[DauS.SoLuongDauS]->TacGia.empty() && 
							KTTenSach(DauS.nodesDauSach[DauS.SoLuongDauS]->TenSach)&&
							!DauS.nodesDauSach[DauS.SoLuongDauS]->TenSach.empty() && 
							(999< DauS.nodesDauSach[DauS.SoLuongDauS]->NamXuatBan&&DauS.nodesDauSach[DauS.SoLuongDauS]->NamXuatBan < 2021) &&
							(10000 > DauS.nodesDauSach[DauS.SoLuongDauS]->SoLuongSach && DauS.nodesDauSach[DauS.SoLuongDauS]->SoLuongSach > 0) &&
							(10000 > DauS.nodesDauSach[DauS.SoLuongDauS]->SoTrang && DauS.nodesDauSach[DauS.SoLuongDauS]->SoTrang) > 0 &&
							!ViTri.empty() && 
							pointer2 == toadoX + 30)
						{
							ChuanHoaChuoi(DauS.nodesDauSach[DauS.SoLuongDauS]->TheLoai);
							ChuanHoaChuoi(DauS.nodesDauSach[DauS.SoLuongDauS]->TacGia);
							ChuanHoaChuoi(DauS.nodesDauSach[DauS.SoLuongDauS]->TenSach);
							ChuanHoaChuoi(ViTri);
							ShowCur(0);
							return true;
						}
						else if (pointer2 == toadoX + 60)
						{
							ShowCur(0);
							return false;
						}
					}
					else if (c == -32)
					{
						c = getch();
						if (c == 72)
						{
							TextColor(7);
							gotoXY(toadoX + 60, toadoY + 25);
							cout << "Cancel";
							gotoXY(toadoX + 30, toadoY + 25);
							cout << "Xac Nhan";
							ShowCur(1);
							pointer1 = pointer1 - 3;
							break;
						}
						else if (c == 80)
						{
							TextColor(7);
							gotoXY(toadoX + 60, toadoY + 25);
							cout << "Cancel";
							gotoXY(toadoX + 30, toadoY + 25);
							cout << "Xac Nhan";
							ShowCur(1);
							pointer1 = toadoY;
							break;
						}
						else if (c == 75)
						{
							if (pointer2 == toadoX + 60)
							{
								pointer2 = toadoX + 30;
							}
							else
								pointer2 = toadoX + 60;
						}
						else if (c == 77)
						{
							if (pointer2 == toadoX + 30)
							{
								pointer2 = toadoX + 60;
							}
							else
								pointer2 = toadoX + 30;
						}
					}
				}

				if (pointer2 == toadoX + 30)
				{
					TextColor(7);
					gotoXY(toadoX + 60, toadoY + 25);
					cout << "Cancel";
					SetColor(5);
					gotoXY(toadoX + 30, toadoY + 25);
					cout << "Xac Nhan";
				}
				else if (pointer2 == toadoX + 60)
				{
					TextColor(7);
					gotoXY(toadoX + 30, toadoY + 25);
					cout << "Xac Nhan";
					SetColor(5);
					gotoXY(toadoX + 60, toadoY + 25);
					cout << "Cancel";
				}

				
			}
		}
		Sleep(100);
	}
}

void ThemMotDauSach(MCONTRO & DauS, int toadoX, int toadoY)
{
	if (DauS.SoLuongDauS == 1000)
	{
		TextColor(240);
		gotoXY(117, 23);
		cout << "LOI!!!";
		gotoXY(115, 25);
		cout << "BO NHO DAY";
		gotoXY(110, 26);
		system("pause");
		TextColor(7);
		return;
	}
	string vitri;
	DauS.nodesDauSach[DauS.SoLuongDauS] = new DauSach;
	if (DauS.nodesDauSach[DauS.SoLuongDauS] == NULL)
	{
		cout << "\a";
		return;
	}
	if (!NhapDauSach(DauS, vitri, toadoX, toadoY))
	{
		delete DauS.nodesDauSach[DauS.SoLuongDauS];
		return;
	}
	DauS.nodesDauSach[DauS.SoLuongDauS]->List_DMS.pHead = NULL;
	DauS.nodesDauSach[DauS.SoLuongDauS]->List_DMS.pTail = NULL;
	for (int i = 1; i <= DauS.nodesDauSach[DauS.SoLuongDauS]->SoLuongSach; i++)
	{
		ThemSachTuDong(DauS, NoiChuoi(DauS.nodesDauSach[DauS.SoLuongDauS]->ISBN, i), vitri);
	}
	DauS.SoLuongDauS++;
}


void SaveDanhSachDauSach(MCONTRO m)
{
	ofstream FileOut;
	FileOut.open("ThemDauSach.txt", ios::out);
	FileOut << m.SoLuongDauS << endl;
	if(m.SoLuongDauS==0)
	{
		return ;
	}
	else
	{
		for (int i = 0; i < m.SoLuongDauS; i++)
		{
			FileOut << m.nodesDauSach[i]->ISBN << ",";
			FileOut << m.nodesDauSach[i]->TenSach << ",";
			FileOut << m.nodesDauSach[i]->SoTrang << ",";
			FileOut << m.nodesDauSach[i]->TacGia << ",";
			FileOut << m.nodesDauSach[i]->NamXuatBan << ",";
			FileOut << m.nodesDauSach[i]->TheLoai << ",";
			FileOut << m.nodesDauSach[i]->SoLuongSach << ",";
			FileOut << m.nodesDauSach[i]->List_DMS.pHead->data.vitri <<","<< endl;
		}
	}
	FileOut.close();
}

void LoadFileDauSach(MCONTRO& DauS)
{
	int n;
	string vitri;
	ifstream FileIn;
	FileIn.open("ThemDauSach.txt", ios::in);
	FileIn >> n;
	FileIn.ignore();
	for (int i = 0; i < n; i++)
	{
		DauS.nodesDauSach[DauS.SoLuongDauS] = new DauSach;
		getline(FileIn, DauS.nodesDauSach[DauS.SoLuongDauS]->ISBN, ',');
		getline(FileIn, DauS.nodesDauSach[DauS.SoLuongDauS]->TenSach, ',');
		FileIn >> DauS.nodesDauSach[DauS.SoLuongDauS]->SoTrang;
		//FileIn.seekg(1, ios::cur);
		FileIn.ignore();
		getline(FileIn, DauS.nodesDauSach[DauS.SoLuongDauS]->TacGia, ',');
		FileIn >> DauS.nodesDauSach[DauS.SoLuongDauS]->NamXuatBan;
		//FileIn.seekg(1, ios::cur);
		FileIn.ignore();
		getline(FileIn, DauS.nodesDauSach[DauS.SoLuongDauS]->TheLoai, ',');
		FileIn >> DauS.nodesDauSach[DauS.SoLuongDauS]->SoLuongSach;
		//FileIn.seekg(1, ios::cur);
		FileIn.ignore();
		getline(FileIn, vitri);
		DauS.nodesDauSach[i]->SoLanMuon = 0;
		DauS.nodesDauSach[DauS.SoLuongDauS]->List_DMS.pHead = NULL;
		DauS.nodesDauSach[DauS.SoLuongDauS]->List_DMS.pTail = NULL;
		for (int i = 1; i <= DauS.nodesDauSach[DauS.SoLuongDauS]->SoLuongSach; i++)
		{
			ThemSachTuDong(DauS, NoiChuoi(DauS.nodesDauSach[DauS.SoLuongDauS]->ISBN, i), vitri);
		}
		DauS.SoLuongDauS++;
	}
	FileIn.close();
}

//===============Xuat DSDS ===============




bool KiemTraTrungTheLoai(MCONTRO DauS,string theloai[],int SoLuong[],int h,int i)
{
	for (int j = 0; j < h; j++)
	{
		if (DauS.nodesDauSach[i]->TheLoai == theloai[j])
		{
			SoLuong[j]++;
			return false;
		}
	}
	return true;
}
void XuatCacDauSachCua1TheLoai(MCONTRO DauS,string TheLoai,int h,int tdX,int tdY)
{
	gotoXY(tdX + 1, tdY + 2);
	cout << "ISBN";
	gotoXY(tdX + 12, tdY + 2);
	cout << "Ten Sach";
	gotoXY(tdX + 53, tdY + 2);
	cout << "So Trang";
	gotoXY(tdX + 66, tdY + 2);
	cout << "Tac Gia";
	gotoXY(tdX + 86, tdY + 2);
	cout << "Nam XB";
	gotoXY(tdX + 95, tdY + 2);
	cout << "Vi Tri";
	gotoXY(tdX + 116, tdY + 2);
	cout << "So Luong Sach";
	int dem = 0;
	for (int i = 0; i < DauS.SoLuongDauS; i++)
	{
		if (DauS.nodesDauSach[i]->TheLoai == TheLoai)
		{
			gotoXY(tdX + 1, tdY + 4 + dem * 2);
			cout << DauS.nodesDauSach[i]->ISBN;
			gotoXY(tdX + 12, tdY + 4 + dem * 2);
			cout << DauS.nodesDauSach[i]->TenSach;
			gotoXY(tdX + 53, tdY + 4 + dem * 2);
			cout << ChuyenSoSangString(DauS.nodesDauSach[i]->SoTrang);
			gotoXY(tdX + 65, tdY + 4 + dem * 2);
			cout << DauS.nodesDauSach[i]->TacGia;
			gotoXY(tdX + 86, tdY + 4 + dem * 2);
			cout << ChuyenSoSangString(DauS.nodesDauSach[i]->NamXuatBan);
			gotoXY(tdX + 95, tdY + 4 + dem * 2);
			cout << DauS.nodesDauSach[i]->List_DMS.pHead->data.vitri;
			gotoXY(tdX + 116, tdY + 4 + dem * 2);
			cout << ChuyenSoSangString(DauS.nodesDauSach[i]->SoLuongSach);
			dem++;
		}
	}
	TextColor(7);
	gotoXY(100, 35);
	system("pause");
}

void XuatDanhSachTheLoai(string a[], int tam,int tam1, int tdX, int tdY)
{
	int j = 0;
	TextColor(7);
	for (int i = tam; i < tam1; i++)
	{
		gotoXY(tdX + 45, j + 5);
		cout << a[i];
		j++;
	}
}

void SapXepDauSachTheoTenSach(MCONTRO &DauS)
{
	DauSach* tam = NULL;	
	for (int i = 0; i < DauS.SoLuongDauS - 1; i++)
	{
		for (int j = i + 1; j < DauS.SoLuongDauS; j++)
		{
			if (DauS.nodesDauSach[i]->TenSach > DauS.nodesDauSach[j]->TenSach)
			{
				tam = DauS.nodesDauSach[i];
				DauS.nodesDauSach[i] = DauS.nodesDauSach[j];
				DauS.nodesDauSach[j] = tam;
			}
		}
	}
}

int XuatDanhSachTheLoaiPage(string a[], int n, int tdX, int tdY)
{
	int page = 1;
	int tam = 0;
	int tam1 = 4;
	int m=0;
	int pointer = 5;
	int q;
	XuatDanhSachTheLoai(a,tam,tam1,tdX,tdY);
	char c;
	while (true)
	{
		gotoXY(tdX+38,pointer);
		cout<<"->";	
		q=tam+m;
		if(q>=n)
		{
			q=n-1;	
		}	
		gotoXY(10,2);
		cout<<q+1;
		if (kbhit())
		{
			c = getch();
			if (c == -32)
			{
				c = getch();
				if (c == 72)
				{
					if(pointer == 5)
					{
						gotoXY(tdX+38,pointer);
						cout<<"    ";
						pointer = 8;						
						m=3;
						gotoXY(tdX+38,pointer);
						cout<<"->";
					}
					else 
					{
						gotoXY(tdX+38,pointer);
						cout<<"    ";
						m--;
						pointer = pointer-1;
						gotoXY(tdX+38,pointer);
						cout<<"->";
					}
				}
				else if ( c== 80)
				{
					if(pointer == 8)
					{
						gotoXY(tdX+38,pointer);
						cout<<"    ";
						pointer = 5;						
						m=0;
						gotoXY(tdX+38,pointer);
						cout<<"->";
					}
					else 
					{
						gotoXY(tdX+38,pointer);
						cout<<"    ";
						m++;
						pointer = pointer+1;
						gotoXY(tdX+38,pointer);
						cout<<"->";
					}
				}
				if (c == 75)
				{
					if (tam != 0)
					{
						tam = tam - 4;
						if (tam < 0)
						{
							tam = 0;
						}
						tam1 = tam + 4;
						if (tam1 > n)
						{
							tam1 = n;
						}
						page--;
						XoaManHinh(tdX+45, 5, 20, 4);
						XuatDanhSachTheLoai(a,tam,tam1,tdX,tdY);
					}
				}
				else if ( c== 77)
				{
					if (tam1 < n)
					{
						tam = tam + 4;
						tam1 = tam1 + 4;
						if (tam1 > n)
						{
							tam1 = n;
						}
						page++;
						XoaManHinh(tdX+45, 5, 20, 4);
						XuatDanhSachTheLoai(a,tam,tam1,tdX,tdY);		
					}
				}
			}
			else if(c==13)
			{
				return q;
			}
			else if(c==27){
				return 0;
			}
		}
		Sleep(100);
	}
}


void XuatTheoTheLoai(MCONTRO &m ,int tdX, int tdY){
	if(m.SoLuongDauS == 0){
		VeHCN(55,13,47,4,15);
		gotoXY(60,15);
		cout<<"THU VIEN KHONG CO QUYEN SACH NAO CA!!!";
		gotoXY(100,50);
		return;
	}
	SapXepDauSachTheoTenSach(m);
	string TheLoai[100];
	int SoLuong[100];
	TheLoai[0] = m.nodesDauSach[0]->TheLoai;
	int h = 1;// so luong pt mang the loai
	for (int i = 1; i < m.SoLuongDauS; i++)
	{
		if (KiemTraTrungTheLoai(m,TheLoai,SoLuong,h,i))
		{
			TheLoai[h++] = m.nodesDauSach[i]->TheLoai;
		}
	}
	gotoXY(tdX + 40, 3);
	cout << "DANH SACH TAT CA CAC THE LOAI";
	int x = XuatDanhSachTheLoaiPage(TheLoai,h,tdX+40,tdY);
	XuatCacDauSachCua1TheLoai(m,TheLoai[x],h,tdX,12);
	
}


// =========== cau e ==================


void XuatMotDauSach(DauSach & DS, int toadoX, int toadoY)
{
	gotoXY(toadoX + 1, toadoY + 4);
	cout << DS.ISBN;
	gotoXY(toadoX + 12, toadoY + 4);
	cout <<DS.TenSach;
	gotoXY(toadoX + 53, toadoY + 4);
	cout << ChuyenSoSangString(DS.SoTrang);
	gotoXY(toadoX + 60, toadoY + 4);
	cout <<DS.TacGia;
	gotoXY(toadoX + 86, toadoY + 4);
	cout << ChuyenSoSangString(DS.NamXuatBan);
	gotoXY(toadoX + 95, toadoY + 4);
	cout << DS.TheLoai;
	gotoXY(toadoX + 116, toadoY + 4);
	cout << ChuyenSoSangString(DS.SoLuongSach);
}
void ChuyenDanhMucSachSangMang(DauSach DauS,DanhMucSach Dms[],int &i)
{
	for (Node_Sach* k = DauS.List_DMS.pHead; k != NULL; k = k->pNext)
	{
		Dms[i++] = k->data;
	}
}
string TrangThaiSach(int &n)
{
	if(n==0)
	{
		return "Sach chua ai muon";
	}
	else if(n==1)
	{
		return "Sach da duoc muon";
	}
	else if(n==2)
	{
		return "Sach bi mat chua den";
	}
	else if(n==3)
	{
		return " Sach bi mat da den";
	}
}

void GiaoDienDMS(int tdX, int tdY)
{
	VeHCN(tdX,tdY+3,25,12,2);
	VeHCN(tdX+25,tdY+3,25,12,2);	
	VeHCN(tdX+50,tdY+3,25,12,2);
}

void XuatDanhSachDanhMucSachBangMang(DanhMucSach Dms[], int tdX, int tdY, int tam,int tam1)
{
	TextColor(9);
	gotoXY(tdX + 8, tdY + 2);
	cout << "Ma Sach";
	gotoXY(tdX + 32, tdY + 2);
	cout << "Trang thai Sach";
	gotoXY(tdX + 57, tdY + 2);
	cout << "Vi Tri";
	gotoXY(tdX + 82, tdY + 2);
	TextColor(7);
	int j = 0;
	for (int i = tam; i < tam1 ; i++)
	{
		gotoXY(tdX + 1, tdY + j + 4);
		cout << Dms[i].MaSach;
		gotoXY(tdX + 26, tdY + j + 4);
		cout << TrangThaiSach(Dms[i].TrangThai);
		gotoXY(tdX + 51, tdY + j + 4);
		cout << Dms[i].vitri;
		gotoXY(tdX + 76, tdY + j + 4);
		j = j + 2;
	}
	TextColor(7);
}




void XuatDanhSachDanhMucSachTheoPage(DanhMucSach Dms[],int tdX,int tdY,int n)
{
	int page = 1;
	int tam = 0;
	int tam1 = 6;
	if (tam1 > n)
	{
		tam1 = n;
	}
	GiaoDienDMS(tdX+20, tdY);
	XuatDanhSachDanhMucSachBangMang(Dms, tdX + 20, tdY, tam, tam1);

	gotoXY(111, 35);
	cout << char(174);
	gotoXY(115, 35);
	cout << char(175);
	gotoXY(113, 35);
	cout << page;
	char c;
	while (true)
	{
		if (kbhit())
		{
			c = getch();
			if (c == -32)
			{
				c = getch();
				if (c == 75)
				{
					if (tam != 0)
					{
						tam = tam - 6;
						if (tam < 0)
						{
							tam = 0;
						}
						tam1 = tam + 6;
						if (tam1 > n)
						{
							tam1 = n;
						}
						page--;
						XoaManHinh(tdX + 20, tdY, 80, 16);
						GiaoDienDMS(tdX+20, tdY);
						XuatDanhSachDanhMucSachBangMang(Dms, tdX + 20, tdY, tam, tam1);
						
						gotoXY(111, 35);
						cout << char(174);
						gotoXY(115, 35);
						cout << char(175);
						gotoXY(113, 35);
						cout << page;
					}
				}
				else if (c == 77)
				{
					if (tam1 < n)
					{
						tam = tam + 6;
						tam1 = tam1 + 6;
						if (tam1 > n)
						{
							tam1 = n;
						}
						page++;
						XoaManHinh(tdX + 20, tdY, 80, 16);
						GiaoDienDMS(tdX+20, tdY);
						XuatDanhSachDanhMucSachBangMang(Dms, tdX + 20, tdY, tam, tam1);
						
						gotoXY(111, 35);
						cout << char(174);
						gotoXY(115, 35);
						cout << char(175);
						gotoXY(113, 35);
						cout << page;
					}
				}

			}
			else if (c == 27)
			{
				return;
			}
		}
	}

}

	//Tim Kiem
void TimTTSachDuaVaoTenSach(MCONTRO m, int tdX, int tdY)
{
	DauSach *DauSachTam=new DauSach[m.SoLuongDauS];
	DanhMucSach Dms[10000];
	int n;
	string ten;
	VeHCN(45,4,38,3,6);
	VeHCN(58,11,50,2,6);
	gotoXY(50,5);
	cout<<"TIM KIEM DUA VAO TEN SACH";
	gotoXY(45, 12);
	cout << "Nhap ten sach";
	int a=12;
	gotoXY(60,12);
	XuLyNhap_TenSach1(ten,60,a,45);
	int SoKetQuaTimKiem = 0;
	if (ten.empty())
	{
		delete[]DauSachTam;
		return;
	}
	ChuanHoaChuoi(ten);
	
	bool KT = false;
	for (int i = 0; i < m.SoLuongDauS; i++)
	{
		if (strstr(m.nodesDauSach[i]->TenSach.c_str(), ten.c_str())!=NULL)
		{
			KT = true;
			DauSachTam[SoKetQuaTimKiem++] = *m.nodesDauSach[i];
		}
	}
	if (KT == false)
	{;
		gotoXY(40, 15);
		cout << "KHONG TIM THAY TEN SACH NAY";
		gotoXY(10, 35);
		system("pause");
		TextColor(7);
		return;
	}
	gotoXY(53, 15);
	cout << "Co " << SoKetQuaTimKiem  << " ket qua tim kiem";
	for (int i = 0; i < SoKetQuaTimKiem; i++)
	{
		XuatMotDauSach(DauSachTam[i], tdX , tdY+4+i*2);
	}

	int pointer1 = tdY + 4;
	char c;
	while (true)
	{
		
		if (kbhit())
		{
			c = getch();
			if (c == -32)
			{
				c = getch();
				if (c == 72)
				{
					if (pointer1==tdY+4)
					{
						pointer1 = tdY + 4 + 2 * (SoKetQuaTimKiem-1);
					}
					else
						pointer1 = pointer1 - 2;
				}
				else if (c == 80)
				{
					if (pointer1 == tdY + 4 + 2 * (SoKetQuaTimKiem-1))
					{
						pointer1 = tdY + 4;
					}
					else
						pointer1 = pointer1 + 2;
				}
			}
			else if (c == 13)
			{
				XoaManHinh(5,18,140,15);
				n = 0;
				gotoXY(51, tdY+7);
				cout <<"Trang Thai Cua Sach " + DauSachTam[(pointer1 - tdY - 4) / 2].TenSach;
				ChuyenDanhMucSachSangMang(DauSachTam[(pointer1 - tdY - 4) / 2], Dms,n);
				XuatDanhSachDanhMucSachTheoPage(Dms, tdX+5, tdY + 9, n);
			}
			else if (c == 27)
			{
				delete[]DauSachTam;
				return;
			}
		}
		Sleep(100);
	}
}

void Savefile_DMS(MCONTRO &t){
	ofstream FileOut;
	FileOut.open("DSSach.txt", ios_base::out);
	for(int i=0;i<t.SoLuongDauS;i++)
	{
		for(Node_Sach *k = t.nodesDauSach[i]->List_DMS.pHead; k != NULL; k = k->pNext)
		{
			FileOut<<t.nodesDauSach[i]->ISBN<<",";
			FileOut<<k->data.MaSach<<",";
			FileOut<<k->data.vitri<<",";
			FileOut<<k->data.TrangThai<<endl;
		}
	}
	FileOut.close();
}

void ThemVaoCuoi(LIST_DMS &l, Node_Sach* p){
	if(l.pHead == NULL){
		l.pHead = l.pTail = p;
	}
	else 
	{
		l.pTail->pNext = p;
		l.pTail = p;
	}
}

void khoitaoList_dms(LIST_DMS &l)
{
	l.pHead=l.pTail=NULL;
}


void LoadFileDMS(MCONTRO &t)
{
	ifstream FileIn;
	FileIn.open("DSSach.txt",ios::in);
	string s="";
	string madausach="";
	string masach="";
	string trangthai="";
	string vitri="";
	int dem=0;
	int bien=0;
	for(int i=0;i<t.SoLuongDauS;i++)
	{
		khoitaoList_dms(t.nodesDauSach[i]->List_DMS);
	}
	while (FileIn.eof() == false)
	{
		getline(FileIn,s);
		for(int i=0;i<s.length();i++)
		{
			if(dem==0)
			{
				if(s[i]==',')
				{
					dem=1;
					i++;
				}
				else
				{
					madausach+=s[i];
				}
			}
			if(dem==1)
			{
				if(s[i]==',')
				{
					i++;
					dem=2;
				}
				else
				{
					masach+=s[i];
				}
			}
			if(dem==2)
			{
				if(s[i]==',')
				{
					i++;
					dem=3;
				}
				else
				{
					vitri+=s[i];
				}
			}
			if(dem==3)
			{
					trangthai+=s[i];
				
			}
		}
		for(int i=0;i<t.SoLuongDauS;i++)
		{
			if(t.nodesDauSach[i]->ISBN==madausach)
			{
				bien=i;
			}
		}
		Node_Sach *node_dms;
		node_dms=new Node_Sach;
		node_dms->pNext=NULL;
		node_dms->data.MaSach=masach;
		node_dms->data.TrangThai= atoi(trangthai.c_str());
		node_dms->data.vitri=vitri;
		if(masach!="")
		{
			ThemVaoCuoi(t.nodesDauSach[bien]->List_DMS,node_dms);
		}
		trangthai="";
		madausach="";
		masach="";
		vitri="";
		dem=0;
	}
	FileIn.close();
}


//======= cau f ==========================================

void GiaoDienMuonSach(int &x){
	gotoXY(80,2);
	cout<<"MUON SACH";
	VeHCN(70,1,28,2,5);
	gotoXY(30,6);
	cout<<"NHAP MA DOC GIA CUA DOC GIA: ";
	VeHCN(60,5,30,2,15);
	gotoXY(62,6);
	int a=62;
	int b=6;
	XuLyNhap_MaDG(x, a, b);
}



int TachLayMaSach(string str)
{
	string str1;
	for (int i = str.length() - 1; i >= 0; i--)
	{
		if (str[i] == '_')
		{
			break;
		}
		str1.push_back(str[i]);
	}
	return atoi(str1.c_str());
}

void XuLyNhap_MaSach(string& InPut)
{
	while (true)
	{
		char c = getch();
		if ((c >= 48 && c <= 57) || c == '_' || (c >= 65 && c <= 90))
		{
			InPut.insert(InPut.begin() + InPut.length(), c);
			cout << c;
		}
		else if (c >= 97 && c <= 122)
		{
			c = c - 32;
			InPut.insert(InPut.begin() + InPut.length(), c);
			cout << c;
		}
		else if (c == 8 && InPut.length() > 0)
		{
			InPut.erase(InPut.begin() + InPut.length() - 1);
			cout << "\b";
			cout << " ";
			cout << "\b";
		}
		else if (c == 27)
		{
			InPut.clear();
			return;
		}
		else if (c == 13 && InPut.length() > 0)
		{
			break;
		}
	}
}



bool LayThongTinSach(MCONTRO DauS, string x ,DauSach *&DauSTam,Node_Sach *&tam)
{
	for (int i = 0; i < DauS.SoLuongDauS; i++)
	{
		if (TachLayMaDauSach(x) == DauS.nodesDauSach[i]->ISBN)
		{
			for (Node_Sach* k = DauS.nodesDauSach[i]->List_DMS.pHead; k != NULL; k = k->pNext)
			{
				if (x == k->data.MaSach)
				{
					DauSTam = DauS.nodesDauSach[i];
					tam = k;
					return true;
				}
			}
		}
	}
	return false;
}

void XuLyMuonSach(TREE& t,DauSach *DauSTam,Node_Sach *s,Date Ngay)
{
	t->data.SoLuongSachDangMuon++;
	s->data.TrangThai = 1;
	DauSTam->SoLanMuon++;
	NODE_DOUBLELIST* p = new NODE_DOUBLELIST;
	if (p == NULL)
	{
		cout << "\nCap phat that bai !";
		return;
	}
	p->data.MaSach = s->data.MaSach;
	p->data.NgayMuon=Ngay;
	p->data.NgayTra.Ngay = 0;;
	p->data.NgayTra.Thang = 0;
	p->data.NgayTra.Nam = 0;
	p->data.TrangThai = 0;
	p->pNext = NULL;
	p->pPrev = NULL;

	if (t->data.DoubleList_MuonTra.pHead == NULL)
	{
		t->data.DoubleList_MuonTra.pHead = p;
		t->data.DoubleList_MuonTra.pTail = p;
	}
	else
	{
		t->data.DoubleList_MuonTra.pTail->pNext = p;
		p->pPrev = t->data.DoubleList_MuonTra.pTail;
		t->data.DoubleList_MuonTra.pTail = p;
	}
}

void GiaoDienMuonSach1(int tdX, int tdY){
	gotoXY(tdX+95,tdY+2);
	cout<<"NHAP MA SACH";
	gotoXY(tdX + 80,tdY + 8);
	cout<<"Ma sach";
	gotoXY(tdX + 80, tdY + 13);
	cout<<"Ngay";
	gotoXY(tdX + 90, tdY + 13);
	cout<<"Thang";
	gotoXY(tdX + 100, tdY + 13);
	cout <<"Nam";
	VeHCN(tdX+86,tdY+12,3,2,5);
	VeHCN(tdX+96,tdY+12,3,2,5);
	VeHCN(tdX+106,tdY+12,6,2,5);
	VeHCN(tdX+90,tdY+7,40,2,5);
	VeHCN(tdX+90,tdY+1,22,3,5);
	gotoXY(tdX + 3, tdY + 4);
	cout<<"Ma the";
	gotoXY(tdX + 3, tdY + 7);
	cout<<"Ho & ten";
	gotoXY(tdX + 3, tdY + 10);
	cout<<"So sach dang muon";
	gotoXY(tdX + 3, tdY + 13);
	cout<<"Trang thai";
	VeHCN(tdX+22,tdY+3,30,2,5);
	VeHCN(tdX+22,tdY+6,30,2,5);
	VeHCN(tdX+22,tdY+9,30,2,5);
	VeHCN(tdX+22,tdY+12,30,2,5);
	gotoXY(tdX + 50, tdY + 16);
	cout << "Xac Nhan";
	gotoXY(tdX + 70, tdY + 16);
	cout << "Cancel";
}


void MuonSach(TREE & t, MCONTRO &m,int tdX,int tdY)
{
	int MaDocGia = 0;
	if (m.SoLuongDauS == 0) 	// thu vien khong co sach nao
	{
		if (m.SoLuongDauS == 0)
		{
			TextColor(240);
			gotoXY(49,7);
			cout << "\aLOI!!!";
			gotoXY(40,10);
			cout << "Thu Vien Van Chua Co Sach Nao";
			gotoXY(40,10 + 1);
			system("pause");
			TextColor(15);
			return;
		}
	}
	bool KTSach = false; // kiem tra tat ca cac sach da cho muon chua
	for (int i = 0; i < m.SoLuongDauS; i++)
	{
		for (Node_Sach* k = m.nodesDauSach[i]->List_DMS.pHead; k != NULL; k = k->pNext)
		{
			if (k->data.TrangThai == 0)
			{
				KTSach = true;
				break;
			}
		}
		if (KTSach == true)
		{
			break;
		}
	}
	
	GiaoDienMuonSach(MaDocGia);
	if (MaDocGia == 0)
	{
		return;
	}
	string q = ChuyenSoSangString(MaDocGia);
	if(KiemTraMaDG(t,q) == false)
	{
		gotoXY(35,15);
		cout<<"MA DOC GIA KHONG TON TAI";
		gotoXY(30,35);
		system("pause");
		return;
	}
	string MaSach;
	char c;
	TREE tam;
	DauSach* DauSTam=NULL;
	Node_Sach* tamSach;
	Date NgayHienTai;
	LayNgayHienTai(NgayHienTai);
	
	LayThongTinDocGia(t, MaDocGia, tam);
	
	gotoXY(tdX + 87, tdY + 13);
	cout << NgayHienTai.Ngay;
	gotoXY(tdX + 97, tdY + 13);
	cout << NgayHienTai.Thang;
	gotoXY(tdX + 107, tdY + 13);
	cout << NgayHienTai.Nam;

	gotoXY(tdX + 24, tdY + 4);
	cout << tam->data.MaThe;
	gotoXY(tdX + 24, tdY + 7);
	cout << tam->data.Ho << " " << tam->data.Ten;
	gotoXY(tdX + 24, tdY + 10);
	cout << tam->data.SoLuongSachDangMuon;
	gotoXY(tdX + 24, tdY + 13);
	cout << ChuyenTrangThaiDocGia(tam->data.TrangThai);
	GiaoDienMuonSach1(tdX, tdY);
	
	// DG duoc quyen muon sach
	if(tam->data.SoLuongSachDangMuon< 3 && tam->data.TrangThai == 1){
		gotoXY(tdX + 91, tdY + 8);
		XuLyNhap_MaSach(MaSach);
		if(LayThongTinSach(m,MaSach,DauSTam, tamSach)){  //DG dang muon sach nay
			for (NODE_DOUBLELIST* k = tam->data.DoubleList_MuonTra.pHead; k != NULL; k = k->pNext)
			{
				if(k->data.TrangThai == 0 && TachLayMaDauSach(k->data.MaSach) == TachLayMaDauSach(MaSach))
				{
					gotoXY(50,30);
					cout<<"DOC GIA DANG MUON SACH NAY";
					return;
				}
			}
			if (tamSach->data.TrangThai == 0)
				{
					int pointer1 = tdX + 50;
					while (true)
					{
						gotoXY(tdX + 50, tdY + 16);
						cout << "Xac Nhan";
						gotoXY(tdX + 70, tdY + 16);
						cout << "Cancel";
	
						if (kbhit())
						{
							c = getch();
							if (c == -32)
							{
								c = getch();
								if (c == 75)
								{
									if (pointer1 == tdX + 50)
									{
										pointer1 = tdX + 70;
									}
									else
										pointer1 = tdX + 50;
								}
								else if (c == 77)
								{
									if (pointer1 == tdX + 70)
									{
										pointer1 = tdX + 50;
									}
									else
										pointer1 = tdX + 70;
								}
							}
							else if (c == 13)
							{
								if (pointer1 == tdX + 50)
								{
									XuLyMuonSach(tam, DauSTam,tamSach,NgayHienTai);
									gotoXY(50,30);
									cout<<"MUON SACH THANH CONG";
									return;
								}
								else if (pointer1 == tdX + 70)
								{
									return;
								}
							}
							else if (c == 27)
							{
								return;
							}
						gotoXY(pointer1, tdY + 16);
						TextColor(192);
						if (pointer1 == tdX + 50)
						{
							cout << "Xac Nhan";
						}
						else if (pointer1 == tdX + 70)
						{
							cout << "Cancel";
						}
						TextColor(7);
						Sleep(100);
					}
				}
			}
			else if(tamSach->data.TrangThai == 2)
			{
					gotoXY(50,30);
					cout<<"SACH NAY DA BI MAT!!!";
					return;
			}
			else
			{
					gotoXY(50,30);
					cout<<"SACH NAY DANG DUOC MUON!!!";
					return;
			}
			
		}
		if(!LayThongTinSach(m,MaSach,DauSTam, tamSach))
		{
			gotoXY(30,30);
			cout<<"MA SACH KHONG TON TAI";
			return;
		}
	}
}

void ThemDSLKKep(DOUBLELIST &k, NODE_DOUBLELIST *p)
{
	if (k.pHead == NULL)
	{
		k.pHead = k.pTail = p;
	}
	else
	{
		k.pTail->pNext = p;
        p->pPrev = k.pTail;
        k.pTail = p;
	}
}

//================== XUAT DANH SACH CAC SACH=============================

string TrangThaiMT(int &n)
{
	if(n==0)
	{
		return "Sach dang muon";
	}
	else if(n==1)
	{
		return "Sach da tra";
	}
	else return "Lam mat sach";
}

void ChuyenDSMuonSangMang(TREE t, MuonTra MT[],int MaDocGia[],int &n)
{
	if (t == NULL)
	{
		return;
	}
	else
	{
		for (NODE_DOUBLELIST* k = t->data.DoubleList_MuonTra.pHead; k != NULL; k = k->pNext)
		{
			if (k->data.TrangThai == 0)
			{
				MaDocGia[n] = t->data.MaThe;
				MT[n] = k->data;
				n++;
			}
		}
		ChuyenDSMuonSangMang(t->pLeft,MT,MaDocGia, n);
		ChuyenDSMuonSangMang(t->pRight, MT,MaDocGia,n);
	}
}



string LayTenSach(MCONTRO DauS, string MaSach)
{
	for (int i = 0; i < DauS.SoLuongDauS; i++)
	{
		if (DauS.nodesDauSach[i]->ISBN == TachLayMaDauSach(MaSach))
		{
			return DauS.nodesDauSach[i]->TenSach;
		}
	}
	return "Ma Sach Ton Tai";
}


void XuatDanhSachMuonBangMang(MCONTRO DauS,MuonTra MT[], int MaDocGia[], int tam,int tam1,int tdX,int tdY)
{
	int j = 0;
	gotoXY(tdX + 3, tdY + j + 2);
	cout <<"MA SACH";
	gotoXY(tdX + 23, tdY + j + 2);
	cout << "ISBN";
	gotoXY(tdX + 38, tdY + j + 2);
	cout << "TEN SACH";
	gotoXY(tdX + 79, tdY + j + 2);
	cout << "NGAY MUON";
	gotoXY(tdX + 99, tdY + j + 2);
	cout << "TRANG THAI";	
	for (int i = tam; i < tam1; i++)
	{
		if(LayTenSach(DauS, MT[i].MaSach)!="Ma Sach Ton Tai"){
			gotoXY(tdX + 1, tdY + j + 4);
			cout << MT[i].MaSach;
			gotoXY(tdX + 21, tdY + j + 4);
			cout << ChuyenSoSangString(MaDocGia[i]);
			gotoXY(tdX + 36, tdY + j + 4);
			cout << LayTenSach(DauS, MT[i].MaSach);
			gotoXY(tdX + 77, tdY + j + 4);
			cout << ChuyenSoSangString(MT[i].NgayMuon.Ngay) + "/" + ChuyenSoSangString(MT[i].NgayMuon.Thang) + "/" + ChuyenSoSangString(MT[i].NgayMuon.Nam);
			gotoXY(tdX + 97, tdY + j + 4);
			cout << TrangThaiMT(MT[i].TrangThai);
			j = j + 2;
		}
	}
	TextColor(7);
}

void GiaoDienCacSachDangMuon()
{
	VeHCN(19,13,20,16,2);
	VeHCN(39,13,15,16,2);
	VeHCN(54,13,41,16,2);
	VeHCN(95,13,20,16,2);
	VeHCN(115,13,17,16,2);
}

void XuatTheoPageDanhSachSachMuon(MCONTRO DauS,MuonTra MT[],int MaDocGia[],int n, int tdX, int tdY)
{
	VeHCN(50,4,29,3,4);
	TextColor(8);
	gotoXY(55,5);
	cout<<"CAC SACH DANG MUON";
	int page = 1;
	char c;
	int tam = 0;
	int tam1 = tam + 8;
	TextColor(7);
	XuatDanhSachMuonBangMang(DauS, MT,MaDocGia,tam, tam1, tdX, tdY);
	GiaoDienCacSachDangMuon();
	XuatPage(page);
	while (true)
	{
		if (kbhit())
		{

			c = getch();
			if (c == -32)
			{
				c = getch();
				if (c == 75)
				{
					if (tam != 0)
					{
						tam = tam - 8;
						if (tam < 0)
						{
							tam = 0;
						}
						tam1 = tam + 8;
						if (tam1 > n)
						{
							tam1 = n;
						}
						page--;
						gotoXY(tdX,tdY+4);
						XoaManHinh(tdX,tdY,120,20);
						GiaoDienCacSachDangMuon();
						XuatDanhSachMuonBangMang(DauS, MT, MaDocGia,tam, tam1, tdX, tdY);
						XuatPage(page);
					}
				}
				else if (c == 77)
				{
					if (tam1 < n)
					{
						tam = tam + 8;
						tam1 = tam1 + 8;
						if (tam1 > n)
						{
							tam1 = n;
						}
						page++;
						gotoXY(tdX,tdY+4);
						XoaManHinh(tdX,tdY,120,20);
						GiaoDienCacSachDangMuon();
						XuatDanhSachMuonBangMang(DauS, MT, MaDocGia,tam, tam1, tdX, tdY);
						XuatPage(page);
					}
				}
			}
			else if (c == 27)
			{
				return;
			}
		}
	}
}

void XuatDanhSachCacSachDangMuon(MCONTRO m,TREE t,int tdX, int tdY)
{
	int n = 0;
	MuonTra MT[5000];
	int MaDocGia[5000];
	
	ChuyenDSMuonSangMang(t, MT,MaDocGia,n);
	XuatTheoPageDanhSachSachMuon(m,MT, MaDocGia, n, tdX, tdY);
}

// ===================== cau g (TRA SACH) ========================
void XuLiTraSach(MuonTra &Mt, MCONTRO &m)
{
	for (int i = 0; i < m.SoLuongDauS; i++)
	{
		if (TachLayMaDauSach(Mt.MaSach) == m.nodesDauSach[i]->ISBN)
		{
			for (Node_Sach* k = m.nodesDauSach[i]->List_DMS.pHead; k != NULL; k = k->pNext)
			{
				if (Mt.MaSach == k->data.MaSach)
				{
					if (Mt.TrangThai == 1)
					{
						k->data.TrangThai = 0;
					}
					else if (Mt.TrangThai == 2)
					{
						k->data.TrangThai = 2;
					}
					break;
				}
			}
			break;
		}
	}
} 


void XuatSachMuonCua1DG(MCONTRO m, TREE t, int tdX, int tdY)
{
	gotoXY(tdX+1,tdY+2);
	cout<<"MA SACH";
	gotoXY(tdX+21,tdY+2);
	cout<<"TEN SACH";
	gotoXY(tdX+62,tdY+2);
	cout<<"NGAY MUON";
	gotoXY(tdX+82,tdY+2);
	cout<<"TRANG THAI";
	int j = 0;
	for (NODE_DOUBLELIST* k = t->data.DoubleList_MuonTra.pHead; k != NULL; k = k->pNext)
	{
		if (k->data.TrangThai == 0)
		{
			gotoXY(tdX + 1, tdY + 4 + j);
			cout << k->data.MaSach, 19;
			gotoXY(tdX + 21, tdY + 4 + j);
			cout << LayTenSach(m, k->data.MaSach);
			gotoXY(tdX + 62, tdY + 4 + j);
			cout << ChuyenSoSangString(k->data.NgayMuon.Ngay) + "/" + ChuyenSoSangString(k->data.NgayMuon.Thang) + "/" + ChuyenSoSangString(k->data.NgayMuon.Nam);
			gotoXY(tdX + 82, tdY + 4 + j);
			cout << k->data.TrangThai;
			j = j + 2;
		}
	}
	TextColor(7);
}

void GiaoDienTraSach(int &x){
	gotoXY(80,2);
	cout<<"TRA SACH";
	VeHCN(70,1,28,2,5);
	gotoXY(30,6);
	cout<<"NHAP MA DOC GIA CUA DOC GIA: ";
	VeHCN(60,5,30,2,15);
	gotoXY(62,6);
	int a=62;
	int b=6;
	XuLyNhap_MaDG(x, a, b);
}


void XuatTrangThaiSachSauKhiTra(MuonTra MT,int tdX, int tdY)
{
	gotoXY(tdX + 1, tdY);
	cout << MT.MaSach;
	gotoXY(tdX + 26, tdY);
	cout <<ChuyenSoSangString(MT.NgayMuon.Ngay) + "/" + ChuyenSoSangString(MT.NgayMuon.Thang) + "/" + ChuyenSoSangString(MT.NgayMuon.Nam);
	gotoXY(tdX + 51, tdY);
	cout <<ChuyenSoSangString(MT.NgayTra.Ngay) + "/" + ChuyenSoSangString(MT.NgayTra.Thang) + "/" + ChuyenSoSangString(MT.NgayTra.Nam);
	gotoXY(tdX + 76, tdY);
	cout << MT.TrangThai;
}

bool KhungSuaTrangThaiSach(NODE_DOUBLELIST *MT){
	Date d;
	LayNgayHienTai(d);
	d=MT->data.NgayTra;
	VeHCN(120,12,40,16,3);
	gotoXY(123,14);
	cout<<"SUA TRANG THAI SACH";
	gotoXY(123,16);
	cout<<"Ma sach:";
	gotoXY(135,16);
	cout<<MT->data.MaSach;
	gotoXY(123,18);
	cout<<"Ngay muon:";
	gotoXY(135,18);
	cout<<MT->data.NgayMuon.Ngay<<"/"<<MT->data.NgayMuon.Thang<<"/"<<MT->data.NgayMuon.Nam;
	gotoXY(123,20);
	cout<<"Ngay tra:";
	gotoXY(135,20);
	cout<<MT->data.NgayTra.Ngay<<"/"<<MT->data.NgayTra.Thang<<"/"<<MT->data.NgayTra.Nam;
	gotoXY(123,22);
	cout<<"Trang thai:";	
	gotoXY(134,22);
	cout<<"LAM MAT SACH";
	char c;
	int pointer = 123;
	while(1){
		gotoXY(123,24);
		cout<<"BAN CO MUON SUA TRANG THAI SACH";
		gotoXY(123,26);
		cout<<"Yes";
		gotoXY(129,26);
		cout<<"No";	
		if(kbhit()){
			c= getch();
			if(c==-32){
				c=getch();
				if(c==77)
				{
					if(pointer == 123){
						pointer =129;
					}
					else pointer = 123;
				}
				else if(c==75)
				{
					if(pointer == 129){
						pointer = 123;
					}
					else pointer = 129;
				}
			}
			else if(c==13){
				if(pointer == 123)
				{
					XoaManHinh(115,11,50,25);
					MT->data.TrangThai = 2;
					return true;
				}
				else if(pointer == 129){
					XoaManHinh(115,11,50,25);
					return false;
				}
			}
		}
		
		gotoXY(pointer, 26);
		TextColor(192);
		switch (pointer)
		{
		case 123:
			cout << "Yes";
			break;
		case 129:
			cout << "No";
			break;
		}
		TextColor(7);
		Sleep(100);
		
		
	}
}

Date CongNgay(Date t,int SoNgay)
{
	Date d = t;
	d.Ngay = d.Ngay + SoNgay;
	if (d.Ngay > SoNgayCuaThang(d.Thang))
	{
		if (d.Thang == 12)
		{
			d.Thang = 1;
			d.Nam = d.Nam + 1;
		}
		else
		{
			d.Thang = d.Thang + 1;
		}
		d.Ngay = d.Ngay - SoNgayCuaThang(t.Thang);
	}
	return d;
}
int SoNgayQuaHan(Date d)
{
	Date d1;
	LayNgayHienTai(d1);
	if (SoSanhNgay(d1, d) == 1)
	{
		if (d.Nam == d1.Nam)
		{
			if (d.Thang == d1.Thang)
			{
				return d1.Ngay - d.Ngay;
			}
			else
			{
				return SoNgayCuaThang(d.Thang) - d.Ngay + d1.Ngay;
			}
		}
		else
		{
			int SoNgayConLaiCuaNam = 0;
			for (int i = 1; i < d.Thang; i++)
			{
				SoNgayConLaiCuaNam = SoNgayConLaiCuaNam + i * SoNgayCuaThang(i);
			}
			if (KTNamNhuan(d.Nam))
			{
				return 366 - SoNgayConLaiCuaNam - d.Ngay + d1.Ngay;
			}
			else
			{
				return 365 - SoNgayConLaiCuaNam - d.Ngay + d1.Ngay;
			}
		}
	}
	else
		return 0;
}

void KiemTraQuaHan1DocGia(TREE &t, Date NgayHienTai)
{
	bool KTKhoaDG=false;
	Date Max;
	LayNgayHienTai(Max);
	for (NODE_DOUBLELIST* k = t->data.DoubleList_MuonTra.pHead; k != NULL; k = k->pNext)
	{
		if (k->data.TrangThai == 0 && SoSanhNgay(NgayHienTai, CongNgay(k->data.NgayMuon,SoNgayDcMuon)) == 1)
		{
			KTKhoaDG = true;
			t->data.TrangThai = 0;
			if (SoSanhNgay(Max, k->data.NgayMuon) == 1)
			{
				Max = k->data.NgayMuon;
			}
		}
		else if (k->data.TrangThai == 2)
		{
			KTKhoaDG = true;
			t->data.TrangThai = 0;
		}
	}
	if (KTKhoaDG == false)
	{
		t->data.TrangThai = 1;
		t->data.SoNgayQuaHanMax = 0;
	}
	else
		t->data.SoNgayQuaHanMax = SoNgayQuaHan(CongNgay(Max,SoNgayDcMuon));
}

void KiemTraQuaHan(TREE& t, Date NgayHienTai)
{
	if (t == NULL)
	{
		return;
	}
	else
	{
		Date Max;
		LayNgayHienTai(Max);
		for (NODE_DOUBLELIST* k = t->data.DoubleList_MuonTra.pHead; k != NULL; k = k->pNext)
		{
			if (k->data.TrangThai == 0 && SoSanhNgay(NgayHienTai, CongNgay(k->data.NgayMuon,SoNgayDcMuon)) == 1)
			{
				t->data.TrangThai = 0;
				if (SoSanhNgay(Max, k->data.NgayMuon) == 1)
				{
					Max = k->data.NgayMuon;
				}
			}
			else if (k->data.TrangThai == 2)
			{
				t->data.TrangThai = 0;
			}
		}
		t->data.SoNgayQuaHanMax = SoNgayQuaHan(CongNgay(Max,SoNgayDcMuon));

		KiemTraQuaHan(t->pLeft, NgayHienTai);
		KiemTraQuaHan(t->pRight, NgayHienTai);
	}
}

bool TrangThaiSachSauKhiTra(NODE_DOUBLELIST *MT[], int n, int tdX, int tdY, MCONTRO &m, TREE &t)
{
	for (int i = 0; i < n; i++)
	{
		MT[i]->data.TrangThai = 1;
		LayNgayHienTai(MT[i]->data.NgayTra);
	}
	a:
	gotoXY(51, tdY - 6);
	cout <<"Doc Gia:" + t->data.Ho + " " + t->data.Ten;
	gotoXY(51, tdY - 3);
	cout << "TRANG THAI CUA SACH SAU KHI TRA ";
	for (int i = 0; i < n; i++)
	{
		XuatTrangThaiSachSauKhiTra(MT[i]->data,tdX+15, tdY + 4 + i * 2);
	}
	int pointer1 = tdY+4;
	int pointer2 = 30;
	char c;
	while(true)
	{
		gotoXY(0,0);
		gotoXY(30, tdY+14);
		cout << "Tiep Tuc";
		gotoXY(60, tdY+14);
		cout << "Tro Lai";
		if (kbhit())
		{
			c = getch();
			if (c == -32)
			{
				c = getch();
				if (c == 72)
				{
					if (pointer1 == tdY + 4)
					{	
						gotoXY(tdX,pointer1);
						cout<<"     ";				
						pointer1 = tdY + 14;
					}
					else if (pointer1 == tdY + 14)
					{
						pointer1 = tdY + 4 + n*2 - 2;
						gotoXY(tdX,pointer1);
						cout<<"->";
					}
					else{
						gotoXY(tdX,pointer1);
						cout<<"     ";	
						pointer1 = pointer1 - 2;
						gotoXY(tdX,pointer1);
						cout<<"->";	
					}
				}
				else if (c == 80)
				{
					if (pointer1 == tdY + 4 + n*2 - 2)
					{
						gotoXY(tdX,pointer1);
						cout<<"     ";	
						pointer1 = tdY + 14;						
					}
					else if (pointer1 == tdY + 14)
					{
						pointer1 = tdY + 4;
						gotoXY(tdX,pointer1);
						cout<<"->";	
					}
					else{
						gotoXY(tdX,pointer1);
						cout<<"     ";
						pointer1 = pointer1 + 2;
						gotoXY(tdX,pointer1);
						cout<<"->";							
					}
				}
				else if (c == 77 && pointer1 == tdY + 14)
				{
					if(pointer2 == 60)
					{
						pointer2 = 30;	
					}
					else if(pointer2==30){
						pointer2=60;
					}
				}
				else if (c == 75 && pointer1 == tdY + 14)
				{
					if(pointer2 == 30)
					{
						pointer2 = 60;	
					}
					else if(pointer2 ==60)
					{
						pointer2 =30;
					}
				}
			}
			else if(c==13){
				if(pointer1 == tdY+4){
					if(KhungSuaTrangThaiSach(MT[0]))
					{
						XoaManHinh(tdX,tdY,150,30);
						goto a;
						gotoXY(10,0);
						cout<<MT[0]->data.TrangThai;
					}
					else
					{
						XoaManHinh(tdX,tdY,150,30);
						goto a;
					}
				}
				else if(pointer1 == tdY+6){
					if(KhungSuaTrangThaiSach(MT[1]))
					{
						XoaManHinh(tdX,tdY,150,30);
						goto a;
						gotoXY(10,0);
						cout<<MT[1]->data.TrangThai;
					}
					else
					{
						XoaManHinh(tdX,tdY,150,30);
						goto a;
					}
				}
				else if(pointer1 == tdY+8){
					if(KhungSuaTrangThaiSach(MT[2]))
					{
						XoaManHinh(tdX,tdY,150,30);
						goto a;
						gotoXY(10,0);
						cout<<MT[2]->data.TrangThai;
					}
					else
					{
						XoaManHinh(tdX,tdY,150,30);
						goto a;
					}
				}
				else if(pointer1 == tdY+14){
					switch(pointer2)
					{
						case 30:
						{
							for(int i=0; i<n; i++){
								t->data.SoLuongSachDangMuon--;
								XuLiTraSach(MT[i]->data,m);
							}
							Date NgayHienTai;
							LayNgayHienTai(NgayHienTai);
							KiemTraQuaHan1DocGia(t, NgayHienTai);							
							gotoXY(tdX+20,tdY+25);
							cout<<"TRA SACH THANH CONG!!";
							system("pause");
							return true;						
							break;

						}
						case 60:
						{
							for(int i=0; i<n; i++){
								MT[i]->data.TrangThai = 0;
								MT[i]->data.NgayTra.Ngay = 0;
								MT[i]->data.NgayTra.Thang = 0;
								MT[i]->data.NgayTra.Nam = 0;
							}
							return false;
						}
					}

				}
			}
		}
		gotoXY(pointer2, tdY+14);
		TextColor(192);
		switch (pointer2)
		{
		case 30:
			cout << "Tiep Tuc";
			break;
		case 60:
			cout << "Tro Lai";
			break;
		}
		TextColor(7);
		Sleep(100);
	}
}


void TraSach(TREE &t, MCONTRO &m, int tdX, int tdY)
{
	int MaDocGia = 0;

	TREE tam = NULL;
	GiaoDienTraSach(MaDocGia);
	if (MaDocGia == 0)
	{
		return;
	}
	string q = ChuyenSoSangString(MaDocGia);
	if(KiemTraMaDG(t,q) == false)
	{
		gotoXY(35,15);
		cout<<"MA DOC GIA KHONG TON TAI";
		gotoXY(30,35);
		system("pause");
		return;
	}
	LayThongTinDocGia(t, MaDocGia, tam);
	
	
	bool ktChon[3]{false};
	 
	NODE_DOUBLELIST* Mt[3];
	int dem = 0;

	NODE_DOUBLELIST* MTtam[3];
	int z = 0;
	for (NODE_DOUBLELIST* k = tam->data.DoubleList_MuonTra.pHead; k != NULL; k = k->pNext)
	{
		if (k->data.TrangThai == 0) // sach dang dc muon
		{
			MTtam[z++] = k;
		}
	}
	
	if(z==0){
		gotoXY(50,12);
		cout<<"DOC GIA NAY CHUA MUON SACH NAO!!";
		return;
	}

	int pointer1 = tdY + 4;
	int pointer2 = 60;
	char c;
	TextColor(7);
	gotoXY(51, tdY-3);
	cout<<"Doc Gia:" + tam->data.Ho + " " + tam->data.Ten;
	gotoXY(51, tdY-1);
	cout<<"Danh Sach Cac Sach Dang Muon ";
	XuatSachMuonCua1DG(m,tam,tdX+10,tdY);
	gotoXY(tdX,pointer1);
	cout<<"->";
	while(true){
		gotoXY(60, tdY + 11);
		cout << "Xac Nhan";
		gotoXY(85, tdY + 11);
		cout << "Tro Lai";
		XuatSachMuonCua1DG(m,tam,tdX+10,tdY);
		TextColor(7);
		int j = tam->data.SoLuongSachDangMuon * 2;
		if (kbhit())
		{
			c = getch();
			if (c == -32)
			{
				c = getch();
				if (c == 72)
				{
					if (pointer1 == tdY + 4)
					{	
						gotoXY(tdX,pointer1);
						cout<<"     ";				
						pointer1 = tdY + 10;
					}
					else if (pointer1 == tdY + 10)
					{
						pointer1 = tdY + 4 + j - 2;
						gotoXY(tdX,pointer1);
						cout<<"->";
					}
					else{
						gotoXY(tdX,pointer1);
						cout<<"     ";	
						pointer1 = pointer1 - 2;
						gotoXY(tdX,pointer1);
						cout<<"->";	
					}
				}
				else if (c == 80)
				{
					if (pointer1 == tdY + 4 + j - 2)
					{
						gotoXY(tdX,pointer1);
						cout<<"     ";	
						pointer1 = tdY + 10;	

					}
					else if (pointer1 == tdY + 10)
					{
						pointer1 = tdY + 4;
						gotoXY(tdX,pointer1);
						cout<<"->";	
					}
					else{
						gotoXY(tdX,pointer1);
						cout<<"     ";
						pointer1 = pointer1 + 2;
						gotoXY(tdX,pointer1);
						cout<<"->";							
					}
				}
				else if (c == 75 && pointer1 == tdY + 10)
				{
					if (pointer2 == 85)
					{
						pointer2 = 60;
					}
					else
						pointer2 = 85;
				}
				else if (c == 77 && pointer1 == tdY + 10)
				{
					if (pointer2 == 60)
					{
						pointer2 = 85;
					}
					else
						pointer2 = 60;
				}
			}else if (c == 13)
			{
				if (pointer1 == tdY + 4)
				{
					if (ktChon[0] == false)
					{
						Mt[dem++] = MTtam[0];
						gotoXY(tdX + 98, tdY+4);
						TextColor(236);
						cout << "*";
						ktChon[0] = true;
					}
					else
					{
						gotoXY(tdX + 98, tdY + 4);
						TextColor(0);
						cout << " ";
						ktChon[0] = false;
						for (int i = 0; i < dem; i++)
						{
							if (Mt[i]->data.MaSach == MTtam[0]->data.MaSach)
							{
								for (int j = i; j < dem - 1; j++)
								{
									Mt[j] = Mt[j + 1];
								}
								dem--;
							}
						}
					}

				}
				else if (pointer1 == tdY + 6)
				{
					if (ktChon[1] == false)
					{
						Mt[dem++] = MTtam[1];
						gotoXY(tdX + 98, tdY + 6);
						TextColor(236);
						cout << "*";
						ktChon[1] = true;
					}
					else
					{
						gotoXY(tdX + 98, tdY + 6);
						TextColor(0);
						cout << " ";
						ktChon[1] = false;
						for (int i = 0; i < dem; i++)
						{
							if (Mt[i]->data.MaSach == MTtam[1]->data.MaSach)
							{
								for (int j = i; j < dem - 1; j++)
								{
									Mt[j] = Mt[j + 1];
								}
							}
							dem--;
						}
					}

				}
				else if (pointer1 == tdY + 8)
				{

					if (ktChon[2] == false)
					{
						Mt[dem++] = MTtam[2];
						gotoXY(tdX + 98, tdY+8);
						TextColor(236);
						cout << "*";
						ktChon[2] = true;
					}
					else
					{
						gotoXY(tdX + 98, tdY + 8);
						TextColor(0);
						cout << " ";
						ktChon[2] = false;
						for (int i = 0; i < dem; i++)
						{
							if (Mt[i]->data.MaSach == MTtam[2]->data.MaSach)
							{
								for (int j = i; j < dem; j++)
								{
									Mt[j] = Mt[j + 1];
								}
							}
							dem--;
						}
					}
				}
				else if (pointer1 == tdY + 10)
				{
					switch (pointer2)
					{
					case 60:
					{
						XoaManHinh(tdX+10,tdY,100,18);
						if (dem > 0)
						{
							if (!TrangThaiSachSauKhiTra(Mt, dem, tdX, tdY, m, tam))
							{
								return;
							}
							else
								return;
						}
						else
						{
							VeHCN(tdX+25,tdY+5,47,4,5);
							gotoXY(tdX+27,tdY+7);
							
							cout<<"Ban phai chon sach muon tra hoac chinh sua!!";
							return;
						}
					}
					case 85:
					{
						return;
					}
					}
				}
			}
		}
		else if (pointer1 == tdY + 10)
		{
			TextColor(80);
			gotoXY(pointer2, tdY + 11);
			switch (pointer2)
			{
			case 60:
				{
					cout << "Xac Nhan";
					break;
				}
			case 85:
				{
					cout << "Tro Lai";
					break;
				}
			}
		}
		TextColor(7);
		Sleep(100);
	}
}
//==================== LOADFILE MUON TRA =====================
void ThemDanhSachSachMuonTraTuFile(TREE& t, int x, ifstream& FileIn, MCONTRO& DauS)
{
	if (t == NULL)
	{
		return;
	}
	else if (t->data.MaThe == x)
	{
		NODE_DOUBLELIST* p = new NODE_DOUBLELIST;
		if (p == NULL)
		{
			cout << "\nCap phat that bai !";
			return;
		}
		getline(FileIn, p->data.MaSach, ',');
		FileIn >> p->data.NgayMuon.Ngay;
		FileIn.ignore();
		//FileIn.seekg(1, ios::cur);
		FileIn >> p->data.NgayMuon.Thang;
		FileIn.ignore();
		//FileIn.seekg(1, ios::cur);
		FileIn >> p->data.NgayMuon.Nam;
		FileIn.ignore();
		//.seekg(1, ios::cur);
		FileIn >> p->data.NgayTra.Ngay;
		FileIn.ignore();
		//FileIn.seekg(1, ios::cur);
		FileIn >> p->data.NgayTra.Thang;
		FileIn.ignore();
		//FileIn.seekg(1, ios::cur);
		FileIn >> p->data.NgayTra.Nam;
		FileIn.ignore();
		//FileIn.seekg(1, ios::cur);
		FileIn >> p->data.TrangThai;
		if (p->data.TrangThai == 0)
		{
			t->data.SoLuongSachDangMuon++;
		}
		else if (p->data.TrangThai == 2)
		{
			t->data.TrangThai = 0;
		}
		p->pNext = NULL;
		p->pPrev = NULL;

		for (int i = 0; i < DauS.SoLuongDauS; i++)
		{
			if (TachLayMaDauSach(p->data.MaSach) == DauS.nodesDauSach[i]->ISBN)
			{
				for (Node_Sach* k = DauS.nodesDauSach[i]->List_DMS.pHead; k != NULL; k = k->pNext)
				{
					if (p->data.MaSach == k->data.MaSach)
					{
						DauS.nodesDauSach[i]->SoLanMuon++;
						if (p->data.TrangThai == 0) 
						{
							k->data.TrangThai = 1; 
						}
						else if (p->data.TrangThai == 2)
						{
							k->data.TrangThai = 2;
						}
						break;
					}
				}
				break;
			}
		}
		
		if (t->data.DoubleList_MuonTra.pHead == NULL)
		{
			t->data.DoubleList_MuonTra.pHead = t->data.DoubleList_MuonTra.pTail = p;
		}
		else
		{
			t->data.DoubleList_MuonTra.pTail->pNext = p;
			p->pPrev = t->data.DoubleList_MuonTra.pTail;
			t->data.DoubleList_MuonTra.pTail = p;
		}
		return;
	}
	else if (t->data.MaThe > x)
	{
		ThemDanhSachSachMuonTraTuFile(t->pLeft, x, FileIn, DauS);
	}
	else
	{
		ThemDanhSachSachMuonTraTuFile(t->pRight, x, FileIn, DauS);
	}
}
void LoadFileDanhSachSachMuonTra(TREE& t, MCONTRO& DauS)
{
	int x;
	ifstream FileIn;
	FileIn.open("MuonTra.txt", ios::in);
	while (1)
	{
		x = 0;
		FileIn >> x;
		FileIn.ignore();
		//FileIn.seekg(1, ios::cur);
		if (x == 0)
		{
			break;
		}
		ThemDanhSachSachMuonTraTuFile(t, x, FileIn, DauS);
		if (FileIn.eof())
		{
			break;
		}
	}
	FileIn.close();
}
		
//============= cau i====================


void GiaoDienXuatDGQuaHan()
{
	VeHCN(19,13,10,16,2);
	VeHCN(29,13,30,16,2);
	VeHCN(59,13,15,16,2);
	VeHCN(74,13,40,16,2);
	VeHCN(114,13,12,16,2);
	VeHCN(126,13,22,16,2);
}
void XuatDanhSachDocGiaQuaHanBangMang(TheDocGia a[],MCONTRO m, int tam,int tam1, int tdX, int tdY)
{
	int j = 0;
	gotoXY(tdX + 1, tdY + 2 + j);
	cout << "Ma DG";
	gotoXY(tdX + 12 + 1, tdY + 2 + j);
	cout << "Ho & Ten";
	gotoXY(tdX + 42 + 1, tdY + 2 + j);
	cout <<"Ma Sach";
	gotoXY(tdX + 57 + 1, tdY + 2 + j);
	cout << "Ten Sach";
	gotoXY(tdX + 95 + 1, tdY + 2 + j);
	cout << "Ngay Muon";
	gotoXY(tdX + 108 + 1, tdY + 2 + j);
	cout << "So Ngay Qua Han";
	TextColor(7);
	for (int i = tam; i < tam1; i++)
	{
		gotoXY(tdX + 1, tdY + 4 + j);
		cout << ChuyenSoSangString(a[i].MaThe);
		gotoXY(tdX + 10 + 1, tdY + 4 + j);
		cout << a[i].Ho + " " + a[i].Ten;
		gotoXY(tdX + 40 + 1, tdY + 4 + j);
		cout <<a[i].DoubleList_MuonTra.pHead->data.MaSach;
		gotoXY(tdX + 55 + 1, tdY + 4 + j);
		cout << LayTenSach(m,a[i].DoubleList_MuonTra.pHead->data.MaSach);
		gotoXY(tdX + 95 + 1, tdY + 4 + j);
		cout << a[i].DoubleList_MuonTra.pHead->data.NgayMuon.Ngay<<"/"<<a[i].DoubleList_MuonTra.pHead->data.NgayMuon.Thang<<"/"<<a[i].DoubleList_MuonTra.pHead->data.NgayMuon.Nam;
		gotoXY(tdX + 107 + 1, tdY + 4 + j);
		cout << (a[i].SoNgayQuaHanMax);
		j = j + 2;
	}
	TextColor(7);
}

void XuatDanhSachDocGiaQuaHan(TheDocGia a[],MCONTRO m, int n, int tdX, int tdY)
{
	VeHCN(30,5,35,3,3);
	gotoXY(35,6);
	TextColor(210);
	cout<<"DANH SACH DOC GIA QUA HAN";
	TextColor(7);
	int page = 1;
	int tam = 0;
	int tam1 = 10;
	if (tam1 > n)
	{
		tam1 = n;
	}
	if(n==0){
		gotoXY(30,25);
		cout<<"KHONG CO DOC GIA NAO QUA HAN";
		gotoXY(30,40);
		system("pause");
		return;
	}

	GiaoDienXuatDGQuaHan();
	XuatDanhSachDocGiaQuaHanBangMang(a,m, tam, tam1, tdX, tdY);
	XuatPage(page);
	char c;
	while (true)
	{
		if (kbhit())
		{
			c = getch();
			if (c == -32)
			{
				c = getch();
				if (c == 75)
				{
					if (tam != 0)
					{
						tam = tam - 10;
						if (tam < 0)
						{
							tam = 0;
						}
						tam1 = tam + 10;
						if (tam1 > n)
						{
							tam1 = n;
						}
						page--;
						XoaManHinh(tdX, tdY+4, 101, 20);
						XuatDanhSachDocGiaQuaHanBangMang(a,m, tam, tam1, tdX, tdY);
						XuatPage(page);
					}
				}
				else if ( c== 77)
				{
					if (tam1 < n)
					{
						tam = tam + 10;
						tam1 = tam1 + 10;
						if (tam1 > n)
						{
							tam1 = n;
						}
						page++;
						XoaManHinh(tdX, tdY + 4, 101, 20);
						XuatDanhSachDocGiaQuaHanBangMang(a,m, tam, tam1, tdX, tdY);
						XuatPage(page);
					}
				}
				
			}
			else if (c == 27)
			{
				return;
			}
		}
	}
}

void DGQuaHanSangMang(TREE t, TheDocGia a[], int &n)
{
	if (t == NULL)
	{
		return;
	}
	else
	{
		DGQuaHanSangMang(t->pLeft, a, n);
		if (t->data.SoNgayQuaHanMax > 0)
		{
			a[n++] = t->data;
		}
		DGQuaHanSangMang(t->pRight, a, n);
	}
}

void XuatDSDocGiaMuonSachQuaHan(TREE t,MCONTRO m, int toadoX, int toadoY)
{
	int n = 0;
	TheDocGia* a = new TheDocGia[SoLuongDocGia];
	TheDocGia tam;
	DGQuaHanSangMang(t, a, n);

	for (int i = 0; i < n - 1; i++)
	{
		for (int j = i; j < n; j++)
		{
			if (a[i].SoNgayQuaHanMax < a[j].SoNgayQuaHanMax)
			{
				tam = a[i];
				a[i] = a[j];
				a[j] = tam;
			}
		}
	}
	XuatDanhSachDocGiaQuaHan(a,m, n, toadoX, toadoY);
}


//======================cau j ===================
void GiaoDien10DSMax(int tdX, int tdY)
{
	
	gotoXY(tdX + 4, tdY + 2);
	cout << "ISBN";
	gotoXY(tdX + 25, tdY + 2);
	cout << "Ten Sach";
	gotoXY(tdX + 57, tdY + 2);
	cout << "So Trang";
	gotoXY(tdX + 70, tdY + 2);
	cout << "Tac Gia";
	gotoXY(tdX + 93, tdY + 2);
	cout << "So Luot Muon";
	VeHCN(tdX,tdY+3,15,20,2);
	VeHCN(tdX+15,tdY+3,41,20,2);
	VeHCN(tdX+56,tdY+3,10,20,2);
	VeHCN(tdX+66,tdY+3,26,20,2);
	VeHCN(tdX+92,tdY+3,15,20,2);
}

bool TimMax(DauSach max[], string str, int n)
{
	for (int i = n - 1; i >= 0; i--)
	{
		if (max[i].ISBN == str)
		{
			return false;
		}
	}
	return true;
}
void Xuat10DauSach(DauSach m[],int tdX,int tdY)
{
	for (int i = 0; i < 10; i++)
	{
		if(m[i].SoTrang!=0){
			gotoXY(tdX + 1, tdY + 4 + i * 2);
			cout << m[i].ISBN;
			gotoXY(tdX + 16, tdY + 4 + i * 2);
			cout << m[i].TenSach;
			gotoXY(tdX + 57, tdY + 4 + i * 2);
			cout << ChuyenSoSangString(m[i].SoTrang);
			gotoXY(tdX + 67, tdY + 4 + i * 2);
			cout << m[i].TacGia;
			gotoXY(tdX + 93, tdY + 4 + i * 2);
			cout << ChuyenSoSangString(m[i].SoLanMuon);
		}
	}
	TextColor(7);
	gotoXY(100, 35);
	system("pause");
}
void Xuat10SachCoSoLuongMuonMax(MCONTRO &m, int tdX, int tdY)
{
	TextColor(9);
	gotoXY(35,5);
	cout<<"10 DAU SACH CO LUOT MUON NHIEU NHAT";
	TextColor(7);
	GiaoDien10DSMax(tdX,tdY);
	SapXepDauSachTheoTenSach(m);
	static DauSach MaxDauS[10];
	for (int h = 0; h < 10; h++)
	{
		for (int i = 0; i < m.SoLuongDauS ; i++)
		{
			if (MaxDauS[h].SoLanMuon < m.nodesDauSach[i]->SoLanMuon && TimMax(MaxDauS, m.nodesDauSach[i]->ISBN, h))
			{
				MaxDauS[h] = *m.nodesDauSach[i];
			}
		}
	}	
	Xuat10DauSach(MaxDauS, tdX, tdY);
}

//=========================Giai phong bo nho//=================
void XoaDauList(LIST_DMS& l)
{
	if (l.pHead == NULL)
	{
		return;
	}
	Node_Sach* t = l.pHead;
	l.pHead = l.pHead->pNext;
	delete t;
}
void XoaDauDoubleList(DOUBLELIST& l)
{
	if (l.pHead == NULL)
	{
		return;
	}
	else
	{
		NODE_DOUBLELIST* t = l.pHead;
		if (l.pHead->pNext == NULL)
		{
			l.pHead = NULL;
		}
		else
		{

			l.pHead = l.pHead->pNext;
			l.pHead->pPrev = NULL;
		}
		delete t;
	}
}
void DeleteCay(TREE& t)
{
	if (t == NULL)
	{
		return;
	}
	else
	{
		DeleteCay(t->pLeft);
		DeleteCay(t->pRight);
		// xoa danh sach muon tra
		while (t->data.DoubleList_MuonTra.pHead != NULL)
		{
			XoaDauDoubleList(t->data.DoubleList_MuonTra);
		}
		delete t;
	}
}
void DeleteBoNho(TREE& t, MCONTRO& DauS)
{
	delete[]MangRandom;
	//Xoa Doc Gia
	DeleteCay(t);
	// Xoa Danh Muc Sach
	for (int i = 0; i < DauS.SoLuongDauS; i++)
	{
		while (DauS.nodesDauSach[i]->List_DMS.pHead != NULL)
		{
			XoaDauList(DauS.nodesDauSach[i]->List_DMS);
		}
		// Xoa Dau Sach
		delete DauS.nodesDauSach[i];
	}
}
//==========================================================


void VeGiaoDienMenu(){
	TextColor(10);
	VeHCN(81,8,19,4,6);
	gotoXY(87,10);
	TextColor(230);
	cout<<"THU VIEN";
	TextColor(10);	
	VeHCN(140,3,35,9,9);
	TextColor(220);
	gotoXY(149,5);
	cout<<"THONG TIN";
	gotoXY(142,7);
	TextColor(8);
	cout<<"Ho&ten sinh vien: VO ANH TUAN";
	gotoXY(142,9);
	cout<<"MSSV: N17DCCN162";
	gotoXY(142,11);
	cout<<"GV huong dan: LUU NGUYEN KY THU";
	TextColor(10);
	VeHCN(10,5,30,12,4);
	gotoXY(20,7);
	cout<<"HUONG DAN";
	gotoXY(12,9);
	cout<<"ENTER: LUA CHON CHUC NANG";
	gotoXY(12,11);
	cout<<"ESC: LUU FILE";
	gotoXY(12,13);
	cout<<"UP: CHON LUA CHON TREN";	
	gotoXY(12,15);
	cout<<"DOWN: CHON LUA CHON DUOI";	
	VeHCN(5,2,177,38,15);
	VeHCN(72,14,39,15,300);
}




////////Menu /////////////////
void menu(str MenuMain[14], int n, TREE t, MCONTRO m){
	int ttac = 0;
	int *mau = new int[n];
	int cot1 = 73;
	int dong1 = 10+5;
	for(int i=0; i>n; i++){
		mau[i]= Mauchu;
	}

	while(1)
	{
		clrscr();
		VeGiaoDienMenu();
		for(int i=0; i<n; i++){
			
			TextColor(mau[i]);
			gotoXY(cot1, dong1+i);
			cout<<MenuMain[i]<<endl;		
		}
		int z = _getch();
		TRANGTHAI trangthai = key(z);
		switch(trangthai)
		{
		case UP:
		{
				if(ttac == 0)
				{
						ttac = n-1;
				}
				else 
				   ttac--;	
				break;
		}
		case DOWN:
		{
				if(ttac==n-1)
				{
						ttac = 0;
				}
				else
		  		  ttac++;
				break;
		}
		case ENTER:
		{
			switch(ttac+1)
			{
				case 1:
					{
						int a=35;
						int b=12;
						system("cls");
						system("color 3");
						TextColor(5);
						TheDocGia x;
						ThemTheDocGia(t,x);
						break;
					}
				case 2:
					{
						int a=35;
						int b=12;
						TextColor(5);
						system("cls");
						XoaDocGia(t,m,a,b);
						system("pause");
						break;
					}
				case 3:
					{
						int a=35;
						int b=19;
						TextColor(5);
						system("cls");
						SuaDocGia(t, a, b);					
						break;
					}
				case 4:
					{
						int a=35;
						int b=12;
						TextColor(5);
						system("cls");
						MoVaKhoaTheDocGia(t,m,a,b);
						system("pause");
						break;
					}
				case 5:
					{
						int a=20;
						int b=10;
						TextColor(5);
						system("cls");
						XuatDanhSachDocGia(t,a,b);
						system("pause");
						break;
					}
				case 6:
					{
						int a=20;
						int b=10;
						system("cls");
						system("color 3");
						ThemMotDauSach(m,a,b);
						break;
					}
				case 7:
					{
						int a=10;
						int b=10;
						system("cls");
						system("color 3");
						XuatTheoTheLoai(m, a,b);
						break;
					}
				case 8:
					{
						int a=10;
						int b=10;
						system("cls");
						system("color 3");
						TimTTSachDuaVaoTenSach(m,a,b);
						break;
						
					}
				case 9:
					{
						int a=10;
						int b=10;
						system("cls");
						system("color 3");
						MuonSach(t,m,a,b);
						gotoXY(0,42);
						system("pause");
						break;
					}
				case 10:
					{
						int a=20;
						int b=10;
						system("cls");
						system("color 3");
						XuatDanhSachCacSachDangMuon(m,t,a,b);
						gotoXY(0,42);
						system("pause");
						break;
					}
				case 11:
					{
						int a=10;
						int b=14;
						system("cls");
						system("color 3");
						TraSach(t,m,a,b);
						gotoXY(0,42);
						system("pause");
						break;
					}
				case 12:
					{
						int a=20;
						int b=10;
						system("cls");
						system("color 3");
						XuatDSDocGiaMuonSachQuaHan(t,m,a,b);
						gotoXY(0,42);
						system("pause");
						break;
					}
				case 13:
					{
						int a=10;
						int b=7;
						system("cls");
						system("color 3");
						Xuat10SachCoSoLuongMuonMax(m,a,b);
						gotoXY(0,42);
						break;
					}						
				case 14:
					{
						DeleteBoNho(t,m);
						gotoXY(0,40);
						return;
					}					
			}
		}
		case BACK:
			{
				
				ofstream fileout;
				ofstream fileout1;
				fileout.open("ThemDocGia.txt", ios_base::out);
				fileout1.open("MuonTra.txt", ios_base::out);
				SaveDanhSachDocGiavaMuonTra(t,fileout,fileout1);
				fileout.close();
				fileout1.close();
				SaveDanhSachDauSach(m);
				Savefile_DMS(m);
			}
		}
		
		for(int i=0; i<n; i++)
			mau[i] = Mauchu;
		mau[ttac]=Maunen;
	}	
}

int main(){	
	TREE t;
	t=NULL;
	MCONTRO m;
	Date NgayHT;
	LoadFileTheDocGia(t);
	LoadFileDauSach(m);
	LoadFileDMS(m);
	LoadFileDanhSachSachMuonTra(t,m);
	LayNgayHienTai(NgayHT);
	KiemTraQuaHan(t, NgayHT);
	menu(MenuMain, 14, t,m);
	return 0;
}
