#pragma once
#include <iostream>  
#include <fstream>  
#include <sstream>  
#include <vector>  
#include <string>  


using namespace std;
struct OBJ_Point
{
	double X;
	double Y;
	double Z;

	/*void print() const;*/
};
struct OBJ_TEXTURE 
{
	double TU;
	double TV;

	/*void print() const;*/
};
struct OBJ_NORMAL
{
	double NX;
	double NY;
	double NZ;

	// ����������������λ��
	void normalize();
	//��ӡ
	/*void print() const;*/
	//����+=
	OBJ_NORMAL operator+=(const OBJ_NORMAL& other);
	
};
struct OBJ_FACE 
{
	int V[3];
	int T[3];
	int N[3];

	/*void print(const OBJ& obj) const;*/
};
class OBJ
{
public:
	vector<OBJ_Point> V;//V�������㡣��ʽΪV X Y Z��V�����X Y Z��ʾ�����������ꡣ������
	vector<OBJ_TEXTURE>  VC;//��ʾ�������ꡣ��ʽΪVT TU TV��������
	vector<OBJ_NORMAL> VN;//VN����������ÿ�������ε��������㶼Ҫָ��һ������������ʽΪVN NX NY NZ��������
	vector<OBJ_FACE> F;//F���档�������ŵ�����ֵ�ֱ������������Ķ��㡢�������ꡢ��������������
	//��ĸ�ʽΪ��f Vertex1/Texture1/Normal1 Vertex2/Texture2/Normal2 Vertex3/Texture3/Normal3


	// �������������ڴ��ַ����зָת������  
	bool parse_To_Vector(const string& line, vector<double>& vec);
	// ��ȡOBJ�ļ��ĺ���  
	bool readOBJFile(const std::string& filename, OBJ& obj);
	// ���㶥�㷨����
	void calculateVertexNormals(OBJ& obj);
	// ��ӡ�ļ���Ϣ
	void printAllInfo() const;



	// ������������ӡOBJ_Point�ṹ��  
	void printPoint(const OBJ_Point& p);

	// ������������ӡOBJ_TEXTURE�ṹ��  
	void printTexture(const OBJ_TEXTURE& t);

	// ������������ӡOBJ_NORMAL�ṹ��  
	void printNormal(const OBJ_NORMAL& n);

	// ������������ӡOBJ_FACE�ṹ��  
    void printFace(const OBJ_FACE& f);

    // ��ӡOBJ�ļ���������Ϣ�ĺ���  
    void printOBJ(const OBJ& obj);
};