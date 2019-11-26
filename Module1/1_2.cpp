#include <iostream>

struct peak
{
    int m_x;
    int m_y;

    peak() : m_x(0), m_y(0) {};
    peak(int x, int y) : m_x(x), m_y(y) {};
};

float countArea(peak left_peak, peak right_peak)
{
    assert( right_peak.m_x > left_peak.m_x && "area is negative" );
    return( left_peak.m_y + right_peak.m_y ) / 2 * ( right_peak.m_x - left_peak.m_x );
}

int main()
{

    int peaks;
    scanf( "%d", &peaks );
    // clear buffer ?
    assert( peaks > 2 && "Amount of peaks less than 3" );

    peak figure[peaks];
    for ( int iii = 0; iii < peaks; ++iii )
    {   
        int x, y;
        scanf("%d %d", &x, &y);
        // Проверка ?
        figure[iii] = peak(x, y);
    }

    float area = 0;

    for ( int iii = 0; iii < peaks; ++iii )
    {
        peak first = figure[iii];

        // That's our way to connect last peak with the first
        // should've been done with cycled list
        peak second;
        if ( iii != peaks - 1 )
            second = figure[iii + 1];
        else
            second = figure[0];
        
        if ( second.m_x < first.m_x )
            area += countArea( second, first );
        else
            area -= countArea(first, second );
    }

    printf("%.2f\n", area);

    return 0;
}

// Площадь может быть отрицательной и не надо делать проверку

/*  Чтобы не организовывать проверку в цикле, можно организовать зацикленный односвязный список
struct peak
{
    int m_x;
    int m_y;
    peak *m_next;

    peak() : m_x(0), m_y(0), m_next(nullptr) {};
    peak(int x, int y, peak *next = nullptr) : m_x(x), m_y(y), m_next(next) {};
};
*/