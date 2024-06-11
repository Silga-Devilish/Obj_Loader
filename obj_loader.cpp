#include"obj_loader.h"




// �������������ڴ��ַ����зָת������  
bool OBJ::parse_To_Vector(const string& line, vector<double>& vec) {
    // ʹ�ÿո���Ϊ�ָ����ָ��ַ���  
    stringstream ss(line);
    string token;
    while (getline(ss, token, ' ')) {
        // ���ַ���ת��Ϊdouble����ӵ�������  
        char* endptr;
        double value = std::strtod(token.c_str(), &endptr);
        if (endptr == token.c_str()) {
            // ת��ʧ��  
            return false;
        }
        vec.push_back(value);
    }
    return true;
}

//��ȡ����
bool OBJ::readOBJFile(const std::string& filename, OBJ& obj) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file: " << filename << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        // �Ƴ�����β�Ŀո�  
        line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
        line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);

        // ���Կ��к�ע�ͣ���#��ͷ���У�  
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // �������㣨v��  
        if (line[0] == 'v' && line[1] == ' ') {
            vector<double> vertexData;
            if (parse_To_Vector(line.substr(2), vertexData)) { // ȥ����ͷ�� 'v ' ���ٽ���
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
        // �����������꣨vt��
        else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ') {
            vector<double> texCoordData;
            if (parse_To_Vector(line.substr(3), texCoordData)) { // ȥ����ͷ�� 'vt ' ���ٽ���
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
        // ������������vn��
        else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ') {
            vector<double> normalData;
            if (parse_To_Vector(line.substr(3), normalData)) { // ȥ����ͷ�� 'vn ' ���ٽ���
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
        // �����棨f��  
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
                // ������������  
                char* endptr;
                face.V[i] = std::strtol(token.c_str(), &endptr, 10) - 1;
                if (endptr == token.c_str()) {
                    cerr << "Failed to parse vertex index in face: " << line << endl;
                    return false;
                }

                // ������������������������ڣ�  
                if (getline(iss, token, '/')) 
                {
                    face.T[i] = std::strtol(token.c_str(), &endptr, 10) - 1;
                    if (endptr == token.c_str()) {
                        cerr << "Failed to parse texture coordinate index in face: " << line << endl;
                        // ע�⣺�������ѡ����Դ���򷵻�false��ȡ�����������  
                        face.T[i] = -1; // ���û���������꣬����Ϊ-1������Ĭ��ֵ  
                    }
                }
                else 
                {
                    face.T[i] = -1; // ���û��ָ���������꣬������Ϊ-1  
                }

                // ����������������������ڣ�  
                if (getline(iss, token)) {
                    face.N[i] = std::strtol(token.c_str(), &endptr, 10) - 1;
                    if (endptr == token.c_str()) {
                        cerr << "Failed to parse normal index in face: " << line << endl;
                        // ע�⣺�������ѡ����Դ���򷵻�false��ȡ�����������  
                        face.N[i] = -1; // ���û�з�����������Ϊ-1������Ĭ��ֵ  
                    }
                }
                else {
                    face.N[i] = -1; // ���û��ָ����������������Ϊ-1  
                }
            }
            obj.F.push_back(face);
        }
    }
    // �ڶ�ȡ�������ݺ󣬵��ü��㶥�㷨�����ĺ���
    calculateVertexNormals(obj);
    return true;
}

// ���㶥�㷨����
void OBJ::calculateVertexNormals(OBJ& obj) 
{
    // ��ʼ�����㷨��������������ÿ������ĳ�ʼ������Ϊ������
    vector<OBJ_NORMAL> vertexNormals(obj.V.size(), OBJ_NORMAL{ 0.0, 0.0, 0.0 });

    // ����ÿ���棬�ۻ�������
    for (const auto& face : obj.F) {
        // ��ȡ��ķ��������˴��򻯴���ֱ��ʹ����ĵ�һ������ķ���������
        // ʵ���У���ķ�����Ӧ���Ǹ��ݶ���˳�����ó���ƽ�����������������ÿ��������ȷ�ķ���������
        int nIndex = face.N[0];
        if (nIndex != -1) { // ȷ������Ч�ķ���������
            const OBJ_NORMAL& faceNormal = obj.VN[nIndex];

            // �ۻ��������������ж���ķ�����
            for (int i = 0; i < 3; ++i) {
                vertexNormals[face.V[i]] += faceNormal;
            }
        }
    }

    // ƽ�������㷨��������λ��
    for (OBJ_NORMAL& normal : vertexNormals) 
    {
        // ��ֹ����0�����������û�в����κ���Ķ���
        if (normal.NX != 0.0 || normal.NY != 0.0 || normal.NZ != 0.0) {
            // ƽ�������˴��򻯴���ʵ��Ӧ���ݶ��������������У�
            normal.normalize(); // ֱ�ӵ�λ��������ÿ�����㷨��������ȷ�ۻ��������������
        }
    }

    // �滻����ӵ�ԭ�еĶ��㷨��������
    obj.VN = vertexNormals;
}

// ������������ӡOBJ_Point�ṹ��  
void OBJ::printPoint(const OBJ_Point& p) {
    std::cout << "V " << p.X << " " << p.Y << " " << p.Z << std::endl;
}

// ������������ӡOBJ_TEXTURE�ṹ��  
void OBJ::printTexture(const OBJ_TEXTURE& t) {
    std::cout << "VT " << t.TU << " " << t.TV << std::endl;
}

// ������������ӡOBJ_NORMAL�ṹ��  
void OBJ::printNormal(const OBJ_NORMAL& n) {
    std::cout << "VN " << n.NX << " " << n.NY << " " << n.NZ << std::endl;
}

// ������������ӡOBJ_FACE�ṹ��  
void OBJ::printFace(const OBJ_FACE& f) {
    std::cout << "f ";
    for (int i = 0; i < 3; ++i) {
        std::cout << f.V[i] + 1 << "/" << f.T[i] + 1 << "/" << f.N[i] + 1 << " "; // ͨ��OBJ������1��ʼ���������+1  
    }
    std::cout << std::endl;
}

// ��ӡOBJ�ļ���������Ϣ�ĺ���  
void OBJ::printOBJ(const OBJ& obj) {
    // ��ӡ����  
    std::cout << "Vertices:" << std::endl;
    for (const auto& p : obj.V) {
        printPoint(p);
    }

    // ��ӡ��������  
    std::cout << "Texture Coordinates:" << std::endl;
    for (const auto& t : obj.VC) {
        printTexture(t);
    }

    // ��ӡ������  
    std::cout << "Normals:" << std::endl;
    for (const auto& n : obj.VN) {
        printNormal(n);
    }

    // ��ӡ��  
    std::cout << "Faces:" << std::endl;
    for (const auto& f : obj.F) {
        printFace(f);
    }
}


/////////////////////////////////struct�ĺ���///////////////////////




// ����������������λ��
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
