// tsm.cpp  — minimal Ant Colony Optimization (ACO) for TSP
#include "tsm.h"
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>

// label helpers per spec: A..Z -> 0..25, a..z -> 26..51
int idx2(char v) { return (v >= 'a' && v <= 'z') ? 26 + (v - 'a') : (v - 'A'); }
char label2(int i) { return (i < 26) ? char('A' + i) : char('a' + (i - 26)); }

// convert a node-index tour to "A B ... A"
std::string to_string_path(const std::vector<int> &tour)
{
    std::string s;
    for (size_t i = 0; i < tour.size(); ++i)
    {
        s.push_back(label2(tour[i]));
        if (i + 1 < tour.size())
            s.push_back(' ');
    }
    return s;
}

// cost of a tour; returns a big number if any hop is invalid (0 edge)
int tour_cost(int G[60][60], const std::vector<int> &tour)
{
    int cost = 0;
    for (size_t i = 1; i < tour.size(); ++i)
    {
        int w = G[tour[i - 1]][tour[i]];
        if (w <= 0)
            return 999999; // 0 means "not connected" per assignment
        cost += w;
    }
    return cost;
}

// roulette-wheel sampling (prob entries are >= 0; not all zero)
int sample_next(const std::vector<double> &prob, std::mt19937 &rng)
{
    double sum = 0.0;
    for (double p : prob)
        sum += p;
    std::uniform_real_distribution<double> dist(0.0, sum);
    double r = dist(rng), cum = 0.0;
    for (int i = 0; i < (int)prob.size(); ++i)
    {
        cum += prob[i];
        if (r <= cum)
            return i;
    }
    return (int)prob.size() - 1;
}

// ACO Traveling
std::string Traveling(int G[60][60], int n, char start)
{
    const int s = idx2(start);
    if (n <= 1)
        return std::string(1, label2(s)) + " " + std::string(1, label2(s));

    // --- small, safe defaults (fast on LMS, decent quality) ---
    const int numAnts = std::max(10, std::min(n * 2, 60));
    const int iters = std::max(40, std::min(200, 15 * n));
    const double alpha = 1.0; // pheromone influence
    const double beta = 3.0;  // heuristic influence (1/weight)
    const double rho = 0.30;  // evaporation rate
    const double Q = 100.0;   // deposit factor

    // pheromone (tau) and heuristic (eta = 1/weight, 0 if no edge)
    std::vector<std::vector<double>> tau(n, std::vector<double>(n, 1.0));
    std::vector<std::vector<double>> eta(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (i != j && G[i][j] > 0)
                eta[i][j] = 1.0 / (double)G[i][j];

    std::mt19937 rng(1234567u); // deterministic for reproducibility

    // keep the best-so-far tour
    std::vector<int> bestTour(n + 1, s);
    for (int i = 1; i < n; ++i)
        bestTour[i] = i;
    bestTour[n] = s;
    int bestCost = tour_cost(G, bestTour);

    std::vector<std::vector<int>> antTours(numAnts, std::vector<int>(n + 1));
    std::vector<int> antCost(numAnts, 999999);

    for (int it = 0; it < iters; ++it)
    {
        // construct tours
        for (int k = 0; k < numAnts; ++k)
        {
            std::vector<char> used(n, 0);
            std::vector<int> &tour = antTours[k];
            tour[0] = s;
            used[s] = 1;
            int cur = s;

            for (int step = 1; step < n; ++step)
            {
                // build probabilities to each candidate
                std::vector<double> prob(n, 0.0);
                bool any = false;
                for (int v = 0; v < n; ++v)
                {
                    if (used[v] || v == cur)
                        continue;
                    if (G[cur][v] <= 0)
                        continue; // not connected
                    prob[v] = std::pow(tau[cur][v], alpha) * std::pow(eta[cur][v], beta);
                    if (prob[v] > 0)
                        any = true;
                }
                int next = -1;
                if (any)
                {
                    next = sample_next(prob, rng);
                }
                else
                {
                    // fallback: nearest unused reachable, else any unused
                    int bestV = -1, bestW = 999999;
                    for (int v = 0; v < n; ++v)
                        if (!used[v] && v != cur && G[cur][v] > 0 && G[cur][v] < bestW)
                        {
                            bestW = G[cur][v];
                            bestV = v;
                        }
                    if (bestV == -1)
                        for (int v = 0; v < n; ++v)
                            if (!used[v])
                            {
                                bestV = v;
                                break;
                            }
                    next = bestV;
                }
                tour[step] = next;
                used[next] = 1;
                cur = next;
            }
            tour[n] = s; // close tour
            antCost[k] = tour_cost(G, tour);
        }

        // update best
        for (int k = 0; k < numAnts; ++k)
            if (antCost[k] < bestCost)
            {
                bestCost = antCost[k];
                bestTour = antTours[k];
            }

        // evaporate
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                tau[i][j] *= (1.0 - rho);

        // deposit from each valid ant tour
        for (int k = 0; k < numAnts; ++k)
        {
            if (antCost[k] >= 999999)
                continue;
            double add = Q / (double)antCost[k];
            for (int i = 1; i < (int)antTours[k].size(); ++i)
            {
                int u = antTours[k][i - 1], v = antTours[k][i];
                tau[u][v] += add;
            }
        }
    }

    // if everything failed, return a trivial round (still correct format)
    if (bestCost >= 999999)
    {
        std::vector<int> trivial(n + 1);
        for (int i = 0; i < n; ++i)
            trivial[i] = i;
        trivial[n] = 0;
        return to_string_path(trivial);
    }
    return to_string_path(bestTour);
}
