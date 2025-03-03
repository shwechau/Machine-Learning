// KmeansClustering.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <vector>
#include <random>
#include <iostream>
#include <limits>

struct Point {
    float x;
    float y;
    float z;
};

class Clustering {
public:
    Clustering(int clusters, int iterations, unsigned int seed = 0):num_clusters(clusters), max_iterations(iterations), gen(seed), dist(0.0f, 10.0f) {
        if (clusters <= 0 || iterations <= 0) {
            throw std::invalid_argument("Number of clusters and iterations must be positive.");
        }
    }
    std::vector<Point> Cluster(const std::vector<Point>& points, std::vector<int>& labels) {
        // Initialize centroids
        std::vector<Point> centroids(num_clusters);
        randomizeCentroids(centroids);
        labels.resize(points.size());
        std::vector<Point> sum_points(num_clusters);
        std::vector<int> count_points(num_clusters);

        for (int iteration = 0; iteration < max_iterations; ++iteration) {
            for (int i = 0; i < num_clusters; ++i) {
                sum_points[i] = { 0, 0, 0 };
                count_points[i] = 0;
            }

            // Assignment step
            for (size_t i = 0; i < points.size(); ++i) {
                int cluster_index = findClosestCentroid(points[i], centroids);
                labels[i] = cluster_index;
                sum_points[cluster_index].x += points[i].x;
                sum_points[cluster_index].y += points[i].y;
                sum_points[cluster_index].z += points[i].z;
                ++count_points[cluster_index];
            }

            // Update step
            for (int i = 0; i < num_clusters; ++i) {
                if (count_points[i] != 0) {
                    centroids[i].x = sum_points[i].x / count_points[i];
                    centroids[i].y = sum_points[i].y / count_points[i];
                    centroids[i].z = sum_points[i].z / count_points[i];
                }
            }
        }
        return centroids;
    }

private:
    int num_clusters;
    int max_iterations;
    std::default_random_engine gen;
    std::normal_distribution<float> dist;
    void randomizeCentroids(std::vector<Point>& centroids) {
        for (auto& centroid : centroids) {
            centroid.x = dist(gen);
            centroid.y = dist(gen);
            centroid.z = dist(gen);
        }
    }
    int findClosestCentroid(const Point& point, const std::vector<Point>& centroids) {
        int index = 0;
        float min_distance = std::numeric_limits<float>::max();
        for (int i = 0; i < num_clusters; ++i) {
            float dx = point.x - centroids[i].x;
            float dy = point.y - centroids[i].y;
            float dz = point.z - centroids[i].z;
            float distance = dx * dx + dy * dy + dz * dz;
            if (distance < min_distance) {
                min_distance = distance;
                index = i;
            }
        }
        return index;
    }
};

int main() {

    Clustering c(3, 10);
    std::vector<int> labels;
    int test_number = 0;
    // Sample input 0: 10 points clustered around (2,2,2), (5,5,5), and (8,8,8)
    std::vector<Point> sample_in_0 = {
        {1.9, 2.1, 2.2}, {2.0, 1.8, 2.1}, {2.2, 2.0, 1.9}, {5.1, 5.2, 5.0}, {4.9, 5.0, 5.2},
        {5.2, 4.8, 5.1}, {8.1, 7.9, 8.0}, {8.0, 8.2, 7.8}, {7.8, 8.1, 8.2}, {2.1, 2.2, 1.8}
    };

    // Sample input 1: 10 points clustered around (10,10,10), (15,15,15), and (20,20,20)
    std::vector<Point> sample_in_1 = {
        {9.8, 10.2, 10.1}, {10.1, 9.9, 10.2}, {10.0, 10.1, 9.8}, {15.2, 14.8, 15.1}, {14.9, 15.1, 15.0},
        {15.0, 14.9, 15.2}, {20.2, 19.8, 20.1}, {19.9, 20.2, 19.8}, {20.0, 19.9, 20.2}, {10.2, 9.8, 10.0}
    };
  
    if (test_number == 0) {
        auto centroids = c.Cluster(sample_in_0, labels);
    }
    else if (test_number == 1) {
        auto centroids = c.Cluster(sample_in_1, labels);
    }
    // Print labels
    for (auto l : labels) {

        std::cout << l << std::endl;

    }

}﻿
