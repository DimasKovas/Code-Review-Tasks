#include <vector>
#include <queue>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <iomanip>
#include <limits>
const int N = 300;

const int INF = std::numeric_limits<int>::max();

struct Edge {

    int from, to, capacity, weight, flow;

    int resCapacity() const {
        return capacity - flow;
    }
};

class Graph {
public:

    Graph() : edges(), nx(), phi(), source(0), target(0) {}

    Graph(int size) : edges(), nx(size), phi(size), source(0), target(0) {}

    void clear() {
        edges.clear();
        nx.clear();
        phi.clear();
        source = 0;
        target = 0;
    }

    void setSource(int val) {
        source = val;
    }

    void setTarget(int val) {
        target = val;
    }

    void resize(size_t size) {
        clear();
        nx.resize(size);
        phi.resize(size);
    }

    void addEdge(int from, int to , int capacity, int weight, bool directed = true) {
        nx[from].push_back(edges.size());
        edges.push_back({from, to, capacity, weight, 0});
        nx[to].push_back(edges.size());
        edges.push_back({to, from, (directed? 0 : capacity), -weight, 0});
    }

    void calcPhi() {
        phi.assign(phi.size(), INF);
        phi[source] = 0;
        bool changed;
        int step = 0;
        do {
            changed = false;
            for (const auto &i : edges) {
                if (i.resCapacity() > 0 && phi[i.from] != INF && phi[i.from] + i.weight < phi[i.to]) {
                    changed = true;
                    phi[i.to] = phi[i.from] + i.weight;
                }
            }
            ++step;
        } while (changed && step < 2 * phi.size());
        assert(!changed);
    }

    int getPhiWeight(const Edge &e) const {
        return e.weight + phi[e.from] - phi[e.to];
    }

    void recalcPhi() {
        std::vector<int> newphi(phi.size(), INF);
        newphi[source] = 0;
        std::priority_queue<std::pair<int, int>, class std::vector<std::pair<int, int>>, class std::greater<std::pair<int, int>>> qu;
        qu.push({0, source});
        while (!qu.empty()) {
            auto cur = qu.top();
            qu.pop();
            if (cur.first <= newphi[cur.second]) {
                newphi[cur.second] = cur.first;
                for (int e : nx[cur.second]) {
                    int dist = cur.first + getPhiWeight(edges[e]);
                    if (edges[e].resCapacity() > 0 && dist < newphi[edges[e].to]) {
                        newphi[edges[e].to] = dist;
                        qu.push({dist, edges[e].to});
                    }
                }
            }
        }
        for (int i = 0; i < phi.size(); ++i) {
            if (newphi[i] != INF) {
                newphi[i] += phi[i];
            }
        }
        swap(phi, newphi);
    }

    int currentFlow() const {
        int flow = 0;
        for (int i : nx[source]) {
            flow += edges[i].flow;
        }
        return flow;
    }

    int currentCost() const {
        int cost = 0;
        for (const auto &i : edges) {
            cost += i.flow * i.weight;
        }
        cost /= 2;
        return cost;
    }

    void findMinCostMaxFlow(int k = INF) {
        calcPhi();
        std::vector<bool> usd(phi.size());
        while (phi[target] != INF && k > 0) {
            usd.assign(usd.size(), 0);
            pushFlow(source, usd);
            recalcPhi();
            --k;
        }
    }

    std::vector<std::vector<int>> getFlowDecomposition() {
        int flow = currentFlow();
        std::vector<std::vector<int>> res(flow);
        for (int i = 0; i < flow; ++i) {
            int v = source;
            while (v != target) {
                for (int j : nx[v]) {
                    if (edges[j].flow > 0) {
                        addFlow(j, -1);
                        v = edges[j].to;
                        res[i].push_back(j / 2);
                        break;
                    }
                }
            }
        }
        return res;
    }

private:

    void addFlow(int e, int flow) {
        edges[e].flow += flow;
        edges[e ^ 1].flow -= flow;
    }

    bool pushFlow(int v, std::vector<bool> &usd) {
        if (v == target) return true;
        usd[v] = true;
        for (int i : nx[v]) {
            if (!usd[edges[i].to] && edges[i].resCapacity() > 0 && phi[edges[i].to] == phi[v] + edges[i].weight) {
                if (pushFlow(edges[i].to, usd)) {
                    addFlow(i, 1);
                    return true;
                }
            }
        }
        return false;
    }

    std::vector<Edge> edges;
    std::vector<std::vector<size_t>> nx;
    std::vector<int> phi;    
    int source, target;
};

int main(){
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    std::cout << std::fixed << std::setprecision(10);
    int n, m, k;
    std::cin >> n >> m >> k;
    Graph g(n);
    g.setTarget(n - 1);
    for (int i = 0; i < m; ++i) {
        int a, b, c;
        std::cin >> a >> b >> c;
        --a; --b;
        g.addEdge(a, b, 1, c);
        g.addEdge(b, a, 1, c);
    }
    g.findMinCostMaxFlow(k);
    if (g.currentFlow() != k) {
        std::cout << "-1" << std::endl;
    } else {
        std::cout << 1.0 * g.currentCost() / k << std::endl;
        auto d = g.getFlowDecomposition();
        for (int i = 0; i < d.size(); ++i) {
            std::cout << d[i].size() << ' ';
            for (int j : d[i]) std::cout << j / 2 + 1 << ' ';
            std::cout << '\n';
        }
    }
    return 0;
}
