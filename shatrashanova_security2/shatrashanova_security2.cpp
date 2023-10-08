#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <Windows.h>

using namespace std;
void print_menu()
{
    system("cls");
    int variant;
    cout << "1) Зашифровать" << "\n";
    cout << "2) Дешифровать" << "\n";
    cout << "3) Выход" << "\n";
};

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    ifstream file;
    ofstream fout;
    string inputFileName, outputFileName, keyword;
    int option = -1;
    while (option) {
        print_menu();
        cin >> option;
        switch (option)
        {
        case 1:
        {
            cout << "Введите название файла, который хотите зашифровать: ";
            cin >> inputFileName;
            cout << "Введите название файла, который в который будет зашифровываться текст: ";
            cin >> outputFileName;
            file.open(inputFileName + ".txt", ios::binary);
            fout.open(outputFileName + ".txt", ios::binary);
            cout << "Введите ключевой слово" << "\n";
            cin >> keyword;
            int i = 0;
            unsigned char shifr;
            unsigned char a;
            unsigned char b;
            size_t counterIndexKey = 0;
            while (file.is_open() && fout.is_open())
            {

                a = file.get();
                shifr = (unsigned char)(a + keyword[counterIndexKey]);
                fout << shifr;
                ++counterIndexKey;
                if (counterIndexKey == keyword.size())
                {
                    counterIndexKey = 0;
                }
                if (file.peek() == EOF)
                {
                    break;
                }
            };
            file.close();
            fout.close();
            break;

        };
        case 2:
        {
            cout << "\nВведите зашифрованный файл: ";
            string cryptfile;
            cin >> cryptfile;
            ifstream fin(cryptfile + ".txt", ios::binary);
            vector<unsigned char> crypt_Text((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
            fin.close();

            // нахождение длины ключа: метод индекса совпадений
            for (int i = 1; i < 20; i++)
            {
                float conformity_Index = 0.0f;
                float n = crypt_Text.size() / i; //разделение всего текста на длину ключа
                int ceil(n);
                vector<int> letters(256);
                for (int j = 0; j < 256; j++)
                {
                    letters[j] = 0;

                    for (int k = 0; k < crypt_Text.size(); k += i)
                    {
                        if (crypt_Text[k] == (unsigned char)j)
                        {
                            letters[j]++;
                        }
                    }

                    if (letters[j] != 0)
                    {
                        conformity_Index += (float)(letters[j] * (letters[j] - 1)) / (n * (n - 1));
                    }
                }
                cout << i << ": " << conformity_Index << endl;//длина ключа: индекс соответстивия 
            }

            cout << "\nВыберите длину ключа: ";
            int keySize;
            cin >> keySize;

            cout << "\nВведите эталонный файл: ";
            string referencefile;
            cin >> referencefile;
            fin = ifstream(referencefile + ".txt", ios::binary);
            vector<unsigned char> reference_Text((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
            fin.close();

            //подсчёт для наиболее частичной буквы через сортировки 

            vector<pair<unsigned char, int>> reference_Letters(256);
            for (int i = 0; i < 256; i++)
            {
                reference_Letters[i] = make_pair(i, 0);
            }
            for (int i = 0; i < reference_Text.size(); i++)
            {
                reference_Letters[reference_Text[i]].second++;
            }
            sort(reference_Letters.begin(), reference_Letters.end(), [](auto& left, auto& right) { return left.second > right.second; });

            unsigned char most_in_reference = reference_Letters[0].first;
            cout << "\nНаиболее частая буква в эталоне: '" << most_in_reference << "'" << '\n';

            //нахождение ключа
            vector<unsigned char> key;

            for (int i = 0; i < keySize; i++)
            {
                vector<int> crypt_Letters(256);
                for (int j = 0; j < 256; j++)
                {
                    crypt_Letters[j] = 0;
                }
                //частота встречи букв с одним сдвигом
                for (int j = i; j < crypt_Text.size(); j += keySize)
                {
                    crypt_Letters[crypt_Text[j]]++;
                }
                unsigned char most_in_crypt = 0;
                int max = 0;
                for (int j = 0; j < 256; j++)
                {
                    if (crypt_Letters[j] > max)
                    {
                        max = crypt_Letters[j];
                        most_in_crypt = j;
                    }
                }

                key.push_back(256 + most_in_crypt - most_in_reference);
            }
            cout << "\nПолучен ключ: ";

            for (const auto& sym : key)
            {
                cout << sym;
            }
            cout << '\n';


            cout << "\nВведите файл для дешифровки: ";
            string decryptfile;
            cin >> decryptfile;
            ofstream fout;
            fout.open(decryptfile + ".txt", ios::binary);
            if (fout.is_open())
            {
                int i = 0;
                for (const auto& sym : crypt_Text)
                {
                    fout << (unsigned char)(sym - key[i]);
                    i++;
                    if (i >= key.size())
                    {
                        i = 0;
                    }
                }
                fout.close();
            }
            break;
        };
        case 3:
        {
            return 0;
        }
        }
    }
}
