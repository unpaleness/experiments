#include <iostream>
#include <optional>
#include <vector>
#include <unordered_set>

namespace {
    typedef std::vector<int32_t> Tiles;
    typedef std::unordered_set<int32_t> Group;
    typedef std::vector<Group> Groups;
    constexpr int32_t N = 5;
    constexpr int32_t GROUPS = 25;
    constexpr int32_t GROUP_SIZE = 5;
    constexpr int32_t UNDEFINED = -1;
    constexpr int32_t PATTERN[GROUP_SIZE][3] = {{0, 0, 0}, {0, 1, 0}, {0, 2, 0}, {1, 2, 0}, {1, 3, 0}};
    int32_t getX(const int32_t i) { return i / (N * N); }
    int32_t getY(const int32_t i) { return i % (N * N) / N; }
    int32_t getZ(const int32_t i) { return i % N; }
    int32_t getIndex(const int32_t x, const int32_t y, const int32_t z) { return x * N * N + y * N + z; }
    bool isInBounds(const int32_t x, const int32_t y, const int32_t z) {
        return x >= 0 && x < N && y >= 0 && y < N && z >= 0 && z < N;
    }
    // There're total of 48 permutations: 3! * 2^3 (6 permutations of coordinates times 8 variations of each coord sign)
    std::optional<Group> getPatternPermutation(int32_t permutationIndex, int32_t positionInPattern, int32_t positionInCube)
    {
        Group result;
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
        for (uint32_t i = 0; i < GROUP_SIZE; ++i) {
            const int32_t x = PATTERN[i][perm0] * xSign - patternAnchorX + cubeAnchorX;
            const int32_t y = PATTERN[i][perm1] * ySign - patternAnchorY + cubeAnchorY;
            const int32_t z = PATTERN[i][perm2] * zSign - patternAnchorZ + cubeAnchorZ;
            if (!isInBounds(x, y, z)) {
                return std::nullopt;
            }
            result.insert(getIndex(x, y, z));
        }
        return result;
    }
    void printPermutation(const std::optional<Group>& groupOpt) {
        if (!groupOpt) {
            std::cout << "Invalid group!\n";
            return;
        }
        for (const auto& i : *groupOpt) {
            std::cout << ' ' << getX(i) << ' ' << getY(i) << ' ' << getZ(i) << '\n';
        }
        std::cout << '\n';
    }
    void init(Tiles& cube) {
        for (uint32_t i = 0; i < N * N * N; ++i) {
            cube[i] = UNDEFINED;
        }
    }
    void outputCube(const Tiles& cube) {
        for (uint32_t i = 0; i < N * N * N; ++i) {
            if (getZ(i) == 0) {
                std::cout << '\n';
                if (getY(i) == 0) {
                    std::cout << '\n';
                }
            }
            std::cout << ' ' << static_cast<char>(0x61 + cube[i]);
        }
    }
    bool canAddToGroup(const Group& group, const uint32_t i) {
        for (int32_t permIndex = 0; permIndex < 48; ++permIndex)
        {
            for (int32_t posInPattern = 0; posInPattern < GROUP_SIZE; ++posInPattern)
            {
                if (const auto patternPermOpt = getPatternPermutation(permIndex, posInPattern, i); patternPermOpt) {
                    bool bIsOk = true;
                    for (const auto& item : group) {
                        if (!patternPermOpt->count(item)) {
                            bIsOk = false;
                        }
                    }
                    if (bIsOk) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    bool solve(Tiles& cube, Groups& groups, const uint32_t i) {
        std::cout << "solve: " << i << '\n';
        if (i >= N * N * N) {
            for (const auto& group : groups) {
                if (group.size() < GROUP_SIZE) {
                    return false;
                }
            }
            return true;
        }
        for (uint32_t groupId = 0; groupId < groups.size(); ++groupId) {
            auto& group = groups[groupId];
            if (group.size() >= GROUP_SIZE) {
                continue;
            }
            if (!canAddToGroup(group, i)) {
                continue;
            }
            cube[i] = groupId;
            group.insert(i);
            if (solve(cube, groups, i + 1)) {
                return true;
            }
            cube[i] = UNDEFINED;
            group.erase(i);
        }

        return false;
    }
}

int main() {
    Tiles cube(N * N * N);
    Groups groups(GROUPS);
    init(cube);
    if (!solve(cube, groups, 0)) {
        std::cout << "Failed to solve puzzle\n";
    }
    outputCube(cube);
    // for (uint32_t i = 0; i < 48; ++i) {
    //     printPermutation(getPatternPermutation(i, 2, 62));
    // }
    return 0;
}
