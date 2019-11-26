/*
“Считалочка”. В круг выстроено N человек, пронумерованных числами от 1 до N.
Будем исключать каждого k-ого до тех пор, пока не уцелеет только один человек.
(Например, если N=10, k=3, то сначала умрет 3-й, потом 6-й, затем 9-й,
затем 2-й, затем 7-й, потом 1-й, потом 8-й, за ним - 5-й, и потом 10-й.
Таким образом, уцелеет 4-й.) Необходимо определить номер уцелевшего.
N, k ≤ 10000.
Требования:  Решить перебором.
*/

// использовать список
#include <iostream>

int main() {

    int amount(0), step(0);
    scanf("%d %d", &amount, &step);
    int circle[amount];
    for (int iii = 1; iii <= amount; ++iii)
        circle[iii] = iii;

    int removed(0), index_to_remove(0);
    while (removed != amount - 1) {
        int moves(0);
        while (moves != 3) {
            if (circle[index_to_remove] != 0) {
                index_to_remove++;
                moves++;
            }
        }
    }

    return 0;
}