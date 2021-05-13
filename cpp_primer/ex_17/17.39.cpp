#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
	string location("E:/code/c++/cppprimerTest.txt");
	fstream inOut(location, fstream::ate | fstream::in | fstream::out); //ÿ�δ��ļ��Ͷ�λ���ļ�β�����ļ����ж���д����

	if (!inOut)
	{
		cerr << "Unable to open file!" << endl;
		return EXIT_FAILURE;
	}
	
	streampos end_mark = inOut.tellg();	//�����ļ�βλ��
	inOut.seekg(0, fstream::end);		//��λ���ļ�ĩβ
	inOut << "\n";						//���ļ�ĩβ���ϻ��з�
	inOut.seekg(0, fstream::beg);		
	size_t cnt = 0;						//�ֽ����ۼ���
	string line;						//���������е�ÿ��


	while (inOut && inOut.tellg() != end_mark && getline(inOut, line))
	{
		cnt += line.size() + 1;	//����line�ĳ��ȣ� +1��ʾ�������з�
		auto mark = inOut.tellg();
		inOut.seekg(0, fstream::end);
		inOut << cnt;
		if (mark != end_mark)
		{
			inOut << " ";
		}
		inOut.seekg(mark);
	}
	
	//inOut.seekg(0, fstream::end); 
	//inOut << "\n";	
	return 0;
}