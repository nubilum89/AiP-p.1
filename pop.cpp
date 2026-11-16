#include <iostream>
#include <cstdlib>

int main()
{
    int height = 0;
    int width = 0;
    std::cin >> height >> width;

    int** table = new int*[height];

    for (int i = 0; i < height; ++i)
    {
        *(table + i) = new int[width];
    }

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            *(*(table + i) + j) = rand() % 10;
        }
    }

    int i = 0;
    while (i < height)
    {
        for (int j = 0; j < width; ++j)
        {
            std::cout << table[i][j] << '\t';
        }
        std::cout << '\n';
        ++i;
    }

    for (int k = 0; k < height; ++k)
    {
        delete[] table[k];
    }
    delete[] table;
}
