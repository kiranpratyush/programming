#include <vector>
#include <iostream>

#define MODULO 1000000007

struct TrieNode
{
    bool isEnd{false};
    TrieNode *children[26]{nullptr};
};

void insert(std::string &word, TrieNode *root)
{
    TrieNode *current_node = root;
    for (char c : word)
    {
        int index = c - 'a';
        if (current_node->children[index] == nullptr)
        {
            current_node->children[index] = new TrieNode();
        }
        current_node = current_node->children[index];
    }
    current_node->isEnd = true;
}
void clear(TrieNode *root)
{
    if (!root)
    {
        return;
    }
    for (int i = 0; i < 26; i++)
    {
        clear(root->children[i]);
    }
    delete root;
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    TrieNode *root = new TrieNode();
    std::string given_string;
    int num_words;
    std::cin >> given_string;
    std::cin >> num_words;
    for (int i = 0; i < num_words; i++)
    {
        std::string x;
        std::cin >> x;
        insert(x, root);
    }
    int stringLength = given_string.length();
    std::vector<int> dp(stringLength + 1, 0);
    dp[stringLength] = 1;
    for (int i = stringLength - 1; i >= 0; i--)
    {
        int count = 0;
        TrieNode *currentNode = root;
        for (int j = i; j <= stringLength - 1; j++)
        {
            auto child = currentNode->children[given_string[j] - 'a'];
            if (child == nullptr)
            {
                break;
            }
            if (child->isEnd)
            {
                count = (count + dp[j + 1]) % MODULO;
            }
            currentNode = child;
        }
        dp[i] = count;
    }
    clear(root);
    std::cout << dp[0];
}