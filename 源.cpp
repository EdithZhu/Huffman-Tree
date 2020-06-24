#include<iostream>
#include<iomanip>
#include<string>
using namespace std;
struct HNode //哈夫曼树结点结构
{
	int weight; //结点权值
	int parent; //双亲指针
	int LChild; //左孩子指针
	int RChild; //右孩子指针
};
struct HCode //编码表结点结构
{
	char data;
	char code[100];
};
class Huffman
{
private:
	HNode* HTree; //哈夫曼树
	HCode* HCodeTable; //哈夫曼编码表
	char str[1024]; //输入原始字符串
public:
	int a[256]; //记录每个出现字符的个数
	char leaf[256]; //叶子结点对应的字符
	int n; //叶子结点数
	void init(); //初始化
	void CreateHTree(int a[],int n);  //创建哈夫曼树
	void SelectMin(int &x,int &y,int s,int e);
	void CreateCodeTable(char b[],int n); //创建编码表
	void Encode(char* d);  //编码
	void Decode(char* s,char* d,int n);  //解码
	void print(int i,int m); //打印哈夫曼树
	void Reverse(char c[]); //逆置字符
	~Huffman();
};
void Huffman::init() //字符读取、统计输入的字符串中字符的频率
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
//创建哈夫曼树
//输入参数a[]存储每种字符的权值，n为字符的种类
void Huffman::CreateHTree(int a[],int n)
{
	HTree=new HNode[2*n-1]; //根据权重数组a[0...n-1]初始化哈夫曼树
	for(int i=0;i<n;i++)
	{
		HTree[i].weight=a[i];
		HTree[i].LChild=HTree[i].RChild=HTree[i].parent=-1;
	}
	int x,y;
	for(int i=n;i<2*n-1;i++) //开始建立哈夫曼树
	{
		SelectMin(x,y,0,i);//从1~i中选出两个权值最小且parent值为-1的结点
		HTree[x].parent=HTree[y].parent=i;
		HTree[i].weight=HTree[x].weight+HTree[y].weight;
		HTree[i].LChild=x;
		HTree[i].RChild=y;
		HTree[i].parent=-1;
	}
}
void Huffman::SelectMin(int &x,int &y,int s,int e) //x为最小权值，y为次小权值，s为权值范围的起始下标，e为结束下标
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
//打印哈夫曼树
#define N 10 //定义树的最大深度
void Huffman::print(int i,int m)
/*使用凹入表的方式打印任意形状的哈夫曼树，二叉树前序遍历，
i表示哈夫曼树的下标为i的结点，m表示该结点的层次*/
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
//创建编码表
void Huffman::CreateCodeTable(char b[],int n)
{
	HCodeTable=new HCode[n];//生成编码表
	for(int i=0;i<n;i++)
	{
		HCodeTable[i].data=b[i];
		int child=i;  //孩子结点编号
		int parent=HTree[i].parent; //当前结点的父结点编号
		int k=0;
		while(parent!=-1)
		{
			if(child == HTree[parent].LChild) 
				HCodeTable[i].code[k]='0';
			else
				HCodeTable[i].code[k]='1';
			k++;
			child=parent;//迭代
			parent=HTree[child].parent;
		}
		HCodeTable[i].code[k]='\0';
		Reverse(HCodeTable[i].code);  //编码字符逆置
	}
	cout<<"每个字符的编码为："<<endl;
	for(int i=1;i<n;i++)
	{
		cout<<HCodeTable[i].data<<":"<<HCodeTable[i].code<<endl;
	}
}
//编码字符逆置
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
//编码
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
//解码
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
//测试数据 I love data Structure,I love Computer.I will try my best to study data Structure. 
void main()
{
	Huffman HFCode;
	cout<<"请输入要编码的字符串：";
	HFCode.init();
	
	cout<<"创建哈夫曼树："<<endl;
	HFCode.CreateHTree(HFCode.a,20);
	HFCode.print(2*HFCode.n-2,1);
	
	cout<<"创建哈夫曼编码表"<<endl;
	HFCode.CreateCodeTable(HFCode.leaf,20);
	
	char d[1024]={0};
	HFCode.Encode(d);
	cout<<"编码结果："<<d<<endl;
	
	char s[1024]={0};
	HFCode.Decode(d,s,20);
	cout<<"解码结果："<<s<<endl;
	
}

