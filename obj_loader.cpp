#include"obj_loader.h"




// 辅助函数：用于从字符串中分割并转换数字  
bool OBJ::parse_To_Vector(const string& line, vector<double>& vec) {
    // 使用空格作为分隔符分割字符串  
    stringstream ss(line);
    string token;
    while (getline(ss, token, ' ')) {
        // 将字符串转换为double并添加到向量中  
        char* endptr;
        double value = std::strtod(token.c_str(), &endptr);
        if (endptr == token.c_str()) {
            // 转换失败  
            return false;
        }
        vec.push_back(value);
    }
    return true;
}

//读取函数
bool OBJ::readOBJFile(const std::string& filename, OBJ& obj) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file: " << filename << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        // 移除行首尾的空格  
        line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
        line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);

        // 忽略空行和注释（以#开头的行）  
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // 解析顶点（v）  
        if (line[0] == 'v' && line[1] == ' ') {
            vector<double> vertexData;
            if (parse_To_Vector(line.substr(2), vertexData)) { // 去掉开头的 'v ' 后再解析
                if (vertexData.size() >= 3) {
                    OBJ_Point vertex;
                    vertex.X = vertexData[0];
                    vertex.Y = vertexData[1];
                    vertex.Z = vertexData[2];
                    obj.V.push_back(vertex);
                }
                else {
                    cerr << "Invalid vertex data: " << line << endl;
                    return false;
                }
            }
            else {
                cerr << "Failed to parse vertex line: " << line << endl;
                return false;
            }
        }
        // 解析纹理坐标（vt）
        else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ') {
            vector<double> texCoordData;
            if (parse_To_Vector(line.substr(3), texCoordData)) { // 去掉开头的 'vt ' 后再解析
                if (texCoordData.size() >= 2) {
                    OBJ_TEXTURE texCoord;
                    texCoord.TU = texCoordData[0];
                    texCoord.TV = texCoordData[1];
                    obj.VC.push_back(texCoord);
                }
                else {
                    cerr << "Invalid texture coordinate data: " << line << endl;
                    return false;
                }
            }
            else {
                cerr << "Failed to parse texture coordinate line: " << line << endl;
                return false;
            }
        }
        // 解析法向量（vn）
        else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ') {
            vector<double> normalData;
            if (parse_To_Vector(line.substr(3), normalData)) { // 去掉开头的 'vn ' 后再解析
                if (normalData.size() >= 3) {
                    OBJ_NORMAL normal;
                    normal.NX = normalData[0];
                    normal.NY = normalData[1];
                    normal.NZ = normalData[2];
                    obj.VN.push_back(normal);
                }
                else {
                    cerr << "Invalid normal data: " << line << endl;
                    return false;
                }
            }
            else {
                cerr << "Failed to parse normal line: " << line << endl;
                return false;
            }
        }
        // 解析面（f）  
        else if (line[0] == 'f' && line[1] == ' ') 
        {
            OBJ_FACE face;
            istringstream iss(line.substr(2));
            string token;
            for (int i = 0; i < 3; ++i) 
            {
                if (!getline(iss, token, '/')) 
                {
                    cerr << "Failed to parse face: " << line << endl;
                    return false;
                }
                // 解析顶点索引  
                char* endptr;
                face.V[i] = std::strtol(token.c_str(), &endptr, 10) - 1;
                if (endptr == token.c_str()) {
                    cerr << "Failed to parse vertex index in face: " << line << endl;
                    return false;
                }

                // 解析纹理坐标索引（如果存在）  
                if (getline(iss, token, '/')) 
                {
                    face.T[i] = std::strtol(token.c_str(), &endptr, 10) - 1;
                    if (endptr == token.c_str()) {
                        cerr << "Failed to parse texture coordinate index in face: " << line << endl;
                        // 注意：这里可以选择忽略错误或返回false，取决于你的需求  
                        face.T[i] = -1; // 如果没有纹理坐标，设置为-1或其他默认值  
                    }
                }
                else 
                {
                    face.T[i] = -1; // 如果没有指定纹理坐标，则设置为-1  
                }

                // 解析法向量索引（如果存在）  
                if (getline(iss, token)) {
                    face.N[i] = std::strtol(token.c_str(), &endptr, 10) - 1;
                    if (endptr == token.c_str()) {
                        cerr << "Failed to parse normal index in face: " << line << endl;
                        // 注意：这里可以选择忽略错误或返回false，取决于你的需求  
                        face.N[i] = -1; // 如果没有法向量，设置为-1或其他默认值  
                    }
                }
                else {
                    face.N[i] = -1; // 如果没有指定法向量，则设置为-1  
                }
            }
            obj.F.push_back(face);
        }
    }
    // 在读取所有数据后，调用计算顶点法向量的函数
    calculateVertexNormals(obj);
    return true;
}

