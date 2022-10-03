//
// Created by Ginkgowind on 2022/10/3.
//

#include <iostream>
#include <vector>

using namespace std;

void merge_sort(int l, int r, vector<int>& nums, vector<int>& tmp) {
    //终止条件
    if (l >= r) return;
    //划分
    int m = (l + r) / 2;
    //分成两份后分别的结果相加
    merge_sort(l, m, nums, tmp);
    merge_sort(m + 1, r, nums, tmp);
    //将[l,r]用一个数组保存合并之前的模样
    for (int k = l; k <= r; k++) {
        tmp[k] = nums[k];
    }
    //对[l,r]中的[l,m]、[m+1,r]开始合并
    int i = l;
    int j = m + 1;
    for (int k = l; k <= r; k++) {
        //m及其左边元素合并完毕,把右边剩下的放入合并后的数组
        if (i == m + 1) {
            nums[k] = tmp[j];
            j++;
        }
            //m+1及其右边元素合并完毕,把左边剩下的放入合并后的数组 或者 左边数组的元素小于等于右边,将左边数组的元素放入结果数组中,并让索引i加1
        else if (j == r + 1 || tmp[i] <= tmp[j]) {
            nums[k] = tmp[i];
            i++;
        }
            //右边数组的元素小于左边,将右边数组的元素其放入结果数组中,并让索引j加1
            //并且此时左边数组中的从i到m的所有数都是大于tmp[j]的(因为m左右的数组都是已经排好序的，第15行代码的功劳)
            //即此时有m-i+1个逆序对，加到res上即可
        else //即if(tmp[i]>tmp[j])这种情况
        {
            nums[k] = tmp[j];
            j++;
        }
    }
}

void print(vector<int> &nums) {
    int n = nums.size();
    for (int i = 0; i < n; i++)
        cout << nums[i] << ' ';
    cout << endl;
}

int main() {
    vector<int> nums = {4, 7, 2, 9, 6, 10};
    vector<int> tmp(nums.size());

    merge_sort(0,nums.size()-1,nums,tmp);

    print(nums);

    return 0;
}