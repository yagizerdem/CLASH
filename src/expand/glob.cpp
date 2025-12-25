//
// Created by pennywise on 12/21/25.
//

#include "glob.h"

#include <atomic>
#include <stdexcept>
#include <variant>
#include <filesystem>
#include <unistd.h>
#include <thread>
#include <sys/wait.h>
#include <chrono>
#include <functional>

// match pattern
bool Glob::match(std::string pattern, std::string wildcard) {
    // this is fucking ultra edge case
    if (wildcard == "" && pattern == "") return true;
    // FUCK

    std::vector<std::variant<Glob::Token, Glob::BracketToken> > token_stream = tokenize(wildcard);
    bool flag = match_recursive(pattern, token_stream, 0, 0);
    return flag;
}

bool Glob::match_recursive(std::string pattern,
                           std::vector<std::variant<Glob::Token, Glob::BracketToken> > token_stream,
                           int consumed,
                           int nextTokenIndex) {
    for (int i = nextTokenIndex; i < token_stream.size() && consumed < pattern.length(); i++) {
        std::variant<Glob::Token, Glob::BracketToken> variant =
                token_stream[i];
        Token *token = std::get_if<Glob::Token>(&variant);
        BracketToken *bracket_token = std::get_if<Glob::BracketToken>(&variant);

        if (token != nullptr) {
            if (token->lexeme == "?") {
                // matches any character
                consumed++;
                nextTokenIndex++;
            } else if (token->lexeme == "*") {
                // matches any character or 0 character
                bool flag = match_recursive(pattern, token_stream, consumed + 1, i);
                if (flag) {
                    return true;
                };
                nextTokenIndex++;
            } else {
                std::string s(1, pattern[consumed]); // convert char to string
                if (s != token->lexeme) {
                    return false;
                }
                consumed++;
                nextTokenIndex++;
            }
        }
        if (bracket_token != nullptr) {
            char c = pattern[consumed];
            if (bracket_token->allowedCharset.find(c) == bracket_token->allowedCharset.end()) {
                return false;
            }
            consumed++;
            nextTokenIndex++;
        }
    }

    if (consumed < pattern.length()) return false;

    // check last token
    std::variant<Glob::Token, Glob::BracketToken> variant = token_stream.back();
    Token *last_token = std::get_if<Glob::Token>(&variant);

    if (last_token != nullptr && last_token->lexeme == "*" && nextTokenIndex == token_stream.size() - 1) return true;

    // no tokens left to consume , pattern fully matches
    return nextTokenIndex == token_stream.size();
}


std::vector<std::variant<Glob::Token, Glob::BracketToken> > Glob::tokenize(std::string wildcard) {
    std::vector<std::variant<Glob::Token, Glob::BracketToken> > token_stream;
    int consumed = 0;
    while (consumed < wildcard.length()) {
        Token token;

        if (wildcard[consumed] == '*') {
            token.lexeme = '*';
            consumed++;
            token_stream.push_back(token);
        } else if (wildcard[consumed] == '?') {
            token.lexeme = '?';
            consumed++;
            token_stream.push_back(token);
        } else if (wildcard[consumed] == '[') {
            BracketToken bracketToken = collectBracket(consumed, wildcard);
            consumed += bracketToken.lexeme.length();
            classifyBracket(bracketToken);
            token_stream.push_back(bracketToken);
        } else {
            token.lexeme = wildcard[consumed];
            consumed++;
            token_stream.push_back(token);
        }
    }

    return token_stream;
}

Glob::BracketToken Glob::collectBracket(int startIndex, std::string wildcard) {
    if (wildcard[startIndex] != '[') {
        throw std::invalid_argument("Glob parse error: expected '[' at position " + startIndex);
    }
    BracketToken bracketToken;
    bracketToken.lexeme = "[";
    startIndex++;

    while (startIndex < wildcard.length()) {
        if (wildcard[startIndex] == ']') {
            bracketToken.lexeme += "]";
            return bracketToken;
        }
        bracketToken.lexeme += wildcard[startIndex];
        startIndex++;
    }

    throw std::invalid_argument("Glob parse error: expected ']' at position " + startIndex);
}

void Glob::classifyBracket(BracketToken &bracketToken) {
    // skip [ and ] lexemes
    if (bracketToken.lexeme[1] == '^')
        bracketToken.negate = true;
    std::unordered_set<char> charset;
    int lastUsedRightRangeIdentifier = 0;
    int lastUsedLeftRangeIdentifier = 0;
    for (int i = 1; i < bracketToken.lexeme.length() - 1; i++) {
        if (i == 1 && bracketToken.lexeme[i] == '^') {
            continue; // this is negate operator not literal !
        }

        if (bracketToken.lexeme[i] == '-') {
            // must have item in left side
            if (i == 0) {
                charset.insert('-');
            }
            // must has item in right side
            else if (i == bracketToken.lexeme.length() - 1) {
                charset.insert('-');
            } else if (i - 1 == lastUsedRightRangeIdentifier) {
                charset.insert('-');
            } else {
                std::unordered_set<char> rangeSet =
                        resolveRange(bracketToken.lexeme[i - 1], bracketToken.lexeme[i + 1]);
                charset.insert(rangeSet.begin(), rangeSet.end());
                lastUsedRightRangeIdentifier = i + 1;
                lastUsedLeftRangeIdentifier = i - 1;
            }
        } else {
            if (i - 1 > 0 && bracketToken.lexeme[i - 1] == '-') {
                if (lastUsedRightRangeIdentifier == i) continue;
                charset.insert(bracketToken.lexeme[i]);
            } else if (i + 1 < bracketToken.lexeme.length() && bracketToken.lexeme[i + 1] == '-') {
                if (lastUsedLeftRangeIdentifier == i) continue;
                charset.insert(bracketToken.lexeme[i]);
            } else {
                charset.insert(bracketToken.lexeme[i]);
            }
        }
    }

    if (!bracketToken.negate) {
        bracketToken.allowedCharset = charset;
    } else {
        std::unordered_set<char> allowedCharset;
        for (unsigned char c = 1; c < 128; ++c) {
            if (c != '/' && charset.find(c) == charset.end()) {
                allowedCharset.insert(c);
            }
        }
        bracketToken.allowedCharset = allowedCharset;
    }
}


