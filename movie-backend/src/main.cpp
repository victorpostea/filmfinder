/**
 * @file main.cpp
 * @brief Main entry point for the Movie Finder backend API
 * @details Implements a REST API using Crow framework that interfaces with TMDB API
 *          to provide movie search and discovery functionality.
 */

#include <cstdlib>
#include <string>
#include "crow.h"
#include <cpr/cpr.h>

/**
 * @brief Main entry point of the application
 * @return int Exit code (0 for success)
 * 
 * Sets up a Crow web application with the following endpoints:
 * - GET /: Root endpoint
 * - GET /movies: Search movies by title
 * - GET /discover: Discover movies by genres
 * - GET /discover/genre: Discover movies by specific genre with filtering options
 */
int main() {
    crow::SimpleApp app;

    // Root route with CORS header
    CROW_ROUTE(app, "/")([]() {
        crow::response res("Hello, Crow!");
        res.add_header("Access-Control-Allow-Origin", "*");
        return res;
    });

    /**
     * @brief OPTIONS handler for /movies endpoint
     * @details Handles CORS preflight requests for the movies endpoint
     */
    CROW_ROUTE(app, "/movies")
        .methods(crow::HTTPMethod::OPTIONS)
        ([](const crow::request&, crow::response& res) {
            res.add_header("Access-Control-Allow-Origin", "*");
            res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
            res.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
            res.code = 204; // No Content
            res.end();
        });

    /**
     * @brief GET handler for /movies endpoint
     * @details Searches for movies using the TMDB API based on a query parameter
     * @param req The incoming HTTP request containing the search query
     * @return crow::response JSON response containing movie search results
     */
    CROW_ROUTE(app, "/movies")
        .methods(crow::HTTPMethod::GET)
        ([](const crow::request& req) {
            auto query_params = crow::query_string(req.url_params);
            if (!query_params.get("query")) {
                crow::response res(400, "Missing 'query' parameter");
                res.add_header("Access-Control-Allow-Origin", "*");
                return res;
            }
            std::string title = query_params.get("query");

            const char* bearer_token = std::getenv("TMDB_BEARER_TOKEN");
            if (!bearer_token) {
                crow::response res(500, "Bearer Token not set");
                res.add_header("Access-Control-Allow-Origin", "*");
                return res;
            }
            std::string bearer_token_str = bearer_token;
            
            // Use CPR's Parameters to properly encode the query
            auto parameters = cpr::Parameters{{"query", title}};
            cpr::Header headers{{"Authorization", "Bearer " + bearer_token_str}, {"Accept", "application/json"}};
            
            cpr::Response r = cpr::Get(
                cpr::Url{"https://api.themoviedb.org/3/search/movie"},
                parameters,
                headers
            );
            
            if (r.status_code != 200) {
                crow::response res(500, "Failed to fetch data from TMDB API");
                res.add_header("Access-Control-Allow-Origin", "*");
                return res;
            }
            crow::response res(r.text);
            res.set_header("Content-Type", "application/json");
            res.add_header("Access-Control-Allow-Origin", "*");
            return res;
        });

    /**
     * @brief OPTIONS handler for /discover endpoint
     * @details Handles CORS preflight requests for the discover endpoint
     */
    CROW_ROUTE(app, "/discover")
        .methods(crow::HTTPMethod::OPTIONS)
        ([](const crow::request&, crow::response& res) {
            res.add_header("Access-Control-Allow-Origin", "*");
            res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
            res.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
            res.code = 204; // No Content
            res.end();
        });

    /**
     * @brief GET handler for /discover endpoint
     * @details Retrieves movie recommendations based on genre IDs
     * @param req The incoming HTTP request containing genre parameters
     * @return crow::response JSON response containing movie recommendations
     */
    CROW_ROUTE(app, "/discover")
        .methods(crow::HTTPMethod::GET)
        ([](const crow::request& req) {
            auto query_params = crow::query_string(req.url_params);
            if (!query_params.get("genres")) {
                crow::response res(400, "Missing 'genres' parameter");
                res.add_header("Access-Control-Allow-Origin", "*");
                return res;
            }
            std::string genres = query_params.get("genres");

            const char* bearer_token = std::getenv("TMDB_BEARER_TOKEN");
            if (!bearer_token) {
                crow::response res(500, "Bearer Token not set");
                res.add_header("Access-Control-Allow-Origin", "*");
                return res;
            }
            std::string bearer_token_str = bearer_token;
            // Construct the TMDB discover URL with genre filter and sort by popularity
            std::string tmdb_url = "https://api.themoviedb.org/3/discover/movie?with_genres=" 
                                   + genres + "&sort_by=popularity.desc";
            cpr::Header headers{{"Authorization", "Bearer " + bearer_token_str}, {"Accept", "application/json"}};
            cpr::Response r = cpr::Get(cpr::Url{tmdb_url}, headers);
            if (r.status_code != 200) {
                crow::response res(500, "Failed to fetch data from TMDB API");
                res.add_header("Access-Control-Allow-Origin", "*");
                return res;
            }
            crow::response res(r.text);
            res.set_header("Content-Type", "application/json");
            res.add_header("Access-Control-Allow-Origin", "*");
            return res;
        });

    /**
     * @brief Mapping of genre names to TMDB genre IDs
     * @details Used to convert human-readable genre names to TMDB's internal genre IDs
     */
    std::unordered_map<std::string, std::string> genreNameToId = {
        {"action", "28"},
        {"adventure", "12"},
        {"animation", "16"},
        {"comedy", "35"},
        {"crime", "80"},
        {"documentary", "99"},
        {"drama", "18"},
        {"family", "10751"},
        {"fantasy", "14"},
        {"history", "36"},
        {"horror", "27"},
        {"music", "10402"},
        {"mystery", "9648"},
        {"romance", "10749"},
        {"sci_fi", "878"},
        {"thriller", "53"},
        {"war", "10752"},
        {"western", "37"}
    };

    /**
     * @brief OPTIONS handler for /discover/genre endpoint
     * @details Handles CORS preflight requests for the genre-specific discover endpoint
     */
    CROW_ROUTE(app, "/discover/genre")
        .methods(crow::HTTPMethod::OPTIONS)
        ([](const crow::request&, crow::response& res) {
            res.add_header("Access-Control-Allow-Origin", "*");
            res.add_header("Access-Control-Allow-Methods", "GET, OPTIONS");
            res.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
            res.code = 204; // No Content
            res.end();
        });

    /**
     * @brief GET handler for /discover/genre endpoint
     * @details Retrieves movies filtered by genre name with optional parameters
     * @param req The incoming HTTP request containing genre name and optional filters
     * @return crow::response JSON response containing filtered movie results
     */
    CROW_ROUTE(app, "/discover/genre")
    .methods(crow::HTTPMethod::GET)
    ([&genreNameToId](const crow::request& req) {
        auto query_params = crow::query_string(req.url_params);

        // Require a genre name
        if (!query_params.get("name")) {
            crow::response res(400, "Missing 'name' parameter");
            res.add_header("Access-Control-Allow-Origin", "*");
            return res;
        }
        std::string genreName = query_params.get("name");

        // Optional parameters
        std::string year = query_params.get("year") ? query_params.get("year") : "";
        std::string minRating = query_params.get("rating") ? query_params.get("rating") : "";
        std::string pageNumber = query_params.get("page") ? query_params.get("page") : "";
        std::string sortBy = query_params.get("sort_by") ? query_params.get("sort_by") : "";

        // Convert genre name to TMDB genre ID
        std::string genreId;
        if (genreNameToId.find(genreName) != genreNameToId.end()) {
            genreId = genreNameToId[genreName];
        } else {
            crow::response res(400, "Invalid genre name");
            res.add_header("Access-Control-Allow-Origin", "*");
            return res;
        }

        const char* bearer_token = std::getenv("TMDB_BEARER_TOKEN");
        if (!bearer_token) {
            crow::response res(500, "Bearer Token not set");
            res.add_header("Access-Control-Allow-Origin", "*");
            return res;
        }
        std::string bearer_token_str = bearer_token;

        // Start with genre filter
        std::string tmdb_url = "https://api.themoviedb.org/3/discover/movie?with_genres=" + genreId;

        // Use provided sort param or default
        if (!sortBy.empty()) {
            tmdb_url += "&sort_by=" + sortBy;
        } else {
            tmdb_url += "&sort_by=popularity.desc"; // fallback sort
        }

        tmdb_url += "&vote_count.gte=250";

        // If 'year' is supplied, set release year and append format to do from then onwards
        if (!year.empty()) {
            tmdb_url += "&primary_release_date.gte=" + year + "-01-01";
        }        

        // If 'rating' is supplied, set vote_average.gte
        if (!minRating.empty()) {
            tmdb_url += "&vote_average.gte=" + minRating;
        }

        // If 'page' is supplied, set page number
        if (!pageNumber.empty()) {
            tmdb_url += "&page=" + pageNumber;
        }

        cpr::Header headers{
            {"Authorization", "Bearer " + bearer_token_str},
            {"Accept", "application/json"}
        };

        // Make the request to TMDB
        cpr::Response r = cpr::Get(cpr::Url{tmdb_url}, headers);

        if (r.status_code != 200) {
            crow::response res(500, "Failed to fetch data from TMDB API");
            res.add_header("Access-Control-Allow-Origin", "*");
            return res;
        }

        crow::response res(r.text);
        res.set_header("Content-Type", "application/json");
        res.add_header("Access-Control-Allow-Origin", "*");
        return res;
    });

    app.port(8080).multithreaded().run();
}
