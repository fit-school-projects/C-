#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
#endif /* __PROGTEST__ */

const int SIZE_FIB = 30;
const int FIBONACCI_NUMBERS [] = {
        1, 2, 3, 5, 8, 13, 21, 34, 55, 89,
        144, 233, 377, 610, 987, 1597, 2584, 4181,
        6765, 10946, 17711, 28657, 46368, 75025, 121393,
        196418, 317811, 514229, 832040, 1346269, 2178309
};

struct TBuffer{
    uint8_t byte;
    size_t size;
};

bool decodeUtf8Char(ifstream & m_InputFile, uint32_t & value){
    value = 0;
    int numOfBytes;
    uint8_t tmp;
    m_InputFile.read(reinterpret_cast<char*>(&tmp), 1);
    // check if next byte is valid
    if (m_InputFile.eof()){
        return true;
    }
    if (m_InputFile.fail()){
        return false;
    }
    if (!(tmp & 0x80)){
        value = tmp & 0x7f;
        return true;
    }
    if ((tmp & 0xe0) == 0xc0){
        numOfBytes = 1;
        value = tmp & 0x1f;
    } else if ((tmp & 0xf0) == 0xe0){
        numOfBytes = 2;
        value = tmp & 0x0f;
    } else if ((tmp & 0xf8) == 0xf0){
        numOfBytes = 3;
        value = tmp & 0x07;
    } else {
        return false;
    }
    for (int i = 0; i < numOfBytes; i++){
        // check if next byte is valid
        if (m_InputFile.eof()){
            return false;
        }
        m_InputFile.read( reinterpret_cast<char*>(&tmp), 1);
        if (m_InputFile.fail()){
            return false;
        }
        // check if next byte is valid
        if ((tmp & 0xc0) != 0x80){
            return false;
        }
        value = (value << 6) | (tmp & 0x3f);
    }
    // check if value is valid
    if (value > 0x10ffff){
        return false;
    }
    return true;
}

bool writeToFile ( uint32_t number, ofstream & m_OutputFile, TBuffer & buffer )
{
    while (number > 0){
        if (buffer.size == 8){
            if (!m_OutputFile.write((char *) &buffer.byte, 1)){
                return false;
            }
            buffer.byte = buffer.size = 0;
        }
        buffer.byte |= (number & 1) << buffer.size++;
        number >>= 1;
    }
    return true;
}

bool encodeToFibonacci ( uint32_t number, ofstream & fout, TBuffer & buffer )
{
    number += 1;
    uint32_t value = 0;
    int flag = 0;

    for (int i = SIZE_FIB; i >= 0; i--){
        if ((int)number >= FIBONACCI_NUMBERS[i]){
            if (!flag){
                value |= 1 << (i+1);
                flag = 1;
            }
            number -= FIBONACCI_NUMBERS[i];
            value |= 1 << i;
        }
    }

    if (!writeToFile(value, fout, buffer)){
        return false;
    }
    return true;
}

bool utf8ToFibonacci ( const char * inFile, const char * outFile )
{
    ifstream m_InputFile(inFile, ios::binary | ios::in);
    if (!m_InputFile.is_open()){
        return false;
    }
    ofstream m_OutputFile(outFile, ios::binary | ios::out);
    if (!m_OutputFile.is_open()){
        return false;
    }
    uint32_t value = 0;
    TBuffer buffer = {0, 0};
    while (true){
        if (!decodeUtf8Char(m_InputFile, value)){
            return false;
        }
        if (m_InputFile.eof()){
            break;
        }
        if (!encodeToFibonacci(value, m_OutputFile, buffer)){
            return false;
        }
    }
    // write last byte
    if (buffer.size != 0){
        buffer.byte |= (0 & 1) << (8 - buffer.size);
        if (!m_OutputFile.write((char *) &buffer.byte, 1)){
            return false;
        }
    }

    m_InputFile.close();
    m_OutputFile.close();
    return true;
}

bool encodeToUtf8(const vector<int> & fib, ofstream & m_OutputFile)
{
    uint32_t value = 0;
    if (fib.size() > SIZE_FIB){
        return false;
    }
    for (size_t i = 0; i < fib.size(); i++){
        if (fib[i] == 1){
            value += FIBONACCI_NUMBERS[i];
        }
    }
    value -= 1;
    if (value > 0x10ffff){
        return false;
    }
    if (value < 128){
        if (!m_OutputFile.put(value)){
            return false;
        }
    } else if (value < 2048){
        if (!m_OutputFile.put(0xc0 | (value >> 6))
            || !m_OutputFile.put(0x80 | (value & 0x3f))){
            return false;
        }
    } else if (value < 65536){
        if (!m_OutputFile.put(0xe0 | (value >> 12))
            || !m_OutputFile.put(0x80 | ((value >> 6) & 0x3f))
            || !m_OutputFile.put(0x80 | (value & 0x3f))){
            return false;
        }

    } else if (value < 2097152){
        if (!m_OutputFile.put(0xf0 | (value >> 18))
            || !m_OutputFile.put(0x80 | ((value >> 12) & 0x3f))
            || !m_OutputFile.put(0x80 | ((value >> 6) & 0x3f))
            || !m_OutputFile.put(0x80 | (value & 0x3f))){
            return false;
        }
    } else {
        return false;
    }
    return true;
}

