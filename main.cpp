#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/* Gosper's Hack, Exercise 7.1.3-20 The Art of Computer Programming,
 * Volume 4A Combinatorial Algorithms, Part 1 by Donald E. Knuth.
 *
 * Takes in the binary representation X of a combination.
 * Returns the next combination represented in as a binary number.
 * e.g. input: 0b110,  output: 0b1001;
 *      input: 0b1001, output: 0b1010.
 */
std::size_t next_combination(std::size_t x) {
    std::size_t u = x & (-x);
    std::size_t v = x + u;
    return v + (((v ^ x) / u) >> 2);
}

/* Returns the Nth (0-indexed) row of the Pascal's Triangle. */
std::vector<std::size_t> pascal(std::size_t n) {
    std::vector<std::size_t> prev(n + 1, 0);
    std::vector<std::size_t> curr(n + 1, 0);
    curr[0] = 1;
    for (std::size_t i = 1; i <= n; ++i) {
        prev.swap(curr);
        curr[0] = 1;
        for (std::size_t j = 1; j <= i; ++j) {
            curr[j] = prev[j - 1] + prev[j];
        }
    }
    return curr;
}

std::vector<std::size_t> construct_moves(std::size_t m, std::size_t n) {
    std::vector<std::size_t> moves;
    for (std::size_t k = 0; k < m * n; ++k) {
        std::size_t pos = 0;
        std::size_t i = k / n;
        std::size_t j = k % n;
        /* Toggle light at center cell. */
        pos ^= std::size_t(1) << k;
        /* Toggle left cell if possible. */
        pos ^= std::size_t(j > 0) << (j > 0) * (k - 1);
        /* Toggle right cell if possible. */
        pos ^= std::size_t(j < n - 1) << (j < n - 1) * (k + 1);
        /* Toggle top cell if possible. */
        pos ^= std::size_t(i > 0) << (i > 0) * (k - n);
        /* Toggle bottom cell if possible. */
        pos ^= std::size_t(i < m - 1) << (i < m - 1) * (k + n);
        moves.push_back(pos);
    }
    return moves;
}

std::size_t pos_from_move(std::size_t moveID, const std::vector<std::size_t> &moves) {
    std::size_t pos = 0;
    std::size_t i = 0;
    while (moveID > 0) {
        pos ^= moves[i] * (moveID & 1);
        ++i;
        moveID >>= 1;
    }
    return pos;
}

char *solve_segment(std::size_t m, std::size_t n, std::size_t seg_i, std::size_t split) {
    if (m > 64 || n > 64 || m * n > 64) {
        std::cout << "too large" << std::endl;
        return nullptr;
    }
    std::size_t mn = m * n;
    std::size_t segment_size = std::size_t(1) << (mn - split);
    std::size_t segment_begin = seg_i * segment_size;
    std::size_t segment_end = segment_begin + segment_size;
    std::vector<std::size_t> mnChoose = pascal(mn);
    std::vector<std::size_t> moves = construct_moves(m, n);
    char *data = new char[segment_size];
    for (std::size_t i = 0; i < segment_size; ++i) {
        data[i] = -1;
    }
    if (seg_i == 0) {
        data[0] = 0;
    }
    for (std::size_t k = 31; k <= mn; ++k) { /* DEBUGGING */
        std::size_t moveID = (size_t(1) << k) - 1;
        for (std::size_t i = 0; i < mnChoose[k]; ++i) {
            std::size_t pos = pos_from_move(moveID, moves);
            if (pos >= segment_begin && pos < segment_end && data[pos - segment_begin] == -1) {
                data[pos - segment_begin] = k;
            }
            moveID = next_combination(moveID);
        }
        std::cout << "segment [" << seg_i << "] k = " << k << " done" << std::endl;
    }
    return data;
}

void solve_and_save_segments(std::size_t m, std::size_t n, std::size_t split = 0) {
    std::size_t num_segments = std::size_t(1) << split;
    for (std::size_t i = 0; i < num_segments; ++i) {
        char *data = solve_segment(m, n, i, split);
        std::ofstream of;
        of.open("segment" + std::to_string(i) + ".bin", std::ofstream::out | std::ofstream::binary);
        of.write(data, std::size_t(1) << (m*n - split));
        of.close();
        delete [] data;
        std::cout << "remove finished segment and press return to continue." << std::endl;
        std::cin.get();
    }
}

char *solve(std::size_t m, std::size_t n) {
    if (m > 64 || n > 64 || m * n > 64) {
        std::cout << "too large" << std::endl;
        return nullptr;
    }
    std::size_t mn = m * n;
    std::vector<std::size_t> mnChoose = pascal(mn);
    std::vector<std::size_t> moves = construct_moves(m, n);
    char *data = new char[std::size_t(1) << mn];
    for (std::size_t i = 0; i < std::size_t(1) << mn; ++i) {
        data[i] = -1;
    }
    data[0] = 0;
    bool updated = false;
    for (std::size_t k = 1; k <= mn; ++k) {
        std::size_t moveID = (1 << k) - 1;
        for (std::size_t j = 0; j < mnChoose[k]; ++j) {
            std::size_t pos = pos_from_move(moveID, moves);
            if (data[pos] == -1) {
                data[pos] = k;
                updated = true;
            }
            moveID = next_combination(moveID);
        }
        std::cout << "k = " << k << " done" << std::endl;
        if (!updated) {
            break;
        }
        updated = false;
    }
    return data;
}

int main() {
    /* Assert 64-bit machine. Hash representation would be incorrect
       if this program is run on a non-64-bit machine. */
    assert(sizeof(std::size_t) == 8);
    std::size_t m = 6;
    std::size_t n = 6;
    solve_and_save_segments(m, n, 2);
    return 0;
}
