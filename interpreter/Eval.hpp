#pragma once
#include <string>
#include <vector>
#include "../tokenizer/token/token.hpp"
#include "../utils/utils.hpp"

#include <iostream>

class Eval {
public:
    static std::string AddNumbers(std::string a, std::string b, Literal type) {
        std::string result = "";
        if ((a == "INF" && b == "-INF") || (a == "0" && b == "-0") || (b == "INF" && a == "-INF") || (b == "0" && a == "-0")) return "0";
        if (type == INT) {
            a = NormalizeInt(a);
            b = NormalizeInt(b);
            if (a == "INF" || b == "INF") return "INF";
            if (a == "-INF" || b == "-INF") return "-INF";
            
            long long A = std::stoll(a);
            long long B = std::stoll(b);
            long long r;
            if (!long_long_addition_overflow(A, B)) r = A + B;
            else return (A < 0 && B < 0) ? "-INF" : "INF";

            result = NormalizeInt(std::to_string(r));
        }
        else if (type == FLOAT) {
            a = NormalizeFloat(a);
            b = NormalizeFloat(b);
            if (a == "INF" || b == "INF") return "INF";
            if (a == "-INF" || b == "-INF") return "-INF";

            std::vector<std::string> A = SplitFloat(a);
            std::vector<std::string> B = SplitFloat(b);

            long long Aint = std::stoll(A[0]);
            long long Bint = std::stoll(B[0]);

            int a_sign = A[0].at(0) == '-' ? -1 : 1;
            int b_sign = B[0].at(0) == '-' ? -1 : 1;

            int Afrac = std::stoi(A[1].size() > 6 ? A[1].substr(0, 7) : A[1]);
            int Bfrac = std::stoi(B[1].size() > 6 ? B[1].substr(0, 7) : B[1]);

            int AfracSize = std::to_string(Afrac).size();
            int BfracSize = std::to_string(Bfrac).size();

            long long integerPart;
            
            if (!long_long_addition_overflow(Aint, Bint)) integerPart = Aint + Bint;
            else return (a_sign == -1 && b_sign == -1) ? "-INF" : "INF";
            
            int maxFracSize = std::max({AfracSize, BfracSize});
            if (AfracSize < maxFracSize) Afrac *= std::pow(10, maxFracSize-AfracSize);
            if (BfracSize < maxFracSize) Bfrac *= std::pow(10, maxFracSize-BfracSize);

            long long fracPartLL = Afrac*a_sign + Bfrac*b_sign;

            int prec = std::pow(10, maxFracSize);
            
            if(!long_long_addition_overflow(integerPart, fracPartLL / prec)) integerPart += fracPartLL / prec;
            else return integerPart > 0 ? "INF" : "-INF";

            fracPartLL %= prec;
            if (fracPartLL < 0) {
                integerPart--;
                fracPartLL += prec;
            }

            std::string fracPart = std::to_string(fracPartLL);

            result = NormalizeFloat(std::to_string(integerPart) + '.' + (fracPart.size() > 6 ? fracPart.substr(0, 7) : fracPart));
            A.clear();
            B.clear();
        }

        return result;
    };
    static std::string SubNumbers(std::string a, std::string b, Literal type) {
        if (b.at(0) != '-') b = "-" + b;
        else b = b.substr(1);
        return AddNumbers(a, b, type);
    }
    static std::string MulNumbers(std::string a, std::string b, Literal type) {
        return "";
    };
    static std::string DivNumbers(std::string a, std::string b, Literal type) {
        return "";
    };
    static std::string ModNumbers(std::string a, std::string b, Literal type) {
        return "";
    };
    static std::string PowNumbers(std::string a, std::string b, Literal type) {
        return "";
    };
    static std::string ConcatenateString(std::string a, std::string b) {
        return a + b;
    }
    static std::string RepeatString(std::string a, std::string b) {
        std::string y = "";
        for (int i = 0; i < std::stoi(b); i++)
            y += a;
    }
    static std::string ExpandE(std::string a, size_t f, Literal type) {
        std::string lside = a.substr(0, f);
        std::string rside = a.substr(f+1, a.size());
        return MulNumbers(lside, PowNumbers("10", rside, type), type);
    }

    static std::string NormalizeInt(std::string a) {
        while (!a.empty() && a[0] == '0') {
            a.erase(a.begin());
        }
        size_t decimalPos = a.find('.');
        if (decimalPos != std::string::npos) {
            a = a.substr(0, decimalPos);
        }
        if (a.empty()) {
            return "0";
        }
        return a;
    }

    static std::string NormalizeFloat(std::string a) {
        size_t p = a.find('.');
        if (p == std::string::npos) {
            p = a.size();
            a += ".0";
        }
        if ((p+1) == a.size()) a += '0';
        std::string f = a.substr(p+1, a.size());
        std::reverse(f.begin(), f.end());
        f = NormalizeInt(f);
        std::reverse(f.begin(), f.end());
        return NormalizeInt(a.substr(0, p))+'.'+f;
    }

    static std::vector<std::string> SplitFloat(std::string a) {
        size_t p = a.find('.');
        std::string intPart = a.substr(0, p);
        std::string floatPart = a.substr(p+1, a.size());
        std::vector<std::string> r = {intPart, floatPart};
        return r;
    }
    static Boolean IsGreater(std::string a, std::string b) {
        if (!IsEqual(a, b)) {
            if (a == "INF") return TRUE;
            if (a == "-INF") return FALSE;
            if (b == "INF") return FALSE;
            if (b == "-INF") return TRUE;
        }
        else return FALSE;
        int sign_a = (a.at(0) == '-') ? -1 : 1;
        int sign_b = (b.at(0) == '-') ? -1 : 1;
        if (sign_a != sign_b) {
            return (sign_a == 1) ? TRUE : FALSE;
        }
        if (sign_a == -1) {
            a.erase(a.begin());
            b.erase(b.begin());
        }
        a = NormalizeFloat(a);
        b = NormalizeFloat(b);
        auto A = SplitFloat(a);
        auto B = SplitFloat(b);
        if (A[0].size() > B[0].size()) return TRUE;
        if (A[0].size() < B[0].size()) return FALSE;
        for (int i = 0; i < a.size(); i++) {
            if (a[i] > b[i]) {
                return TRUE;
            } else if (a[i] < b[i]) {
                return FALSE;
            }
        }
        return FALSE;
    }
    static Boolean IsLesser(std::string a, std::string b) {
        return (!IsGreater(a, b) && !IsEqual(a, b)) ? TRUE : FALSE;
    }
    static Boolean IsEqual(std::string a, std::string b) {
        a = NormalizeFloat(a);
        b = NormalizeFloat(b);
        return (a == b) ? TRUE : FALSE;
    }
    static Boolean IsNotEqual(std::string a, std::string b) {
        return (!IsEqual(a, b)) ? TRUE : FALSE;
    }
    static Boolean IsGreatOrEq(std::string a, std::string b) {
        return (IsEqual(a, b) == TRUE || IsGreater(a, b) == TRUE) ? TRUE : FALSE;
    }
    static Boolean IsLessOrEq(std::string a, std::string b) {
        return (IsEqual(a, b) == TRUE || IsLesser(a, b) == TRUE) ? TRUE : FALSE;
    }
    static std::string CheckInfinity(std::string a) {
        if (IsGreater(a, std::to_string(LLONG_MAX))) return "INF";
        else if (IsLesser(a, std::to_string(LLONG_MIN))) return "-INF";
        return a;
    }
};