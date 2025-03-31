#include<bits/stdc++.h>

using namespace std;

typedef unsigned long long hashValueType;

const unsigned int X = 131;
const unsigned int maxn = 19260817;

char ref[maxn], query[maxn], refReverse[maxn];
int refLen, queryLen;

hashValueType refHash[maxn], refReverseHash[maxn], queryHash[maxn], powX[maxn];

int dp[maxn];

FILE* inFile, outFile;

void hashTo(char* str, int st, int end, hashValueType* hval) {
    hashValueType htmp = 0;
    for (int i = st; i < end; i++) {
        htmp = htmp * 131 + str[i];
        hval[i] = htmp;
    }
}

void invTo(char* str, char* invStr, int len) {
    for (int i = 0; i < len; i++) {
        invStr[i] = str[len - i - 1];
        switch (invStr[i]) {
            case 'A':
                invStr[i] = 'T';
                break;
            case 'T':
                invStr[i] = 'A';
                break;
            case 'C':
                invStr[i] = 'G';
                break;
            case 'G':
                invStr[i] = 'C';
                break;
            default:
                break;
        }
    }
    invStr[len] = '\0';
}

hashValueType getHash(int l, int r, hashValueType* hval) {
    if (l == 0) {
        return hval[r];
    }
    return hval[r] - hval[l - 1] * powX[r - l + 1];
}

typedef struct substringType {
    int st, len;
    bool isrev;
    substringType(int s, int l, bool f) {
        st = s; len = l; isrev = f;
    }
} substringType;

typedef struct traceType {
    substringType str;
    int nxt;
    traceType(substrings s, int n) {
        str = s; nxt = n;
    }
} traceType;

unordered_map<hashValueType, substringType> substrings;
traceType traces[maxn];
vector<traceType> ans;

int main() {
    inFile = fopen("in.txt", "r");
    // outFile = fopen("out.txt", "w");
    fscanf(inFile, "%s", ref); refLen = strlen(ref);
    fscanf(inFile, "%s" ,query); queryLen = strlen(query);
    invTo(ref, refReverse, refLen);
    powX[0] = 1;
    for (int i = 1; i <= maxn; i++) {
        powX[i] = powX[i - 1] * X;
    }
    hashTo(ref, 0, refLen, refHash);
    hashTo(refReverse, 0, refLen, refReverseHash);
    hashTo(query, 0, queryLen, queryHash);

    for (int i = 0; i < refLen; i++) {
        for (int j = i; j < refLen; j++) {
            hashValueType currentSubstring = getHash(i, j, refHash);
            hashValueType currentSubstringReversed = getHash(i, j, refReverseHash);
            int currentLen = j - i + 1;
            if (substrings.find(currentSubstring) == substrings.end()) {
                substrings[currentSubstring] = substringType(i, currentLen, 0);
            }
            if (substrings.find(currentSubstringReversed) == substrings.end()) {
                substrings[currentSubstringReversed] = substringType(i, currentLen, 1);
            }
        }
    }

    memset(dp, 0x3f3f3f3f, sizeof(dp));
    dp[queryLen] = 0;

    for (int i = queryLen; i >= 0; i--) {
        for (int j = i; j < queryLen; j++) {
            hashValueType currentHash = getHash(i, j, queryHash);
            if (substrings.find(currentHash) != substrings.end()) {
                substringType currentSubstring = substrings[currentHash];
                if (dp[i] < dp[j + 1] + 1 || (dp[i] == dp[j + 1] + 1 && currentSubstring.isrev == 0)) {
                    dp[i] = dp[j + 1] + 1;
                    traces[i] = traceType(currentSubstring, j + 1);
                }
            }
        }
    }

    int pos = 0;
    while (pos < queryLen) {
        ans.push_back(traces[pos]);
        pos = traces[pos].nxt;
    }

    for (i = 0; i < ans.size(); i++) {
        printf("match! starting pos in ref: %d, starting pos in query: %d, len: %d\n", ans[i].str.st, i == 0 ? 0 : ans[i - 1].nxt, ans[i].str.len);
    }
    return 0;
}
