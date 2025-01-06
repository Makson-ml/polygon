#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <limits>
#include <cmath>
#include <queue>


struct Edge {
    double lon;
    double lat;
    double weight;
};

struct Node {
    double lon;
    double lat;
    std::vector<Edge> edges;
};

struct Distance {
    int nodeIndex;
    double distance;

    bool operator>(const Distance& other) const {
        return distance > other.distance; // Для min-очереди
    }
};

std::vector<Node> parseDataFromFile(const std::string& filename) {
    std::vector<Node> nodes;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return nodes;
    }

    auto addNode = [&nodes](double lon, double lat) {
        Node node{ lon, lat };
        nodes.push_back(node);
        return nodes.size() - 1; // Возвращаем индекс нового узла
        };

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string nodePart, edgesPart;
        std::getline(ss, nodePart, ':');

        std::stringstream nodeStream(nodePart);
        double lon, lat;
        char comma;

        if (!(nodeStream >> lon >> comma >> lat) || comma != ',') {
            std::cerr << "Invalid node coordinates in line: " << line << std::endl;
            continue;
        }

        int currentNodeIndex = addNode(lon, lat);
        std::getline(ss, edgesPart);
        std::stringstream edgesStream(edgesPart);
        std::string edgeStr;

        while (std::getline(edgesStream, edgeStr, ';')) {
            std::stringstream edgeStream(edgeStr);
            double edgeLon, edgeLat, weight;
            char edgeComma1, edgeComma2;

            if (!(edgeStream >> edgeLon >> edgeComma1 >> edgeLat >> edgeComma2 >> weight) ||
                (edgeComma1 != ',' || edgeComma2 != ',')) {
                std::cerr << "Invalid edge data in edge: " << edgeStr << std::endl;
                continue;
            }

            // Проверяем, существует ли целевой узел
            auto it = std::find_if(nodes.begin(), nodes.end(), [&](const Node& n) {
                return n.lon == edgeLon && n.lat == edgeLat;
                });

            int targetNodeIndex = (it != nodes.end()) ? std::distance(nodes.begin(), it) : addNode(edgeLon, edgeLat);
            nodes[currentNodeIndex].edges.push_back({ edgeLon, edgeLat, weight }); // Добавляем ребро
        }
    }

    return nodes;
}




int findNodeIndex(double lon, double lat, const std::vector<Node>& nodes) {
    for (int i = 0; i < nodes.size(); ++i) {
        if (nodes[i].lon == lon && nodes[i].lat == lat) {
            return i;
        }
    }
    return -1;
}


void dfs(const std::vector<Node>& nodes, int currentNodeIndex, int targetNodeIndex, double currentWeight, double& minWeight, std::unordered_set<int>& visited) {
    if (visited.find(currentNodeIndex) != visited.end()) {
        return;
    }

    if (currentNodeIndex == targetNodeIndex) {
        minWeight = std::min(minWeight, currentWeight);
        return;
    }

    visited.insert(currentNodeIndex);

    for (const Edge& edge : nodes[currentNodeIndex].edges) {
        for (int i = 0; i < nodes.size(); ++i) {
            if (nodes[i].lon == edge.lon && nodes[i].lat == edge.lat) {
                dfs(nodes, i, targetNodeIndex, currentWeight + edge.weight, minWeight, visited);
                break;
            }
        }
    }

    visited.erase(currentNodeIndex);
}


void bfs(const std::vector<Node>& nodes, int currentNodeIndex, int targetNodeIndex, double& minWeight, std::unordered_set<int> visited) {
    std::queue<std::pair<int, double>> queue; // Храним пару: индекс узла и текущий вес
    queue.push({ currentNodeIndex, 0.0 });
    visited.insert(currentNodeIndex);

    // Устанавливаем начальное значение minWeight в максимально возможное
    minWeight = std::numeric_limits<double>::infinity();

    while (!queue.empty()) {
        auto front = queue.front();
        int nodeIndex = front.first;
        double weight = front.second;
        queue.pop();

        // Если достигли целевого узла, обновляем minWeight
        if (nodeIndex == targetNodeIndex) {
            minWeight = std::min(minWeight, weight);
            break;
        }

        // Обходим все смежные ребра текущего узла
        for (const auto& edge : nodes[nodeIndex].edges) {
            int nextNodeIndex = findNodeIndex(edge.lon, edge.lat, nodes);
            double edgeWeight = edge.weight;
            if (visited.find(nextNodeIndex) == visited.end()) {
                // Добавляем соседний узел в очередь и отмечаем его как посещенный
                queue.push({ nextNodeIndex, weight + edgeWeight });
                visited.insert(nextNodeIndex);
            }
        }
    }
}


std::vector<double> dijkstra(int startNodeIndex, const std::vector<Node>& nodes, std::unordered_set<int> visited) {
    std::vector<double> distances(nodes.size(), std::numeric_limits<double>::infinity());
    distances[startNodeIndex] = 0;

    std::priority_queue<Distance, std::vector<Distance>, std::greater<Distance>> queue;
    queue.push({ startNodeIndex, 0 });
    while (!queue.empty()) {
        int currentNodeIndex = queue.top().nodeIndex;
        double currentDistance = queue.top().distance;
        queue.pop();

        // Если узел уже посещён, пропускаем его
        if (visited.find(currentNodeIndex) != visited.end()) {
            continue;
        }

        // Добавляем узел к посещённым
        visited.insert(currentNodeIndex);

        // Проходим все ребра текущего узла
        for (const auto& edge : nodes[currentNodeIndex].edges) {
            double newDistance = currentDistance + edge.weight;
            int targetIndex = findNodeIndex(edge.lon, edge.lat, nodes);

            // Если найден более короткий путь до смежного узла
            if (newDistance < distances[targetIndex]) {
                distances[targetIndex] = newDistance;
                queue.push({ targetIndex, newDistance });
            }
        }
    }

    return distances; // Возвращаем массив расстояний
}


int main() {
    const std::string filename = "graph.txt";
    std::vector<Node> nodes = parseDataFromFile(filename);

    double startLon = 30.4141326;
    double startLat = 59.9470649;
    double targetLon = 30.4140936;
    double targetLat = 59.9469059;

    int startNodeIndex = findNodeIndex(startLon, startLat, nodes);
    int targetNodeIndex = findNodeIndex(targetLon, targetLat, nodes);

    if (startNodeIndex == -1 || targetNodeIndex == -1) {
        std::cout << "Invalid start or target node." << std::endl;
        return 1;
    }

    double minWeight = std::numeric_limits<double>::infinity();
    std::unordered_set<int> visited;

    dfs(nodes, startNodeIndex, targetNodeIndex, 0.0, minWeight, visited);
    if (minWeight < std::numeric_limits<double>::infinity()) {
        std::cout << "minWeight: " << minWeight << std::endl;
    }
    else {
        std::cout << "None." << std::endl;
    }

    bfs(nodes, startNodeIndex, targetNodeIndex, minWeight, visited);
    if (minWeight < std::numeric_limits<double>::infinity()) {
        std::cout << "minWeight: " << minWeight << std::endl;
    }
    else {
        std::cout << "None." << std::endl;
    }

    std::vector<double> distances = dijkstra(startNodeIndex, nodes, visited);
    if (distances[targetNodeIndex] < std::numeric_limits<double>::infinity()) {
        std::cout << "minWeight: " << minWeight << std::endl;
    }
    else {
        std::cout << "None." << std::endl;
    }

    return 0;
}