// 计算顶点法向量
void OBJ::calculateVertexNormals(OBJ& obj) 
{
    // 初始化顶点法向量容器，假设每个顶点的初始法向量为零向量
    vector<OBJ_NORMAL> vertexNormals(obj.V.size(), OBJ_NORMAL{ 0.0, 0.0, 0.0 });

    // 遍历每个面，累积法向量
    for (const auto& face : obj.F) {
        // 获取面的法向量，此处简化处理，直接使用面的第一个顶点的法向量索引
        // 实际中，面的法向量应该是根据顶点顺序计算得出的平均法向量，这里假设每个面有正确的法向量索引
        int nIndex = face.N[0];
        if (nIndex != -1) { // 确保有有效的法向量索引
            const OBJ_NORMAL& faceNormal = obj.VN[nIndex];

            // 累积到参与此面的所有顶点的法向量
            for (int i = 0; i < 3; ++i) {
                vertexNormals[face.V[i]] += faceNormal;
            }
        }
    }

    // 平均化顶点法向量并单位化
    for (OBJ_NORMAL& normal : vertexNormals) 
    {
        // 防止除以0的情况，跳过没有参与任何面的顶点
        if (normal.NX != 0.0 || normal.NY != 0.0 || normal.NZ != 0.0) {
            // 平均化（此处简化处理，实际应根据顶点参与的面数进行）
            normal.normalize(); // 直接单位化，假设每个顶点法向量已正确累积，无需除以面数
        }
    }

    // 替换或添加到原有的顶点法向量容器
    obj.VN = vertexNormals;
}

// 辅助函数：打印OBJ_Point结构体  
void OBJ::printPoint(const OBJ_Point& p) {
    std::cout << "V " << p.X << " " << p.Y << " " << p.Z << std::endl;
}

// 辅助函数：打印OBJ_TEXTURE结构体  
void OBJ::printTexture(const OBJ_TEXTURE& t) {
    std::cout << "VT " << t.TU << " " << t.TV << std::endl;
}

// 辅助函数：打印OBJ_NORMAL结构体  
void OBJ::printNormal(const OBJ_NORMAL& n) {
    std::cout << "VN " << n.NX << " " << n.NY << " " << n.NZ << std::endl;
}

// 辅助函数：打印OBJ_FACE结构体  
void OBJ::printFace(const OBJ_FACE& f) {
    std::cout << "f ";
    for (int i = 0; i < 3; ++i) {
        std::cout << f.V[i] + 1 << "/" << f.T[i] + 1 << "/" << f.N[i] + 1 << " "; // 通常OBJ索引从1开始，这里假设+1  
    }
    std::cout << std::endl;
}

// 打印OBJ文件内所有信息的函数  
void OBJ::printOBJ(const OBJ& obj) {
    // 打印顶点  
    std::cout << "Vertices:" << std::endl;
    for (const auto& p : obj.V) {
        printPoint(p);
    }

    // 打印纹理坐标  
    std::cout << "Texture Coordinates:" << std::endl;
    for (const auto& t : obj.VC) {
        printTexture(t);
    }

    // 打印法向量  
    std::cout << "Normals:" << std::endl;
    for (const auto& n : obj.VN) {
        printNormal(n);
    }

    // 打印面  
    std::cout << "Faces:" << std::endl;
    for (const auto& f : obj.F) {
        printFace(f);
    }
}


/////////////////////////////////struct的函数///////////////////////




// 辅助函数：向量单位化
void OBJ_NORMAL::normalize() 
{
    double length = sqrt(NX * NX + NY * NY + NZ * NZ);
    if (length > 0.0) {
        NX /= length;
        NY /= length;
        NZ /= length;
    }
}
OBJ_NORMAL OBJ_NORMAL::operator+=(const OBJ_NORMAL& other)
{
    NX += other.NX;
    NY += other.NY;
    NZ += other.NZ;
    return *this;
}
