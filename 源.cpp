#include<iostream>
#include<iomanip>
#include<string>
using namespace std;
struct HNode //�����������ṹ
{
	int weight; //���Ȩֵ
	int parent; //˫��ָ��
	int LChild; //����ָ��
	int RChild; //�Һ���ָ��
};
struct HCode //�������ṹ
{
	char data;
	char code[100];
};
class Huffman
{
private:
	HNode* HTree; //��������
	HCode* HCodeTable; //�����������
	char str[1024]; //����ԭʼ�ַ���
public:
	int a[256]; //��¼ÿ�������ַ��ĸ���
	char leaf[256]; //Ҷ�ӽ���Ӧ���ַ�
	int n; //Ҷ�ӽ����
	void init(); //��ʼ��
	void CreateHTree(int a[],int n);  //������������
	void SelectMin(int &x,int &y,int s,int e);
	void CreateCodeTable(char b[],int n); //���������
	void Encode(char* d);  //����
	void Decode(char* s,char* d,int n);  //����
	void print(int i,int m); //��ӡ��������
	void Reverse(char c[]); //�����ַ�
	~Huffman();
};
void Huffman::init() //�ַ���ȡ��ͳ��������ַ������ַ���Ƶ��
{
	int nNum[256]={0}; 
	int ch=cin.get(); 
	int i=0;
	while((ch!='\r')&&(ch!='\n')) 
	{
		nNum[ch]++;
		str[i++]=ch;
		ch=cin.get();
	}
	str[i]='\0';
	n=0;
	for(i=0;i<256;i++)
	{
		if(nNum[i]>0) 
		{
			leaf[n]=(char)i;
			a[n]=nNum[i];
			n++;
		}
	}
}
//������������
//�������a[]�洢ÿ���ַ���Ȩֵ��nΪ�ַ�������
void Huffman::CreateHTree(int a[],int n)
{
	HTree=new HNode[2*n-1]; //����Ȩ������a[0...n-1]��ʼ����������
	for(int i=0;i<n;i++)
	{
		HTree[i].weight=a[i];
		HTree[i].LChild=HTree[i].RChild=HTree[i].parent=-1;
	}
	int x,y;
	for(int i=n;i<2*n-1;i++) //��ʼ������������
	{
		SelectMin(x,y,0,i);//��1~i��ѡ������Ȩֵ��С��parentֵΪ-1�Ľ��
		HTree[x].parent=HTree[y].parent=i;
		HTree[i].weight=HTree[x].weight+HTree[y].weight;
		HTree[i].LChild=x;
		HTree[i].RChild=y;
		HTree[i].parent=-1;
	}
}
void Huffman::SelectMin(int &x,int &y,int s,int e) //xΪ��СȨֵ��yΪ��СȨֵ��sΪȨֵ��Χ����ʼ�±꣬eΪ�����±�
{
	int i;
	for(i=s;i<=e;i++)
		if(HTree[i].parent==-1)
		{
			x=y=i;
			break;
		}
		for(i=s;i<e;i++)
			if(HTree[i].parent==-1)
			{
			if(HTree[i].weight<HTree[x].weight)
			{
				y=x;
				x=i;
			}
			else if((x==y)||(HTree[i].weight<HTree[y].weight))
				y=i;
			}
}
//��ӡ��������
#define N 10 //��������������
void Huffman::print(int i,int m)
/*ʹ�ð����ķ�ʽ��ӡ������״�Ĺ���������������ǰ�������
i��ʾ�����������±�Ϊi�Ľ�㣬m��ʾ�ý��Ĳ��*/
{
	if(HTree[i].LChild==-1)
		cout<<setfill(' ')<<setw(m+1)<<leaf[i]<<setfill('-')<<setw(N-m)<<'\n';
	
	else
	{
		cout<<setfill(' ')<<setw(m+1)<<HTree[i].weight<<setfill('-')<<setw(N-m)<<'\n';
		print(HTree[i].LChild,m+1);
		print(HTree[i].RChild,m+1);
	}
}
//���������
void Huffman::CreateCodeTable(char b[],int n)
{
	HCodeTable=new HCode[n];//���ɱ����
	for(int i=0;i<n;i++)
	{
		HCodeTable[i].data=b[i];
		int child=i;  //���ӽ����
		int parent=HTree[i].parent; //��ǰ���ĸ������
		int k=0;
		while(parent!=-1)
		{
			if(child == HTree[parent].LChild) 
				HCodeTable[i].code[k]='0';
			else
				HCodeTable[i].code[k]='1';
			k++;
			child=parent;//����
			parent=HTree[child].parent;
		}
		HCodeTable[i].code[k]='\0';
		Reverse(HCodeTable[i].code);  //�����ַ�����
	}
	cout<<"ÿ���ַ��ı���Ϊ��"<<endl;
	for(int i=1;i<n;i++)
	{
		cout<<HCodeTable[i].data<<":"<<HCodeTable[i].code<<endl;
	}
}
//�����ַ�����
void Huffman::Reverse(char c[])
{
	int n=0;
	char temp;
	while(c[n+1]!='\0')
	{
		n++;
	}
	for (int i=0;i<=n/2;i++)
	{
		temp=c[i];
		c[i]=c[n-i];
		c[n-i]=temp;
	}
}
//����
void Huffman::Encode(char* d)
{
	char* s=str;
	while(*s!='\0')
	{
		for(int i=0;i<n;i++)
			if(*s==HCodeTable[i].data)
			{
				strcat(d,HCodeTable[i].code);
				break;
			}
			s++;
	}
}
//����
void Huffman::Decode(char* s,char* d,int n)
{
	while(*s!='\0')
	{
		int parent=2*n-2;
		while (HTree[parent].LChild!=-1)
		{
			if(*s=='0')
				parent=HTree[parent].LChild;
			else
				parent=HTree[parent].RChild;
			s++;
		}
		*d=HCodeTable[parent].data;
		d++;
	}
}
Huffman::~Huffman()
{
	delete []HTree;
	delete []HCodeTable;
}
//�������� I love data Structure,I love Computer.I will try my best to study data Structure. 
void main()
{
	Huffman HFCode;
	cout<<"������Ҫ������ַ�����";
	HFCode.init();
	
	cout<<"��������������"<<endl;
	HFCode.CreateHTree(HFCode.a,20);
	HFCode.print(2*HFCode.n-2,1);
	
	cout<<"���������������"<<endl;
	HFCode.CreateCodeTable(HFCode.leaf,20);
	
	char d[1024]={0};
	HFCode.Encode(d);
	cout<<"��������"<<d<<endl;
	
	char s[1024]={0};
	HFCode.Decode(d,s,20);
	cout<<"��������"<<s<<endl;
	
}

