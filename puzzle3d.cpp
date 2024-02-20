#include <algorithm>
#include <format>
#include <iostream>
#include <optional>
#include <vector>

namespace {
    typedef std::vector<int32_t> Vector;
    constexpr int32_t UNDEFINED = -1;
    int32_t N;
    int32_t GROUPS_AMOUNT;
    std::vector<Vector> PATTERN;
    Vector CUBE;
    bool readConfig() {
        int32_t buf;
        int32_t i = 0;
        int32_t x = 0;
        int32_t y = 0;
        int32_t z = 0;
        bool bCompletePatternCoordinate = false;
        while (std::cin >> buf) {
            if (buf < 0) {
                std::cerr << "Invalid input (value below zero)\n";
                return false;
            }
            if (i == 0) {
                if (buf == 0) {
                    std::cerr << "Invalid input (N value is zero)\n";
                    return false;
                }
                N = buf;
            } else if (i == 1) {
                GROUPS_AMOUNT = buf;
            } else {
                switch ((i - 2) % 3) {
                    case 0:
                        x = buf;
                        bCompletePatternCoordinate = false;
                        break;
                    case 1:
                        y = buf;
                        break;
                    case 2:
                        z = buf;
                        PATTERN.push_back({x, y, z});
                        bCompletePatternCoordinate = true;
                        break;
                }
            }
            ++i;
        }
        if (!bCompletePatternCoordinate) {
            std::cerr << "Invalid input (empty or incomplete pattern coordinates)\n";
            return false;
        }
        CUBE = Vector(N * N * N);
        for (i = 0; i < N * N * N; ++i) {
            CUBE[i] = UNDEFINED;
        }

        return true;
    }
    int32_t getZ(const int32_t i) { return i / (N * N); }
    int32_t getY(const int32_t i) { return i % (N * N) / N; }
    int32_t getX(const int32_t i) { return i % N; }
    int32_t getIndex(const int32_t x, const int32_t y, const int32_t z) { return z * N * N + y * N + x; }
    bool isInBounds(const int32_t x, const int32_t y, const int32_t z) {
        return x >= 0 && x < N && y >= 0 && y < N && z >= 0 && z < N;
    }
    // There're total of 48 permutations: 3! * 2^3 (6 permutations of coordinates times 8 variations of each coord sign)
    std::optional<Vector> tryGetPatternPermutation(int32_t permutationIndex, int32_t positionInPattern, int32_t positionInCube)
    {
        // std::cout << std::format("tryGetPatternPermutation: permutationIndex: {}, positionInPattern: {}, positionInCube: {}", permutationIndex, positionInPattern, positionInCube) << std::endl;
        Vector result;
        result.reserve(PATTERN.size());
        permutationIndex = permutationIndex % 48;
        const int32_t coordVariation = permutationIndex / 6;
        // coordinate permutation
        const int32_t c = permutationIndex % 6;
        const int32_t xSign = coordVariation % 2 ? 1 : -1;
        const int32_t ySign = (coordVariation >> 1) % 2 ? 1 : -1;
        const int32_t zSign = (coordVariation >> 2) % 2 ? 1 : -1;
        const int32_t perm0 = c / 2;
        const int32_t perm1 = (c == 2 || c == 4) ? 0 : (c == 0 || c == 5) ? 1 : 2;
        const int32_t perm2 = (c == 3 || c == 5) ? 0 : (c == 1 || c == 4) ? 1 : 2;
        const int32_t patternAnchorX = PATTERN[positionInPattern][perm0] * xSign;
        const int32_t patternAnchorY = PATTERN[positionInPattern][perm1] * ySign;
        const int32_t patternAnchorZ = PATTERN[positionInPattern][perm2] * zSign;
        const int32_t cubeAnchorX = getX(positionInCube);
        const int32_t cubeAnchorY = getY(positionInCube);
        const int32_t cubeAnchorZ = getZ(positionInCube);
        for (uint32_t i = 0; i < PATTERN.size(); ++i) {
            const int32_t x = PATTERN[i][perm0] * xSign - patternAnchorX + cubeAnchorX;
            const int32_t y = PATTERN[i][perm1] * ySign - patternAnchorY + cubeAnchorY;
            const int32_t z = PATTERN[i][perm2] * zSign - patternAnchorZ + cubeAnchorZ;
            const int32_t indexInCube = getIndex(x, y, z);
            // std::cout << std::format("tryGetPatternPermutation: positionInGroup: {}, ({}, {}, {}), indexInCube: {}", i, x, y, z, indexInCube) << std::endl;
            if (!isInBounds(x, y, z) || CUBE[indexInCube] != UNDEFINED) {
                // std::cout << std::format("tryGetPatternPermutation: permutationIndex: {} - bad, return", permutationIndex) << std::endl;
                return std::nullopt;
            }
            result.push_back(indexInCube);
        }
        return result;
    }
    void printPermutation(const Vector& permutation, const int32_t occurences) {
        if (permutation.empty()) {
            std::cout << "Empty\n";
        }
        for (const auto& i : permutation) {
            std::cout << ' ' << getX(i) << ' ' << getY(i) << ' ' << getZ(i) << '\n';
        }
        std::cout << "Occurences: " << occurences << '\n';
        std::cout << '\n';
    }
    void testPermutationsUniqueness() {
        std::vector<Vector> permutations;
        std::vector<int32_t> occurencesList;
        for (uint32_t i = 0; i < 48; ++i) {
            // std::cout << std::format("testPermutationsUniqueness: i: {}", i) << std::endl;
            auto permutationOpt = tryGetPatternPermutation(i, 0, 0);
            if (!permutationOpt) {
                // std::cout << std::format("testPermutationsUniqueness: i: {}, invalid permutation", i) << std::endl;
                continue;
            }
            auto permutation = *permutationOpt;
            std::sort(permutation.begin(), permutation.end());
            bool bIsUnique = true;
            for (uint32_t j = 0; j < permutations.size(); ++j) {
                // std::cout << std::format("testPermutationsUniqueness: i: {}, j: {}", i, j) << std::endl;
                if (std::equal(permutation.begin(), permutation.end(), permutations[j].begin())) {
                    // std::cout << std::format("testPermutationsUniqueness: not unique: i: {}", i) << std::endl;
                    ++occurencesList[j];
                    bIsUnique = false;
                    break;
                }
            }
            if (bIsUnique) {
                // std::cout << std::format("testPermutationsUniqueness: unique: i: {}", i) << std::endl;
                permutations.emplace_back(std::move(permutation));
                occurencesList.push_back(1);
            }
        }
        std::cout << "Permutations count: " << permutations.size() << '\n';
        for (uint32_t i = 0; i < permutations.size(); ++i) {
            printPermutation(permutations[i], occurencesList[i]);
        }
    }
    void outputCube() {
        for (int32_t i = 0; i < N * N * N; ++i) {
            if (getX(i) == 0) {
                std::cout << '\n';
                if (getY(i) == 0) {
                    std::cout << '\n';
                }
            }
            std::cout << ' ' << static_cast<char>(0x61 + CUBE[i]);
        }
    }
    bool tryPlaceGroup(const int32_t groupId) {
        // std::cout << "tryPlaceGroup: groupId: " << groupId << '\n';
        if (groupId >= GROUPS_AMOUNT) {
            return true;
        }
        for (int32_t posInCube = 0; posInCube < N * N * N; ++posInCube) {
            if (CUBE[posInCube] != UNDEFINED) {
                continue;
            }
            for (int32_t permIndex = 0; permIndex < 48; ++permIndex)
            {
                for (uint32_t posInPattern = 0; posInPattern < PATTERN.size(); ++posInPattern)
                {
                    if (const auto patternPermOpt = tryGetPatternPermutation(permIndex, posInPattern, posInCube); patternPermOpt) {
                        for (const auto posInCubeOfGroup : *patternPermOpt) {
                            CUBE[posInCubeOfGroup] = groupId;
                        }
                        if (tryPlaceGroup(groupId + 1)) {
                            return true;
                        }
                        for (const auto posInCubeOfGroup : *patternPermOpt) {
                            CUBE[posInCubeOfGroup] = UNDEFINED;
                        }
                    }
                }
            }
            return false;
        }
        return false;
    }
}

int main() {
    if (!readConfig()) {
        return 1;
    }
    if (!tryPlaceGroup(0)) {
        std::cout << "Failed to solve puzzle\n";
    }
    outputCube();
    // testPermutationsUniqueness();
    return 0;
}
