#include <bits/stdc++.h>
#include <stdlib.h>

using namespace std;

string s;
// 1 + (1 + (2 * 3))
//      +
//    1        +
//           1   *
//              2 3
//
// 1 + 2 * 3
//     +
//   1    *
//       2 3
//
// 1 * 2 + 3
//       *        <- Node1
//     1   2
//
//           +
//     Node1   3
//
//         +   <- Node2.left = Node1
//      *     3
//     1 2

// 1 * 2 / 3 + 4
//     *
//    1 2
//
// 1 * 2 * 3
// 1 + 2 * 3

// 1 + (2 + (23 * 34))

struct Node {
    char c;
    int x = 0;
    bool is_operand = false;
    int priority = 0;
    Node* left = nullptr;
    Node* right = nullptr;
};

Node* parse_substring(int pos);

bool is_brace(char c) {
    return c == '(' || c == ')';
}
bool is_operand(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}
bool is_digit(char c) {
    return c >= '0' && c <= '9';
}
int priority(char c) {
    return 1 + static_cast<int>(c == '*' || c == '/');
}


Node* get_next_token(int& pos) {
    if (s[pos] == '(') {
        ++pos;
        return parse_substring(pos);
    } else if (is_digit(s[pos])) {
        Node* curnode = new Node();

        int start = pos;
        while (pos < s.size() && is_digit(s[pos])) {
            ++pos;
        }
        string x_str = s.substr(start, pos - start);
        curnode->x = stoi(x_str);
        return curnode;
    } else {
        Node* curnode = new Node();
        curnode->is_operand = true;
        curnode->c = s[pos];
        curnode->priority = priority(s[pos]);
        ++pos;
        return curnode;
    }
    return nullptr;
}


// 1 + 2 + 3
// 1 + (2 + 3 * (4 + 5))
// cur_token_node->x = 2
// next_token_node->c = *

//     +  <- cur_node
// 1       *  <- next_token_node
//       2
//       ^
//       cur_token_node

/*
Node* parse_substring(string& s, int pos) {
    Node* root = get_next_token(s, pos);

    Node* cur_node = root;
    int last_priority = 3; // + / -: 1, * / /: 2
    Node* cur_token_node = nullptr;
    Node* next_token_node = nullptr;

    while (pos < s.size() && s[pos] != ')') {
        if (next_token_node) {
            cur_token_node = next_token_node;
            next_token_node = nullptr;
        }
        cur_token_node = get_next_token(s, pos);

        if (cur_token_node->is_operand == false) {
            next_token_node = get_next_token(s, pos);

            if (last_priority < priority(next_token_node->c)) {
                if (!cur_node->left) {
                    cur_node->left = next_token_node;
                    next_token_node->left = cur_token_node;
                } else if (!cur_node->right) {
                    cur_node->right = next_token_node;
                    next_token_node->left = cur_token_node;
                }
                next_token_node = nullptr;
            } else {
                if (!cur_node->left) {
                    cur_node->left = cur_token_node;
                } else {
                    cur_node->right = cur_token_node;
                }
            }
        } else if (cur_token_node->is_operand == true) {
            cur_token_node->left = root;
            root = cur_token_node;
            cur_node = root;
        } else {
            // subtree
            if (!cur_node->left) {
                cur_node->left = cur_token_node;
            } else {
                cur_node->right = cur_token_node;
            }
        }
    }

    return root;
}
*/

Node* parse_substring(int pos) {
    Node* left_son = get_next_token(pos);
    Node* root = get_next_token(pos);
    Node* right_son = get_next_token(pos);
    root->left = left_son;
    root->right = right_son;

    Node* cur_node = root;
    Node* cur_token_node = nullptr;

    while (pos < s.size() && s[pos] != ')') {
        cur_token_node = get_next_token(pos);

        if (cur_token_node->is_operand) {

            if (cur_token_node->priority >= cur_node->priority) {
                cur_node = root;
                while (true) {
                    if (cur_node->right->is_operand) {
                        cur_node = cur_node->right;
                    } else {
                        cur_token_node->left = cur_node->right;
                        cur_node->right = cur_token_node;
                        cur_node = cur_node->right;
                        break;
                    }
                }
            } else {
                cur_token_node->left = root;
                root = cur_token_node;
                cur_node = root;
            }
        } else {
            while (true) {
                if (!cur_node->left) {
                    cur_node->left = cur_token_node;
                    break;
                } else if (!cur_node->right) {
                    cur_node->right = cur_token_node;
                    break;
                } else {
                    cur_node = cur_node->right;
                }
            }
        }
    }

    return root;
}
