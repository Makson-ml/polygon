#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

void cocktailsort(std::vector<float>& vec)//сортировка шевелением 
{
    bool swapped = true;
    int start = 0;
    int end = vec.size() - 1;
    while (swapped)
    {
        swapped = false;
        for (int i = start; i < end; ++i)// Проход слева направо
        {
            if (vec[i] > vec[i + 1])
            {
                std::swap(vec[i], vec[i + 1]);
                swapped = true;
            }
        }
        if (!swapped)// Если не было обменов, массив отсортирован
        {
            break;
        }
        end--;
        swapped = false;
        for (int i = end; i > start; --i)// Проход справа налево
        {
            if (vec[i] < vec[i - 1])
            {
                std::swap(vec[i], vec[i - 1]);
                swapped = true;
            }
        }
        start++;
    }
}

void shellsort(std::vector<float>& vec)//o(n^2) prost o(n)
{
    int n = vec.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = vec[i];
            int j;
            for (j = i; j >= gap && vec[j - gap] > temp; j -= gap) {
                vec[j] = vec[j - gap];
            }
            vec[j] = temp;
        }
    }
}

void bucketSort(std::vector<float>& arr) {
    int n = arr.size();
    if (n <= 0) return;
    std::vector<std::vector<float>> buckets(n);
    for (float num : arr) {
        int bucketIndex = static_cast<int>(n * num);
        if (bucketIndex >= n) {
            bucketIndex = n - 1;
        }
        buckets[bucketIndex].push_back(num);
    }
    arr.clear();
    for (int i = 0; i < n; i++) {
        shellsort(buckets[i]);
        for (float num : buckets[i]) {
            arr.push_back(num);
        }
    }
}


int main()
{

    std::vector<float> bedtest = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    std::vector<float> midtest = { 3, 6, 1, 8, 4, 2, 5, 10, 9, 7 };
    std::vector<float> goodtest = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<float> anstest = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    cocktailsort(bedtest);
    cocktailsort(midtest);
    cocktailsort(goodtest);
    assert(bedtest == anstest);
    assert(goodtest == anstest);
    assert(midtest == anstest);
    bedtest = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
    midtest = { 3, 6, 1, 8, 4, 2, 5, 10, 9, 7 };
    goodtest = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    anstest = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    shellsort(bedtest);
    shellsort(midtest);
    shellsort(goodtest);
    assert(bedtest == anstest);
    assert(goodtest == anstest);
    assert(midtest == anstest);
    bedtest = { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
    midtest = { 0.1, 0.4, 0.3, 0.8, 0.9, 0.7, 0.2, 0.6};
    goodtest = { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    bucketSort(bedtest);
    bucketSort(midtest);
    bucketSort(goodtest);
    std::vector<float> ans = { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 };
    assert(bedtest == ans);
    ans = { 0.1, 0.2, 0.3, 0.4, 0.6, 0.7, 0.8, 0.9};
    assert(midtest == ans);
    ans = { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 };
    assert(goodtest == ans);
    std::vector<float> vec;
    int sizen;
    std::cin >> sizen;
    float g;
    for (int i = 0; i < sizen; i++) {
        std::cin >> g;
        vec.push_back(g);
    }
    bucketSort(vec);
    for (int i = 0; i < sizen; i++) {
        std::cout << vec[i] << " ";
    }
    return 0;
}