// resolve glob
std::unordered_set<char> Glob::resolveRange(char c1, char c2) {
    std::unordered_set<char> result;

    if (static_cast<unsigned char>(c1) >
        static_cast<unsigned char>(c2)) {
        return result;
    }

    for (unsigned char c = static_cast<unsigned char>(c1);
         c <= static_cast<unsigned char>(c2);
         ++c) {
        result.insert(static_cast<char>(c));
    }

    return result;
}

std::vector<std::string> Glob::resolveGlob(std::string globPattern, std::string cwd) {
    std::vector<std::string> fileAbsolutePaths;
    if (globPattern.empty()) return fileAbsolutePaths;
    if (globPattern[0] == '/') cwd = "/";


    std::atomic<bool> cancelationToken = false;
    std::atomic<bool> done = false;
    std::thread pet([this, &fileAbsolutePaths, globPattern, cwd, &cancelationToken, &done]() {
        fileAbsolutePaths = resolveGlob_recursive(globPattern, cwd,0, cancelationToken);
        done = true;
    });

    int MAX_TIME = 5000; // 5 seconds
    while (true) {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        if (done) {
            pet.join();
            break;
        }

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        int elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        MAX_TIME -=elapsedTime;

        if (MAX_TIME < 0) {
            // kill thread
            cancelationToken = true;
        }
    }

    return  fileAbsolutePaths;


}

std::vector<std::string> Glob::resolveGlob_recursive(std::string globPattern,
    std::string cwd,
    int nextSegmentIndex,
    std::atomic<bool>& cancellationToken) {
    std::vector<std::string> result;
    if (globPattern.empty()) return result;
    if (cancellationToken) return  result;


    std::vector<std::string> segments = stringUtil.split(globPattern, "/");

    for (int i = nextSegmentIndex; i < segments.size(); i++) {
        std::string curSegment = segments[i];

        // cur segment refers file name in this context
        if (i == segments.size() - 1) {
            // file name
            std::vector<std::string> fileNames = getAllFilesUnderPath(cwd);
            for (std::string fileName : fileNames) {
                if (match(fileName, curSegment)) {
                    std::string absPath;
                    if (cwd != "/") {
                        absPath = cwd + "/" + fileName;
                    }
                    else {
                        absPath = cwd + fileName;
                    }
                    result.push_back(absPath);
                }
            }
            return result;
        }

        // cur segement refers folder name in this context
        if (curSegment == ".") {
            // this refers to current path
        }
        else if (curSegment == "..") {
            cwd = getParentPath(cwd);
        }
        else {
            std::vector<std::string> folderNames = getAllFoldersUnderPath(cwd);
            std::vector<std::string> matchedFolderNames;
            for (std::string folderName : folderNames) {
                bool flag = match(folderName, curSegment);
                if (flag) {
                    matchedFolderNames.push_back(folderName);
                }
            }
            // recursively scan all all matched folders
            for (std::string matchedFolderName : matchedFolderNames) {
                std::string nextCwd;
                if (cwd != "/") {
                    nextCwd = cwd + "/" + matchedFolderName;
                }
                else {
                    nextCwd = cwd + matchedFolderName;
                }


                std::vector<std::string> fileAbsolutePaths = resolveGlob_recursive(globPattern,nextCwd, i+1, cancellationToken);

                if (!fileAbsolutePaths.empty()) {
                    for (std::string fileAbsolutePath : fileAbsolutePaths) {
                        result.push_back(fileAbsolutePath);
                    }
                }

            }
        }
    }
}

std::string Glob::getParentPath(std::string path) {
    std::filesystem::path p(path);
    p = p.parent_path();
    std::string parent = p.string();
    return parent;
}

std::vector<std::string> Glob::getAllFoldersUnderPath(const std::string path) {
    std::vector<std::string> result;

    std::error_code ec;
    std::filesystem::directory_iterator it(path, ec);

    if (ec) {
        return result;
    }

    for (const auto& entry : it) {
        if (entry.is_directory(ec) && !ec) {
            result.push_back(entry.path().filename().string());
        }
    }

    return result;
}


std::vector<std::string> Glob::getAllFilesUnderPath(const std::string path) {
    std::vector<std::string> result;

    std::error_code ec;
    std::filesystem::directory_iterator it(path, ec);

    if (ec) {
        return result;
    }

    for (const auto& entry : it) {
        if (entry.is_regular_file(ec) && !ec) {
            result.push_back(entry.path().filename().string());
        }
    }

    return result;
}
