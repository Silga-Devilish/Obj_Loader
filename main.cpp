#include <iostream>  
#include <sstream>  
#include <string>  
#include <vector>  

std::vector<int> parseFaceVertices(const std::string& line) 
{
    std::vector<int> vertices;
    std::istringstream iss(line.substr(2)); // ���� "f " ǰ׺  
    std::string token;

    while (iss >> token) { // ʹ��Ĭ�ϵĿո�ָ�����ȡtoken  
        size_t pos = token.find('/'); // ���ҵ�һ��'/'��λ��  
        if (pos != std::string::npos) {
            // ����ҵ���'/'����ֻ��ȡ��һ��'/'֮ǰ�Ĳ�����Ϊ������  
            token = token.substr(0, pos);
        }

        // ���Խ�tokenת��Ϊ��������ӵ�vertices��  
        try {
            int vertex = std::stoi(token);
            vertices.push_back(vertex);
        }
        catch (const std::invalid_argument& e) {
            // ת��ʧ�ܣ����磬token������Ч������������ӡ�����˳�ѭ��  
            std::cerr << "Failed to parse vertex: " << token << std::endl;
            break;
        }
        catch (const std::out_of_range& e) {
            // ����̫���޷�����int����ӡ�����˳�ѭ��  
            std::cerr << "Vertex value out of range: " << token << std::endl;
            break;
        }
    }

    return vertices;
}

int main() {
    std::string line1 = "f 77/2/1 362/44/2 20/1/3";
    std::string line2 = "f 77//1 362//2 20//3";
    std::string line3 = "f 77 362 20";

    std::vector<int> vertices1 = parseFaceVertices(line1);
    std::vector<int> vertices2 = parseFaceVertices(line2);
    std::vector<int> vertices3 = parseFaceVertices(line3);

    // ��ӡ���  
    for (int vertex : vertices1) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;

    for (int vertex : vertices2) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;

    for (int vertex : vertices3) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;

    return 0;
}