#include <cstdlib>
#include <string>
#include "crow.h"
#include <cpr/cpr.h>

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() {
        return "Hello, Crow!";
    });

    CROW_ROUTE(app, "/movies").methods(crow::HTTPMethod::GET)([](const crow::request& req){
        // Get query parameter
        auto query_params = crow::query_string(req.url_params);
        if (!query_params.get("query")) {
            return crow::response(400, "Missing 'query' parameter");
        }

        std::string title = query_params.get("query");

        // Get Bearer Token from environment variable
        const char* bearer_token = std::getenv("TMDB_BEARER_TOKEN");
        if (!bearer_token) {
            return crow::response(500, "Bearer Token not set");
        }

        std::string bearer_token_str = bearer_token;

        // Construct TMDB API URL
        std::string tmdb_url = "https://api.themoviedb.org/3/search/movie?query=" + title;

        // Set the Authorization header with Bearer Token
        cpr::Header headers{{"Authorization", "Bearer " + bearer_token_str}, {"Accept", "application/json"}};

        // Make the API request using CPR with Bearer Token
        cpr::Response r = cpr::Get(cpr::Url{tmdb_url}, headers);

        // Check if the request was successful
        if (r.status_code != 200) {
            return crow::response(500, "Failed to fetch data from TMDB API");
        }

        // Return the raw JSON response to the client
        crow::response res(r.text); // Return raw JSON string from TMDB
        res.set_header("Content-Type", "application/json");
        return res;
    });

    app.port(8080).multithreaded().run();
}