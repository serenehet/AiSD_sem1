

#include "Huffman.h"
#include <ctime>
#include <random>
#include <iostream>

int main() {
    string text = "Напишите две функции для создания архива из одного файла и извлечения файла из архива.\n"
                  "Метод архивирует данные из потока original\n"
                  "void Encode(IInputStream& original, IOutputStream& compressed);\n"
                  "Метод восстанавливает оригинальные данные\n"
                  "void Decode(IInputStream& compressed, IOutputStream& original);\n"
                  "где:\n"
                  "typedef char byte;\n"
                  "\n"
                  "interface IInputStream {\n"
                  "// Возвращает false, если поток закончился\n"
                  "virtual bool Read(byte& value) = 0;\n"
                  "};\n"
                  "\n"
                  "interface IOutputStream {\n"
                  "virtual void Write(byte value) = 0;\n"
                  "};\n"
                  "В архиве сохраняйте дерево Хаффмана и код Хаффмана от исходных данных. Дерево Хаффмана требуется хранить эффективно - не более 10 бит на каждый 8-битный символ.\n"
                  "В контест необходимо отправить .cpp файл содержащий функции Encode, Decode, а также включающий файл Huffman.h. Тестирующая программа выводит размер сжатого файла в процентах от исходного.\n"
                  "Пример минимального решения:\n"
                  "#include \"Huffman.h\"\n"
                  "\n"
                  "static void copyStream(IInputStream&input, IOutputStream& output) {\n"
                  "byte value;\n"
                  "while(input.Read(value)) { output.Write(value); }\n"
                  "}\n"
                  "\n"
                  "void Encode(IInputStream& original, IOutputStream& compressed) {\n"
                  "copyStream(original, compressed);\n"
                  "}\n"
                  "\n"
                  "void Decode(IInputStream& compressed, IOutputStream& original) {\n"
                  "copyStream(compressed, original);";
    VectorInputStream input(text);

    cout << "size0 - " << input.buf.size();
    vector<byte> test1 = input.buf;
    VectorOutputStream output;
    Encode(input, output);
    string comp;

    input.buf = output.buf;
    input.c = 0;
    cout << "size2 " << output.buf.size() << endl;
    VectorOutputStream output2;
    Decode(input, output2);
    vector<byte> test2 = output2.buf;
    cout << "size - " << test2.size();
    return 0;
}
