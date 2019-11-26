/*
Даны два строго возрастающих массива целых чисел A[0..n) и B[0..m) и число k.
Найти количество таких пар индексов (i, j), что A[i] + B[j] = k.
n, m ≤ 100000.
Указание. Обходите массив B от конца к началу.
Требования:  Время работы O(n + m).
*/

#include <iostream>
#include <string>
#include <sstream>

void createArray(int *new_array, int size)
{
    std::cin.ignore();
    std::string str;
    std::getline(std::cin, str);
    std::stringstream ss(str);

    int n;
    for (int iii = 0; (iii < size) && (ss >> n); ++iii)
        new_array[iii] = n;
}

int count(int *arr1, int size1, int *arr2, int size2, int k)
{
    int result = 0;

    int start_A = 0;
    for (int iii = size2; iii >= 0; --iii)
    {
        for (int jjj = start_A; jjj < size1; ++jjj)
        {
            start_A = jjj;
            if (arr2[iii] + arr1[jjj] > k)
                break; // read as ++iii
            else if (arr2[iii] + arr1[jjj] == k)
            {
                result++;
                if (jjj + 1 != size1)
                    start_A = jjj + 1;
                break; // read as ++iii
            }
        }
    }
    return result;
}

int main()
{
    // Create first array
    int size_A;
    scanf( "%d", &size_A );
    int A[size_A];
    createArray(A, size_A);

    // Create second array
    int size_B;
    scanf("%d", &size_B);
    int B[size_B];
    createArray(B, size_B);
    
    // Input k
    int k;
    scanf("%d", &k);

    // Counting
    printf("%d\n", count(A, size_A, B, size_B, k));
    
    return 0;
}