bool decodeFibonacciNumber(ifstream & m_InputFile, ofstream & m_OutputFile)
{
    vector <int> fib;
    uint8_t tmp;
    bool previousBit = false;
    while (m_InputFile.read(reinterpret_cast<char *> (&tmp), 1)){
        for (int i = 0 ; i < 8; i++){
            bool bit = (tmp >> i) & 1;
            if (previousBit && bit){
                if (!encodeToUtf8(fib, m_OutputFile)){
                    return false;
                }
                fib.clear();
                previousBit = false;
            } else {
                if (bit){
                    previousBit = true;
                } else {
                    previousBit = false;
                }
                fib.push_back(bit);
            }
        }
    }
    for (size_t i = 0; i < fib.size(); i++){
        if (fib[i] == 1){
            return false;
        }
    }
    return true;
}

bool fibonacciToUtf8 ( const char * inFile, const char * outFile )
{
    ifstream m_InputFile(inFile, ios::binary | ios::in);
    if (!m_InputFile.is_open()){
        return false;
    }
    ofstream m_OutputFile(outFile, ios::binary | ios::out);
    if (!m_OutputFile.is_open()){
        return false;
    }
    while (true){
        if (!decodeFibonacciNumber(m_InputFile, m_OutputFile)){
            return false;
        }
        if (m_InputFile.eof()){
            break;
        }
        if (m_InputFile.fail()){
            return false;
        }
    }
    return true;
}

#ifndef __PROGTEST__

bool identicalFiles ( const char * file1, const char * file2 )
{
    /**
    ifstream f1 ( file1, ios::in | ios :: binary );
    ifstream f2 ( file2, ios::in | ios :: binary );
    if (!f1.is_open() || !f2.is_open()){
        cout << "Error opening file" << endl;
        return false;
    }

    char b1, b2;
    bool isIdentical = true;
    while (f1.get(b1) && f2.get(b2))
        if (b1 != b2)
        {
            cout << "Files are not identical" << endl;
            isIdentical = false;
            break;
        }
    f1.close();
    f2.close();
    return isIdentical;
    */
}

int main ( int argc, char * argv [] )
{
    /**
    assert ( utf8ToFibonacci ( "example/src_0.utf8", "output.fib" )
             && identicalFiles ( "output.fib", "example/dst_0.fib" ) );

    assert ( utf8ToFibonacci ( "example/src_1.utf8", "output.fib" )
             && identicalFiles ( "output.fib", "example/dst_1.fib" ) );

    assert ( utf8ToFibonacci ( "example/src_2.utf8", "output.fib" )
             && identicalFiles ( "output.fib", "example/dst_2.fib" ) );

    assert ( utf8ToFibonacci ( "example/src_3.utf8", "output.fib" )
             && identicalFiles ( "output.fib", "example/dst_3.fib" ) );

    assert ( utf8ToFibonacci ( "example/src_4.utf8", "output.fib" )
             && identicalFiles ( "output.fib", "example/dst_4.fib" ) );

    assert ( ! utf8ToFibonacci ( "example/src_5.utf8", "output.fib" ) );

    assert ( fibonacciToUtf8 ( "example/src_6.fib", "output.utf8" )
             && identicalFiles ( "output.utf8", "example/dst_6.utf8" ) );
    assert ( fibonacciToUtf8 ( "example/src_7.fib", "output.utf8" )
             && identicalFiles ( "output.utf8", "example/dst_7.utf8" ) );
    assert ( fibonacciToUtf8 ( "example/src_8.fib", "output.utf8" )
             && identicalFiles ( "output.utf8", "example/dst_8.utf8" ) );
    assert ( fibonacciToUtf8 ( "example/src_9.fib", "output.utf8" )
             && identicalFiles ( "output.utf8", "example/dst_9.utf8" ) );
    assert ( fibonacciToUtf8 ( "example/src_10.fib", "output.utf8" )
             && identicalFiles ( "output.utf8", "example/dst_10.utf8" ) );
    assert ( ! fibonacciToUtf8 ( "example/src_11.fib", "output.utf8" ) );
    */
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */

