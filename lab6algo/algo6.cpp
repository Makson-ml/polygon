#include <vector>
#include <string>

bool isMatch(const std::string& s, const std::string& p) {
    std::vector<std::vector<bool>> dp(s.size() + 1, std::vector<bool>(p.size() + 1, false));
    dp[0][0] = true;
    for (size_t j = 1; j <= p.size(); ++j) {
        if (p[j - 1] == '*') {
            dp[0][j] = dp[0][j - 2];
        }
    }
    for (size_t i = 1; i <= s.size(); ++i) {
        for (size_t j = 1; j <= p.size(); ++j) {
            if (p[j - 1] == s[i - 1] || p[j - 1] == '.') {
                dp[i][j] = dp[i - 1][j - 1];
            }
            else if (p[j - 1] == '*') {
                dp[i][j] = dp[i][j - 2] || (dp[i - 1][j] && (s[i - 1] == p[j - 2] || p[j - 2] == '.'));
            }
        }
    }
    return dp[s.size()][p.size()];
}

#include <iostream>

int main() {
    std::cout << std::boolalpha;
    std::cout << isMatch("aaa", "a*") << std::endl;       // Вывод: false
    std::cout << isMatch("aa", "a*") << std::endl;     // Вывод: true
    std::cout << isMatch("ab", ".*") << std::endl;     // Вывод: true
    return 0;
}