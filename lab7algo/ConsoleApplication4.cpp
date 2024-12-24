#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    vector<int> maxNumber(vector<int>& nums1, vector<int>& nums2, int k) {
        vector<int> maxResult;
        for (int i = max(0, k - (int)nums2.size()); i <= min(k, (int)nums1.size()); ++i) {
            vector<int> firstPart = getMaxArray(nums1, i);
            vector<int> secondPart = getMaxArray(nums2, k - i);
            vector<int> merged = merge(firstPart, secondPart);
            maxResult = max(maxResult, merged);
        }
        return maxResult;
    }
private:
    vector<int> getMaxArray(const vector<int>& nums, int k) {
        vector<int> result;
        int drop = nums.size() - k;
        for (int num : nums) {
            while (drop > 0 && !result.empty() && result.back() < num) {
                result.pop_back();
                drop--;
            }
            result.push_back(num);
        }
        result.resize(k);
        return result;
    }

    vector<int> merge(const vector<int>& nums1, const vector<int>& nums2) {
        vector<int> result;
        int i = 0, j = 0;
        while (i < nums1.size() || j < nums2.size()) {
            if (j == nums2.size() || (i < nums1.size() &&
                (nums1[i] > nums2[j] || (nums1[i] == nums2[j] && isFirstGreater(nums1, nums2, i, j))))) {
                result.push_back(nums1[i++]);
            }
            else {
                result.push_back(nums2[j++]);
            }
        }
        return result;
    }

    bool isFirstGreater(const vector<int>& nums1, const vector<int>& nums2, int i, int j) {
        while (i < nums1.size() && j < nums2.size()) {
            if (nums1[i] != nums2[j]) {
                return nums1[i] > nums2[j];
            }
            i++;
            j++;
        }
        return i < nums1.size();
    }
};