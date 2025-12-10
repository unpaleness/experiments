#include <algorithm>
#include <format>
#include <iostream>
#include <optional>
#include <vector>

namespace {
struct Point {
    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    bool operator<(const Point& other) const { return x < other.x || y < other.y || z < other.z; }
    bool operator==(const Point& other) const { return x == other.x && y == other.y && z == other.z; }
    Point operator+(const Point& other) const { return Point{x + other.x, y + other.y, z + other.z}; }
    Point operator-(const Point& other) const { return Point{x - other.x, y - other.y, z - other.z}; }
    Point operator*(const Point& other) const { return Point{x * other.x, y * other.y, z * other.z}; }
};
template <>
struct std::formatter<Point> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }
    auto format(const Point& p, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "{} {} {}", p.x, p.y, p.z);
    }
};
typedef std::vector<int32_t> Vector;
typedef std::vector<Point> Pattern;
typedef std::vector<Pattern> Patterns;
typedef std::vector<Vector> VVector;
constexpr int32_t UNDEFINED = -1;
int32_t DEEPEST_ITERATION = -1;
int64_t PROBES_TRIED = 0;
int32_t N;
int32_t GROUPS_AMOUNT;
Pattern PATTERN;
Patterns PATTERN_VARIATIONS;
Vector CUBE;
VVector GROUPS;
int32_t getZ(const int32_t i) {
    return i / (N * N);
}
int32_t getY(const int32_t i) {
    return i % (N * N) / N;
}
int32_t getX(const int32_t i) {
    return i % N;
}
int32_t getIndex(const Point& p) {
    return p.z * N * N + p.y * N + p.x;
}
bool isInBounds(const Point& p) {
    return p.x >= 0 && p.x < N && p.y >= 0 && p.y < N && p.z >= 0 && p.z < N;
}
Point permutatePoint(const Point& p, int32_t permutationIndex) {
    const int32_t c = permutationIndex % 6;
    Point result;
    result.x = (c == 0 || c == 1) ? p.x : (c == 2 || c == 3) ? p.y : p.z;
    result.y = (c == 2 || c == 4) ? p.x : (c == 0 || c == 5) ? p.y : p.z;
    result.z = (c == 3 || c == 5) ? p.x : (c == 1 || c == 4) ? p.y : p.z;
    return result;
}
void printPattern() {
    for (uint32_t j = 0; j < PATTERN.size(); ++j) {
        const auto& patternPoint = PATTERN[j];
        std::cout << std::format("printPattern: patternPointIndex: {}, point: {}", j, patternPoint) << std::endl;
    }
}
void printPatternVariations() {
    for (uint32_t i = 0; i < PATTERN_VARIATIONS.size(); ++i) {
        std::cout << std::format("printPatternVariations: patternVariation: {}", i) << std::endl;
        const auto& patternVariation = PATTERN_VARIATIONS[i];
        for (uint32_t j = 0; j < patternVariation.size(); ++j) {
            const auto& patternPoint = patternVariation[j];
            std::cout << std::format("printPatternVariations: patternPointIndex: {}, point: {}", j, patternPoint)
                      << std::endl;
        }
        std::cout << std::endl;
    }
}
void generatePatternVariations() {
    // There're total of 48 permutations: 3! * 2^3 (6 permutations of coordinates times 8 variations of each coord sign)
    for (uint32_t permutationIndex = 0; permutationIndex < 48; ++permutationIndex) {
        Pattern patternVariation(PATTERN.size());
        // std::cout << std::format("generatePatternVariations: permutationIndex: {}", permutationIndex) << std::endl;
        const int32_t coordVariation = permutationIndex / 6;
        Point sign;
        sign.x = coordVariation % 2 ? 1 : -1;
        sign.y = (coordVariation >> 1) % 2 ? 1 : -1;
        sign.z = (coordVariation >> 2) % 2 ? 1 : -1;
        for (uint32_t i = 0; i < PATTERN.size(); ++i) {
            patternVariation[i] = permutatePoint(PATTERN[i], permutationIndex) * sign;
            // std::cout << std::format("generatePatternVariations: permutationIndex: {}, indexInPattern: {}, coords: {}
            // {} {}", permutationIndex, i, patternVariation.back()[0], patternVariation.back()[1],
            // patternVariation.back()[2]) << std::endl;
        }
        std::sort(patternVariation.begin(), patternVariation.end());
        bool bIsUnique = true;
        for (uint32_t j = 0; j < PATTERN_VARIATIONS.size(); ++j) {
            // std::cout << std::format("testPermutationsUniqueness: permutationIndex: {}, j: {}", permutationIndex, j)
            // << std::endl;
            if (std::equal(patternVariation.begin(), patternVariation.end(), PATTERN_VARIATIONS[j].begin())) {
                // std::cout << std::format("testPermutationsUniqueness: permutationIndex: {} - not unique",
                // permutationIndex) << std::endl;
                bIsUnique = false;
                break;
            }
        }
        if (bIsUnique) {
            // std::cout << std::format("testPermutationsUniqueness: permutationIndex: {} - unique", permutationIndex)
            // << std::endl;
            PATTERN_VARIATIONS.push_back(std::move(patternVariation));
        }
    }
    std::cout << std::format("generatePatternVariations: {}", PATTERN_VARIATIONS.size()) << std::endl;
    // printPatternVariations();
}
bool readConfig() {
    PATTERN.clear();
    PATTERN_VARIATIONS.clear();
    CUBE.clear();
    GROUPS.clear();
    int32_t buf;
    int32_t bufferIndex = 0;
    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    bool bCompletePatternCoordinate = false;
    while (std::cin >> buf) {
        if (buf < 0) {
            std::cerr << "Invalid input (value below zero)\n";
            return false;
        }
        if (bufferIndex == 0) {
            if (buf == 0) {
                std::cerr << "Invalid input (N value is zero)\n";
                return false;
            }
            N = buf;
        } else if (bufferIndex == 1) {
            GROUPS_AMOUNT = buf;
        } else {
            switch ((bufferIndex - 2) % 3) {
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
        ++bufferIndex;
    }
    if (!bCompletePatternCoordinate) {
        std::cerr << "Invalid input (empty or incomplete pattern coordinates)\n";
        return false;
    }
    printPattern();
    CUBE = Vector(N * N * N);
    for (int32_t i = 0; i < N * N * N; ++i) {
        CUBE[i] = UNDEFINED;
    }
    GROUPS.reserve(GROUPS_AMOUNT);
    for (int32_t i = 0; i < GROUPS_AMOUNT; ++i) {
        GROUPS.emplace_back(Vector(PATTERN.size()));
    }
    generatePatternVariations();

    return true;
}
bool tryPlaceGroup(uint32_t patternVariationIndex, int32_t positionInPattern, int32_t positionInCube, int32_t groupId) {
    // std::cout << std::format("tryPlaceGroup: patternVariationIndex: {}, positionInPattern: {}, positionInCube: {}",
    // patternVariationIndex, positionInPattern, positionInCube) << std::endl;
    const auto& patternVariation = PATTERN_VARIATIONS[patternVariationIndex];
    const Point patternAnchor = patternVariation[positionInPattern];
    Point cubeAnchor;
    cubeAnchor.x = getX(positionInCube);
    cubeAnchor.y = getY(positionInCube);
    cubeAnchor.z = getZ(positionInCube);
    for (uint32_t i = 0; i < PATTERN.size(); ++i) {
        const Point p = patternVariation[i] - patternAnchor + cubeAnchor;
        const int32_t indexInCube = getIndex(p);
        // std::cout << std::format("tryPlaceGroup: positionInGroup: {}, {}, indexInCube: {}", i, p, indexInCube) <<
        // std::endl;
        if (!isInBounds(p) || CUBE[indexInCube] != UNDEFINED) {
            // std::cout << std::format("tryPlaceGroup: patternVariationIndex: {} - bad, return", patternVariationIndex)
            // << std::endl;
            return false;
        }
        GROUPS[groupId][i] = indexInCube;
    }
    return true;
}
// void testPermutationsUniqueness() {
//     VVector permutations;
//     std::vector<int32_t> occurencesList;
//     for (uint32_t i = 0; i < 48; ++i) {
//         // std::cout << std::format("testPermutationsUniqueness: i: {}", i) << std::endl;
//         if (!tryPlaceGroup(i, 0, 0, 0)) {
//             // std::cout << std::format("testPermutationsUniqueness: i: {}, invalid permutation", i) << std::endl;
//             continue;
//         }
//         auto& group = GROUPS[0];
//         std::sort(group.begin(), group.end());
//         bool bIsUnique = true;
//         for (uint32_t j = 0; j < permutations.size(); ++j) {
//             // std::cout << std::format("testPermutationsUniqueness: i: {}, j: {}", i, j) << std::endl;
//             if (std::equal(group.begin(), group.end(), permutations[j].begin())) {
//                 // std::cout << std::format("testPermutationsUniqueness: not unique: i: {}", i) << std::endl;
//                 ++occurencesList[j];
//                 bIsUnique = false;
//                 break;
//             }
//         }
//         if (bIsUnique) {
//             // std::cout << std::format("testPermutationsUniqueness: unique: i: {}", i) << std::endl;
//             permutations.push_back(group);
//             occurencesList.push_back(1);
//         }
//     }
//     std::cout << "Permutations count: " << permutations.size() << '\n';
//     for (uint32_t i = 0; i < permutations.size(); ++i) {
//         printPermutation(permutations[i], occurencesList[i]);
//     }
// }
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
bool iterateGroup(const int32_t groupId) {
    if (groupId > DEEPEST_ITERATION) {
        DEEPEST_ITERATION = groupId;
        std::cout << std::format("iterateGroup: deepest iteration so far: {}, probes: {}", DEEPEST_ITERATION,
                                 PROBES_TRIED)
                  << std::endl;
    }
    // std::cout << std::format("iterateGroup: groupId: {}", groupId) << std::endl;
    if (groupId >= GROUPS_AMOUNT) {
        return true;
    }
    for (int32_t posInCube = 0; posInCube < N * N * N; ++posInCube) {
        if (CUBE[posInCube] != UNDEFINED) {
            continue;
        }
        for (uint32_t patternVariationIndex = 0; patternVariationIndex < PATTERN_VARIATIONS.size();
             ++patternVariationIndex) {
            // std::cout << std::format("iterateGroup: groupId: {}, patternVariationIndex: {}", groupId,
            // patternVariationIndex) << std::endl;
            for (uint32_t posInPattern = 0; posInPattern < PATTERN.size(); ++posInPattern) {
                // std::cout << std::format("iterateGroup: groupId: {}, patternVariationIndex: {}, posInPattern: {}",
                // groupId, patternVariationIndex, posInPattern) << std::endl;
                if (tryPlaceGroup(patternVariationIndex, posInPattern, posInCube, groupId)) {
                    ++PROBES_TRIED;
                    for (const auto posInCubeOfGroup : GROUPS[groupId]) {
                        CUBE[posInCubeOfGroup] = groupId;
                    }
                    if (iterateGroup(groupId + 1)) {
                        return true;
                    }
                    for (const auto posInCubeOfGroup : GROUPS[groupId]) {
                        CUBE[posInCubeOfGroup] = UNDEFINED;
                    }
                }
            }
        }
        return false;
    }
    return false;
}
}  // namespace

int main() {
    if (!readConfig()) {
        return 1;
    }
    if (!iterateGroup(0)) {
        std::cout << "Failed to solve puzzle\n";
    }
    outputCube();
    // testPermutationsUniqueness();
    return 0;
}
