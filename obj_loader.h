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

	// 辅助函数：向量单位化
	void normalize();
	//打印
	/*void print() const;*/
	//重载+=
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
	vector<OBJ_Point> V;//V：代表顶点。格式为V X Y Z，V后面的X Y Z表示三个顶点坐标。浮点型
	vector<OBJ_TEXTURE>  VC;//表示纹理坐标。格式为VT TU TV。浮点型
	vector<OBJ_NORMAL> VN;//VN：法向量。每个三角形的三个顶点都要指定一个法向量。格式为VN NX NY NZ。浮点型
	vector<OBJ_FACE> F;//F：面。面后面跟着的整型值分别是属于这个面的顶点、纹理坐标、法向量的索引。
	//面的格式为：f Vertex1/Texture1/Normal1 Vertex2/Texture2/Normal2 Vertex3/Texture3/Normal3


	// 辅助函数：用于从字符串中分割并转换数字  
	bool parse_To_Vector(const string& line, vector<double>& vec);
	// 读取OBJ文件的函数  
	bool readOBJFile(const std::string& filename, OBJ& obj);
	// 计算顶点法向量
	void calculateVertexNormals(OBJ& obj);
	// 打印文件信息
	void printAllInfo() const;



	// 辅助函数：打印OBJ_Point结构体  
	void printPoint(const OBJ_Point& p);

	// 辅助函数：打印OBJ_TEXTURE结构体  
	void printTexture(const OBJ_TEXTURE& t);

	// 辅助函数：打印OBJ_NORMAL结构体  
	void printNormal(const OBJ_NORMAL& n);

	// 辅助函数：打印OBJ_FACE结构体  
    void printFace(const OBJ_FACE& f);

    // 打印OBJ文件内所有信息的函数  
    void printOBJ(const OBJ& obj);
};