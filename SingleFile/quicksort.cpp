//
// Created by Ginkgowind on 2022/10/3.
//

#include <iostream>
#include <vector>

using namespace std;

void print(vector<int> &nums) {
    int n = nums.size();
    for (int i = 0; i < n; i++)
        cout << nums[i] << ' ';
    cout << endl;
}

// 对arr数组进行排序
void quicksort(vector<int> &arr, int l, int r) {
    // 子数组长度为 1 时终止递归
    if (l >= r) return;
    // 哨兵划分操作（以 arr[l] 作为基准数）
    int i = l, j = r;
    while (i < j) {
        while (i < j && arr[j] >= arr[l]) j--;
        while (i < j && arr[i] <= arr[l]) i++;
        swap(arr[i], arr[j]);
    }    //最后 i==j
    swap(arr[i], arr[l]);

    quicksort(arr, l, i - 1);
    quicksort(arr, i + 1, r);
}

int main() {
    vector<int> nums = {4, 7, 2, 9, 6, 1};
    int len = nums.size();

    quicksort(nums, 0, nums.size() - 1);

    print(nums);

    return 0;
}