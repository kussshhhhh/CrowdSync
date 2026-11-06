#include "httplib.h"
#include <nlohmann/json.hpp>
#include <cmath>
#include <vector>

using json = nlohmann::json;
constexpr double EARTH_RADIUS_KM = 6371.0;

// Haversine formula for geospatial distance
double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;
    lat1 = lat1 * M_PI / 180.0;
    lat2 = lat2 * M_PI / 180.0;

    double a = pow(sin(dLat / 2), 2) + pow(sin(dLon / 2), 2) * cos(lat1) * cos(lat2);
    double c = 2 * asin(sqrt(a));
    return EARTH_RADIUS_KM * c;
}

int main() {
    httplib::Server svr;

    svr.Post("/match", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto body = json::parse(req.body);
            double t_lat = body["target"]["lat"];
            double t_lon = body["target"]["lon"];
            double radius = body["radius"];
            
            json matched_users = json::array();

            for (const auto& user : body["users"]) {
                double dist = calculateDistance(t_lat, t_lon, user["lat"], user["lon"]);
                if (dist <= radius) {
                    json match = user;
                    match["distance_km"] = dist;
                    matched_users.push_back(match);
                }
            }

            json response;
            response["matches"] = matched_users;
            res.set_content(response.dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 400;
            res.set_content(R"({"error": "Invalid payload"})", "application/json");
        }
    });

    svr.listen("0.0.0.0", 8080);
    return 0;
}