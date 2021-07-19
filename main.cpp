/**
 * @author Павел Назимко
 *
 * @date Дата выдачи задания: 14.07.2021
 *
 * @brief II Задача о парковке
 *
 * Дана последовательность пар вида {start, end}
 *
 * start - время заезда автотранспорта на парковку
 * end - время выезда автотранспорта с парковки
 *
 * Нужно найти максимальное количество автотранспорта, находящихся одновременно на парковке
 *
 */

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cstdint>
#include <cassert>

enum class Status : uint8_t {Start = 0x1, End = 0x2};

/**
 * @brief Функция, вычисляющая искомое значение
 *
 * @param timestamps Ссылка на последовательность пар временных меток {start, end}.
 *
 * @return Максимальное количество автотранспорта, находящихся одновременно на парковке.
 *
 */

size_t solution(const std::vector<std::pair<size_t, size_t>> &timestamps)
{
    /// Данное решение подразумевает, что при одновременном выезде и выезде машин
    /// на парковке находятся как въезжающие, так и выезжающие машины.
    /// Также данное решение подразумевает валидность (корректность) входных данных (неотрицательные метки времени, start <= end и т.д.)

    std::map<size_t, std::multiset<Status>> sorted;

    /// Производим сортировку временных меток с одновременным присвоением им флагов выезда/выезда.
    /// Флаги Start будут отсортированы в начале multiset-контейнера.
    /// Таким образом, для одинаковых временных меток будет сформирован набор флагов.

    for (const auto &time_pair : timestamps)
    {
        sorted[time_pair.first].insert(Status::Start);
        sorted[time_pair.second].insert(Status::End);
    }

    size_t cur_intersections = 0;
    size_t max_intersections = 0;

    /// В цикле производится обход каждой метки, и для каждой метки во вложенном цикле будет производиться
    /// обход флагов метки, где по флагам Start и End будут соответственно производиться
    /// инкремент и декремент текущих «пересечений» машин на парковке.
    /// Максимальное число «пересечений» будет записано в отдельную переменную.

    for (const auto &time_status_pair: sorted)
    {
        for (const auto status: time_status_pair.second)
        {
            cur_intersections += (status == Status::Start) ? 1 : -1;
            if (cur_intersections > max_intersections)
            {
                max_intersections = cur_intersections;
            }
        }
    }

    return max_intersections;
}

/// Также с целью проверки написанного выше решения напишем «тупое» решение через перебор всех пар значений

/**
 * @brief Функция, вычисляющая искомое значение
 *
 * @param timestamps Ссылка на последовательность пар временных меток {start, end}.
 *
 * @return Максимальное количество автотранспорта, находящихся одновременно на парковке.
 *
 */

size_t dumb_solution(const std::vector<std::pair<size_t, size_t>> &timestamps)
{
    size_t intersections = 0;

    std::multimap<size_t, size_t> sorted;

    for (const auto &time_pair : timestamps)
    {
        sorted.insert({time_pair.first, time_pair.second});
    }

    for (auto iter1 = sorted.cbegin(); iter1 != sorted.cend(); ++iter1)
    {
        std::pair<size_t, size_t> intersection{iter1->first, iter1->second};
        size_t cur_intersections = 0;
        for (auto iter2 = sorted.cbegin(); iter2 != sorted.cend(); ++iter2)
        {
            if (intersection.second < iter2->first)
            {
                break;
            }
            if (intersection.second >= iter2->second && (intersection.first <= iter2->second))
            {
                intersection.second = iter2->second;
                ++cur_intersections;
                if ((intersection.first <= iter2->first) && (intersection.second >= iter2->first))
                {
                    intersection.first = iter2->first;
                }
            }
            else if ((intersection.first <= iter2->first) && (intersection.second >= iter2->first))
            {
                intersection.first = iter2->first;
                ++cur_intersections;
            }

            if (cur_intersections > intersections)
            {
                ++intersections;
            }
        }
    }
    return intersections;
}

int main()
{
    /// Проверка решения на нескольких тест-кейсах:
    std::vector<std::pair<size_t, size_t>> t1 = {{1, 8}, {2, 3}, {2, 4}, {2, 5}, {7, 8}, {8, 12}, {9, 10}, {3, 4}, {5, 7}, {5, 8}};
    std::vector<std::pair<size_t, size_t>> t2 = {{1, 8}, {9, 10}};
    std::vector<std::pair<size_t, size_t>> t3 = {{1, 8}};
    std::vector<std::pair<size_t, size_t>> t4 = {};
    std::vector<std::pair<size_t, size_t>> t5 = {{11, 12}, {11, 11}, {10, 11}, {1, 2}, {2, 4}, {7, 8}, {9, 10}};
    std::vector<std::pair<size_t, size_t>> t6 = {{0, 12}, {11, 13}, {50, 99}, {100, 101}, {101, 105}};
    std::vector<std::pair<size_t, size_t>> t7 = {{100, 200}, {110, 130}, {210, 220}, {218, 219}, {219, 220}};

    assert(solution(t1) == 5);
    assert(solution(t2) == 1);
    assert(solution(t3) == 1);
    assert(solution(t4) == 0);
    assert(solution(t5) == 3);
    assert(solution(t6) == 2);
    assert(solution(t7) == 3);

    assert(solution(t1) == dumb_solution(t1));
    assert(solution(t2) == dumb_solution(t2));
    assert(solution(t3) == dumb_solution(t3));
    assert(solution(t4) == dumb_solution(t4));
    assert(solution(t5) == dumb_solution(t5));
    assert(solution(t6) == dumb_solution(t6));
    assert(solution(t7) == dumb_solution(t7));

    return 0;
}
