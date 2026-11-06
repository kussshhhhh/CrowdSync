# CrowdSync 🌍⚡

![FastAPI](https://img.shields.io/badge/FastAPI-005571?style=flat&logo=fastapi)
![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=c%2B%2B)
![Docker](https://img.shields.io/badge/Docker-2496ED?style=flat&logo=docker)

A cloud-native, 2-tier microservice architecture designed for ultra-low latency geospatial matching. CrowdSync dynamically pairs users based on shared profile interests and real-time geographic proximity. 

**[🚀 Test the Live API (Interactive Swagger Docs)](https://crowdsync-api.onrender.com/docs#/default/create_user_users__post)** 
*(Note: Hosted on Render's free tier. The first request may take ~50 seconds to wake the server.)*

---

## 🏗️ Architecture

CrowdSync is engineered to prevent the API gateway from bottlenecking under heavy mathematical loads by splitting the logic into two specialized containers:

1. **Routing Layer (Python / FastAPI):** Acts as the primary client gateway. Manages in-memory user states and handles O(N) string-matching logic to filter users by intersecting interests. 
2. **Compute Engine (C++ / httplib):** A dedicated, high-performance geospatial microservice. It receives filtered user subsets from the Python gateway and calculates precise 2D proximity using the Haversine formula, returning valid matches in milliseconds.

## ⚙️ Features
- **Interest-Based Filtering:** Rapidly computes set intersections to find peers with shared metadata.
- **Geospatial Offloading:** Heavy distance calculations are isolated in a compiled C++ environment for maximum CPU efficiency.
- **Fully Containerized:** Infrastructure is defined via Docker Compose for seamless local testing and deployment.

---

## 🚀 Local Development (Docker)

To run this backend locally, you only need Docker installed on your machine.

**1. Clone the repository**
```bash
