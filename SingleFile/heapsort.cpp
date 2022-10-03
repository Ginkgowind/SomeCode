//
// Created by Ginkgowind on 2022/10/3.
//

#include <iostream>
#include <vector>

using namespace std;

void sink(vector<int> &nums, int i, int len);

void print(vector<int> &nums);

int main() {
    vector<int> nums = {4, 7, 2, 9, 6, 3};
    int len = nums.size();
    // 构建堆
    for (int i = len / 2 - 1; i >= 0; i--)
        sink(nums, i, len);
    // 排序
    for (int i = len - 1; i > 0; i--) {
        //将堆顶元素与末位元素调换
        swap(nums[0], nums[i]);
        //数组长度-1 隐藏堆尾元素
        len--;
        //将堆顶元素下沉 目的是将最大的元素浮到堆顶来
        sink(nums, 0, len);
    }
    print(nums);
}

void print(vector<int> &nums) {
    int n = nums.size();
    for (int i = 0; i < n; i++)
        cout << nums[i] << ' ';
    cout << endl;
}

void sink(vector<int> &nums, int i, int len) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int now = i;
    //下沉左边
    if (left < len && nums[left] > nums[now]) now = left;
    //下沉右边
    if (right < len && nums[right] > nums[now]) now = right;
    //如果下标不相等 证明调换过了
    if (now != i) {
        swap(nums[i], nums[now]);
        sink(nums, now, len);
    }
}