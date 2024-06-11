#include <iostream>  
#include <sstream>  
#include <string>  
#include <vector>  

std::vector<int> parseFaceVertices(const std::string& line) 
{
    std::vector<int> vertices;
    std::istringstream iss(line.substr(2)); // 跳过 "f " 前缀  
    std::string token;

    while (iss >> token) { // 使用默认的空格分隔符读取token  
        size_t pos = token.find('/'); // 查找第一个'/'的位置  
        if (pos != std::string::npos) {
            // 如果找到了'/'，则只提取第一个'/'之前的部分作为顶点编号  
            token = token.substr(0, pos);
        }

        // 尝试将token转换为整数并添加到vertices中  
        try {
            int vertex = std::stoi(token);
            vertices.push_back(vertex);
        }
        catch (const std::invalid_argument& e) {
            // 转换失败（例如，token不是有效的整数），打印错误并退出循环  
            std::cerr << "Failed to parse vertex: " << token << std::endl;
            break;
        }
        catch (const std::out_of_range& e) {
            // 整数太大，无法放入int，打印错误并退出循环  
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

    // 打印结果  
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